#include "Optimizer.hpp"
Optimizer::Optimizer(Graph* graph, size_t vCount, vector<pChromosome>& chromosomes
	, vector<Chromosome>& tempChromosomes) : vCount(vCount), chromosomes(chromosomes), tempChromosomes(tempChromosomes), graph(graph) {

}
bool fitnessComparator2(shared_ptr<Chromosome>& a, shared_ptr<Chromosome>& b) {
	return a->fitness > b->fitness;
}

void Optimizer::reInit(size_t generation, float geneRatio) {
	startTime = generation;
	size_t updatePopulation = 250;
	size_t population = chromosomes.size() + updatePopulation;
	chromosomes.clear();
	chromosomes.reserve(population);
	resetDuration += 10;


	for (size_t i = 0; i < population; i++) {
		pChromosome elem = make_shared<Chromosome>(Chromosome(graph, vCount, false, geneRatio));
		chromosomes.emplace_back(elem);
	}
	for (size_t i = 0; i < (size_t)(updatePopulation * 0.07 + 1); i++) {
		tempChromosomes.emplace_back(Chromosome(graph, vCount, false, geneRatio));
	}
}

bool Optimizer::isReInitCondition(size_t iterCount, size_t generation) {

	if (maxChromosome->fitness < chromosomes[0]->fitness) {
		maxChromosome = chromosomes[0];
	}

	int convergeFitness = chromosomes[(size_t)(chromosomes.size() * 0.75)]->fitness;
	for (size_t i = 0; i < 5; i++) {
		if (restartStack <= i && chromosomes[2]->fitness == convergeFitness) {
			if (generation - startTime >= resetDuration) {
				restartStack++;
				return true;
			}
			else {
				if (deepFinding(iterCount, generation)) {
					sort(chromosomes.begin(), chromosomes.end(), fitnessComparator2);
				}
			}
		}
	}
	return false;
}

bool Optimizer::deepFinding(size_t iteration, size_t generation) {
	if (generation < 400) {
		size_t count = 0;
		bool isFind = false;
		while (count < 8) {
			for (size_t i = 1; i < chromosomes.size(); i++) {
				chromosomes[i]->mutateByDeep();
				chromosomes[i]->searchToLocal(graph);
				if (chromosomes[i]->fitness > maxChromosome->fitness) {
					isFind = true;
					break;
				}
			}

			if (isFind) {
				return true;
			}
			count++;
		}
	}
	return false;
}

int Optimizer::getMaxFitness() {
	return maxChromosome->fitness;
}

Optimizer::~Optimizer() {
	maxChromosome = nullptr;
}
