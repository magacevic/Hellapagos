#include "gamewindow.h"
#include "ui_gamewindow.h"

/*GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameWindow)
{
    ui->setupUi(this);
}
*/
GameWindow::GameWindow(Player *player, QWidget *parent)
    : QMainWindow(parent), currentPlayer(player), ui(new Ui::GameWindow)
{
    ui->setupUi(this);  // Always call setupUi in the constructor
    qDebug() << "Player's Name in GameWindow:" << currentPlayer->getName();

    // Optionally, display player's name on a QLabel or similar widget in the UI
    ui->playerNameLabel->setText(currentPlayer->getName());
}

GameWindow::~GameWindow()
{
    delete ui;
}
