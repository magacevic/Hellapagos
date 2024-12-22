#include "gameserver.h"
#include <QDebug>
#include <QTcpSocket>
#include <QTcpServer>

#include <QSet>

GameServer::GameServer(QObject* parent)
    : QObject(parent),roundCounter(1)
{
    roundManager = new RoundManager(this);
    // Initialize resource counts to zero
    resources.insert("food", 0);
    resources.insert("water", 0);
    resources.insert("wood", 0);

    //connect(this, &GameServer::weatherCardChanged, this, &GameServer::broadcastWeatherCardChange);
    connect(roundManager, &RoundManager::currentAndNextPlayerUpdated,
            this, &GameServer::currentAndNextPlayerUpdated);

    //connect(roundManager, &RoundManager::changeWeatherCard, this,&GameServer::onWeatherCardRequest);


    // Poveži signal za promenu igrača
    connect(roundManager, &RoundManager::playerChanged, this, &GameServer::onPlayerChanged);

    connect(roundManager, &RoundManager::roundCompleted, this, &GameServer::startNextRound);

    for (int i = 1; i < 13; ++i) {
        availableCards.insert(i);
    }


}

RoundManager* GameServer::getRoundManager() const {
    return roundManager;
}

void GameServer::addResource(const QString& resourceName, int amount) {
    resources[resourceName] += amount;
    qDebug() << "Resource" << resourceName << "updated. New amount:" << resources[resourceName];
}


int GameServer::getResourceAmount(const QString& resourceName) const {
    if (resources.contains(resourceName)) {
        return resources[resourceName];
    }
    return 0; // Return 0 if the resource does not exist
}


void GameServer::addPlayer(const QString& playerName)
{
    if (!players.contains(playerName)) {
        players.append(playerName);
        roundManager->addToTurnPlayer(playerName);
        qDebug() << "Player added:" << playerName;
    } else {
        qDebug() << "Player" << playerName << "already exists!";
    }
}

QStringList GameServer::getPlayers() const
{
    return players;
}

QMap<QString, int> GameServer::getResources() const
{
    return resources;
}
/*
void GameServer::changeWeatherCard(int newIndex)
{
    currentWeatherCardIndex = newIndex;
    emit weatherCardChanged(newIndex); // Signal for serverApp to notify clients
}
*/
int GameServer::getWeatherCardIndex() const
{
    return currentWeatherCardIndex;
}

void GameServer::changeWeatherCard() {
    if (availableCards.isEmpty()) {
        qDebug() << "No cards left to reveal!";
        return;
    }

    int randomIndex = *availableCards.begin();
    availableCards.remove(randomIndex);

    emit weatherCardChanged(randomIndex);
}
void GameServer::onWeatherCardRequest() {
    changeWeatherCard();
}

// Slot koji prihvata signal za promenu igrača
void GameServer::onPlayerChanged(int newPlayerId)
{
    // Validate the player ID before accessing the list
    if (newPlayerId < 0 || newPlayerId >= players.size()) {
        qDebug() << "Invalid player ID:" << newPlayerId;
        return;
    }

    QString currentPlayer = players.at(newPlayerId);
    qDebug() << "Current Player:" << currentPlayer;

    // Calculate the next player safely
    QString nextPlayer;
    if (players.size() > 1) {
        int nextPlayerId = (newPlayerId + 1) % players.size();
        nextPlayer = players.at(nextPlayerId);
    } else {
        nextPlayer = "None";  // If only one player exists, no next player
    }

    qDebug() << "Next Player:" << nextPlayer;

    // Emit signal for the current and next player update
    emit currentAndNextPlayerUpdated(currentPlayer, nextPlayer);
}


void GameServer::startNextRound()
{
    roundCounter++;
    qDebug() << "Starting Round:" << roundCounter;

    emit roundStarted(roundCounter);
}



/*
void GameServer::collectFoodFromPlayer(const QString& playerName, int foodAmount)
{
    if (resources.contains("food")) {
        resources["food"] += foodAmount;  // Add collected food to total food on the server
        qDebug() << playerName << "collected" << foodAmount << "food. Total food: " << resources["food"];

        // Optionally broadcast the food collection update to all clients
        broadcastFoodUpdate(playerName, foodAmount);
    } else {
        qDebug() << "Resources map does not contain 'food'!";
    }
}
*/

void GameServer::broadcastFoodUpdate(const QString& playerName, int foodAmount)
{
    QString message = QString("%1 collected %2 food. Total food on server: %3")
    .arg(playerName)
        .arg(foodAmount)
        .arg(resources["food"]);

    // Send the message to all connected clients
    for (QTcpSocket* clientSocket : clientSockets) {
        if (clientSocket->state() == QTcpSocket::ConnectedState) {
            clientSocket->write(message.toUtf8());
        }
    }
}


// Assuming you have a QTcpServer in your class
void GameServer::onNewClientConnected()
{
    QTcpSocket* clientSocket = tcpServer->nextPendingConnection();  // Get the next pending connection
    clientSockets.append(clientSocket);  // Add the socket to the list of clients

    // Connect necessary signals for the client
    connect(clientSocket, &QTcpSocket::disconnected, this, &GameServer::onClientDisconnected);
    connect(clientSocket, &QTcpSocket::readyRead, this, &GameServer::onReadyRead);
}

void GameServer::onClientDisconnected()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (clientSocket) {
        clientSockets.removeOne(clientSocket);  // Remove the client socket from the list when disconnected
        clientSocket->deleteLater();  // Clean up the socket
    }
}
void GameServer::onReadyRead()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (clientSocket) {
        QByteArray data = clientSocket->readAll();  // Read all data sent by the client
        qDebug() << "Received from client:" << data;
        // Handle the received data here...
    }
}

