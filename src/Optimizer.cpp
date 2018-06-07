#include "Optimizer.hpp"
Optimizer::Optimizer(size_t vCount, vector<pChromosome>& chromosomes) : vCount(vCount), chromosomes(chromosomes) {

}

void Optimizer::reInit(float generationRatio, float geneRatio) {
	//pChromosome currMaxChromosome = chromosomes[0];
	//size_t startIndex = 1;
	//size_t population = chromosomes.size();
	//chromosomes.clear();
	//chromosomes.reserve(population);


	//if (maxChromosome->fitness < currMaxChromosome->fitness) {
	//	maxChromosome = currMaxChromosome;
	//}
	//if (currMaxFit < currMaxChromosome->fitness) {
	//	currMaxFit = currMaxChromosome->fitness;
	//	restartStack = 0;
	//}

	//size_t resetCond = isFindBest ? 2 * (1 + generationRatio * 4) : 1 * (1 + generationRatio * 4);
	//if (maxChromosome->fitness < currMaxChromosome->fitness) {
	//	maxChromosome = currMaxChromosome;
	//	isFindBest = true;
	//	restartStack = 0;
	//	chromosomes.emplace_back(currMaxChromosome);
	//}
	//else if (restartStack > resetCond) {
	//	isFindBest = false;
	//	startIndex = 0;
	//	restartStack = 0;
	//	currMaxFit = 0;
	//}
	//else {
	//	restartStack++;
	//	chromosomes.emplace_back(currMaxChromosome);
	//}

	//for (size_t i = startIndex; i < population; i++) {
	//	pChromosome elem = make_shared<Chromosome>(Chromosome(vCount, true, geneRatio));
	//	chromosomes.emplace_back(elem);
	//}
	//if (startIndex == 0) {
	//	cout << maxChromosome->fitness << endl;
	//}
	cout << "reset: " << restartStack << endl;
	size_t population = chromosomes.size();
	chromosomes.clear();
	chromosomes.reserve(population);
	size_t startIndex = 1;


	chromosomes.emplace_back(maxChromosome);

	for (size_t i = startIndex; i < population; i++) {
		pChromosome elem = make_shared<Chromosome>(Chromosome(vCount, false, geneRatio));
		chromosomes.emplace_back(elem);
	}
}

bool Optimizer::isReInitCondition(size_t iterCount, size_t generation) {
	vector<size_t> resetTimes = { 20, 60, 130, 230, 360 };
	if (maxChromosome->fitness < chromosomes[0]->fitness) {
		maxChromosome = chromosomes[0];
	}
	int convergeFitness = chromosomes[(size_t)(chromosomes.size() * 0.7)]->fitness;
	for (size_t i = 0; i < resetTimes.size(); i++) {
		if (restartStack <= i && generation >= resetTimes[i] && chromosomes[0]->fitness == convergeFitness) {
			restartStack++;
			return true;
		}
	}
	return false;

	//bool isSatisfied = true;
	//int fitness = chromosomes[0]->fitness;

	//if (maxChromosome->fitness < fitness) {
	//	maxChromosome = chromosomes[0];
	//}

	//int convergeFitness = chromosomes[(size_t)(chromosomes.size() * 0.7)]->fitness;
	//if (fitness != convergeFitness) {
	//	isSatisfied = false;
	//}

	//if (isSatisfied) {
	//	float result = 0;
	//	for (auto chromosome : chromosomes) {
	//		result += chromosome->fitness;
	//	}

	//	// For debug
	//	tempMaxFitnesses.emplace_back(make_pair(iterCount, fitness));
	//	tempAverages.emplace_back(result / chromosomes.size());
	//}

	//if (tempMaxFit == chromosomes[0]->fitness
	//	&& tempSecondFit == chromosomes[1]->fitness) {
	//	iterStack++;
	//}
	//else {
	//	iterStack = 0;
	//	tempMaxFit = chromosomes[0]->fitness;
	//	tempSecondFit = chromosomes[1]->fitness;
	//}

	//if (iterStack > 200) {
	//	isSatisfied = true;
	//	tempMaxFitnesses.emplace_back(make_pair(iterCount, fitness));
	//	float result = 0;
	//	for (auto chromosome : chromosomes) {
	//		result += chromosome->fitness;
	//	}
	//	tempAverages.emplace_back(result / chromosomes.size());
	//}

	//return isSatisfied;
}

int Optimizer::getMaxFitness() {
	return maxChromosome->fitness;
}
