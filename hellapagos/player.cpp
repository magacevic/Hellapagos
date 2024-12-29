// Player.cpp
#include "player.h"
#include "actionselector.h"
#include <QMessageBox>
#include <QInputDialog>

Player::Player(const QString& name)
    : playerName(name), alive(true), sick(false), totalFood(0), totalWater(0), totalWood(0), eliminatedPlayer(nullptr), sickPlayer(nullptr)
{
    // Inicijalizacija objekata za akcije
    collectFoodAction = new CollectFood();
    collectWaterAction = new CollectWater();
    collectWoodAction = new CollectWood();

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
    if (sick && sickPlayer == nullptr) {
        sickPlayer = new SickPlayer();
    }
}

bool Player::canPerformActions() const {
    return !sick || (sickPlayer && sickPlayer->canPerformActions());
}

bool Player::canVote() const {
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
	

    int foodCollectedInRound = collectFoodAction->performAction();
    addFood(foodCollectedInRound);

}

void Player::collectWater(QWidget *card) {
	
    collectWaterAction->collectWaterFromCard(card);
    int waterCollectedInRound = collectWaterAction->performAction();
    addWater(waterCollectedInRound);

}

void Player::collectWood() {

    collectWoodAction->performAction();
    int woodCollected = collectWoodAction->woodCollected;
    addWood(woodCollected);
    qDebug() <<"Wood collected so far by this player: " << totalWood;
    collectWoodAction->getCollectedWood();
    
}

void Player::collectAdditionalWood(int trips) {

    collectWoodAction->goToForest(trips);
    //sick = collectWoodAction->playerGotSick;
    if (collectWoodAction->playerGotSick) {
        sick = true;
        qDebug() << playerName  <<" got sick while collecting wood!";
    }
    	
    int woodCollected = collectWoodAction->additionalWoodCollected;
    addWood(woodCollected);
    qDebug() <<"Wood collected so far by this player: " << totalWood;
    collectWoodAction->getCollectedWood();
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
        sickPlayer->decrementRecovery();
        if (sickPlayer->isRecovered()) {
            sick = false;
            delete sickPlayer;
            sickPlayer = nullptr;
            qDebug() << "Player has recovered!";
        }
    }
}

void Player::eliminate() {
    alive = false;
    // Kreira novu instancu EliminatedPlayer kada je igrač eliminisan
    if (eliminatedPlayer == nullptr) {
        eliminatedPlayer = new EliminatedPlayer();
    }
}
EliminatedPlayer* Player::getEliminatedPlayer() const {
    return eliminatedPlayer;
}

