#include "Chromosome.h"
Chromosome::Chromosome() {
}

Chromosome::Chromosome(size_t size) {
	genes.reserve(size);
}

void Chromosome::mutate(int generation, int maxGeneration, MutateOption option, Graph* graph) {
	//float mutationRatio = (float)(rand() / RAND_MAX);
	//if (mutationRatio < 0.4) {
	//	set<int> indexSet;
	//	while (indexSet.size() < count) {
	//		int index = rand() % (int)genes.size();
	//		indexSet.insert(index);
	//	}
	//	float probability = (float)rand() / (float)RAND_MAX;
	//	float condition = (float)generation / (float)maxGeneration;

	//	for (auto currIndex : indexSet) {
	//		if (probability > condition) {
	//			if (genes[currIndex] == 1) {
	//				genes[currIndex] = 0;
	//			}
	//			else {
	//				genes[currIndex] = 1;
	//			}
	//		}
	//	}
	//}

	for (size_t i = 0; i < genes.size(); i++) {
		float mutateRatio = (float)(rand() / RAND_MAX);
		if (mutateRatio < option.mutationRatio) {
			genes[i] = genes[i] == 1 ? 0 : 1;
		}
	}

	int currFitness = 0;
	for (Edge& edge : graph->eArray) {
		if (genes[get<0>(edge)] != genes[get<1>(edge)]) {
			currFitness += get<2>(edge);
		}
	}
	graph->x++;
	fitness = currFitness;
}