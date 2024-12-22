#ifndef ELIMINATEDPLAYER_H
#define ELIMINATEDPLAYER_H

#include <vector>
#include <QString>

class EliminatedPlayer {
public:
    // Konstruktor
    EliminatedPlayer(const QString& name = "");


    void setSpectating(bool spectate);  // Postavlja igraca u rezim gledanja
    bool canVote() const;              // Funkcija koja proverava da li može da glasa
    bool canPerformActions() const;    // Funkcija koja proverava da li može da izvodi akcije
    bool wantsToSpectate() const;     // Funkcija koja proverava da li želi da gleda igru
    bool canCollect() const;         // Funkcija koja proverava da li može da prikuplja resurse

    QString getPlayerName() const;   //Funkcija za dobijanje imena igraca


private:
    bool isSpectating; // Status da li igrač gleda
    QString playerName; //Ime eliminisanog igraca
};

#endif // ELIMINATEDPLAYER_H


