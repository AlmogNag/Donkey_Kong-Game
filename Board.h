#pragma once
#include "Utils.h"
#include "Point.h"
#include <string>
#include <vector>
constexpr char LADDER = 'H';
constexpr char FLOOR[] = { '=', '>' , '<' };
constexpr int START_LEVEL = 1;

class Board {

	char originalBoard[MAX_Y][MAX_X + 1];
	char currboard[MAX_Y][MAX_X + 1];
	Point startPos;
	Point endPos;
	Point donkeyPos; 
	Point hammerPos;
	Point LPos = NULL;
	Point ghostPos;
	Point explodePos; 
	int currentLevel = START_LEVEL;
	vector<string> allScreenFiles;
	vector<Point> ghosts;
	bool foundPaulin = false;
	bool foundMario = false;
	bool foundDonkey = false;
	bool foundL = false;
	string lastError; 

public:
	bool load(const std::string& file_name); 
	void reset();
	void print(int score, int lives);
	void setChar(int x, int y, char ch);
	char getChar(int x, int y) const;
	bool isFloor(char ch);
	bool loadAllLevels();
	int getCurrentLevel() const { return currentLevel; }
	bool resetToLevel(int level);
	void handleSpecialChar(int y, int x);
	void printAllScreenFiles();
	Point getExplodePos() { return explodePos; }
	void setExplodePos(int x, int y); 
	Point getStartPos() const { return startPos; }
	Point getEndPos() const { return endPos; }	
	Point getDonkyPos() const { return donkeyPos; }
	Point getHammerPos() const { return hammerPos; }
	Point getLPos() const { return LPos; }
	vector <Point> getGhosts() const { return ghosts; }
	size_t getNuOfFiles() const { return allScreenFiles.size(); }
	void deleteFromBoard(int x, int y); 

private:
	void loadScreenFileList(); 
	static vector<string> findAllScreenFiles();
};