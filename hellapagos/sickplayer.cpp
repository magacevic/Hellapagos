#include "sickplayer.h"
#include <QDebug>

SickPlayer::SickPlayer() : recoveryRounds(1) {}

void SickPlayer::decrementRecovery() {
    if (recoveryRounds > 0) {
        recoveryRounds--;
    }

    if (recoveryRounds == 0) {
        setRecovered(true);;
    }
}

bool SickPlayer::canVote() const {
    return recoveryRounds == 0;
}

bool SickPlayer::canPerformActions() const {
    return recoveryRounds == 0;
}

bool SickPlayer::isRecovered() const {
    return recovered;
}

void SickPlayer::setRecovered(bool recovered)
{
    this->recovered = recovered;
}




