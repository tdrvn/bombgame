#pragma once
#include "game.h"
#include <string>

extern std::string TeamNames[2];
extern int currentTick;
const int MAX_TICKS = 1500;
void drawArena(int gameState, GameTable *table);

int mainLoop(int argc, char **argv);

void display();

void nextTick(int whatAction);
