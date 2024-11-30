#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QObject>
#include <QString>
#include <QMap>
#include "QSet"
#include "roundmanager.h"


class GameServer : public QObject
{
    Q_OBJECT

public:
    explicit GameServer(QObject* parent = nullptr);

    void addPlayer(const QString& playerName);
    QStringList getPlayers() const;
    QMap<QString, int> getResources() const;
    void changeWeatherCard();
    int getWeatherCardIndex() const;
    void onWeatherCardRequest();

signals:
    void weatherCardChanged(int newIndex);
    void currentAndNextPlayerUpdated(const QString& currentPlayer, const QString& nextPlayer);

private:
    QStringList players;      // List of player names
    QMap<QString, int> resources; // Shared resources: e.g., "food", "water", etc.
    int currentWeatherCardIndex = 0; // Default or initial weather card index
    QSet<int> availableCards;  // Set to store available card indices
    RoundManager* roundManager;
};

#endif // GAMESERVER_H
