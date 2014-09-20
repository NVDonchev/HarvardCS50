//
// breakout.c
//
// Computer Science 50
// Problem Set 4
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// paddle properties
#define PADDLE_HEIGHT 8
#define PADDLE_WIDTH 60
#define PADDLE_HALF_WIDTH PADDLE_WIDTH / 2
#define PADDLE_DIST_FROM_BOTTOM 20

// brick properties
#define BRICK_HEIGHT 15
#define BRICK_WIDTH 37
#define BRICK_SPACING 3

// ball initial position
#define BALL_INIT_POS_X (getWidth(window) / 2) - RADIUS
#define BALL_INIT_POS_Y (getHeight(window) / 2) - RADIUS

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);
void movePaddle(GWindow window, GRect paddle);
char* getColor(int row);
void moveBall(GWindow window, GOval ball, double* ball_movement_x, double* ball_movement_y);
void setInitialBallVelosity(double* ball_movement_x, double* ball_movement_y, double rand);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;

    // adjusts the velosity of the ball
    double rand = drand48();
    double* ball_movement_x = malloc(sizeof(double));
    double* ball_movement_y = malloc(sizeof(double));
    setInitialBallVelosity(ball_movement_x, ball_movement_y, rand);
    
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        // moves the paddle by following mouse cursor movement
        movePaddle(window, paddle);
        
        // moves the ball and bouning it from the edges
        moveBall(window, ball, ball_movement_x, ball_movement_y);
        
        // detects and handles ball collisions
        GObject object = detectCollision(window, ball);
        if (object != NULL)
        {
            if (object == paddle)
            {
                *ball_movement_y = -*ball_movement_y;
            }
            else if (strcmp(getType(object), "GRect") == 0)
            {
                removeGWindow(window, object);
                *ball_movement_y = -*ball_movement_y;
                points++;
                bricks--;
            }
        }
        
        // handles losing of live
        if (getY(ball) >= HEIGHT - 20)
        {
            lives--;
            setLocation(ball, BALL_INIT_POS_X, BALL_INIT_POS_Y);
            setInitialBallVelosity(ball_movement_x, ball_movement_y, rand);
            waitForClick();
        }
        
        // updates scores
        updateScoreboard(window, label, points);
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    // sets initial brick position
    int brick_pos_x = 1;
    int brick_pos_y = 12;
    
    for (int row = 0; row < ROWS; row++)
    {
        // gets differrent color for every row
        char* color = getColor(row);
    
        for (int col = 0; col < COLS; col++)
        {
            // creates a brick
            GRect brick = newGRect(brick_pos_x, brick_pos_y, BRICK_WIDTH, BRICK_HEIGHT);
            setFilled(brick, true);
            setColor(brick, color);
            
            // sets the brick position x
            brick_pos_x += BRICK_WIDTH + BRICK_SPACING;
            
            add(window, brick);
        }
        
        // resets brick position so a new row is started
        brick_pos_x = 1;
        brick_pos_y += BRICK_HEIGHT + BRICK_SPACING;
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    // sets the ball initial position
    int ball_pos_x = BALL_INIT_POS_X;
    int ball_pos_y = BALL_INIT_POS_Y;
    
    // creates a ball
    GOval ball = newGOval(ball_pos_x, ball_pos_y, RADIUS * 2, RADIUS * 2);
    setFilled(ball, true);
    setColor(ball, "BLACK");
    
    // adds the ball to the game window
    add(window, ball);
    
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    // calculates paddle initial coordinates
    int paddle_pos_x = (getWidth(window) / 2) - PADDLE_HALF_WIDTH;
    int paddle_pos_y = getHeight(window) - PADDLE_DIST_FROM_BOTTOM;
    
    // creates a paddle at initial location, with appropriate size and color
    GRect paddle = newGRect(paddle_pos_x, paddle_pos_y, PADDLE_WIDTH, PADDLE_HEIGHT);
    setFilled(paddle, true);
    setColor(paddle, "BLACK");
   
    // addes the paddle to the game window
    add(window, paddle);
    
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    // creates a label
    GLabel label = newGLabel("0");
    setFont(label, "SansSerif-42");
    setColor(label, "LIGHT_GRAY");
    int x = (WIDTH - getWidth(label)) / 2;
    int y = (HEIGHT + getFontAscent(label)) / 2;
    
    // sets it's location
    setLocation(label, x, y);
    
    // adds it to the game window
    add(window, label);
    
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}

/**
 * Moves the paddle according to the mouse pointer movement.
 */
void movePaddle(GWindow window, GRect paddle)
{
    // initializes a mouse event
    GEvent event = getNextEvent(MOUSE_EVENT);
        
    // checks if there is activity
    if (event != NULL)
    {
    // checks if the activity is of type mouse moved
        if (getEventType(event) == MOUSE_MOVED)
        {
            // gets the coordinates of the paddle
            double x = getX(event) - PADDLE_HALF_WIDTH;
            double y = getHeight(window) - PADDLE_DIST_FROM_BOTTOM;
            
            // if the coordinates are in the game window,
            // sets the location of the paddle
            if (x >= 0 && x < WIDTH - PADDLE_WIDTH)
            {
                setLocation(paddle, x, y);
            }
            // if the coordinates are to the left of the game window, 
            // sets the location of the paddle to far left
            else if (x < 0)
            {
                x = 0;
                setLocation(paddle, x, y);
            }
            // if the coordinates are to the right of the game window, 
            // sets the location of the paddle to far right
            else
            {
                x = WIDTH - PADDLE_WIDTH;
                setLocation(paddle, x, y);
            }
        }
    }
}

/**
 * Gets a color for the bricks depending on the row number,
 * so that every row of bricks is different color.
 */
char* getColor(int row)
{
    if (row > 4)
    {
        row = row % 5;
    }
    
    switch (row)
    {
        case 0:
            return "MAGENTA";
            break;
        case 1:
            return "BLUE";
            break;
        case 2:
            return "ORANGE";
            break;
        case 3:
            return "GREEN";
            break;
        case 4:
            return "CYAN";
            break;
        default:
            return "BLACK";
            break;
        
    }

    return NULL;
}

/**
 * Moves the ball.
 */
void moveBall(GWindow window, GOval ball, double* ball_movement_x, double* ball_movement_y)
{
    // changes move direction on the x axis
    if (getX(ball) <= 0 || getX(ball) >= WIDTH - getWidth(ball))
    {
        *ball_movement_x = -*ball_movement_x;
    }
    // changes move direction on the y axis
    else if (getY(ball) <= 0 || getY(ball) >= HEIGHT - getWidth(ball))
    {
        *ball_movement_y = -*ball_movement_y;
    }
    
    // moves the ball
    move(ball, *ball_movement_x, *ball_movement_y);
    
    // pauses so movement speed is good
    pause(2);
}

/**
 * Sets a random velosity to the ball.
 */
void setInitialBallVelosity(double* ball_movement_x, double* ball_movement_y, double rand)
{
    if (rand <= 0.4)
    {
        *ball_movement_x = 0.4;
    }
    else if (rand <= 0.7)
    {
         *ball_movement_x = -rand;
    }
    else
    {
        *ball_movement_x = rand;
    }
    
    *ball_movement_y = 1;
}
