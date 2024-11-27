#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <QRandomGenerator>
/*GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameWindow)
{
    ui->setupUi(this);
}
*/
GameWindow::GameWindow(Player *player,Client *client, QWidget *parent)
    : QMainWindow(parent), currentPlayer(player),m_client(client), ui(new Ui::GameWindow)
{
    ui->setupUi(this);  // Always call setupUi in the constructor
    qDebug() << "Player's Name in GameWindow:" << currentPlayer->getName();

    // Optionally, display player's name on a QLabel or similar widget in the UI
    ui->playerNameLabel->setText(currentPlayer->getName());
    connect(m_client, &Client::weatherCardUpdated, this, &GameWindow::updateWeatherCard);
    connect(ui->revealButton, &QPushButton::clicked, this, &GameWindow::onWeatherCardClicked);


}

GameWindow::~GameWindow()
{
    delete ui;
}
void GameWindow::updateWeatherCard(int index)
{
   /* if (index < 0 || index >= ui->weatherCards->count()) {
        qDebug() << "Invalid weather card index:" << index;
        return;
    }*/
    qDebug() << "Revealing weather card with index:" << index;

    ui->weatherCards->setCurrentIndex(index);
}

void GameWindow::onWeatherCardClicked()
{
    if (ui->weatherCards->count() <= 1) {
        qDebug() << "Not enough weather cards to reveal.";
        return; // Exit early if there are not enough cards
    }
   /* qDebug()<< "Num cards:"<< ui->weatherCards->count();
    int randomIndex = QRandomGenerator::global()->bounded(1, ui->weatherCards->count()); // Avoid first and last
    qDebug() << "Revealing weather card with index:" << randomIndex;


    ui->weatherCards->setCurrentIndex(randomIndex);

    if (m_client) {
        m_client->sendWeatherCardChange(randomIndex);
    } else {
        qDebug() << "Client object is null!";
    }*/

    qDebug() << "Requesting card reveal from server";
    if (m_client) {
        m_client->sendWeatherCardRequest();  // Placeholder; server will decide the actual card
    } else {
        qDebug() << "Client object is null!";
    }
    // Notify server
   // m_client->sendWeatherCardChange(randomIndex);
}
