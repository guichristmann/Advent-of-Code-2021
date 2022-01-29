#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>

constexpr int g_player1_start = 8;
constexpr int g_player2_start = 5;
//constexpr int g_player1_start = 4;
//constexpr int g_player2_start = 8;

int64_t getStateScore(const int position)
{
    if ( position == 0 )
        return 10;
    else
        return position;
}

struct State
{
    int player1Pos;
    int player2Pos;
    int player1Score;
    int player2Score;

    State(int p1, int p2, int s1, int s2) : player1Pos(p1), player2Pos(p2), player1Score(s1), 
        player2Score(s2) {};

    bool operator==(const State& other) const
    {
        return (player1Pos == other.player1Pos &&
                player2Pos == other.player2Pos &&
                player1Score == other.player1Score &&
                player2Score == other.player2Score);
    }

    friend std::ostream& operator<<(std::ostream&, const State&);
};

std::ostream& operator<<(std::ostream& out, const State& s)
{
    out << "(" << s.player1Pos << ", " << s.player2Pos << ", " << s.player1Score << ", " << s.player2Score << ")";

    return out;
}

template <>
struct std::hash<State>
{
    std::size_t operator()(const State& s) const
    {
        return (std::hash<int>()(s.player1Pos) ^
                std::hash<int>()(s.player2Pos) ^
                std::hash<int>()(s.player1Score) ^
                std::hash<int>()(s.player2Score));
    }
};

int main()
{
    int64_t player1Wins = 0;
    int64_t player2Wins = 0;

    // Keeps track of how many universes are in each state.
    std::unordered_map<State, uint64_t> stateCount;
    State start(g_player1_start, g_player2_start, 0, 0);
    stateCount.emplace(start, 1);

    std::vector<std::pair<int, int>> rollCounts = {
        {3, 1},
        {4, 3},
        {5, 6},
        {6, 7},
        {7, 6},
        {8, 3},
        {9, 1}
    };

    bool finished = false;
    while ( !finished  )
    {
        finished = true;
        std::unordered_map<State, uint64_t> newStateCount;
        for ( auto& e : stateCount )
        {
            if ( e.second == 0 )
                continue;

            finished = false;

            uint64_t nUniverses = e.second;
            int p1Pos = e.first.player1Pos;
            int p1Score = e.first.player1Score;
            int p2Pos = e.first.player2Pos;
            int p2Score = e.first.player2Score;
            for ( const auto& roll : rollCounts )
            {
                int newP1Position = (p1Pos + roll.first) % 10;
                int newP1Score = p1Score + getStateScore(newP1Position);
                if ( newP1Score >= 21 )
                {
                    player1Wins += nUniverses * roll.second;
                    continue; // No need to roll for player 2. Player 1 already won.
                }

                for ( const auto& roll2: rollCounts )
                {
                    int newP2Position = (p2Pos + roll2.first) % 10;
                    int newP2Score = p2Score + getStateScore(newP2Position);
                    if ( newP2Score >= 21 )
                    {
                        player2Wins += nUniverses * roll2.second * roll.second;
                    }
                    else
                    {
                        newStateCount[State(newP1Position, newP2Position, newP1Score, newP2Score)] += 
                            nUniverses * roll2.second * roll.second;
                    }
                }
            }
        }

        stateCount = newStateCount;
    }

    std::cout << "Player1: " << player1Wins << ", Player2: " << player2Wins << std::endl;
    std::cout << std::max(player1Wins, player2Wins) << std::endl;

    return 0;
}
