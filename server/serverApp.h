#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    ~Server();

    void startServer();

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();

private:
    QTcpServer *tcpServer;
    QList<QTcpSocket*> clientSockets;     // List of connected clients

    void broadcastMessage(const QString& message);
    int readyPlayers = 0;
};

#endif // SERVER_H
