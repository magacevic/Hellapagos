#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include"gameserver.h"

class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    ~Server();

    void startServer();
    void onWeatherCardChanged(int newIndex);
    void broadcastWeatherCardChange(int index);
    void onRoundStarted(int roundNumber);
    void broadcastResourcesInfo();
    void onRequestGameStatistics();
    void sendStatsToClient(const QString& statsText);
    
signals:
    // Signal koji obaveštava klijente o trenutnom i narednom igraču
    void currentAndNextPlayerUpdated(const QString& currentPlayer, const QString& nextPlayer);
    // Signal za slanje statistike klijentu
    void sendGameStatisticsToClient(const QString &statsText);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();
    void onCurrentAndNextPlayerUpdated(const QString& currentPlayer, const QString& nextPlayer);

private:
    int maxPlayers;

    QTcpServer *tcpServer;
    QList<QTcpSocket*> clientSockets;     // List of connected clients
    GameServer* gameServer;
    void broadcastMessage(const QString& message);
    int readyPlayers = 0;
    QMap<QString, QTcpSocket*> playerSocketMap;
    bool weatherCardRevealed = false;
};

#endif // SERVER_H
