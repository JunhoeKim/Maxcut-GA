#include "GeneticSpace.h"
#include "Utils.h"
bool Utils::isStopCondition(size_t generation, size_t maxGeneration) {
	return generation > maxGeneration;
}

int Utils::selectIndex(float* scores, float totalScore) {
	float currValue = (float)rand() / RAND_MAX * totalScore;
	size_t scoresLength = sizeof(scores) / sizeof(*scores);
	int index = -1;
	do {
		currValue -= scores[++index];
		scoresLength--;
	} while (currValue > 1e-6 && scoresLength > 0);
	return index;
}

bool Utils::isReInitCondition(vector<pChromosome>& chromosomes) {
	int fitness = chromosomes[0]->fitness;
	for (size_t i = 0; i < chromosomes.size() / 2; i++) {
		if (fitness != chromosomes[i]->fitness) {
			return false;
		}
	}
	return true;
}
