#include "roads.h"
#include <iostream>
#include <ostream>
#include <vector>
#include <list>
#include <algorithm>
#include <array>

using namespace std;

Route::Route(vector<vector<char>>& input) //constructor which takes the map info from the main and initializes distance to 0
{
    dist = 0;
    map = input;
}

bool Route::getUserInput(char mode) //handle user inputs
{
    char c;
    if(!isdigit(mode)) //make sure its a number
        return false;
    if(mode - '0' > 4 || mode - '0' < 1) //make sure it's 1-4
        return false;
    if(mode == '1')
        printGraph('A', 1);
    if(mode == '2')
    {
        std::cout << "Stop to check connections of (letter): ";
        std::cin >> c;
        printGraph(toupper(c), 2);
    }
    if(mode == '3')
    {
        std::cout << "Stop to start from (letter): ";
        std::cin >> c;
        prims(toupper(c));
    }
    if(mode == '4')
    {
        startDisj();
        printDis();
    }
    return true;
}

void Route::CopyMap() //makes copy of map to work with
{
    mapCopy = map;
}

void Route::getStops() //find all the stops to work with
{
    for (int r = 0; r < map.size(); r++)
    {
        for (int c = 0; c < map.size(); c++)
        {
            if (isalpha(map[r][c]))
            {
                stops.push_back(map[r][c]);          //add to stops vector
                stopsRC.push_back(make_pair(r, c));  //add location to row/colum vector for each stop
            }
        }
    }
    bucketSort();
}

void Route::bucketSort() //order the list of stops for alphabetical output
{
    array<std::vector<std::pair<char,std::pair<int, int>>>, 26> buckets; int j = 0;
    for (int i = 0; i < stops.size(); ++i) 
    {
        buckets[stops[i] - 'A'].push_back({stops[i], stopsRC[i]});
    }
    for (auto& bucket : buckets) 
    {
        std::sort(bucket.begin(), bucket.end());
        for (auto& pair : bucket) 
        {
            stops[j] = pair.first;
            stopsRC[j] = pair.second;
            j++;
        }
    }
}

bool Route::isValid(char checking, int r, int c) //check if next spot is offroad or the starting spot
{
    if (mapCopy[r][c] != '-' && mapCopy[r][c] != checking)
    {
        return true;
    }
    return false;
}

bool Route::getConn(char checking, int r, int c) //find all the connections from a spot
{
    int nrow, ncol;
    bool wasDS = false;
    if (mapCopy[r][c] == '.' || mapCopy[r][c] == checking) //exceptionhandling so you dont increment/decrement distance on curves
        wasDS = true;

    if (!wasDS)
    {
        dist++;
    }
    //base case, have to scan because can travel diagonally to another road sticking out of the stop otherwise
    if(isalpha(mapCopy[r][c]) && mapCopy[r][c] != checking)
    {
        L.push_back(make_pair(mapCopy[r][c], dist));
        mapCopy[r][c] = '-';
        dist--;
        return false;
    }
    
    mapCopy[r][c] = '-';
    
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if ((i != 0 || j != 0) && (i == 0 || j == 0) && r + i >= 0 && r + i < map.size() && c + j >= 0 && c + j < map.size())
            {
                nrow = r + i; ncol = c + j; 
                if(isValid(checking, nrow, ncol))
                    getConn(checking, nrow, ncol);
            }
        }
    }

    if (!wasDS)
        dist--;
    
    wasDS = false;
    return false;
}

void Route::getStopConnections()
{
    for (int i = 0; i < stops.size(); i++)
    {
        CopyMap(); dist = 0;
        getConn(stops[i], stopsRC[i].first, stopsRC[i].second);
        graph.push_back(L); //add the new list to the vector
        L.clear(); //reset for the next list
    }
}
void Route::startDisj()
{
    for (int i = 0; i < graph.size(); i++)//loop through the graph
    {
        for (it = graph[i].begin(); it != graph[i].end(); it++)
        {
            makeSet(stops[i], it->first, it->second);//make disjoint-set
            U(stops[i], it->first, it->second);//check for duplicate connections, delete them, merge
        }
    }
}

void Route::makeSet(char src, char dest, int edge) {
    element newElem;
    newElem.root = src;
    newElem.dest = dest;
    newElem.edge = edge;

    elementMap.push_back(newElem);//add new

}

bool Route::find(element x) {
    int count = 0;
    for (int i = 0; i < elementMap.size(); i++)
    {

        if (((elementMap[i].root == x.root && elementMap[i].dest == x.dest) || (elementMap[i].root == x.dest && elementMap[i].dest == x.root)) && elementMap[i].edge == x.edge)
        {
            count++;
            if (count > 1) {
                elementMap.erase(elementMap.begin() + i);
                return true;
            }
        }
    }
    return false;

}

void Route::U(char src, char dest, int w) {
    element temp = {src, dest, w};
    if(find(temp)) {
        return;
    }
}

bool Route::isConnected(vector<char>& conn, char c) //checks if the node is already connected for prims
{
    for (int i = 0; i < conn.size(); i++) 
    {
        if (conn[i] == c)
            return true;
    }
    return false;
}

void Route::prims(char start)
{
    //keeping track of the shortest iterator, shortest connection and its postion, and which positions are already connected
    vector<char> connected; list<pair<char, int>>::iterator lowestIt; int lowestPos = -1, MST = 0, count = 0; 
    connected.push_back(start);
    
    //loop through whole graph
    while (connected.size() < stops.size())
    {
        for (int i = 0; i < graph.size(); i++) 
        {
            for (it = graph[i].begin(); it != graph[i].end(); ++it) 
            {
                //only look at pairs going from a connected node to disconnected
                if(isConnected(connected,stops[i]) && !isConnected(connected,it->first))
                {
                    //set initial lowest if not done so
                    if(lowestPos < 0)
                    {
                        lowestIt = it; lowestPos = i;
                    }
                    //check if lower
                    if (it->second < lowestIt->second)
                    {
                        lowestIt = it; lowestPos = i;
                    }
                
                }
            }
        }
        int* counter = &count;
        //cout << stops[lowestPos] << " -> " << lowestIt->first << " : " << lowestIt->second << "\n"; //output
        printRoute(counter, stops[lowestPos], lowestIt->first, lowestIt->second);
        count++;
        if ((count + 2) % 5 == 0) //just to space the output a bit
            std::cout << endl;
        
        connected.push_back(lowestIt->first); //add new connection to connected
        lowestPos = -1; MST += lowestIt->second; //reset lowest and add to distance total
    }
    cout << "\nMST: " << MST << endl;
}

void Route::printDis() {

    for (int i = 0; i < elementMap.size(); i++) {

        cout << elementMap[i].root << " -- " << elementMap[i].dest << " : " << elementMap[i].edge << endl;

    }

}

void Route::printGraph(char c, int mode)
{
    for (int i = 0; i < graph.size(); i++) 
    {
        if (c == stops[i] || mode == 1)
        {
            std::cout << stops[i] << ": ";
            for (it = graph[i].begin(); it != graph[i].end(); ++it) 
            {
                cout << it->first << " - " << it->second << " | ";
            }
        cout << "\n";
        }
    }
}

void Route::printRoute(int* counter, char src, char dest, int edge) {
    // Mark the edges of the MST on the MST map
    if (*counter == 0) { //print car, first node
        cout <<"  ______\n" << " /|_||_\\`.__\n"
             << "(   _    _ _\\\n" << "=`-(_)--(_)-' "
             << src << " -" << edge << "-> " << dest << ", ";
    } else { cout << src << " -" << edge << "-> " << dest << ", "; }

}
