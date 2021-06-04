#pragma once
#include <cassert>
#include <initializer_list>

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
const int RED_TEAM = 0;
const int BLUE_TEAM = 1;

const int TEAMS[2] = {RED_TEAM, BLUE_TEAM};

//PLAYER CLASSES CONSTANTS

const int BOMBER = 0;
const int NINJA = 1;
const int TANK = 2;


// not const since it might change over the course of the game
extern int DEFAULT_RESPAWN_TIME; // TBD
extern int DEFAULT_SLOW_TIME;
extern int DEFAULT_INVISIBILITY_TIME;

const int RADIUS[3] = {6, 4, 8};
const int VIEW_RADIUS = 10;

const int COOLDOWNS[3] = {9, 15, 10};

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
const int MOVE_ABILITY = 5;

const int DEFAULT_SPEED[2][3] = {{3, 4, 2},
								 {2, 3, 2}};
const int MAX_SPEED = 4;

const int DEFAULT_HP[3] = {1, 1, 2};

/* TODO: make it constant
*/
extern int DEFAULT_MAP[ROWS][COLUMNS];
extern int _DISTANCE[3][ROWS][COLUMNS];
const int CELL_FREE = 0;
const int CELL_OCCUPIED = 1;


/* keeps coordinates of a point
 */
struct Coordinates{
	int row, col;
	Coordinates() : row(0), col(0){}
	Coordinates(int _row, int _col) : row(_row), col(_col){}
	
	bool operator == (Coordinates other) const{
		return row == other.row && col == other.col;
	}
	bool operator != (Coordinates other) const{
		return !((*this) == other);
	}
};

const int NUMBER_OF_SPAWNS = 2;
//spawn positions! 
const Coordinates spawns[NUMBER_OF_TEAMS][NUMBER_OF_SPAWNS] ={ {(Coordinates){17, 70}, (Coordinates){28, 81} },
											    {				(Coordinates){82, 30}, (Coordinates){72, 19} }  
											  };
const Coordinates UNKNOWN_POSITION = {-1, -1};
//flag home positions
const Coordinates flag_home[NUMBER_OF_TEAMS] = {(Coordinates){12, 87},
											    (Coordinates){87, 12}};
											    
//flag constants
const int AT_HOME = -1;
const int ON_GROUND = -2;

struct PlayerState{
	Coordinates position;
	int speed;
	int respawnTime; // is alive if respawnTime = 0, in ticks
	int slowTime; // is not slowed if slowTime = 0, in ticks
	int invisibleTime; // is visible if invisibleTime = 0, in ticks
	int cooldown;
	
	int hp;
	int team;
	bool hasFlag;
	
	int classType;
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


// actions to be made by player
struct PlayerMessage {
	int actions[MAX_SPEED];
	PlayerMessage( std::initializer_list<int> listActions){
		int cnt = 0;
		for(int e:listActions){
			actions[cnt] = e;
			cnt++;
			if(cnt > MAX_SPEED)
				break;
		}
	}
	PlayerMessage() : PlayerMessage({}){
	}
};


void makeMovesTick(GameTable &table, PlayerMessage msg[NUMBER_OF_PLAYERS], int whatAction);
void initGameTable(GameTable &table);
void getDefaultMap();
GameTable hidePlayers(GameTable table, int team);
