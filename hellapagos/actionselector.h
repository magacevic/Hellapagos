#ifndef ACTIONSELECTOR_H
#define ACTIONSELECTOR_H

#include <QObject>
#include "collectfood.h"
#include "collectwater.h"
#include "collectwood.h"

// Enum za različite tipove akcija
enum ActionType {
    COLLECT_FOOD = 0,
    COLLECT_WATER = 1,
    COLLECT_WOOD = 2
};

class ActionSelector : public QObject
{
    Q_OBJECT

public:
    // Konstruktor i destruktor
    explicit ActionSelector(QObject *parent = nullptr);
    ~ActionSelector();

    // Metoda koja omogućava selektovanje akcije (sakupljanje hrane, drveta, vode)
    void selectAction(ActionType actionType);  // Parametar koji odredjuje koju akciju da izvrši

private:
    // Pokazivači na objekte koji predstavljaju svaku akciju
    CollectFood *collectFoodAction;
    CollectWater *collectWaterAction;
    CollectWood *collectWoodAction;
};

#endif // ACTIONSELECTOR_H
