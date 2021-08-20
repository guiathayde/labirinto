#include <stdio.h>

/********************************************************/
/* Defines                                              */
/********************************************************/
#define DEBUG 1

#define FALSE 0
#define TRUE 1

#define START 'S'
#define WALL '#'
#define OPEN ' '
#define GOAL 'G'
#define PATH '+'
#define BAD_PATH 'x'

#define NROWS 10
#define NCOLS 20

/********************************************************/
/* Struct for storing a position                        */
/********************************************************/
typedef struct pos
{
  int row;
  int col;
} pos;

/********************************************************/
/* Function declarations                                */
/********************************************************/
void printMaze();
pos getStart();
int rec_backtrack(int x, int y);

/********************************************************/
/* Initialize the maze                                  */
/********************************************************/
char maze[NROWS][NCOLS] =
    {
        "#########S##########",
        "#               ## #",
        "# ### ######## ### #",
        "# ###    ####      #",
        "##### ## ###########",
        "#     ## ###     ###",
        "########     ### ###",
        "################ ###",
        "####             ###",
        "####G###############"};

/********************************************************/
/* Main function                                        */
/********************************************************/
int main()
{

  printMaze();

  pos s = getStart();

  if (s.col == -1 && s.row == -1)
    printf("Error: Missing start symbol S!\n");
  else if (s.col == -2 && s.row == -2)
    printf("Error: Multiple start symbols!\n");
  else
  {
    int found = rec_backtrack(s.col, s.row);
    if (found)
      printf("Path found!\n");
    else
      printf("Path not found.\n");
    printMaze();
  }

  return 0;
}

/********************************************************/
/* Function for printing the maze                       */
/********************************************************/
void printMaze()
{
  //for (int i=0; i<NROWS; i++) printf("%.*s\n", NCOLS, maze[i]);
  for (int y = 0; y < NROWS; y++)
  {
    for (int x = 0; x < NCOLS; x++)
    {
      if (maze[y][x] == PATH)
        printf("\033[1;32m");
      else if (maze[y][x] == BAD_PATH)
        printf("\033[1;31m");
      printf("%c", maze[y][x]);
      printf("\033[0m");
    }
    printf("\n");
  }
  printf("\n");
}

/********************************************************/
/* Function that returns the start position (S)         */
/* Returns (x,y) = (-1,-1) if S was not found.          */
/* Returns (x,y) = (-2,-2) if more than one S was found */
/********************************************************/
pos getStart()
{
  int found = 0;
  pos p;
  for (int y = 0; y < NROWS; y++)
  {
    for (int x = 0; x < NCOLS; x++)
    {
      if (maze[y][x] == START)
      {
        if (found > 0)
        {
          p.row = -2;
          p.col = -2;
          return p;
        }
        found = 1;
        p.row = y;
        p.col = x;
      }
    }
  }
  if (!found)
  {
    p.row = -1;
    p.col = -1;
  }
  return p;
}

/********************************************************/
/* Solve maze using recursive backtracking              */
/********************************************************/
int rec_backtrack(int x, int y)
{
  // If (x,y) is outside boundries, return FALSE
  if (x < 0 || x > NCOLS - 1 || y < 0 || y > NROWS - 1)
    return FALSE;

  // If (x,y) is on GOAL, return TRUE
  if (maze[y][x] == GOAL)
    return TRUE;

  // If (x,y) is not OPEN, return FALSE
  if (maze[y][x] != OPEN && maze[y][x] != START)
    return FALSE;

  // Mark (x,y) as part of the solution
  if (maze[y][x] != START)
    maze[y][x] = PATH;

  // If there is a path north of (x,y), return TRUE
  if (rec_backtrack(x, y - 1))
    return TRUE;
  // If there is a path east  of (x,y), return TRUE
  if (rec_backtrack(x + 1, y))
    return TRUE;
  // If there is a path south of (x,y), return TRUE
  if (rec_backtrack(x, y + 1))
    return TRUE;
  // If there is a path west of (x,y), return TRUE
  if (rec_backtrack(x - 1, y))
    return TRUE;

  // Mark (x,y) as a invalid solution
  if (maze[y][x] != START)
    maze[y][x] = BAD_PATH;

  return FALSE;
}