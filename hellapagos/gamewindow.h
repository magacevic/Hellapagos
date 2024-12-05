#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include "player.h"
#include "client.h"
#include "collectwood.h"
#include "collectwater.h"
#include "collectfood.h"
#include "actionselector.h"

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

private slots:
    void on_collectWoodButton_clicked();
    void on_collectWaterButton_clicked();
    void on_collectFoodButton_clicked();

private:
    Ui::GameWindow *ui;
    Player *currentPlayer;
    Client* m_client;
    CollectWood collectWood;
    CollectWater collectWater;
    CollectFood collectFood;
    ActionSelector *actionSelector;
    bool isCollectingWater = false;
    bool isCollectingFood = false;
};

#endif // GAMEWINDOW_H
