#include "Chromosomes.h"
#ifndef UTILS_H_
#define UTILS_H_

class Utils {
  public:
    static ELEM crossover(pair<ELEM, ELEM> selectedPair);
    static ELEM mutate(ELEM elem);
    static void replace(vector<ELEM> elems, Chromosomes chromosomes);
    static bool isStopCondition();
    static int ITER;
};
#endif