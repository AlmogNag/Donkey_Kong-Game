#include "Point.h"
#include "Board.h"

void Point::move(DirectionType direction) {
    switch (direction) {
    case UP:
        --y;
        if (y < 1)
            y = 0;
        break;

    case STAY:
        break;

    case LEFT:
        --x;
        if (x < 1)
            x = 0;
        break;

    case RIGHT:
        ++x;
        if (x > 79)
            x = 79;
        break;

    case DOWN:
        ++y;
        if (y > 24)
            y = 24;
        break;
    }
}

void Point::fallDown(int x, int y)
{
    this->y = y + 1;
    if (this->y > 24)
        this->y = 24;
}

void Point::draw() {
    gotoxy(x, y);
    std::cout << symbol << std::flush;
}

void Point::erase() {
    gotoxy(x, y);
    std::cout << lastChar << std::flush;  // Print the last character to restore it
}

void Point::saveLastChar(char ch) {
    lastChar = ch;
}