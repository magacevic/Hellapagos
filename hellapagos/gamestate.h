#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QObject>
#include <QString>
#include <QDebug>

class GameState
{
private:
    int roundNumber;               // Current round
    int water;                     // Amount of water
    int food;                      // Amount of food
    int wood;                      // Amount of wood
    int raftsMade;                 // Number of rafts made
public:
    GameState();

    void updateState(int waterChange, int foodChange, int woodChange, int raftChange);
    void printState(int roundNumber, int water, int food, int wood, int raftsMade);
};

#endif // GAMESTATE_H
