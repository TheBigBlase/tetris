#include "tetris.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>


//PROTOTYPES 
void checkInput();


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
	//TODO
}


void initBar(){
	void * pieces[NUMBER_SHAPE][NUMBER_POSITION];//dont conserve type
	int * tmp = malloc(2); //2 bytes = 16 bits

	pieces[BAR][ZERO] = (void*)tmp; //no type needed
}

void initGame(){
	//init grid 
	char grid[GRID_Y][GRID_X];
	for(int k = 0 ; k < GRID_X ; k++){
		for(int l = 0 ; l < GRID_Y ; l ++){
			grid[k][l] = 0;
		}
	}
	//init rand
	srand(time(0));
	pauseGame = 0;
	speed = 500000; //.5s
	if((pid = fork()) == 0){
		checkInput();
	}

	pid = 0;
}

void renderFrame(){
	//TODO
	for(char y = 0 ; y < GRID_Y ; y++){
		for(char x = 0 ; x < GRID_X ; x++){
			if(grid[x][y]){
				printf("X");//TODO colors of block
			}
			else{
				printf(" ");
			}
		}
		printf("\n");
	}
}

void play(){
	initGame();

	while(!pauseGame){
		renderFrame();
		usleep(speed);
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

void checkInput(){
	for(;;){
		//TODO
	}
}

void endGame(){
	if(pid == 0){
		//wait for press q then exit both
	}
	wait(NULL);
	exit(0);
}
