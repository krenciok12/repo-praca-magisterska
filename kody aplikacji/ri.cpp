#include "ri.h"
#include <QDebug>

Rank::Rank(int m, int i, int vis, int neig, int unv, int m_degree):m(m),i(i),vis(vis),neig(neig), unv(unv), m_degree(m_degree){

}

void Rank::clear(){
  this->m = -1;
  this->vis = -1;
  this->neig = -1;
  this->unv = -1;
  this->i = -1;
}

int Rank::check(int sm,int si, int svis, int sneig, int sunv, int sm_degree){
  if (this->vis>svis)
    return 0;

  if (this->vis<svis){
    this->m = sm;
    this->vis = svis;
    this->neig = sneig;
    this->unv = sunv;
    this->i = si;
    this->m_degree=sm_degree;
    return 1;
  }

  if (this->neig>sneig)
    return 0;

  if (this->neig<sneig){
    this->m = sm;
    this->vis = svis;
    this->neig = sneig;
    this->unv = sunv;
    this->i = si;
    this->m_degree=sm_degree;
    return 1;
  }

  if (this->unv>sunv)
    return 0;

  if (this->unv<sunv){
    this->m = sm;
    this->vis = svis;
    this->neig = sneig;
    this->unv = sunv;
    this->i = si;
    this->m_degree=sm_degree;
    return 1;
  }

  if (this->i>si)
    return 0;

  if (this->i<si){
    this->m = sm;
    this->vis = svis;
    this->neig = sneig;
    this->unv = sunv;
    this->i = si;
    this->m_degree=sm_degree;
    return 1;
  }

  if (this->m_degree>sm_degree)
    return 0;

  if (this->m_degree<sm_degree){
    this->m = sm;
    this->vis = svis;
    this->neig = sneig;
    this->unv = sunv;
    this->i = si;
    this->m_degree=sm_degree;
    return 1;
  }

  return 2;
}


std::pair<std::vector<std::vector<int>>, std::vector<int>> gtf(Graph * g){
  int n = g->getVertex();
  int v0 = 0;
  std::set<int> v;
  std::vector<int> v0vec;
  int cc = 0;
  for (int i=1; i<n; i++){
    if (g->geVertexEdges(i).size() > g->geVertexEdges(v0).size()){
      v0 = i;
    }
  }
  for (int i=0; i<n; i++){
    if (g->geVertexEdges(i).size() == g->geVertexEdges(v0).size()){
      v0vec.push_back(i);
      cc++;
    }
    else
      v.insert(i);
  }

  std::vector<std::set<int>> vec;
  for (int i=1; i<n; i++){
    std::set<int> s;
    vec.push_back(s);
  }

  int max = 1;

  std::vector<int> v2(n,0);
  std::vector<int> numb(n,0);


  std::vector<int> e(n,0);

  for (auto el2:v0vec){
    v2[el2]=1;
  }

  for (auto el2:v0vec){
    for (auto el:g->geVertexEdges(el2)){
      if (v2[el] != 1){
        e[el]+=1;
      }
    }
  }
  for (int i=0;i<n;i++){
    if (e[i]>0)
      vec[e[i]].insert(i);
  }


  std::vector<std::vector<int>> path;
  numb[v0] = path.size();
  path.push_back(v0vec);

  std::vector<int> parents;
  parents.push_back(-1);

  Rank r(-1,-1,-1,-1,-1,-1);

  while(!v.empty()){
    int size = path.size();
    std::vector<int> newV;

    for (auto el: vec[max]){
      int vis = 0, neig = 0, unv = 0, i = n+1;
      std::vector<int> tmp(n,0);

      for (auto el2: g->geVertexEdges(el)){
        if (v2[el2] == 1){
          vis+=1;
          if (numb[el2]<i)
            i = numb[el2];
          continue;
        }

        neig+=e[el2];

        if (e[el2] == 0){
          unv+=1;
        }
      }
      int ch = r.check(el,i,vis,neig,unv,g->geVertexEdges(el).size());
      if (ch == 1){
        newV.clear();
        newV.push_back(el);
      }
      if (ch == 2){
        newV.push_back(el);
      }
    }

    parents.push_back(r.i);

    path.push_back(newV);
    for (int el2:newV){
      numb[el2] = path.size()-1;
      vec[max].erase(el2);
      while(vec[max].size()==0)
        max-=1;
      v2[el2]=1;
      v.erase(el2);
    }
    for (int el2:newV){
      for (int el: g->geVertexEdges(el2))
      {
        if (v2[el]!=1){
          vec[e[el]].erase(el);
          e[el]+=1;
          vec[e[el]].insert(el);
          if (e[el] > max)
            max = e[el];
        }
      }
    }

    r.clear();
  }
  ;
  return std::pair<std::vector<std::vector<int>>, std::vector<int>> {path,parents};
}

bool feasibility(Graph * g1, Graph *g2, std::map<int,int> m, std::vector<int> m1, std::vector<int> m2, int k1, int k2){
  std::vector<int> adj1, adj2;
  adj1 = g1->geVertexEdges(k1);
  adj2 = g2->geVertexEdges(k2);

  if (adj1.size()!=adj2.size())
    return false;

  for (auto el: adj2){
    if (m2[el] == 1)
      m2[el] = 2;
  }

  for (auto el: adj1){
    if (m1[el] == 1){
      if (m2[m[el]] != 2){
        return false;
      }
    }
  }

  return true;
}


std::map<int,int> ri_gi(Graph * g1, Graph *g2){

  std::pair<std::vector<std::vector<int>>, std::vector<int>> pair1 = gtf(g1);
  std::pair<std::vector<std::vector<int>>, std::vector<int>> pair2 = gtf(g2);
  std::map<int,int> m;
  if (pair1.first.size() != pair2.first.size())
    return m;

  std::map<int,std::vector<int>> tab1;
  for (int i=0; i <  pair1.first.size(); i++){
    int s1 = pair1.first[i].size(), s2 = pair2.first[i].size();
    if (s1 != s2)
      return m;

    std::map<int,std::vector<int>>::iterator it = tab1.find(s1);

    if (it!=tab1.end()){
      tab1[s1].push_back(i);
    }else{
      std::vector<int> tmp;
      tmp.push_back(i);
      tab1[s1] = tmp;
    }
  }

  std::vector<std::pair<int,int>> path;
  int n = g1->getVertex();
  std::vector<int> m1(n,0),m2(n,0);
  std::vector<std::vector<int>> path1, path2;

  for (auto el: tab1){

    for (auto el2: el.second){
      path1.push_back(pair1.first[el2]);
      path2.push_back(pair2.first[el2]);
    }
  }

  int i=0;

  std::vector<int> add1, add2, last;
  std::vector<int> tmp1 = path1[i], tmp2 = path2[i];
  int k1 = tmp1.size()-1, k2 = tmp2.size()-1;

  while (true){
    if (m.size() == n)
      return m;
    if (k1 < 0){
      i++;
      tmp1 = path1[i];
      tmp2 = path2[i];
      k1 = tmp1.size()-1;
      k2 = tmp2.size()-1;
    }
    if (k2>=0){
      if (feasibility(g1,g2,m,m1,m2,tmp1[k1],tmp2[k2])){
        m[tmp1[k1]] = tmp2[k2];
        m1[tmp1[k1]] = 1;
        m2[tmp2[k2]] = 1;
        add1.push_back(tmp1[k1]);
        add2.push_back(tmp2[k2]);
        tmp1.erase(tmp1.begin() + k1);
        tmp2.erase(tmp2.begin() + k2);
        last.push_back(k2);
        k1-=1;
        k2 = k1;
      }
      else{
        k2--;
      }
    }
    else{


      if (last.empty())
        return m;

      m.erase(add1[add1.size()-1]);
      m1[add1[add1.size()-1]] = 0;
      m2[add2[add2.size()-1]] = 0;
      if (tmp1.size() == path1[i].size()){
        tmp1.clear();
        tmp2.clear();
        i--;
      }
      tmp1.push_back(add1[add1.size()-1]);
      tmp2.push_back(add2[add2.size()-1]);
      add1.erase(add1.begin() + add1.size()-1);
      add2.erase(add2.begin() + add2.size()-1);
      k1 = tmp1.size()-1;
      k2 = last[last.size()-1] - 1;
      last.erase(last.begin() + last.size()-1);

    }
  }

  return m;

}
