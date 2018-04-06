#include "GeneticSpace.h"
#include "Utils.h"
#include "Chromosome.h"
using namespace std;

bool WeightPairComparator(const WeightPair& a, const WeightPair& b) {
	return a->second > b->second;
};

GeneticSpace::GeneticSpace(size_t size, shared_ptr<Graph> graph) : size(size), graph(graph) {
	int vCount = graph->getVCount();
	for (size_t i = 0; i < size; i++) {
		Chromosome elem(vCount);
		for (int j = 0; j < vCount; j++) {
			int value = rand() % 2;
			elem.genes.emplace_back(value);
		}
		chromosomes.push_back(elem);
	}
}

void GeneticSpace::printElems() {
	cout << "Elements ---------" << endl;
	for (size_t i = 0; i < size; i++) {
		Chromosome elem = chromosomes[i];
		cout << i << " : ";
		for (size_t j = 0; j < elem.genes.size(); j++) {
			cout << elem.genes[j] << ' ';
		}
		cout << endl;
	}

	cout << "Scores --------" << endl;
	weights.sort(WeightPairComparator);

	for (auto it = weights.cbegin(); it != weights.cend(); ++it) {
		cout << (*it)->first << " : " << (*it)->second << endl;
	}
}

void GeneticSpace::updateScore() {
	int vCount = graph->getVCount();
	weights.clear();
	for (size_t i = 0; i < size; i++) {
		int currScore = 0;
		for (int j = 0; j < vCount; j++) {
			int currValue = chromosomes[i].genes[j];
			for (int k = 0; k < vCount; k++) {
				if (k == j) {
					continue;
				}
				int otherValue = chromosomes[i].genes[k];
				if (otherValue != currValue) {
					currScore += graph->getMatrix()[j][k];
				}
			}
		}
		weights.push_back(make_shared<pair<int, int>>(make_pair(i, currScore / 2)));
	}
	weights.sort(WeightPairComparator);
}

pair<INDEX, INDEX> GeneticSpace::select() {
	float* scores = new float[size];
	float min_p = 0.1f;
	float max_p = 1.0f;
	float totalScore = 0;
	for (size_t i = 0; i < size; i++) {
		scores[i] = max_p + i * (min_p - max_p) / (size - 1);
		totalScore += scores[i];
	}
	
	int firstIndex = Utils::selectIndex(scores, totalScore);
	int secondIndex = Utils::selectIndex(scores, totalScore);
	delete[] scores;
	return make_pair(firstIndex, secondIndex);
}

Chromosome GeneticSpace::crossover(const INDEX first, const INDEX second) {
	int vCount = graph->getVCount();
	int cutPoint = rand() % vCount;
	Chromosome firstElem = chromosomes[first];
	Chromosome secondElem = chromosomes[second];;
	Chromosome crossovered((size_t)vCount);
	for (int i = 0; i < vCount; i++) {
		if (i < cutPoint) {
			crossovered.genes.emplace_back(firstElem.genes[i]);
		} else {
			crossovered.genes.emplace_back(secondElem.genes[i]);
		}
	}
	return crossovered;
}

void GeneticSpace::replace(vector<Chromosome>& newChromosomes) {
	for (int i = 0; i < newChromosomes.size(); i++) {
		/*
		int replaceIndex = rand() % size;
		chromosomes[replaceIndex] = newChromosomes[i];
		*/
		auto it = weights.cend();
		for (int i = 0; i < newChromosomes.size(); i++) {
			--it;
			int replaceIndex = (*it)->first;
			chromosomes[replaceIndex] = newChromosomes[i];
		}
	}
}
