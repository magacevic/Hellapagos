#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "client.h"
#include "soundmanager.h"
#include <QPixmap>
#include <QLabel>
#include <QTimer>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , client(new Client(this))
    , soundManager(new SoundManager(this))
    , errorLabel(nullptr)
{
    ui->setupUi(this);
    client->connectToServer();
    setFixedWindowSize();
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
    connect(ui->mainmenuButton_2, &QPushButton::clicked, this, &MainWindow::on_mainmenuButton_2_clicked);

    connect(ui->horizontalSlider, &QSlider::valueChanged, this, &MainWindow::on_horizontalSlider_valueChanged);
    connect(ui->MuteSoundButton, &QPushButton::toggled, this, &MainWindow::on_MuteSoundButton_toggled);

}

MainWindow::~MainWindow()
{
    delete ui;
}

// Slot za join dugme
void MainWindow::onJoinButtonClicked()
{
    QString playerName = ui->playerNameLineEdit->text();
    if (playerName.isEmpty()) {
        // Ako je prazno, prikazujemo sliku i poruku o grešci
        showErrorMessage();
    } else {
        // Ako nije prazno, šaljemo JOIN komandu sa imenom
        client->sendJoinCommand(playerName);  // Pošaljemo JOIN komandu sa imenom igrača
        ui->stackedWidget->setCurrentIndex(1);  // Prebacimo na stranicu 2
    }
}
void MainWindow::setFixedWindowSize()
{
    // Postavi minimalnu i maksimalnu veličinu na istu vrednost
    this->setFixedSize(750, 600);  // Na primer, 800x600 je željena veličina prozora
}


void MainWindow::showErrorMessage()
{
    // Ako već postoji labela za sliku, ne kreiraj novu
    if (!errorLabel) {
        errorLabel = new QLabel(this);
        errorLabel->setGeometry(100, 100, 300, 200);  // Pozicija i dimenzije labela za sliku
        errorLabel->setStyleSheet("QLabel { background: transparent; }");  // Postavljanje pozadine na transparentnu
    }

    // Postavljanje slike
    QPixmap pixmap("../../wood2.png");  // Putanja do slike u resursima
    int newWidth = 500;  // Nova širina slike
    int newHeight = 800; // Nova visina slike
    pixmap = pixmap.scaled(newWidth, newHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    errorLabel->setPixmap(pixmap);
    errorLabel->setAlignment(Qt::AlignCenter);  // Centriraj sliku

    // Dobijanje dimenzija trenutnog prozora
    QRect screenGeometry = this->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    // Dimenzije slike
    int imageWidth = pixmap.width();
    int imageHeight = pixmap.height();

    // Računanje pozicije za centar prozora
    int xPos = (screenWidth - imageWidth) / 2;
    int yPos = (screenHeight - imageHeight) / 2;

    // Postavljanje geometrije (pozicija i dimenzije)
    errorLabel->setGeometry(xPos, yPos, imageWidth, imageHeight);

    // Prikazivanje slike
    errorLabel->show();

    // Skriva sliku i poruku nakon 2 sekunde
    QTimer::singleShot(2000, errorLabel, &QLabel::hide);

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

void MainWindow::on_mainmenuButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    qDebug() << "Main menu clicked!";
}


// Volume control
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    float volume = value / 100.0f;

    if ((soundManager->getPreviousVolume() == 0.0f || soundManager->isSoundMuted()) && volume > 0.0f) {
        soundManager->toggleMute();  // Unmute
        ui->MuteSoundButton->setText("Mute");
        ui->MuteSoundButton->setChecked(false);
        qDebug() << "Sound unmuted with slider!";
    }


    soundManager->setVolume(volume);

    if (volume == 0.0f) {
        ui->MuteSoundButton->setText("Unmute");
        ui->MuteSoundButton->setChecked(true);
        qDebug() << "Sound muted with slider!";
    }
}

// Mute/Unmute button
void MainWindow::on_MuteSoundButton_toggled(bool checked)
{
    static bool alreadyToggled = false;
    if (alreadyToggled) return;  // Ignore the second toggle
    alreadyToggled = true;

    qDebug() << "Mute button pressed!";

    soundManager->toggleMute();

    if (checked) {
        // mute
        ui->MuteSoundButton->setText("Unmute");
        ui->horizontalSlider->setValue(0);
        qDebug() << "Sound is muted!";
    } else {
        // unmute
        ui->MuteSoundButton->setText("Mute");
        float previousVolume = soundManager->getPreviousVolume();

        if (previousVolume == 0.0f) {
            ui->horizontalSlider->setValue(10);  // Set slider to minimal value
            soundManager->setVolume(0.1f);
        } else {
            // Restore the previous volume
            ui->horizontalSlider->setValue(static_cast<int>(previousVolume * 100));
            soundManager->setVolume(previousVolume);
        }

        qDebug() << "Sound is unmuted!";
    }
    QTimer::singleShot(100, [this](){ alreadyToggled = false; });

}
