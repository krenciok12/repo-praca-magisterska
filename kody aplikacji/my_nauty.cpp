#include "my_nauty.h"

std::vector<int> getKeysFrommap(std::map<int,std::vector<int>> map){
  std::vector<int> vints;
  for(auto  imap: map){
    vints.push_back(imap.first);
  }
  return vints;
}

void print(std::vector<int> vec){
  std::cout << "<";
  for (int el:vec){
    std::cout<<" " << el <<" ";
  }
  std::cout << ">\n";
}

void print2(std::vector<int> vec, std::map<int,std::vector<int>> map){
  std::cout << "<";
  for (int el:vec){
    std::cout << "<("<<el<<")";
    for (int el2:map[el])
      std::cout<<" " << el2 <<" ";
    std::cout << ">";
  }
  std::cout << ">\n";
}

std::vector<std::vector<int>> canonical(Graph g){
  int n = g.getVertex();

  std::vector<int> c1;
  for (int i=0;i<n;i++){
    c1.push_back(0);
  }

  std::vector<int> vertex;
  for (int i=0;i<n;i++){
    vertex.push_back(i);
  }

  std::map<int, std::vector<int>> p1;
  p1[0] = vertex;

  std::pair<std::vector<int>,std::map<int,std::vector<int>>> pair =  color_ref2(g,c1,p1,0);
  std::vector<int> path;
  std::vector<int> color_order = getKeysFrommap(pair.second);
  sort(color_order.begin(), color_order.end());
  Nauty_node * tmp = new Nauty_node(nullptr,path,pair.first,color_order,pair.second);

  Aut_group aut;
  std::vector<Nauty_node *> nodes;
  nodes.push_back(tmp);


  while(true){

    c1 = tmp->getColors();
    p1 = tmp->getVertexByColor();

    if (p1.size() == n ){

      std::vector<int> vec;
      for (int el: tmp->getColorOrder()){
        vec.push_back(p1[el][0]);
      }
      aut.addAutomorphism(vec);

      tmp = tmp->getParent();
      if (tmp == nullptr)
        break;

      continue;
    }

    tmp->increaseLastChecked();

    if ( tmp->getLastChecked() == p1[tmp->getCheckedColor()].size()){
      tmp = tmp->getParent();
      if (tmp == nullptr)
        break;

      continue;
    }

    if (tmp->getLastChecked()!=0 && aut.isOnTheSameOrbit(p1[tmp->getCheckedColor()][0], p1[tmp->getCheckedColor()][tmp->getLastChecked()], tmp->getStart(),tmp->getStart() +  tmp->getLastChecked())){
      if ( tmp->getLastChecked() == p1[tmp->getCheckedColor()].size()-1)
        tmp = tmp->getParent();
      if (tmp == nullptr)
        break;

      continue;
    }




    int new_color = *std::max_element(c1.begin(), c1.end()) + 1;

    int ind = p1[tmp->getCheckedColor()][tmp->getLastChecked()];
    p1[new_color] = std::vector<int> {ind};
    c1[ind] = new_color;
    p1[tmp->getCheckedColor()].erase(p1[tmp->getCheckedColor()].begin() + tmp->getLastChecked());

    pair = color_ref2(g,c1,p1,new_color);


    std::vector<int> old_color_order = tmp->getColorOrder();
    std::vector<int> new_color_order;

    for (int i=0; i<tmp->getStart(); i++){
      new_color_order.push_back(old_color_order[i]);
    }
    new_color_order.push_back(new_color);
    std::set<int> set;
    for (int el: p1[tmp->getCheckedColor()]){
      set.insert(pair.first[el]);
    }
    for (int el: set){
      new_color_order.push_back(el);
    }
    set.clear();
    for (int i=tmp->getStart()+1; i<tmp->getColorOrder().size(); i++){
      if (p1[old_color_order[i]].size()==1)
        new_color_order.push_back(old_color_order[i]);
      else{
        for (int el: p1[old_color_order[i]]){
          set.insert(pair.first[el]);
        }
        for (int el: set){
          new_color_order.push_back(el);
        }
        set.clear();
      }
    }
    std::vector<int> new_path = tmp->getPath();
    new_path.push_back(ind);
    Nauty_node * tmp2 = new Nauty_node(tmp,new_path,pair.first,new_color_order,pair.second);
    tmp=tmp2;
    nodes.push_back(tmp);

  }

  for (auto el:nodes){
    delete el;
  }

  aut.createAutomorphisms();

  return aut.getAutomorphisms();
};

std::map<int,int> irAlg(Graph g1,Graph g2){

    std::map<int,int> result;

    if(g1.getVertex()!=g2.getVertex())
        return result;

    std::vector<std::vector<int>> res = canonical(g1);
    std::vector<std::vector<int>> res2 = canonical(g2);

    for (auto el1:res){
      for (auto el2:res2){
        std::map<int,int> m1;
        std::map<int,int> m2;
        bool match = true;
        for (int i = 0; i < el1.size(); ++i){
          m1[el1[i]] = i;
          m2[el2[i]] = i;
        }
        std::vector<int> numb1(el1.size(),0);
        std::vector<int> numb2(el2.size(),0);
        for (int i = 0; i < el1.size(); i++){
          for (auto e: g1.geVertexEdges(el1[i])){
            numb1[m1[e]]++;
          }
          for (auto e: g2.geVertexEdges(el2[i])){
            numb2[m2[e]]++;
            if (numb2[m2[e]] != numb1[m2[e]]){
              match = false;
              break;
            }

          }
          if (!match)
            break;
        }

        if (match){
          for (int i = 0; i < el1.size(); i++){
            result[el1[i]] = el2[i];
          }
          return result;
        }

      }
    }
    return result;
}
