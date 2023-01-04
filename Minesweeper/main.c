#include <stdio.h>
#include <stdlib.h>
#include "mineSweeper.h"
#include "getch.h"
#include "colorPrint.h"


int
main ()
{
  GameBoard g;
  
  int rowsNum, colsNum, level;
  
  printf("Enter the rows:");
  scanf("%d", &rowsNum);
  printf("Enter the cols: ");
  scanf("%d", &colsNum);
  printf("Enter the level: ");
  scanf("%d", &level);
  getchar();

  if (!initBoard (&g, rowsNum, colsNum, level))
    {
      exit (1);
    }

  Command cmd;
  int saman[2];
  saman[0] = 0;			// rows
  saman[1] = 0;			// cols
  cmd = UP;
  while (cmd != QUIT)
    {

      cmd = getch (); // receive the input of the user and check different options 

      if (cmd == RIGHT) 
	{
	  if (saman[1] < colsNum - 1)
	    {
	      saman[1]++; // go to right
	    }
	}
      else if (cmd == LEFT)
	{
	  if (saman[1] > 0)
	    {
	      saman[1]--; // go to left
	    }
	}
      else if (cmd == DOWN)
	{
	  if (saman[0] < rowsNum - 1)
	    {
	      saman[0]++; // go to down
	    }
	}
      else if (cmd == UP)
	{
	  if (saman[0] > 0)
	    {
	      saman[0]--; // go to up
	    }
	}

      else if (cmd == QUIT)
	    exit (1);


      else if (cmd == FLAG_TILE)
	{
	  flagTile (&g, saman[0], saman[1]); // call the function of flag 
	}

      else if (cmd == CLICK_TILE)
	{
	  clickTile (&g, saman[0], saman[1]); // call the function CLICK_TILE 
	}

      clearScreen (); // clear screen so the board wont be printed a lot of times 
      printBoard (&g, saman);

    }




  for (int i = 0; i < g.rows; i++)	// free the board. First free each Tail, and after all the board
    {
      free (g.board[i]);
    }
  free (g.board);

  return 0;
}
