#ifndef COLLECTFOOD_H
#define COLLECTFOOD_H

#include <QObject>

class CollectFood : public QObject {
    Q_OBJECT

public:
    explicit CollectFood(QObject *parent = nullptr);

    // Metoda za izvođenje akcije sakupljanja hrane
    void performAction();

    // Vraća količinu sakupljene hrane
    int getCollectedFood();

    // Vraća ukupnu količinu hrane
    int getTotalFood();

private:
    int foodCollected;  // Hrana sakupljena u trenutnoj akciji
    int currentFood;    // Ukupna količina hrane koju igrač ima
};

#endif // COLLECTFOOD_H

