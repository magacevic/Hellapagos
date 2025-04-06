#ifndef SICKPLAYER_H
#define SICKPLAYER_H

#include <QObject>


class SickPlayer {
public:
    int recoveryRounds;
    bool recovered;

    SickPlayer();

    void decrementRecovery();

    bool canVote() const;

    bool canPerformActions() const;

    bool isRecovered() const;

    void setRecovered(bool recovered);

};

#endif // SICKPLAYER_H
