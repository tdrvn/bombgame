#include <chrono>
#include <initializer_list>
#include <stdio.h>
#include <stdlib.h>
#include "headers/game.h"
#include "headers/protocol.h"
#include "headers/viewer.h"

typedef std::chrono::high_resolution_clock Clock;
auto lastRenderTime = Clock::now();

char* pipeFiles[NUMBER_OF_PLAYERS][2];
FILE *pipes[NUMBER_OF_PLAYERS][2];
int gameState;
GameTable table;
bool communicationEnded;

void display() {
  drawArena(gameState, &table);
}

void nextTick() {
	//auto currentTime = Clock::now();
	//int duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastRenderTime).count();

	if (gameState == PLAYING) {
		printf("nextTick\n");
		PlayerMessage playerMessages[NUMBER_OF_PLAYERS];
		PlayerMessage currentMessage;
		for(int currentPlayer = 0; currentPlayer < NUMBER_OF_PLAYERS; ++currentPlayer)
		{
			int errorCode = sendGameState(pipes[currentPlayer][0], ServerMessage{table, currentPlayer})
						 || receiveMove(pipes[currentPlayer][1], &currentMessage);
			if(errorCode)
				playerMessages[currentPlayer] = (MOVE_STAY, MOVE_STAY, MOVE_STAY);
			else
				playerMessages[currentPlayer] = currentMessage;
		}
      makeMovesTick(table, playerMessages);
      gameState=table.gameState;
      assert(gameState!=ERROR);
    } else {
		if (!communicationEnded) {
			communicationEnded = true;
        
			for (int currentPlayer = 0; currentPlayer < NUMBER_OF_PLAYERS; ++currentPlayer) {
			sendGameState(pipes[currentPlayer][0], ServerMessage{table, currentPlayer});
			}
			for (int i = 0; i < NUMBER_OF_PLAYERS; ++i) {
				for (int j : {0, 1}) {
					fclose(pipes[i][j]);
				}
			}
		}
    }
  return;
}

int main(int argc, char** argv) {

    
	for (int i = 0; i < NUMBER_OF_PLAYERS; ++i)
	{
		pipeFiles[i][1] = argv[2 * i + 1]; // first input read file, then write file
		pipeFiles[i][0] = argv[2 * i + 2];
	}

	for (int i = 0; i < NUMBER_OF_PLAYERS; ++i)
	{
		pipes[i][0] = fopen(pipeFiles[i][0], "wb");
		pipes[i][1] = fopen(pipeFiles[i][1], "rb");
	}
  
	gameState = PLAYING;
	initGameTable(table);
	communicationEnded = false;
  
	mainLoop(argc, argv);
	return 0;
}
