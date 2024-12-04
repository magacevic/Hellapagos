#ifndef COLLECTWATER_H
#define COLLECTWATER_H

#include <QObject>
#include <QStackedWidget>
#include <QWidget>

class CollectWater : public QObject
{
    Q_OBJECT

public:
    explicit CollectWater(QObject *parent = nullptr);
    void collectWaterFromCard(QWidget *card);
    int performAction();

private:
    int waterCollected;
    int currentWater;
};

#endif // COLLECTWATER_H
