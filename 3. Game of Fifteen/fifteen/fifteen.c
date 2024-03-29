/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements the Game of Fifteen (generalized to d x d).
 *
 * Usage: ./fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [MIN,MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// board's minimal dimension
#define MIN 3

// board's maximal dimension
#define MAX 9

// board, whereby board[i][j] represents row i and column j
int board[MAX][MAX];

// board's dimension
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void save(void);

int main(int argc, string argv[])
{
    // greet player
    greet();

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < MIN || d > MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            MIN, MIN, MAX, MAX);
        return 2;
    }

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // saves the current state of the board (for testing)
        save();

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();
        
        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(50000);
        }

        // sleep for animation's sake
        usleep(100);
    }

    // that's all folks
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
    printf("GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1,
 * (i.e., fills board with values but does not actually print them),
 * whereby board[i][j] represents row i and column j.
 */
void init(void)
{
    // initializes gaming board
    int tile_value = (d * d) - 1;
    int tile_count = tile_value;
   
    for (int row = 0; row < d; row++)
    {
        for (int col = 0; col < d; col++)
        {
            board[row][col] = tile_value--;
        }
        
        printf("\n");
    }
    
    if (tile_count % 2 != 0)
    {
        board[d - 1][d - 2] = 2;
        board[d - 1][d - 3] = 1;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // draws the gaming board to the console
    for (int row = 0; row < d; row++)
    {
        for (int col = 0; col < d; col++)
        {
            if (board[row][col] >= 10)
            {
                printf("%d ", board[row][col]);
            }
            else
            {
                if (board[row][col] != 0)
                {
                    printf("%d  ", board[row][col]);
                }
                else
                {
                    printf("_  ");
                }
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
    // searches for the tile to move
    int tile_row;
    int tile_col;
    
    // checks if tile is valid
    int max_tale = (d * d) - 1;
    if (tile > max_tale || tile < 1)
    {
        return false;
    }
    
    for (int row = 0; row < d; row++)
    {
        for (int col = 0; col < d; col++)
        {
            if (tile == board[row][col])
            {
                tile_row = row;
                tile_col = col;
            }
        }
    }
    
    // checks if move is valid
    // checks upward
    if (tile_row - 1 >= 0)
    {
        if (board[tile_row - 1][tile_col] == 0)
        {
            board[tile_row - 1][tile_col] = board[tile_row][tile_col];
            board[tile_row][tile_col] = 0;
            
            return true;
        }
    }
    
    // checks rightwards
    if (tile_col + 1 < d && board[tile_row][tile_col + 1] == 0)
    {
        board[tile_row][tile_col + 1] = board[tile_row][tile_col];
        board[tile_row][tile_col] = 0;
        
        return true;
    }
    
    // checks downwards
    if (tile_row + 1 < d && board[tile_row + 1][tile_col] == 0)
    {
        board[tile_row + 1][tile_col] = board[tile_row][tile_col];
        board[tile_row][tile_col] = 0;
        
        return true;
    }
    
    // checks leftwards
    if (tile_col - 1 >= 0 && board[tile_row][tile_col - 1] == 0)
    {
        board[tile_row][tile_col - 1] = board[tile_row][tile_col];
        board[tile_row][tile_col] = 0;
        
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
    // TODO
    int number_counter = 1;
    
    for (int row = 0; row < d; row++)
    {
        for (int col = 0; col < d; col++)
        {
            if (board[row][col] != 0 && number_counter != board[row][col])
            {
                return false;
            }
            
            number_counter++;
        }
    }
    
    return true;
}

/**
 * Saves the current state of the board to disk (for testing).
 */
void save(void)
{
    // log
    const string log = "log.txt";

    // delete existing log, if any, before first save
    static bool saved = false;
    if (!saved)
    {
        unlink(log);
        saved = true;
    }

    // open log
    FILE* p = fopen(log, "a");
    if (p == NULL)
    {
        return;
    }

    // log board
    fprintf(p, "{");
    for (int i = 0; i < d; i++)
    {
        fprintf(p, "{");
        for (int j = 0; j < d; j++)
        {
            fprintf(p, "%i", board[i][j]);
            if (j < d - 1)
            {
                fprintf(p, ",");
            }
        }
        fprintf(p, "}");
        if (i < d - 1)
        {
            fprintf(p, ",");
        }
    }
    fprintf(p, "}\n");

    // close log
    fclose(p);
}
