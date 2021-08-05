#ifndef AUT_GROUP
#define AUT_GROUP

#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <queue>

class DisjointSet {

   public:
     DisjointSet();
     ~DisjointSet();
     void makeSet(std::vector<int> set);
     int find(int k);
     void un(int m, int n);
   private:
     std::unordered_map<int, int> parent;
};

class NodeAut{


public:

  NodeAut();
  ~NodeAut();

  void addNeighbour(int n, int p);
  std::set<int> getNeighbours();
  int getEd(int n);
  std::map<int,int> getEds();

private:
  std::set<int> neig;
  std::map<int,int> ed;

};

class GraphAut{
  std::vector<NodeAut> nodes;
  public:

  GraphAut();
  ~GraphAut();
  void makeGraph(std::vector<int> vec);
  void addToNode(int n, int m, int p);
  std::vector<std::pair<int,int>> getPath(int n, int m);
  void printNeigbours();
};



class Aut_group{
public:
  Aut_group();
  ~Aut_group();

  void addAutomorphism(std::vector<int> aut);
  std::vector<std::vector<int>> getAutomorphisms();
  void createAutomorphisms();
  bool isOnTheSameOrbit(int n, int m, int p1, int p2);

private:
  std::vector<std::vector<int>> automorphisms;
  DisjointSet * ds;
  GraphAut * ga;
  std::map<int,int> byPos;
};

#endif
