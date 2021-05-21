#pragma once
#include "game.h"

void drawArena(int gameState, GameTable *table);

int mainLoop(int argc, char **argv);

void display();

void nextTick(int whatAction);
