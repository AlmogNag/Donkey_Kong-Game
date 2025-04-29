#pragma once
#include "Point.h"
#include "Board.h"

class Ghost
{
    Point position;
    DirectionType currentDirection;
    Board* pBoard = nullptr;

public: 
    Ghost(int x = 79, int y = 0) : position(x, y, 'x'), currentDirection(RIGHT) {}

    void setBoard(Board& board) { pBoard = &board; }
    void setDirection(DirectionType dir) { currentDirection = dir; }
    void setPos(int x, int y) { position.setX(x); position.setY(y); }

    int getX() const { return position.getX(); }
    int getY() const { return position.getY(); }

    void move();
    void draw();
    void erase();
};