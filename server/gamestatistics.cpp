#include "gamestatistics.h"
#include "gameserver.h"
#include "jsonserializer.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QDir>


GameStatistics::GameStatistics() {
    fileName = "game_results.json";
    filePath = "";
}

void GameStatistics::addGameResult(const QList<QString>& winners, GameServer* gameServer) {

    QDateTime gameDate = QDateTime::currentDateTime();

    QMap<QString, int> resources = gameServer->getResources();
    int totalFood = resources.value("food", 0);
    int totalWater = resources.value("water", 0);
    int totalWood = resources.value("wood", 0);

    GameResult result;
    result.winners = winners;
    result.gameDate = gameDate;
    result.remainingFood = totalFood;
    result.remainingWater = totalWater;
    result.remainingWood = totalWood;

    gameResults.append(result);

    JSONSerializer jsonSerializer;
    jsonSerializer.save(*this, filePath);
}

int GameStatistics::getTotalGamesPlayed() const {
    return gameResults.size();
}

QVariant GameStatistics::toVariant() const
{
    QVariantMap map;

    map.insert("fileName", fileName);

    // Pravimo listu rezultata
    QVariantList gameResultsList;
    for (const GameResult& result : gameResults) {
        QVariantMap resultMap;
        resultMap.insert("winners", QVariant::fromValue(result.winners));
        resultMap.insert("gameDate", result.gameDate.toString(Qt::ISODate));
        resultMap.insert("remainingFood", result.remainingFood);
        resultMap.insert("remainingWater", result.remainingWater);
        resultMap.insert("remainingWood", result.remainingWood);
        gameResultsList.append(resultMap);
    }

    map.insert("gameResults", gameResultsList);

    return map;
}

void GameStatistics::fromVariant(const QVariant &variant)
{
    const QVariantMap map = variant.toMap();

    // Učitavamo naziv fajla
    fileName = map.value("fileName").toString();

    // Učitavamo rezultate igre
    const QVariantList gameResultsList = map.value("gameResults").toList();
    gameResults.clear();  // Čistimo postojeće rezultate

    for (const QVariant& resultVariant : gameResultsList) {
        const QVariantMap resultMap = resultVariant.toMap();

        GameResult result;
        result.winners = resultMap.value("winners").toStringList();
        result.gameDate = QDateTime::fromString(resultMap.value("gameDate").toString(), Qt::ISODate);
        result.remainingFood = resultMap.value("remainingFood").toInt();
        result.remainingWater = resultMap.value("remainingWater").toInt();
        result.remainingWood = resultMap.value("remainingWood").toInt();

        gameResults.append(result);
    }
}

QList<GameStatistics::GameResult> GameStatistics::getGameResults() const {
    return gameResults;
}

void GameStatistics::setFileName(const QString& filename) {
    fileName = filename;
}

QString GameStatistics::getFileName() const {
    return fileName;
}

void GameStatistics::setFilePath(const QString& path) {
    // QDir objekat za trenutni direktorijum
    QDir dir = QDir::current();

    // Pomeranje do server foldera (trenutno je build)
    dir.cdUp();
    dir.cdUp();

    // putanja
    filePath = dir.filePath(fileName);
    //qDebug() << "set file path:" << filePath;
}

QString GameStatistics::getFilePath() const {
    return filePath;
}




