#include "client.h"
#include <QDebug>
#include <QCoreApplication>
Client::Client(QObject *parent)
    : QObject(parent), tcpSocket(new QTcpSocket(this))
{
}

Client::~Client()
{
    tcpSocket->disconnectFromHost();
}

void Client::connectToServer()
{
    connect(tcpSocket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &Client::onDisconnected);

    tcpSocket->connectToHost("127.0.0.1", 1234);  // Connect to the server (localhost)
}

void Client::sendMessage(const QString &message)
{
    if (tcpSocket->state() == QTcpSocket::ConnectedState) {
        tcpSocket->write(message.toUtf8());
    }
}

void Client::sendJoinCommand(const QString &playerName)
{
    QString command = "JOIN:" + playerName;
    sendMessage(command); // Send JOIN command to the server
}


void Client::onConnected()
{
    qDebug() << "Connected to server!";
    sendMessage("Hello from client!");
}

void Client::sendReadySignal()
{
    sendMessage("Ready!");
}


void Client::onReadyRead()
{
    QByteArray data = tcpSocket->readAll();
    qDebug() << "Received from server:" << data;

    QString message = QString::fromUtf8(data).trimmed();

    if (message == "AllReady") {
        emit allPlayersReady(); }
    else if (message == "Server full. Connection rejected.") {
        qDebug() << "Server is full. Closing the client...";

        emit serverFullSignal();  // Emit the signal to close the window
        QCoreApplication::quit();  // Optionally, close the application
    }
}

void Client::onDisconnected()
{
    qDebug() << "Disconnected from server!";
}
