#include <iostream>

// Input is quite simple in this one so we define the bounds directly.
#define MIN_Y -107
#define MAX_Y -57

bool simulate(int yVel, int& outMaxHeight)
{
    bool reachedTarget = false;
    bool pastTarget = false;
    
    int steps = 0;
    int yPos = 0;
    int currSimMaxHeight = 0;
    int gap = MAX_Y - MIN_Y;
    while ( !reachedTarget && !pastTarget )
    {
        yPos += yVel; 
        yVel -= 1;
        if ( yPos > currSimMaxHeight )
            currSimMaxHeight = yPos;

        if ( yPos >= MIN_Y && yPos <= MAX_Y )
            reachedTarget = true;
        else if ( yPos <= MIN_Y )
            pastTarget = true;

        steps++;
    }

    if ( reachedTarget ) 
        outMaxHeight = currSimMaxHeight;

    return false;
}

int main(int argc, char* argv[])
{
    int maxHeight = 0;
    for ( int startVel = 0; startVel < -MIN_Y; startVel++ )
    {
        simulate(startVel, maxHeight);
    }

    std::cout << maxHeight << std::endl;

    return 0;
}
