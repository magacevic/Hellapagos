#include "collectwater.h"

#include <QDebug>
#include <QStackedWidget>
#include <QWidget>

CollectWater::CollectWater(QObject* parent) : QObject(parent), waterCollected(0) {}

void CollectWater::collectWaterFromCard(QWidget* card)
{
    if (!card)
    {
        qDebug() << "Invalid card!";
        return;
    }

    if (card->objectName() == "page_5" || card->objectName() == "page_6" ||
        card->objectName() == "page_9")
    {
        // 1 kapljica
        waterCollected++;
    }
    else if (card->objectName() == "page_7" || card->objectName() == "page_8" ||
             card->objectName() == "page_10")
    {
        // 2 kapljice
        waterCollected += 2;
    }
    else if (card->objectName() == "page_2" || card->objectName() == "page_3" ||
             card->objectName() == "page_4")
    {
        // 0 kapljica
        //  qDebug() << "Total collected water:" << waterCollected;
    }
    else if (card->objectName() == "page_11" || card->objectName() == "page_12")
    {
        // 3 kapljice
        waterCollected += 3;
    }
    else if (card->objectName() == "page_13")
    {
        // uragan
        waterCollected += 2;
    }
}

int CollectWater::performAction()
{
    // Reset the collected water in a turn to 0
    int waterCollectedInRound = waterCollected;
    waterCollected = 0;
    // emit actionCompleted("COLLECT_WATER");
    return waterCollectedInRound;
}
