#include "Chromosome.h"

using namespace std;

Chromosome::Chromosome() {

}

Chromosome::Chromosome(size_t size) {
	genes.reserve(size);
}

void Chromosome::mutate(int generation, int maxGeneration) {
	int index = rand() % (int)genes.size();
	float probability = (float)rand() / (float)RAND_MAX;
	float condition = (float)generation / (float)maxGeneration;

	if (probability > condition) {
		if (genes[index] == 1) {
			genes[index] = 0;
		} else {
			genes[index] = 1;
		}
	}
}