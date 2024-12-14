#include "gamestate.h"

GameState::GameState()
    : roundNumber(0), water(4), food(4), wood(0), raftsMade(0){

}

void GameState::updateState(int waterChange, int foodChange, int woodChange, int raftChange) {
    roundNumber++;
    water += waterChange;
    food += foodChange;
    wood += woodChange;
    raftsMade += raftChange;

    // If we have made a raft, reset the wood count
    if (wood >= 6) {
        raftsMade++;
        wood = 0;
    }
}
