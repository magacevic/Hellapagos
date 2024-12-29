#ifndef GAMESTATISTICS_H
#define GAMESTATISTICS_H

#include <QString>
#include <QList>
#include <QDateTime>
#include "gameserver.h"

// Forward declare GameServer
class GameServer;

class GameStatistics {
public:
    struct GameResult {
        QList<QString> winners;
        QDateTime gameDate;
        int remainingFood;
        int remainingWater;
        int remainingWood;
    };

    GameStatistics();

    void addGameResult(const QList<QString>& winners, GameServer* gameServer);
    int getTotalGamesPlayed() const;
    void saveToJson(const QString& filename);
    void loadFromJson(const QString& filename);
    void setFileName(const QString& filename);
    QString getFileName() const;
    QList<GameResult> getGameResults() const;

private:
    QList<GameResult> gameResults;
    QString fileName;
};

#endif // GAMESTATISTICS_H
