#pragma once
#include "Point.h"
#include "Board.h"
#include "Barrel.h"
#include "Ghost.h"
#include <vector>

using std::vector; 
constexpr int NUM_OF_MOVES = 6;
constexpr int JUMP_HEUGHT = 2;
constexpr int NUM_OF_LIVES = 3;
constexpr int FATAL_FALL_HEIGHT = 5;


class Mario
{
	Point position;
	DirectionType direction = STAY; 
	DirectionType moveDirection;

	char arrowKeys[NUM_OF_MOVES] = { '\0' };
	Board* pBoard = nullptr;
	int life = NUM_OF_LIVES;
	int fallDistance = 0;

	bool isJumping = false;
	int jumpPhase = 0;
	bool keyWasPressed = false;

	char getHammer = '\0';
	bool hasHammer = false;



public:
	Mario(int startX = 0, int startY = 23, DirectionType moveDirection = STAY) : position(startX, startY, '@') {}

	Point getPos() { return position; }
	void setPos(int x, int y) { position.setX(x); position.setY(y); }
	void setArrowKeys(const char* keys);
	void setDirection(DirectionType dir) { direction = dir; }
	void setBoard(Board& board) { pBoard = &board; }
	void resetKeyPress() { keyWasPressed = true; }
	int keyPressed(char key);
	bool move(std::vector<Barrel>& barrels, std::vector<Ghost>& ghosts);
	void updateMoveDirection();
	void handleUpAndJump(int currX, int currY, char currCh, char underMe);
	void handleDown(int currX, int currY, char underMe);
	void executeJump(DirectionType moveDirection);
	void handleFalling(int currX, int currY, char underme);
	void handleUseHammer();
	bool hit(std::vector<Barrel>& barrels, std::vector<Ghost>& ghosts);
	void hitByMove(char moveKeys[], std::vector<Barrel>& barrels, std::vector<Ghost>& ghosts);
	void jump();
	void jump_on_move_R();
	void jump_on_move_L();
	void updatePosition();
	bool getpauline();

	int getLives() const { return life; }
	void loseLife();
	void resetFallDistance() { fallDistance = 0; }
	bool isAlive() const { return life > 0; }
	bool nearTheExplosion();
	void init();


private: 
	bool isAtHammerPos() const {
		return (position.getX() == pBoard->getHammerPos().getX() && position.getY() == pBoard->getHammerPos().getY()); 
	}
	void tryToPickHammer(char key) {
		if (isAtHammerPos() && key == 'p' && !hasHammer) {
			hasHammer = true;
			pBoard->setChar(pBoard->getHammerPos().getX(), pBoard->getHammerPos().getY(), ' ');
			gotoxy(pBoard->getHammerPos().getX(), pBoard->getHammerPos().getY());
			std::cout << ' ';

			// Update mario's symbol based on direction
			if (direction == RIGHT) {
				position.setSymbol('p');
			}
			else if (direction == LEFT) {
				position.setSymbol('q');
			}
			gotoxy(position.getX(), position.getY());
			std::cout << position.getSymbol();
		}
	}
};