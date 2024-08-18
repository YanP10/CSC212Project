#include <iostream>
#include <vector>
#include <list>
#include <ostream>

using namespace std;

struct element 
{
    char root, dest;
    int edge;
};

class Route
{
    public:
        int dist;                               //distance tracker
        vector<vector<char>> map, mapCopy, mstMap;  //map, map copy, and mst map
        vector<char> stops;                     //The stops and the order in which they are kept
        vector<pair<int,int>> stopsRC, primPairs; //Stop co-ordinates which match with stops vector indexing, and pairs for prims
        vector<element> elementMap;             //map for disjoint
        vector<list<pair<char, int>>> graph;    //graph
        list<pair<char, int>> L;                //list to put into graph
        list<pair<char, int>>::iterator it;     //iterator to go through pairs in list

        Route();
        Route(vector<vector<char>>& input);
        bool getUserInput(char mode);
        void bucketSort();
        void CopyMap();
        void getStops();
        bool isValid(char checking, int r, int c);
        bool getConn(char checking, int r, int c);
        void getStopConnections();
        void startDisj();
        void makeSet(char src, char dest, int edge);
        bool find(element x);
        void U(char src, char dest, int w);
        void printDis();
        bool isConnected(vector<char>& conn, char c);
        void prims(char start);
        void printGraph(char c, int mode);
        void printRoute(int* counter, char src, char dest, int edge);
};
