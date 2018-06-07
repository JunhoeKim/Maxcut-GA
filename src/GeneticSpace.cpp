#include "GeneticSpace.h"
#include "Utils.h"
#include "Chromosome.h"
#include "Config.h"
#include <cmath>
using namespace std;

bool fitnessComparator(shared_ptr<Chromosome>& a, shared_ptr<Chromosome>& b) {
	return a->fitness > b->fitness;
}

GeneticSpace::GeneticSpace(size_t _population, Graph* graph) : population(_population), graph(graph){
	setGeneRatio();
	size_t vCount = graph->getVCount();
	for (size_t i = 0; i < population; i++) {
		pChromosome elem = make_shared<Chromosome>(Chromosome(vCount, false, geneRatio));
		chromosomes.emplace_back(elem);
	}
	initFitnesses();
	optimizer = new Optimizer(vCount, chromosomes);
}

void GeneticSpace::setGeneRatio() {
	float min = 0.3;
	geneRatio = min + Utils::getRandZeroToOne() * 0.4;
}

void GeneticSpace::reInitChromosomes(float generationRatio) {
	setGeneRatio();
	optimizer->reInit(generationRatio, geneRatio);
	initFitnesses();
}

float GeneticSpace::getAvg() {
	float result = 0;
	for (auto chromosome : chromosomes) {
		result += chromosome->fitness;
	}
	return result / chromosomes.size();
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

	for (size_t i = 0; i < chromosomes.size(); i++) {
		cout << chromosomes[i]->fitness << endl;
	}
}

void GeneticSpace::initFitnesses() {
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

void GeneticSpace::alignPopulations() {
	sort(chromosomes.begin(), chromosomes.end(), fitnessComparator);
}


pair<shared_ptr<Chromosome>, shared_ptr<Chromosome>> GeneticSpace::select(SelectOption option) {
	switch (option.selectType) {
	case SelectRandom:
		return selectByRandom();
	case Tournament:
		return selectByTournament(0.7f, option.tournamentSize);
	case Rank:
		return selectByRank();
	default:
		return selectByRandom();
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
	return make_pair(chromosomes[firstIndex], chromosomes[secondIndex]);
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
	}

	while (candSize > 2) {
		for (size_t i = 0; i < candSize / 2; i++) {
			float randValue = Utils::getRandZeroToOne();
			pChromosome first = candidates[i * 2];
			pChromosome second = candidates[i * 2 + 1];
			
			if (cond > randValue) {
				candidates[i] = first->fitness > second->fitness ? first : second;
			} else {
				candidates[i] = first->fitness > second->fitness ? second : first;
			}

		}
		candSize /= 2;
	}
	pChromosome first = candidates[0];
	pChromosome second = candidates[1];
	delete[] candidates;
	return make_pair(first, second);
}

pair<pChromosome, pChromosome> GeneticSpace::selectByRandom() {
	return make_pair(chromosomes[rand() % population], chromosomes[rand() % population]);
}

Chromosome GeneticSpace::crossover(shared_ptr<Chromosome> first, shared_ptr<Chromosome> second, CrossoverOption option) {
	return uniformCrossover(first, second);
	//return crossoverByPoint(first, second, option.pointCount);
}

Chromosome GeneticSpace::crossoverByPoint(shared_ptr<Chromosome> first, shared_ptr<Chromosome> second, const size_t pointNum) {
	size_t vCount = graph->getVCount();
	vector<size_t> cutPoints;
	for (size_t i = 0; i < pointNum; i++) {
		size_t cutPoint = rand() % vCount;
		cutPoints.emplace_back(cutPoint);
	}
	sort(cutPoints.begin(), cutPoints.end());
	Chromosome crossovered(vCount);
	size_t cutIndex = 0;
	bool isFirst = true;
	for (size_t i = 0; i < vCount; i++) {
		crossovered.genes.emplace_back((isFirst ? first : second)->genes[i]);
		if (cutIndex < pointNum && i == cutPoints[cutIndex]) {
			cutIndex++;
			isFirst = !isFirst;
		}
	}
	return crossovered;
}

Chromosome GeneticSpace::uniformCrossover(pChromosome first, pChromosome second) {
	size_t vCount = graph->getVCount();
	Chromosome crossovered((size_t)vCount);
	for (size_t i = 0; i < vCount; i++) {
		crossovered.genes.emplace_back((Utils::getRandZeroToOne() > 0.6 ? first : second)->genes[i]);
	}
	return crossovered;
}

void GeneticSpace::replace(vector<Chromosome>& newChromosomes, 
	size_t generation, size_t maxGeneration,
	ReplaceOption* replaceOption) {

	if (replaceOption->replaceType == ReplaceRandom) {
		vector<int> replaceIndices;
		replaceIndices.reserve(newChromosomes.size());
		float progressFactor = 0.2f * generation / maxGeneration * population;
		int randomizeFactor = (int)(population * 0.5f - progressFactor);
		for (size_t i = 0; i < newChromosomes.size(); i++) {
			size_t randValue = rand() % randomizeFactor;
			int replaceIndex = min(population - 1, (size_t)(0.5f * population + progressFactor + randValue));
			replaceIndices.emplace_back(replaceIndex);
		}
		size_t newIndex = 0;
		for (auto index : replaceIndices) {
			*chromosomes[index] = newChromosomes[newIndex++];
		}
	} else if (replaceOption->replaceType == Genitor) {
		vector<int> replaceIndices;
		auto it = chromosomes.cend();
		size_t replaceIndex = chromosomes.size() - 1;
		for (size_t i = 0; i < newChromosomes.size(); i++) {
			--it;
			**it = newChromosomes[i];
			replaceIndices.emplace_back(--replaceIndex);
		}
	}
}

GeneticSpace::~GeneticSpace() {
	delete optimizer;
}
