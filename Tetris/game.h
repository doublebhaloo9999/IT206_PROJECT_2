#ifndef GAME_H
#define GAME_H

#include <vector>
#include <windows.h>
#include <string>

void startGame(bool advancedMode);
void initialize();
void resetGameState();
void gameLoop();
void draw(HANDLE hConsole, COORD bufferSize, CHAR_INFO* buffer);

#endif