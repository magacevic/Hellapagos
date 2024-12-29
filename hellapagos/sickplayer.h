#ifndef SICKPLAYER_H
#define SICKPLAYER_H

#include <QObject>


class SickPlayer {
private:
    int recoveryRounds;
    bool recovered;

public:
    SickPlayer();

    void decrementRecovery();

    bool canVote() const;

    bool canPerformActions() const;

    bool isRecovered() const;

    void setRecovered(bool recovered);

};

#endif // SICKPLAYER_H
