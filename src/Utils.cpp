#include "Chromosomes.h"
#include "Utils.h"

int Utils::ITER = 0;
ELEM Utils::crossover(pair<ELEM, ELEM> selectedPair) {
  return selectedPair.first;
}

ELEM Utils::mutate(ELEM elem) {
  return elem;
}

void Utils::replace(vector<ELEM> elems, Chromosomes chromosomes) {

}

bool Utils::isStopCondition() {
  return ITER++ > 100 ? true : false;
}
