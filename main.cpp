#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QProcess>
#include "pythonhandler.h"
#include "networkmanager.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QString exePath = "D:/Afoot/SDK/LightVista/LightVista.exe";

    // QProcess process;
    // process.start(exePath, QStringList());

    // 创建并初始化 PythonHandler 实例
    PythonHandler *pyHandler = new PythonHandler;
    pyHandler->initialize();

    NetworkManager *networkManager = new NetworkManager;

    int ch_num;
    QList<int> ch_no;
    PyObject *u = nullptr;
    if (pyHandler->deviceInitFunction(exePath, ch_num, ch_no, u)) {
        qDebug() << "通道数 =" << ch_num;
        qDebug() << "通道列表: " << ch_no;
        pyHandler->startDataThread(ch_no, u);
    } else {
        qDebug() << "device_init 调用失败";
    }

    // 启动 QML 引擎，并传递 PythonHandler 实例
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("pyHandler", pyHandler);
    engine.rootContext()->setContextProperty("networkManager", networkManager);
    engine.load(QUrl("qrc:/main.qml"));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
