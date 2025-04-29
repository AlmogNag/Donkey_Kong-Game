#pragma once
#include "Utils.h"
#include "Mario.h"
#include "Board.h"
#include "Barrel.h"
#include "Ghost.h"
#include "Menu.h"
#include <vector>
#include <cstdlib>
#include <ctime>

constexpr int  MAX_KEY_PRESSED =  2; 

class TheGame {
    enum { ESC = 27 };
    Mario mario;
    Point donkey; 
    Board game_board;
    Menu menu;
    int barrelSpawnTimer = 0;
	static bool lose_level;
    bool gameOver = false;
    int score = 0;
    std::vector<Barrel> barrels;
    std::vector<Ghost> ghosts;
    int startLevel = 1;

    void createGhosts();
    bool restartLevel();
public:
    void init();
    void run();
    void checkCollisions();
    void handleloseLevel();
    void handleGameOver();
    void handleWinLevel();
    void handleWinGame();
    void setScore(int num) { score = score + num; }
    vector<Barrel>& getBarrels() { return barrels;  }
    vector<Ghost>& getGhost() { return ghosts; }
};