#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QProcess>

class NetworkManager : public QObject {
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);

public slots:
    void connectToWifi(const QString &ssid, const QString &password);
    void connectToTcp(const QString &ip, int port);

signals:
    void wifiConnectionStatus(bool success, const QString &message);
    void tcpConnectionStatus(bool success, const QString &message);
    void tcpDataReceived(const QString &data);

private slots:
    void onTcpConnected();
    void onTcpDisconnected();
    void onTcpError();
    void onTcpDataReady();

private:
    QTcpSocket *tcpSocket;
};

#endif // NETWORKMANAGER_H
