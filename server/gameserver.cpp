#include "gameserver.h"
#include <QDebug>
GameServer::GameServer(QObject* parent)
    : QObject(parent)
{
    // Initialize resource counts to zero
    resources.insert("food", 0);
    resources.insert("water", 0);
    resources.insert("wood", 0);
}

void GameServer::addPlayer(const QString& playerName)
{
    if (!players.contains(playerName)) {
        players.append(playerName);
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
