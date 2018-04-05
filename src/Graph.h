#ifndef GRAPH_H_
#define GRAPH_H_
#include <iostream>
class Graph {
  private:
    int vCount;
    int eCount;
    int** vArray;

  public:
    Graph(int vCount, int eCount);
    int getVCount();
    int getECount();
    void addEdge(int first, int second, int weight);
    void printGraph();
    int** getMatrix();
};
#endif
