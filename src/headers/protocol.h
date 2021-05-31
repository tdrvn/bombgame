#pragma once

#include "game.h"
#include <stdio.h>

// sent by server, received by players
struct ServerMessage {
	GameTable table;
	int currentPlayer;
};

struct PlayerInitMessage{
	int type;
};

void sendMove(FILE *pipe, PlayerMessage m);

int receiveMove(FILE *pipe, PlayerMessage *m);

int sendGameState(FILE *pipe, ServerMessage m);

void receiveGameState(FILE *pipe, ServerMessage *m);



void receiveInitPlayer(FILE *pipe, PlayerInitMessage *m);
int sendInitPlayer(FILE *pipe, PlayerInitMessage m);
