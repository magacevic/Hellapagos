#include "serverApp.h"
#include <QDebug>

Server::Server(QObject *parent)
    : QObject(parent), tcpServer(new QTcpServer(this))
{
}

Server::~Server()
{

    for (QTcpSocket* socket : clientSockets) {
        socket->disconnectFromHost();
        socket->deleteLater();
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
    if (clientSockets.size() >= 2) {
        QTcpSocket* extraClient = tcpServer->nextPendingConnection();
        qDebug() << "Rejected connection: Maximum players reached!";
        extraClient->write("Server full. Connection rejected.");
        extraClient->flush();
        extraClient->disconnectFromHost();
        extraClient->deleteLater();
        return;
    }

    QTcpSocket* newClient = tcpServer->nextPendingConnection();
    clientSockets.append(newClient);
    connect(newClient, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    connect(newClient, &QTcpSocket::disconnected, this, &Server::onDisconnected);

    qDebug() << "New client connected! Total players:" << clientSockets.size();

    if (clientSockets.size() == 2) {
        qDebug() << "Game ready to start!";
        broadcastMessage("All players connected. Game starting...");
    }
}

void Server::onReadyRead()
{
    QTcpSocket* senderSocket = qobject_cast<QTcpSocket*>(sender());
    if (!senderSocket) return;

    QByteArray data = senderSocket->readAll();
    QString message = QString::fromUtf8(data).trimmed();
    qDebug() << "Received data:" << message;

    if (message.startsWith("JOIN:")) {
        QString playerName = message.section("JOIN:", 1, 1).trimmed();
        qDebug() << "Hi," << playerName;

        senderSocket->write(("Hello, " + playerName + "! Welcome to the server!").toUtf8());
    }
}

void Server::onDisconnected()
{
    QTcpSocket* senderSocket = qobject_cast<QTcpSocket*>(sender());
    if (!senderSocket) return;

    qDebug() << "Client disconnected!";
    clientSockets.removeAll(senderSocket);
    senderSocket->deleteLater();

    qDebug() << "Total players now:" << clientSockets.size();

    }

void Server::broadcastMessage(const QString& message)
{
    QByteArray data = message.toUtf8();
    for (QTcpSocket* socket : clientSockets) {
        socket->write(data);
    }
}
