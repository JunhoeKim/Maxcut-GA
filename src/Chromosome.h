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
	Chromosome(size_t vCount);
	Chromosome(size_t vCount, bool doNormal, float geneRatio);
	vector<int> genes;
	vector<int> gains;
	void mutate(MutateOption* option, Graph* graph, float generationRatio);
	void searchToLocal(Graph*);
	int fitness = 0;
	friend ostream& operator<<(std::ostream& os, const Chromosome& obj);


private:
	void mutateByUniform(MutateOption*);
	void mutateBySwap(Graph*);
	void mutateByTypical(Graph*, float);
	int getGain(Graph*, size_t index);
	bool isNormal = false;
};
#endif
