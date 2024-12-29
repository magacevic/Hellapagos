#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QString>  // Include QString to handle names
#include "eliminatedplayer.h"
#include "sickplayer.h"

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);
    ~Client();

    void sendMessage(const QString &message);

    void connectToServer();
    void sendJoinCommand(const QString &playerName); // New function
    void sendReadySignal();
    void sendWeatherCardChange(int index);
    void sendWeatherCardRequest();
    void addPlayer(SickPlayer* player1);
    // void sendMessage(const QString &message);
    // In client.h

signals:
    void serverFullSignal();  // Signal to notify the server is full
    void allPlayersReady();
    void weatherCardUpdated(int newIndex);
    void collectWaterApproved();
    void collectWoodApproved();
    void collectFoodApproved();
    void gameStatsReceived(const QString &stats);


private slots:
    void onConnected();
    void onReadyRead();
    void onDisconnected();
    //void onPlayerEliminated(const QString& playerName); // Slot za prijem eliminisanog igraca


private:
    QTcpSocket *tcpSocket;

    QList<SickPlayer*> players;

    EliminatedPlayer* player;  // Instanca eliminisanog igraca

};

#endif // CLIENT_H
