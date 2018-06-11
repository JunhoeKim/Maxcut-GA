#include "GeneticSpace.h"
#include "Utils.h"
#include "Chromosome.h"
#include "Config.h"
#include <cmath>
using namespace std;

float GeneticSpace::getRandZeroToOne() {
	double randValue = ((double)rand()) / RAND_MAX;
	return (float)randValue;
}

bool fitnessComparator(shared_ptr<Chromosome>& a, shared_ptr<Chromosome>& b) {
	return a->fitness > b->fitness;
}

GeneticSpace::GeneticSpace(size_t _population, Graph* graph, size_t K) : population(_population), graph(graph){
	setGeneRatio();
	size_t vCount = graph->getVCount();
	for (size_t i = 0; i < population; i++) {
		pChromosome elem = make_shared<Chromosome>(Chromosome(graph, vCount, false, geneRatio));
		chromosomes.emplace_back(elem);
	}

	for (size_t i = 0; i < K; i++) {
		tempChromosomes.emplace_back(Chromosome(graph, vCount, false, geneRatio));
	}
	sort(chromosomes.begin(), chromosomes.end(), fitnessComparator);
	optimizer = new Optimizer(graph, vCount, chromosomes, tempChromosomes);
}

void GeneticSpace::setGeneRatio() {
	float min = 0.3;
	geneRatio = min + GeneticSpace::getRandZeroToOne() * 0.4;
}

void GeneticSpace::reInitChromosomes(size_t generation) {
	setGeneRatio();
	optimizer->reInit(generation, geneRatio);
	sort(chromosomes.begin(), chromosomes.end(), fitnessComparator);
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
			float randValue = GeneticSpace::getRandZeroToOne();
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

void GeneticSpace::crossover(shared_ptr<Chromosome> first, shared_ptr<Chromosome> second, CrossoverOption option, size_t index) {
	return uniformCrossover(first, second, index);
	//return crossoverByPoint(first, second, option.pointCount);
}

void GeneticSpace::crossoverByPoint(shared_ptr<Chromosome> first, shared_ptr<Chromosome> second, const size_t pointNum) {
	size_t vCount = graph->getVCount();
	vector<size_t> cutPoints;
	for (size_t i = 0; i < pointNum; i++) {
		size_t cutPoint = rand() % vCount;
		cutPoints.emplace_back(cutPoint);
	}
	sort(cutPoints.begin(), cutPoints.end());
	Chromosome crossovered(graph, vCount);
	size_t cutIndex = 0;
	bool isFirst = true;
	for (size_t i = 0; i < vCount; i++) {
		crossovered.genes.emplace_back((isFirst ? first : second)->genes[i]);
		if (cutIndex < pointNum && i == cutPoints[cutIndex]) {
			cutIndex++;
			isFirst = !isFirst;
		}
	}
}

void GeneticSpace::uniformCrossover(pChromosome first, pChromosome second, size_t index) {
	size_t vCount = graph->getVCount();
	for (size_t i = 0; i < vCount; i++) {
		tempChromosomes[index].genes[i] = ((GeneticSpace::getRandZeroToOne() > 0.6 ? first : second)->genes[i]);
	}
}

void GeneticSpace::replace(size_t generation, size_t maxGeneration, ReplaceOption* replaceOption) {

	if (replaceOption->replaceType == ReplaceRandom) {
		vector<int> replaceIndices;
		replaceIndices.reserve(tempChromosomes.size());
		float progressFactor = 0.2f * generation / maxGeneration * population;
		int randomizeFactor = (int)(population * 0.5f - progressFactor);
		for (size_t i = 0; i < tempChromosomes.size(); i++) {
			size_t randValue = rand() % randomizeFactor;
			int replaceIndex = min(population - 1, (size_t)(0.5f * population + progressFactor + randValue));
		}
		size_t newIndex  = 0;
		for (auto index : replaceIndices) {
			*chromosomes[index] = tempChromosomes[newIndex++];
		}
	} else if (replaceOption->replaceType == Genitor) {
		size_t vCount = graph->getVCount();
		size_t population = chromosomes.size();
		for (size_t i = 0; i < tempChromosomes.size(); i++) {
			for (size_t j = 0; j < vCount; j++) {
				chromosomes[population - i - 1]->genes[j] = tempChromosomes[i].genes[j];
			}
			chromosomes[population - i - 1]->fitness = tempChromosomes[i].fitness;
		}
	} 
	else if (replaceOption->replaceType == PreSelection) {
		auto it = chromosomes.cend();
		size_t replaceIndex = chromosomes.size() - 1;
		for (size_t i = 0; i < tempChromosomes.size(); i++) {
			Chromosome chromosome = tempChromosomes[i];
			pChromosome first = chromosome.firstParent;
			pChromosome second = chromosome.secondParent;
			
			if (chromosome.fitness < first->fitness && chromosome.fitness < second->fitness) {
				--it;
				**it = tempChromosomes[i];
				(*it)->firstParent = nullptr;
				(*it)->secondParent = nullptr;
			}
			else {
				pChromosome replacedChromosome = first->fitness < second->fitness ? first : second;
				*replacedChromosome = chromosome;
				replacedChromosome->firstParent = nullptr;
				replacedChromosome->secondParent = nullptr;
			}
		}
	}
}

GeneticSpace::~GeneticSpace() {
	delete optimizer;
}
