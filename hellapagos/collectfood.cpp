#include "collectfood.h"
#include <QDebug>
#include <cstdlib>

CollectFood::CollectFood(QObject *parent) : QObject(parent), currentFood(0) {}

void CollectFood::performAction() {
    // Generate random number feom 1 to 6
    int randomValue = (rand() % 6) + 1;

    // from drawn number, determine how much food is drawn
    if (randomValue <= 3) {
        this->foodCollected = 1; // 3/6 chance for 1
    } else if (randomValue <= 5) {
        this->foodCollected = 2; // 2/6 chance for 2
    } else {
        this->foodCollected = 3; // 1/6 chance for 3
    }

    // add new food
    currentFood += this->foodCollected;

    qDebug() << "Food collected: " << this->foodCollected;
    qDebug() << "Total food: " << currentFood;
}

int CollectFood::getCollectedFood() {
    int food = this->foodCollected;
    this->foodCollected = 0;
    return food;
}

int CollectFood::getTotalFood() {
    return currentFood;
}
