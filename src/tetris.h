#ifndef TETRIS_H
#define TETRIS_H

// CONST
#define GRID_X 10
#define GRID_Y 20

//ENUMS
enum state { RUNNING = 0, GAMEOVER, ERROR };
enum shape { BAR = 0, Z, INVZ, T, SQUARE, L, INVL, NUMBER_SHAPE,
	NOTHING };//number = size of enum
enum position { ZERO = 0, HALFPI, PI, MINUSHALFPI, NUMBER_POSITION };

//extern char pieces[NUMBER_SHAPE][NUMBER_POSITION][4][4];
extern char grid[GRID_X][GRID_Y]; //grid contains shapes
extern char pauseGame;
extern int speed;
extern int pid;


typedef struct Game{
	int score;
	int level;//TODO speed ?
	int state;
} Game;

typedef struct Piece{//colors are intrinsic to piece
	int location[2];//[x, y]
	int position;//rotation wise
	int shape;
	char isFalling;
} Piece;

typedef struct Shape{
	char shape[4][4];
}Shape;

// FUNCS 
void initGame();
void endGame();//needed here ?
void play();
void rotate(Piece*);
void fall(Piece*);
Piece * newPiece();
int * arrayToInt(char **);
char ** intToArray(int *);
#endif
