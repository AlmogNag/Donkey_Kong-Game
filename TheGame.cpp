#include "TheGame.h"
bool TheGame::lose_level = false;

void TheGame::init() {
    mario.setArrowKeys("wsadxp");
    mario.setBoard(game_board);

    auto current_time = static_cast<unsigned int>(time(nullptr) & 0xFFFFFFFF);
    srand(current_time);
}

void TheGame::createGhosts() {
    vector<Point> ghostPositions = game_board.getGhosts();
    ghosts.clear();
    for (const Point& ghostPos : ghostPositions) {
        Ghost ghost(ghostPos.getX(), ghostPos.getY());
        ghost.setBoard(game_board);
        ghost.setDirection(rand() % 2 == 0 ? RIGHT : LEFT);
        ghosts.push_back(ghost);
    }
    for (auto& ghost : ghosts) {
        ghost.draw();
    }
}

void TheGame::checkCollisions() {
    int marioX = mario.getPos().getX();
    int marioY = mario.getPos().getY();

    for (const auto& barrel : barrels) {
        if (barrel.getX() == marioX && barrel.getY() == marioY) {
            mario.loseLife();
            lose_level = true;
            handleloseLevel();
            return; 
        }
    }
    for (const auto& ghost : ghosts) {
        if (ghost.getX() == marioX && ghost.getY() == marioY) {
            mario.loseLife();
            lose_level = true;
            handleloseLevel();
            return; 
        }
    }
}

bool TheGame::restartLevel() {
    if (!game_board.resetToLevel(startLevel)) {
        return false;
    }
    createGhosts();
    hideCursor();
    game_board.print(score, mario.getLives());
    mario.setBoard(game_board);
    mario.init();
    Point startPoint = game_board.getStartPos();
    mario.setPos(startPoint.getX(), startPoint.getY());
    mario.getPos().draw();
    return true;
}

void TheGame::run() {
    DirectionType dir;
    char directionKey = 0;
    char menuKeys[MAX_KEY_PRESSED];
    bool startGame = false;
    bool isPause = false;
    bool flag = false;
    int barrelCount = 0;
    char levelKey;
    static bool firstTime = true;
	
    // Display menu
    clear_screen();
    menu.print();
    hideCursor();

    // Main game loop
    do {
        int n = 0;
        for (int i = 0; i < 2; i++) {
            if (_kbhit()) {
                menuKeys[i] = _getch();
                n++;
            }
        }
        if (n == 1) {
            // Handle menu state
            if (!startGame) {
                startGame = menu.handleInput(menuKeys[0]);
                if (startGame) {
                    if (game_board.loadAllLevels()) {
                        levelKey = _getch(); 
                        startLevel = menu.handleInputLevel(levelKey, (int)game_board.getNuOfFiles(), lose_level);
                        if (startLevel == -1 && mario.getLives() > 0) {
                            handleWinGame();
                        }
                        else if (mario.getLives() > 0) {
                            if (!restartLevel()) {
                                Sleep(1000);
                                startGame = false;
                                menu.print();
                                continue;
                            }
                        }
                        else {
                            gameOver = true;
                            break;
                        }
                        
                    }            
                }
                continue;
            }
            // Handle game input
            if (menuKeys[0] == ESC) {
                isPause = !isPause;
                if (isPause) {
                    gotoxy(MAX_X / 2 - 15, MAX_Y / 2);
                    std::cout << "Game Pause - press ESC to resume";
                }
                else {
                    game_board.print(score, mario.getLives());
                    mario.getPos().draw();

                    for (auto& barrel : barrels) {
                        barrel.draw();
                    }
					for (auto& ghost : ghosts) {
						ghost.draw();
					}
                }
            }
            else {
                int result = mario.keyPressed(menuKeys[0]);
                if (result != -1) {
                    dir = (DirectionType)result;
                    mario.setDirection(dir);
                }
            }
            flag = true;
        }
        else if (n == 2) {
            bool found = false;
            if (menuKeys[0] == 'p') {
                swap(menuKeys[0], menuKeys[1]);
                found = true;
            }
            else if (menuKeys[1] == 'p') {
                found = true;
            }
            if (found) {
                mario.hitByMove(menuKeys, barrels, ghosts);
            }
        }
        else if (!isPause && startGame) {
            mario.resetKeyPress();
            flag = true;
        }
        if (mario.getpauline())
        {
            score += 20; 
			lose_level = false;
            handleWinLevel();
        }

        if (flag && !isPause && startGame) {
            barrelCount++;

            for (auto& barrel : barrels) {
                barrel.erase();
            }

            for (auto& ghost : ghosts) {
                ghost.erase();
            }
            if (barrelCount % 30 == 0) {
                barrels.push_back(Barrel::spawnBarrel(game_board));
            }

            for (auto it = barrels.begin(); it != barrels.end();) {
                it->move();
                
                if (it->hasExploded() || it->atTheEnd()) {
                    it = barrels.erase(it);
                }
                else {
                    it->draw();
                    ++it;
                }
            }
            for (auto& ghost : ghosts) {
                ghost.erase();
                ghost.move();
                ghost.draw();
            }
            if (menuKeys[0] == EXIT)
                exit(0);
            game_board.print(score, mario.getLives());
            if (!mario.move(barrels, ghosts) && (mario.getLives() > 0)) {
                handleloseLevel();
                continue;
            }

            // Check for collisions
            checkCollisions();
        }
        if (mario.hit(barrels, ghosts)) {
            score += 10; 
        }
        Sleep(100); 
    } while (!gameOver);

}

void TheGame::handleloseLevel() {
	lose_level = true;
    gotoxy(0,0);
    menu.printLoseLevel(); 
    Sleep(2000);
    gotoxy(0, 0); 
    restartLevel();
    return;
}

void TheGame::handleGameOver() {
    gotoxy(0,0);
    menu.printLoseGame(); 
    Sleep(2000);
    exit(0); 
}

void TheGame::handleWinLevel()
{
    gotoxy(0,0);
    menu.printWinLevel();
    Sleep(2000);
    gotoxy(0,0);
    startLevel++;
    if (startLevel < game_board.getNuOfFiles()) {
        restartLevel();
    }
    else {
        handleWinGame();
    }
}

void TheGame::handleWinGame()
{
    gotoxy(MAX_X / 2 - 15, MAX_Y / 2);
    std::cout << "YOU WIN!";
    Sleep(2000);
    gotoxy(0, 24);
    exit(0);
}