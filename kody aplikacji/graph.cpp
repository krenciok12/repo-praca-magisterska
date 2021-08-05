#include "graph.h"

Graph::Graph(int n): nodes(n){
  std::vector<std::vector<int>> ed;
  this->edges = ed;
  for (int i=0; i<n; i++){
    std::vector<int> vec;
    edges.push_back(vec);
  }
}

Graph::~Graph(){

}

void Graph::addEdge(std::pair<int,int> edge){
  this->edges[edge.first].push_back(edge.second);
  this->edges[edge.second].push_back(edge.first);
}

void Graph::addEdges(std::vector<std::pair<int,int>> edges){
  for(std::pair<int,int> e : edges){
    addEdge(e);
  }
}

std::vector<int> Graph::geVertexEdges(int v){
  return edges[v];
}

int Graph::getVertex(){
  return this->nodes;
}
