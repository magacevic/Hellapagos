#ifndef SICKPLAYER_H
#define SICKPLAYER_H

#include <QObject>


class SickPlayer {
private:
    // Number of rounds until the player recovers
    int recoveryRounds;

public:
    SickPlayer();

    // Reduces the number of remaining rounds for recovery
    void decrementRecovery();

    // Check if the player can vote
    bool canVote() const;

    // Check if the player can perform actions
    bool canPerformActions() const;

    // Check if the player has recovered
    bool isRecovered() const;
};

#endif // SICKPLAYER_H
