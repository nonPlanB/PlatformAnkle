#ifndef PYTHONHANDLER_H
#define PYTHONHANDLER_H

// 避免 Qt 和 Python 宏冲突
#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")
#include <QObject>
#include <QDebug>
#include <QVector>
#include <QThread>

class PythonHandler : public QObject
{
    Q_OBJECT
public:
    explicit PythonHandler(QObject *parent = nullptr);

    void initialize();
    bool deviceInitFunction(const QString &exePath, int &ch_num, QList<int> &ch_no, PyObject *&u);
    QList<QVector<double>> recvDataFunction(PyObject *u, const QList<int> &ch_no);
    void startDataThread(const QList<int> &ch_no, PyObject *u);

signals:
    void emgValueReceived(int channel, double value);

private:
    bool m_initialized = false;
    QThread *m_thread = nullptr;

};

#endif // PYTHONHANDLER_H
