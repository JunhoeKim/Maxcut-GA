#include "GeneticSpace.h"
#include "Utils.h"
#include "Chromosome.h"
#include "Config.h"
#include <cmath>
using namespace std;
using pChromosome = shared_ptr<Chromosome>;

//bool WeightPairComparator(const WeightPair& a, const WeightPair& b) {
//	return a->second > b->second;
//};

bool weightComparator(int i, int j) {
	return i < j;
}

bool fitnessComparator(shared_ptr<Chromosome>& a, shared_ptr<Chromosome>& b) {
	return a->fitness > b->fitness;
}

GeneticSpace::GeneticSpace(size_t _population, Graph* graph) : population(_population), graph(graph) {
	int vCount = graph->getVCount();
	for (size_t i = 0; i < population; i++) {
		Chromosome* elem = new Chromosome();
		for (int j = 0; j < vCount; j++) {
			int value = rand() % 2;
			elem->genes.emplace_back(value);
		}
		chromosomes.emplace_back(elem);
	}

}

void GeneticSpace::printElems() {
	
	cout << "Elements ---------" << endl;
	for (size_t i = 0; i < population; i++) {
		Chromosome elem = *chromosomes[i];
		cout << i << " : ";
		for (size_t j = 0; j < elem.genes.size(); j++) {
			cout << elem.genes[j] << ' ';
		}
		cout << endl;
	}
	
	cout << "Fitnesses --------" << endl;
	sort(chromosomes.begin(), chromosomes.end(), fitnessComparator);

	size_t index = 0;
	for (size_t i = 0; i < chromosomes.size(); i++) {
		cout << chromosomes[i]->fitness << endl;
	}
}

void GeneticSpace::initWeights() {
	int vCount = graph->getVCount();
	for (size_t i = 0; i < population; i++) {
		vector<int> currGenes = chromosomes[i]->genes;
		int currFitness = 0;
		for (auto it = graph->eArray.cbegin(); it != graph->eArray.cend(); ++it) {
			if (currGenes[get<0>(*it)] != currGenes[get<1>(*it)]) {
				currFitness += get<2>(*it);
			}
		}
		chromosomes[i]->fitness = currFitness;
	}
	sort(chromosomes.begin(), chromosomes.end(), fitnessComparator);
}

void GeneticSpace::updateWeights(vector<int>& replaceIndices) {
	int vCount = graph->getVCount();
	//for (auto index : replaceIndices) {
	//	vector<int>& currGenes = chromosomes[index]->genes;
	//	int currFitness = 0;
	//	for (const Edge& edge : graph->eArray) {
	//		if (currGenes[get<0>(edge)] != currGenes[get<1>(edge)]) {
	//			currFitness += get<2>(edge);
	//		}
	//	}
	//	chromosomes[index]->fitness = currFitness;
	//	graph->y++;
	//}
	sort(chromosomes.begin(), chromosomes.end(), fitnessComparator);
}


pair<shared_ptr<Chromosome>, shared_ptr<Chromosome>> GeneticSpace::select(SelectOption option) {
	if (option.selectType == SelectType::Rank) {
		return selectByRank();
	} else {
		return selectByTournament(0.7f, option.tournamentSize);
	}
}

pair<shared_ptr<Chromosome>, shared_ptr<Chromosome>> GeneticSpace::selectByRank() {
	float* scores = new float[population];
	float min_p = 0.1f;
	float max_p = 1.0f;
	float totalScore = 0;
	for (size_t i = 0; i < population; i++) {
		scores[i] = max_p + i * (min_p - max_p) / (population - 1);
		totalScore += scores[i];
	}

	int firstIndex = Utils::selectIndex(scores, totalScore);
	int secondIndex = Utils::selectIndex(scores, totalScore);
	delete[] scores;
	return make_pair(chromosomes[0], chromosomes[1]);
}

pair<shared_ptr<Chromosome>, shared_ptr<Chromosome>> GeneticSpace::selectByTournament(float cond, int K) {
	size_t candSize = (size_t)std::pow(2, K);
	set<pChromosome> candidateSet;
	while (candidateSet.size() < candSize) {
		int randIndex = rand() % chromosomes.size();
		candidateSet.insert(chromosomes[randIndex]);
	}

	pChromosome* candidates = new shared_ptr<Chromosome>[candSize];
	size_t candIndex = 0;
	for (auto it = candidateSet.cbegin(); it != candidateSet.cend(); ++it) {
		candidates[candIndex++] = (*it);
		//cout << "fitness " << candIndex << " value : " << (*it)->fitness << endl;
	}

	while (candSize > 2) {
		for (size_t i = 0; i < candSize / 2; i++) {
			float randValue = (float)(rand() % RAND_MAX);
			pChromosome first = candidates[i * 2];
			pChromosome second = candidates[i * 2 + 1];
			
			if (cond > randValue) {
				candidates[i] = first->fitness > second->fitness ? first : second;
			} else {
				candidates[i] = first->fitness > second->fitness ? second : first;
			}
			
			//if (cond > randValue) {
			//	candidates[i] = first;
			//}
			//else {
			//	candidates[i] = second;
			//}

		}
		candSize /= 2;
	}
	pChromosome first = candidates[0];
	pChromosome second = candidates[1];
	delete[] candidates;
	return make_pair(first, second);
}

Chromosome GeneticSpace::crossover(shared_ptr<Chromosome> first, shared_ptr<Chromosome> second, CrossoverOption option) {
	return crossoverByPoint(first, second, option.pointCount);
}

Chromosome GeneticSpace::crossoverByPoint(shared_ptr<Chromosome> first, shared_ptr<Chromosome> second, const size_t pointNum) {
	size_t vCount = graph->getVCount();
	vector<int> cutPoints;
	for (size_t i = 0; i < pointNum; i++) {
		int cutPoint = rand() % vCount;
		cutPoints.emplace_back(cutPoint);
	}
	sort(cutPoints.begin(), cutPoints.end());
	Chromosome crossovered((size_t)vCount);
	size_t cutIndex = 0;
	bool isFirst = true;
	for (size_t i = 0; i < vCount; i++) {
		crossovered.genes.emplace_back((isFirst ? first : second)->genes[i]);
		if (cutIndex < pointNum && i == cutPoints[cutIndex]) {
			cutIndex++;
			isFirst = !isFirst;
		}
	}
	//int currFitness = 0;
	//for (Edge& edge : graph->eArray) {
	//	if (crossovered.genes[get<0>(edge)] != crossovered.genes[get<1>(edge)]) {
	//		currFitness += get<2>(edge);
	//	}
	//}
	//graph->x++;
	//crossovered.fitness = currFitness;
	return crossovered;
}

vector<int> GeneticSpace::replace(vector<Chromosome>& newChromosomes, 
	size_t generation, size_t maxGeneration,
	ReplaceOption* replaceOption) {

	if (replaceOption->replaceType == Huristic) {
		vector<int> replaceIndices;
		replaceIndices.reserve(newChromosomes.size());
		float progressFactor = 0.2f * generation / maxGeneration * population;
		int randomizeFactor = (int)(population * 0.5f - progressFactor);
		for (size_t i = 0; i < newChromosomes.size(); i++) {
			size_t randValue = rand() % randomizeFactor;
			int replaceIndex = min(population -1, (size_t)(0.5f * population + progressFactor + randValue));
			replaceIndices.emplace_back(replaceIndex);
		}
		size_t newIndex = 0;
		for (auto index : replaceIndices) {
			*chromosomes[index] = newChromosomes[newIndex++];
		}
		return replaceIndices;
	}

	vector<int> replaceIndices;
	auto it = chromosomes.cend();
	size_t replaceIndex = chromosomes.size() - 1;
	for (size_t i = 0; i < newChromosomes.size(); i++) {
		--it;
		**it = newChromosomes[i];
		replaceIndices.emplace_back(--replaceIndex);
	}
	return replaceIndices;
}
