#include "pythonhandler.h"

PythonHandler::PythonHandler(QObject *parent)
    : QObject{parent}
{}

void PythonHandler::initialize()
{
    if (m_initialized) {
        qDebug() << "Python 已经初始化过，跳过";
        return;
    }
    constexpr wchar_t pythonHome[] = L"D:/Afoot/python311";
    // Python C API初始化Python解释器配置
    PyStatus status;
    PyConfig config;
    PyConfig_InitPythonConfig(&config);
    status = PyConfig_SetString(&config, &config.home, pythonHome);
    if (PyStatus_Exception(status)) {
        qCritical() << "设置Python Home失败";
        PyConfig_Clear(&config);
        return;
    }

    // 初始化Python解释器
    status = Py_InitializeFromConfig(&config);
    // 释放 config 结构体
    PyConfig_Clear(&config);
    if (PyStatus_Exception(status)) {
        qCritical() << "Python初始化失败";
        return;
    }

    // 设置模块路径
    QString sdkPath = "D:/Afoot/SDK";
    QString cmd = QString("import sys; sys.path.append(r'%1')").arg(sdkPath);
    int pyResult = PyRun_SimpleString(cmd.toUtf8().constData());
    if (pyResult != 0) {
        qCritical() << "添加sys.path失败";
    }
    m_initialized = true;
    qDebug() << "Python 初始化成功";
}

bool PythonHandler::deviceInitFunction(const QString &exePath, int &ch_num, QList<int> &ch_no, PyObject *&u)
{
    if (!m_initialized) {
        qCritical() << "Python未初始化";
        return false;
    }
    // 导入模块
    PyObject *pModule = PyImport_ImportModule("device_init");
    if (!pModule) {
        qCritical() << "导入device_init模块失败";
        PyErr_Print();
        return false;
    }
    // 获取函数
    PyObject *pFunc = PyObject_GetAttrString(pModule, "device_init");
    if (!pFunc || !PyCallable_Check(pFunc)) {
        qCritical() << "找不到函数 device_init";
        PyErr_Print();
        Py_DECREF(pModule);
        return false;
    }
    // 构造参数
    PyObject *pArgs = PyTuple_New(1);
    PyObject *pExePath = PyUnicode_FromString(exePath.toUtf8().constData());
    PyTuple_SetItem(pArgs, 0, pExePath);
    // 调用函数
    PyObject *pResult = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);
    Py_DECREF(pFunc);
    Py_DECREF(pModule);

    if (!pResult || !PyTuple_Check(pResult) || PyTuple_Size(pResult) != 3) {
        qCritical() << "device_init返回值异常";
        PyErr_Print();
        Py_XDECREF(pResult);
        return false;
    }
    // 解析返回值
    PyObject *pChNum = PyTuple_GetItem(pResult, 0);
    PyObject *pChNoList = PyTuple_GetItem(pResult, 1);
    PyObject *pU = PyTuple_GetItem(pResult, 2);
    ch_num = static_cast<int>(PyLong_AsLong(pChNum));
    if (!PyList_Check(pChNoList)) {
        qCritical() << "ch_no不是列表";
        Py_DECREF(pResult);
        return false;
    }
    ch_no.clear();
    for (Py_ssize_t i = 0; i < PyList_Size(pChNoList); ++i) {
        PyObject *item = PyList_GetItem(pChNoList, i);
        if (PyLong_Check(item)) {
            ch_no.append(PyLong_AsLong(item));
        }
    }
    u = pU;
    Py_INCREF(u);
    Py_DECREF(pResult);
    return true;
}

QList<QVector<double>> PythonHandler::recvDataFunction(PyObject *u, const QList<int> &ch_no)
{
    QList<QVector<double>> result;

    if (!u || !m_initialized) {
        qWarning() << "recvDataFunction: Python 未初始化或设备句柄无效";
        return result;
    }
    // 导入模块
    PyObject *pModule = PyImport_ImportModule("recv_data");
    if (!pModule) {
        qCritical() << "导入recv_data模块失败";
        PyErr_Print();
        return result;
    }
    // 获取函数
    PyObject *pFunc = PyObject_GetAttrString(pModule, "recv_data");
    if (!pFunc || !PyCallable_Check(pFunc)) {
        qCritical() << "找不到 recv_data 函数";
        PyErr_Print();
        Py_DECREF(pModule);
        return result;
    }
    // 构造参数
    PyObject *pArgs = PyTuple_New(2);
    Py_INCREF(u);
    PyTuple_SetItem(pArgs, 0, u);

    PyObject *pList = PyList_New(ch_no.size());
    for (int i = 0; i < ch_no.size(); ++i) {
        PyList_SetItem(pList, i, PyLong_FromLong(ch_no[i]));
    }
    PyTuple_SetItem(pArgs, 1, pList);

    // 调用recv_data函数
    PyObject *pReturn = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);
    Py_DECREF(pFunc);
    Py_DECREF(pModule);
    if (!pReturn || !PyList_Check(pReturn)) {
        qCritical() << "recv_data 返回值异常";
        PyErr_Print();
        Py_XDECREF(pReturn);
        return result;
    }
    // 转换结果：List[List[float or None]] → QList<QVector<double>>
    Py_ssize_t chCount = PyList_Size(pReturn);
    for (Py_ssize_t i = 0; i < chCount; ++i) {
        PyObject *chData = PyList_GetItem(pReturn, i);
        QVector<double> buffer;
        if (chData && PyList_Check(chData)) {
            for (Py_ssize_t j = 0; j < PyList_Size(chData); ++j) {
                PyObject *item = PyList_GetItem(chData, j);
                if (PyFloat_Check(item) || PyLong_Check(item)) {
                    buffer.append(PyFloat_AsDouble(item));
                }
            }
        }
        result.append(buffer);
    }
    Py_DECREF(pReturn);
    return result;
}

void PythonHandler::startDataThread(const QList<int> &ch_no, PyObject *u)
{
    if (m_thread) return;

    m_thread = QThread::create([=]() {
        while (true) {
            // 持续从 Python 获取数据
            QList<QVector<double>> newData = recvDataFunction(u, ch_no);

            // 对每个通道的数据，逐条打印
            for (int i = 0; i < newData.size(); ++i) {
                const QVector<double>& channelData = newData[i];
                if (!channelData.isEmpty()) {
                    double latest = channelData.last();
                    // qDebug() << QString("Ch%1: %2").arg(ch_no[i]).arg(latest);
                    emit emgValueReceived(ch_no[i], latest);
                }
            }
            QThread::msleep(20);
        }
    });

    connect(m_thread, &QThread::finished, m_thread, &QObject::deleteLater);
    m_thread->start();
}




