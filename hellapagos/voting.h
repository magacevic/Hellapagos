#ifndef VOTING_H
#define VOTING_H

#include <QObject>
#include <QMap>
#include <QString>
#include "player.h"

class Voting : public QObject
{
    Q_OBJECT


public:
    explicit Voting(QObject *parent = nullptr);
    ~Voting();

    void startVoting();
    void concludeVoting();
    void resetVoting();
    void castVote(Player* player, int vote);
    void checkForWaterShortage();
    void checkForFoodShortage();
    Player* getEliminatedPlayer() const;
    bool isVotingInProgress() const;
    bool votingInProgress;
    void on_castVoteButton_clicked();
    void resetVotes();

signals:
    void votingStarted();
    void votingConcluded();

private:
    QMap<Player*, int> votes;
    Player* eliminatedPlayer = nullptr;
    QString reason;
    bool isVoteValid(Player* player);
};

#endif // VOTING_H
