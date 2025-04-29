#include "Ghost_new.h"
#include <random>
#include <ctime>

void Ghost_new::move()
{
	if (!pBoard) return;
    static std::mt19937 gen;
    static bool seeded = false;
    if (!seeded) {
        std::random_device rd;
        gen.seed(rd());
        seeded = true;
    }
    std::uniform_real_distribution<> dis(0.0, 1.0);
    double probability = dis(gen);
	position.draw();
	char currX = position.getX();
	char currY = position.getY();
	erase();
    int deltaX = (currentDirection == RIGHT) ? 1 : -1;
	char nextUnder = (currY + 1 < MAX_Y) ? pBoard->getChar(currX + deltaX, currY + 1) : ' ';
	if (probability <= 0.05 || !pBoard->isFloor(nextUnder)) {
		// Change direction if 5% probability or if ghost reaches the end of the floor
		currentDirection = (currentDirection == RIGHT) ? LEFT : RIGHT;
	}

    if (currentDirection == RIGHT && currX < MAX_X - 1) {
        position.setX(currX + 1);
    }
    else if (currentDirection == LEFT && currX > 0) {
        position.setX(currX - 1);
    }

    // Draw at new position
    draw();
}

void Ghost_new::draw()
{
    if (!pBoard) return;  // Safety check

    int x = position.getX();
    int y = position.getY();

    if (x < 0 || x >= MAX_X || y < 0 || y >= MAX_Y) return;

    // Save the character at the new position
    char newPosChar = pBoard->getChar(x, y);
    if (newPosChar != 'x') {
        position.saveLastChar(newPosChar);
    }

    gotoxy(x, y);
    std::cout << 'x' << std::flush;
    pBoard->setChar(x, y, 'x');
}


void Ghost_new::erase()
{
    if (!pBoard) return;

    int x = position.getX();
    int y = position.getY();

    gotoxy(x, y);
    char originalChar = position.getLastChar();
    std::cout << originalChar << std::flush;
    pBoard->setChar(x, y, originalChar);
}

