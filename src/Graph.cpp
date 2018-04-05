#include "Graph.h"

using namespace std;
Graph::Graph(int vCount, int eCount): vCount(vCount), eCount(eCount) {
  vArray = new int*[vCount];
  for (int i = 0; i < vCount; i++) {
    vArray[i] = new int[vCount]();
  } 
}

void Graph::addEdge(int first, int second, int weight) {
  vArray[first - 1][second - 1] = weight;
  vArray[second - 1][first - 1] = weight;
}

void Graph::printGraph() {
  for (int i = 0; i < vCount; i++) {
    for (int j = 0; j < vCount; j++) {
      cout << vArray[i][j] << " ";
    }
    cout << endl;
  }
}

int** Graph::getMatrix() {
  return vArray;
}

int Graph::getVCount() {
  return vCount;
}