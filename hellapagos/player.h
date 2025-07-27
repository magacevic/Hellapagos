#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QWidget>
#include <QObject>
#include "collectfood.h"
#include "collectwater.h"
#include "collectwood.h"
#include "eliminatedplayer.h"
#include "sickplayer.h"
#include "client.h"
class ActionSelector;

class Player: public QObject {
    Q_OBJECT
public:
    Player(const QString& name, Client* client); // Konstruktor

    // Geteri i Seteri
    QString getName() const;
    void setName(const QString& name);

    bool isAlive() const;
    void setAlive(bool alive);

    bool isSick() const;
    void setSick(bool sick);
    
    bool canPerformActions();
    bool canVote() const;

    // Upravljanje hranom
    int getTotalFood() const;
    void addFood(int food);

    // Upravljanje vodom
    int getTotalWater() const;
    void addWater(int water);
    void resetWood();
    // Upravljanje drvima
    int getTotalWood() const;
    void addWood(int wood);

    // Metode za pozivanje akcija
    void collectFood();
    void collectWater(QWidget *card);
    void collectWood();
    void collectAdditionalWood(int trips);
    void raftCreated();
    void recover();

    // Metoda za eliminaciju igrača
    void eliminate();
    // Getter za eliminisane igrače
    EliminatedPlayer* getEliminatedPlayer() const;
    void reportResourcesToServer(int newlyCollectedFood, int newlyCollectedWater);


private:
    QString playerName;
    bool alive;
    bool sick;

    int totalFood;
    int foodCollectedInRound;
    int totalWater;
    int totalWood;
    int tWater=0;
    int tFood=0;
    SickPlayer* sickPlayer;

    QWidget getCurrentCard();

    CollectFood *collectFoodAction;
    CollectWater *collectWaterAction;
    CollectWood *collectWoodAction;

    EliminatedPlayer* eliminatedPlayer;
    Client* m_client = nullptr;

private slots:
    void onPlayerGotBitten();
};

#endif // PLAYER_H
