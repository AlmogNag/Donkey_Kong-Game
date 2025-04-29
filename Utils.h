#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

constexpr int ESC = 27;
static constexpr int MAX_X = 80;
static constexpr int MAX_Y = 25;
struct Direction { int x, y; };
void gotoxy(int x, int y); 
void hideCursor(); 
void clear_screen(); 