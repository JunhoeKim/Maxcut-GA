#ifndef UTILS_H_
#define UTILS_H_

#include "GeneticSpace.h"
using pChromosome = shared_ptr<Chromosome>;

class Utils {
  public:
    static bool isStopCondition(size_t generation, size_t maxGeneration);
	static bool isReInitCondition(vector<pChromosome>& chromosomes);
	static int selectIndex(float*, float);
};
#endif