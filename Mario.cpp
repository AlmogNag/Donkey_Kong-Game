#include "Mario.h"
#include "TheGame.h"

void Mario::setArrowKeys(const char* keys) {
    if (!keys) return;
    for (int i = 0; i < NUM_OF_MOVES && keys[i] != '\0'; i++) {
        arrowKeys[i] = keys[i];
    }
}

void Mario::init() {
    direction = STAY;
    moveDirection = STAY;
    fallDistance = 0;
    isJumping = false;
    jumpPhase = 0;
    keyWasPressed = false;
    getHammer = '\0';
    hasHammer = false;
}

bool Mario::move(std::vector<Barrel>& barrels, std::vector<Ghost>& ghosts) {
    int currX = position.getX();
    int currY = position.getY();
    char currCh = pBoard->getChar(currX, currY);
    char underMe = pBoard->getChar(currX, currY + 1);
    position.saveLastChar(currCh);

    // Check for collision with explosion at current position
    if (nearTheExplosion()) {
        gotoxy(0, 0);
        std::cout << "Lives: " << life << " (Explosion damage!)";
        loseLife();
        return false;
    }

    updateMoveDirection();

    if (hasHammer && direction == HAMMER) {
        // CHECK IF THERE IS GHOST OR BARREL IN THE NEXT CHAR AND EXPLODE THEM 
        hit(barrels, ghosts);
        position.move(direction);
    }

    Point nextPosition = position;
    nextPosition.move(direction);

    if (direction != STAY) {
        if (pBoard->getChar(nextPosition.getX(), nextPosition.getY()) == 'Q')
            direction = STAY;
    }

    if (direction == UP)
        handleUpAndJump(currX, currY, currCh, underMe);
    else if (direction == DOWN)
        handleDown(currX, currY, underMe);
    else {
        keyWasPressed = false;
        if (!isJumping) {
            position.move(direction);
        }
    }

    // Prevent going through floor during jump
    if (isJumping) {
        int nextY = position.getY() - 1;
        char aboveChar = pBoard->getChar(position.getX(), nextY);

        if (pBoard->isFloor(aboveChar)) {
            isJumping = false;
            jumpPhase = 0;
        }
        else {
            executeJump(moveDirection);
        }
    }
    else {
        handleFalling(currX, currY, underMe);
    }

    updatePosition();
    return true;
}

void Mario::updateMoveDirection()
{
    if (direction == RIGHT || direction == LEFT) {
        moveDirection = direction;

        if (hasHammer) {
            if (direction == RIGHT)
                position.setSymbol('p');
            else if (direction == LEFT)
                position.setSymbol('q');
        }
    }
    else if (direction == STAY)
        moveDirection = STAY;
}

void Mario::handleUpAndJump(int currX, int currY, char currCh, char underMe)
{
    char nextCh = pBoard->getChar(currX, currY - 1);
    char nextNextCh = pBoard->getChar(currX, currY - 2);
    // Right path checks
    char firstStepR = pBoard->getChar(currX + 1, currY - 1);
    char secondStepR = pBoard->getChar(currX + 2, currY - 2);
    char thirdStepR = pBoard->getChar(currX + 3, currY - 1);
    char fourthStepR = pBoard->getChar(currX + 4, currY);
    // Left path checks 
    char firstStepL = pBoard->getChar(currX - 1, currY - 1);
    char secondStepL = pBoard->getChar(currX - 2, currY - 2);
    char thirdStepL = pBoard->getChar(currX - 3, currY - 1);
    char fourthStepL = pBoard->getChar(currX - 4, currY);

    // check if we're on near the ladder
    if (currCh == LADDER || nextCh == LADDER || pBoard->getChar(currX, currY + 1) == LADDER) {
        position.move(direction);
        moveDirection = STAY;
    }
    else if (!isJumping && !keyWasPressed &&
        (
            // Straight up path is clear
            (!pBoard->isFloor(nextCh) && !pBoard->isFloor(nextNextCh)) ||
            // Right path is clear
            (!pBoard->isFloor(firstStepR) && !pBoard->isFloor(secondStepR) &&
                !pBoard->isFloor(thirdStepR) && !pBoard->isFloor(fourthStepR)) ||
            //// Left path is clear
            (!pBoard->isFloor(firstStepL) && !pBoard->isFloor(secondStepL) &&
                !pBoard->isFloor(thirdStepL) && !pBoard->isFloor(fourthStepL))
            )
        ) {
        isJumping = true;
        jumpPhase = 1;
        keyWasPressed = true;
    }
    else {
        direction = STAY; 
        position.move(moveDirection);
    }
}

void Mario::handleDown(int currX, int currY, char underMe)
{
    char underch = pBoard->getChar(currX, currY + 1);
    char under_2_ch = pBoard->getChar(currX, currY + 2);

    if ((underch == LADDER || under_2_ch == LADDER || pBoard->getChar(currX, currY - 1) == LADDER) && !pBoard->isFloor(underch) ||
        pBoard->isFloor(underMe) && under_2_ch == LADDER) {

        position.move(direction);
        moveDirection = STAY;
    }
}

void Mario::executeJump(DirectionType moveDirection)
{
    if (moveDirection == RIGHT) {
        jump_on_move_R();
        moveDirection = RIGHT;
        direction = RIGHT;
    }
    else if (moveDirection == LEFT) {
        jump_on_move_L();
        moveDirection = LEFT;
        direction = LEFT;
    }
    else {
        jump();
    }
}

void Mario::handleFalling(int currX, int currY, char underMe)
{
    underMe = pBoard->getChar(currX, currY + 1);
    //bool isNowOnGround = !pBoard->isFloor(underMe);

    if ((direction == RIGHT || direction == LEFT || direction == STAY) && !isJumping && !pBoard->isFloor(underMe)) {
        position.move(DOWN);
        fallDistance++;
    }

    else
        fallDistance = 0;

    if (fallDistance == FATAL_FALL_HEIGHT)
        loseLife();
}

void Mario::handleUseHammer()
{
    if (hasHammer)
    {
        if (direction == RIGHT)
        {
            position.setSymbol('p');
            position.move(RIGHT);
        }
        else if (direction == LEFT)
        {
            position.setSymbol('q');
            position.move(LEFT);
        }
    }
}

bool Mario::hit(std::vector<Barrel>& barrels, std::vector<Ghost>& ghosts)
{
    if (direction == HAMMER)
    {
        int targetX = position.getX();
        int targetY = position.getY();

        if (moveDirection == RIGHT) {
            targetX++;
        }
        else if (moveDirection == LEFT) {
            targetX--;
        }
        else if (moveDirection == UP) {
            targetY++;
        }
        else if (moveDirection == DOWN) {
            targetY++;
        }

        gotoxy(targetX, targetY);
        cout << ' ' << endl;
        pBoard->setChar(targetX, targetY, ' ');

        for (std::vector<Barrel>::iterator it = barrels.begin(); it != barrels.end(); ++it) {
            Barrel& barrel = *it;
            if ((barrel.getX() == targetX) && (barrel.getY() == targetY)) {
                barrels.erase(it);
                return true;
            }
        }

        for (std::vector<Ghost>::iterator it = ghosts.begin(); it != ghosts.end(); ++it) {
            Ghost& ghost = *it;
            if ((ghost.getX() == targetX) && (ghost.getY() == targetY)) {
                ghosts.erase(it);
                return true;
            }
        }
    }
    return false; 
}

void Mario::hitByMove(char moveKeys[], std::vector<Barrel>& barrels, std::vector<Ghost>& ghosts)
{
    if (moveKeys[0] == 'd') {
        moveDirection = RIGHT;
        hit(barrels, ghosts);
    }
    else if (moveKeys[0] == 'a') {
        moveDirection = LEFT;
        hit(barrels, ghosts);
    }
    else if (moveKeys[0] == 'w') {
        moveDirection = UP;
        hit(barrels, ghosts);
    }
    else if (moveKeys[0] == 'x') {
        moveDirection = DOWN;
        hit(barrels, ghosts);
    }
    else if (moveKeys[0] == 's') {
        moveDirection = STAY;
        hit(barrels, ghosts);
    }
}


void Mario::jump() {
    int nextY;
    char nextChar;

    switch (jumpPhase) {
    case 1:
        nextY = position.getY() - 1;
        nextChar = pBoard->getChar(position.getX(), nextY);
        if (!pBoard->isFloor(nextChar)) {
            position.setY(nextY);
            jumpPhase = 2;
        }
        else {
            isJumping = false;
            jumpPhase = 0;
        }
        break;

    case 2:
        nextY = position.getY() - 1;
        nextChar = pBoard->getChar(position.getX(), nextY);
        if (!pBoard->isFloor(nextChar)) {
            position.setY(nextY);
            jumpPhase = 3;
        }
        else {
            isJumping = false;
            jumpPhase = 0;
        }
        break;

    case 3:
        position.setY(position.getY() + 1);
        jumpPhase = 4;
        break;

    case 4:
        position.setY(position.getY() + 1);
        isJumping = false;
        jumpPhase = 0;
        keyWasPressed = false;
        break;
    }
}

void Mario::jump_on_move_R()
{
    switch (jumpPhase) {
    case 1:
        position.setX(position.getX() + 1);
        position.setY(position.getY() - 1);
        jumpPhase = 2;
        break;

    case 2:
        position.setX(position.getX() + 1);
        position.setY(position.getY() - 1);
        jumpPhase = 3;
        break;
    case 3:
        position.setX(position.getX() + 1);
        position.setY(position.getY() + 1);
        jumpPhase = 4;
        break;
    case 4:
        position.setX(position.getX() + 1);
        position.setY(position.getY() + 1);
        isJumping = false;
        jumpPhase = 0;
        keyWasPressed = false;  // Reset the key pressed flag when jump is complete
        break;
    }
}

void Mario::jump_on_move_L()
{
    switch (jumpPhase) {
    case 1:
        position.setX(position.getX() - 1);
        position.setY(position.getY() - 1);
        jumpPhase = 2;
        break;

    case 2:
        position.setX(position.getX() - 1);
        position.setY(position.getY() - 1);
        jumpPhase = 3;
        break;
    case 3:
        position.setX(position.getX() - 1);
        position.setY(position.getY() + 1);
        jumpPhase = 4;
        break;
    case 4:
        position.setX(position.getX() - 1);
        position.setY(position.getY() + 1);
        isJumping = false;
        jumpPhase = 0;
        keyWasPressed = false;  // Reset the key pressed flag when jump is complete
        break;
    }
}

void Mario::updatePosition()
{
    gotoxy(position.getX(), position.getY());
    std::cout << position.getSymbol();
}

bool Mario::getpauline()
{
    if (position.getX() == pBoard->getEndPos().getX() && position.getY() == pBoard->getEndPos().getY())

        return true;
    return false;
}

void Mario::loseLife()
{
    if (life > 0) {
        life--;
        /*
        if (life == 0) {
            TheGame::handleGameOver();
        }
        */
    }
}

bool Mario::nearTheExplosion() {
    int currX = position.getX();
    int currY = position.getY();
    Point explodePos = pBoard->getExplodePos();
    if ((explodePos.getX() == 0) && (explodePos.getY() == 0)) {
        return false;
    }
    pBoard->setExplodePos(0, 0);
    if ((abs(explodePos.getX() - currX) <= 3) && (abs(explodePos.getY() - currY) <= 3)) {
        return true;
    }

    return false;
}

int Mario::keyPressed(char key) {
    int answer = -1;
    for (size_t i = 0; i < NUM_OF_MOVES; ++i) {
        if (tolower(key) == arrowKeys[i]) {
            setDirection(static_cast<DirectionType>(i));
            answer = (int)i;
            break;
        }
    }
    if (key == 'p') {
        tryToPickHammer('p');
    }
    return answer;
}