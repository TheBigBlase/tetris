#ifndef TETRIS_H
#define TETRIS_H

// CONST
#define GRID_X 10
#define GRID_Y 20
#define SPEED 500000
#define KEYBOARD_SPEED 50000

//dirty, cuz type doesnt look like a pointer now
typedef char (*grid_t)[GRID_X][GRID_Y];

//ENUMS
enum state { RUNNING = 0, GAMEOVER, ERROR };
enum shape { BAR = 0, Z, INVZ, T, SQUARE, L, INVL, NUMBER_SHAPE,
	NOTHING };//number = size of enum
enum position { ZERO = 0, HALFPI, PI, MINUSHALFPI, NUMBER_POSITION };

enum directions { LEFT = 0, RIGHT, DOWN};

extern int pid;

typedef struct {//colors are intrinsic to piece
	char (*location)[2];//[x, y]
	int rotation;//rotation wise
	char (*shape)[4][4];
	char isFalling;
} Piece;


typedef struct {
	int * keyboardInput;
	char pauseGame;
	char (*grid)[GRID_X][GRID_Y];//pointer to array of arrays
	int pid;
	int speed;
	Piece * fallingPiece;
} GameState;

typedef struct Game{
	int score;
	int level;//TODO speed ?
	int state;
} Game;

// FUNCS 
GameState * initGame();
void endGame(int);//needed here ?
void play();
void rotate(Piece*);
Piece * newPiece();
int * arrayToInt(char [4][4]);
char (*intToArray(int *))[4][4];

#endif
