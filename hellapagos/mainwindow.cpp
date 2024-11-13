#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "client.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,client(new Client(this))
{
    ui->setupUi(this);
    client->connectToServer();


    connect(ui->joinButton, &QPushButton::clicked, this, [this]() {
        QString playerName = ui->playerNameLineEdit->text();
        client->sendJoinCommand(playerName); // Send JOIN command with the player name
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
