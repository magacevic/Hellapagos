#include "serverApp.h"
#include <QDebug>
#include "gameserver.h"

Server::Server(QObject *parent)
    : QObject(parent), tcpServer(new QTcpServer(this)), gameServer(new GameServer(this))
{
    connect(gameServer, &GameServer::weatherCardChanged, this, &Server::onWeatherCardChanged);
    //connect(gameServer, &GameServer::weatherCardChanged, this, &Server::onWeatherCardChanged);
    connect(gameServer, &GameServer::currentAndNextPlayerUpdated, this, &Server::onCurrentAndNextPlayerUpdated);
    

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
/*
void Server::onWeatherCardChanged(int newIndex)
{
    broadcastMessage(QString("WeatherCard:%1").arg(newIndex));
}*/

void Server::onNewConnection()
{
    if (clientSockets.size() >= 2) {
        QTcpSocket* extraClient = tcpServer->nextPendingConnection();
        qDebug() << "Rejected connection: Maximum players reached!";
        extraClient->write("Server full. Connection rejected.");
        extraClient->flush();
        extraClient->disconnectFromHost();
        extraClient->deleteLater();
        qDebug() << "Rejected new connection: server is full.";
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

    if (message.startsWith("WEATHER_CARD:")) {
        int index = message.split(":").last().toInt();
        broadcastWeatherCardChange(index);
    }

    if (message == "REQUEST_WEATHER_CARD") {
        // Pass the request to the GameServer to handle the logic
        gameServer->onWeatherCardRequest();
    }

    if (message.startsWith("JOIN:")) {
        QString playerName = message.section("JOIN:", 1, 1).trimmed();
        qDebug() << "Hi," << playerName;

        gameServer->addPlayer(playerName);
        qDebug() << "Current players:" << gameServer->getPlayers();

        senderSocket->write(("Hello, " + playerName + "! Welcome to the server!").toUtf8());
    }else if (message == "Ready!") {
        readyPlayers++;
        qDebug() << "Player is ready! Total ready players:" << readyPlayers;

        if (readyPlayers == clientSockets.size()) {  // All players are ready
            qDebug() << "All players are ready. Starting the game!";
            broadcastMessage("AllReady");
            
            gameServer->getRoundManager()->nextTurn();  // Pozovi početak prve runde
            qDebug() << "First turn started";
        }
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
void Server::onWeatherCardChanged(int newIndex) {
    broadcastWeatherCardChange(newIndex);
}

void Server::onCurrentAndNextPlayerUpdated(const QString& currentPlayer, const QString& nextPlayer)
{
    // Obaveštavamo sve klijente o trenutnom i narednom igraču
    QString message = QString("Current Player: %1, Next Player: %2").arg(currentPlayer).arg(nextPlayer);
    broadcastMessage(message);
    
    qDebug() << "Current Player:" << currentPlayer << ", Next Player:" << nextPlayer;
}

void Server::broadcastWeatherCardChange(int index) {
    QString message = QString("WEATHER_CARD:%1").arg(index);
   /* for (QTcpSocket* client : clientSockets) {
        client->write(message.toUtf8());
    }*/
    for (QTcpSocket *clientSocket : clientSockets) {
        if (clientSocket->isOpen()) {
            clientSocket->write(message.toUtf8());
        }
    }
}

