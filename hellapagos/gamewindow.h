#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include "player.h"
namespace Ui {
class GameWindow;
}

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(Player *player,QWidget *parent = nullptr);
    ~GameWindow();

private:
    Ui::GameWindow *ui;
    Player *currentPlayer;
};

#endif // GAMEWINDOW_H
