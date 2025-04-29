#pragma once
#include "Point.h"
#include "Board.h"

constexpr int BOARDPOS_X_R = 79; 
constexpr int BOARDPOS_X_L = 0; 
constexpr int BOARDPOS_Y = 23; 

class Barrel {
    Point position;
    DirectionType currentDirection;
    Board* pBoard = nullptr;
    int fallDistance = 0;
    bool isExploded = false;

public:
    Barrel(int x = 38, int y = 4) : position(x, y, 'O'), currentDirection(RIGHT) {}

    void setBoard(Board& board) { pBoard = &board; }
    void setDirection(DirectionType dir) { currentDirection = dir; }
    void setPos(int x, int y) { position.setX(x); position.setY(y); }

    int getX() const { return position.getX(); }
    int getY() const { return position.getY(); }

    void move();
    void draw();
    void erase();
    void eraseFromBoard(); 
    void explode();
    bool hasExploded() const { return isExploded; }
    bool atTheEnd() const {
        return ((position.getY() == BOARDPOS_Y && position.getX() == BOARDPOS_X_L) || (position.getY() == BOARDPOS_Y && position.getX() == BOARDPOS_X_R));
    }
    static Barrel spawnBarrel(Board& board); 
};