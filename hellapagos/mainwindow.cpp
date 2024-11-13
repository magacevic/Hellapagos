#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "client.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , client(new Client(this))
{
    ui->setupUi(this);
    client->connectToServer();

    // Povezivanje join dugmeta sa slotom
    connect(ui->joinButton, &QPushButton::clicked, this, &MainWindow::onJoinButtonClicked);
    // Povezivanje dugmadi na stranici 2 sa slotovima
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onSettingsButtonClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::onRulesButtonClicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::onStartGameButtonClicked);  // Povezivanje Start Game dugmeta
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
    // Ovde možeš dodati kod za otvaranje prozora sa podešavanjima
    qDebug() << "Settings clicked!";
}

// Slot za rules dugme (radiš sa pravilima)
void MainWindow::onRulesButtonClicked()
{
    // Ovde možeš dodati kod za otvaranje prozora sa pravilima
    qDebug() << "Rules clicked!";
}

// Slot za start game dugme
void MainWindow::onStartGameButtonClicked()
{
    // Ovde možeš dodati kod za početak igre
    qDebug() << "Start Game clicked!";
}
