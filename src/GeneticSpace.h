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
#include "Optimizer.hpp"

using namespace std;
using pChromosome = shared_ptr<Chromosome>;
class GeneticSpace {
public:
	GeneticSpace(size_t size, Graph* graph);
	~GeneticSpace();
	void printElems();
	void alignPopulations();
	void initFitnesses();
	void reInitChromosomes(float generationRatio);
	void setGeneRatio();
	float getAvg();
	Chromosome crossover(shared_ptr<Chromosome> first, shared_ptr<Chromosome> second, CrossoverOption option);
	pair<shared_ptr<Chromosome>, shared_ptr<Chromosome>> select(SelectOption option);
	void replace(vector<Chromosome>& newChromosomes, size_t generation, size_t maxGeneration, ReplaceOption* replaceOption);
	vector<pChromosome> chromosomes;
	pair<pChromosome, pair<bool, int>> maxInfo = make_pair(make_shared<Chromosome>(Chromosome()), make_pair(false, 0));
	Optimizer* optimizer;
private:
	size_t population;
	Graph* graph;
	pair<shared_ptr<Chromosome>, shared_ptr<Chromosome>> selectByRank();
	pair<shared_ptr<Chromosome>, shared_ptr<Chromosome>> selectByTournament(float, int);
	pair<pChromosome, pChromosome> selectByRandom();
	Chromosome crossoverByPoint(pChromosome first, pChromosome second, const size_t pointNum);
	Chromosome uniformCrossover(pChromosome first, pChromosome second);
	float geneRatio = 0.5;
};

#endif
