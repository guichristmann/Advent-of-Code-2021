#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <queue>

struct Cave
{
    std::string m_identifier; 
    bool bigCave;
    std::vector<Cave*> connections;

    Cave() {}
    Cave(std::string identifier, bool bigCave) : m_identifier {identifier}, bigCave {bigCave} {}
};

std::vector<Cave*>::const_iterator find(const std::vector<Cave*>& caves, std::string identifier)
{
    std::vector<Cave*>::const_iterator itr;
    for (itr = caves.begin(); itr != caves.end(); itr++)
    {
        if ((*itr)->m_identifier == identifier)
            break;
    }

    return itr;
}

void checkAndInsertNewNode(std::vector<Cave*>& caves, const std::string& identifier_a, 
        const std::string& identifier_b)
{
    auto itr_b = find(caves, identifier_b);
    Cave* caveB;
    if ( itr_b == caves.end() )
    {
        caveB = new Cave(identifier_b, std::all_of(identifier_b.begin(), identifier_b.end(), ::isupper));
        caves.push_back(caveB);
    } else 
    {
        //std::cout << caveB->m_identifier << " already in graph." << std::endl;
        caveB = *itr_b;
    }

    auto itr_a = find(caves, identifier_a);
    Cave* caveA;
    if (itr_a == caves.end())
    {
        //std::cout << identifier_a << " not found in caves." << std::endl;

        caveA = new Cave(identifier_a, 
                std::all_of(identifier_a.begin(), identifier_a.end(), ::isupper));
        caves.push_back(caveA);
    } else 
    {
        caveA = *itr_a;
        //std::cout << caveA->m_identifier << " already in graph." << std::endl;
    }

    auto itrBinA = std::find(caveA->connections.begin(), caveA->connections.end(), caveB);
    if (itrBinA == caveA->connections.end())
        caveA->connections.push_back(caveB);

    auto itrAinB = std::find(caveB->connections.begin(), caveB->connections.end(), caveA);
    if (itrAinB == caveB->connections.end())
        caveB->connections.push_back(caveA);
}

// Returns the root node of the cave graph.
Cave* readInputAndBuildGraph(std::ifstream& is)
{
    // Utility vector to keep track of every cave.
    std::vector<Cave*> caves { };

    std::string tmpLine;
    while ( std::getline(is, tmpLine) )
    {
        std::string::size_type separatorIndex = tmpLine.find('-');

        std::string identifier_a = tmpLine.substr(0, separatorIndex);
        std::string identifier_b = tmpLine.substr(separatorIndex + 1);

        //std::cout << identifier_a << " -> " << identifier_b << std::endl;

        checkAndInsertNewNode(caves, identifier_a, identifier_b);
    }

    Cave* start;

    for (Cave* c : caves)
    {
        if ( c->m_identifier == "start" )
            start = c;
    }

    return start;
}

void printGraph(Cave* start)
{
    std::vector<Cave*> q; 
    q.push_back(start);
    std::vector<Cave*> visited = { start };

    while ( !q.empty() )
    {
        Cave* currCave = q.at(0);
        q.erase(q.begin());
        visited.push_back(currCave);

        for ( Cave* neighbor : currCave->connections)
        {
            auto itr = std::find(visited.begin(), visited.end(), neighbor);
            auto itr2 = std::find(q.begin(), q.end(), neighbor);
            if ( itr == visited.end() && itr2 == q.end())
            {
                q.push_back(neighbor);
            }
        }

        std::cout << currCave->m_identifier << " : " << currCave->bigCave << std::endl;
    }
}

void traverse(Cave* currNode, std::vector<Cave*> path, int& validPathCount, bool firstDoubleVisit)
{
    if (currNode->m_identifier == "end")
    {
        validPathCount++;
        return;
    }

    // Have I been to this node before?
    auto itr = std::find(path.begin(), path.end(), currNode);
    bool isBigCave = std::all_of(currNode->m_identifier.begin(), currNode->m_identifier.end(), ::isupper);
    if ( itr != path.end() && !isBigCave)
    {
        if (!firstDoubleVisit || currNode->m_identifier == "start" || currNode->m_identifier == "end")
            return;
        else
            firstDoubleVisit = false;
    }

    //std::cout << currNode->m_identifier << std::endl;

    path.push_back(currNode);
    for ( auto neighborCave : currNode->connections )
    {
        traverse(neighborCave, path, validPathCount, firstDoubleVisit);
    }
}

int countPossiblePaths(Cave* start)
{
    int validPathCount = 0;
    std::vector<Cave*> path;
    bool firstDoubleVisit = true;

    traverse(start, path, validPathCount, firstDoubleVisit);

    return validPathCount;
}


int main(int argc, char* argv[])
{
    std::ifstream is(argv[1]);

    if ( !is.is_open() )
    {
        std::cerr << "Failed to open file!" << std::endl;
        return 1;
    }

    Cave* start = readInputAndBuildGraph(is);

    std::cout << countPossiblePaths(start) << std::endl;


    return 0;
}
