#include "client.h"
#include <QDebug>
#include <QCoreApplication>
Client::Client(QObject *parent)
    : QObject(parent), tcpSocket(new QTcpSocket(this))
{
}

Client::~Client()
{
    if (tcpSocket->state() == QTcpSocket::ConnectedState) {
        tcpSocket->flush();  // Make sure all data is sent before disconnecting
    }
    tcpSocket->disconnectFromHost();
    tcpSocket->waitForDisconnected();
}

void Client::connectToServer()
{
    connect(tcpSocket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &Client::onDisconnected);

    tcpSocket->connectToHost("127.0.0.1", 1234);  // Connect to the server (localhost)
}

void Client::sendMessage(const QString &message)
{
    if (tcpSocket->state() == QTcpSocket::ConnectedState) {
        tcpSocket->write(message.toUtf8());
        //tcpSocket->flush();
    } else {
        qDebug() << "Cannot send message. Socket is not connected.";
    }
}

void Client::sendJoinCommand(const QString &playerName)
{
    QString command = "JOIN:" + playerName;
    sendMessage(command); // Send JOIN command to the server
}


void Client::onConnected()
{
    qDebug() << "Connected to server!";
    sendMessage("Hello from client!");
}

void Client::sendReadySignal()
{
    sendMessage("Ready!");
}


void Client::onReadyRead()
{
    QByteArray data = tcpSocket->readAll();
    qDebug() << "Received from server:" << data;

    QString message = QString::fromUtf8(data).trimmed();

    if (message.startsWith("PLAYER_ELIMINATED:")) {
        QString playerName = message.split(":").last();
        qDebug() << "Player eliminated:" << playerName;

        //Pretvori igraca u posmatraca
        if(playerName == player->getPlayerName()) {
            player->setSpectating(true);
            qDebug() << playerName << "is now spectator.";
        }
    }

    if (message.startsWith("ROUND_STARTED:")) {
        int roundNumber = message.split(":").last().toInt();
        qDebug() << "New Round Started: Round" << roundNumber;

        // Update the UI or perform any other logic
       // gamewindow->ui->roundLabel->setText(QString("Round: %1").arg(roundNumber));

    }

    if (message.startsWith("RESOURCES:")) {
        QString resourceData = message.mid(QString("RESOURCES:").length());
        QStringList resourceList = resourceData.split(',');

        int water = 0, food = 0, wood = 0, rafts = 0;

        // Loop through the resourceList to extract values
        for (const QString& resource : resourceList) {
            QStringList keyValue = resource.split(':');
            if (keyValue.size() == 2) {
                QString key = keyValue[0].trimmed();
                int value = keyValue[1].toInt();
                if (key == "water") {
                    water = value;
                } else if (key == "food") {
                    food = value;
                } else if (key == "wood") {
                    wood = value;
                } else if (key == "rafts") {
                    rafts = value;
                }
            }
        }

        qDebug() << "Resources updated - Water:" << water << ", Food:" << food << ", Wood:" << wood << ", Rafts:" << rafts;

    }

    if (message.startsWith("WEATHER_CARD:")) {
        int index = message.split(":").last().toInt();
        emit weatherCardUpdated(index);
    }


    if (message.startsWith("ACTION_APPROVED:")) {
        QString action = message.split(":").last();

        // Trigger the corresponding action in the game
        if (action == "COLLECT_WATER") {
            emit collectWaterApproved();
        } else if (action == "COLLECT_WOOD") {
            emit collectWoodApproved();
        }else if (action == "COLLECT_FOOD") {
            emit collectFoodApproved();
        }
    } else if (message.startsWith("Sorry, not your turn.")) {
       // QMessageBox::information(nullptr, "Not Your Turn", "Sorry, it's not your turn to play.");
        qDebug()<<"Not your turn...";

    }

    if (message == "AllReady") {
        emit allPlayersReady(); }
    else if (message == "Server full. Connection rejected.") {
        qDebug() << "Server is full. Closing the client...";

        emit serverFullSignal();  // Emit the signal to close the window
        QCoreApplication::quit();  // Optionally, close the application
    } else if (message == "DISCONNECT") {
        qDebug() << "Server requested a disconnect.";
        tcpSocket->disconnectFromHost();
    }
}

void Client::onDisconnected()
{
    qDebug() << "Disconnected from server!";
    if (tcpSocket->state() == QTcpSocket::UnconnectedState) {
        qDebug() << "Socket is unconnected.";
    }
}
/*
void Client::sendWeatherCardChange(int index)
{
    sendMessage(QString("WeatherCard:%1").arg(index));
}*/
// In Client class
void Client::sendWeatherCardChange(int index)
{
   // QString message = QString("WEATHER_CARD:%1").arg(index);
    // Send to server (assuming `tcpSocket` is connected to the server)
    QString message = "WEATHER_CARD:" + QString::number(index);
    sendMessage(message);  // Send the revealed card index to the server

    //tcpSocket->write(message.toUtf8());
}
void Client::sendWeatherCardRequest() {
    if (tcpSocket) {  // Use tcpSocket instead of socket
        QByteArray data = "REQUEST_WEATHER_CARD";
        tcpSocket->write(data);  // Send the request to the server
        tcpSocket->flush();  // Ensure the data is sent
    }
}
