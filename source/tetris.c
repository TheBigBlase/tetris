#include "tetris.h"
#include <stdlib.h>
#include <time.h>

void rotate(Piece * p){
	p->position ++;
}

void fall(Piece * p){
	//update piece location
	if(!p->isFalling){
		exit(-1);//can ignore this warning, this part is single thread
	}
	//check if can fall
	if(grid[p->location[0]][p->location[1]] == 1){ // TODO for all square not just center
		//if square full then lay piece
		p->isFalling = 0;
		
	}

	p->location[1] --;

}

Piece * newPiece(){
	int n = rand() % NUMBER_SHAPE;
	
}

char pieces[NUMBER_SHAPE][NUMBER_POSITION][4][4];

void initBar(){
	pieces[BAR][ZERO] = {
		{ 0, 0, 0, 0 }, 
		{ 0, 0, 0, 0 }, 
		{ 0, 0, 0, 0 }, 
		{ 0, 0, 0, 0 }
	};
}

void intiGame(){
	//init grid 
	char grid[GRID_Y][GRID_X];
	for(int k = 0 ; k < GRID_X ; k++){
		for(int l = 0 ; l < GRID_Y ; l ++){
			grid[k][l] = 0;
		}
	}
	//init rand
	srand(time(0));
}

