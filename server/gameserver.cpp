#include "gameserver.h"
#include <QDebug>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTimer>
#include <QSet>

GameServer::GameServer(QObject* parent)
    : QObject(parent),roundCounter(1)
{
    roundManager = new RoundManager(this);
    votingSystem = new Voting(this);

    resources.insert("food", 8);
    resources.insert("water", 10);
    resources.insert("wood", 0);
    resources.insert("rafts",0);

      connect(roundManager, &RoundManager::currentAndNextPlayerUpdated,
            this, &GameServer::currentAndNextPlayerUpdated);

    connect(roundManager, &RoundManager::playerChanged, this, &GameServer::onPlayerChanged);

    connect(roundManager, &RoundManager::roundCompleted, this, &GameServer::startNextRound);
    connect(roundManager, &RoundManager::roundCompleted, this, &GameServer::decreaseResources);
    connect(roundManager,&RoundManager::lastRoundStarted,this,&GameServer::onLastRoundLogic);
    connect(roundManager,&RoundManager::lastRoundEnded,this,&GameServer::gameEnd);

    // Povezi signale za glasanje
   // connect(votingSystem, &Voting::votingStarted, this, &GameServer::onVotingStarted);
    //connect(votingSystem, &Voting::votingConcluded, this &GameServer::onVotingConcluded);

    // Povezi signal za eliminaciju igraca, Voting i RoundManager
    connect(votingSystem, &Voting::playerEliminatedForRoundManager, roundManager, &RoundManager::handlePlayerEliminated);

    for (int i = 1; i < 12; ++i) {
        availableCards.insert(i);
    }

    initSickPlayersMap();
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

int GameServer::getWeatherCardIndex() const
{
    return currentWeatherCardIndex;
}

void GameServer::changeWeatherCard() {
    if (availableCards.isEmpty()) {
        qDebug() << "No cards left to reveal!";
        return;
    }



    if(roundCounter==7){
        availableCards.insert(13);
    }



    int randomIndex = *availableCards.begin();
    availableCards.remove(randomIndex);

    if(roundCounter==3){
        randomIndex=13;

    }

    if(randomIndex==13){
        roundManager->setLastRound(true);
    }

   // availableCards.remove(randomIndex);

    emit weatherCardChanged(randomIndex);
}
void GameServer::onWeatherCardRequest() {
    changeWeatherCard();
}
void GameServer::onPlayerChanged(int newPlayerId)
{
    if (newPlayerId < 0 || newPlayerId >= players.size()) {
        qDebug() << "Invalid player ID:" << newPlayerId;
        return;
    }

    QString currentPlayer = players.at(newPlayerId);
    qDebug() << "Current Player: " << currentPlayer;

    if(sickPlayers[currentPlayer]==1){
        sickPlayers[currentPlayer]=0;
        qDebug()<<"Player "<<currentPlayer<<" is sick!Next turn...";

        emit thisPlayerIsSick(currentPlayer);

        return;
    }

    else{

        QString nextPlayer;
        if (players.size() > 1) {
            int nextPlayerId = (newPlayerId + 1) % players.size();
            nextPlayer = players.at(nextPlayerId);
        } else {
            nextPlayer = "None";  // If only one player exists, no next player
        }

        qDebug() << "Next Player:" << nextPlayer;

        emit currentAndNextPlayerUpdated(currentPlayer, nextPlayer);
    }
}

void GameServer::startNextRound()
{
    resourcesDeductedThisRound = false;

    if (roundManager->getLastRound()) {
        qDebug() << "This is the last round!";
    }

    roundCounter++;
    qDebug() << "Starting Round:" << roundCounter;

    if (roundCounter == 13) {
        roundManager->setLastRound(true);
        qDebug() << "Setting next round as the last round.";
    }


    emit roundStarted(roundCounter);

}


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

void GameServer::onLastRoundLogic()
{
    qDebug()<<"Ovo je poslednja runda.";

}

void GameServer::gameEnd()
{
    qDebug()<<"GAME OVER";
    emit gameOverSignal();
}

bool GameServer::checkWaterAmount()
{
    //    while(resources["water"]<players.size()){

    if(resources["water"]>=players.size()){
        return true;
    }
    else if(resources["water"]<players.size()){
        int num=players.size()-resources["water"];
        while(num>0){
            qDebug()<< "Please vote out this many players" << num <<" [water]";
            //Logika za voting
            num--;
        }
        if(num==0) return true;
    }



   return true;

}

bool GameServer::checkFoodAmount()
{

    if(resources["food"]>=players.size()){
        return true;
    }
    else if(resources["food"]<players.size()){
        int num=players.size()-resources["food"];
        while(num>0){
            qDebug()<< "Please vote out this many players" << num <<" [food]";
            //Logika za voting
            num--;
        }
        if(num==0) return true;
    }



    return true;
}


void GameServer::checkRafts()
{
    const int woodRequired = 6;

    if(resources["wood"] >= woodRequired ) {
        resources["wood"] -= woodRequired;
        resources["rafts"] += 1;

        qDebug() << "Converted 6 wood into 1 raft.";
        qDebug() << "Wood left:" << resources["wood"] << ", rafts available:" << resources["rafts"];

    }

}

void GameServer::decreaseResources(){

    if (resourcesDeductedThisRound) {
        qDebug() << "[DEBUG] Skipping duplicate resource deduction.";
        return;
        }

    resourcesDeductedThisRound=true;

    int numOfPlayers = players.size();
    if(checkWaterAmount()){
        resources["water"] -= numOfPlayers;
        if(checkFoodAmount()){
            resources["food"] -= numOfPlayers;
        }

    }

    if(resources["wood"] >=6){
        resources["rafts"] +=1;
        qDebug()<<"You have created a raft.";
        resources["wood"] -=6;
       }
}

QString GameServer::getGameStatisticsText() const
{
    GameStatistics gameStats;
    gameStats.loadFromJson("game_results.json");

    QString statsText;
    statsText += QString("Total Games Played: %1\n\n").arg(gameStats.getTotalGamesPlayed());
    statsText += "Game Statistics:\n\n";

    for (const GameStatistics::GameResult& result : gameStats.getGameResults()) {
        statsText += QString("Game Date: %1\n").arg(result.gameDate.toString(Qt::ISODate));
        statsText += QString("Winners: %1\n").arg(result.winners.join(", "));
        statsText += QString("Remaining Food: %1\n").arg(result.remainingFood);
        statsText += QString("Remaining Water: %1\n").arg(result.remainingWater);
        statsText += QString("Remaining Wood: %1\n\n").arg(result.remainingWood);
    }

    return statsText;
}

void GameServer::updateSickPlayersMap(const QString &playerName)
{
    sickPlayers[playerName]=1;
}


void GameServer::initSickPlayersMap(){
    for(QString playerName:players){
        sickPlayers[playerName]=0;
    }
}

void GameServer::printSickPlayersMap() const {
    qDebug() << "Sick Players Map:";
    for(QString playerName:players){
        qDebug()<< playerName<< ": "<<sickPlayers[playerName]<<"-----";
    }
}
