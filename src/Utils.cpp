#include "GeneticSpace.h"
#include "Utils.h"

int Utils::ITER = 0;

Chromosome Utils::mutate(Chromosome elem) {
	return elem;
}

bool Utils::isStopCondition() {
	return ITER++ > 100 ? true : false;
}

int Utils::selectIndex(float* scores, float totalScore) {
	float currValue = (float)rand() / RAND_MAX * totalScore;
	int index = -1;
	do {
		currValue -= scores[++index];
	} while (currValue > 0);
	//cout << "select index : " << index << " score : " << scores[index] << endl;
	return index;
}