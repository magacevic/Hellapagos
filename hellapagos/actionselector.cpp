#include "actionselector.h"
#include "collectwater.h"
#include <QStackedWidget>
#include <QDebug>
#include <QWidget>

ActionSelector::ActionSelector(QObject *parent) : QObject(parent)
{
    // Kreiramo objekte za svaku od akcija
    collectFoodAction = new CollectFood(this);
    collectWaterAction = new CollectWater(this);
    collectWoodAction = new CollectWood(this);
}

// Implementacija destruktora
ActionSelector::~ActionSelector()
{
    // Ako je potrebno, možemo osloboditi resurse ili izvršiti čišćenje
    // U ovom slučaju, Qt će automatski obrisati collectFoodAction jer je postavljen za roditeljsku instancu
    qDebug() << "ActionSelector destructor called.";
}

void ActionSelector::selectAction(ActionType actionType)
{
    switch (actionType) {
    case COLLECT_FOOD:  // 0 znači "Collect Food"
        qDebug() << "Selected action: Collect Food";
        collectFoodAction->performAction();  // Pozivamo funkciju za sakupljanje hrane
        break;
    case COLLECT_WATER:
        qDebug() << "Selected action: Collect Water";
        collectWaterAction->performAction();
    case COLLECT_WOOD: // 2 znači "Collect Wood"
        qDebug() << "Selected action: Collect Wood";
        collectFoodAction->performAction();
    default:
        qDebug() << "Invalid action selected!";
        break;
    }
}

