#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "client.h"
#include "soundmanager.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , client(new Client(this))
    , soundManager(new SoundManager(this))
{
    ui->setupUi(this);
    client->connectToServer();
    soundManager->playBackgroundMusic(":/audio/uke.wav");  // Update path as needed


    // Centriranje teksta unutar QLineEdit
    ui->playerNameLineEdit->setAlignment(Qt::AlignCenter);

    // Povezivanje join dugmeta sa slotom
    connect(ui->joinButton, &QPushButton::clicked, this, &MainWindow::onJoinButtonClicked);
    // Povezivanje dugmadi na stranici 2 sa slotovima
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onSettingsButtonClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::onRulesButtonClicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::onStartGameButtonClicked);  // Povezivanje Start Game dugmeta
    connect(ui->mainmenuButton, &QPushButton::clicked, this, &MainWindow::onMainMenuButtonClicked);
    connect(ui->next_1, &QPushButton::clicked, this, &MainWindow::onNext_1ButtonClicked);
    connect(ui->next_2, &QPushButton::clicked, this, &MainWindow::onNext_2ButtonClicked);
    connect(ui->next_3, &QPushButton::clicked, this, &MainWindow::onNext_3ButtonClicked);
    connect(ui->next_4, &QPushButton::clicked, this, &MainWindow::onNext_4ButtonClicked);
    connect(ui->next_5, &QPushButton::clicked, this, &MainWindow::onNext_5ButtonClicked);
    connect(ui->next_6, &QPushButton::clicked, this, &MainWindow::onNext_6ButtonClicked);
    connect(ui->next_7, &QPushButton::clicked, this, &MainWindow::onNext_7ButtonClicked);
    connect(ui->previous_1, &QPushButton::clicked, this, &MainWindow::onPrevious_1ButtonClicked);
    connect(ui->previous_2, &QPushButton::clicked, this, &MainWindow::onPrevious_2ButtonClicked);
    connect(ui->previous_3, &QPushButton::clicked, this, &MainWindow::onPrevious_3ButtonClicked);
    connect(ui->previous_4, &QPushButton::clicked, this, &MainWindow::onPrevious_4ButtonClicked);
    connect(ui->previous_5, &QPushButton::clicked, this, &MainWindow::onPrevious_5ButtonClicked);
    connect(ui->previous_6, &QPushButton::clicked, this, &MainWindow::onPrevious_6ButtonClicked);
    connect(ui->previous_7, &QPushButton::clicked, this, &MainWindow::onPrevious_7ButtonClicked);
    connect(ui->previous_8, &QPushButton::clicked, this, &MainWindow::onPrevious_8ButtonClicked);


}

MainWindow::~MainWindow()
{
    delete ui;
}

// Slot za join dugme
void MainWindow::onJoinButtonClicked()
{
    QString playerName = ui->playerNameLineEdit->text();
    client->sendJoinCommand(playerName);  // Pošaljemo JOIN komandu sa imenom igrača
    ui->stackedWidget->setCurrentIndex(1);  // Prebacimo na stranicu 2
}
// Slot za settings dugme (radiš sa podešavanjima)
void MainWindow::onSettingsButtonClicked()
{
    ui->stackedWidget->setCurrentIndex(10);
    qDebug() << "Settings clicked!";
}

// Slot za rules dugme (radiš sa pravilima)
void MainWindow::onRulesButtonClicked()
{
    // Ovde možeš dodati kod za otvaranje prozora sa pravilima
    ui->stackedWidget->setCurrentIndex(2);
    qDebug() << "Rules clicked!";
}

void MainWindow::onMainMenuButtonClicked()
{
    // Ovde možeš dodati kod za otvaranje prozora sa pravilima
    ui->stackedWidget->setCurrentIndex(1);
    qDebug() << "Main menu clicked!";
}

// Slot za start game dugme
void MainWindow::onStartGameButtonClicked()
{
    // Ovde možeš dodati kod za početak igre


    qDebug() << "Start Game clicked!";
}

void MainWindow::onNext_1ButtonClicked(){

    ui->stackedWidget->setCurrentIndex(3);

    qDebug()<<"Next_1 button clicked!";
}

void MainWindow::onNext_2ButtonClicked(){

    ui->stackedWidget->setCurrentIndex(4);

    qDebug()<<"Next_2 button clicked!";
}

void MainWindow::onNext_3ButtonClicked(){

    ui->stackedWidget->setCurrentIndex(5);

    qDebug()<<"Next_3 button clicked!";
}

void MainWindow::onNext_4ButtonClicked(){

    ui->stackedWidget->setCurrentIndex(6);

    qDebug()<<"Next_4 button clicked!";
}

void MainWindow::onNext_5ButtonClicked(){

    ui->stackedWidget->setCurrentIndex(7);

    qDebug()<<"Next_5 button clicked!";
}

void MainWindow::onNext_6ButtonClicked(){

    ui->stackedWidget->setCurrentIndex(8);

    qDebug()<<"Next_6 button clicked!";
}

void MainWindow::onNext_7ButtonClicked(){

    ui->stackedWidget->setCurrentIndex(9);

    qDebug()<<"Next_7 button clicked!";
}

void MainWindow::onPrevious_1ButtonClicked(){

    ui->stackedWidget->setCurrentIndex(1);

    qDebug()<< "Previous_1 clicked!";
}

void MainWindow::onPrevious_2ButtonClicked(){

    ui->stackedWidget->setCurrentIndex(2);

    qDebug()<< "Previous_2 clicked!";
}

void MainWindow::onPrevious_3ButtonClicked(){

    ui->stackedWidget->setCurrentIndex(3);

    qDebug()<< "Previous_3 clicked!";
}

void MainWindow::onPrevious_4ButtonClicked(){

    ui->stackedWidget->setCurrentIndex(4);

    qDebug()<< "Previous_4 clicked!";
}

void MainWindow::onPrevious_5ButtonClicked(){

    ui->stackedWidget->setCurrentIndex(5);

    qDebug()<< "Previous_5 clicked!";
}

void MainWindow::onPrevious_6ButtonClicked(){

    ui->stackedWidget->setCurrentIndex(6);

    qDebug()<< "Previous_6 clicked!";
}

void MainWindow::onPrevious_7ButtonClicked(){

    ui->stackedWidget->setCurrentIndex(7);

    qDebug()<< "Previous_7 clicked!";
}

void MainWindow::onPrevious_8ButtonClicked(){

    ui->stackedWidget->setCurrentIndex(8);

    qDebug()<< "Previous_8 clicked!";
}


