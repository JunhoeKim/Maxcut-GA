#include <iostream>
#include <fstream>
#include "Graph.h"
#include "GeneticSpace.h"
#include "Utils.h"

using namespace std;
int main() {
	srand((unsigned int)time(NULL));
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

	ifstream ifile("../unweighted_50.txt", ifstream::in);
	ifile >> vCount >> eCount;
	cout << "|V| : " << vCount << " |E| : " << eCount << endl;
	auto graph = make_shared<Graph>(vCount, eCount);

	while (ifile >> first >> second >> weight) {
		graph->addEdge(first, second, weight);
	}

	GeneticSpace chromosomes(100u, graph);
	int K = 20;
	chromosomes.printElems();
	chromosomes.updateScore();
	do {
		vector<Chromosome> replaceElems;
		for (int i = 0; i < K; i++) {
			auto selectedPair = chromosomes.select();
			auto processed = chromosomes.crossover(selectedPair.first, selectedPair.second);
			processed.mutate(Utils::ITER, 100);
			replaceElems.emplace_back(processed);
		}
		cout << "replace elems size : " << replaceElems.size() << endl;
		chromosomes.replace(replaceElems);
		chromosomes.updateScore();
		cout << "population index : " << Utils::ITER << endl;
	} while (!Utils::isStopCondition());
	chromosomes.printElems();

	int x;
	cin >> x;

	return 0;
}