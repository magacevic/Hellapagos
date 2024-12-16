#ifndef SICKPLAYER_H
#define SICKPLAYER_H

#include <QObject>


class SickPlayer {
private:
    int recoveryRounds;

public:
    SickPlayer();

    void decrementRecovery();

    bool canVote() const;

    bool canPerformActions() const;

    bool isRecovered() const;
};

#endif // SICKPLAYER_H
