#include "roads.h"
#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
using namespace std;
//g++ main.cpp roads.cpp -o prog && ./prog test1.txt

//parameters are:
//1 for all connections
//2 for connections of a character
//3 for Prim's starting a character
//4 for disjoint (all pairs without repetition)

int main(int argc, char*argv[])
{
    vector<vector<char>> input;

    string file = argv[1];//file name
    ifstream ifs(file);
    string line;
    char ans;
    while (getline(ifs, line)) {
        if (line.empty()) {
            continue; // Skip empty lines
        }
        istringstream iss(line);
        vector<char> row;
        char cell;
        while (iss >> cell) {
            row.push_back(cell);
        }
        input.push_back(row);
    }
    ifs.close();
    
    Route route(input);

    route.getStops(); //find all the stops and their locations
    route.getStopConnections(); //get the connections for each stop

    std::cout << "1 for all connections\n2 for connections of one character\n3 for Prim's\n4 for disjoint pairs\n-------------------------\n";
    bool valid = false;
    while (!valid)
    {
        std::cout << "Please enter a valid mode: ";
        cin >> ans;
        valid = route.getUserInput(ans);
    }
}

