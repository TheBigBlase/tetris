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
char lineComplete(char ** array);

void rotate(Piece * p){
	p->rotation ++;
}

char lineComplete(char ** grid){
	char found = 0;
	for(char k ; k < GRID_Y ; k++){
		for(char l ; l < GRID_X ; l++){
			if(grid[l][k]){
				found = 1;
			}
		}
	}
}

void dropOrSpawn(GameState * state){
	if(!state->fallingPiece){
		if(!!lineComplete(state->grid)){//!! <=> char to bool
			//TODO remove line comptlete
		}
		else{
			state->fallingPiece = newPiece();
		}
	}
	else{
		fall(state);
	}
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

Piece * newPiece(){
	int n = rand() % NUMBER_SHAPE;
	//TODO

	Piece * p = malloc(sizeof(Piece)); //TODO calloc ?
	p->location[0] = 0;
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
				char **tmp = {
						{0, 0, 1, 0},
						{0, 0, 1, 0},
						{0, 0, 1, 0},
						{0, 0, 1, 0}
						};

				shape->shape[0] = arrayToInt(tmp)[0];
				shape->shape[1] = arrayToInt(tmp)[1];
				break;
		 }

		case Z:
			{
				char **tmp = {
						{0, 0, 0, 0},
						{0, 1, 1, 0},
						{0, 0, 1, 1},
						{0, 0, 0, 0}
						};

				shape->shape[0] = arrayToInt(tmp)[0];
				shape->shape[1] = arrayToInt(tmp)[1];
				break;
		 }

		case INVZ:
			{
				char **tmp = {
						{0, 0, 0, 0},
						{0, 0, 1, 1},
						{0, 1, 1, 0},
						{0, 0, 0, 0}
						};

				shape->shape[0] = arrayToInt(tmp)[0];
				shape->shape[1] = arrayToInt(tmp)[1];
				break;
		 }

		case T:
			{
				char **tmp = {
						{0, 0, 0, 0},
						{0, 0, 1, 0},
						{0, 1, 1, 0},
						{0, 0, 1, 0}
						};

				shape->shape[0] = arrayToInt(tmp)[0];
				shape->shape[1] = arrayToInt(tmp)[1];
				break;
		 }

		case SQUARE:
			{
				char **tmp = {
						{0, 0, 0, 0},
						{0, 1, 1, 0},
						{0, 1, 1, 0},
						{0, 0, 0, 0}
						};

				shape->shape[0] = arrayToInt(tmp)[0];
				shape->shape[1] = arrayToInt(tmp)[1];
				break;
		 }

		case L:
			{
				char **tmp = {
						{0, 0, 0, 0},
						{0, 1, 1, 0},
						{0, 0, 1, 0},
						{0, 0, 1, 0}
						};

				shape->shape[0] = arrayToInt(tmp)[0];
				shape->shape[1] = arrayToInt(tmp)[1];
				break;
		 }
		case INVL:

			{
				char **tmp = {
						{0, 0, 0, 0},
						{0, 1, 1, 0},
						{0, 1, 0, 0},
						{0, 1, 0, 0}
						};

				shape->shape[0] = arrayToInt(tmp)[0];
				shape->shape[1] = arrayToInt(tmp)[1];
				break;
		 }

	}
	p->shape = shape;
}


GameState * initGame(){
	//init grid 
	GameState * state = malloc(sizeof(GameState));

	for(int k = 0 ; k < GRID_X ; k++){
		for(int l = 0 ; l < GRID_Y ; l ++){
			state->grid[k][l] = 0;
		}
	}
	//init rand
	srand(time(0));
	state->pauseGame = 0;
	state->speed = 500000; //.5s
	if((state->pid = fork()) == 0){
		checkInput(state);
	}

	state->pid = 0;
	return state;
}

void renderFrame(GameState * state){
	//TODO
	clear();
	for(char y = 1 ; y < GRID_Y ; y++){
		for(char x = 0 ; x < GRID_X ; x++){
			if(state->grid[x][y]){
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
}

char ** intToArray(int * n){
	// goal : take 2 ints and give back 4x4 representation, with bool
	// in each cell
	//TODO
}

int * arrayToInt(char ** c){//needed ?
	//same as above but other way arround 
	char tmp[16];
	for(char k = 0 ; k < 4 ; k++){
		for(char l = 0 ; l < 4 ; l++){
			tmp[k + l] = c[k][l];
		}
	}

	int * res = malloc(2 * sizeof(int));

	for(char k = 0 ; k < 8 ; k++){//1st int 
		res[0] = 1 << tmp[k]; //???? TODO
	}

	for(char k = 0 ; k < 8 ; k++){//2nd int 
		res[1] = 1 << tmp[k]; //???? TODO
	}
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
