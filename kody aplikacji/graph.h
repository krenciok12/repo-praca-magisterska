#ifndef GRAPH
#define GRAPH

#include <vector>

class Graph
{
public:
    Graph(int n);
    ~Graph();

    void addEdge(std::pair<int,int> edge);
    void addEdges(std::vector<std::pair<int,int>> edges);
    std::vector<int> geVertexEdges(int v);
    int getVertex();

private:
    int nodes;
    std::vector<std::vector<int>> edges;
};

#endif
