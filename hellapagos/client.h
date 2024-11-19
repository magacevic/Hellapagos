#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QString>  // Include QString to handle names

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);
    ~Client();



    void connectToServer();
    void sendJoinCommand(const QString &playerName); // New function
    void sendReadySignal();
    // void sendMessage(const QString &message);
    // In client.h

signals:
    void serverFullSignal();  // Signal to notify the server is full
    void allPlayersReady();

private slots:
    void onConnected();
    void onReadyRead();
    void onDisconnected();


private:
    QTcpSocket *tcpSocket;
    void sendMessage(const QString &message);

};

#endif // CLIENT_H
