#include <stdio.h>
#include "tetris.h"
#include <ncurses.h>

int main(int argc, const char* argv[])
{

  (void)argc; //TODO take care of args later
  (void)argv; //woop goes to trash

	initscr();
	play();
  return 0;
}
