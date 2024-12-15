#include "voting.h"
#include <QDebug>

Voting::Voting(QObject *parent) : QObject(parent) {
}

Voting::~Voting() {
}

void Voting::startVoting() {
    if (votingInProgress) {
        qDebug() << "Voting is already in progress.";
        return;
    }
    votingInProgress = true;
    resetVotes();
    emit votingStarted();
}

void Voting::concludeVoting() {
    if (!votingInProgress) {
        qDebug() << "Voting has not started.";
        return;
    }

    // naci igraca sa najvise glasova
    Player* mostVotedPlayer = nullptr;
    int maxVotes = 0;

    for (auto it = votes.begin(); it != votes.end(); ++it) {
        if (it.value() > maxVotes) {
            maxVotes = it.value();
            mostVotedPlayer = it.key();
        }
    }

    if (mostVotedPlayer) {
        eliminatedPlayer = mostVotedPlayer;
        qDebug() << "Player eliminated:" << eliminatedPlayer->getName();
    } else {
        qDebug() << "No one was eliminated.";
    }

    votingInProgress = false;
    emit votingConcluded();
}

void Voting::resetVoting() {
    votes.clear();
    eliminatedPlayer = nullptr;
    reason.clear();
}

void Voting::castVote(Player* player, int vote) {
    if (!votingInProgress) {
        qDebug() << "Voting is not in progress.";
        return;
    }

    if (isVoteValid(player)) {
        if (votes.contains(player)) {
            qDebug() << player->getName() << "has already voted.";
            return;
        }
        votes[player] = vote;
        qDebug() << player->getName() << "cast a vote: " << vote;
    } else {
        qDebug() << "Invalid vote from player: " << player->getName();
    }
}

void Voting::checkForWaterShortage() {
    reason = "Water Shortage";
    startVoting();
}

void Voting::checkForFoodShortage() {
    reason = "Food Shortage";
    startVoting();
}

bool Voting::isVoteValid(Player* player) {
    return player != nullptr;
}


Player* Voting::getEliminatedPlayer() const {
    return eliminatedPlayer;
}


bool Voting::isVotingInProgress() const {
    return votingInProgress;
}

void Voting::on_castVoteButton_clicked() {
    qDebug() << "Vote button clicked";
}

void Voting::resetVotes() {
    qDebug() << "Votes have been reset";
}


