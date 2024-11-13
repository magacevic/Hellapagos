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

   // void sendMessage(const QString &message);

private slots:
    void onConnected();
    void onReadyRead();
    void onDisconnected();

private:
    QTcpSocket *tcpSocket;
    void sendMessage(const QString &message);

};

#endif // CLIENT_H
