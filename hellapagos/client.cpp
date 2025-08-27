#include "client.h"
#include <QDebug>
#include <QCoreApplication>
#include <QList>
Client::Client(QObject *parent)
    : QObject(parent), tcpSocket(new QTcpSocket(this))
{
    players = new QList<SickPlayer*>();

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
        QByteArray data = message.toUtf8();
        data.append('\n');

        tcpSocket->write(data);
        tcpSocket->flush();
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



    //QString message = QString::fromUtf8(data).trimmed();
    QStringList lines = QString::fromUtf8(data).split("\n\n", Qt::SkipEmptyParts);
    for (const QString& message : lines) {
        handleMessage(message.trimmed());
    }}


void Client::handleMessage(const QString& message)
{
    if (message.contains("PLAYERS_LIST:")) {
        int start = message.indexOf("PLAYERS_LIST:") + QString("PLAYERS_LIST:").length();
        int end = message.indexOf('\n', start);

        QString playersList = message.mid(start,end-start).trimmed();
        playerNames = playersList.split(",");
        qDebug() << "Received Players List:" << playerNames;
        emit playersListUpdated(playerNames);
    }

    if (message == "Name already taken. Choose a different name.") {
        emit nameTaken();
    }

    if (message.contains("PLAYER_ELIMINATED:")) {
        int start = message.indexOf("PLAYER_ELIMINATED:") + QString("PLAYER_ELIMINATED:").length();
        int end = message.indexOf('\n', start);

        QString playerName = message.mid(start,end-start).trimmed();
        qDebug() << "Player eliminated:" << playerName;

       /* if(playerName == player->getPlayerName()) {
            player->setSpectating(true);
            qDebug() << playerName << "is now spectator.";
        }*/
    }

    if (message.startsWith("CURRENT_PLAYER:")) {
        QString currentPlayer = message.section("CURRENT_PLAYER:", 1, 1).trimmed();
        //qDebug() << "It's now" << currentPlayer << "'s turn";

        emit currentPlayerChanged(currentPlayer);
    }

    if (message.contains("ROUND_STARTED:")) {
        int start = message.indexOf("ROUND_STARTED:") + QString("ROUND_STARTED:").length();
        int end = message.indexOf('\n', start);

        int roundNumber = message.mid(start,end-start).trimmed().toInt();
        qDebug() << "New Round Started: Round" << roundNumber;

        for (SickPlayer* player1 : *players) {
            if (!player1) {
                qDebug() << "Invalid player pointer.";
                continue;
            }
            player1->decrementRecovery();
        }
    }

    if (message.contains("RESOURCES:")) {
        int start = message.indexOf("RESOURCES:") + QString("RESOURCES:").length();
        int end = message.indexOf('\n', start);

        QString resourceData = message.mid(start,end-start).trimmed();
        QStringList resourceList = resourceData.split(',');

        int water = 0, food = 0, wood = 0, rafts = 0;

        for (const QString& resource : resourceList) {
            QStringList keyValue = resource.split(':');
            if (keyValue.size() == 2) {
                QString key = keyValue[0].trimmed();
                int value = keyValue[1].toInt();
                if (key == "water") water = value;
                else if (key == "food") food = value;
                else if (key == "wood") wood = value;
                else if (key == "rafts") rafts = value;
            }
        }

        qDebug() << "Resources updated - Water:" << water << ", Food:" << food << ", Wood:" << wood << ", Rafts:" << rafts;
    }

    if (message.contains("WEATHER_CARD:")) {
        int start = message.indexOf("WEATHER_CARD:") + QString("WEATHER_CARD:").length();
        int end = message.indexOf('\n', start);

        int index = message.mid(start,end-start).trimmed().toInt();
        emit weatherCardUpdated(index);
    }

    if (message.contains("ACTION_APPROVED:")) {
        int start = message.indexOf("ACTION_APPROVED:") + QString("ACTION_APPROVED:").length();
        int end = message.indexOf('\n', start);

        QString action = message.mid(start,end-start).trimmed();

        if (action == "COLLECT_WATER") emit collectWaterApproved();
        else if (action == "COLLECT_WOOD") emit collectWoodApproved();
        else if (action == "COLLECT_FOOD") emit collectFoodApproved();
    }
    else if (message.contains("Sorry, not your turn.")) {
        qDebug()<<"Not your turn...";
        emit notYourTurn();
    }

    if (message.contains("START_VOTE:")) {
        if (voteInProgress) {
            qDebug() << "Ignored duplicate START_VOTE";
            return;
        }
        voteInProgress = true;

        int start = message.indexOf("START_VOTE:") + QString("START_VOTE:").length();
        int end = message.indexOf('\n', start);

        QStringList voteTargets = message.mid(start,end-start).split(",");
        for (QString& name : voteTargets) name = name.trimmed();
        qDebug() << "Vote triggered. Eligible players:" << voteTargets;
        emit voteStarted(voteTargets);
    }

    if (message.contains("VOTE_RESULT:")) {
        voteInProgress = false;

        int start = message.indexOf("VOTE_RESULT:") + QString("VOTE_RESULT:").length();
        int end = message.indexOf('\n', start);

        QString eliminatedPlayer = message.mid(start,end-start).trimmed();
        qDebug() << "Player voted out IIIIs:" << eliminatedPlayer;
        emit playerVotedOut(eliminatedPlayer);
    }

    if(message == "GAME OVER") emit gameOverSignal();

    if (message.contains("GAME_STATS:")) {
        int start = message.indexOf("GAME_STATS:") + QString("GAME_STATS:").length();
        QString stats = message.mid(start).trimmed();
        emit gameStatsReceived(stats);
    }

    if (message.contains("AllReady")) emit allPlayersReady();

    else if (message == "Server full. Connection rejected.") {
        qDebug() << "Server is full. Closing the client...";
        emit serverFullSignal();
        QCoreApplication::quit();
    }
    else if (message == "DISCONNECT") {
        qDebug() << "Server requested a disconnect.";
        tcpSocket->disconnectFromHost();
    }
}

/*
void Client::handleMessage(const QString& message)
{
    if (message.startsWith("PLAYERS_LIST:")) {
        QString playersList = message.mid(QString("PLAYERS_LIST:").length()).trimmed();
        playerNames = playersList.split(",");
        qDebug() << "Received Players List:" << playerNames;

        emit playersListUpdated(playerNames);
    }

    if (message == "Name already taken. Choose a different name.") {
        emit nameTaken();
    }

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
        for (SickPlayer* player1 : *players) {
            if (!player1) {
                qDebug() << "Invalid player pointer.";
                continue;
            }
            player1->decrementRecovery();
        }

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
        emit notYourTurn();


    }

    if (message.startsWith("START_VOTE:")) {
        if (voteInProgress) {
            qDebug() << "Ignored duplicate START_VOTE";
            return;
        }
        voteInProgress = true;

        QStringList voteTargets = message.mid(QString("START_VOTE:").length()).split(",");
        for (QString& name : voteTargets) {
            name = name.trimmed();  // Prevents "333\n" artifacts
        }
        qDebug() << "Vote triggered. Eligible players:" << voteTargets;
        emit voteStarted(voteTargets);  // emit to GameWindow to open dialog
    }

    if (message.startsWith("VOTE_RESULT:")) {
        voteInProgress = false;

        QString eliminatedPlayer = message.section("VOTE_RESULT:", 1);
        qDebug() << "Player voted out:" << eliminatedPlayer;
        emit playerVotedOut(eliminatedPlayer);
    }


    if(message=="GAME OVER"){
        emit gameOverSignal();
    }
    if (message.startsWith("GAME_STATS:")) {
        QString stats = message.mid(QString("GAME_STATS:").length()).trimmed();
        emit gameStatsReceived(stats);
    }

    if (message.startsWith("AllReady")) {
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
*/

void Client::onDisconnected()
{
    qDebug() << "Disconnected from server!";
    if (tcpSocket->state() == QTcpSocket::UnconnectedState) {
        qDebug() << "Socket is unconnected.";
    }
}

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

void Client::addPlayer(SickPlayer* player1) {
    if (!player1) {
        qDebug() << "Cannot add player. The pointer is null.";
        return;
    }

    // if (players->contains(player1)) {
    //     return;
    // }

    if (!player1->isRecovered()) {
        players->push_back(player1);
    }
}

QStringList Client::getPlayers() {
    return playerNames;
}

