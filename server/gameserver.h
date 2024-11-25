#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QObject>
#include <QString>
#include <QMap>

class GameServer : public QObject
{
    Q_OBJECT

public:
    explicit GameServer(QObject* parent = nullptr);

    void addPlayer(const QString& playerName);
    QStringList getPlayers() const;
    QMap<QString, int> getResources() const;

private:
    QStringList players;      // List of player names
    QMap<QString, int> resources; // Shared resources: e.g., "food", "water", etc.
};

#endif // GAMESERVER_H
