#include "gamestate.h"
#include <QString>
#include <QDebug>

GameState::GameState()
    : roundNumber(0), water(4), food(4), wood(0), raftsMade(0){

}

void GameState::updateState(int waterChange, int foodChange, int woodChange, int raftChange) {
    roundNumber++;
    water += waterChange;
    food += foodChange;
    wood += woodChange;
    raftsMade += raftChange;

    if (wood >= 6) {
        raftsMade++;
        wood = 0;
    }
}

void GameState::printState(int roundNumber, int water, int food, int wood, int raftsMade){
    QString output = "Game State:\n";
    output += "  Round Number: " + QString::number(roundNumber) + "\n";
    output += "  Water: " + QString::number(water) + "\n";
    output += "  Food: " + QString::number(food) + "\n";
    output += "  Wood: " + QString::number(wood) + "\n";
    output += "  Rafts Made: " + QString::number(raftsMade) + "\n";

    qDebug() << output;
}
