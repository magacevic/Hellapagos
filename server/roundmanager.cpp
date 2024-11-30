#include "roundmanager.h"
#include <QDebug>  // Za ispisivanje u terminalu

RoundManager::RoundManager(QObject* parent) : QObject(parent), currentPlayerIndex(0)
{
}

void RoundManager::addToTurnPlayer(const QString& playerName)
{
    players.append(playerName);  // Dodaj igrača na listu
}

void RoundManager::nextTurn()
{
    // Premesti indeks na narednog igrača (cirkularno)
    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();

    // Dohvati trenutnog i narednog igrača
    QString current = currentPlayer();
    QString next = nextPlayer();

    // Emituj signal sa trenutnim i narednim igračem
    emit currentAndNextPlayerUpdated(current, next);
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
