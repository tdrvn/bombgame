#include <chrono>
#include <initializer_list>
#include <stdio.h>
#include <stdlib.h>
#include "headers/game.h"
#include "headers/protocol.h"
#include "headers/viewer.h"
#include "pthread.h"
#include <thread>

typedef std::chrono::high_resolution_clock Clock;
auto lastRenderTime = Clock::now();

char* pipeFiles[NUMBER_OF_PLAYERS][2];
FILE *pipes[NUMBER_OF_PLAYERS][2];

std::string TeamNames[2];
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

GameTable hiddenTable[2];

void nextTick(int whatAction) {
	if(currentTick >= MAX_TICKS){
		gameState = DRAW;
		table.gameState = DRAW;
		nextTable.gameState = DRAW;
		communicationEnded = true;
		return;
	}
	//auto currentTime = Clock::now();
	//int duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastRenderTime).count();
	
	if (!communicationEnded) {
		if(whatAction == 0){
		//	printf("nextTick\n");
			
			PlayerMessage currentMessage;
			for(int currentPlayer = 0; currentPlayer < NUMBER_OF_PLAYERS; ++currentPlayer)
			{
				oldPlayerMessages[currentPlayer] = playerMessages[currentPlayer];
				playerMessages[currentPlayer] = PlayerMessage({MOVE_STAY, MOVE_STAY, MOVE_STAY, MOVE_STAY});
			}
			for(int i = 0; i < MAX_SPEED; i++)
				makeMovesTick(nextTable, oldPlayerMessages, i);
			gameState=nextTable.gameState;
			hiddenTable[0] = hidePlayers(nextTable, 0);
			hiddenTable[1] = hidePlayers(nextTable, 1);
			currentTick++;
			if(currentTick % 100 == 0)
				++DEFAULT_RESPAWN_TIME;
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
  FILE* pipes[2];
};

pthread_mutex_t gameLock;

void *playerCommunication(void* arg) {
  
	ThreadArgs* args = (ThreadArgs*)arg;
	printf("Thread communication %d started.\n", args->playerID);
	
  
  /*
	printf("Waiting for initial message from player %d\n", args->playerID);
	PlayerInitMessage m;
	receiveInitPlayer( pipes[1] , &m);
	table.players[args->playerID].classType = m.type;
	printf("Player %d is of class %d\n", args->playerID, table.players[args->playerID].classType);
  
  */
  
  
  
  
	int lastTick =  -1;
	auto lastTime = Clock::now();
	while (gameState == PLAYING) {
		if(currentTick != lastTick) {
		
			PlayerMessage currentMessage;
			int errorCode = sendGameState(args->pipes[0], ServerMessage{hiddenTable[args->playerID/5], args->playerID}) || receiveMove(args->pipes[1], &currentMessage);
			if(errorCode)
				playerMessages[args->playerID] = PlayerMessage({MOVE_STAY, MOVE_STAY, MOVE_STAY, MOVE_STAY});
			else
				playerMessages[args->playerID] = currentMessage;
			lastTick = currentTick;
		} else {
			std::this_thread::sleep_for(std::chrono::milliseconds(60));
		}
	}
	//Final gamestate
  
	sendGameState(args->pipes[0], ServerMessage{nextTable, args->playerID}); 
	fclose(args->pipes[0]);
	fclose(args->pipes[1]);
	pthread_exit(NULL);
}


std::string _classes[3]={"BOMBER","NINJA","TANK"};

int main(int argc, char** argv) {
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	assert(pthread_mutex_init(&gameLock, NULL) == 0);
	pthread_t threads[NUMBER_OF_PLAYERS];
	ThreadArgs threadArgs[NUMBER_OF_PLAYERS];
	
	for(int i = 0; i < NUMBER_OF_PLAYERS; i++){
		threadArgs[i].pipes[0] = fopen( argv[2*i + 2], "wb"); // first write then read
		threadArgs[i].pipes[1] = fopen( argv[2*i + 1], "rb");
		threadArgs[i].playerID =  i;
	}
	
	TeamNames[0]=argv[2*NUMBER_OF_PLAYERS + 1];
	TeamNames[1]=argv[2*NUMBER_OF_PLAYERS + 2];
	
	for(int i = 0; i < NUMBER_OF_PLAYERS; i++)
		table.players[i].classType = UNKNOWN_CLASS;
	for(int i = 0; i < NUMBER_OF_PLAYERS/NUMBER_OF_TEAMS; i++){
		int newClass[2];
		for(int j = 0; j <=1 ; j++){
			
			PlayerInitMessage m;
			printf("Waiting for initial message from player %d\n", i + j * 5);
			sendGameState(threadArgs[i +  j * 5].pipes[0], ServerMessage{table, i + j * 5});
			
			receiveInitPlayer( threadArgs[i + j * 5].pipes[1] , &m);
			
			newClass[j] = m.type;
			
		}
		
		table.players[i].classType = newClass[0];
		printf("Player %d is a %s\n", i, _classes[table.players[i].classType].c_str());
		
		table.players[i + 5].classType = newClass[1];
		printf("Player %d is a %s\n", i + 5, _classes[table.players[i + 5].classType].c_str());
		
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	
	gameState = PLAYING;
	initGameTable(table);
	nextTable = table;
	
	hiddenTable[0] = hidePlayers(nextTable, 0);
	hiddenTable[1] = hidePlayers(nextTable, 1);
	
	communicationEnded = false;
	currentTick = 0;
	
	for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
		assert(pthread_create(&threads[i], NULL,
        playerCommunication, &threadArgs[i]) == 0);
		printf("Main Thread\n");
	}

  //
	
	
	mainLoop(argc, argv);//
	return 0;
}
