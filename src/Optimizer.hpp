#ifndef OPTIMIZER_H_
#define OPTIMIZER_H_

#include "Chromosome.h"
#include <vector>
#include <memory>

using namespace std;
using pChromosome = shared_ptr<Chromosome>;

class Optimizer {
  public:
    Optimizer(Graph* graph, size_t vCount, vector<pChromosome>& chromosomes, vector<Chromosome>& tempChromosomes);
	~Optimizer();
    void reInit(size_t generation, float geneRatio);
    int getMaxFitness();
    bool isReInitCondition(size_t iterCount, size_t generation);
	//void checkInheritence(size_t generation);
	bool deepFinding(size_t iteration, size_t generation);
	vector<pair<size_t, int>> tempMaxFitnesses;
	vector<float> tempAverages;
	pChromosome maxChromosome = make_shared<Chromosome>(Chromosome());


  private:
	Graph * graph;
    size_t vCount;
    vector<pChromosome>& chromosomes;
	vector<Chromosome>& tempChromosomes;
    bool isFindBest = false;
    size_t restartStack = 0;
    size_t iterStack = 0;
	size_t startTime = 0;
	size_t resetDuration = 60;
};
#endif