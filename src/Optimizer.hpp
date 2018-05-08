#ifndef OPTIMIZER_H_
#define OPTIMIZER_H_

#include "Chromosome.h"
#include <vector>
#include <memory>

using namespace std;
using pChromosome = shared_ptr<Chromosome>;

class Optimizer {
  public:
    Optimizer(size_t vCount, vector<pChromosome>& chromosomes);
    void reInit(float generationRatio, float geneRatio);
    int getMaxFitness();
    bool isReInitCondition();
  private:
    size_t vCount;
    vector<pChromosome>& chromosomes;
    pChromosome maxChromosome = make_shared<Chromosome>(Chromosome());
    bool isFindBest = false;
    size_t restartStack = 0;
    size_t iterStack = 0;
    int currMaxFit = 0;
    int tempMaxFit = 0;
    int tempSecondFit = 0;
};
#endif