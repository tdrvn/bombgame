#include "headers/game.h"
#include <stdio.h>
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
/*
Coordinates GetNewCoordinates(Coordinates oldPos, int moveType, bool &willExplode){
	Coordinates newPos = oldPos;
	if(move == MOVE_UP){
		newPos.row
	}
}
void makeMovesTick(GameTable &table, PlayerMessage msg[NUMBER_OF_PLAYERS]){
	
}
*/
