#pragma once

#include "Utils.h"
#include <iostream>


using namespace std;

enum DirectionType {
	UP = 0,
	STAY,
	LEFT,
	RIGHT,
	DOWN, 
	HAMMER
};



class Point
{
	static constexpr Direction directions[] = {
		{ 0, -1 }, // UP
		{ 0, 0 },  // STAY
		{ -1, 0 }, // LEFT
		{ 1, 0 },  // RIGHT
		{ 0, 1 }, // DOWN
		{ 0, 0 } // HAMMER- no moving
	};
	int x, y;
	char symbol;
	char lastChar; // To store the last character Mario was on

public:

	Point(int px = 0, int py = 0, char ch = '*') : x(px), y(py), symbol(ch), lastChar(' ') {}

	void setX(int _X) { x = _X; }
	void setY(int _Y) { y = _Y; }
	void setSymbol(char ch) { symbol = ch; }

	int getX() const { return x; }
	int getY() const { return y; }
	char getSymbol() const { return symbol; }
	char getLastChar()const { return lastChar; }

	void saveLastChar(char ch);
	void draw();
	void erase();
	void move(DirectionType direction);
	void fallDown(int x, int y);
};