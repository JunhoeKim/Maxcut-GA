#include <iostream>
#include <fstream>
#include <ctime>
#include <numeric>
#include <cmath>
#include <limits>
#include "Graph.h"
#include "GeneticSpace.h"
#include "Utils.h"
#include "Config.h"

using namespace std;

int process(const Config& config);
float avg(vector<int>& fitnesses);
int max(vector<int>& fitnesses);
float stdev(vector<int>& fitnesses);

int main() {
	srand((unsigned int)time(NULL));
	//srand(0);
	vector<size_t> tournamentCounts = { 2, 3, 5 };
	vector<size_t> pointCounts = { 2, 4, 8 };
	size_t maxGeneration = 60;
	vector<size_t> populations = { 300, 700, 1000};
	vector<string> paths = { "unweighted_50.txt", "unweighted_100.txt", "unweighted_500.txt", "weighted_500.txt", "weighted_chimera_297.txt"};
	vector<float> childrenRatios = { 0.03f, 0.05f, 0.07f };
	vector<ReplaceType> replaceTypes = { Genitor, ReplaceRandom };
	Config config;
	SelectOption* selectOption = new SelectOption(Tournament, tournamentCounts[0]);
	ReplaceOption* replaceOption = new ReplaceOption(Genitor);
	MutateOption* mutateOption = new MutateOption(IntervalSwap, 0.01f);
	config.selectOption = selectOption;
	config.replaceOption = replaceOption;
	config.maxGeneration = maxGeneration;
	config.mutateOption = mutateOption;

	//for (auto tournamentCount : tournamentCounts) {
	//	for (auto pointCount : pointCounts) {
	//		for (auto population : populations) {
	//			for (auto childrenRatio : childrenRatios) {
	//				for (auto replaceType : replaceTypes) {
	//					for (auto path : paths) {
	//						config.selectOption->tournamentSize = tournamentCount;
	//						config.replaceOption->replaceType = replaceType;
	//						config.crossoverOption = new CrossoverOption(pointCount);
	//						config.population = population;
	//						config.childrenRatio = childrenRatio;
	//						config.inputFilePath = path;
	//						process(config);
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	config.selectOption = new SelectOption(Tournament, 2);
	config.crossoverOption = new CrossoverOption(4);
	config.maxGeneration = 175;
	config.population = 100;
	config.inputFilePath = "../proj1_instances/weighted_chimera_297.txt";
	config.childrenRatio = 0.1f;
	process(config);

	//ofstream ofile("result.csv", fstream::out | fstream::app);
	//ofile << "Instance,Maximum,Average,Standard Deviation" << endl;
	//for (auto path : paths) {
	//	vector<int> fitnesses;
	//	fitnesses.reserve(30);
	//	config.inputFilePath = path;
	//	for (size_t i = 0; i < 30; i++) {
	//		fitnesses.emplace_back(process(config));
	//	}
	//	ofile << path << "," << max(fitnesses) << "," << avg(fitnesses) << "," << stdev(fitnesses) << endl;
	//}

	return 0;
}

float avg(vector<int>& fitnesses) {
	size_t size = fitnesses.size();
	float avg = 0;
	for (int fitness : fitnesses) {
		avg += fitness;
	}
	return avg / size;
}

float stdev(vector<int>& fitnesses) {
	size_t size = fitnesses.size();
	float stdev = 0;
	float avgValue = avg(fitnesses);
	for (int fitness : fitnesses) {
		stdev += (avgValue - fitness) * (avgValue - fitness);
	}
	return sqrt(stdev / size);
}

int max(vector<int>& fitnesses) {
	int max = numeric_limits<int>::min();
	for (int fitness : fitnesses) {
		if (max < fitness) {
			max = fitness;
		}
	}
	return max;
}

int process(const Config& config) {
	auto start = clock();
	int vCount = 0, eCount = 0;
	int first = 0, second = 0, weight = 0;

	ifstream ifile(config.inputFilePath, ifstream::in);
	ofstream ofile("maxcut.out", fstream::out);
	ifile >> vCount >> eCount;
	Graph graph(vCount, eCount);

	while (ifile >> first >> second >> weight) {
		graph.addEdge(first, second, weight);
	}

	GeneticSpace geneticSpace(config.population, &graph);
	size_t generation = 0;
	auto debugStart = clock();
	int debugGeneration = 0;
	geneticSpace.initFitnesses();
	cout << " max: " << geneticSpace.chromosomes[0]->fitness << " mean: " << geneticSpace.getAvg() << endl;
	//ofile << "Generation,Maximum Fitness,Minimum Fitness,Average" << endl;
	//ofile << "0," << geneticSpace.chromosomes[0]->fitness 
	//	<< "," << geneticSpace.chromosomes[config.population -1]->fitness
	//	<< "," << geneticSpace.getAvg() << endl;
	size_t iterCount = 0;
	do {
		vector<Chromosome> replaceElems;
		const size_t K = (size_t)(config.population * config.childrenRatio * (1 + generation / config.maxGeneration * 2));
		replaceElems.reserve(K);
		for (size_t i = 0; i < K; i++) {
			auto selectedPair = geneticSpace.select(*config.selectOption);
			auto processed = geneticSpace.crossover(selectedPair.first, selectedPair.second, *config.crossoverOption);
			processed.mutate(config.mutateOption, &graph);
			processed.searchToLocal(&graph);
			replaceElems.emplace_back(processed);
		}
		auto end = clock();
		generation = (end - start) / CLOCKS_PER_SEC;
		debugGeneration = (end - debugStart) / CLOCKS_PER_SEC;
		if (debugGeneration >= 1) {
			debugStart = clock();
			debugGeneration = -1;
		}
		geneticSpace.replace(replaceElems, generation, config.maxGeneration, config.replaceOption);
		geneticSpace.alignPopulations();
		if (debugGeneration == -1) {
			cout << " max: " << geneticSpace.chromosomes[0]->fitness << " " << geneticSpace.chromosomes[1]->fitness << " mean: " << geneticSpace.getAvg() << endl;
		//	ofile << iterCount 
		//		<< "," << geneticSpace.chromosomes[0]->fitness
		//		<< "," << geneticSpace.chromosomes[config.population - 1]->fitness
		//		<< "," << geneticSpace.getAvg() << endl;
		}
		iterCount++;
		if (Utils::isReInitCondition(geneticSpace.chromosomes)) {
			geneticSpace.reInitChromosomes();
		}
	} while (!Utils::isStopCondition(generation, config.maxGeneration));
	cout << "iter Count : " << iterCount << endl;
	//ofile << config << endl << "max fitness : " << geneticSpace.chromosomes[0]->fitness << endl << endl;
	ofile << *(geneticSpace.chromosomes[0]);
	return geneticSpace.chromosomes[0]->fitness;

}