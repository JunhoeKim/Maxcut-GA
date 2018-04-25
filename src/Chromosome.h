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
	Chromosome(size_t size);
	vector<int> genes;
	void mutate(MutateOption* option, Graph* graph);
	void searchToLocal(Graph*);
	int fitness = 0;
	friend ostream& operator<<(std::ostream& os, const Chromosome& obj);


private:
	void mutateByUniform(MutateOption*);
	void mutateBySwap(Graph*);
	void mutateByTypical(Graph*);
	int getFitnessDelta(Graph*, size_t index);
};
#endif
