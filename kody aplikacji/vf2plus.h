#ifndef VF2_PLUS
#define VF2_PLUS

#include <vector>
#include <iostream>
#include <map>
#include "graph.h"
#include <set>
#include <algorithm>
#include <utility>

struct State {

  State(Graph * g1,Graph * g2);
  std::map<int,int> m;
  std::vector<int> m1;
  std::vector<int> m2;
  std::vector<int> t1;
  std::vector<int> t2;
  std::vector<int> n1;
  std::vector<int> n2;
  std::vector<int> prob1;
  std::vector<int> prob2;
  std::vector<int> degree1;
  std::vector<int> degree2;
  std::vector<std::vector<int>> tt1;
  std::vector<std::vector<int>> tt2;

  int maximum;

  void sort(int k);


  void print(){
    std::cout << "1 <";
    for (int i=0; i<this->m1.size(); i++){
      if (this->m1[i]>0)
        std::cout <<" "<<i<<" ";
    }
    std::cout << ">";
    std::cout << "<";
    for (int i=0; i<this->m1.size(); i++){
      if (this->t1[i]>0 && this->m1[i]==0)
        std::cout <<" "<<i<<" ";
    }
    std::cout << ">";
    std::cout << "<";
    for (int i=0; i<this->m1.size(); i++){
      if (this->n1[i]>0)
        std::cout <<" "<<i<<" ";
    }
    std::cout << ">\n";

    std::cout << "2 <";
    for (int i=0; i<this->m1.size(); i++){
      if (this->m2[i]>0)
        std::cout <<" "<<i<<" ";
    }
    std::cout << ">";
    std::cout << "<";
    for (int i=0; i<this->m1.size(); i++){
      if (this->t2[i]>0 && this->m2[i]==0)
        std::cout <<" "<<i<<" ";
    }
    std::cout << ">";
    std::cout << "<";
    for (int i=0; i<this->m2.size(); i++){
      if (this->n2[i]>0)
        std::cout <<" "<<i<<" ";
    }
    std::cout << ">\n";
  }

};

bool match(Graph *g1, Graph *g2, State * state);
std::map<int,int> vf2plus(Graph *g1, Graph *g2);

#endif
