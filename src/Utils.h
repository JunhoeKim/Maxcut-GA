#ifndef UTILS_H_
#define UTILS_H_

#include "GeneticSpace.h"

class Utils {
  public:
    static Chromosome mutate(Chromosome elem);
    static bool isStopCondition();
    static int ITER;
	static int selectIndex(float*, float);
};
#endif