#include "serverApp.h"
#include <QDebug>
#include "gameserver.h"
#include "gamestatistics.h"
#include <QTimer>

Server::Server(QObject *parent)
    :   QObject(parent),
        tcpServer(new QTcpServer(this)),
        gameServer(new GameServer(this)),
        maxPlayers(3)
{
    connect(gameServer, &GameServer::weatherCardChanged, this, &Server::onWeatherCardChanged);
    connect(gameServer, &GameServer::currentAndNextPlayerUpdated, this, &Server::onCurrentAndNextPlayerUpdated);
    connect(gameServer, &GameServer::roundStarted, this, &Server::onRoundStarted);
    connect(gameServer,&GameServer::thisPlayerIsSick,this,&Server::playerSickSkip);
    connect(gameServer,&GameServer::gameOverSignal,this,&Server::onGameOver);
    connect(this, &Server::sendGameStatisticsToClient, this, &Server::sendStatsToClient);
    connect(gameServer, &GameServer::startVoteSignal, this, &Server::onStartVote);
    connect(gameServer, &GameServer::playerEliminatedSignal, this, &Server::onPlayerEliminated);
    connect(gameServer, &GameServer::voteResultSignal, this, &Server::onVoteResult);
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
    if (clientSockets.size() >= maxPlayers) {
        QTcpSocket* extraClient = tcpServer->nextPendingConnection();
        qDebug() << "Rejected connection: Maximum players reached!";
        extraClient->write("Server full. Connection rejected.");
        extraClient->flush();
        extraClient->disconnectFromHost();
        extraClient->deleteLater();
        return;
    }
    else if (clientSockets.size()<maxPlayers){
        // Accept new connections
        QTcpSocket* newClient = tcpServer->nextPendingConnection();
        clientSockets.append(newClient);
        connect(newClient, &QTcpSocket::readyRead, this, &Server::onReadyRead);
        connect(newClient, &QTcpSocket::disconnected, this, &Server::onDisconnected);

        qDebug() << "New client connected! Total players:" << clientSockets.size();
        broadcastPlayersList();
    }

    else if (clientSockets.size() == maxPlayers) {
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


    //int currentPlayerIndex = gameServer->getRoundManager()->getCurrentPlayerIndex();
    //qDebug() << "Current Player Index:" << currentPlayerIndex;

    if (message.startsWith("WEATHER_CARD:")) {
        int index = message.split(":").last().toInt();
        broadcastWeatherCardChange(index);
    }

    if (message == "REQUEST_WEATHER_CARD") {
        gameServer->onWeatherCardRequest();
    }

    if (message == "REQUEST_PLAYER_LIST") {
        broadcastPlayersList();
    }

    if(message.startsWith("SICK: ")){
        QString sickPlayerName=message.split(' ').last();
        //qDebug()<<sickPlayerName<< " is sick";
        broadcastMessage(sickPlayerName+" got BITTEN\n");
        gameServer->updateSickPlayersMap(sickPlayerName);
    }

    if (message.startsWith("ACTION_COMPLETED:")) {

        if(message.startsWith("ACTION_COMPLETED:COLLECT_WATER")){
            QString numberString = message.split(' ').last();
            int collectedWater=numberString.toInt();
           // gameServer->resources["water"]+=collectedWater;
            gameServer->addResource("water", collectedWater);
            int waterAmount = gameServer->getResourceAmount("water");
            qDebug() << "Current amount of water:" << waterAmount;
            broadcastResourcesInfo();
           // qDebug() << "Water resource updated. Total water:" << gameServer->resources["water"];
            QTimer::singleShot(500, this, [this]() {

                gameServer->getRoundManager()->actionCompleted();
            });

        } else if(message.startsWith("ACTION_COMPLETED:COLLECT_WOOD")){
            QString numberString = message.split(' ').last();
            int collectedWood=numberString.toInt();
            // gameServer->resources["water"]+=collectedWater;
            gameServer->addResource("wood", collectedWood);
            int woodAmount = gameServer->getResourceAmount("wood");
            qDebug() << "Current amount of wood:" << woodAmount;
            gameServer->checkRafts();
            broadcastResourcesInfo();
            // qDebug() << "Water resource updated. Total water:" << gameServer->resources["water"];
            QTimer::singleShot(500, this, [this]() {

                gameServer->getRoundManager()->actionCompleted();
            });

        }else if(message.startsWith("ACTION_COMPLETED:COLLECT_FOOD")){
            QString numberString = message.split(' ').last();
            int collectedFood=numberString.toInt();
            // gameServer->resources["water"]+=collectedWater;
            gameServer->addResource("food", collectedFood);
            int foodAmount = gameServer->getResourceAmount("food");
            qDebug() << "Current amount of food:" << foodAmount;
            broadcastResourcesInfo();
            // qDebug() << "Water resource updated. Total water:" << gameServer->resources["water"];
            QTimer::singleShot(500, this, [this]() {

                gameServer->getRoundManager()->actionCompleted();
            });
        }

    }

    if (message.startsWith("RESOURCES_REPORT:")) {
        gameServer->handleResourcesReport(message);
    }


    if (message.startsWith("REQUEST_ACTION:")) {
        QString action = message.split(":").last();
        QString currentPlayer = gameServer->getRoundManager()->currentPlayer();
        QTcpSocket* currentPlayerSocket = playerSocketMap.value(currentPlayer);

        if (senderSocket == currentPlayerSocket) {
            senderSocket->write(QString("ACTION_APPROVED:%1").arg(action).toUtf8());
        } else {
            senderSocket->write(QString("Sorry, not your turn.ACTION DENIED: %1").arg(action).toUtf8());
            }
    }
/*
    if (message.startsWith("ACTION_COMPLETED:COLLECT_FOOD")) {
        QString playerName = message.split(":").at(1);
        int foodAmount = message.split(":").at(2).toInt();
        gameServer->collectFoodFromPlayer(playerName, foodAmount);
    }
*/

    if (message == "REQUEST_GAME_STATISTICS") {
        QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
        onRequestGameStatistics(socket);
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
        gameServer->addPlayer(playerName, senderSocket);

        senderSocket->write(("Hello, " + playerName + "! Welcome to the server!").toUtf8());
    }
    else if (message == "Ready!") {
        readyPlayers++;
        qDebug() << "Player is ready! Total ready players:" << readyPlayers;

        if (readyPlayers == maxPlayers) {
            qDebug() << "All players are ready. Starting the game!";

            broadcastMessage("AllReady");

            QTimer::singleShot(500, this, [this]() {

                broadcastPlayersList();

                QTimer::singleShot(1000, this, [this]() {
                    // Reveal the first weather card
                    if (!weatherCardRevealed && clientSockets.size() == maxPlayers) {
                        gameServer->changeWeatherCard();
                        weatherCardRevealed = true;

                    }

                    // Start the first round
                    qDebug() << "First turn started";
                    gameServer->getRoundManager()->firstTurn();
                });
            });
        }
    }
    else if (message.startsWith("VOTE:")) {
        QString castVote = message.split(':').last();
        QString voter = gameServer->getPlayerNameFromSocket(senderSocket);

        if(gameServer->isPlayerEliminated(voter)){
            return;
        }

        if (!castVote.isEmpty() && !voter.isEmpty()) {
            gameServer->submitVote(voter, castVote);
        }

    }
}

void Server::onStartVote(const QStringList& voteTargets)
{
    QString message = "START_VOTE:" + voteTargets.join(",") + "\n";
    broadcastMessage(message);
}

void Server::onPlayerEliminated(const QString& playerName)
{
    broadcastMessage("PLAYER_ELIMINATED:" + playerName);
}

void Server::onVoteResult(const QString& playerName)
{
    broadcastMessage("VOTE_RESULT:" + playerName);
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

    broadcastPlayersList();
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
            socket->flush();
        }
    }
}

void Server::onWeatherCardChanged(int newIndex)
{
    broadcastWeatherCardChange(newIndex);
}

void Server::onCurrentAndNextPlayerUpdated(const QString& currentPlayer, const QString& nextPlayer)
{
    //qDebug() << "Current Player:server" << currentPlayer << ", Next Player:" << nextPlayer;

    if (playerSocketMap.contains(nextPlayer)) {
        QTcpSocket* nextPlayerSocket = playerSocketMap[nextPlayer];
        // Notify the next player or update their turn
    } else {
        qDebug() << "Next player not found in the map!";
    }


}

void Server::playerSickSkip(const QString& currentPlayer){
     QTimer::singleShot(1000, this, [this]() {

         gameServer->getRoundManager()->actionCompleted();
    });
}

void Server::onGameOver()
{
    QString gameOverMessage = QString("GAME OVER");
    broadcastMessage(gameOverMessage);
    //da se zapise u statistike ono sto je ostalo ko je pobedio itd
    //datum igranja i to

}

void Server::broadcastResourcesInfo(){

    QString resourceMessage = QString("RESOURCES:water:%1,food:%2,wood:%3,rafts:%4")
    .arg(gameServer->getResourceAmount("water"))
        .arg(gameServer->getResourceAmount("food"))
        .arg(gameServer->getResourceAmount("wood"))
        .arg(gameServer->getResourceAmount("rafts"));
    broadcastMessage(resourceMessage);

}

void Server::onRoundStarted(int roundNumber)
{
    //qDebug()<<"Starting round: "<<gameServer->roundCounter;
    QString message = QString(" ROUND_STARTED:%1 ").arg(roundNumber);
    broadcastMessage(message);

    gameServer->printSickPlayersMap();
    QTimer::singleShot(500, this, [this]() {


        QString resourcemessage = QString(" Decreasing resources after round ");
        broadcastMessage(resourcemessage);

    });
    QTimer::singleShot(800, this, [this]() {
        gameServer->decreaseResources();
        broadcastResourcesInfo();
    });

    QTimer::singleShot(1000, this, [this]() {
        gameServer->changeWeatherCard();
    });
}

void Server::onRequestGameStatistics(QTcpSocket *requestingClient)
{
    // fetch GameStatistics from gameServer
    QString statsText = gameServer->getGameStatisticsText();
    QString stats = "GAME_STATS:" + statsText;

    // Emit signal to send statistics to the client
    emit sendGameStatisticsToClient(stats, requestingClient);
}

void Server::sendStatsToClient(const QString& statsText, QTcpSocket *client)
{
    if (client && client->state() == QTcpSocket::ConnectedState) {
        client->write(statsText.toUtf8());
        client->flush();
        qDebug() << "Sent game statistics to client:" << statsText;
    } else {
        qDebug() << "Error: Client is not connected.";
    }
}

void Server::broadcastPlayersList()
{
    QString playersListMessage = "PLAYERS_LIST:";

    for (const QString& player : gameServer->getPlayers()) {
        playersListMessage += " " + player + ",";
    }

    if (playersListMessage.endsWith(",")) {
        playersListMessage.chop(1);
    }

    broadcastMessage(playersListMessage);
}



