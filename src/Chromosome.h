#ifndef CHROMOSOME_H_
#define CHROMOSOME_H_

#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include "Config.h"
#include "Graph.h"

using namespace std;

class Chromosome {

public:
	Chromosome();
	Chromosome(Graph* graph, size_t vCount);
	Chromosome(Graph* graph, size_t vCount, bool doNormal, float geneRatio);
	vector<int> genes;
	void mutate(MutateOption* option, Graph* graph, size_t iteration);
	void searchToLocal(Graph*);
	int fitness = 0;
	friend ostream& operator<<(std::ostream& os, const Chromosome& obj);
	pair<shared_ptr<Chromosome>, shared_ptr<Chromosome>> parentIndexPair;
	shared_ptr<Chromosome> firstParent;
	shared_ptr<Chromosome> secondParent;
	void mutateByDeep();
	void doSA();

private:
	Graph * graph;
	void calFitness();
	void mutateByTypical(Graph*, MutateOption*, size_t iteration);
	void mutateByUniform(MutateOption*, size_t iteration);
	void mutateBySwap(Graph*);
	int getGain(Graph*, size_t index);
	bool isNormal = false;
};

#endif
