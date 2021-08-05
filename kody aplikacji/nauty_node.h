#ifndef NAUTY_NODE
#define NAUTY_NODE

#include <vector>
#include <map>

class Nauty_node
{
public:
  Nauty_node(Nauty_node  * par,std::vector<int> path,std::vector<int> colors,std::vector<int> color_order,std::map<int,std::vector<int>> vertexByColor);
  ~Nauty_node();

  Nauty_node *  getParent();
  std::vector<int> getPath();
  std::vector<int> getColors();
  std::vector<int> getColorOrder();
  std::map<int,std::vector<int>> getVertexByColor();
  int getCheckedColor();
  int getLastChecked();
  int getStart();
  void increaseLastChecked();
  void setStart(int s);

private:
  Nauty_node  * par;
  std::vector<int> path;
  std::vector<int> colors;
  std::vector<int> color_order;
  std::map<int,std::vector<int>> vertexByColor;
  int checkedColor;
  int lastChecked;
  int start;


};

#endif
