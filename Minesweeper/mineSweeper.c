#include "mineSweeper.h"
#include "colorPrint.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>


bool
initBoard (GameBoard * g, int rows, int cols, int level)	//returns true upon success
{
  srand (time (NULL));
  
    g->cols = cols; // check that the entered values are correct 
     g->rows = rows;
  
  if(g->rows > MAX_BOARD_DIM)
    g->rows = MAX_BOARD_DIM;
  
    if(g->rows <= 0)
    g->rows = 4;
  
    
    if(g->cols > MAX_BOARD_DIM)
        g->cols = MAX_BOARD_DIM;
        
    if(g->cols <= 0)
        g->cols = 4;
        
    if(level > 3 || level <= 0)
        level = 1;
     


  g->board = (Tile **) malloc (rows * sizeof (Tile *));	// build the luach (rows) and check if malloc worked properly
  if (g->board == NULL)
    {
      return FALSE;
    }
  for (int i = 0; i < rows; i++)	// build the luach (cols) and check if malloc worked properly
    {
      g->board[i] = (Tile *) malloc (cols * sizeof (Tile));
      if (g->board[i] == NULL)
	{
	  return FALSE;
	}
    }


  for (int i = 0; i < rows; ++i)
    {
      for (int j = 0; j < cols; ++j)	// start all the Tiles with default values
	{
	  g->board[i][j].numOfMines = 0;
	  g->board[i][j].isMine = FALSE;
	  g->board[i][j].isFlagged = FALSE;
	  g->board[i][j].isVisible = FALSE;

	}
    }


  g->totalMines = 0; // initialize everything 
  g->isMineClicked = FALSE;
  g->hiddenTiles = rows * cols;

  populateMines (g, level); // call the function to put bombs 
  markNumbers (g); // call the function to update numOfMines

  int saman[2]; // will be the Saman 
  saman[0] = 0;
  saman[1] = 0;
  printBoard (g, saman); // call the function with the Saman so the board will be printed 

  return TRUE;
}


void
populateMines (GameBoard * g, int level)
{

  g->totalMines = 0;


   if (level == 1)		// decide the amount of bombs depending of the game level 
    {
      g->totalMines = ((g->rows * g->cols) * EASY_LEVEL_PERCENT) / 100;
    }
  else if (level == 2)
    {
      g->totalMines = ((g->rows * g->cols) * MEDIUM_LEVEL_PERCENT) / 100;
    }
  else if (level == 3)
    {
      g->totalMines = ((g->rows * g->cols) * HARD_LEVEL_PERCENT) / 100;
    }
  else
    {
      return;
    }
  int temp = g->totalMines;
  while (g->totalMines > 0)	// randomly put bombs in luach 
    {
      int numX = rand () % g->rows;	// random numbers X and Y (coordenates)
      int numY = rand () % g->cols;
      if (!(g->board[numX][numY].isMine))
	{
	  g->board[numX][numY].isMine = TRUE;
	  g->totalMines--; // to know how many pumps are still to be placed discount every time I put one
	  g->board[numX][numY].numOfMines = -1;	// if there is a bomb, the Tail will have '-1'
	}
    }
  g->totalMines = temp; // will use it after to know if the game is over 


}


void
markNumbers (GameBoard * g)
{
  // count the number of bombs that each Tail has near (schenim): 
  for (int i = 0; i < g->rows; i++)
    {
      for (int j = 0; j < g->cols; j++) // will go through all the board 
	{

	  if (g->board[i][j].isMine == TRUE)
	    {

	      if (i + 1 < g->rows && g->board[i + 1][j].isMine == FALSE)	//right
		g->board[i + 1][j].numOfMines++;

	      if (i + 1 < g->rows && j + 1 < g->cols && g->board[i + 1][j + 1].isMine == FALSE)	//right down
		g->board[i + 1][j + 1].numOfMines++;

	      if (j + 1 < g->cols && g->board[i][j + 1].isMine == FALSE)	// down
		g->board[i][j + 1].numOfMines++;

	      if (i - 1 >= 0 && j + 1 < g->cols && g->board[i - 1][j + 1].isMine == FALSE)	// left down
		g->board[i - 1][j + 1].numOfMines++;

	      if (i - 1 >= 0 && g->board[i - 1][j].isMine == FALSE)	// left
		g->board[i - 1][j].numOfMines++;

	      if (i - 1 >= 0 && j - 1 >= 0 && g->board[i - 1][j - 1].isMine == FALSE)	// left up 
		g->board[i - 1][j - 1].numOfMines++;

	      if (j - 1 >= 0 && g->board[i][j - 1].isMine == FALSE)	// up
		g->board[i][j - 1].numOfMines++;

	      if (i + 1 < g->rows && j - 1 >= 0 && g->board[i + 1][j - 1].isMine == FALSE)	// right up 
		g->board[i + 1][j - 1].numOfMines++;
	    }
	}
    }
}


void
clickTile (GameBoard * g, int row, int col)
{

  if (g->board[row][col].isVisible == TRUE)
    {				// if the Tail is visible, dont do anything
      return;
    }

  else if (g->board[row][col].isVisible == FALSE
	   && g->board[row][col].isFlagged == TRUE)
    {				// not visible, and with flag, dont do anything 
      return;
    }

  else if (g->board[row][col].isVisible == FALSE && g->board[row][col].isFlagged == FALSE)
    {				// not visible, and not flag:
      if (g->board[row][col].isMine == TRUE)
	{			// if the Tail is a bomb
	  g->isMineClicked = TRUE;
	  //boom(&g,luach );
	}
      else if (g->board[row][col].numOfMines > 0)
	{			// if Tail has a number bigger than 0, make it visible
	  g->board[row][col].isVisible = TRUE;
	  g->hiddenTiles--; // deducts one from the total number of hidden tails 
	  if (g->hiddenTiles == g->totalMines) // check if there are no more tails left without hidden bombs, if yes, you win and the game is over 
	    {
	      printf ("YOU WIN :)");
	      exit (1);
	    }
	}
      else if (g->board[row][col].numOfMines == 0)
	{			// if Tail is equal to 0, recursion to check neighbords

	  g->board[row][col].isVisible = TRUE; // make visible the actual Tail 
	  g->hiddenTiles--; 
	  if (g->hiddenTiles == g->totalMines) // check if win and game over 
	    {
	      printf ("YOU WIN :)");
	      exit (1);
	    }

	  //check right
	  if ((col + 1) < g->cols)
	    {
	      if (g->board[row][col + 1].numOfMines != -1
		  && g->board[row][col + 1].isVisible == FALSE)
		clickTile (g, row, col + 1);
	    }

	  //check up
	  if ((row + 1) < g->rows)
	    {
	      if (g->board[row + 1][col].numOfMines != -1
		  && g->board[row + 1][col].isVisible == FALSE)
		clickTile (g, row + 1, col);
	    }

	  //check down
	  if ((row - 1) >= 0)
	    {
	      if (g->board[row - 1][col].numOfMines != -1
		  && g->board[row - 1][col].isVisible == FALSE)
		clickTile (g, row - 1, col);
	    }

	  //check left
	  if ((col - 1) >= 0)
	    {
	      if (g->board[row][col - 1].numOfMines != -1
		  && g->board[row][col - 1].isVisible == FALSE)
		clickTile (g, row, col - 1);
	    }


	}

    }
}


void
flagTile (GameBoard * g, int row, int col)
{
  if (g->board[row][col].isVisible == TRUE)	// if Tail visible, dont do anything
    return;
  if (g->board[row][col].isFlagged == TRUE)
    {				// if is already flagged, will be unflagged
      g->board[row][col].isFlagged = FALSE;
      return;
    }
  if (g->board[row][col].isFlagged == FALSE)	// if is not flagged, will be flagged
    g->board[row][col].isFlagged = TRUE;

}


void
printBoard (GameBoard * g, int cursorCoords[2])
{

  int rowsNow = cursorCoords[0];
  int colsNow = cursorCoords[1];


  char luach[3]; // build each tail of the board 
  luach[0] = ' ';
  luach[1] = ' ';
  luach[2] = '\0';


  for (int i = 0; i < g->rows; i++)
    {				// print the board
      for (int j = 0; j < g->cols; j++)
	{
	    
	      if (g->isMineClicked == TRUE) // if bomb is clicked
	    {			
	      printf ("\n\nYou lost!");
	      exit (1);
	    }
	    
	    
	    else if (i == rowsNow && j == colsNow) // will print the "saman", depending on the state of the actual Tail 
	    {			
	    
	        if(g->board[i][j].isVisible == TRUE){ // if visible show in a different background the number of numOfMines
	            luach[1] = g->board[i][j].numOfMines + '0';
	            colorPrint (FG_White, BG_Black, ATT_Def, (char *) &luach);
	            
	        }
	        else if(g->board[i][j].isFlagged == TRUE && g->board[i][j].isVisible == FALSE){ // if not visible and flagged 
	            luach[1] = 'F';
	            colorPrint (FG_White, BG_Black, ATT_Def, (char *) &luach);
	        }
	        else{ // not visible and not flagged 
	            luach[1] = '*';
	            colorPrint (FG_White, BG_Black, ATT_Def, (char *) &luach);
	        }
	    }
	    

	  else if (g->board[i][j].isVisible == FALSE
	      && g->board[i][j].isFlagged == FALSE)
	    {			// not visible and not flag
	      luach[1] = ' ';
	      colorPrint (FG_Black, BG_Magenta, ATT_Def, (char *) &luach);
	    }

	  else if (g->board[i][j].isVisible == FALSE
	      && g->board[i][j].isFlagged == TRUE)
	    {			// not visible and flag
	      luach[1] = 'F';
	      colorPrint (FG_Black, BG_Yellow, ATT_Def, (char *) &luach);
	    }

	  else if (g->board[i][j].isVisible == TRUE
	      && g->board[i][j].numOfMines == 0)
	    {			// visible and 0
	      luach[1] = '0';
	      colorPrint (FG_Black, BG_Green, ATT_Def, (char *) &luach);
	    }
	  else if (g->board[i][j].isVisible == TRUE
	      && g->board[i][j].numOfMines > 0)
	    {			// visible and number bigger than 0
	      luach[1] = g->board[i][j].numOfMines + '0';
	      colorPrint (FG_Black, BG_Green, ATT_Def, (char *) &luach);
	    }
	}
      printf ("\n");
    }



}

