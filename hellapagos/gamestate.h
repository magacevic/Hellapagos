#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QObject>

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

    // Updating game state
    void updateState(int waterChange, int foodChange, int woodChange, int raftChange);
};

#endif // GAMESTATE_H
