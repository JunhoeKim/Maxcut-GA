#ifndef CHROMOSOMES_H_
#define CHROMOSOMES_H_
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <utility>
#include "Graph.h"

using namespace std;
typedef vector<int> ELEM; 
class Chromosomes {
  public:
    Chromosomes(int size, Graph graph);
    void printElems();
    void updateScore();
    pair<ELEM, ELEM> selectElemPair();

  private:
    int size;
    int vCount;
    Graph graph;
    vector<ELEM> elements;
    vector<int> scores;
};
#endif