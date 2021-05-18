#pragma once

#include <stdio.h>
#include "game.h"

// sent by server, received by players
struct ServerMessage {
	GameTable table;
	int currentPlayer;
};

// sent by players, received by server
struct PlayerMessage {
	int actions[3];
	PlayerMessage(int first = 0, int second = 0, int third = 0){
			actions[0] = first;
			actions[1] = second;
			actions[2] = third;
	}
};

void sendMove(FILE *pipe, PlayerMessage m);

int receiveMove(FILE *pipe, PlayerMessage *m);

int sendGameState(FILE *pipe, ServerMessage m);

void receiveGameState(FILE *pipe, ServerMessage *m);
