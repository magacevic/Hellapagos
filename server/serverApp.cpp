#include "serverApp.h"
#include <QDebug>
#include "gameserver.h"

Server::Server(QObject *parent)
    : QObject(parent), tcpServer(new QTcpServer(this)), gameServer(new GameServer(this))
{
    connect(gameServer, &GameServer::weatherCardChanged, this, &Server::onWeatherCardChanged);
    connect(gameServer, &GameServer::currentAndNextPlayerUpdated, this, &Server::onCurrentAndNextPlayerUpdated);
    connect(gameServer, &GameServer::roundStarted, this, &Server::onRoundStarted);
}

Server::~Server()
{
    // Properly disconnect and delete sockets when the server is destroyed
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
    // Reject connections if the maximum number of players is reached
    if (clientSockets.size() >= 3) {
        QTcpSocket* extraClient = tcpServer->nextPendingConnection();
        qDebug() << "Rejected connection: Maximum players reached!";
        extraClient->write("Server full. Connection rejected.");
        extraClient->flush();
        extraClient->disconnectFromHost();
        extraClient->deleteLater();
        return;
    }

    // Accept new connections
    QTcpSocket* newClient = tcpServer->nextPendingConnection();
    clientSockets.append(newClient);
    connect(newClient, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    connect(newClient, &QTcpSocket::disconnected, this, &Server::onDisconnected);

    qDebug() << "New client connected! Total players:" << clientSockets.size();

    if (clientSockets.size() == 3) {
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
        gameServer->onWeatherCardRequest();
    }

    if (message.startsWith("ACTION_COMPLETED:")) {
        gameServer->getRoundManager()->actionCompleted();

        if(message.startsWith("ACTION_COMPLETED:COLLECT_WATER")){
            QString numberString = message.split(' ').last();
            int collectedWater=numberString.toInt();
           // gameServer->resources["water"]+=collectedWater;
            gameServer->addResource("water", collectedWater);

           // qDebug() << "Water resource updated. Total water:" << gameServer->resources["water"];

        }
    }

    if (message.startsWith("REQUEST_ACTION:")) {
        QString action = message.split(":").last();
        QString currentPlayer = gameServer->getRoundManager()->currentPlayer();
        QTcpSocket* currentPlayerSocket = playerSocketMap.value(currentPlayer);

        if (senderSocket == currentPlayerSocket) {
            senderSocket->write(QString("ACTION_APPROVED:%1").arg(action).toUtf8());
        } else {
            senderSocket->write(QString("Sorry, not your turn. Player %1").arg(currentPlayer).toUtf8());
        }
    }

    if (message.startsWith("ACTION_COMPLETED:COLLECT_FOOD")) {
        QString playerName = message.split(":").at(1);
        int foodAmount = message.split(":").at(2).toInt();
        gameServer->collectFoodFromPlayer(playerName, foodAmount);
    }

    if (message.startsWith("JOIN:")) {
        QString playerName = message.section("JOIN:", 1, 1).trimmed();
        qDebug() << "Hi," << playerName;

        // Check if the name is already taken
        if (playerSocketMap.contains(playerName)) {
            senderSocket->write("Name already taken. Choose a different name.");
            return;
        }

        playerSocketMap[playerName] = senderSocket;
        gameServer->addPlayer(playerName);

        senderSocket->write(("Hello, " + playerName + "! Welcome to the server!").toUtf8());
    }
    else if (message == "Ready!") {
        readyPlayers++;
        qDebug() << "Player is ready! Total ready players:" << readyPlayers;

        if (readyPlayers == clientSockets.size()) {
            qDebug() << "All players are ready. Starting the game!";
            broadcastMessage("AllReady");
            gameServer->getRoundManager()->nextTurn();
            qDebug() << "First turn started";
        }
    }
}

void Server::onDisconnected()
{
    QTcpSocket* senderSocket = qobject_cast<QTcpSocket*>(sender());
    if (!senderSocket) return;

    // Properly disconnect and remove client from map
    QString playerName;
    for (auto it = playerSocketMap.begin(); it != playerSocketMap.end(); ++it) {
        if (it.value() == senderSocket) {
            playerName = it.key();
            playerSocketMap.erase(it);
            break;
        }
    }

    qDebug() << "Client disconnected!";
    clientSockets.removeAll(senderSocket);
    senderSocket->deleteLater();
}

void Server::broadcastMessage(const QString& message)
{
    QByteArray data = message.toUtf8();
    for (QTcpSocket* socket : clientSockets) {
        if (socket && socket->isOpen()) {
            socket->write(data);
            socket->flush();
        }
    }
}

void Server::broadcastWeatherCardChange(int index)
{
    QString message = QString("WEATHER_CARD:%1").arg(index);
    for (QTcpSocket* socket : clientSockets) {
        if (socket && socket->isOpen()) {
            socket->write(message.toUtf8());
        }
    }
}

void Server::onWeatherCardChanged(int newIndex)
{
    broadcastWeatherCardChange(newIndex);
}

void Server::onCurrentAndNextPlayerUpdated(const QString& currentPlayer, const QString& nextPlayer)
{
    qDebug() << "Current Player:" << currentPlayer << ", Next Player:" << nextPlayer;

    if (playerSocketMap.contains(nextPlayer)) {
        QTcpSocket* nextPlayerSocket = playerSocketMap[nextPlayer];
        // Notify the next player or update their turn
    } else {
        qDebug() << "Next player not found in the map!";
    }
}

void Server::onRoundStarted(int roundNumber)
{
    QString message = QString("ROUND_STARTED:%1").arg(roundNumber);
    broadcastMessage(message);
}
