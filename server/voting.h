#ifndef VOTING_H
#define VOTING_H

#include <QString>
#include <QMap>
#include <QSet>
#include <QObject>

class Voting : public QObject {
    Q_OBJECT

public:
    explicit Voting(QObject* parent = nullptr);

    void startVote(const QSet<QString>& activePlayers);
    void submitVote(const QString& voter, const QString& target);
    bool allVotesIn(const QStringList& currentPlayers) const;
    QString getPlayerWithMostVotes() const;
    void reset();
    int getVoteCountFor(const QString& player) const;
    QStringList getPlayersWithMostVotes() const;



private:
    QMap<QString, QString> votes;  // voter -> target

signals:
    void votingStarted(const QSet<QString>& eligiblePlayers);
};

#endif // VOTING_H
