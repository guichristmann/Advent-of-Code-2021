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

typedef std::array<char, 2> Room;

struct State
{
    State(){};

    std::array<char, HALLWAY_SIZE> hallway{'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'};
    std::unordered_map<char, Room> rooms = {
        {'A', {'.', '.'}},
        {'B', {'.', '.'}},
        {'C', {'.', '.'}},
        {'D', {'.', '.'}}};

    int64_t totalEnergy = 0;

    void Print() const;
    std::vector<State> findTransitions() const;
    bool roomIsValid(char) const;
    bool wayIsValid(const int32_t, const int32_t) const;
    bool isEndState() const;

    bool operator==(const State &other) const
    {
        for (int32_t i = 0; i < HALLWAY_SIZE; i++)
        {
            if (hallway[i] != other.hallway[i])
            {
                return false;
            }
        }

        if (rooms.at('A')[0] != other.rooms.at('A')[0] ||
            rooms.at('A')[1] != other.rooms.at('A')[1] ||
            rooms.at('B')[0] != other.rooms.at('B')[0] ||
            rooms.at('B')[1] != other.rooms.at('B')[1] ||
            rooms.at('C')[0] != other.rooms.at('C')[0] ||
            rooms.at('C')[1] != other.rooms.at('C')[1] ||
            rooms.at('D')[0] != other.rooms.at('D')[0] ||
            rooms.at('D')[1] != other.rooms.at('D')[1])
        {
            return false;
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
    // 3rd line.
    std::cout << "###";
    std::cout << rooms.at('A')[0] << "#";
    std::cout << rooms.at('B')[0] << "#";
    std::cout << rooms.at('C')[0] << "#";
    std::cout << rooms.at('D')[0] << "#";
    std::cout << "##" << std::endl;

    // 4th line.
    std::cout << "  #" << rooms.at('A')[1] << "#";
    std::cout << rooms.at('B')[1] << "#";
    std::cout << rooms.at('C')[1] << "#";
    std::cout << rooms.at('D')[1] << "#";
    std::cout << std::endl;

    std::cout << "  #########  " << std::endl;
    std::cout << "Total energy: " << totalEnergy << std::endl;
    std::cout << std::endl;
}

// Checks if the room is in a valid "enterable" state.
bool State::roomIsValid(char room_type) const
{
    if (rooms.at(room_type)[0] == '.' && rooms.at(room_type)[1] == '.')
    {
        return true;
    }
    else if (rooms.at(room_type)[0] != '.' && rooms.at(room_type)[1] == '.')
    {
        if (rooms.at(room_type)[0] == room_type)
        {
            return true;
        }
    }
    else if (rooms.at(room_type)[0] == '.' && rooms.at(room_type)[1] != '.')
    {
        if (rooms.at(room_type)[1] == room_type)
        {
            return true;
        }
    }

    return false;
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

std::vector<State> State::findTransitions() const
{
    std::vector<State> transitions;

    // From Room into the hallway.
    for (const auto &pair : rooms)
    {
        const char room_type = pair.first;
        const Room room = pair.second;

        if (room[1] == '.')
        {
            assert(room[0] == '.');
        }
        else if (room[0] != '.')
        {
            assert(room[1] != '.');
            if ( room[0] == room_type && room[1] == room_type )
                continue;

            const char amphipod_type = room[0];
            for (int32_t hallwayIdx = 0; hallwayIdx < HALLWAY_SIZE; hallwayIdx++)
            {
                if (hallwayIdx == room_entrance.at('A') || hallwayIdx == room_entrance.at('B') ||
                    hallwayIdx == room_entrance.at('C') || hallwayIdx == room_entrance.at('D') )
                    continue;

                if (hallway.at(hallwayIdx) == '.' && wayIsValid(room_entrance.at(room_type), hallwayIdx))
                {
                    State new_state(*this);
                    new_state.hallway[hallwayIdx] = amphipod_type;
                    new_state.rooms.at(room_type)[0] = '.';
                    int64_t dist = std::abs(hallwayIdx - room_entrance.at(room_type)) + 1;
                    assert(dist > 0);
                    new_state.totalEnergy += dist * move_cost.at(amphipod_type);
                    transitions.push_back(new_state);
                }
            }
        }
        else if ((room[0] == '.' && room[1] != '.') && room[1] != room_type)
        {
            const char amphipod_type = room[1];
            for (int32_t hallwayIdx = 0; hallwayIdx < HALLWAY_SIZE; hallwayIdx++)
            {
                if (hallwayIdx == room_entrance.at('A') || hallwayIdx == room_entrance.at('B') ||
                    hallwayIdx == room_entrance.at('C') || hallwayIdx == room_entrance.at('D') )
                    continue;

                if (hallway.at(hallwayIdx) == '.' && wayIsValid(room_entrance.at(room_type), hallwayIdx))
                {
                    State new_state(*this);
                    new_state.hallway[hallwayIdx] = amphipod_type;
                    new_state.rooms.at(room_type)[1] = '.';
                    int64_t dist = std::abs(hallwayIdx - room_entrance.at(room_type)) + 2;
                    assert(dist > 0);
                    new_state.totalEnergy += dist * move_cost.at(amphipod_type);
                    transitions.push_back(new_state);
                }
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
            if (new_state.rooms.at(amphipod_type)[1] == '.')
            {
                new_state.rooms.at(amphipod_type)[1] = amphipod_type;
                int64_t dist = std::abs(hallwayIdx - room_entrance.at(amphipod_type)) + 2;
                assert(dist > 0);
                new_state.totalEnergy += dist * move_cost.at(amphipod_type);
            }
            else
            {
                new_state.rooms.at(amphipod_type)[0] = amphipod_type;
                int64_t dist = std::abs(hallwayIdx - room_entrance.at(amphipod_type)) + 1;
                assert(dist > 0);
                new_state.totalEnergy += dist * move_cost.at(amphipod_type);
            }

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

        if (!(room.at(0) != '.' && room.at(0) == room_type &&
              room.at(1) != '.' && room.at(1) == room_type))
            return false;
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
    // state.rooms.at('A')[1] = 'A';
    // state.rooms.at('B')[0] = 'C';
    // state.rooms.at('B')[1] = 'D';
    // state.rooms.at('C')[0] = 'B';
    // state.rooms.at('C')[1] = 'C';
    // state.rooms.at('D')[0] = 'D';
    // state.rooms.at('D')[1] = 'A';

    // Real input.
    state.rooms.at('A')[0] = 'D';
    state.rooms.at('A')[1] = 'C';
    state.rooms.at('B')[0] = 'A';
    state.rooms.at('B')[1] = 'A';
    state.rooms.at('C')[0] = 'D';
    state.rooms.at('C')[1] = 'B';
    state.rooms.at('D')[0] = 'C';
    state.rooms.at('D')[1] = 'B';

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
            std::cout << min_energy << std::endl;
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
