#ifndef GAMESTATISTICS_H
#define GAMESTATISTICS_H

#include <QString>
#include <QList>
#include <QDateTime>
#include "gameserver.h"
#include "serializable.h"

// Forward declare GameServer
class GameServer;

class GameStatistics : public Serializable {
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

    QVariant toVariant() const override;
    void fromVariant(const QVariant &variant) override;

    void setFileName(const QString& filename);
    QString getFileName() const;
    void setFilePath(const QString& path);
    QString getFilePath() const;
    QList<GameResult> getGameResults() const;

private:
    QList<GameResult> gameResults;
    QString fileName;
    QString filePath;
};

#endif // GAMESTATISTICS_H
