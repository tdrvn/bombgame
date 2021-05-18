#include "headers/game.h"
#include <stdio.h>

const int dr[5] = {0, -1, 0, 1, 0};
const int dc[5] = {0, 0, -1, 0, 1};

void getDefaultMap(){
	char readMap[ROWS][COLUMNS];
	
	FILE *mapLocation;
	mapLocation = fopen("../files/_default_map", "r");
	for(int row = 0; row < ROWS; row++){
		fgets(readMap[row], ROWS + 1, mapLocation);
	}
	fclose(mapLocation);
	
	for(int row = 0; row < ROWS; row++){
		for(int cols = 0; cols < COLUMNS; cols++){
			if(readMap[row][cols] == '0')
				DEFAULT_MAP[row][cols] = CELL_FREE;
			else
				DEFAULT_MAP[row][cols] = CELL_OCCUPIED;
		}
		
	}
}

void initGameTable(GameTable &table){
	for(int i = 0; i < NUMBER_OF_PLAYERS; ++i){
		table.players[i].hasFlag = false;
		if(i < 5)
			table.players[i].team = RED_TEAM;
		else
			table.players[i].team = BLUE_TEAM;
		table.players[i].respawnTime = 0;
		table.players[i].speed = DEFAULT_SPEED;
		table.players[i].position = spawns[table.players[i].team];
	}
	for(int i = 0; i < NUMBER_OF_TEAMS; ++i){
		table.flags[i].isAtPlayer = AT_HOME;
		table.flags[i].position = flag_home[TEAMS[i]];
	}
	table.gameState = PLAYING;
}

void move(Coordinates &pos, int moveType){
	pos.row += dr[moveType];
	pos.col += dc[moveType];
	
	if(DEFAULT_MAP[pos.row][pos.col] == CELL_OCCUPIED){
		pos.row -= dr[moveType];
		pos.col -= dc[moveType];
	}
	
	assert(DEFAULT_MAP[pos.row][pos.col] == CELL_FREE);
}

bool alive[NUMBER_OF_PLAYERS];
bool willDie[NUMBER_OF_PLAYERS];

void killPlayer (GameTable &table, int player){
	int currentTeam = table.players[player].team;
	if(table.players[player].hasFlag == true)
	{
		table.players[player].hasFlag = false;
		table.flags[1 - currentTeam].isAtPlayer = ON_GROUND;
		
		assert(table.flags[1 - currentTeam].position == table.players[player].position);
	}
	table.players[player].respawnTime = DEFAULT_RESPAWN_TIME;
	table.players[player].speed = DEFAULT_SPEED;
}


// TODO: random spawn positions
void respawnPlayer (GameTable &table, int player){
	table.players[player].position = spawns[table.players[player].team];
	
	assert(table.players[player].hasFlag == false);
	assert(table.players[player].speed == DEFAULT_SPEED);
	// IMPORTANT !! IF RESPAWNING ON A FLAG, DO WHAT IS NECESSARY !!
}


void makeMove (GameTable &table, int player, int moveType){
	int currentTeam = table.players[player].team;
	int enemyTeam = 1 - currentTeam;
	
	// making the move:
	move(table.players[player].position, moveType);
	
	// Winning the game:
	if(table.players[player].hasFlag == true && table.players[player].position == flag_home[currentTeam])
	{
		if(table.gameState != PLAYING)
		{
			if(currentTeam == RED_TEAM)
			{
				if(table.gameState == DRAW || table.gameState == BLUE_WINS)
					table.gameState = DRAW;
			}
			else
			{
				if(table.gameState == DRAW || table.gameState == RED_WINS)
					table.gameState = DRAW;
			}
		}
		else
		{
			if(currentTeam == RED_TEAM)
				table.gameState = RED_WINS;
			else
				table.gameState = BLUE_WINS;
		}
	}
	
	// Resetting own flag position: (TBD if this stays)
	if(table.flags[currentTeam].isAtPlayer == ON_GROUND &&
	   table.flags[currentTeam].position == table.players[player].position){

		table.flags[currentTeam].isAtPlayer = AT_HOME;
		table.flags[currentTeam].position = flag_home[currentTeam];
	}
	
	// Picking up enemy flag:
	if((table.flags[enemyTeam].isAtPlayer == ON_GROUND ||
		table.flags[enemyTeam].isAtPlayer == AT_HOME) &&
		table.flags[enemyTeam].position == table.players[player].position){

		table.flags[enemyTeam].isAtPlayer = player;
		table.players[player].hasFlag = true;
		table.players[player].speed = SLOW_SPEED;
	}
	
	// Moving with the enemy flag:
	if(table.flags[enemyTeam].isAtPlayer == player)
	{
		table.flags[enemyTeam].position = table.players[player].position;
		
		assert(table.players[player].hasFlag == true);
	}
	
	// Is this all ? not sure
}


void makeMovesTick(GameTable &table, PlayerMessage msg[NUMBER_OF_PLAYERS]){
	
	// Determine what players are alive
	for(int i = 0; i < NUMBER_OF_PLAYERS; ++i){
		if(table.players[i].respawnTime != 0){
			alive[i] = false;
			--table.players[i].respawnTime;
		}
		else
			alive[i] = true;
		willDie[i] = false;
	}
	
	// Doing the actual actions
	for(int currentAction = 0; currentAction < DEFAULT_SPEED && table.gameState == PLAYING; ++currentAction){
		for(int i = 0; i < NUMBER_OF_PLAYERS; ++i){
			if(alive[i] == false || table.players[i].speed <= currentAction)
				continue;
			if(msg[i].actions[currentAction] != MOVE_BOOM)
				makeMove(table, i, msg[i].actions[currentAction]);
		}
		
		for(int i = 0; i < NUMBER_OF_PLAYERS; ++i){
			if(alive[i] == false || table.players[i].speed <= currentAction)
				continue;
			if(msg[i].actions[currentAction] == MOVE_BOOM)
				makeBoom(table, i); // TODO
		}
		
		for(int i = 0; i < NUMBER_OF_PLAYERS; ++i){
			if(willDie[i] == true)
				killPlayer(table, i); 
			willDie[i] = false;
		}
	}
	
	// Respawn players
	for(int i = 0; i < NUMBER_OF_PLAYERS; ++i){
		if(alive[i] == false && table.players[i].respawnTime == 0)
			respawnPlayer(table, i);
	}
}

