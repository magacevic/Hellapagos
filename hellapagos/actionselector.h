#ifndef ACTIONSELECTOR_H
#define ACTIONSELECTOR_H

#include <QObject>
#include <QWidget>

// Enum za različite tipove akcija
enum ActionType {
    COLLECT_FOOD = 0,
    COLLECT_WATER = 1,
    COLLECT_WOOD = 2
};

class Player;


class ActionSelector : public QObject
{
    Q_OBJECT

public:
    // Konstruktor koji prihvata postojeceg igraca.
    explicit ActionSelector(Player *player, QObject *parent = nullptr);
    ~ActionSelector();

    // Metod za biranje akcije
    void selectAction(ActionType actionType, QWidget *card = nullptr);

private:
    Player *player;

};

#endif // ACTIONSELECTOR_H
