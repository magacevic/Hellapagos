// Player.cpp
#include "player.h"

Player::Player(const QString& name)
    : playerName(name), alive(true), sick(false) {}

QString Player::getName() const {
    return playerName;
}

void Player::setName(const QString& name) {
    playerName = name;
}

bool Player::isAlive() const {
    return alive;
}

void Player::setAlive(bool alive) {
    this->alive = alive;
}

bool Player::isSick() const {
    return sick;
}

void Player::setSick(bool sick) {
    this->sick = sick;
}
