#include "color_ref2.h"

std::pair<std::vector<int>,std::map<int,std::vector<int>>> color_ref2(Graph g, std::vector<int> c, std::map<int,std::vector<int>> p, int max){

  std::vector<std::set<int>> nC, nA;

  std::vector<int> maxcdeg, mincdeg, cdeg, colour;
  for (int i=0; i<g.getVertex(); i++){
    nC.push_back(std::set<int> {});
    nA.push_back(std::set<int> {});
    maxcdeg.push_back(0);
    mincdeg.push_back(0);
  }
  for (int i=0; i<g.getVertex(); i++){
    nC[c[i]].insert(i);
    cdeg.push_back(0);
    colour.push_back(c[i]);
  }

  int k = max;

  std::set<int> sref;
  sref.insert(max);
  std::set<int> col_adj;

  while(!sref.empty()){

    int r = *sref.begin();
    sref.erase(r);

    for (auto v: nC[r]){
      for (auto w: g.geVertexEdges(v)){
        cdeg[w]+=1;
        if (cdeg[w]==1)
          nA[colour[w]].insert(w);
        col_adj.insert(colour[w]);
        if (cdeg[w]>maxcdeg[colour[w]])
          maxcdeg[colour[w]] = cdeg[w];
      }
    }

    for (auto cc: col_adj){
      if (nC[cc].size() != nA[cc].size())
        mincdeg[cc] = 0;
      else{
        mincdeg[cc] = maxcdeg[cc];
        for (auto v: nA[cc]){
          if (cdeg[v] < mincdeg[cc])
            mincdeg[cc] = cdeg[v];
        }
      }
    }

    std::vector<int> col_split;

    for (auto cc: col_adj){
      if (mincdeg[cc]<maxcdeg[cc])
        col_split.push_back(cc);
    }
    for (auto s: col_split){
      std::vector<int> numcdeg;

      for (int i=0;i<=maxcdeg[s];i++){
        numcdeg.push_back(0);
      }
      numcdeg[0] = nC[s].size() - nA[s].size();

      for (auto v: nA[s]){
        numcdeg[cdeg[v]]+=1;
      }

      int b = 0;

      for (int i=1;i<=maxcdeg[s];i++){
        if (numcdeg[i]>numcdeg[b])
          b=i;
      }

      int instack = 0;
      if (sref.find(s)!=sref.end())
        instack = 1;

      std::vector<int> f(g.getVertex()+1,0);
      for (int i=0; i<=maxcdeg[s]; i++){
        if (numcdeg[i]>=1){
          if (i == mincdeg[s]){
            f[i] = s;
            if (instack == 0 && b!=i)
              sref.insert(f[i]);
          }else{
            k+=1;
            f[i]=k;
            if (instack == 1 || i!=b)
              sref.insert(f[i]);
          }
        }
      }
      for (auto v: nA[s]){
        if (f[cdeg[v]]!=s){
          nC[s].erase(v);
          nC[f[cdeg[v]]].insert(v);
          colour[v] = f[cdeg[v]];
        }
      }


    }
    for (auto cc: col_adj){
      for (auto v: nA[cc]){
        cdeg[v] = 0;
      }
      maxcdeg[cc] = 0;
      nA[cc].clear();
    }
    col_adj.clear();



  }
  std::map<int,std::vector<int>> p2;
  for (int i=0; i<g.getVertex(); i++){
    if (nC[i].size()>0){
      p2[i] = std::vector<int> {};
      for (auto v: nC[i])
        p2[i].push_back(v);
    }
  }

  return std::pair<std::vector<int>,std::map<int,std::vector<int>>> {colour,p2};
}
