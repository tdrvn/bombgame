#include "headers/game.h"
#include <stdio.h>
#include <queue>
#include <vector>
#include <stdlib.h>
#include <time.h>
const int dr[5] = {0, -1, 0, 1, 0};
const int dc[5] = {0, 0, -1, 0, 1};
int _DISTANCE[3][ROWS][COLUMNS];
int DEFAULT_MAP[ROWS][COLUMNS];
int DEFAULT_RESPAWN_TIME = 10;
int DEFAULT_SLOW_TIME = 8;
int DEFAULT_INVISIBILITY_TIME = 5;
void getDefaultMap(){
	
	char readMap[ROWS][COLUMNS];
	
	FILE *mapLocation;
	mapLocation = fopen("./files/_default_map", "r");
	if(mapLocation == NULL)
		printf("ERROR READING MAP\n");
	for(int row = 0; row < ROWS; row++){
		if(fgets(readMap[row], COLUMNS + 2, mapLocation) == NULL )
			printf("ERROR READING COLUMN OF MAP\n");
	}
	fclose(mapLocation);
	
	for(int row = 0; row < ROWS; row++){
		for(int col = 0; col < COLUMNS; col++){
			if(readMap[row][col] == '0')
				DEFAULT_MAP[row][col] = CELL_FREE;
			else
			{
				DEFAULT_MAP[row][col] = CELL_OCCUPIED;
			}
		}
		
	}

}

void initGameTable(GameTable &table){
	getDefaultMap();
	for(int i = 0; i < NUMBER_OF_PLAYERS; ++i){
		table.players[i].hasFlag = false;
		if(i < NUMBER_OF_PLAYERS / 2)
			table.players[i].team = RED_TEAM;
		else
			table.players[i].team = BLUE_TEAM;
		table.players[i].respawnTime = 0;
		table.players[i].speed = DEFAULT_SPEED[0][table.players[i].classType];
		table.players[i].hp = DEFAULT_HP[table.players[i].classType];
		table.players[i].position = spawns[table.players[i].team][rand() % NUMBER_OF_SPAWNS];
		table.players[i].invisibleTime = 0;
		table.players[i].slowTime = 0;
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

void interactWithFlag(GameTable &table, int player){
	int currentTeam = table.players[player].team;
	int enemyTeam = 1 - currentTeam;
	
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
	}

}


void killPlayer (GameTable &table, int player){
	int currentTeam = table.players[player].team;
	if(table.players[player].hasFlag == true)
	{
		table.players[player].hasFlag = false;
		table.flags[1 - currentTeam].isAtPlayer = ON_GROUND;
		
		assert(table.flags[1 - currentTeam].position == table.players[player].position);
	}
	table.players[player].respawnTime = DEFAULT_RESPAWN_TIME;
	alive[player] = false;
	
	//maybe yes maybe no
	srand(time(0));
	table.players[player].position = spawns[table.players[player].team][rand() % NUMBER_OF_SPAWNS];
}


void respawnPlayer (GameTable &table, int player){
	
	//table.players[player].position = spawns[table.players[player].team][rand() % NUMBER_OF_SPAWNS];
	
	table.players[player].hp = DEFAULT_HP[table.players[player].classType];
	
	assert(table.players[player].hasFlag == false);
	
	interactWithFlag(table, player);
}

std::vector <Coordinates> allPos;
void resetAbility ()
{
	for(Coordinates coord : allPos)
		for(int i=0;i<=2;++i)
			_DISTANCE[i][coord.row][coord.col] = 0;
	allPos.clear();
}

void makeAbility(GameTable &table, int player, int whatAbility){
	table.players[player].cooldown = COOLDOWNS[whatAbility];
	std::queue <Coordinates> q;
	
	q.push(table.players[player].position);
	allPos.push_back(table.players[player].position);
	_DISTANCE[whatAbility][table.players[player].position.row][table.players[player].position.col] = 1;
	
	while(q.size()){
		Coordinates cur = q.front();
		
		allPos.push_back(cur);
		
		q.pop();
		for(int pl = 0; pl < NUMBER_OF_PLAYERS; pl++){
			if(table.players[pl].position == cur){
				if(whatAbility == BOMBER) {
					if(table.players[pl].hp)
						--table.players[pl].hp;
					if(table.players[pl].hp == 0)
						willDie[pl] = true;
				}
				else if(whatAbility == TANK && table.players[pl].team != table.players[player].team) {
					table.players[pl].slowTime = DEFAULT_SLOW_TIME;
				}
				else if(whatAbility == NINJA && table.players[pl].team == table.players[player].team) {
					table.players[pl].invisibleTime = DEFAULT_INVISIBILITY_TIME;
				}
			}
		}
		if(_DISTANCE[whatAbility][cur.row][cur.col] == RADIUS[whatAbility])
			continue;
		for(int dir = 1; dir<=4; dir++){
			Coordinates newcur(cur.row + dr[dir], cur.col + dc[dir]);
			if(DEFAULT_MAP[newcur.row][newcur.col] == CELL_FREE && _DISTANCE[whatAbility][newcur.row][newcur.col] == 0 && _DISTANCE[whatAbility][cur.row][cur.col] < RADIUS[whatAbility]){
				q.push(newcur);
				_DISTANCE[whatAbility][newcur.row][newcur.col] = 1 + _DISTANCE[whatAbility][cur.row][cur.col];
			}
		}
	}
	
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
	
	interactWithFlag(table, player);
	// Moving with the enemy flag:
	if(table.flags[enemyTeam].isAtPlayer == player)
	{
		table.flags[enemyTeam].position = table.players[player].position;
		
		assert(table.players[player].hasFlag == true);
	}
	
	// Is this all ? not sure
}


void makeMovesTick(GameTable &table, PlayerMessage msg[NUMBER_OF_PLAYERS], int whatAction){
	
	// Determine what players are alive
	if(whatAction == 0){
		for(int i = 0; i < NUMBER_OF_PLAYERS; ++i){
			if(table.players[i].respawnTime != 0){
				alive[i] = false;
			}
			else
				alive[i] = true;
			willDie[i] = false;
		}
	}
	resetAbility();
	// Doing the actual actions
	int currentAction = whatAction;
		for(int i = 0; i < NUMBER_OF_PLAYERS; ++i){
			if(alive[i] == false || table.players[i].speed <= currentAction)
				continue;
			if(msg[i].actions[currentAction] != MOVE_ABILITY)
				makeMove(table, i, msg[i].actions[currentAction]);
			else if(table.players[i].cooldown)
				makeMove(table, i, MOVE_STAY);
		}
		
		for(int i = 0; i < NUMBER_OF_PLAYERS; ++i){
			if(alive[i] == false || table.players[i].speed <= currentAction)
				continue;
			if(msg[i].actions[currentAction] == MOVE_ABILITY && !table.players[i].cooldown)
				makeAbility(table, i, table.players[i].classType);
		}
		
		for(int i = 0; i < NUMBER_OF_PLAYERS; ++i){
			if(willDie[i] == true)
				killPlayer(table, i); 
			willDie[i] = false;
		}
	
	// Respawn players / apply abilities
	if(whatAction == MAX_SPEED - 1){
		for(int i = 0; i < NUMBER_OF_PLAYERS; ++i){
			if(table.players[i].respawnTime != 0)
				--table.players[i].respawnTime;
			if(alive[i] == false && table.players[i].respawnTime == 0)
				respawnPlayer(table, i);
			table.players[i].slowTime = std::max(0, table.players[i].slowTime - 1);
			table.players[i].speed = DEFAULT_SPEED[table.players[i].hasFlag][table.players[i].classType] - (table.players[i].slowTime != 0);
			//printf("Player %d has speed: %d\n", i, table.players[i].speed);
			table.players[i].cooldown = std::max(0, table.players[i].cooldown - 1);
			table.players[i].invisibleTime = std::max(0, table.players[i].invisibleTime - 1);
		}
	}
}
//

bool inBounds(Coordinates pos){
	if(pos != UNKNOWN_POSITION && pos.row >= 0 && pos.row < ROWS && pos.col >= 0 && pos.col < COLUMNS)
		return true;
	return false;
}
int _VIEW_DISTANCE[ROWS][COLUMNS];
GameTable hidePlayers(GameTable table, int team){
	for(int i = 0; i < NUMBER_OF_PLAYERS; i++){
		if(inBounds(table.players[i].position) == false){
			printf("Wrong information stop cheating !! ");
			assert(false);
		}
	}
	GameTable newTable = table;
	std::queue <Coordinates> q;
	std::vector<Coordinates> visited;
	
	for(int i = team * 5; i < (team + 1) * 5; i++){
		q.push(table.players[i].position);
		visited.push_back(table.players[i].position);
		_VIEW_DISTANCE[table.players[i].position.row][table.players[i].position.col] = 1;
	}
	
	
	while(q.size()){
		Coordinates cur = q.front();
		
		visited.push_back(cur);
		
		q.pop();
		
		if(_VIEW_DISTANCE[cur.row][cur.col] == VIEW_RADIUS)
			continue;
		for(int dir = 1; dir<=4; dir++){
			Coordinates newcur(cur.row + dr[dir], cur.col + dc[dir]);
			if(DEFAULT_MAP[newcur.row][newcur.col] == CELL_FREE && _VIEW_DISTANCE[newcur.row][newcur.col] == 0 && _VIEW_DISTANCE[cur.row][cur.col] < VIEW_RADIUS){
				q.push(newcur);
				_VIEW_DISTANCE[newcur.row][newcur.col] = 1 + _VIEW_DISTANCE[cur.row][cur.col];
			}
		}
	}
	for(int i = (1 - team) * 5; i < (2 - team) * 5; i++){
		Coordinates pos = table.players[i].position;
		int manh_dist = abs(pos.row - flag_home[team].row) + abs(pos.col - flag_home[team].col);
		if((_VIEW_DISTANCE[pos.row][pos.col] == 0 || table.players[i].invisibleTime) && table.players[i].hasFlag == false && manh_dist > 13){
			newTable.players[i].position = UNKNOWN_POSITION;
		}
	}
	for(Coordinates pos : visited)
		_VIEW_DISTANCE[pos.row][pos.col] = 0;
	visited.clear();
	return newTable;
}
