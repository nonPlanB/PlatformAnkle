#include "networkmanager.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QNetworkProxy>

NetworkManager::NetworkManager(QObject *parent) : QObject(parent) {
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, &QTcpSocket::connected, this, &NetworkManager::onTcpConnected);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &NetworkManager::onTcpDisconnected);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &NetworkManager::onTcpDataReady);
    connect(tcpSocket, &QTcpSocket::errorOccurred, this, &NetworkManager::onTcpError);
}

void NetworkManager::connectToWifi(const QString &ssid, const QString &password) {
    // 创建 Wi-Fi 配置文件
    QString profile = QString(
                          "<?xml version=\"1.0\"?>\n"
                          "<WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\">\n"
                          "    <name>%1</name>\n"
                          "    <SSIDConfig>\n"
                          "        <SSID>\n"
                          "            <name>%1</name>\n"
                          "        </SSID>\n"
                          "    </SSIDConfig>\n"
                          "    <connectionType>ESS</connectionType>\n"
                          "    <connectionMode>auto</connectionMode>\n"
                          "    <MSM>\n"
                          "        <security>\n"
                          "            <authEncryption>\n"
                          "                <authentication>WPA2PSK</authentication>\n"
                          "                <encryption>AES</encryption>\n"
                          "                <useOneX>false</useOneX>\n"
                          "            </authEncryption>\n"
                          "            <sharedKey>\n"
                          "                <keyType>passPhrase</keyType>\n"
                          "                <protected>false</protected>\n"
                          "                <keyMaterial>%2</keyMaterial>\n"
                          "            </sharedKey>\n"
                          "        </security>\n"
                          "    </MSM>\n"
                          "</WLANProfile>").arg(ssid, password);

    // 保存配置文件到临时文件
    QFile file("wifi_profile.xml");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << profile;
        file.close();
    } else {
        emit wifiConnectionStatus(false, "Failed to create Wi-Fi profile");
        return;
    }

    // 使用 netsh 命令添加并连接 Wi-Fi
    QProcess process;
    process.start("netsh", QStringList() << "wlan" << "add" << "profile" << "filename=wifi_profile.xml");
    process.waitForFinished();
    if (process.exitCode() != 0) {
        emit wifiConnectionStatus(false, "Failed to add Wi-Fi profile");
        return;
    }
    process.start("netsh", QStringList() << "wlan" << "connect" << QString("name=%1").arg(ssid));
    process.waitForFinished();
    if (process.exitCode() != 0) {
        emit wifiConnectionStatus(false, "Failed to connect to Wi-Fi");
        return;
    }

    // 检查连接状态
    QProcess checkProcess;
    checkProcess.start("netsh", QStringList() << "wlan" << "show" << "interfaces");
    checkProcess.waitForFinished();
    QString output = checkProcess.readAllStandardOutput();
    if (output.contains(ssid)) {
        emit wifiConnectionStatus(true, QString("Connected to %1").arg(ssid));
    } else {
        emit wifiConnectionStatus(false, "Wi-Fi connection failed");
    }
}

void NetworkManager::connectToTcp(const QString &ip, int port) {
    if (tcpSocket->state() == QAbstractSocket::ConnectedState) {
        tcpSocket->disconnectFromHost();
    }
    // 禁用代理
    tcpSocket->setProxy(QNetworkProxy::NoProxy);
    tcpSocket->connectToHost(ip, port);
}

void NetworkManager::onTcpConnected() {
    emit tcpConnectionStatus(true, "Connected to TCP server");
    // 示例：发送数据
    tcpSocket->write("Hello from PC!");
}

void NetworkManager::onTcpDisconnected() {
    emit tcpConnectionStatus(false, "Disconnected from TCP server");
}

void NetworkManager::onTcpError() {
    emit tcpConnectionStatus(false, QString("TCP error: %1").arg(tcpSocket->errorString()));
}

void NetworkManager::onTcpDataReady() {
    QString data = tcpSocket->readAll();
    emit tcpDataReceived(data);
}
