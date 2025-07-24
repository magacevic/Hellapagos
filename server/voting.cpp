#include "voting.h"
#include <QDebug>

Voting::Voting(QObject* parent) : QObject(parent) {}

void Voting::startVote(const QSet<QString>& activePlayers)
{
    votes.clear();
    for (const QString& player : activePlayers) {
        votes[player] = "";  // Reset votes
    }

    emit votingStarted(activePlayers);
}

void Voting::submitVote(const QString& voter, const QString& target)
{
    if (votes.contains(voter)) {
        votes[voter] = target;
        qDebug() << voter << "voted for" << target;
    }
}

bool Voting::allVotesIn(const QStringList& currentPlayers) const
{
    for (const QString& player : currentPlayers) {
        if (!votes.contains(player) || votes[player].isEmpty()) {
            return false;
        }
    }
    return true;
}

QString Voting::getPlayerWithMostVotes() const
{
    QMap<QString, int> voteCounts;
    for (const QString& target : votes.values()) {
        if (!target.isEmpty()) {
            voteCounts[target]++;
        }
    }

    // Find player with max votes
    QString maxPlayer;
    int maxVotes = -1;
    for (auto it = voteCounts.begin(); it != voteCounts.end(); ++it) {
        if (it.value() > maxVotes) {
            maxVotes = it.value();
            maxPlayer = it.key();
        }
    }

    return maxPlayer;
}

void Voting::reset()
{
    votes.clear();
}
