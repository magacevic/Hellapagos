#include "sickplayer.h"
#include <QDebug>

SickPlayer::SickPlayer() : recoveryRounds(1) {}

// Reduces the number of remaining recovery rounds
void SickPlayer::decrementRecovery() {
    if (recoveryRounds > 0) {
        recoveryRounds--;
    }
}

// Check if the player can vote
bool SickPlayer::canVote() const {
    return recoveryRounds == 0;
}

// Check if the player can perform actions
bool SickPlayer::canPerformActions() const {
    return recoveryRounds == 0;
}

// Check if the player has recovered
bool SickPlayer::isRecovered() const {
    return recoveryRounds == 0;
}




