#ifndef BOARD_H
#define BOARD_H
#include "chess.h"

class Node
{
public:
    int x, y, id;
    bool isCamp;
    Node(){
        x = y = 0;
        id = -1;
        isCamp = false;
    }
    ~Node(){}
};

class Board
{
public:
    Node ** board;
    int ** road;
    Board();
    ~Board() {};
    int isEatable(int x, int y);
    bool isValid(int type, int x1, int y1, int x2, int y2);
    int max(int x, int y) { return (x > y ? x : y); }
    int min(int x, int y) { return (x < y ? x : y); }
};

#endif // BOARD_H
