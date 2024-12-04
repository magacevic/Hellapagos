#ifndef COLLECTWOOD_H
#define COLLECTWOOD_H
#include <QObject>

class CollectWood : public QObject
{
    Q_OBJECT
public:
    explicit CollectWood(QObject *parent = nullptr);

    int woodCollected;

    bool playerGotSick;
    int additionalWood;

    void performAction();
    void goToForest(int additionalWood);
    void getCollectedWood();
    void didPlayerGetSick();
    bool checkForSnakeBite(int woodToCollect);
};

#endif // COLLECTWOOD_H
