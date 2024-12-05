#include "actionselector.h"
#include <QStackedWidget>
#include <QDebug>
#include "player.h"

ActionSelector::ActionSelector(Player *player, QObject *parent) : QObject(parent), player(player) {
    // Instanca Player-a se prosledjuje u konstruktoru, nema potrebe da pravimo novu ovde.
}

ActionSelector::~ActionSelector() {
    // Brisanje obavlja gameWindow
}

void ActionSelector::selectAction(ActionType actionType, QWidget *card) {
    switch (actionType) {
    case COLLECT_FOOD:
        player->collectFood();
        break;
    case COLLECT_WATER:
        player->collectWater(card);
        break;
    case COLLECT_WOOD:
        //player->collectWood();
        break;
    default:
        break;
    }
}


