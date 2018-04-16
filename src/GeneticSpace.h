#ifndef GENETIC_SPACE_H_
#define GENETIC_SPACE_H_
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <list>
#include <memory>
#include <functional>
#include <set>
#include <algorithm>
#include "Graph.h"
#include "Chromosome.h"
#include "Config.h"

using namespace std;
class GeneticSpace {
public:
	GeneticSpace(size_t size, Graph* graph);
	void printElems();
	void updateWeights(vector<int>& replaceIndices);
	void initWeights();
	Chromosome crossover(shared_ptr<Chromosome> first, shared_ptr<Chromosome> second, CrossoverOption option);
	pair<shared_ptr<Chromosome>, shared_ptr<Chromosome>> select(SelectOption option);
	vector<int> replace(vector<Chromosome>& newChromosomes, size_t generation, size_t maxGeneration, ReplaceOption* replaceOption);
	vector<shared_ptr<Chromosome>> chromosomes;
private:
	size_t population;
	Graph* graph;
	pair<shared_ptr<Chromosome>, shared_ptr<Chromosome>> selectByRank();
	pair<shared_ptr<Chromosome>, shared_ptr<Chromosome>> selectByTournament(float, int);
	Chromosome crossoverByPoint(shared_ptr<Chromosome> first, shared_ptr<Chromosome> second, const size_t pointNum);
};

#endif
