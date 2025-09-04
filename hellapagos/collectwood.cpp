#include "collectwood.h"
#include <QDebug>
#include <cstdlib>
#include <ctime>
#include <QMessageBox>

CollectWood::CollectWood(QObject *parent)
    : QObject(parent),
    woodCollected(0),
    playerGotSick(false),
    additionalWood(0),
    additionalWoodCollected(0)
{
    // Random number generator for snake bite chance
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void CollectWood::performAction()
{
    woodCollected = 0;
    totalWoodCollected = 0;
    // Player always collects one piece of wood
    woodCollected += 1;
    totalWoodCollected +=1;
}

// After the first piece, player can choose how many more pieces to collect
void CollectWood::goToForest(int additionalWood) {
    additionalWoodCollected = 0;

    if (checkForSnakeBite(additionalWood)) {
        //qDebug() << "You got bitten by a snake and got sick!";
        playerGotSick = true;
        QMessageBox::critical(nullptr, "Snake Bite", "You got bitten by a snake and got sick!");
        emit playerGotBitten();
        qDebug() << "Player got bitten";

    } else {
        // Add the additional wood collected
        additionalWoodCollected += additionalWood;
        totalWoodCollected += additionalWood;
        //qDebug() << "You collected " << additionalWood << " more pieces of wood!";
    }
}

void CollectWood::getCollectedWood()
{
    qDebug() << "Total wood collected: " << totalWoodCollected;

    if (totalWoodCollected >= 6) {
        totalWoodCollected -= 6;
        qDebug() << "Raft created!";
    }
}


void CollectWood::didPlayerGetSick()
{
    if (playerGotSick) {
        qDebug() << "The player got sick.";
    } else {
        qDebug() << "The player is healthy!";
    }
}

bool CollectWood::checkForSnakeBite(int woodToCollect)
{
    // Chance of being bitten by a snake
    int chance = std::rand() % 6 + 1;
    qDebug() << "chance: " << chance;
    return chance <= woodToCollect;
}
