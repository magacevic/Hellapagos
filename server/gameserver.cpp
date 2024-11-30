#include "gameserver.h"
#include <QDebug>

#include <QSet>

GameServer::GameServer(QObject* parent)
    : QObject(parent)
{
    roundManager = new RoundManager(this);
    // Initialize resource counts to zero
    resources.insert("food", 0);
    resources.insert("water", 0);
    resources.insert("wood", 0);

    //connect(this, &GameServer::weatherCardChanged, this, &GameServer::broadcastWeatherCardChange);
    connect(roundManager, &RoundManager::currentAndNextPlayerUpdated,
            this, &GameServer::currentAndNextPlayerUpdated);

    for (int i = 1; i < 13; ++i) {
        availableCards.insert(i);
    }
}

void GameServer::addPlayer(const QString& playerName)
{
    if (!players.contains(playerName)) {
        players.append(playerName);
        roundManager->addToTurnPlayer(playerName);
        qDebug() << "Player added:" << playerName;
    } else {
        qDebug() << "Player" << playerName << "already exists!";
    }
}

QStringList GameServer::getPlayers() const
{
    return players;
}

QMap<QString, int> GameServer::getResources() const
{
    return resources;
}
/*
void GameServer::changeWeatherCard(int newIndex)
{
    currentWeatherCardIndex = newIndex;
    emit weatherCardChanged(newIndex); // Signal for serverApp to notify clients
}
*/
int GameServer::getWeatherCardIndex() const
{
    return currentWeatherCardIndex;
}

void GameServer::changeWeatherCard() {
    if (availableCards.isEmpty()) {
        qDebug() << "No cards left to reveal!";
        return;
    }

    // Pick a random card from the set
    int randomIndex = *availableCards.begin();
    availableCards.remove(randomIndex);  // Remove the chosen card

    emit weatherCardChanged(randomIndex);  // Notify the clients with the revealed card
}
void GameServer::onWeatherCardRequest() {
    // Trigger the card reveal logic when the client requests it
    changeWeatherCard();
}
