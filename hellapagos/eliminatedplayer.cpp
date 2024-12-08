#include "eliminatedplayer.h"

// Konstruktor
EliminatedPlayer::EliminatedPlayer() : isSpectating(false) {}

// Funkcija koja postavlja igrača u režim gledanja
void EliminatedPlayer::setSpectating(bool spectate) {
    isSpectating = spectate;
}

// Proverava da li igrač može da glasa
bool EliminatedPlayer::canVote() const {
    return false; // Eliminisani igrači ne mogu da glasaju
}

// Proverava da li igrač može da izvodi akcije
bool EliminatedPlayer::canPerformActions() const {
    return false; // Eliminisani igrači ne mogu da izvode akcije
}

// Proverava da li igrač želi da gleda igru
bool EliminatedPlayer::wantsToSpectate() const {
    return isSpectating; // Vraća status da li igrač gleda
}

// Proverava da li igrač može da prikuplja resurse
bool EliminatedPlayer::canCollect() const {
    return false; // Eliminisani igrači ne mogu da prikupljaju resurse
}

// Funkcija koja briše igrača iz liste aktivnih igrača
//void EliminatedPlayer::deleteFromPlayerList(std::vector<EliminatedPlayer>& playerList, const EliminatedPlayer& player) {
//  playerList.erase(std::move(playerList.begin(), playerList.end(), player), playerList.end());
//}

