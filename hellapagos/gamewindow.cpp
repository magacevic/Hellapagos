#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "votedialog.h"
#include <QRandomGenerator>
#include <QInputDialog>
#include <QMessageBox>
#include <QTimer>
#include <QVBoxLayout>
#include <QScrollArea>

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



    ui->playerNameLabel->setText(currentPlayer->getName());
    connect(m_client, &Client::weatherCardUpdated, this, &GameWindow::updateWeatherCard);
    connect(ui->revealButton, &QPushButton::clicked, this, &GameWindow::onWeatherCardClicked);
    connect(ui->collectWaterButton, &QPushButton::clicked, this, &GameWindow::on_collectWaterButton_clicked);
    connect(ui->collectFoodButton, &QPushButton::clicked, this, &GameWindow::on_collectFoodButton_clicked);
    //connect(ui->voteButton, &QPushButton::clicked, this, &GameWindow::on_voteButton_clicked);
    connect(m_client,&Client::gameOverSignal,this,&GameWindow::on_GameOver);
    connect(m_client, &Client::gameStatsReceived, this, &GameWindow::onGameStatsReceived);
    connect(m_client, &Client::notYourTurn,this,&GameWindow::on_notYourTurn);
    connect(m_client, &Client::voteStarted, this, &GameWindow::onVoteStarted);
    connect(m_client, &Client::playerVotedOut, this, &GameWindow::onPlayerVotedOut);
    connect(client, &Client::currentPlayerChanged, this, [this](const QString& name){
        ui->currPlayerName->setText(name);
    });
    connect(m_client, &Client::resourcesUpdated, this, [this](int water, int food, int wood, int rafts) {
        ui->label_water->setText(QString::number(water));
        ui->label_food->setText(QString::number(food));
        ui->label_wood->setText(QString::number(wood));
        ui->label_rafts->setText(QString::number(rafts));
    });

}

GameWindow::~GameWindow()
{
    delete ui;
    delete actionSelector;
}
// U vašem prozoru ili glavnoj funkciji




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
    currentPlayer->resetWood();
    if (isCollectingWood) {
        qDebug() << "Already collecting wood, ignoring the click."; // Debugging
        return;
    }

    isCollectingWood = true;
    qDebug() << "Started collecting wood.";

     ui->collectWoodButton->setEnabled(false);

    if (m_client) {
        QString actionMessage = "REQUEST_ACTION:COLLECT_WOOD";
        m_client->sendMessage(actionMessage);
        qDebug() << "Sent action request message to server: " << actionMessage;
    } else {
        qDebug() << "Client object is null!";
        return;
    }

    disconnect(m_client, &Client::collectWoodApproved, this, nullptr);

    connect(m_client, &Client::collectWoodApproved, this, [this]() {

        // Always collect 1 wood
        actionSelector->selectAction(COLLECT_WOOD);

        // Ask the player if they want to collect more wood
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Go To Forest",
                                      "You have collected 1 wood. Do you want to go to the forest and collect more?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            // Ask how many more pieces of wood they want to collect (1-5)
            bool ok = true;
            int additionalWood = QInputDialog::getInt(this, "Collect More",
                                                      "Select number of wood (1-5):",
                                                      1, 1, 5, 1, &ok);
            qDebug() << additionalWood;

            if (ok) {
                // Perform action based on the number of additional wood the player wants to collect
                actionSelector->selectAction(COLLECT_ADDITIONAL_WOOD, nullptr, additionalWood);

            }

        }

        int collectedWood=currentPlayer->getTotalWood();
        qDebug() << "- Total collected wood:" << collectedWood;

        if (m_client) {
            QString actionMessage = QString("ACTION_COMPLETED:COLLECT_WOOD %1").arg(collectedWood);
            m_client->sendMessage(actionMessage);
            qDebug() << "Sent action completion message to server: " << actionMessage;

        } else {
            qDebug() << "Client object is null!";
        }
        // Reset the current players total wood after round
        qDebug() << "Wood collected by current player: " << currentPlayer->getTotalWood();

        QTimer::singleShot(500, this, [this]() {
            ui->collectWoodButton->setEnabled(true);
            isCollectingWood = false;  // Resetujemo flag
        });
    });

    QTimer::singleShot(1000, this, [this]() {
        ui->collectWoodButton->setEnabled(true);
        isCollectingWood = false;  // Resetujemo flag
    });
}


void GameWindow::on_collectWaterButton_clicked()
{
    ui->collectWaterButton->setEnabled(false); // Ovo radimo da bismo izbegli registraciju vise klikova

    qDebug() << "Button clicked!";
    if (isCollectingWater) {
        qDebug() << "Already collecting water, ignoring the click.";
        return;
    }

    ui->collectWaterButton->setEnabled(false); // Ovo radimo da bismo izbegli registraciju vise klikova

    isCollectingWater = true;
    qDebug() << "Started collecting water.";

    if (m_client) {
        QString actionMessage = "REQUEST_ACTION:COLLECT_WATER";
        m_client->sendMessage(actionMessage);
        qDebug() << "Sent action request message to server: " << actionMessage;
    } else {
        qDebug() << "Client object is null!";
        return;
    }

    disconnect(m_client, &Client::collectWaterApproved, this, nullptr);

    connect(m_client, &Client::collectWaterApproved, this, [this]() {

        QWidget *currentCard = ui->weatherCards->currentWidget();

        ui->collectWaterButton->setEnabled(false); // Ovo radimo da bismo izbegli registraciju vise klikova

        if (currentCard) {
            actionSelector->selectAction(COLLECT_WATER, currentCard);
            int collectedWater=currentPlayer->getTotalWater();
            qDebug() << "- Total collected water:" << collectedWater;
            if (m_client) {
                QString actionMessage = QString("ACTION_COMPLETED:COLLECT_WATER %1").arg(collectedWater);
                m_client->sendMessage(actionMessage);
                qDebug() << "Sent action completion message to server: " << actionMessage;
            } else {
                qDebug() << "Client object is null!";
            }

        } else {
           qDebug() << "No active card available to collect water from!";
        }


        QTimer::singleShot(500, this, [this]() {
            ui->collectWaterButton->setEnabled(true);
            isCollectingWater = false;  // Resetujemo flag
        });
    });
    QTimer::singleShot(1000, this, [this]() {
        ui->collectWaterButton->setEnabled(true);
        isCollectingWater = false;  // Resetujemo flag
    });
}

void GameWindow::on_collectFoodButton_clicked()
{
    qDebug() << "Button clicked!";
    if (isCollectingFood) {
        qDebug() << "Already collecting the food, ignoring the click.";
        return;
    }

     ui->collectFoodButton->setEnabled(false);

    isCollectingFood = true;
    qDebug() << "Started collecting food.";

   // Ovo radimo da bismo izbegli registraciju vise klikova

    if (m_client) {
        QString actionMessage = "REQUEST_ACTION:COLLECT_FOOD";
        m_client->sendMessage(actionMessage);
        qDebug() << "Sent action request message to server: " << actionMessage;
    } else {
        qDebug() << "Client object is null!";
        return;
    }

   // disconnect(m_client, &Client::collectFoodApproved, this, nullptr);

    connect(m_client, &Client::collectFoodApproved, this, [this]() {
        actionSelector->selectAction(COLLECT_FOOD);

        int totalFood = currentPlayer->getTotalFood();
        qDebug() << "Total food collected so far: " << totalFood;

        if (m_client) {
            QString actionMessage = QString("ACTION_COMPLETED:COLLECT_FOOD %1").arg(totalFood);
            m_client->sendMessage(actionMessage);
            qDebug() << "Sent action completion message to server: " << actionMessage;
        } else {
            qDebug() << "Client object is null!";
        }

        QTimer::singleShot(500, this, [this]() {
            ui->collectFoodButton->setEnabled(true);
            isCollectingFood = false;  // Resetujemo flag
        });
    });

    QTimer::singleShot(1000, this, [this]() {
        ui->collectFoodButton->setEnabled(true);
        isCollectingFood = false;  // Resetujemo flag
    });
}

/*
void GameWindow::on_voteButton_clicked() {
    QStringList playerNames = m_client->getPlayers();
    QStringList eligiblePlayerNames;
    for (const QString& name : playerNames) {
        if (name != currentPlayer->getName()) {
            eligiblePlayerNames.append(name);
        }
    }

    // Open the vote dialog with the filtered players list
    VoteDialog voteDialog(eligiblePlayerNames, this);
    if (voteDialog.exec() == QDialog::Accepted) {
        QString selectedPlayerName = voteDialog.getSelectedPlayerName();
        if (selectedPlayerName.isEmpty()) {
            // Handle the vote logic (e.g., send it to the server)
            qDebug() << "Player voted for: " << selectedPlayerName;
            if (m_client) {
                m_client->sendMessage(QString("VOTE:%1").arg(selectedPlayerName));
            }
        }
    }

}
*/

void GameWindow::onGameStatsReceived(const QString &stats)
{
    // dizajn prozora za prikaz statistike
    QWidget *statsWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(statsWidget);

    QLabel *statsLabel = new QLabel(stats, statsWidget);
    statsLabel->setWordWrap(true);
    layout->addWidget(statsLabel);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(statsWidget);
    scrollArea->setWidgetResizable(true);

    QDialog *statsDialog = new QDialog(this);
    statsDialog->setWindowTitle("Game Statistics");

    QVBoxLayout *dialogLayout = new QVBoxLayout(statsDialog);
    dialogLayout->addWidget(scrollArea);

    statsDialog->resize(350, 400);
    statsDialog->exec();

}

void GameWindow::on_notYourTurn()
{
    QMessageBox::critical(this,"Not Your Turn","Sorry,not your turn to play!");
}

void GameWindow::onVoteStarted(const QStringList& voteTargets)
{
    qDebug() << "[UI] onVoteStarted triggered with players:" << voteTargets;

    QStringList eligiblePlayers;
    for (const QString& playerName : voteTargets) {
        if (playerName != currentPlayer->getName()) {
            eligiblePlayers.append(playerName);
        }
    }

    if (!voteTargets.contains(currentPlayer->getName())) {
        qDebug() << "This player is not participating in the vote.";
        return;
    }

    VoteDialog voteDialog(eligiblePlayers, this);
    if (voteDialog.exec() == QDialog::Accepted) {
        QString votedPlayer = voteDialog.getSelectedPlayerName();
        if (!votedPlayer.isEmpty() && m_client) {
            QString voteMsg = "VOTE:" + votedPlayer;
            m_client->sendMessage(voteMsg);
            qDebug() << "Sent vote to server:" << voteMsg;
        }
    }
}

void GameWindow::onPlayerVotedOut(const QString& playerName)
{
   /* QMessageBox::information(this, "Player Eliminated",
                             QString("Player %1 was voted out!").arg(playerName));
*/
    QMessageBox* msgBox = new QMessageBox(QMessageBox::Information,
                                          "Player Eliminated",
                                          QString("Player %1 was voted out!").arg(playerName),
                                          QMessageBox::NoButton,  // bez dugmadi
                                          this);
    msgBox->setWindowModality(Qt::NonModal);
    msgBox->show();

    // zatvori automatski posle 1s (1000 ms)
    QTimer::singleShot(1000, msgBox, &QMessageBox::close);

    if (playerName == currentPlayer->getName()) {
        //currentPlayer->setSpectating(true);  // Or handle in a custom way
        qDebug() << "You have been voted out. You are now a spectator.";


    ui->collectFoodButton->setEnabled(false);
    ui->collectWaterButton->setEnabled(false);
    ui->collectWoodButton->setEnabled(false);
    ui->revealButton->setEnabled(false);
    }
}

void GameWindow::on_GameOver()
{
    ui->collectFoodButton->setEnabled(false);
    ui->collectWaterButton->setEnabled(false);
    ui->collectWoodButton->setEnabled(false);
    ui->revealButton->setEnabled(false);

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Game Over");
    msgBox.setText("Game Over, thanks for playing!");

    QPushButton *statsButton = msgBox.addButton(tr("Show Statistics"), QMessageBox::ActionRole);
    QPushButton *okButton = msgBox.addButton(QMessageBox::Ok);

    msgBox.exec();

    if (msgBox.clickedButton() == statsButton) {
        if (m_client) {
            m_client->sendMessage("REQUEST_GAME_STATISTICS");
        }
    }
}

void GameWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event); // pozovi originalni resize

    auto makeEllipse = [](QLabel* label) {
        if (!label) return;
        label->setAlignment(Qt::AlignCenter);
        QRegion ellipseRegion(0, 0, label->width(), label->height(), QRegion::Ellipse);
        label->setMask(ellipseRegion);
    };

    makeEllipse(ui->label_food);
    makeEllipse(ui->label_water);
    makeEllipse(ui->label_wood);
    makeEllipse(ui->label_rafts);
}


