#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include "player.h"
#include "client.h"
namespace Ui {
class GameWindow;
}

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(Player *player,Client *client,QWidget *parent = nullptr);
    ~GameWindow();

    void updateWeatherCard(int index);
    void onWeatherCardClicked();

private:
    Ui::GameWindow *ui;
    Player *currentPlayer;
    Client* m_client;
};

#endif // GAMEWINDOW_H
