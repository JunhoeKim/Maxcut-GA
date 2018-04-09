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
#include "Graph.h"
#include "Chromosome.h"

using namespace std;
typedef int INDEX;
typedef int WEIGHT;
typedef shared_ptr<pair<INDEX, WEIGHT>> WeightPair;

class GeneticSpace {
public:
	GeneticSpace(size_t size, Graph* graph);
	void printElems();
	void updateScore();
	Chromosome crossover(const INDEX first, const INDEX second);
	void replace(vector<Chromosome>& newChromosomes);
	pair<INDEX, INDEX> select();

private:
	size_t size;
	Graph* graph;
	vector<Chromosome> chromosomes;
	list<WeightPair> weights;
};

#endif
