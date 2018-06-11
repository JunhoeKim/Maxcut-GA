#ifndef UTILS_H_
#define UTILS_H_

#include "GeneticSpace.h"
#include "Chromosome.h"
#include <cmath>
#include <ctime>
using namespace std;
using pChromosome = shared_ptr<Chromosome>;

class Utils {
  public:
  static bool isStopCondition(size_t generation, size_t maxGeneration);
  static int selectIndex(float*, float);
  static float getRandZeroToOne();
};
#endif