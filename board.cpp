#include "board.h"
#include <stack>
#include <iostream>

Board::Board()
{
    board = new Node *[12];
    for (int i = 0; i < 12; i++) board[i] = new Node[5];
    for (int i = 0; i < 12; i++)
        for (int j = 0; j < 5; j++){
            if (i < 6) board[i][j].y = 20 + i*37;
            else board[i][j].y = 320 + (i - 6) * 37;
            board[i][j].x = 20 + 155 * j / 2;
            if ((i == 2 || i == 4 || i == 7 || i == 9)  && (j == 1 || j == 3)) board[i][j].isCamp = true;
            if ((i == 3 || i == 8) && (j == 2)) board[i][j].isCamp = true;
        }
    road = new int*[60];
    for (int i = 0; i < 60; i++) road[i] = new int[60];
    for (int i = 0; i < 60; i++)
        for (int j = 0; j < 60; j++)
        {
            road[i][j] = 0;
        }
    // Central Roads
    for (int i = 0; i < 60; i++)
    {
        if (board[i / 5][i % 5].isCamp)
        {
            road[i][i-6] = road[i-6][i] = 1;
            road[i][i-5] = road[i-5][i] = 1;
            road[i][i-4] = road[i-4][i] = 1;
            road[i][i-1] = road[i-1][i] = 1;
            road[i][i+6] = road[i+6][i] = 1;
            road[i][i+5] = road[i+5][i] = 1;
            road[i][i+4] = road[i+4][i] = 1;
            road[i][i+1] = road[i+1][i] = 1;
        }
        if (i == 17 || i == 42)
        {
            road[i-5][i-10] = road[i-10][i-5] = 1;
            road[i+5][i+10] = road[i+10][i+5] = 1;
            road[i-1][i-2] = road[i-2][i-1] = 1;
            road[i+1][i+2] = road[i+2][i+1] = 1;
        }
    }
    //Marginal roads
    for (int i = 0; i < 5; i++)
    {
        road[i][i+5] = road[i+5][i] = 1;
        if ((i % 5) > 0) road[i][i-1] = road[i-1][i] = 1;
        if ((i % 5) < 4) road[i][i+1] = road[i+1][i] =1;
    }
    for (int i = 55; i < 60; i++)
    {
        road[i][i-5] = road[i-5][i] = 1;
        if ((i % 5) > 0) road[i][i-1] = road[i-1][i] = 1;
        if ((i % 5) < 4) road[i][i+1] = road[i+1][i] = 1;
    }
    //Straight Railway
    for (int i = 1; i <= 10; i++)
    {
        for (int j = 1; j <= 10; j++)
        {
            road[i * 5][j * 5] = 2;
            road[i * 5 + 4][j * 5 + 4] = 2;
        }
    }
    for (int i = 5; i < 10; i++)
    {
        for (int j = 5; j < 10; j++)
        {
            road[i][j] = 2;
        }
    }
    for (int i = 25; i < 30; i++)
    {
        for (int j = 25; j < 30; j++)
        {
            road[i][j] = 2;
        }
    }
    for (int i = 30; i < 35; i++)
    {
        for (int j = 30; j < 35; j++)
        {
            road[i][j] = 2;
        }
    }
    for (int i = 50; i < 55; i++)
    {
        for (int j = 50; j < 55; j++)
        {
            road[i][j] = 2;
        }
    }
    road[27][32] = road[32][27] = 2;
    //Turning Railway
    int* station = new int[32];
    int index = 0;
    for (int i = 1; i <= 10; i++)
    {

        station[index]=i * 5; index++;
        station[index]=i * 5 + 4; index++;
    }
    for (int i = 6; i <= 8; i++)
    {
        station[index]=i; index++;
        station[index]=i + 20; index++;
        station[index]=i + 25; index++;
        station[index]=i + 45; index++;;
    }
    for (int i = 0; i < 32; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            if (road[station[i]][station[j]] != 2 && road[station[j]][station[i]] != 2)
                road[station[i]][station[j]] = road[station[j]][station[i]] = 3;
        }
    }
    //Self-reflection
    for (int i = 0; i < 60; i++) road[i][i] = 0;
}

int Board::isEatable(int x, int y)
{
    //std::cout << "compare " << x << " " << y << std::endl;
    if (x <= 9 && y <= 9)
    {
        if (x < y) return 1;
        if (x == y) return 0;
        if (x > y) return -1;
    }
    if (x == 11 || y == 11) return 0;
    if (y == 12)
    {
        if (x == 9) return 1;
        else if (x == 11) return 0;
        else return -2;
    }
    return -2;
}

bool Board::isValid(int type, int x1, int y1, int x2, int y2)
{
    int dpt = x1 * 5 + y1;
    int dst = x2 * 5 + y2;

    /*std::cout << "type " << type << " (" << x1 << "," << y1 << ")" << "(" << x2 << "," << y2 << ")"
        << " road " << road[dpt][dst] << std::endl;*/

    if (type != 9) {
        if (road[dpt][dst] == 1) return true;
        else if (road[dpt][dst] == 2)
        {
            if (x1 == x2)
            {
                for (int i = min(y1, y2) + 1; i < max(y1, y2); i++)
                {
                    if (board[x1][i].id != -1) return false;
                }
                return true;
            }
            else if (y1 == y2)
            {
                for (int i = min(x1, x2) + 1; i < max(x1, x2); i++)
                {
                    if (board[i][y1].id != -1) return false;
                }
                return true;
            }
            else return false;
        }
        else return false;
    }
    else
    {
        if (road[dpt][dst] == 1) return true;
        else if (road[dpt][dst] == 2)
        {
            if (x1 == x2)
            {
                //std::cout << "x1==x2" << std::endl;
                for (int i = min(y1, y2) + 1; i < max(y1, y2); i++)
                {
                    if (board[x1][i].id != -1) return false;
                }
                return true;
            }
            else if (y1 == y2)
            {
                //std::cout << "y1==y2" << std::endl;
                for (int i = min(x1, x2) + 1; i < max(x1, x2); i++)
                {
                    if (board[i][y1].id != -1) return false;
                }
                return true;
            }
            else return false;
        }
        else if (road[dpt][dst] == 3)
        {
            std::stack<int> search;
            int v = 0;
            bool find = false;
            int * lab = new int[60];
            int * dir = new int[4];
            dir[0] = -1; dir[1] = 1; dir[2] = -5; dir[3] = 5;
            for (int i = 0; i < 60; i++)
            {
                lab[i] = 0;
            }
            search.push(dpt);
            while (!search.empty())
            {
                v = search.top();
                //std::cout << "pop " << "[" << v / 5 << "," << v % 5 << "]" << std::endl;
                if (v == dst){
                    find = true;
                    break;
                }
                search.pop();
                if (lab[v] == 0){
                    lab[v] = 1;
                    for (int j = 0; j < 4; j++)
                    {
                        int d = v + dir[j];
                        if (d >= 0 && d < 60)
                        {
                            /* std::cout << "[" << d / 5 << "," << d % 5 << "] id " << board[d / 5][d % 5].id
                                    << " road " << road[v][d] << std::endl;*/
                            if (road[v][d] == 2)
                            {
                                if (board[d / 5][d % 5].id == -1)
                                {
                                    search.push(d);
                                    //std::cout << "push " << "[" << d / 5 << "," << d % 5 << "]" << std::endl;
                                }
                                else
                                {
                                    if (d == dst)
                                    {
                                        find = true;
                                        return find;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            return find;
        }
    }
    return false;
}

