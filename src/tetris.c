#include "tetris.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <ncurses.h>


//PROTOTYPES 
//
void fall(GameState * state);
void dropOrSpawn(GameState * state);
void checkInput(GameState * state);
void genNewPiece(Piece * piece, int n);
char lineComplete(char grid[GRID_X][GRID_Y]);
void updatePieceOnGrid(GameState * state);

void rotate(Piece * p){
	p->rotation ++;
}

char lineComplete(char grid[GRID_X][GRID_Y]){
	char found = 0;
	for(char k ; k < GRID_Y ; k++){
		for(char l ; l < GRID_X ; l++){
			if(grid[l][k]){
				found = 1;
			}
		}
	}
	return found;
}

void dropOrSpawn(GameState * state){
	if(!state->fallingPiece){
		if(!!lineComplete(*state->grid)){//!! <=> char to bool
			//TODO remove line comptlete
		}
		else{
			state->fallingPiece = newPiece();
		}
	}
	else{
		fall(state);
	}
	updatePieceOnGrid(state);
}

void fall(GameState * state){
	//update piece location
	if(!state->fallingPiece->isFalling){
		exit(-1);//can ignore this warning, this part is single thread
	}
	//check if can fall
	if(*state->grid[state->fallingPiece->location[0]]
			[state->fallingPiece->location[1]] == 1){ 
		// TODO for all square not just center
		//if square full then lay piece
		state->fallingPiece->isFalling = 0;
	}

	state->fallingPiece->location[1] --;
}

void updatePieceOnGrid(GameState * state){
	char x = state->fallingPiece->location[0];
	char y = state->fallingPiece->location[1];
	for(char k = 0 ; k < GRID_X ; k++){
		for(char l = 0 ; l < GRID_Y ; l++){
			if(state->grid[k + x][l + y] == 1)
				endGame();
			*state->grid[k + x][l + y]	 = intToArray(state->fallingPiece->shape)[l][k];
		}
	}
}

Piece * newPiece(){
	int n = rand() % NUMBER_SHAPE;
	//TODO

	Piece * p = malloc(sizeof(Piece)); //TODO calloc ?
	p->location[0] = 5;
	p->location[1] = 0;
	p->rotation = 0;
	p->isFalling = 1;

	genNewPiece(p, n);

	return p;
}

void genNewPiece(Piece * p, int n){
	Shape * shape;
	char tmp[4][4];

	switch(n){
		case BAR:
			{
				char tmp[4][4] = {
						{0, 0, 1, 0,},
						{0, 0, 1, 0,},
						{0, 0, 1, 0,},
						{0, 0, 1, 0,},
					};

				shape->shape[0] = arrayToInt(tmp)[0];
				shape->shape[1] = arrayToInt(tmp)[1];
				break;
		 }

		case Z:
			{
				char tmp[4][4] = {
						{0, 0, 0, 0,},
						{0, 1, 1, 0,},
						{0, 0, 1, 1,},
						{0, 0, 0, 0,},
					};

				shape->shape[0] = arrayToInt(tmp)[0];
				shape->shape[1] = arrayToInt(tmp)[1];
				break;
		 }

		case INVZ:
			{
				char tmp[4][4] = {
						{0, 0, 0, 0,},
						{0, 0, 1, 1,},
						{0, 1, 1, 0,},
						{0, 0, 0, 0,},
					};

				shape->shape[0] = arrayToInt(tmp)[0];
				shape->shape[1] = arrayToInt(tmp)[1];
				break;
		 }

		case T:
			{
				char tmp[4][4] = {
						{0, 0, 0, 0,},
						{0, 0, 1, 0,},
						{0, 1, 1, 0,},
						{0, 0, 1, 0,},
					};

				shape->shape[0] = arrayToInt(tmp)[0];
				shape->shape[1] = arrayToInt(tmp)[1];
				break;
		 }

		case SQUARE:
			{
				char tmp[4][4] = {
						{0, 0, 0, 0,},
						{0, 1, 1, 0,},
						{0, 1, 1, 0,},
						{0, 0, 0, 0,},
					};

				shape->shape[0] = arrayToInt(tmp)[0];
				shape->shape[1] = arrayToInt(tmp)[1];
				break;
		 }

		case L:
			{
				char tmp[4][4] = {
						{0, 0, 0, 0,},
						{0, 1, 1, 0,},
						{0, 0, 1, 0,},
						{0, 0, 1, 0,},
					};

				shape->shape[0] = arrayToInt(tmp)[0];
				shape->shape[1] = arrayToInt(tmp)[1];
				break;
		 }
		case INVL:

			{
				char tmp[4][4] = {
						{0, 0, 0, 0,},
						{0, 1, 1, 0,},
						{0, 1, 0, 0,},
						{0, 1, 0, 0,},
					};

				shape->shape[0] = arrayToInt(tmp)[0];
				shape->shape[1] = arrayToInt(tmp)[1];
				break;
		 }

	}
	p->shape = shape;
}


GameState * initGame(){

	initscr();
	clear();
	refresh();
	//
	//init grid 
	GameState * state = malloc(sizeof(GameState));

	state->grid = malloc(GRID_X * GRID_Y);
	for(char k = 0 ; k < GRID_X ; k++){
		for(char l = 0 ; l < GRID_Y ; l ++){
			*state->grid[k][l] = 0;
		}
	}
	//init rand
	srand(time(0));
	state->pauseGame = 0;
	state->speed = 500000; //.5s
//	if((state->pid = fork()) == 0){
//		checkInput(state);
//	}
//
	state->pid = 0;
	return state;
}

void renderFrame(GameState * state){
	//TODO
	clear();
	for(char y = 1 ; y < GRID_Y ; y++){
		for(char x = 0 ; x < GRID_X ; x++){
			if(*state->grid[x][y]){
				printw("X");//TODO colors of block
			}
			else{
				printw(" ");
			}
		}
		printw("\n");
	}
	refresh();
}

void play(){
	GameState * state = initGame();

	while(!state->pauseGame){
		renderFrame(state);
		dropOrSpawn(state);
		usleep(state->speed);
	}
	endGame();
}

char ** intToArray(int * n){
	// goal : take 2 ints and give back 4x4 representation, with bool
	// in each cell
	//TODO
}

int * arrayToInt(char c[4][4]){//needed ?
	//same as above but other way arround 
	char flat[16] = {0};//flatten input
	for(char k = 0 ; k < 4 ; k++){
		for(char l = 0 ; l < 4 ; l++){
			flat[k * 4 + l] = c[k][l];
			if(c[k][l] > 1)
				endGame();
		}
	}

	int * res = malloc(2 * sizeof(int));
	int res1 = 0;
	int res2 = 0;

	for(char k = 0 ; k < 8 ; k++){//1st int 
		if(flat[k])
			res1 = res1 | (1 << k);
	}

	for(char k = 0 ; k < 8 ; k++){//2nd int 
		if(flat[k + 7])
			res2 = res2 | (1 << k);
	}

	res[0] = res1;
	res[1] = res2;

	return res;
}

void checkInput(GameState * state){
	for(;;){
		state->keyPressed = getch();
	}
}

void endGame(GameState * state){
	if(pid == 0){
		//wait for press q then exit both
	}
	wait(NULL);
	exit(0);
}
