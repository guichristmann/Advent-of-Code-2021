#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <unordered_map>
#include <cassert>
#include <algorithm>
#include <queue>

constexpr int32_t HALLWAY_SIZE = 11;

// Index of the room entrnace in hallway.
const std::unordered_map<char, int32_t> room_entrance({{'A', 2},
                                                       {'B', 4},
                                                       {'C', 6},
                                                       {'D', 8}});

const std::unordered_map<char, int32_t> move_cost({{'A', 1},
                                                   {'B', 10},
                                                   {'C', 100},
                                                   {'D', 1000}});

typedef std::array<char, 4> Room;

struct State
{
    State(){};

    std::array<char, HALLWAY_SIZE> hallway{'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'};
    std::unordered_map<char, Room> rooms = {
        {'A', {'.', '.', '.', '.'}},
        {'B', {'.', '.', '.', '.'}},
        {'C', {'.', '.', '.', '.'}},
        {'D', {'.', '.', '.', '.'}}};

    int64_t totalEnergy = 0;

    void Print() const;
    std::vector<State> findTransitions() const;
    bool roomIsValid(char) const;
    bool wayIsValid(const int32_t, const int32_t) const;
    bool isEndState() const;
    int32_t findRoomSpot(const char) const;

    bool operator==(const State &other) const
    {
        for (int32_t i = 0; i < HALLWAY_SIZE; i++)
        {
            if (hallway[i] != other.hallway[i])
            {
                return false;
            }
        }

        for (int32_t i = 0; i < 4; i++)
        {
            if (rooms.at('A')[i] != other.rooms.at('A')[i] ||
                rooms.at('B')[i] != other.rooms.at('B')[i] ||
                rooms.at('C')[i] != other.rooms.at('C')[i] ||
                rooms.at('D')[i] != other.rooms.at('D')[i])
            {
                return false;
            }
        }

        return true;
    }
};

void State::Print() const
{
    std::cout << "#############" << std::endl;
    // 2nd line, the hallway.
    std::cout << "#";
    for (const char c : hallway)
    {
        std::cout << c;
    }
    std::cout << "#" << std::endl;

    std::cout << "###";
    std::cout << rooms.at('A')[0] << "#";
    std::cout << rooms.at('B')[0] << "#";
    std::cout << rooms.at('C')[0] << "#";
    std::cout << rooms.at('D')[0] << "#";
    std::cout << "##" << std::endl;

    std::cout << "  #" << rooms.at('A')[1] << "#";
    std::cout << rooms.at('B')[1] << "#";
    std::cout << rooms.at('C')[1] << "#";
    std::cout << rooms.at('D')[1] << "#";
    std::cout << std::endl;

    std::cout << "  #" << rooms.at('A')[2] << "#";
    std::cout << rooms.at('B')[2] << "#";
    std::cout << rooms.at('C')[2] << "#";
    std::cout << rooms.at('D')[2] << "#";
    std::cout << std::endl;

    std::cout << "  #" << rooms.at('A')[3] << "#";
    std::cout << rooms.at('B')[3] << "#";
    std::cout << rooms.at('C')[3] << "#";
    std::cout << rooms.at('D')[3] << "#";
    std::cout << std::endl;

    std::cout << "  #########  " << std::endl;
    std::cout << "Total energy: " << totalEnergy << std::endl;
    std::cout << std::endl;
}

// Checks if the room is in a valid "enterable" state.
bool State::roomIsValid(char room_type) const
{
    for (int32_t i = 3; i >= 0; i--)
    {
        if (rooms.at(room_type).at(i) != room_type && rooms.at(room_type).at(i) != '.')
        {
            return false;
        }
    }

    return true;
}

bool State::wayIsValid(const int32_t hallway_start, const int32_t hallway_target) const
{
    int32_t startIdx = std::min(hallway_start, hallway_target);
    int32_t stopIdx = std::max(hallway_start, hallway_target);
    for (int32_t idx = startIdx + 1; idx < stopIdx; idx++)
    {
        if (hallway.at(idx) != '.')
        {
            // Something is on the way.
            return false;
        }
    }

    return true;
}

int32_t State::findRoomSpot(const char room_type) const
{
    int32_t i = 3;
    for (; i >= 0; i--)
    {
        if (rooms.at(room_type).at(i) == '.')
        {
            return i;
        }
    }

    return i;
}

std::vector<State> State::findTransitions() const
{
    std::vector<State> transitions;

    // From Room into the hallway.
    for (const auto &pair : rooms)
    {
        const char room_type = pair.first;
        const Room& room = pair.second;

        for (int32_t roomIdx = 0; roomIdx < 4; roomIdx++)
        {
            if (room[roomIdx] != '.')
            {
                const char amphipod_type = room[roomIdx];

                bool othersAllCorrect = true;
                for (int32_t tmpRoomIdx = roomIdx + 1; tmpRoomIdx < 4; tmpRoomIdx++)
                {
                    if (room[tmpRoomIdx] != room_type )
                    {
                        othersAllCorrect = false;
                        break;
                    }
                }

                if (amphipod_type == room_type && othersAllCorrect)
                {
                    // No need to move anything.
                    break;
                }

                // Move amphipod into the hallway.
                for (int32_t hallwayIdx = 0; hallwayIdx < HALLWAY_SIZE; hallwayIdx++)
                {
                    if (hallway.at(hallwayIdx) != '.' || !wayIsValid(room_entrance.at(room_type), hallwayIdx) ||
                        hallwayIdx == room_entrance.at('A') || hallwayIdx == room_entrance.at('B') ||
                        hallwayIdx == room_entrance.at('C') || hallwayIdx == room_entrance.at('D'))
                    {
                        continue;
                    }

                    State new_state(*this);
                    new_state.hallway[hallwayIdx] = amphipod_type;
                    new_state.rooms.at(room_type)[roomIdx] = '.';
                    int64_t dist = std::abs(hallwayIdx - room_entrance.at(room_type)) + roomIdx + 1;
                    new_state.totalEnergy += dist * move_cost.at(amphipod_type);

                    transitions.push_back(new_state);
                }

                break;
            }
        }
    }

    // From hallway into the room.
    for (int32_t hallwayIdx = 0; hallwayIdx < HALLWAY_SIZE; hallwayIdx++)
    {
        // No amphipod at this spot in the hallway.
        if (hallway[hallwayIdx] == '.')
            continue;

        const char amphipod_type = hallway[hallwayIdx];
        if (roomIsValid(amphipod_type) && wayIsValid(hallwayIdx, room_entrance.at(amphipod_type)))
        {
            // Generate the new state, moving the amphipod into the room.
            State new_state(*this);
            new_state.hallway[hallwayIdx] = '.';
            int32_t roomIdx = findRoomSpot(amphipod_type);
            assert(roomIdx >= 0 && roomIdx < 4);

            new_state.rooms.at(amphipod_type)[roomIdx] = amphipod_type;
            int64_t dist = std::abs(hallwayIdx - room_entrance.at(amphipod_type)) + roomIdx + 1;
            new_state.totalEnergy += dist * move_cost.at(amphipod_type);

            transitions.push_back(new_state);
        }
    }

    return transitions;
}

bool State::isEndState() const
{
    for (int32_t i = 0; i < HALLWAY_SIZE; i++)
    {
        if (hallway.at(i) != '.')
            return false;
    }

    for (const auto &pair : rooms)
    {
        const char room_type = pair.first;
        const Room room = pair.second;

        for ( int32_t idx = 0; idx < 4; idx++ )
        {
            if (room.at(idx) != room_type)
            {
                return false;
            }
        }
    }

    return true;
}

class Compare
{
public:
    bool operator()(const State &a, const State &b)
    {
        return a.totalEnergy < b.totalEnergy;
    }
};

int main(int argc, char *argv[])
{
    State state;
    state.totalEnergy = 0;

    // Setting initial condition.
    // Example input.
    // state.rooms.at('A')[0] = 'B';
    // state.rooms.at('A')[1] = 'D';
    // state.rooms.at('A')[2] = 'D';
    // state.rooms.at('A')[3] = 'A';

    // state.rooms.at('B')[0] = 'C';
    // state.rooms.at('B')[1] = 'C';
    // state.rooms.at('B')[2] = 'B';
    // state.rooms.at('B')[3] = 'D';

    // state.rooms.at('C')[0] = 'B';
    // state.rooms.at('C')[1] = 'B';
    // state.rooms.at('C')[2] = 'A';
    // state.rooms.at('C')[3] = 'C';

    // state.rooms.at('D')[0] = 'D';
    // state.rooms.at('D')[1] = 'A';
    // state.rooms.at('D')[2] = 'C';
    // state.rooms.at('D')[3] = 'A';

    state.rooms.at('A')[0] = 'D';
    state.rooms.at('A')[1] = 'D';
    state.rooms.at('A')[2] = 'D';
    state.rooms.at('A')[3] = 'C';

    state.rooms.at('B')[0] = 'A';
    state.rooms.at('B')[1] = 'C';
    state.rooms.at('B')[2] = 'B';
    state.rooms.at('B')[3] = 'A';

    state.rooms.at('C')[0] = 'D';
    state.rooms.at('C')[1] = 'B';
    state.rooms.at('C')[2] = 'A';
    state.rooms.at('C')[3] = 'B';

    state.rooms.at('D')[0] = 'C';
    state.rooms.at('D')[1] = 'A';
    state.rooms.at('D')[2] = 'C';
    state.rooms.at('D')[3] = 'B';

    state.Print();

    int64_t min_energy = INT64_MAX;

    std::priority_queue<State, std::vector<State>, Compare> queue;
    queue.push(state);
    std::vector<State> visited = {};
    while (!queue.empty())
    {
        const State currState = queue.top();
        queue.pop();
        visited.push_back(currState);

        if (currState.isEndState() && currState.totalEnergy <= min_energy)
        {
            min_energy = currState.totalEnergy;
            std::cout << min_energy << " " << visited.size() << " " << queue.size() << std::endl;
            continue;
        }

        for (const State &s : currState.findTransitions())
        {
            auto idx = std::find(visited.begin(), visited.end(), s);
            if (idx == visited.end())
            {
                queue.push(s);
            }
            else if (s.totalEnergy < idx->totalEnergy)
            {
                queue.push(s);
                visited.erase(idx);
            }
        }
    }

    std::cout << "Min energy is " << min_energy << std::endl;

    return 0;
}
