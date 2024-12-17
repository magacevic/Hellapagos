#ifndef VOTING_H
#define VOTING_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>

class Voting : public QObject
{
    Q_OBJECT

public:
    explicit Voting(QObject* parent = nullptr);

    void startVoting();
    void concludeVoting();
    void resetVoting();
    void castVote(const QString& playerName);
    void checkForWaterShortage();
    void checkForFoodShortage();

signals:
    void votingStarted();
    void votingConcluded();

private:
    QMap<QString, int> votes;
    QString eliminatePlayer;
    bool votingInProgress;
    QString reason;
    QStringList players;

    void handleVotingOutcome();
};

#endif // VOTING_H
