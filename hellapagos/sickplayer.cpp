#include "sickplayer.h"
#include <QDebug>

sickPlayer::sickPlayer(QObject *parent)
    : CollectWood(parent),
    isSick(false),
    safeWood(1) // Initial wood is always safe
{
}

void sickPlayer::enterForest(bool collectAdditional, int additionalWood)
{
    // Player collects the initial piece of wood
    performAction();

    if (collectAdditional) {
        qDebug() << "Player decides to collect more wood.";
        goToForest(additionalWood);

        if (playerGotSick) {
            // If bitten by a snake, reset wood and mark player as sick
            isSick = true;
            totalWoodCollected = safeWood; // Retain only the safe wood
            qDebug() << "Player is bitten by a snake and lost all additional wood!";
        } else {
            qDebug() << "Player successfully collected " << additionalWoodCollected
                     << " additional wood.";
        }
    } else {
        qDebug() << "Player chose not to collect additional wood.";
    }

    // Check if enough wood is collected to build a raft
    checkRaftCreation();
}

void sickPlayer::checkRaftCreation()
{
    if (totalWoodCollected >= 6) {
        totalWoodCollected -= 6; // Use 6 pieces to create a raft
        qDebug() << "A raft has been created! Remaining wood: " << totalWoodCollected;
    } else {
        qDebug() << "Current total wood: " << totalWoodCollected
                 << ". More wood is needed for a raft.";
    }
}

void sickPlayer::inspectWreck()
{
    // Simulate drawing a card from the wreck
    int cardDrawn = std::rand() % 10 + 1; // Example: Random card from 1 to 10
    qDebug() << "Player inspected the wreck and drew card #" << cardDrawn;
    wreckCards.append(cardDrawn); // Add card to the player's deck
}

void sickPlayer::displayStatus()
{
    if (isSick) {
        qDebug() << "Player is currently sick!";
    } else {
        qDebug() << "Player is healthy.";
    }

    qDebug() << "Safe wood: " << safeWood << ", Total wood collected: " << totalWoodCollected;
    qDebug() << "Wreck cards: " << wreckCards;
}



