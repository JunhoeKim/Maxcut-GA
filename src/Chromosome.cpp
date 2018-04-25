#include "Chromosome.h"
Chromosome::Chromosome() {
}

Chromosome::Chromosome(size_t size) {
	genes.reserve(size);
}

void Chromosome::mutate(int generation, int maxGeneration, MutateOption* option, Graph* graph) {
	switch (option->mutateType) {
	case Uniform:
		mutateByUniform(option);
		break;
	case IntervalSwap:
		mutateBySwap(graph);
		break;
	case Typical:
		mutateByTypical(graph);
		break;
	}
	//if (generation / maxGeneration > 0.5f) {
	//	mutateByUniform(option);
	//}
	//else {
	//	mutateBySwap(graph);
	//}

	int currFitness = 0;
	for (Edge& edge : graph->eArray) {
		if (genes[get<0>(edge)] != genes[get<1>(edge)]) {
			currFitness += get<2>(edge);
		}
	}
	fitness = currFitness;
}

void Chromosome::mutateBySwap(Graph* graph) {
	size_t vCount = graph->getVCount();
	size_t swapSize = max((size_t)5, (size_t)(rand() % (size_t)(vCount * 0.03)));
	vector<int> first;
	vector<int> second;
	first.reserve(swapSize);
	second.reserve(swapSize);
	size_t firstIndex = rand();
	size_t secondIndex = rand();
	for (size_t i = 0; i < swapSize; i++) {
		first.emplace_back(genes[(firstIndex + i) % vCount]);
		second.emplace_back(genes[(secondIndex + i) % vCount]);
	}

	for (size_t i = 0; i < swapSize; i++) {
		genes[(firstIndex + i) % vCount] = second[i];
	}

	for (size_t i = 0; i < swapSize; i++) {
		genes[(secondIndex + i) % vCount] = first[i];
	}
}

void Chromosome::mutateByUniform(MutateOption* option) {
	for (size_t i = 0; i < genes.size(); i++) {
		float mutateRatio = (float)(rand() / RAND_MAX);
		if (mutateRatio < option->mutationRatio) {
			genes[i] = genes[i] == 1 ? 0 : 1;
		}
	}
}

void Chromosome::mutateByTypical(Graph* graph) {
	size_t mutateCount = graph->getVCount() * 0.04f;
	for (size_t i = 0; i < mutateCount; i++) {
		if (rand() / RAND_MAX > 0.7f) {
			genes[rand() % mutateCount] == 1 ? 0 : 1;
		}
	}
}

std::ostream& operator<<(std::ostream& os, const Chromosome& obj) {
	for (size_t i = 0; i < obj.genes.size(); i++) {
		if (obj.genes[i] == 0) {
			os << i + 1 << " ";
		}
	}
	return os;
}

