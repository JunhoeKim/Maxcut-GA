#include "Chromosome.h"
#include "Utils.h"
Chromosome::Chromosome() {
}

Chromosome::Chromosome(Graph* graph, size_t vCount) : graph(graph) {
	genes.reserve(vCount);
}

Chromosome::Chromosome(Graph* graph, size_t vCount, bool doNormal, float geneRatio) : isNormal(doNormal), graph(graph) {

	genes.reserve(vCount);

	for (size_t i = 0; i < vCount; i++) {
		int value = Utils::getRandZeroToOne() > geneRatio ? 0 : 1;
		genes.emplace_back(value);
	}

	if (doNormal && genes[0] == 1) {
		for (size_t i = 0; i < vCount; i++) {
			genes[i] = 1 - genes[i];
		}
	}
	calFitness();
}

void Chromosome::calFitness() {

	int currFitness = 0;
	for (Edge& edge : graph->eArray) {
		if (genes[get<0>(edge)] != genes[get<1>(edge)]) {
			currFitness += get<2>(edge);
		}
	}

	fitness = currFitness;
}

void Chromosome::mutate(MutateOption* option, Graph* graph, size_t iteration) {
	switch (option->mutateType) {

	case Uniform:
		mutateByUniform(option, iteration);
		break;

	case IntervalSwap:
		mutateBySwap(graph);
		break;

	case Typical:
		mutateByTypical(graph, option, iteration);
		break;
	}

	if (isNormal && genes[0] == 1) {
		for (size_t i = 0; i < graph->getVCount(); i++) {
			genes[i] = 1 - genes[i];
		}
	}
}

void Chromosome::searchToLocal(Graph* graph) {
	calFitness();
	size_t vCount = graph->getVCount();
	vector<int> indexVector;
	indexVector.reserve(vCount);
	for (size_t i = 0; i < vCount; i++) {
		indexVector.emplace_back(i);
	}
	random_shuffle(indexVector.begin(), indexVector.end());
	vector<int> gains;
	gains.reserve(vCount);
	for (size_t i = 0; i < vCount; i++) {
		gains.emplace_back(getGain(graph, i));
	}

	bool improved = true;
	// 1-bit
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
	// 2-bit
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
					gains[currIndex] = -gains[currIndex];
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

int Chromosome::getGain(Graph* graph, size_t index) {
	//vector<pair<INDEX, WEIGHT>> adjacentList = graph->adjacentLists[index];
	int fitnessDelta = 0;
	int currValue = genes[index];
	for (auto pair : graph->adjacentLists[index]) {
		if (currValue == genes[pair.first]) {
			fitnessDelta += pair.second;
		}
		else {
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

void Chromosome::mutateByUniform(MutateOption* option, size_t iteration) {
	float mutationRatio = 0;
	switch (iteration % 4) {
	case 0:
		mutationRatio = 0.004;
		break;

	case 1:
		mutationRatio = 0.017;
		break;

	case 2:
		mutationRatio = 0.027;
		break;

	case 3:
		mutationRatio = 0.04;
		break;
	}
	for (size_t i = 0; i < genes.size(); i++) {
		double randValue = ((double)rand()) / RAND_MAX;
		float mutateRatio = randValue;
		if (mutateRatio < mutationRatio) {
			genes[i] = genes[i] == 1 ? 0 : 1;
		}
	}
}

void Chromosome::mutateByDeep() {
	float mutationRatio = 0;


	switch (rand() % 4) {
	case 0:
		mutationRatio = 0.005;
		break;

	case 1:
		mutationRatio = 0.02;
		break;

	case 2:
		mutationRatio = 0.05;
		break;

	case 3:
		mutationRatio = 0.1;
		break;
	}

	size_t vCount = graph->getVCount();
	size_t mutateCount = max((size_t)3, (size_t)(rand() % (size_t)(vCount * mutationRatio)));
	//cout << mutateCount << endl;
	for (size_t i = 0; i < mutateCount; i++) {
		size_t currIndex = rand() % vCount;
		genes[currIndex] = genes[currIndex] == 1 ? 0 : 1;
	}
}

void Chromosome::mutateByTypical(Graph* graph, MutateOption* option, size_t iteration) {
	float mutationRatio = 0;

	switch (iteration % 4) {
	case 0:
		mutationRatio = 0.008;
		break;

	case 1:
		mutationRatio = 0.035;
		break;

	case 2:
		mutationRatio = 0.055;
		break;

	case 3:
		mutationRatio = 0.08;
		break;
	}

	size_t vCount = graph->getVCount();
	size_t mutateCount = max((size_t)3, (size_t)(rand() % (size_t)(vCount * mutationRatio)));
	for (size_t i = 0; i < mutateCount; i++) {
		size_t currIndex = rand() % vCount;
		genes[currIndex] = genes[currIndex] == 1 ? 0 : 1;
	}
}

void Chromosome::doSA() {
	auto start = clock();
	cout << "init fitness0 : " << fitness << endl;
	size_t vCount = graph->getVCount();
	vector<int> tempGenes;
	tempGenes.reserve(vCount);

	for (size_t i = 0; i < vCount; i++) {
		tempGenes.emplace_back(rand() % 2);
	}
	float temperature = 1.0;
	float maxTemperature = temperature;
	int tempFitness = 0;
	searchToLocal(graph);
	cout << "init fitness1 : " << fitness << endl;

	while (temperature > 0.0001) {
		temperature *= 0.9999;
		size_t vCount = graph->getVCount();

		size_t mutateCount = max((size_t)3, (size_t)(rand() % (size_t)(vCount * 0.1)));
		for (size_t i = 0; i < mutateCount; i++) {
			size_t currIndex = rand() % vCount;
			genes[currIndex] = genes[currIndex] == 1 ? 0 : 1;
		}

		calFitness();
		searchToLocal(graph);
		double randRatio = exp((fitness - tempFitness) / temperature);

		bool but = Utils::getRandZeroToOne() < randRatio;
		if (tempFitness < fitness || but) {
			for (size_t i = 0; i < vCount; i++) {
				genes[i] = tempGenes[i];
			}
			//if (but && tempFitness >= fitness) {
			//	cout << "---------------------------------" << endl;
			//	cout << "temp Fitness : " << tempFitness << endl;
			//	cout << (float)((fitness - tempFitness) / temperature) << endl;
			//	cout << Utils::getRandZeroToOne() << endl;
			//	cout << randRatio << endl;
			//	cout << count << endl;
			//	count++;
			//}
			tempFitness = fitness;
		}
	}
	auto end = clock();
	size_t elapsedTime = (end - start) / CLOCKS_PER_SEC;
	cout << "elapsed time : " + elapsedTime << endl;
	cout << "init fitness2 : " << tempFitness << endl;
}

std::ostream& operator<<(std::ostream& os, const Chromosome& obj) {
	for (size_t i = 0; i < obj.genes.size(); i++) {
		if (obj.genes[i] == 0) {
			os << i + 1 << " ";
		}
	}
	return os;
}


