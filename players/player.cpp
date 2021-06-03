#include <initializer_list>
#include <stdio.h>
#include <stdlib.h>
#include "../src/headers/game.h"
#include "../src/headers/protocol.h"
#include <queue>
#include <time.h>
#include <thread>
#include <chrono>

void PlayRandom(FILE *readP, FILE *writeP, int RANDOM_SEED){
	srand(RANDOM_SEED);
	ServerMessage serverMessage;
	  
	PlayerMessage myMessage({});
	  
	receiveGameState(readP, &serverMessage);
	  
	int IAm = serverMessage.currentPlayer;
	while (serverMessage.table.gameState == PLAYING) {
		
		
		for(int i = 0; i < serverMessage.table.players[IAm].speed; ++i)
		{
			if((rand()^rand())%500==0)
				myMessage.actions[i] = MOVE_ABILITY;
			else
				myMessage.actions[i] = (rand()^rand()) % 5;
		}
		sendMove(writeP, myMessage);
		receiveGameState(readP, &serverMessage);
	}
}
//

void PlayTarget(FILE *readP, FILE *writeP){
	ServerMessage serverMessage;
	int dr[5] = {0, -1, 0, 1, 0};
	int dc[5] = {0, 0, -1, 0, 1};
	
	PlayerMessage myMessage;
	
	receiveGameState(readP, &serverMessage);
	int IAm = serverMessage.currentPlayer;
	int team = IAm / 5;
	while (serverMessage.table.gameState == PLAYING) {
		PlayerState me = serverMessage.table.players[IAm];
		Coordinates target = spawns[1 - team][rand()%2];
		for(int i = 0; i < NUMBER_OF_PLAYERS; i++){
			if(serverMessage.table.players[i].team != team && serverMessage.table.players[i].position != UNKNOWN_POSITION)
				target = serverMessage.table.players[i].position;
		}
		int prev[ROWS][COLUMNS];
		int dist[ROWS][COLUMNS];
		for(int i = 0; i < ROWS; i++){
			for(int j = 0; j < COLUMNS; j++){
				prev[i][j] = -1;
				dist[i][j] = 0;
			}
		}
		
		std::queue<Coordinates> q = std::queue<Coordinates> ();
		q.push(target);
		prev[target.row][target.col] = MOVE_STAY;
		dist[target.row][target.col] = 1;
		//printf("%d\n",DEFAULT_MAP[0][0]);
		while(q.size()){
			Coordinates cur = q.front();
			q.pop();
			assert(DEFAULT_MAP[cur.row][cur.col] == CELL_FREE);
			for(int i = 0; i < 5; i++){
				Coordinates ncur(cur.row + dr[i], cur.col + dc[i]);
				if(DEFAULT_MAP[ncur.row][ncur.col] == CELL_FREE && prev[ncur.row][ncur.col] == -1){
					prev[ncur.row][ncur.col] = i;
					dist[ncur.row][ncur.col] = 1 + dist[cur.row][cur.col];
					q.push(ncur);
				}
			}
			if(prev[me.position.row][me.position.col] != -1)
				break;
		}
		Coordinates pos = me.position;
		for(int i = 0; i < serverMessage.table.players[IAm].speed; ++i)
		{
			//printf("%d %d %d\n", pos.row, pos.col, prev[pos.row][pos.col]);
			int val = prev[pos.row][pos.col];
			int dst = dist[pos.row][pos.col];
			if(dst <= 4){
				myMessage.actions[i] = MOVE_ABILITY;
				continue;
			}
				
			//printf("%d %d %d\n\n", pos.row, pos.col, val);
			if(val != MOVE_STAY)
				val = (val + 1)%4 + 1; // mutarea opusa
			//printf("%d %d %d\n", pos.row, pos.col, val);
			myMessage.actions[i] = val;
			pos.row += dr[val];
			pos.col += dc[val];
		}
		//printf("New turn:\n\n");
		sendMove(writeP, myMessage);
		receiveGameState(readP, &serverMessage);
	}
	
}

void PlayAttack(FILE *readP, FILE *writeP){
	
	ServerMessage serverMessage;
	int dr[5] = {0, -1, 0, 1, 0};
	int dc[5] = {0, 0, -1, 0, 1};
	
	PlayerMessage myMessage;
	
	receiveGameState(readP, &serverMessage);
	  
	int IAm = serverMessage.currentPlayer;
	//printf("%d\n", serverMessage.table.gameState);
	while (serverMessage.table.gameState == PLAYING) {
		//std::this_thread::sleep_for(std::chrono::milliseconds(160)); //
		//printf("checkpoint 2\n");
		PlayerState me = serverMessage.table.players[IAm];
		Coordinates target;
		if(me.hasFlag)
			target = flag_home[me.team];
		else
			target = serverMessage.table.flags[1 - me.team].position;
		int prev[ROWS][COLUMNS];
		for(int i = 0; i < ROWS; i++){
			for(int j = 0; j < COLUMNS; j++){
				prev[i][j] = -1;
			}
		}
		
		std::queue<Coordinates> q = std::queue<Coordinates> ();
		q.push(target);
		prev[target.row][target.col] = MOVE_STAY;
		//printf("%d\n",DEFAULT_MAP[0][0]);
		while(q.size()){
			Coordinates cur = q.front();
			q.pop();
			assert(DEFAULT_MAP[cur.row][cur.col] == CELL_FREE);
			for(int i = 0; i < 5; i++){
				Coordinates ncur(cur.row + dr[i], cur.col + dc[i]);
				if(DEFAULT_MAP[ncur.row][ncur.col] == CELL_FREE && prev[ncur.row][ncur.col] == -1){
					prev[ncur.row][ncur.col] = i;
					q.push(ncur);
				}
			}
			if(prev[me.position.row][me.position.col] != -1)
				break;
		}
		Coordinates pos = me.position;
		for(int i = 0; i < serverMessage.table.players[IAm].speed; ++i)
		{
			//printf("%d %d %d\n", pos.row, pos.col, prev[pos.row][pos.col]);
			int val = prev[pos.row][pos.col];
			//printf("%d %d %d\n\n", pos.row, pos.col, val);
			if(val != MOVE_STAY)
				val = (val + 1)%4 + 1; // mutarea opusa
			//printf("%d %d %d\n", pos.row, pos.col, val);
			myMessage.actions[i] = val;
			pos.row += dr[val];
			pos.col += dc[val];
		}
		//printf("New turn:\n\n");
		sendMove(writeP, myMessage);
		receiveGameState(readP, &serverMessage);
	}
	
}

int main(int argc, char** argv) {
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	getDefaultMap();
	char* pipeFiles[2];
	pipeFiles[1] = argv[1]; // first where the player writes, 2nd where the player reads
	pipeFiles[0] = argv[2];
	FILE *pipes[2];
	pipes[0] = fopen(pipeFiles[0], "rb");
	pipes[1] = fopen(pipeFiles[1], "wb");
	int id = argv[1][20] - '0';
	sendInitPlayer(pipes[1], PlayerInitMessage{ (id%2)*2 } );
	if(argv[3][0] == 'r'){
		PlayRandom(pipes[0], pipes[1], argv[1][20]);
	}
	else if(argv[3][0] == 'A'){
		PlayAttack(pipes[0], pipes[1]);
	}
	else if(argv[3][0] == 'T'){
		srand(id * time(0));
		PlayTarget(pipes[0], pipes[1]);
	}
	for (int i : {0, 1}) {
		fclose(pipes[i]);
	}
	printf("Player ends gracefully!\n");//
	return 0;
}
