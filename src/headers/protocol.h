#pragma once

#include <stdio.h>
#include "headers/game.h"

// sent by server, received by players
struct ServerMessage {
	GameTable table;
	int currentplayer;
};

// sent by players, received by server
struct PlayerMessage {
	int move;
};

void sendMove(FILE *pipe, PlayerMessage m);

int receiveMove(FILE *pipe, PlayerMessage *m);

int sendGameState(FILE *pipe, ServerMessage m);

void receiveGameState(FILE *pipe, ServerMessage *m);
