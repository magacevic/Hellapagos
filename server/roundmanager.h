#ifndef ROUNDMANAGER_H
#define ROUNDMANAGER_H

#include <QObject>
#include <QStringList>


class RoundManager : public QObject
{
    Q_OBJECT

public:
    explicit RoundManager(QObject* parent = nullptr);

    // Dodaj igrača u redosled igre
    void addToTurnPlayer(const QString& playerName);

    // Metoda koja se poziva za premenu igrača
    void nextTurn();

    // Dohvati trenutnog i narednog igrača
    QString currentPlayer() const;
    QString nextPlayer() const;
    int getCurrentPlayerIndex();
    void actionCompleted();

    void setLastRound(bool isLast);
    bool getLastRound() const;
    void firstTurn();

signals:
    // Signal koji obaveštava server kada se promeni trenutni i naredni igrač
    void currentAndNextPlayerUpdated(const QString& currentPlayer, const QString& nextPlayer);
    
    // Signal koji obaveštava kada se igrač promeni
    void playerChanged(int newPlayerId);
    void roundCompleted();
    //void changeWeatherCard();

    //Signal za azuriranje liste igraca
    void playerListUpdated(const QVector<QString>& updatePlayers);
    void lastRoundStarted();
    void lastRoundEnded();

public slots:
    void handlePlayerEliminated(const QString& playerName);  // Rukuje eliminacijom igraca

private:
    QStringList players;      // List of player names
    int currentPlayerIndex;   // Trenutni indeks igrača
    bool isLastRound;

};

#endif // ROUNDMANAGER_H
