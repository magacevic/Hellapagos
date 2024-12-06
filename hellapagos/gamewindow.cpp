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
GameWindow::GameWindow(Player *player,Client *client,QWidget *parent)
    : QMainWindow(parent), currentPlayer(player),m_client(client), ui(new Ui::GameWindow)
{
    ui->setupUi(this);  // Always call setupUi in the constructor
    qDebug() << "Player's Name in GameWindow:" << currentPlayer->getName();
    actionSelector = new ActionSelector(currentPlayer, this);


    voting = new Voting(this);

    // Connect signals related to voting
    connect(voting, &Voting::votingStarted, this, &GameWindow::onVotingStarted);
    connect(voting, &Voting::votingConcluded, this, &GameWindow::onVotingConcluded);
    connect(ui->castVoteButton, &QPushButton::clicked, voting, &Voting::on_castVoteButton_clicked);



    // Optionally, display player's name on a QLabel or similar widget in the UI
    ui->playerNameLabel->setText(currentPlayer->getName());
    connect(m_client, &Client::weatherCardUpdated, this, &GameWindow::updateWeatherCard);
    connect(ui->revealButton, &QPushButton::clicked, this, &GameWindow::onWeatherCardClicked);
    connect(ui->collectWaterButton, &QPushButton::clicked, this, &GameWindow::on_collectWaterButton_clicked);
    connect(ui->collectFoodButton, &QPushButton::clicked, this, &GameWindow::on_collectFoodButton_clicked);



}

GameWindow::~GameWindow()
{
    delete ui;
    delete actionSelector;
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

     ui->collectWaterButton->setEnabled(false); // Ovo radimo da bismo izbegli registraciju vise klikova

    QWidget *currentCard = ui->weatherCards->currentWidget();

    if (currentCard) {
        actionSelector->selectAction(COLLECT_WATER, currentCard);

        qDebug() << "- Total collected water:" << currentPlayer->getTotalWater();
    } else {
        qDebug() << "No active card available to collect water from!";
    }


    QTimer::singleShot(500, this, [this]() {
        ui->collectWaterButton->setEnabled(true);
        isCollectingWater = false;  // Resetujemo flag
    });
}

void GameWindow::on_collectFoodButton_clicked()
{
    if (isCollectingFood) {
        qDebug() << "Already collecting the food, ignoring the click.";
        return;
    }

    isCollectingFood = true;
    qDebug() << "Started collecting food.";

    ui->collectFoodButton->setEnabled(false); // Ovo radimo da bismo izbegli registraciju vise klikova
    actionSelector->selectAction(COLLECT_FOOD);

    int totalFood = currentPlayer->getTotalFood();
    qDebug() << "Total food collected so far: " << totalFood;

    QTimer::singleShot(500, this, [this]() {
        ui->collectFoodButton->setEnabled(true);
        isCollectingFood = false;  // Resetujemo flag
    });
}

void GameWindow::checkForWaterShortage()
{
    voting->checkForWaterShortage();
}

void GameWindow::checkForFoodShortage()
{
    voting->checkForFoodShortage();
}

void GameWindow::onVotingStarted()
{
    QMessageBox::information(this, "Voting Started", "Voting has started! Please cast your votes.");
}

void GameWindow::onVotingConcluded()
{
    Player* eliminatedPlayer = voting->getEliminatedPlayer();  // Use the getter

    if (eliminatedPlayer) {
        QMessageBox::information(this, "Voting Concluded",
                                 "Player " + eliminatedPlayer->getName() + " has been eliminated.");
    } else {
        QMessageBox::information(this, "Voting Concluded", "No player was eliminated.");
    }
}


void GameWindow::on_castVoteButton_clicked()
{
    /*
    if (voting && voting->votingInProgress) {
        QList<Player*> otherPlayers;

        for (Player* player : allPlayers) {  // Assuming `allPlayers` contains all players
            if (player != currentPlayer) {
                otherPlayers.append(player);
            }
        }

        if (otherPlayers.isEmpty()) {
            qDebug() << "No players available to vote for.";
            return;
        }

        bool ok;
        QStringList playerNames;
        for (Player* player : otherPlayers) {
            playerNames.append(player->getName());
        }

        QString selectedPlayerName = QInputDialog::getItem(this, "Select Player to Vote For",
                                                           "Choose a player to vote for:", playerNames, 0, false, &ok);

        if (ok && !selectedPlayerName.isEmpty()) {
            Player* selectedPlayer = nullptr;
            for (Player* player : otherPlayers) {
                if (player->getName() == selectedPlayerName) {
                    selectedPlayer = player;
                    break;
                }
            }

            if (selectedPlayer) {
                int voteValue = 1; // You can customize the vote value (1 means vote for elimination, for example)
                voting->castVote(selectedPlayer, voteValue);
                qDebug() << "Voted for player:" << selectedPlayer->getName();
            }
        }
    }
*/
}


