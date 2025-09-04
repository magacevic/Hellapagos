// Player.cpp
#include "player.h"
#include "actionselector.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QTimer>

Player::Player(const QString& name, Client* client)
    : playerName(name), alive(true), sick(false), totalFood(0), totalWater(0), totalWood(0), eliminatedPlayer(nullptr), sickPlayer(nullptr)
{
    // Inicijalizacija objekata za akcije
    collectFoodAction = new CollectFood();
    collectWaterAction = new CollectWater();
    collectWoodAction = new CollectWood();
    m_client = client;

    connect(collectWoodAction,&CollectWood::playerGotBitten,this,&Player::onPlayerGotBitten);

}

QString Player::getName() const {
    return playerName;
}

void Player::setName(const QString& name) {
    playerName = name;
}

bool Player::isAlive() const {
    return alive;
}

void Player::setAlive(bool alive) {
    this->alive = alive;
}

bool Player::isSick() const {
    return sick;
}

void Player::setSick(bool sick) {
    this->sick = sick;
   /* sickPlayer = new SickPlayer();
    sickPlayer->setRecovered(false);
    m_client->addPlayer(sickPlayer);
*/
    if (sick && !sickPlayer) {
        sickPlayer = QSharedPointer<SickPlayer>::create();
        sickPlayer->setRecovered(false);
        m_client->addPlayer(sickPlayer); // Client sada prima QSharedPointer<SickPlayer>
    }}

bool Player::canPerformActions() {

    if(eliminatedPlayer)
        return eliminatedPlayer->canPerformActions();

    if (sickPlayer && sickPlayer->isRecovered()) {
        recover();
    }
    return !sick; //|| (sickPlayer && sickPlayer->canPerformActions());
}

bool Player::canVote() const {

    if(eliminatedPlayer)
        return eliminatedPlayer->canVote();

    return !sick || (sickPlayer && sickPlayer->canVote());
}


int Player::getTotalFood() const {
    return totalFood;
}

int Player::getTotalWater() const {
    return totalWater;
}

int Player::getTotalWood() const {
    return totalWood;
}

void Player::addFood(int food) {
    totalFood = food;
}

void Player::addWater(int water) {
    totalWater = water;
}

void Player::resetWood(){
    totalWood=0;
}
void Player::addWood(int wood) {
    totalWood += wood;
}

//U zavisnosti od izabrane akcije u actionSelectoru, pozivamo odgovarajuce metode
void Player::collectFood() {
    if (!canPerformActions()) {
        qDebug() << playerName << " is sick and cannot collect food!";

        return;
    }
    int foodCollectedInRound = collectFoodAction->performAction();
    addFood(foodCollectedInRound);
    reportResourcesToServer(foodCollectedInRound, 0);

}

void Player::collectWater(QWidget *card) {
    if (!canPerformActions()) {
        qDebug() << playerName << " is sick and cannot collect water!";
        return;
    }
    collectWaterAction->collectWaterFromCard(card);
    int waterCollectedInRound = collectWaterAction->performAction();
    addWater(waterCollectedInRound);
    reportResourcesToServer(0, waterCollectedInRound);

}

void Player::collectWood() {
    if (!canPerformActions()) {
        qDebug() << playerName << " is sick and cannot collect wood!";
        return;
    }

    collectWoodAction->performAction();
    int woodCollected = collectWoodAction->woodCollected;
    addWood(woodCollected);
    qDebug() <<"Wood collected so far by this player: " << totalWood;
    collectWoodAction->getCollectedWood();
    reportResourcesToServer(0,0);
    
}

void Player::collectAdditionalWood(int trips) {

    collectWoodAction->goToForest(trips);
    //sick = collectWoodAction->playerGotSick;
    if (collectWoodAction->playerGotSick) {
        setSick(true);
        qDebug() << playerName  <<" got sick while collecting wood!";
    }
    	
    int woodCollected = collectWoodAction->additionalWoodCollected;
    addWood(woodCollected);
    qDebug() <<"Wood collected so far by this player: " << totalWood;
    collectWoodAction->getCollectedWood();
    reportResourcesToServer(0,0);
}

/*
void Player::raftCreated() {
    qDebug() << "Total wood collected: " << totalWood;

    if (totalWood >= 6) {
        totalWood -= 6;
        qDebug() << "Raft created!";
    }
}
*/

void Player::recover() {
    if (sick && sickPlayer) {
        sick = false;
        //delete sickPlayer;
        //sickPlayer = nullptr;
        sickPlayer.clear();
        qDebug() << "Player has recovered!";
    }
}

void Player::eliminate() {
    alive = false;
    // Kreira novu instancu EliminatedPlayer kada je igrač eliminisan
    if (eliminatedPlayer == nullptr) {
        eliminatedPlayer = new EliminatedPlayer();
    }
    eliminatedPlayer->setSpectating(true);
}
EliminatedPlayer* Player::getEliminatedPlayer() const {
    return eliminatedPlayer;
}

void Player::onPlayerGotBitten()
{
    m_client->sendMessage("SICK: "+playerName+ "\n");

    qDebug()<<"POSLAO SAM";
}


void Player::reportResourcesToServer(int newlyCollectedFood, int newlyCollectedWater) {

    tFood += newlyCollectedFood;
    tWater += newlyCollectedWater;

    QTimer::singleShot(500, this, [this](){

    int total = tFood + tWater + totalWood;

    QString report = QString("RESOURCES_REPORT: player=%1: food=%2: water=%3: wood=%4: total=%5\n")
                         .arg(playerName)
                         .arg(tFood)
                         .arg(tWater)
                         .arg(totalWood)
                         .arg(total);
    m_client->sendMessage(report);
    });
}

