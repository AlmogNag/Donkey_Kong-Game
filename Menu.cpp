#include <iostream>
#include <iomanip>
#include "Menu.h"
using namespace std;

void Menu::print() const {
	gotoxy(0, 0); 
	for (int i = 0; i < MAX_Y - 1; i++) { 
		if (menu[i] != nullptr) {    
			cout << menu[i] << endl;
		}
	}
	cout << menu[MAX_Y - 1];
}

void Menu::printLoseLevel() const
{
	gotoxy(0, 0);
	for (int i = 0; i < MAX_Y - 1; i++) {
		if (loseLevel[i] != nullptr) {
			cout << loseLevel[i] << endl;
		}
	}
	cout << loseLevel[MAX_Y - 1];
}

void Menu::printWinLevel() const
{
	gotoxy(0, 0);
	for (int i = 0; i < MAX_Y - 1; i++) {
		if (winLevel[i] != nullptr) {
			cout << winLevel[i] << endl;
		}
	}
	cout << winLevel[MAX_Y - 1];
}

void Menu::printLoseGame() const
{
	gotoxy(0, 0);
	for (int i = 0; i < MAX_Y - 1; i++) {
		if (loseGame[i] != nullptr) {
			cout << loseGame[i] << endl;
		}
	}
	cout << loseGame[MAX_Y - 1];
}

void Menu::printWinGame() const
{
	gotoxy(0, 0);
	for (int i = 0; i < MAX_Y - 1; i++) {
		if (winGame[i] != nullptr) {
			cout << winGame[i] << endl;
		}
	}
	cout << winGame[MAX_Y - 1];
}



void Menu::printInstruction() const
{
	cout << "Objective: " << endl; 
	cout << "Your goal is to rescue Princess Pauline ($) by guiding Mario (@) to her." << endl; 
	cout << "Rules and Gameplay: " << endl; 
	cout << " 1. Avoid Rolling Barrels And Ghosts" << endl; 
	cout << "    * Barrels roll along the path, and Mario must avoid colliding with them." << endl; 
	cout << "    * Ghost move right and left" << endl; 
	cout << " 2. Falling Barrels Explode" << endl; 
	cout << "    * Falling barrels will explode after 8 moves." << endl; 
	cout << "    * Mario must not be near the barrels when they explode." << endl; 
	cout << " 3. Free Fall Limit." << endl; 
	cout << "    * If Mario falls more than 5 floors in free fall, he loses a life." << endl;
	cout << " 4. Use Hammer" << endl;
	cout << "    * search for hammer on the board" << endl;
	cout << "    * by press p you can kill ghosts or smash barrels " << endl;
	cout << " 4. Winning the Game" << endl; 
	cout << "    * The game ends successfully when Mario reaches Princess Pauline ($)." << endl;
	cout << "GOOD LUCK " << endl;

	displayControls(); 
}

//from GPT
void Menu::displayControls() const 
{
	cout << left;
	cout << setw(10) << "Action" << setw(15) << "Key(s)" << endl;
	cout << string(25, '-') << endl; // Separator line
	cout << setw(10) << "LEFT" << setw(15) << "a or A" << endl;
	cout << setw(10) << "RIGHT" << setw(15) << "d or D" << endl;
	cout << setw(10) << "UP/JUMP" << setw(15) << "w or W" << endl;
	cout << setw(10) << "DOWN" << setw(15) << "x or X" << endl;
	cout << setw(10) << "STAY" << setw(15) << "s or S" << endl;
	cout << setw(10) << "GET/USE HAMMER" << setw(15) << "p or P" << endl;
	cout << setw(10) << "PAUSE" << setw(15) << "ESC" << endl; 
	cout << setw(10) << "EXIT" << setw(15) << "9" << endl; 
}

// claude -  i did it at first like the Direction 
bool Menu::handleInput(char key) {
	switch (key) {
	case PressType::NEW_GAME:
		//displayScreens(); 
		return true;
	case PressType::INSTRUCTIONS:
		printInstruction();
		std::cout << "\nPress any key to return to menu...";
		_getch();
		clear_screen();
		print();
		return false;
	case PressType::EXIT:
		exit(0);
	default:
		return false;
	}
}


int Menu::handleInputLevel(char key, int num, bool win) {
	static bool first = true; 
	static int level = 1;
	if (first || win == false) {
		first = false;
		level = key - '0'; 
		if (level > num) {
			level = 1;
		}
	return level;
	}
	else if (level + 1 < num)
		{
			level++;
			return level;
		}
	return -1; 
}