#include "Barrel.h"
#include <iostream>

void Barrel::move() {
    if (!pBoard) return;

    if (atTheEnd()) {
        erase(); 
    }

    int currX = position.getX();
    int currY = position.getY();

    if (currX < 0 || currX >= MAX_X || currY < 0 || currY >= MAX_Y) {
        explode();
        return;
    }

    char underMe = pBoard->getChar(currX, currY + 1);
    char currPos = pBoard->getChar(currX, currY);

    if (!pBoard->isFloor(underMe) && underMe != LADDER) {
        fallDistance++;
    }
    else {
        fallDistance = 0;
    }

    if (fallDistance >= 8) {
        explode();
        return;
    }

    char savedChar = pBoard->getChar(currX, currY);
    if (savedChar != 'O') {
        position.saveLastChar(savedChar);
    }

    position.erase();

    if (pBoard->isFloor(underMe) || underMe == LADDER) {
        if (underMe == '>') {
            currentDirection = RIGHT;
            position.move(RIGHT);
        }
        else if (underMe == '<') {
            currentDirection = LEFT;
            position.move(LEFT);
        }
        else if (underMe == '=' || underMe == LADDER) {
            position.move(currentDirection);
        }
    }
    else {
        position.move(DOWN);
    }
    draw();
    
    if (atTheEnd()) {
        eraseFromBoard(); 
    }

}
void Barrel::draw() {
    if (!pBoard) return;  

    int x = position.getX();
    int y = position.getY();

    
    if (x < 0 || x >= MAX_X || y < 0 || y >= MAX_Y) {
        explode();
        return;
    }

    char newPosChar = pBoard->getChar(x, y);
    if (newPosChar != 'O') {
        position.saveLastChar(newPosChar);
    }

    gotoxy(x, y);
    std::cout << 'O' << std::flush;
    pBoard->setChar(x, y, 'O');
}

void Barrel::erase() {
    if (!pBoard) return;  

    int x = position.getX();
    int y = position.getY();

    if (x < 0 || x >= MAX_X || y < 0 || y >= MAX_Y) return;

    gotoxy(x, y);
    char originalChar = position.getLastChar();
    std::cout << originalChar << std::flush;
    pBoard->setChar(x, y, originalChar);
}

void Barrel::eraseFromBoard() {
    if (!pBoard) return;

    int x = position.getX(); 
    int y = position.getY(); 
    pBoard->setChar(x, y, ' '); 
    gotoxy(x, y);
    std::cout << ' ';
    pBoard->setChar(x, y, ' ');

}

void Barrel::explode() {
    if (!pBoard) return;

    erase();

    int x = position.getX();
    int y = position.getY();
    pBoard->setExplodePos(x, y); 

    for (int dy = -2; dy <= 2; dy++) {
        for (int dx = -2; dx <= 2; dx++) {
            int newX = x + dx;
            int newY = y + dy;
            if (newX >= 0 && newX < MAX_X && newY >= 0 && newY < MAX_Y) {
                gotoxy(newX, newY);
                std::cout << 'X';
                pBoard->setChar(newX, newY, 'X');
            }
        }
    }
    Sleep(100);  

    for (int dy = -2; dy <= 2; dy++) {
        for (int dx = -2; dx <= 2; dx++) {
            int newX = x + dx;
            int newY = y + dy;
            if (newX >= 0 && newX < MAX_X && newY >= 0 && newY < MAX_Y) {
                char originalChar = pBoard->getChar(newX, newY);
                if (originalChar == 'X') {
                    gotoxy(newX, newY);
                    std::cout << ' ';
                    pBoard->setChar(newX, newY, ' ');
                }
            }
        }
    }
    isExploded = true;
}

Barrel Barrel::spawnBarrel(Board& board)
{
    Barrel newBarrel; 
    newBarrel.setBoard(board); 
    if (rand() % 2 == 0) {
        newBarrel.setPos(board.getDonkyPos().getX() + 1, board.getDonkyPos().getY()); 
        newBarrel.setDirection(RIGHT); 
    }
    else {
        newBarrel.setPos(board.getDonkyPos().getX() - 1, board.getDonkyPos().getY());
        newBarrel.setDirection(LEFT);
    }
    return newBarrel;
}