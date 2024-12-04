#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <QRandomGenerator>
#include <QInputDialog>
#include <QMessageBox>
#include <QTimer>

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
    connect(ui->collectWaterButton, &QPushButton::clicked, this, &GameWindow::on_collectWaterButton_clicked);



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

void GameWindow::on_collectWoodButton_clicked()
{
    // Always collect 1 wood
    collectWood.performAction();  // Collect 1 wood

    // Ask the player if they want to collect more wood
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Go To Forest",
                                  "You have collected 1 wood. Do you want to go to the forest and collect more?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Ask how many more pieces of wood they want to collect (1-5)
        bool ok;
        int additionalWood = QInputDialog::getInt(this, "Collect More",
                                                  "Select number of wood (1-5):",
                                                  1, 1, 5, 1, &ok);

        if (ok) {
            // Perform action based on the number of additional wood the player wants to collect
            collectWood.goToForest(additionalWood);
        }
    }
    // Total collected wood
    collectWood.getCollectedWood();
}


void GameWindow::on_collectWaterButton_clicked()
{

    if (isCollectingWater) {
        qDebug() << "Already collecting water, ignoring the click."; // Debugging
        return;
    }

    isCollectingWater = true;
    qDebug() << "Started collecting water.";  // Debugging

     ui->collectWaterButton->setEnabled(false);

    QWidget *currentCard = ui->weatherCards->currentWidget();

    if (currentCard) {
        collectWater.collectWaterFromCard(currentCard);

        qDebug() << "- Total collected water:" << collectWater.performAction();
    } else {
        qDebug() << "No active card available to collect water from!";
    }

    // Re-enable the button after a short delay and reset the flag
    QTimer::singleShot(500, this, [this]() {
        ui->collectWaterButton->setEnabled(true);  // Re-enable the button
        isCollectingWater = false;  // Reset the flag
    });
}
