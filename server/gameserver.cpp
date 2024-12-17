#include "gameserver.h"
#include <QDebug>

#include <QSet>

GameServer::GameServer(QObject* parent)
    : QObject(parent),roundCounter(1)
{
    roundManager = new RoundManager(this);
    // Initialize resource counts to zero
    resources.insert("food", 0);
    resources.insert("water", 0);
    resources.insert("wood", 0);

    //connect(this, &GameServer::weatherCardChanged, this, &GameServer::broadcastWeatherCardChange);
    connect(roundManager, &RoundManager::currentAndNextPlayerUpdated,
            this, &GameServer::currentAndNextPlayerUpdated);
            
    // Poveži signal za promenu igrača
    connect(roundManager, &RoundManager::playerChanged, this, &GameServer::onPlayerChanged);

    connect(roundManager, &RoundManager::roundCompleted, this, &GameServer::startNextRound);

    for (int i = 1; i < 13; ++i) {
        availableCards.insert(i);
    }
}

RoundManager* GameServer::getRoundManager() const {
    return roundManager;
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


    int randomIndex = *availableCards.begin();
    availableCards.remove(randomIndex);

    emit weatherCardChanged(randomIndex);
}
void GameServer::onWeatherCardRequest() {
      changeWeatherCard();
}

// Slot koji prihvata signal za promenu igrača
void GameServer::onPlayerChanged(int newPlayerId)
{
    // Logika kada se promeni igrač (prihvatamo ID igrača)
   // qDebug() << "Igrač promenjen. Novi igrač ID:" << newPlayerId;

    if (newPlayerId < 0 || newPlayerId >= players.size()) {
        qDebug() << "Invalid player ID";
        return;
    }

    QString currentPlayer = players.at(newPlayerId);
    qDebug() << "Trenutni igrač:" << currentPlayer;

    // Možemo implementirati logiku za sledeće akcije (npr. emitovanje novih podataka o igraču)
    QString nextPlayer = players.at((newPlayerId + 1) % players.size());  // Sledeći igrač
    emit currentAndNextPlayerUpdated(currentPlayer, nextPlayer);
}

void GameServer::startNextRound()
{
    roundCounter++;
    qDebug() << "Starting Round:" << roundCounter;

    emit roundStarted(roundCounter);
}
