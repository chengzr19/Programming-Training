#ifndef CHESS_H
#define CHESS_H

#include <QLabel>
#include <QMouseEvent>
#include <QMessageBox>

class Board;

struct coordinate {
    int x;
    int y;
};

class Chess : public QLabel
{
    Q_OBJECT
public:
    int color, type, id;
    static int redDead, blueDead;
    bool isFlipped, isChosen, isDead;
    coordinate posi;
    Chess(QWidget *parent = 0): QLabel(parent){
        isFlipped = false;
        isChosen = false;
        isDead = false;
    };
    Chess(const Chess & obj): QLabel(nullptr){
        color = obj.color;
        type = obj.type;
        isFlipped = obj.isFlipped;
        isChosen = obj.isChosen;
    }
    ~Chess(){};
    void die();
    bool isMovable(){
        if (type == 10 || type == 12) return false;
        else return true;
    }
};

#endif // CHESS_H
