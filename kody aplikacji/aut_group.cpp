#include "aut_group.h"


DisjointSet::DisjointSet(){

}

DisjointSet::~DisjointSet(){
  this->parent.clear();
}

void DisjointSet::makeSet(std::vector<int> set){
std::unordered_map<int, int> par;
 for (int i : set){
   par[i] = i;
 }
 this->parent = par;

}

int DisjointSet::find(int k) {
 if (parent[k] == k)
    return k;
 return find(parent[k]);
}

void DisjointSet::un(int m, int n) {
 int x = find(m);
 int y = find(n);
 parent[x] = y;
}

NodeAut::NodeAut(){
  this->neig = std::set<int>{};
  this->ed = std::map<int,int>{};
}

NodeAut::~NodeAut(){

}

void NodeAut::addNeighbour(int n, int p){
  this->neig.insert(n);
  this->ed[n] = p;
}

std::set<int> NodeAut::getNeighbours(){
  return this->neig;
}

int NodeAut::getEd(int n){
  return this->ed[n];
}

std::map<int,int> NodeAut::getEds(){
  return this->ed;
}

GraphAut::GraphAut(){

}

GraphAut::~GraphAut(){

}

void GraphAut::makeGraph(std::vector<int> vec){
  for (int i:vec){
    NodeAut node;
    nodes.push_back(node);
  }
}

void GraphAut::addToNode(int n, int m, int p){
  nodes[n].addNeighbour(m,p);
  nodes[m].addNeighbour(n,p);
}

std::vector<std::pair<int,int>> GraphAut::getPath(int n, int m){
  std::set<int> deceived;
  std::queue<std::pair<int,std::vector<std::pair<int,int>>>> q;
  q.push(std::pair<int,std::vector<std::pair<int,int>>>{n,std::vector<std::pair<int,int>>{}});
  deceived.insert(n);
  while (!q.empty()){
    std::pair<int,std::vector<std::pair<int,int>>> r = q.front();
    q.pop();
    auto set = this->nodes[r.first].getNeighbours();
    auto it = set.find(m);
    if (it != set.end()) {
       r.second.push_back(std::pair<int,int>{this->nodes[r.first].getEd(m), m});
       return r.second;
    }
    for (int el: set){
      it = deceived.find(el);
      if (it == deceived.end()){
        deceived.insert(el);
        std::vector<std::pair<int,int>> tmp = r.second;
        tmp.push_back(std::pair<int,int>{this->nodes[r.first].getEd(el),el});
        q.push(std::pair<int,std::vector<std::pair<int,int>>>{el,tmp});
      }
    }
  }
  return std::vector<std::pair<int,int>>{};
}

void GraphAut::printNeigbours(){
  for (auto el2: this->nodes){
    std::cout<<"nodes "<<" < ";
    for (auto el:el2.getNeighbours()){
      std::cout <<" "<<el<<" ";
    }
    std::cout <<">\n";
  }
}

Aut_group::Aut_group(){
  std::vector<std::vector<int>> vec;
  this->automorphisms = vec ;
  DisjointSet *ds = new DisjointSet();
  this->ds = ds;
  GraphAut * ga = new GraphAut();
  this->ga = ga;

}

Aut_group::~Aut_group(){
  delete this->ds;
  delete this->ga;
}

void Aut_group::addAutomorphism(std::vector<int> aut){

  if (this->automorphisms.size() == 0){
    this->ds->makeSet(aut);
    this->ga->makeGraph(aut);
    for (int i=0; i<aut.size(); i++){
      this->byPos[aut[i]] = i;
    }
  }
  else{
    std::vector<int> tmp = this->automorphisms[0];
    for (int i=0; i<aut.size();i++){
      if (this->ds->find(tmp[i]) != this->ds->find(aut[i])){
        this->ds->un(tmp[i],aut[i]);

      }
      this->ga->addToNode(tmp[i],aut[i],this->automorphisms.size());
    }
  }


  this->automorphisms.push_back(aut);

}

std::vector<std::vector<int>> Aut_group::getAutomorphisms(){
  return this->automorphisms;
}

bool Aut_group::isOnTheSameOrbit(int n, int m, int p1, int p2){
  if (this->ds->find(n) != this->ds->find(m))
    return false;

  std::vector<std::pair<int,int>> vec = this->ga->getPath(n,m);

  std::vector<int> tmp = this->automorphisms[0];
  for (auto el: vec){

    std::map<int,int> map;
    if (el.second == this->automorphisms[el.first][this->byPos[tmp[0]]]){
      for (int i=0; i<tmp.size(); i++){
        map[this->automorphisms[0][i]] = this->automorphisms[el.first][i];
      }
    }
    else{
      for (int i=0; i<tmp.size(); i++){
        map[this->automorphisms[el.first][i]] = this->automorphisms[0][i];
      }
    }
    for (int i=0; i<tmp.size(); i++){
      tmp[i] = map[tmp[i]];
    }
  }
  std::map<int,int> map;
  for (int i=0; i<tmp.size(); i++){
    map[this->automorphisms[0][i]]  = tmp[i];
  }

  int s = this->automorphisms.size();
  for (int i=0; i<s; i++){
    if (this->automorphisms[i][p1] == n){
      for (int j = 0; j< this->automorphisms[i].size(); j++){
        tmp[j] = map[this->automorphisms[i][j]];
      }

      this->automorphisms.push_back(tmp);
    }
  }


  return true;

}

void Aut_group::createAutomorphisms(){



}
