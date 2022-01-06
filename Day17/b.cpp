#include <iostream>

// Input is quite simple in this one so we define the bounds directly.
#define MIN_Y -107
#define MAX_Y -57
#define MIN_X 230
#define MAX_X 283

// Test input.
//#define MIN_Y -10
//#define MAX_Y -5
//#define MAX_X 30
//#define MIN_X 20

bool simulate(int xVel, int yVel)
{
    bool reachedTarget = false;
    bool pastTarget = false;
    
    int steps = 0;
    int xPos = 0;
    int yPos = 0;
    while ( !reachedTarget && !pastTarget )
    {
        xPos += xVel;
        yPos += yVel; 
        yVel -= 1;
        xVel -= 1;
        
        if ( xVel < 0 )
            xVel = 0;

        if ( yPos >= MIN_Y && yPos <= MAX_Y && xPos >= MIN_X && xPos <= MAX_X)
            reachedTarget = true;
        else if ( yPos < MIN_Y || xPos > MAX_X )
            pastTarget = true;

        steps++;
    }

    return reachedTarget;
}

int main(int argc, char* argv[])
{
    // Brute force with delimited search space. Still quite fast at < 10 ms.
    int passThroughCount = 0;
    for ( int startY = MIN_Y; startY <= -MIN_Y; startY++ )
    {
        for ( int startX = 0; startX <= MAX_X; startX++ )
        if ( simulate(startX, startY) )
        {
            passThroughCount++;
        }
    }

    std::cout << passThroughCount << std::endl;

    return 0;
}
