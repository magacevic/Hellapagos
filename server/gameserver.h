#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QObject>
#include <QString>
#include <QMap>
#include "QSet"
#include "roundmanager.h"
#include "voting.h"
#include "gamestatistics.h"


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
    //DODALA
    RoundManager* getRoundManager() const;
    void startNextRound();
    void broadcastFoodUpdate(const QString& playerName, int foodAmount);
    QList<QTcpSocket*> clientSockets;
    void collectFoodFromPlayer(const QString& playerName, int foodAmount);
    void addResource(const QString& resourceName, int amount);
    int getResourceAmount(const QString& resourceName) const;
    void checkRafts();
    void decreaseResources();
    QString getGameStatisticsText() const;
    void updateSickPlayersMap(const QString& playerName);
    void initSickPlayersMap();
    void printSickPlayersMap() const;
public slots:
    // Slot koji prihvata signal za promenu igrača
    void onPlayerChanged(int newPlayerId);
    void onNewClientConnected();
    void onClientDisconnected();
    void onReadyRead();


signals:
    void weatherCardChanged(int newIndex);
    void currentAndNextPlayerUpdated(const QString& currentPlayer, const QString& nextPlayer);
    void roundStarted(int roundNumber);

private:
    QStringList players;      // List of player names
    QMap<QString, int> resources; // Shared resources: e.g., "food", "water", etc.
    int currentWeatherCardIndex = 0; // Default or initial weather card index
    QSet<int> availableCards;  // Set to store available card indices
    RoundManager* roundManager;
    int roundCounter;
    QTcpServer* tcpServer;
    //bool weatherCardRevealed = false; // Flag to track if weather card has been revealed this round
    Voting* votingSystem;
    QMap<QString,int> sickPlayers;
};

#endif // GAMESERVER_H
