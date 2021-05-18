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
		PlayerMessage playerMessages[10];
		for(int i = 0; i < NUMBER_OF_PLAYERS; ++i)
		{
			int errorCode = sendGameState(pipes[currentPlayer][0], ServerMessage{table, i})
						 || receiveMove(pipes[currentPlayer][1], &playerMessage);
		}
      gameState = move(&board, playerMessage.row, playerMessage.col);
      if (gameState == INVALID_MOVE) {
        if (currentPlayer == 0) {
          gameState = ZERO_WINS;
        } else {
          gameState = X_WINS;
        }
      } else if (gameState == PLAYING) {
        currentPlayer = 1 - currentPlayer;
      }
    } else {
      if (!communicationEnded) {
        communicationEnded = true;
        writeBoard();
        
        for (int i : {0, 1}) {
          sendGameState(pipes[i][0], ServerMessage{
              gameState,
              board});
        }
        for (int i : {0, 1}) {
          for (int j : {0, 1}) {
            fclose(pipes[i][j]);
          }
        }
      }
    }
  display();
  return;
}

int main(int argc, char** argv) {

    
	for (int i = 0; i < NUMBER_OF_PLAYERS; ++i)
	{
		pipeFiles[i][0] = argv[2 * i + 1];
		pipeFiles[i][1] = argv[2 * i + 2];
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
