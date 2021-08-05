#ifndef MY_NAUTY
#define MY_NAUTY

#include <bits/stdc++.h>
#include "color_ref2.h"
#include "nauty_node.h"
#include "aut_group.h"
#include <algorithm>
#include <iostream>


std::vector<int> getKeysFrommap(std::map<int,std::vector<int>> map);
void print(std::vector<int> vec);

void print2(std::vector<int> vec, std::map<int,std::vector<int>> map);

std::vector<std::vector<int>> canonical(Graph g);

std::map<int,int> irAlg(Graph g1,Graph g2);

#endif
