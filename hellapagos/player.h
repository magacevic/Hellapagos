#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QWidget>
#include "collectfood.h"
#include "collectwater.h"
#include "collectwood.h"

class ActionSelector;

class Player {
public:
    Player(const QString& name); // Konstruktor

    // Geteri i Seteri
    QString getName() const;
    void setName(const QString& name);

    bool isAlive() const;
    void setAlive(bool alive);

    bool isSick() const;
    void setSick(bool sick);

    // Upravljanje hranom
    int getTotalFood() const;
    void addFood(int food);

    // Upravljanje vodom
    int getTotalWater() const;
    void addWater(int water);

    // Metode za pozivanje akcija
    void collectFood();
    void collectWater(QWidget *card);

private:
    QString playerName;
    bool alive;
    bool sick;

    int totalFood;
    int foodCollectedInRound;
    int totalWater;

    QWidget getCurrentCard();

    CollectFood *collectFoodAction;
    CollectWater *collectWaterAction;
    CollectWood *collectWoodAction;

};

#endif // PLAYER_H
