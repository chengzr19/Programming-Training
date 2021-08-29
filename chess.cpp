#include "chess.h"
#include <iostream>

int Chess::redDead = 0;
int Chess::blueDead = 0;

void Chess::die()
{
    isDead = true;
    if (this -> color == 1) {
        this->setGeometry(410, 480 - redDead * 5, 50, 25);
        redDead++;
    }
    if (this -> color == 2) {
        this->setGeometry(470, 480 - blueDead * 5, 50, 25);
        blueDead++;
    }
    this->raise();
}

