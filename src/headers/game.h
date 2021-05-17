#pragma once

// map parameteres
const int ROWS = 101;
const int COLUMNS = 101;
const int NUMBER_OF_PLAYERS = 5;

//player parametres
const int NUMBER_OF_TEAMS = 2; 
const char RED_TEAM = 0;
const char BLUE_TEAM = 1;

//moves parametres
const int INVALID_MOVE = 0;
const int BLUE_WINS = 1;
const int RED_WINS = 2;
const int DRAW = 3;
const int PLAYING = 4;


//TODO : SET SPEEDS
const int DEFAULT_SPEED = 0;
const int SLOW_SPEED = 0;

/* TODO
const int MAP[ROWS][COLUMNS];
*/

struct Coordinates{
/* keeps coordinates of a point
 */
	int row, col;
	Coordinates(int _row, int _col) : row(_row), col(_col){}
}
struct PlayerState{
	Coordinates position;
	int speed;
	
	char team;
	bool hasFlag;
};

struct GameTable{
	PlayerState players[NUMBER_OF_TEAMS][NUMBER_OF_PLAYERS];
	
	
};



