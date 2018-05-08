#include "Optimizer.hpp"
Optimizer::Optimizer(size_t vCount, vector<pChromosome>& chromosomes) : vCount(vCount), chromosomes(chromosomes) {

}

void Optimizer::reInit(float generationRatio, float geneRatio) {
  pChromosome currMaxChromosome = chromosomes[0];
  size_t startIndex = 1;
  size_t population = chromosomes.size();
  chromosomes.clear();
  chromosomes.reserve(population);

  if (currMaxFit < currMaxChromosome->fitness) {
    currMaxFit = currMaxChromosome->fitness;
    restartStack = 0;
  }

	size_t resetCond = isFindBest ? 10 * (1 + generationRatio * 2) : 4 * (1 + generationRatio * 2);
	if (maxChromosome->fitness < currMaxChromosome->fitness) {
		maxChromosome = currMaxChromosome;
		isFindBest = true;
		restartStack = 0;
		chromosomes.emplace_back(currMaxChromosome);
	} else if (restartStack > resetCond){
		isFindBest = false;
		startIndex = 0;
		restartStack = 0;
    currMaxFit = 0;
    cout << "total reinit!!" << endl;
	} else {
    restartStack++;
		chromosomes.emplace_back(currMaxChromosome);
	}

	for (size_t i = startIndex; i < population; i++) {
		pChromosome elem = make_shared<Chromosome>(Chromosome(vCount, true, geneRatio));
		chromosomes.emplace_back(elem);
	}

}

bool Optimizer::isReInitCondition() {

  bool isSatisfied = true;
  int fitness = chromosomes[0]->fitness;

  if (maxChromosome->fitness < fitness) {
    maxChromosome = chromosomes[0];
  }

	for (size_t i = 0; i < chromosomes.size() / 3; i++) {
		if (fitness != chromosomes[i]->fitness) {
      isSatisfied = false;
      break;
		}
	}

  if (isSatisfied) {
    cout << "origin condition !" << endl;
  }

  if (tempMaxFit == chromosomes[0]->fitness
  && tempSecondFit == chromosomes[1]->fitness) {
    iterStack++;
  } else {
    iterStack = 0;
    tempMaxFit = chromosomes[0]->fitness;
    tempSecondFit = chromosomes[1]->fitness;
  }

  if (iterStack > 600) {
    isSatisfied = true;
    cout << "hello new condition!!" << endl;
  }

	return isSatisfied;
}

int Optimizer::getMaxFitness() {
  return maxChromosome->fitness;
}