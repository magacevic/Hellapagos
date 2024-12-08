#ifndef ELIMINATEDPLAYER_H
#define ELIMINATEDPLAYER_H

#include <vector>

class EliminatedPlayer {
public:
    // Konstruktor
    EliminatedPlayer();

    // Set funkcija za spectating status
    void setSpectating(bool spectate);

    // Funkcija koja proverava da li može da glasa
    bool canVote() const;

    // Funkcija koja proverava da li može da izvodi akcije
    bool canPerformActions() const;

    // Funkcija koja proverava da li želi da gleda igru
    bool wantsToSpectate() const;

    // Funkcija koja proverava da li može da prikuplja resurse
    bool canCollect() const;

    // Funkcija koja briše igrača iz liste aktivnih igrača
    static void deleteFromPlayerList(std::vector<EliminatedPlayer>& playerList, const EliminatedPlayer& player);

private:
    bool isSpectating; // Status da li igrač gleda
};

#endif // ELIMINATEDPLAYER_H


