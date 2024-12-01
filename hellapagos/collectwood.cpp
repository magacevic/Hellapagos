#include "collectwood.h"
#include <QDebug>
#include <cstdlib>
#include <ctime>

CollectWood::CollectWood(QObject *parent)
    : QObject(parent),
    woodCollected(0),
    playerGotSick(false),
    additionalWood(0)
{
    // Random number generator for snake bite chance
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void CollectWood::performAction()
{
    // Player always collects one piece of wood
    woodCollected += 1;
}

// After the first piece, player can choose how many more pieces to collect
void CollectWood::goToForest(int additionalWood) {

    if (checkForSnakeBite(additionalWood)) {
        qDebug() << "You got bitten by a snake and got sick!";
        playerGotSick = true;

    } else {
        // Add the additional wood collected
        woodCollected += additionalWood;
        qDebug() << "You collected " << additionalWood << " more pieces of wood!";
    }
}

void CollectWood::getCollectedWood()
{
    qDebug() << "Total wood collected: " << woodCollected;

    if (woodCollected >= 6) {
        woodCollected -= 6;
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
    return chance <= woodToCollect;
}
