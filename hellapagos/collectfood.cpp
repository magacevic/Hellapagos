#include "collectfood.h"
#include <QDebug>
#include <cstdlib>

CollectFood::CollectFood(QObject *parent) : QObject(parent), foodCollectedInRound(0) {}

int CollectFood::performAction() {
    // Generate a random number from 1 to 6
    int randomValue = (rand() % 6) + 1;

    // Determine how much food is collected based on the random number
    if (randomValue <= 3) {
        foodCollectedInRound = 1; // 3/6 chance for 1
    } else if (randomValue <= 5) {
        foodCollectedInRound = 2; // 2/6 chance for 2
    } else {
        foodCollectedInRound = 3; // 1/6 chance for 3
    }

    qDebug() << "Food collected this round: " << foodCollectedInRound;
    return foodCollectedInRound;
}
