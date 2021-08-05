#ifndef RI_H
#define RI_H

#include <vector>
#include <iostream>
#include <map>
#include "graph.h"
#include <set>
#include <utility>

struct Rank{

  Rank(int m, int i, int vis, int neig, int unv, int m_degree);
  int m;
  int i;
  int vis;
  int neig;
  int unv;
  int m_degree;

  void clear();
  int check(int sm, int si, int svis, int sneig, int sunv, int sm_degree);

};

std::pair<std::vector<std::vector<int>>, std::vector<int>> gtf(Graph * g1);
bool feasibility(Graph * g1, Graph *g2, std::map<int,int> m, std::vector<int> m1, std::vector<int> m2, int k1, int k2);
std::map<int,int> ri_gi(Graph * g1, Graph *g2);




#endif
