#ifndef UTILS_H_
#define UTILS_H_

#include "GeneticSpace.h"

class Utils {
  public:
    static bool isStopCondition(size_t generation, size_t maxGeneration);
	static int selectIndex(float*, float);
};
#endif