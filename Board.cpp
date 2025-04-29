#include <cstring>
#include <iostream>
#include <fstream>
#include "Board.h"
#include "Utils.h"
#include "Ghost.h"
#include <filesystem>
#include <algorithm>

using namespace std;
namespace fs = std::filesystem;

bool Board::load(const std::string& filename) {
	foundPaulin = false;
	foundMario = false;
	foundDonkey = false;
	foundL = false;
	ghosts.clear();
	std::ifstream screen_file(filename);
	if (!screen_file.is_open()) {
		cout << "Error opening file: " << filename << endl;
		return false;
	}
	int curr_row = 0;
	int curr_col = 0;
	char c;
	while (!screen_file.get(c).eof() && curr_row < MAX_Y) {
		if (c == '\n') {
			if (curr_col < MAX_X) {
			#pragma warning(suppress : 4996) // to allow strcpy
				strcpy(originalBoard[curr_row] + curr_col, std::string(MAX_X - curr_col - 1, ' ').c_str());
			}
			++curr_row;
			curr_col = 0;
			continue;
		}

		if (curr_col < MAX_X) {

			originalBoard[curr_row][curr_col] = c;
			handleSpecialChar(curr_row, curr_col);
			curr_col++; 

		}	
	}
	if (foundL) {
		strncpy(&(originalBoard[LPos.getY()][LPos.getX()]), "Lives:  ", 8);
		strncpy(&(originalBoard[LPos.getY() + 1][LPos.getX()]), "Score:    ", 10);
	}
	if (!foundMario || !foundPaulin) {
		clear_screen(); 
		gotoxy(0, 0);
		cout << "board is fail" << endl;
		return false;
	}
	for (int i = curr_row; i < MAX_Y; ++i) {
		std::fill(originalBoard[i], originalBoard[i] + MAX_X, ' ');
	}
	return true;
}

void Board::reset() {
	for (size_t i = 0; i < MAX_Y; i++) {
		memcpy(currboard[i], originalBoard[i], MAX_X + 1);
	}
}

void Board::print(int score, int lives) {
	static int lastScore = score;
	static int lastLives = lives;
	static bool firstTime = true;

	if (firstTime || (lastScore != score) || (lastLives != lives)) {
		char num[7];
		sprintf(num, "%d", lives);
		strncpy(&(originalBoard[LPos.getY()][LPos.getX() + 7]), num, strlen(num));
		sprintf(num, "%d", score);
		strncpy(&(originalBoard[LPos.getY() + 1][LPos.getX() + 7]), num, strlen(num));

		lastScore = score;
		lastLives = lives;
		firstTime = false;
	}

	gotoxy(0, 0);
	for (size_t y = 0; y < MAX_Y - 1; y++) {
		for (size_t x = 0; x < MAX_X; x++) {
			cout << originalBoard[y][x];
		}
		cout << endl;
	}
	for (size_t x = 0; x < MAX_X; x++) {
		cout << originalBoard[MAX_Y - 1][x];
	}
}

void Board::setChar(int x, int y, char ch) {
	originalBoard[y][x] = ch;
}

char Board::getChar(int x, int y) const
{
	return originalBoard[y][x];
}

bool Board::isFloor(char ch) {
	for (char floorCh : FLOOR) {
		if (ch == floorCh)
			return true;  // Return true if the character is a floor type
	}
	return false;
}

void Board::printAllScreenFiles()
{
	clear_screen(); 
	gotoxy(0, 0);
	cout << "Choose the level you want to play" << endl;
	cout << endl; 
	for (int i = 0; i < allScreenFiles.size(); i++)
	{
		cout << setw(10) << (i + 1) << ". " << setw(15) << allScreenFiles[i] << endl;
	}
}

void Board::setExplodePos(int x, int y)
{
	explodePos = { x,y }; 
}

bool Board::loadAllLevels()
{
	if (allScreenFiles.empty())
		loadScreenFileList();
	else
	{
		///cout << "There isn't any screens to load" << endl;
		printAllScreenFiles();
		return true;
	}

	if (currentLevel >= allScreenFiles.size())
	{
		cout << "No more levels to load" << endl;
		return false;
	}
	return true;	
	
}


bool Board::resetToLevel(int level)
{
	if (level < START_LEVEL || level > getNuOfFiles())
		return false;
	currentLevel = level;
	return load(allScreenFiles[currentLevel - 1]);
}

void Board::handleSpecialChar(int y, int x)
{
	if (originalBoard[y][x] == '@') {
		if (!foundMario) {
			foundMario = !foundMario;
			startPos = { x,y };
		}
		deleteFromBoard(y, x);
	}
	
	else if (originalBoard[y][x] == '$') {
		if (!foundPaulin) {
			foundPaulin = !foundPaulin;
			endPos = { x,y };
		}
		else {
			deleteFromBoard(y, x);
		}
	}

	else if (originalBoard[y][x] == '&') {
		if (!foundDonkey) {
			foundDonkey = !foundDonkey; 
			donkeyPos = { x,y };
		}
		else {
			deleteFromBoard(y, x);
		}
	}

	else if (originalBoard[y][x] == 'p') {
		hammerPos = { x,y };
	}

	else if (originalBoard[y][x] == 'x') {
		ghostPos = { x,y }; 
		originalBoard[y][x] = ' ';
		ghosts.push_back(ghostPos);
	}

	else if (originalBoard[y][x] == 'L')
	{
		foundL = !foundL; 
		LPos = { x,y };
		originalBoard[y][x] = ' ';		
	}

}

void Board::deleteFromBoard(int y, int x)
{
	originalBoard[y][x] = ' ';
	gotoxy(x, y);
	cout << " ";
}

void Board::loadScreenFileList()
{
	allScreenFiles = findAllScreenFiles();
	if (allScreenFiles.empty()) {
		std::cout << "No screen files found!" << std::endl;
	}
	else {
		printAllScreenFiles(); 
	}
}

vector<string> Board::findAllScreenFiles()
{
	vector<string> allScreenFiles;
	const fs::path searchPath = fs::current_path();

	// Find all files matching pattern
	for (const auto& entry : fs::directory_iterator(searchPath)) {
		if (entry.is_regular_file()) {
			std::string filename = entry.path().filename().string();

			// Check if filename starts with "dkong_" and ends with ".screen"
			if (filename.substr(0, 6) == "dkong_" &&
				filename.substr(filename.length() - 7) == ".screen") {
				allScreenFiles.push_back(filename);
			}
		}
	}
	// Sort files lexicographically
	std::sort(allScreenFiles.begin(), allScreenFiles.end());

	return allScreenFiles;
}