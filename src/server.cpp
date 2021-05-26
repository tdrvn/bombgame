#include <chrono>
#include <initializer_list>
#include <stdio.h>
#include <stdlib.h>
#include "headers/game.h"
#include "headers/protocol.h"
#include "headers/viewer.h"
#include "pthread.h"

typedef std::chrono::high_resolution_clock Clock;
auto lastRenderTime = Clock::now();

char* pipeFiles[NUMBER_OF_PLAYERS][2];
FILE *pipes[NUMBER_OF_PLAYERS][2];

int gameState;
GameTable table;
bool communicationEnded;
int currentTick;

void display() {
  drawArena(gameState, &table);
}
PlayerMessage playerMessages[NUMBER_OF_PLAYERS];
PlayerMessage oldPlayerMessages[NUMBER_OF_PLAYERS];


GameTable nextTable;


void nextTick(int whatAction) {
	//auto currentTime = Clock::now();
	//int duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastRenderTime).count();
	
	if (!communicationEnded) {
		if(whatAction == 0){
		//	printf("nextTick\n");
			
			PlayerMessage currentMessage;
			for(int currentPlayer = 0; currentPlayer < NUMBER_OF_PLAYERS; ++currentPlayer)
			{
				oldPlayerMessages[currentPlayer] = playerMessages[currentPlayer];
				playerMessages[currentPlayer] = (MOVE_STAY, MOVE_STAY, MOVE_STAY);
			}
			for(int i = 0; i < MAX_SPEED; i++)
				makeMovesTick(nextTable, oldPlayerMessages, i);
			gameState=nextTable.gameState;
			currentTick++;
		}
		makeMovesTick(table, oldPlayerMessages, whatAction);
		if(whatAction == MAX_SPEED - 1)
			table = nextTable;
		assert(gameState != ERROR);
		if(gameState != PLAYING && whatAction == MAX_SPEED - 1)
			communicationEnded = true;
    } 
  return;
}


struct ThreadArgs {
  int playerID;
  char* pipeFiles[2];
};

pthread_mutex_t gameLock;

void *playerCommunication(void* arg) {
  
  ThreadArgs* args = (ThreadArgs*)arg;
  printf("Thread communication %d started.\n", args->playerID);
  FILE *pipes[2];
  pipes[0] = fopen(args->pipeFiles[0], "wb");
  pipes[1] = fopen(args->pipeFiles[1], "rb");
  
  int lastTick =  -1;
  auto lastTime = Clock::now();
  while (gameState == PLAYING) {
	if(currentTick != lastTick){
		
		PlayerMessage currentMessage;
		int errorCode = sendGameState(pipes[0], ServerMessage{nextTable, args->playerID}) || receiveMove(pipes[1], &currentMessage);
		if(errorCode)
			playerMessages[args->playerID] = (MOVE_STAY, MOVE_STAY, MOVE_STAY);
		else
			playerMessages[args->playerID] = currentMessage;
		lastTick = currentTick;
    }
  }
  //Final gamestate
  
  sendGameState(pipes[0], ServerMessage{nextTable, args->playerID}); 
  fclose(pipes[0]);
  fclose(pipes[1]);
  pthread_exit(NULL);
}




int main(int argc, char** argv) {

	assert(pthread_mutex_init(&gameLock, NULL) == 0);
	pthread_t threads[NUMBER_OF_PLAYERS];
	ThreadArgs threadArgs[NUMBER_OF_PLAYERS];
	
	for(int i = 0; i < NUMBER_OF_PLAYERS; i++){
		threadArgs[i] = {i, argv[2*i + 2], argv[2*i + 1]}; // first write then read
	}
	
	
	gameState = PLAYING;
	initGameTable(table);
	nextTable = table;
	communicationEnded = false;
	currentTick = 0;
	
	for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
		assert(pthread_create(&threads[i], NULL,
        playerCommunication, &threadArgs[i]) == 0);
		printf("Main Thread\n");
	}

  
	
	
	mainLoop(argc, argv);//
	return 0;
}
