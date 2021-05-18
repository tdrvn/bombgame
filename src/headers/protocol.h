#pragma once

#include "game.h"
#include <stdio.h>

// sent by server, received by players
struct ServerMessage {
	GameTable table;
	int currentPlayer;
};


void sendMove(FILE *pipe, PlayerMessage m);

int receiveMove(FILE *pipe, PlayerMessage *m);

int sendGameState(FILE *pipe, ServerMessage m);

void receiveGameState(FILE *pipe, ServerMessage *m);
