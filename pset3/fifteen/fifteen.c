/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// position of space
int spaceX;
int spaceY;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void swap(int x1, int y1, int x2, int y2);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(5000);
        }

        // sleep thread for animation's sake
        usleep(5000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    // initialize all
    int num = d*d-1;
    
    for(int i = 0 ; i < d ; i ++ )
    {
        for(int j = 0 ; j < d ; j ++ )
        {
            board[i][j] = num;
            num--;
        }
    }
    // if deminsion is even , changing place witch last one and previous one
    if(d%2==0)
    {
        int x,y;
        
        for(int i = 0 ; i < d ; i ++ )
        {
            for(int j = 0 ; j < d ; j ++ )
            {
                if(board[i][j]==2)
                {
                    x=i;
                    y=j;
                    break;
                }
            }
        }
        
        int temp = board[x][y];
        board[x][y] = board[x][y+1];
        board[x][y+1] = temp;
    }
    
    // initialize position of space bar
    spaceX = d-1;
    spaceY = d-1;
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // draw the all
    for(int i = 0 ; i < d ; i ++ )
    {
        for(int j = 0 ; j < d ; j ++ )
        {
            // for space bar
            if(board[i][j]==0)
            {
                printf("%c",(char)board[i][j]);
            }
            // rest of all
            else
            {
                printf("%2i ",board[i][j]);
            }
            
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // find the position of selected tile
    int x,y;
    for(int i = 0 ; i < d ; i ++ )
    {
        for(int j = 0 ; j < d ; j ++ )
        {
            if(board[i][j] == tile)
            {
                x = i;
                y = j;
                break;
            }
        }
    }
    
    // check if selected tile are located above , bottom , left , and right of space bar
    if(spaceX-1 == x && spaceY == y)
    {
        swap(spaceX,spaceY,x,y);
        return true;
    }
    if(spaceX+1 == x && spaceY == y)
    {
        swap(spaceX,spaceY,x,y);
        return true;
    }
    if(spaceX == x && spaceY-1 == y)
    {
        swap(spaceX,spaceY,x,y);
        return true;
    }
    if(spaceX == x && spaceY+1 == y)
    {
        swap(spaceX,spaceY,x,y);
        return true;
    }
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int start = 1;
    int end = d*d-1;
    
    // check if all number is placed on proper position
    
    for(int i = 0 ; i < d ; i ++ )
    {
        for(int j = 0 ; j < d ; j ++ )
        {
            if(board[i][j]==start)
            {
                if(start == end && board[d-1][d-1] == 0)
                {
                    return true;
                }
                start++;
            }
        }
    }
    return false;
}

// To swap two number with its position
void swap(int x1, int y1, int x2, int y2)
{
    int num1 = board[x1][y1];
    int num2 = board[x2][y2];
    int temp;
    
    temp = num1;
    num1 = num2;
    num2 = temp;
    
    board[x1][y1] = num1;
    board[x2][y2] = num2;
    
    spaceX = x2;
    spaceY = y2;

    return;
}

