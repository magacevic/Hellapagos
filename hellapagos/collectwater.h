#ifndef COLLECTWATER_H
#define COLLECTWATER_H

#include <QObject>
#include <QStackedWidget>
#include <QWidget>

class CollectWater : public QObject
{
    Q_OBJECT

public:
    explicit CollectWater(QStackedWidget *weatherCards, QObject *parent = nullptr);

    void collectWaterFromCard(QWidget *card);
    int getCollectedWater() const;

private:
    QStackedWidget *weatherCards; // Promenjen tip na QStackedWidget
    int waterCollected;
};

#endif // COLLECTWATER_H
