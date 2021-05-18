#pragma once
#include "protocol.h"


// map parameteres
const int ROWS = 100;
const int COLUMNS = 100;
const int NUMBER_OF_PLAYERS = 10;
/* ids are from 0 to 9
 * 0-4 are red team
 * 5-9 are blue team
 */ 

//player parametres
const int NUMBER_OF_TEAMS = 2; 
const char RED_TEAM = 0;
const char BLUE_TEAM = 1;

//game states
const int ERROR = 0;
const int BLUE_WINS = 1;
const int RED_WINS = 2;
const int DRAW = 3;
const int PLAYING = 4;

//move constants
const int MOVE_STAY = 0;
const int MOVE_UP = 1;
const int MOVE_LEFT = 2;
const int MOVE_DOWN = 3;
const int MOVE_RIGHT = 4;
const int MOVE_BOOM = 5;

//TODO : SET SPEEDS
const int DEFAULT_SPEED = 3;
const int SLOW_SPEED = 2;


/* TODO: make it constant
*/
const int DEFAULT_MAP[ROWS][COLUMNS]={0};
const int CELL_FREE = 0;
const int CELL_OCCUPIED = 1;

struct Coordinates{
/* keeps coordinates of a point
 */
	int row, col;
	Coordinates(int _row, int _col) : row(_row), col(_col){}
};

//spawn positions
const Coordinates spawns[NUMBER_OF_TEAMS] = {(Coordinates){0, 0},
											 (Coordinates){ROWS, COLUMNS}};
struct PlayerState{
	Coordinates position;
	int speed;
	int respawnTime; // is alive if respawnTime = 0, in ms
	
	char team;
	bool hasFlag;
};
struct FlagState{
	Coordinates position;
	
	int isAtPlayer; 
};

struct GameTable{
	PlayerState players[NUMBER_OF_PLAYERS];
	FlagState flags[NUMBER_OF_TEAMS];
	int gameState;
};

void makeMovesTick(GameTable &table, PlayerMessage msg[NUMBER_OF_PLAYERS]);
void initGameTable(GameTable &table);
void getDefaultMap();
