#include "tetris.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <curses.h>
#include <string.h>


//PROTOTYPES 
//
void moveLeft(Piece *);
void moveRight(Piece *);
char checkHitLeft(Piece *, grid_t);
char checkHitRight(Piece *, grid_t);
void fall(Piece *);
void fallOrSpawn(GameState * state);
void checkInput(GameState * state);
void genNewPiece(Piece * piece, int n);
char lineComplete(char grid[GRID_X][GRID_Y]);
void updatePieceOnGrid(Piece *, grid_t);
void renderFrame(GameState *);
char checkHit(Piece * p, grid_t);
char checkPieceRange(Piece *, char, char);

void rotate(Piece * p)
{
	//TODO reshape here
	p->rotation ++;
}

char lineComplete(char grid[GRID_X][GRID_Y])
{
	char found = 0;
	char k = 0;
	char l = 0;
	while(k < GRID_Y && found == 0) {
		while(l < GRID_X && found == 0) {
			if(grid[l][k]) {
				found = 1;
			}
			l++;
		}
		k++;
	}
	return found;
}

void fallOrSpawn(GameState * state)
{
	if(!state->fallingPiece){
		if(!!lineComplete(*state->grid)) {//!! <=> char to bool
			//TODO remove line comptlete
		}
		else {
			state->fallingPiece = newPiece();
		}
	}
	else {
		if(checkHit(state->fallingPiece, state->grid)) {
			updatePieceOnGrid(state->fallingPiece, state->grid);

			state->fallingPiece = newPiece();
		}
		else {
			fall(state->fallingPiece);
		}
	}
}

void fall(Piece * p)
{
	//update piece location
	(*p->location)[1] ++;
}

//checks if can move downward
//TODO recheck issue here
char checkHit(Piece * p, grid_t grid)
{
	char px = (*p->location)[0];
	char py = (*p->location)[1];

	char y = 3;//check backward
	
	char hit = 0;
	while(y > 0 && !hit) {
		char x = 3;
		while(x > 0 && !hit) {
			if(((*p->shape)[x][y] && py + y + 1 >= GRID_Y)
						|| ((*p->shape)[x][y] && (*grid)[px + x][py + y + 1])) {
				hit = 1;
			}
			x--;
		}
		y--;
	}

	return hit;
}

char checkHitLeft(Piece * p, grid_t grid)
{
	char px = (*p->location)[0];
	char py = (*p->location)[1];

	char x = 3;//check backward
	
	char hit = 0;
	while(x > 0 && !hit) {
		char y = 3;
		while(y > 0 && !hit) {
			if(((*p->shape)[x][y] && px + x - 1 < 0)
						|| ((*p->shape)[x][y] && (*grid)[px + x - 1][py + y])) {
				hit = 1;
			}
			x--;
		}
		y--;
	}

	return hit;
}

char checkHitRight(Piece * p, grid_t grid)
{
	char px = (*p->location)[0];
	char py = (*p->location)[1];

	char y = 3;//check backward
	
	char hit = 0;
	while(y > 0 && !hit) {
		char x = 3;
		while(x > 0 && !hit) {
			if(((*p->shape)[x][y] && py + y + 1 >= GRID_Y)
						|| ((*p->shape)[x][y] && (*grid)[px + x][py + y + 1])) {
				hit = 1;
			}
			x--;
		}
		y--;
	}

	return hit;
}

void moveRight(Piece * p)
{
	(*p->location)[0]++;
}

void moveLeft(Piece * p)
{
	(*p->location)[0]--;
}

void updatePieceOnGrid(Piece * p, grid_t grid)
{
	//puts a piece on the grid (locks it) and frees it
	char px = *p->location[0];
	char py = *p->location[1];

	for(char x = 0 ; x < 4 ; x++) {
		for(char y = 0 ; y < 4 ; y++) {//overflow y
			(*grid)[px + x][py + y] = (*p->shape)[x][y]
					|| (*grid)[px + x][py + y];
		}
	}

	free(p->location);
	p->location = NULL;
	free(p->shape);
	p->shape = NULL;
	free(p);
	p = NULL;
}

Piece * newPiece(){
	int n = rand() % NUMBER_SHAPE;
	//TODO

	Piece * p = malloc(sizeof(Piece)); //TODO calloc ?
	p->shape = malloc(sizeof(char[4][4]));

	p->location = malloc(sizeof(char[2]));

	(*p->location)[0] = (int) (GRID_X  - 4) / 2;
	(*p->location)[1] = 0;
	p->rotation = 0;
	p->isFalling = 1;

	genNewPiece(p, n);

	return p;
}

void genNewPiece(Piece * p, int n)
{
	switch(n) {
		//careful, angle + pi/2
		case BAR:
			{
				char tmp[4][4] = {
					{0, 0, 1, 0},
					{0, 0, 1, 0},
					{0, 0, 1, 0},
					{0, 0, 1, 0},
				};
				memcpy(p->shape, &tmp, 4*4);
				break;
			}

		case Z:
			{
				char tmp[4][4] = {
					{0, 0, 0, 0},
					{0, 1, 1, 0},
					{0, 0, 1, 1},
					{0, 0, 0, 0},
				};
				memcpy(p->shape, &tmp, 4*4);
				break;
			}

		case INVZ:
			{
				char tmp[4][4] = {
					{0, 0, 0, 0},
					{0, 0, 1, 1},
					{0, 1, 1, 0},
					{0, 0, 0, 0},
				};

				memcpy(p->shape, &tmp, 4*4);
				break;
			}

		case T:
			{
				char tmp[4][4] = {
					{0, 0, 0, 0},
					{0, 0, 1, 0},
					{0, 1, 1, 0},
					{0, 0, 1, 0},
				};

				memcpy(p->shape, &tmp, 4*4);
				break;
			}

		case SQUARE:
			{
				char tmp[4][4] = {
					{0, 0, 0, 0},
					{0, 1, 1, 0},
					{0, 1, 1, 0},
					{0, 0, 0, 0},
				};

				memcpy(p->shape, &tmp, 4*4);
				break;
			}

		case L:
			{
				char tmp[4][4] = {
					{0, 0, 0, 0},
					{0, 1, 1, 0},
					{0, 0, 1, 0},
					{0, 0, 1, 0},
				};

				memcpy(p->shape, &tmp, 4*4);
				break;
			}

		case INVL:
			{
				char tmp[4][4] = {
					{0, 0, 0, 0},
					{0, 1, 1, 0},
					{0, 1, 0, 0},
					{0, 1, 0, 0},
				};

				memcpy(p->shape, &tmp, 4*4);
				break;
			}
	}
}


GameState * initGame()
{
	initscr();

	//init grid 
	GameState * state = malloc(sizeof(GameState));

	state->grid = malloc(sizeof(char[GRID_X][GRID_Y]));

	for(char x = 0 ; x < GRID_X ; x++){
		for(char y = 0 ; y < GRID_Y ; y ++){
			(*state->grid)[x][y] = 0;
		}
	}

	//init rand
	srand(time(0));
	state->pauseGame = 0;
	state->speed = SPEED; //.1s

	if((state->pid = fork()) == 0){
		checkInput(state);
	}

	//state->pid = 0; //????????
	//key
	state->keyboardInput = malloc(sizeof(int));
	return state;
}

//check if a piece fits into grid
char checkPieceRange(Piece * p, char x, char y)
{
	char res = 0;
	if((*p->location[0]) > x - 4 && (*p->location)[0] <= x) {
		if((*p->location[1]) > y - 4 && (*p->location)[1] <= y) {
			res = 1;
		}
	}
	return res;
}

void renderFrame(GameState * state)
{
	char size = 0;
	clear();
	for(char y = 0 ; y < GRID_Y ; y++) {
		addch('|');//start of line
		for(char x = 0 ; x < GRID_X  ; x++) {
			//print grid
			if((*state->grid)[x][y]) {
				addch('X');//TODO colors of block
			}
			else {
				//print piece
				if(state->fallingPiece && state->fallingPiece->isFalling) {
					Piece * p = state->fallingPiece;
					if(checkPieceRange(p, x, y)) {
						//peices are drawn upside down, so y then x
						if((*p->shape)[x - (*p->location)[0]][y - (*p->location)[1]]) {
							addch('O');
						}
						else {
							addch('$');
						}
					}
					else {
						addch(' ');
					}
				}
				else {
					addch(' ');
				}
			}
		}
		addstr("|\n");
	}
	addstr("+---------+");
	refresh();
}

void play()
{
	GameState * state = initGame();

	while(!state->pauseGame) {
		//applyInputs();//Multi thread will make game slugish, cant move between refresh
		fallOrSpawn(state);
		renderFrame(state);
		usleep(state->speed);
	}

	endGame(0);
}

char (*intToArray(int * n))[4][4]
{//pointer to array 4x4
	// goal : take 2 ints and give back 4x4 representation, with bool
	// in each cell
	//TODO
	char (*res)[4][4] = malloc(4 * 4);
	int tmp = 0;
	
	for(char k = 0 ; k < 2 ; k++){
		for(char l = 0 ; l < 4 ; l++){
		 *res[k][l] = n[0] & (1 << (k * 4 + l));
		}
	}
	
	for(char k = 0 ; k < 4 ; k++){
		for(char l = 0 ; l < 4 ; l++){
		 *res[k][l] = n[1] & (1 << (k * 4 + l));
		}
	}

	return res;
}

int * arrayToInt(char c[4][4])
{
	//same as above but other way arround 
	char flat[16] = {0};//flatten input
	for(char k = 0 ; k < 4 ; k++){
		for(char l = 0 ; l < 4 ; l++){
			flat[k * 4 + l] = c[k][l];
			if(c[k][l] > 1)
				endGame(-1);
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

void checkInput(GameState * state)
{
	for(;;){
		int keypressed = getchar_unlocked();
		switch(keypressed) {
		case KEY_LEFT :
				*state->keyboardInput = LEFT;
				if(!checkHitLeft(state->fallingPiece, state->grid))
					moveLeft(state->fallingPiece);
				break;
		case KEY_RIGHT:
				*state->keyboardInput = RIGHT;
				if(!checkHitRight(state->fallingPiece, state->grid))
					moveRight(state->fallingPiece);
				break;
		case KEY_DOWN:
				*state->keyboardInput = DOWN;
				if(!checkHit(state->fallingPiece, state->grid))
					fall(state->fallingPiece);
				break;
		}

		renderFrame(state);

		keypressed = 0;
		usleep(KEYBOARD_SPEED);
	}
}

void endGame(int code)
{
	if(pid == 0){
		//wait for press q then exit both
	}
	wait(NULL);
	exit(code);
}
