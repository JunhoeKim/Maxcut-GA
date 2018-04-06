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
    ~Graph();
    int getVCount() const;
    int getECount() const;
    void addEdge(int first, int second, int weight);
    void printGraph();
    int** getMatrix() const;
};
#endif
