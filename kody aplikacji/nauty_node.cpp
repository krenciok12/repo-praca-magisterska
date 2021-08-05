#include "nauty_node.h"


Nauty_node::Nauty_node(Nauty_node  * par,std::vector<int> path,std::vector<int> colors,std::vector<int> color_order,std::map<int,std::vector<int>> vertexByColor):
par(par),path(path),colors(colors), color_order(color_order),vertexByColor(vertexByColor){
  int n = this->color_order.size();

  for (int i=0; i<n;i++){
    if (this->vertexByColor[this->color_order[i]].size()>1){
      this->start = i;
      this->checkedColor = this->color_order[i];
      this->lastChecked = -1;
      return;
    }
  }

  this->start = n+1;
  this->checkedColor = n+1;
  this->lastChecked = n+1;

}

Nauty_node::~Nauty_node(){

}

Nauty_node * Nauty_node::getParent(){
  return this->par;
}

std::vector<int> Nauty_node::getPath(){
  return this->path;
}

std::vector<int> Nauty_node::getColors(){
  return this->colors;
}

std::vector<int> Nauty_node::getColorOrder(){
  return this->color_order;
}

std::map<int,std::vector<int>> Nauty_node::getVertexByColor(){
  return this->vertexByColor;
}

int Nauty_node::getCheckedColor(){
  return this->checkedColor;
}

int Nauty_node::getLastChecked(){
  return this->lastChecked;
}

int Nauty_node::getStart(){
  return this->start;
}

void Nauty_node::setStart(int s){
  this->start=s;
}

void Nauty_node::increaseLastChecked(){
  this->lastChecked+=1;
}
