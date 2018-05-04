#ifndef GRAPH_H_
#define GRAPH_H_
#include <iostream>
#include <tuple>
#include <vector>
#include <utility>
typedef int INDEX;
typedef int WEIGHT;
using namespace std;
typedef tuple<INDEX, INDEX, WEIGHT> Edge;
class Graph {
  private:
    size_t vCount;
    size_t eCount;

  public:
    Graph(size_t vCount, size_t eCount);
    ~Graph();
    size_t getVCount() const;
    size_t getECount() const;
    void addEdge(int first, int second, int weight);
	vector<Edge> eArray;
    vector<vector<pair<INDEX, WEIGHT> > > adjacentLists;
};
#endif
