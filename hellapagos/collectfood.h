#ifndef COLLECTFOOD_H
#define COLLECTFOOD_H

#include <QObject>

class CollectFood : public QObject {
    Q_OBJECT

public:
    explicit CollectFood(QObject *parent = nullptr);

    // Metoda za izvođenje akcije sakupljanja hrane
    int performAction();

private:
    int foodCollectedInRound;  // Hrana sakupljena u trenutnoj akciji
};

#endif // COLLECTFOOD_H

