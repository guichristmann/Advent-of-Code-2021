#include <iostream>

constexpr int g_player1_start = 8;
//constexpr int g_player1_start = 4;
constexpr int g_player2_start = 5;
//constexpr int g_player2_start = 8;

int64_t rollDie(const int64_t rollCount)
{
    int64_t start = (rollCount % 100) * 3;
    return (start + 1) + (start + 2) + (start + 3);
}

int main()
{
    int player1 = g_player1_start;
    int player2 = g_player2_start;
    int64_t rollCount = 0;
    int64_t player1Score = 0;
    int64_t player2Score = 0;

    while ( player1Score < 1000 && player2Score < 1000 )
    {
        int64_t v = rollDie(rollCount);
        player1 = (player1 + v) % 10;
        if ( player1 == 0 )
            player1Score += 10;
        else
            player1Score += player1;
        rollCount++;

        // Break early if score reached.
        if ( player1Score >= 1000 )
            break;

        v = rollDie(rollCount);
        player2 = (player2 + v) % 10;
        if ( player2 == 0 )
            player2Score += 10;
        else
            player2Score += player2;
        rollCount++;
    }

    int64_t result = std::min(player1Score, player2Score) * rollCount * 3;
    std::cout << result << std::endl;

    return 0;
}
