#include <iostream>
#include <fstream>
#include "Graph.h"
#include "Chromosomes.h"
#include "Utils.h"

using namespace std;
int main () {
  /*
  do {
    for (i = 0; i < k; i++) {
      Select two parent solutions;
      Crossover;
      Mutation;
    }
    Replace;
  } while (not stopping condition);
  Report the best solution;
  */

  int vCount, eCount = 0;
  int first, second, weight = 0;

  ifstream ifile("unweighted_50.txt");
  ifile >> vCount >> eCount;
  cout << "|V| : " << vCount << " |E| : " << eCount << endl;
  Graph graph = Graph(vCount, eCount);
  //graph.printGraph();
  while (ifile >> first >> second >> weight) {
    graph.addEdge(first, second, weight);
  }

  Chromosomes chromosomes = Chromosomes(100, graph);
  do {
    vector<ELEM> replaceElems;
    for (int i = 0; i < 100; i++) {
      pair<ELEM, ELEM> selectedPair = chromosomes.selectElemPair();
      ELEM crossoevered = Utils::crossover(selectedPair);
      ELEM mutated = Utils::mutate(crossoevered);
      replaceElems.push_back(mutated);
    }
    Utils::replace(replaceElems, chromosomes);
    chromosomes.updateScore();
  } while (Utils::isStopCondition());
  chromosomes.printElems();
  return 0;
}