#ifndef SICKPLAYER_H
#define SICKPLAYER_H

#include "collectwood.h"
#include <QObject>
#include <QWidget>

class sickPlayer : public CollectWood
{
    Q_OBJECT
public:
    explicit sickPlayer(QObject *parent = nullptr);

    // Methods for player actions
    void enterForest(bool collectAdditional, int additionalWood = 0);
    void inspectWreck();
    void displayStatus();

private:
    bool isSick;                  // Tracks if the player is sick
    int safeWood;                 // Wood that is always retained
    QVector<int> wreckCards;      // Cards collected from inspecting the wreck

    void checkRaftCreation();     // Check if a raft can be created
};

#endif // SICKPLAYER_H
