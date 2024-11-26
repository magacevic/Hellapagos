#ifndef PLAYER_H
#define PLAYER_H

#include <QString>

class Player {
public:
    Player(const QString& name); // Constructor

    // Getters and setters
    QString getName() const;
    void setName(const QString& name);

    bool isAlive() const;
    void setAlive(bool alive);

    bool isSick() const;
    void setSick(bool sick);

private:
    QString playerName;
    bool alive;
    bool sick;
};

#endif // PLAYER_H
