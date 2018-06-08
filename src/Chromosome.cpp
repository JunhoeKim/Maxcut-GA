#include "Chromosome.h"
#include "Utils.h"
Chromosome::Chromosome() {
}

Chromosome::Chromosome(size_t vCount) {
	genes.reserve(vCount);
	gains.reserve(vCount);
	for (size_t i = 0; i < vCount; i++) {
		gains.emplace_back(0);
	}
}

Chromosome::Chromosome(size_t vCount, bool doNormal, float geneRatio) : isNormal(doNormal) {

	genes.reserve(vCount);
	gains.reserve(vCount);

	for (size_t i = 0; i < vCount; i++) {
		int value = Utils::getRandZeroToOne() > geneRatio ? 0 : 1;
		genes.emplace_back(value);
		gains.emplace_back(0);
	}

	if (doNormal && genes[0] == 1) {
		for (size_t i = 0; i < vCount; i++) {
			genes[i] = 1 - genes[i];
		}
	}
}

void Chromosome::mutate(MutateOption* option, Graph* graph, float generationRatio) {
	switch (option->mutateType) {
	case Uniform:
		mutateByUniform(option);
		break;
	case IntervalSwap:
		mutateBySwap(graph);
		break;
	case Typical:
		mutateByTypical(graph, generationRatio);
		break;
	}

	if (isNormal && genes[0] == 1) {
		for (size_t i = 0; i < graph->getVCount(); i++) {
			genes[i] = 1 - genes[i];
		}
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
	vector<int> indexVector;
	indexVector.reserve(vCount);
	for (size_t i = 0; i < vCount; i++) {
		indexVector.emplace_back(i);
	}
	random_shuffle(indexVector.begin(), indexVector.end());

	for (size_t i = 0; i < vCount; i++) {
		gains[i] = getGain(graph, i);
	}

	bool improved = true;
	while (improved) {
		while (improved) {
			improved = false;
			for (size_t i = 0; i < vCount; i++) {
				size_t currIndex = indexVector[i];
				if (gains[currIndex] > 0) {
					fitness += gains[currIndex];
					genes[currIndex] = 1 - genes[currIndex];
					gains[currIndex] = -gains[currIndex];

					for (auto pair : graph->adjacentLists[currIndex]) {
						gains[pair.first] = (genes[currIndex] == genes[pair.first]) ?
							(gains[pair.first] + 2 * pair.second) : (gains[pair.first] - 2 * pair.second);
					}
					improved = true;
				}
			}
		}

		improved = true;
		while (improved) {
			improved = false;
			for (size_t i = 0; i < vCount; i++) {
				size_t currIndex = indexVector[i];
				for (auto pair : graph->adjacentLists[currIndex]) {

					int sign = genes[currIndex] == genes[pair.first] ? 1 : -1;
					int delta = gains[currIndex] + gains[pair.first] - 2 * sign * pair.second;

					if (delta > 0) {
						improved = true;
						fitness += delta;

						genes[currIndex] = 1 - genes[currIndex];
						gains[currIndex] = - gains[currIndex];
						for (auto innerPair : graph->adjacentLists[currIndex]) {
							gains[innerPair.first] = (genes[currIndex] == genes[innerPair.first]) ?
								(gains[innerPair.first] + 2 * innerPair.second) : (gains[innerPair.first] - 2 * innerPair.second);
						}

						genes[pair.first] = 1 - genes[pair.first];
						gains[pair.first] = -gains[pair.first];
						for (auto innerPair : graph->adjacentLists[pair.first]) {
							gains[innerPair.first] = (genes[pair.first] == genes[innerPair.first]) ?
								(gains[innerPair.first] + 2 * innerPair.second) : (gains[innerPair.first] - 2 * innerPair.second);
						}
					}
				}
			}
		}
	}
}

int Chromosome::getGain(Graph* graph, size_t index) {
	//vector<pair<INDEX, WEIGHT>> adjacentList = graph->adjacentLists[index];
	int fitnessDelta = 0;
	int currValue = genes[index];
	for (auto pair: graph->adjacentLists[index]) {
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
		float mutateRatio = Utils::getRandZeroToOne();
		if (mutateRatio < option->mutationRatio) {
			genes[i] = genes[i] == 1 ? 0 : 1;
		}
	}
}

void Chromosome::mutateByTypical(Graph* graph, float generationRatio) {
	size_t vCount = graph->getVCount();
	size_t mutateCount = max((size_t)5, (size_t)(rand() % (size_t)(vCount * 0.03 + 1)));
	for (size_t i = 0; i < mutateCount; i++) {
		size_t currIndex = rand() % vCount;
		genes[currIndex] = genes[currIndex] == 1 ? 0 : 1;
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

