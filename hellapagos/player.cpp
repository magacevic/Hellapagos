// Player.cpp
#include "player.h"
#include "actionselector.h"

Player::Player(const QString& name)
    : playerName(name), alive(true), sick(false), totalFood(0), totalWater(0), foodCollectedInRound(0)
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
}

int Player::getTotalFood() const {
    return totalFood;
}

int Player::getTotalWater() const {
    return totalWater;
}

void Player::addFood(int food) {
    totalFood += food;
}

void Player::addWater(int water) {
    totalWater += water;
}

//U zavisnosti od izabrane akcije u actionSelectoru, pozivamo odgovarajuce metode
void Player::collectFood() {
    foodCollectedInRound = collectFoodAction->performAction();
    addFood(foodCollectedInRound);
}

void Player::collectWater(QWidget *card) {
    collectWaterAction->collectWaterFromCard(card);
    int waterCollectedInRound = collectWaterAction->performAction();
    addWater(waterCollectedInRound);
}
