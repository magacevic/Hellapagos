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
}

MainWindow::~MainWindow()
{
    delete ui;
}
