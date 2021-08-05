#include "vf2plus.h"
#include <QDebug>

State::State(Graph * g1,Graph * g2){

  int n = g1->getVertex();
  std::map<int,int> map;
  std::vector<int> sm1(n,0), sm2(n,0), st1(n,0), st2(n,0), sn1(n,1), sn2(n,1), d1,d2;

  this->m = map;
  this->m1 = sm1;
  this->m2 = sm2;
  this->t1 = st1;
  this->t2 = st2;
  this->n1 = sn1;
  this->n2 = sn1;
  this->maximum = 0;


  int max = 0;
  std::vector<int> vec(n,0);
  for (int i=0; i<n; i++){
    int k = g1->geVertexEdges(i).size();
    vec[k]+=1;
    if (k>max)
      max = k;
  }


  std::vector<int> v1(n,0);
  for (int i=0; i<n; i++){
    int k = g1->geVertexEdges(i).size();
    v1[i] = vec[k];
    d1.push_back(k);
  }
  this->prob1 = v1;

  std::vector<int> v2(n,0);
  for (int i=0; i<n; i++){
    int k = g2->geVertexEdges(i).size();
    v2[i] = vec[k];
    d2.push_back(k);
  }
  this->prob2 = v2;

  this->degree1 = d1;
  this->degree2 = d2;

  std::vector<std::vector<int>> new1,new2;
  for (int i=0; i<=max; i++){
    std::vector<int> tmp;
    new1.push_back(tmp);
    new2.push_back(tmp);
  }
  for (int i=0; i<n; i++){
    new1[0].push_back(i);
    new2[0].push_back(i);
  }
  this->tt1 = new1;
  this->tt2 = new2;

  this->sort(this->maximum);
}

void State::sort(int k){
  std::sort(this->tt1[k].begin(),this->tt1[k].end(),
            [&](int A, int B) -> bool {

              if (this->prob1[A] == this->prob1[B]){
                if (this->degree1[A] == this->degree1[B])
                  return A<B;
                else
                  return this->degree1[A] < this->degree1[B];
              }else{
                return this->prob1[A] > this->prob1[B];
              }

            });

  std::sort(this->tt2[k].begin(),this->tt2[k].end(),
            [&](int A, int B) -> bool {
              if (this->prob2[A] == this->prob2[B]){
                if (this->degree2[A] == this->degree2[B])
                  return A<B;
                else
                  return this->degree2[A] < this->degree2[B];
              }else{
                return this->prob2[A] > this->prob2[B];
              }
            });

}




bool feasibility(Graph *g1, Graph *g2, State * state, int k1, int k2){

  std::vector<int> adj1, adj2;
  adj1 = g1->geVertexEdges(k1);
  adj2 = g2->geVertexEdges(k2);

  if (adj1.size()!=adj2.size())
    return false;

  std::map<std::pair<int,int>,int> ift1;
  std::map<std::pair<int,int>,int> ift2;

  for (auto el2: adj2){
    if (state->m2[el2] == 1){
      state->m2[el2]=2;
    }
  }

  for (auto el: adj1){
    if (state->m1[el] == 1){
      if (state->m2[state->m[el]] != 2){
          for (auto el2: adj2){
            if (state->m2[el2] == 2){
              state->m2[el2]=1;
            }
          }
        return false;
      }
    }
    else{
      std::pair<int,int> p(state->t1[el],state->degree1[el]);
      std::map<std::pair<int,int>,int>::iterator it = ift1.find(p);

      if(it != ift1.end()){
        ift1[p]+=1;
      }else{
        ift1[p]=1;
      }
    }
  }

  for (auto el2: adj2){
    if (state->m2[el2] == 2){
      state->m2[el2]=1;
    }
  }

  for (auto el: adj2){
    if (state->m2[el] != 1){
      std::pair<int,int> p(state->t2[el],state->degree2[el]);
      std::map<std::pair<int,int>,int>::iterator it = ift2.find(p);

      if(it != ift2.end()){
        ift2[p]+=1;
      }else{
        ift2[p]=1;
      }
    }
  }
  for (auto el:ift1){
    if (ift2[el.first]!=el.second)
      return false;
  }



  return true;
}

void addToState(Graph *g1, Graph *g2, State * state, int el1, int el2, int p1, int p2){
  state->m[el1] = el2;

  state->m1[el1]=1;
  state->tt1[state->maximum].erase(std::remove(state->tt1[state->maximum].begin(),state->tt1[state->maximum].end(),el1),state->tt1[state->maximum].end());

  state->m2[el2]=1;
  state->tt2[state->maximum].erase(std::remove(state->tt2[state->maximum].begin(),state->tt2[state->maximum].end(),el2),state->tt2[state->maximum].end());

  for (int v: g1->geVertexEdges(el1)){
    state->n1[v] = 0;
    if (state->m1[v]!=1){
      state->tt1[state->t1[v]].erase(std::remove(state->tt1[state->t1[v]].begin(), state->tt1[state->t1[v]].end(),v),state->tt1[state->t1[v]].end());
      state->t1[v]+=1;
      state->tt1[state->t1[v]].push_back(v);
      if (state->t1[v] > state->maximum)
        state->maximum = state->t1[v];
    }
  }
  for (int v: g2->geVertexEdges(el2)){
    state->n2[v] = 0;
    if (state->m2[v]!=1){
      state->tt2[state->t2[v]].erase(std::remove(state->tt2[state->t2[v]].begin(), state->tt2[state->t2[v]].end(),v),state->tt2[state->t2[v]].end());
      state->t2[v]+=1;
      state->tt2[state->t2[v]].push_back(v);
      if (state->t2[v] > state->maximum)
        state->maximum = state->t2[v];
    }
  }
  while (state->maximum > 0 && state->tt1[state->maximum].size() == 0)
    state->maximum-=1;
}

void removeFromState(Graph *g1, Graph *g2, State * state, int el1, int el2, int p1, int p2){

  state->m.erase(el1);

  state->m1[el1] = 0;
  state->m2[el2] = 0;

  state->tt1[state->t1[el1]].push_back(el1);
  state->tt2[state->t2[el2]].push_back(el2);

  if (state->t1[el1] > state->maximum)
    state->maximum = state->t1[el1];

  for (int v: g1->geVertexEdges(el1)){

    if (state->m1[v]!=1){
      state->tt1[state->t1[v]].erase(std::remove(state->tt1[state->t1[v]].begin(), state->tt1[state->t1[v]].end(),v),state->tt1[state->t1[v]].end());
      state->t1[v]-=1;
      state->tt1[state->t1[v]].push_back(v);
      if (state->t1[v] == 0)
        state->n1[v]=1;

    }

  }

  for (int v: g2->geVertexEdges(el2)){

    if (state->m2[v]!=1){
      state->tt2[state->t2[v]].erase(std::remove(state->tt2[state->t2[v]].begin(), state->tt2[state->t2[v]].end(),v),state->tt2[state->t2[v]].end());
      state->t2[v]-=1;
      state->tt2[state->t2[v]].push_back(v);
      if (state->t2[v] == 0)
        state->n2[v]=1;
    }

  }
  while (state->tt1[state->maximum].size() == 0)
    state->maximum-=1;

  state->sort(state->maximum);


}

bool match(Graph *g1, Graph *g2, State * state){

  if (state->m.size() == g2->getVertex()){
    return true;
  }

  state->sort(state->maximum);


  int size1=0;
  int size2=0;

  size1 = state->tt1[state->maximum].size();
  size2 = state->tt2[state->maximum].size();



  for (int i=0; i<=state->maximum;i++){
    size1 = state->tt1[state->maximum].size();
    size2 = state->tt2[state->maximum].size();
    if (size1!=size2)
      return false;
  }


  for (int i=size1-1; i>=0; i--){
    for (int j=size2-1; j>=0;j--){
      int el1 = state->tt1[state->maximum][i];
      int el2 = state->tt2[state->maximum][j];
      if (feasibility(g1,g2,state,el1,el2)){
        int maxi = state->maximum;
        addToState(g1,g2,state,el1,el2,i,j);
        bool f = match(g1,g2,state);
        if (f)
          return true;

        removeFromState(g1,g2,state,el1,el2,i,j);

      }
    }
  }


  return false;
}


std::map<int,int> vf2plus(Graph *g1, Graph *g2){
  std::map<int,int> map;
  if (g1->getVertex()!=g2->getVertex())
      return map;

  State * state = new State(g1,g2);
  bool e = match(g1,g2,state);
  if (e){
    map = state->m;
  }
  delete state;

  return map;

}
