#include "eliminatedplayer.h"

// Konstruktor
EliminatedPlayer::EliminatedPlayer(const QString& name) : playerName(name), isSpectating(false) {}


void EliminatedPlayer::setSpectating(bool spectate) {
    isSpectating = spectate;
}


bool EliminatedPlayer::canVote() const {
    return false; // Eliminisani igrači ne mogu da glasaju
}


bool EliminatedPlayer::canPerformActions() const {
    return false; // Eliminisani igrači ne mogu da izvode akcije
}


bool EliminatedPlayer::wantsToSpectate() const {
    return isSpectating; // Vraća status da li igrač gleda
}


bool EliminatedPlayer::canCollect() const {
    return false; // Eliminisani igrači ne mogu da prikupljaju resurse
}

QString EliminatedPlayer::getPlayerName() const {
    return playerName;
}

