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


void Voting::reset()
{
    votes.clear();
}
int Voting::getVoteCountFor(const QString& player) const
{
    int count = 0;
    for (const QString& votedFor : votes.values()) {
        if (votedFor == player) {
            ++count;
        }
    }
    return count;
}
QStringList Voting::getPlayersWithMostVotes() const {
    QMap<QString, int> voteCounts;
    for (const QString& target : votes.values()) {
        if (!target.isEmpty()) {
            voteCounts[target]++;
        }
    }

    int maxVotes = -1;
    QStringList topCandidates;

    for (auto it = voteCounts.begin(); it != voteCounts.end(); ++it) {
        if (it.value() > maxVotes) {
            maxVotes = it.value();
            topCandidates = { it.key() };
        } else if (it.value() == maxVotes) {
            topCandidates.append(it.key());
        }
    }

    return topCandidates;
}
