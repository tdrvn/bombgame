#include <initializer_list>
#include <stdio.h>
#include <stdlib.h>
#include "../src/headers/game.h"
#include "../src/headers/protocol.h"

int main(int argc, char** argv) {
  char* pipeFiles[2];
  pipeFiles[1] = argv[1]; // first where the player writes, 2nd where the player reads
  pipeFiles[0] = argv[2];
  FILE *pipes[2];
  pipes[0] = fopen(pipeFiles[0], "rb");
  pipes[1] = fopen(pipeFiles[1], "wb");

  ServerMessage serverMessage;
  
  PlayerMessage myMessage;
  
  receiveGameState(pipes[0], &serverMessage);
  
  int IAm = serverMessage.currentPlayer;
  while (serverMessage.table.gameState == PLAYING) {
	for(int i = 0; i < serverMessage.table.players[IAm].speed; ++i)
	{
		if(rand()%20==0)
			myMessage.actions[i] = MOVE_BOOM;
		else
			myMessage.actions[i] = rand() % 5;
	}
    sendMove(pipes[1], myMessage);
    receiveGameState(pipes[0], &serverMessage);
  }
  
  for (int i : {0, 1}) {
    fclose(pipes[i]);
  }
  printf("Player ends gracefully!\n");
  return 0;
}
