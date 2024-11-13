#include "serverApp.h"
#include <QDebug>

Server::Server(QObject *parent)
    : QObject(parent), tcpServer(new QTcpServer(this)), clientSocket(nullptr)
{
}

Server::~Server()
{
    if (clientSocket) {
        clientSocket->disconnectFromHost();
    }
}

void Server::startServer()
{
    if (tcpServer->listen(QHostAddress::Any, 1234)) {
        qDebug() << "Server started on port 1234";
        connect(tcpServer, &QTcpServer::newConnection, this, &Server::onNewConnection);
    } else {
        qDebug() << "Server could not start!";
    }
}

void Server::onNewConnection()
{
    clientSocket = tcpServer->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::onDisconnected);

    qDebug() << "New client connected!";
}

void Server::onReadyRead()
{
    QByteArray data = clientSocket->readAll();
    QString message = QString::fromUtf8(data).trimmed();
    qDebug() << "Received data:" << message;

    // Check if the message starts with "JOIN:"
    if (message.startsWith("JOIN:")) {
        // Extract the player's name after "JOIN:"
        QString playerName = message.section("JOIN:", 1, 1).trimmed();
        qDebug() << "Hi," << playerName;  // Greet the player by name

        // Optionally, send a response to the client
        QString response = "Hello, " + playerName + "! Welcome to the server!";
        clientSocket->write(response.toUtf8());
    }
}

void Server::onDisconnected()
{
    qDebug() << "Client disconnected!";
    clientSocket->deleteLater();
    clientSocket = nullptr;
}
