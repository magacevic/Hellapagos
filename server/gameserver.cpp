#include "gameserver.h"
#include "voting.h"
#include "jsonserializer.h"
#include <QDebug>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTimer>
#include <QSet>
#include <QList>
#include <QFile>

GameServer::GameServer(QObject* parent)
    : QObject(parent),roundCounter(1)
{
    roundManager = new RoundManager(this);
    votingManager = new Voting(this);

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
    connect(votingManager, &Voting::votingStarted, this, &GameServer::onVotingStarted);

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
    return 0;
}


void GameServer::addPlayer(const QString& playerName, QTcpSocket* socket)
{
    if (!players.contains(playerName)) {
        players.append(playerName);
        roundManager->addToTurnPlayer(playerName);
        qDebug() << "Player added:" << playerName;
        socketToPlayerMap[socket] = playerName;
    } else {
        qDebug() << "Player" << playerName << "already exists!";
    }
}

QString GameServer::getPlayerNameFromSocket(QTcpSocket* socket) const
{
    return socketToPlayerMap.value(socket, "");
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



    int randomIndex = *availableCards.begin();
    availableCards.remove(randomIndex);

    if(roundCounter==6){
       // qDebug()<<"Ovdje sam 1";
        availableCards.insert(13);

    }

    if(roundCounter==7){
       // qDebug()<<"Ovdje sam 2";
        randomIndex=13;

    }

    if(randomIndex==13){
        roundManager->setLastRound(true);
    }


    emit weatherCardChanged(randomIndex);
}
void GameServer::onWeatherCardRequest() {
    changeWeatherCard();
}
void GameServer::onPlayerChanged(int newPlayerId)
{
    if (votingActive) {
        qDebug() << "[DEBUG] Player change delayed. Move in progress or voting active.";
        delayedPlayerChange = newPlayerId; // memorisi ID za kasnije
        return;
    }
    else{
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
            nextPlayer = "None";  // ako imamo samo jednog,nema sledeceg
        }

        qDebug() << "Next Player:" << nextPlayer;

        emit currentAndNextPlayerUpdated(currentPlayer, nextPlayer);
    }
}
}

void GameServer::startNextRound()
{

    if (votingActive) {
        qDebug() << "[DEBUG] Cannot start next round yet.";
        nextRoundDelay= true; // default false
        return; // ako glasanje traje ili je već startovana sledeća runda
    }

    else{
    votedThisRound = false;
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
}


void GameServer::onNewClientConnected()
{
    QTcpSocket* clientSocket = tcpServer->nextPendingConnection();
    clientSockets.append(clientSocket);


    connect(clientSocket, &QTcpSocket::disconnected, this, &GameServer::onClientDisconnected);
    connect(clientSocket, &QTcpSocket::readyRead, this, &GameServer::onReadyRead);
}

void GameServer::onClientDisconnected()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (clientSocket) {
        clientSockets.removeOne(clientSocket);
        clientSocket->deleteLater();
    }
}

void GameServer::onReadyRead()
{
}

void GameServer::onLastRoundLogic()
{
    qDebug()<<"This is the last round. Survive untill the end!";

}

void GameServer::gameEnd()
{
    if(votingActive){
        gameEndDelayed=true;
        return;
    }
    else{
        qDebug()<<"GAME OVER";

        // Lista pobednika(samo neeliminisani igrači)
        qDebug() << "Winners: " << players.join(", ");

        GameStatistics gameStats;
        JSONSerializer jsonSerializer;

        gameStats.setFileName("game_results.json");
        gameStats.setFilePath("game_results.json");

        QFile file(gameStats.getFilePath());
        if (!file.exists()) {
            qDebug() << "File does not exist.";
            return;
        }

        jsonSerializer.load(gameStats, gameStats.getFilePath());
        gameStats.addGameResult(players, this);
        jsonSerializer.save(gameStats, gameStats.getFilePath());


        emit gameOverSignal();
    }}

bool GameServer::checkWaterAmount()
{


    if(resources["water"]>=players.size()){
        return true;
    }
    else return false;

}

bool GameServer::checkFoodAmount()
{

    if(resources["food"]>=players.size()){
        return true;
    }
    else return false;
}
bool GameServer::checkLastRoundRafts()
{
    if (roundManager->getLastRound() && resources["rafts"] < players.size()) {
        // poslednja runda i nema dovoljno splavova
        return false;
    }
    return true; //sve ostalo
}


void GameServer::checkEnoughRafts(){
    if(resources["rafts"]>=players.size()){
        roundManager->setLastRound(true);
        qDebug()<<"You have made enough rafts! Initiating last round...";
    }


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

    if (resources["water"] <= 0 || resources["food"] <= 0) {
            qDebug() << "There is no water or food left";
            QStringList stillActive = players;
            for (const QString &p : stillActive) {
                eliminatePlayer(p);
                //emit voteResultSignal(p); // ako hoces da UI dobije update za svakog eliminisanog
        }
            gameEnd();
            return;
    }

     if(numOfPlayers==2 && resources["water"] == 1){
            qDebug() << "Special case: Only 1 water for 2 players! Eliminating player with least resources.";

        QString eliminatedPlayer;
        int minTotal = INT_MAX;

        for (const QString& player : players) {
            int total = playerTotalResources.value(player, 0);
            if (total < minTotal) {
                minTotal = total;
                eliminatedPlayer = player;
            }
        }

        if (!eliminatedPlayer.isEmpty()) {
            eliminatePlayer(eliminatedPlayer);
            emit voteResultSignal(eliminatedPlayer);
        }

    }

     else if (!checkFoodAmount() || !checkWaterAmount() ||
              !checkLastRoundRafts()) {
        startVoteIfNeeded();

    }



    if(resources["wood"] >=6){
        resources["rafts"] +=1;
        qDebug()<<"You have created a raft.";
        resources["wood"] -=6;
       }



}

void GameServer::newRoundDecrease(){
    if (votingActive) {
        qDebug() << "[DEBUG] Voting active. Delaying resource deduction.";
        delayedDecreaseResources = true;
        return;
    }
    else{
    int numOfPlayers = players.size();

    if(checkWaterAmount()){
        resources["water"] -= numOfPlayers;
        //qDebug()<<resources["water"]<<" voda";
    }
    if(checkFoodAmount()){
            resources["food"] -= numOfPlayers;
        }
    }

    emit newRoundResources();
}

void GameServer::startVoteIfNeeded()
{
   // if (votingActive || votedThisRound) return;
    if (votingActive) return;

    QSet<QString> currentPlayers = QSet<QString>(players.begin(), players.end());
    votingActive = true;
    votedThisRound = true;
    votingManager->startVote(currentPlayers);
    onVotingStarted(currentPlayers);
}

void GameServer::onVotingStarted(const QSet<QString>& eligiblePlayers)
{
    QStringList playerList = eligiblePlayers.values();
    emit startVoteSignal(playerList);
}

void GameServer::submitVote(const QString& voter, const QString& target){
    votingManager->submitVote(voter, target);

    if (votingManager->allVotesIn(players)) {
        QString eliminatedPlayer;
        QStringList topVoted = votingManager->getPlayersWithMostVotes();

        if (topVoted.size() == 1) {
            eliminatedPlayer = topVoted.first();
        } else {
            // Tie: eliminiši onog sa najmanje resursa
            int minTotal = INT_MAX;
            for (const QString& player : topVoted) {
                int total = playerTotalResources.value(player, 0);
                if (total < minTotal) {
                    minTotal = total;
                    eliminatedPlayer = player;
                }
            }

            QString tieMessage = QString("Tie detected. Eliminating player with least resources: \"%1\"").arg(eliminatedPlayer);
            qDebug() << tieMessage;

        }

        if (!eliminatedPlayer.isEmpty()) {
            eliminatePlayer(eliminatedPlayer);
            emit voteResultSignal(eliminatedPlayer);


        }

        votingManager->reset();
        votingActive = false;
     //   emit roundStarted(roundCounter);

        if (!checkWaterAmount() || !checkFoodAmount() ||
            !checkLastRoundRafts()) {
            // i dalje fali – pokreni još jedan voting
            nextVotingNeeded = true;

            //startVoteIfNeeded();

        }
        else{
            nextVotingNeeded=false;
            if (delayedPlayerChange != -1) {
                onPlayerChanged(delayedPlayerChange);
                delayedPlayerChange = -1;
            }
            if(gameEndDelayed){
                gameEndDelayed=false;
                gameEnd();
            }
            if (delayedDecreaseResources) {
                delayedDecreaseResources = false;
                newRoundDecrease();
            }
            if(nextRoundDelay){
                nextRoundDelay=false;
                startNextRound();
            }
            }
  /*  if (nextVotingNeeded) {
            QTimer::singleShot(0, this, [this]() {
                startVoteIfNeeded();
            });
        }
 */     emit votingAgain();
        QTimer::singleShot(2500, this, [this]() {
        if(nextVotingNeeded){
        QMetaObject::invokeMethod(this, "triggerNextVoting", Qt::QueuedConnection);
            }
            });
    }
}
void GameServer::triggerNextVoting()
{
    // Ako je između vremena ipak postao aktivan neki drugi voting – preskoči
    if (votingActive) return;

    nextVotingNeeded = false;
    startVoteIfNeeded();
}


void GameServer::eliminatePlayer(const QString& playerName)
{
    if (eliminatedPlayers.contains(playerName))
        return;

    eliminatedPlayers.insert(playerName);

    players.removeAll(playerName);

    roundManager->removeFromTurnPlayer(playerName);

    emit playerEliminatedSignal(playerName);


    qDebug() << "Player eliminated:" << playerName;
}

bool GameServer::isPlayerEliminated(const QString& playerName) const
{
    return eliminatedPlayers.contains(playerName);
}

QString GameServer::getGameStatisticsText() const
{
    // deserijalizacija
    JSONSerializer jsonSerializer;

    GameStatistics gameStats;
    gameStats.setFileName("game_results.json");
    gameStats.setFilePath("game_results.json");

    QFile file(gameStats.getFilePath());
    if (!file.exists()) {
        qDebug() << "File does not exist.";
    }

    jsonSerializer.load(gameStats, gameStats.getFilePath());
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

void GameServer::handleResourcesReport(const QString& message) {
    QStringList parts = message.split(": ");
    if (parts.size() < 6) {
        qDebug() << "Invalid RESOURCES_REPORT message format:" << message;
        return;
    }

    QString playerName = parts[1].mid(QString("player=").length());
    int food = parts[2].mid(QString("food=").length()).toInt();
    int water = parts[3].mid(QString("water=").length()).toInt();
    int wood = parts[4].mid(QString("wood=").length()).toInt();
    int total = parts[5].mid(QString("total=").length()).toInt();

    playerTotalResources[playerName] = total;
    qDebug()<<QString(" %1 : total %2 ").arg(playerName).arg(playerTotalResources[playerName]);

}

QMap<QString, int> GameServer::getPlayerTotalResources() const {
    return playerTotalResources;
}
