#include "roundmanager.h"
#include <QDebug>
RoundManager::RoundManager(QObject* parent) : QObject(parent), currentPlayerIndex(0)
{
}

void RoundManager::addToTurnPlayer(const QString& playerName)
{
    players.append(playerName);  // Dodaj igrača na listu
}

void RoundManager::nextTurn()
{
	if (players.isEmpty()) return;
	
    // Premesti indeks na narednog igrača (cirkularno)
    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();

    // Dohvati trenutnog i narednog igrača
    QString current = currentPlayer();
    QString next = nextPlayer();
    
    qDebug() << "Emitting currentAndNextPlayerUpdated with"
             << "current:" << current << "and next:" << next;

    // Emituj signal sa trenutnim i narednim igračem
    emit currentAndNextPlayerUpdated(current, next);
    
    // Emituj signal sa ID-jem novog igrača
    emit playerChanged(currentPlayerIndex);

    if (currentPlayerIndex == 0) {
        emit roundCompleted(); // Signal that a round is completed
    }

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
