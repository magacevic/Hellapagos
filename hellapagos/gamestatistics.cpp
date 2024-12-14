#include "gamestatistics.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QDir>

GameStatistics::GameStatistics() {
    fileName = "game_results.json";
}

void GameStatistics::addGameResult(const QList<QString>& winners, int totalFood, int totalWater, int totalWood) {

    QDateTime gameDate = QDateTime::currentDateTime();

    GameResult result;
    result.winners = winners;
    result.gameDate = gameDate;
    result.remainingFood = totalFood;
    result.remainingWater = totalWater;
    result.remainingWood = totalWood;

    gameResults.append(result);

    saveToJson(fileName);
}

int GameStatistics::getTotalGamesPlayed() const {
    return gameResults.size();
}

void GameStatistics::saveToJson(const QString& filename) {
    QJsonArray gameResultsArray;

    for (const GameResult& result : gameResults) {
        QJsonObject obj;
        obj["winners"] = QJsonArray::fromStringList(result.winners);
        obj["gameDate"] = result.gameDate.toString(Qt::ISODate);
        obj["remainingFood"] = result.remainingFood;
        obj["remainingWater"] = result.remainingWater;
        obj["remainingWood"] = result.remainingWood;
        gameResultsArray.append(obj);
    }

    QJsonObject root;
    root["gameResults"] = gameResultsArray;

    // upis u fajl
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(root);
        file.write(doc.toJson());
        file.close();
    } else {
        qWarning("Couldn't open file for writing.");
    }
}

void GameStatistics::loadFromJson(const QString& filename) {

    QDir dir = QDir::current();

    // Vracanje dva foldera unazad (fajl nije u build nego u hellapagos folderu)
    dir.cdUp();  // Go up one directory level
    dir.cdUp();  // Go up another directory level

    QString filePath = dir.filePath(filename);

    //qDebug() << "Absolute file path:" << filePath;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open file for reading.");
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        qWarning("Invalid JSON format.");
        return;
    }

    QJsonObject root = doc.object();

    QJsonArray gameResultsArray = root["gameResults"].toArray();

    // brise postojece gameResults
    gameResults.clear();

    for (const QJsonValue& value : gameResultsArray) {
        QJsonObject resultObject = value.toObject();

        GameResult result;

        QJsonArray winnersArray = resultObject["winners"].toArray();
        for (const QJsonValue& winner : winnersArray) {
            result.winners.append(winner.toString());
        }
        result.gameDate = QDateTime::fromString(resultObject["gameDate"].toString(), Qt::ISODate);
        result.remainingFood = resultObject["remainingFood"].toInt();
        result.remainingWater = resultObject["remainingWater"].toInt();
        result.remainingWood = resultObject["remainingWood"].toInt();


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
