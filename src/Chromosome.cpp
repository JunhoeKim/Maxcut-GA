#include "Chromosome.h"
Chromosome::Chromosome() {
}

Chromosome::Chromosome(size_t size) {
	genes.reserve(size);
}

void Chromosome::mutate(MutateOption* option, Graph* graph) {
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
	int currFitness = 0;
	for (Edge& edge : graph->eArray) {
		if (genes[get<0>(edge)] != genes[get<1>(edge)]) {
			currFitness += get<2>(edge);
		}
	}
	fitness = currFitness;

}

void Chromosome::searchToLocal(Graph* graph) {
	size_t vCount = graph->getVCount();
	bool improved = true;
	while (improved) {
		improved = false;
		for (size_t i = 0; i < vCount; i++) {
			int fitnessDelta = getFitnessDelta(graph, i);
			if (fitnessDelta > 0) {
				genes[i] = 1 - genes[i];
				fitness += fitnessDelta;
				improved = true;
			}
		}
	}
}

int Chromosome::getFitnessDelta(Graph* graph, size_t index) {
	vector<pair<INDEX, WEIGHT>> adjacentList = graph->adjacentLists[index];
	int fitnessDelta = 0;
	int currValue = genes[index];
	for (auto pair: adjacentList) {
		if (currValue == genes[pair.first]) {
			fitnessDelta += pair.second;
		} else {
			fitnessDelta -= pair.second;
		}
	}
	return fitnessDelta;
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
		size_t currIndex = (firstIndex + i) % vCount;
		int currValue = genes[currIndex];
		int nextValue = second[i];
		if (currValue != nextValue) {
			genes[currIndex] = nextValue;
		}
	}

	for (size_t i = 0; i < swapSize; i++) {
		size_t currIndex = (secondIndex + i) % vCount;
		int currValue = genes[currIndex];
		int nextValue = first[i];
		if (currValue != nextValue) {
			genes[currIndex] = nextValue;
		}
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
			size_t currIndex = rand() % mutateCount;
			genes[currIndex] = genes[currIndex] == 1 ? 0 : 1;
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

