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

signals:
    // Signal koji obaveštava server kada se promeni trenutni i naredni igrač
    void currentAndNextPlayerUpdated(const QString& currentPlayer, const QString& nextPlayer);
    
    // Signal koji obaveštava kada se igrač promeni
    void playerChanged(int newPlayerId);

private:
    QStringList players;      // List of player names
    int currentPlayerIndex;   // Trenutni indeks igrača
};

#endif // ROUNDMANAGER_H
