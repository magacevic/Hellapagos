#include "voting.h"
#include <QDebug>

Voting::Voting(QObject* parent) : QObject(parent), votingInProgress(false)
{
}

void Voting::startVoting()
{
    if (votingInProgress) {
        qWarning() << "Voting is already in progress!";
        return;
    }

    votingInProgress = true;
    emit votingStarted();
    qDebug() << "Voting started!";
    votes.clear();
}

void Voting::concludeVoting()
{
    if (!votingInProgress) {
        qWarning() << "No voting in progress!";
        return;
    }

    votingInProgress = false;
    emit votingConcluded();
    qDebug() << "Voting concluded!";
    handleVotingOutcome();
}

void Voting::resetVoting()
{
    votes.clear();
    votingInProgress = false;
    eliminatePlayer.clear();
    reason.clear();
    qDebug() << "Voting reset.";
}

void Voting::castVote(const QString& playerName)
{
    if (!votingInProgress) {
        qWarning() << "Voting is not in progress!";
        return;
    }

    if (!players.contains(playerName)) {
        qWarning() << "Player not in the voting list!";
        return;
    }

    if (votes.contains(playerName)) {
        votes[playerName] += 1;
    } else {
        votes[playerName] = 1;
    }

    qDebug() << "Vote casted for: " << playerName;
}

void Voting::checkForWaterShortage()
{
    qDebug() << "Checking for water shortage...";
}

void Voting::checkForFoodShortage()
{
    qDebug() << "Checking for food shortage...";
}

void Voting::handleVotingOutcome()
{
    if (votes.isEmpty()) {
        qWarning() << "No votes casted!";
        return;
    }

    QString playerToEliminate;
    int highestVotes = -1;

    for (auto it = votes.begin(); it != votes.end(); ++it) {
        if (it.value() > highestVotes) {
            highestVotes = it.value();
            playerToEliminate = it.key();
        }
    }

    eliminatePlayer = playerToEliminate;
    reason = "Eliminated due to majority vote.";
    qDebug() << "Player to eliminate: " << eliminatePlayer << " with " << highestVotes << " votes.";
}
