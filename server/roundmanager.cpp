#include "roundmanager.h"
#include <QDebug>
#include <QTimer>

RoundManager::RoundManager(QObject* parent) : QObject(parent), currentPlayerIndex(0),isLastRound(false)
{
}

void RoundManager::addToTurnPlayer(const QString& playerName)
{
    players.append(playerName);  // Dodaj igrača na listu
}

void RoundManager::setLastRound(bool lastRound)
{
    isLastRound = lastRound;
    if (isLastRound) {
        qDebug() << "Last round initiated!";
        emit lastRoundStarted();
    }
}

bool RoundManager::getLastRound() const
{
    return isLastRound;
}

void RoundManager::nextTurn()
{
	if (players.isEmpty()) return;
	
   currentPlayerIndex = (currentPlayerIndex + 1) % players.size();

    QString current = currentPlayer();
    QString next = nextPlayer();
    
   emit currentAndNextPlayerUpdated(current, next);
    
    // Emituj signal sa ID-jem novog igrača
    emit playerChanged(currentPlayerIndex);


}

void RoundManager::firstTurn()
{
    if (players.isEmpty()) return;
    QString current = currentPlayer();
    QString next = nextPlayer();
    emit currentAndNextPlayerUpdated(current, next);

    // Emituj signal sa ID-jem novog igrača
    emit playerChanged(currentPlayerIndex);
}


QString RoundManager::currentPlayer() const
{
    return players.at(currentPlayerIndex);
}

QString RoundManager::nextPlayer() const
{
    int nextIndex = (currentPlayerIndex + 1) % players.size();
    return players.at(nextIndex);
}
int RoundManager::getCurrentPlayerIndex() {
    return currentPlayerIndex; // Replace with your actual logic
}


void RoundManager::actionCompleted()
{
    static int completedTurns = 0;
    completedTurns++;

    qDebug() << "Player turn completed. Total completed turns:" << completedTurns;

     if (completedTurns >= players.size()) {

        qDebug() << "All players completed their actions. Starting next round.";

        emit roundCompleted();
        completedTurns = 0;
    }

     if (isLastRound && completedTurns == 0) {
         emit lastRoundEnded();
     }


     QTimer::singleShot(500, this, [this]() {
         nextTurn();
     });

}

void RoundManager::handlePlayerEliminated(const QString& playerName)
{
    players.removeAll(playerName);
    qDebug() << "Player eliminated:" << playerName;

    emit playerListUpdated(players);
}

void RoundManager::removeFromTurnPlayer(const QString& playerName)
{
    players.removeAll(playerName);
    qDebug() << "Removed player from turn list:" << playerName;

    // Opcionalno, možeš emitovati signal ako želiš da obavestiš ostale delove sistema
    emit playerListUpdated(players);

    // Ako je potrebno, možeš da prilagodiš currentPlayerIndex ako je eliminisani igrač onaj na redu
    if (players.isEmpty()) {
        currentPlayerIndex = 0;
    } else if (currentPlayerIndex >= players.size()) {
        currentPlayerIndex = currentPlayerIndex % players.size();
    }
}




