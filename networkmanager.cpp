#include "networkmanager.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QNetworkProxy>
#include <QtConcurrent>

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

    // 异步线程5s内检查连接状态
    QtConcurrent::run([=](){
        for (int i = 0; i < 5; ++i) {
            QProcess checkProcess;
            checkProcess.start("netsh", QStringList() << "wlan" << "show" << "interfaces");
            checkProcess.waitForFinished();
            //转化为GBK编码
            QString output = QString::fromLocal8Bit(checkProcess.readAllStandardOutput());
            qDebug() << "Netsh output:" << output;
            if (output.contains(ssid)) {
                emit wifiConnectionStatus(true, QString("Connected to %1").arg(ssid));
            } else {
                emit wifiConnectionStatus(false, "Wi-Fi connection failed");
            }
            QThread::msleep(1000);
        }
    });
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
}

void NetworkManager::onTcpDisconnected() {
    emit tcpConnectionStatus(false, "Disconnected from TCP server");
}

void NetworkManager::onTcpError() {
    emit tcpConnectionStatus(false, QString("TCP error: %1").arg(tcpSocket->errorString()));
}

void NetworkManager::onTcpDataReady() {
    QByteArray data = tcpSocket->readAll();
    if (data.size() >= 10 && data[0] == static_cast<char>(0xFE) && data[1] == static_cast<char>(0xFD)) {
        int pxAngle = (static_cast<unsigned char>(data[2]) << 8) + static_cast<unsigned char>(data[3]);
        int pyAngle = (static_cast<unsigned char>(data[4]) << 8) + static_cast<unsigned char>(data[5]);
        int pzAngle = (static_cast<unsigned char>(data[6]) << 8) + static_cast<unsigned char>(data[7]);
        if (pxAngle > 0x8000) pxAngle -= 0x10000;
        if (pyAngle > 0x8000) pyAngle -= 0x10000;
        if (pzAngle > 0x8000) pzAngle -= 0x10000;
        pxAngle = pxAngle / 100;
        pyAngle = pyAngle / 100;
        pzAngle = pzAngle / 100;
        emit controlDataReceived(pxAngle, pyAngle, pzAngle);
    }
    emit tcpDataReceived(data.toHex(' '));
}

void NetworkManager::sendControlData(int x, int y, int z, int speed, int mode) {
    if (tcpSocket->state() != QAbstractSocket::ConnectedState) {
        emit tcpConnectionStatus(false, "Not connected to TCP server");
        return;
    }
    int xAngle = x * 100;
    int yAngle = y * 100;
    int zAngle = z * 100;
    QByteArray msg;
    msg.append(static_cast<char>(0xFE));
    msg.append(static_cast<char>(0xFD));
    msg.append(static_cast<char>((xAngle & 0xFF00) >> 8));
    msg.append(static_cast<char>(xAngle & 0x00FF));
    msg.append(static_cast<char>((yAngle & 0xFF00) >> 8));
    msg.append(static_cast<char>(yAngle & 0x00FF));
    msg.append(static_cast<char>((zAngle & 0xFF00) >> 8));
    msg.append(static_cast<char>(zAngle & 0x00FF));
    msg.append(static_cast<char>(speed & 0xFF));
    msg.append(static_cast<char>(mode & 0xFF));
    msg.append(static_cast<char>(0xFD));
    msg.append(static_cast<char>(0xFE));
    tcpSocket->write(msg);
}
