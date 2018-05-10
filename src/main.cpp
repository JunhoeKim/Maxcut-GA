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

int process(const Config& config, string name);
float avg(vector<int>& fitnesses);
int max(vector<int>& fitnesses);
float stdev(vector<int>& fitnesses);

int main() {
	srand((unsigned int)time(NULL));
	vector<size_t> tournamentCounts = { 2, 3, 5 };
	vector<size_t> pointCounts = { 2, 4, 8 };
	size_t maxGeneration = 60;
	vector<size_t> populations = { 300, 700, 1000};
	vector<string> paths = { "chimera_946.txt", "planar_800.txt", "cubic_1000.txt", "random_1000.txt", "toroidal_800.txt", "random_500.txt"};
	vector<string> names = { "chimera_", "planar_", "cubic_", "random_1000_", "toroidal_", "random_500_"};
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
	config.selectOption = new SelectOption(Tournament, 2);
	config.crossoverOption = new CrossoverOption(3);
	config.maxGeneration = 175;
	config.population = 300;
	config.inputFilePath = "../proj2_instances/cubic_1000.txt";
	config.childrenRatio = 0.07f;
	//process(config, "cubic_");
	for (size_t i = 1; i < paths.size(); i++) {
		config.inputFilePath = "../proj2_instances/" + paths[i];
		for (size_t j = 0; j < 10; j++) {
			process(config, names[i]);
		}
	}

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

int process(const Config& config, string name) {

	auto start = clock();
	int vCount = 0, eCount = 0;
	int first = 0, second = 0, weight = 0;

	ifstream ifile(config.inputFilePath, ifstream::in);
	ofstream ofile("./results/" + name + "result.csv", fstream::out | fstream::app);
	ifile >> vCount >> eCount;
	Graph graph(vCount, eCount);
	size_t population = 1000000 / eCount;

	while (ifile >> first >> second >> weight) {
		graph.addEdge(first, second, weight);
	}

	GeneticSpace geneticSpace(population, &graph);
	size_t generation = 0;
	auto debugStart = clock();
	int debugGeneration = 0;
	cout << " max: " << geneticSpace.chromosomes[0]->fitness << " mean: " << geneticSpace.getAvg() << endl;
	//ofile << "Generation,Maximum Fitness,Minimum Fitness,Average" << endl;
	//ofile << "0," << geneticSpace.chromosomes[0]->fitness 
	//	<< "," << geneticSpace.chromosomes[config.population -1]->fitness
	//	<< "," << geneticSpace.getAvg() << endl;
	size_t iterCount = 0;
	do {
		vector<Chromosome> replaceElems;
		const size_t K = (size_t)(population * config.childrenRatio);
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
			cout << " result max: " << geneticSpace.optimizer->getMaxFitness()
				<< " " << geneticSpace.chromosomes[0]->fitness
				<< " " << geneticSpace.chromosomes[1]->fitness
				<< " mean: " << geneticSpace.getAvg() << endl;
			//	ofile << iterCount 
			//		<< "," << geneticSpace.chromosomes[0]->fitness
			//		<< "," << geneticSpace.chromosomes[config.population - 1]->fitness
			//		<< "," << geneticSpace.getAvg() << endl;
		}
		iterCount++;
		if (geneticSpace.optimizer->isReInitCondition()) {
			geneticSpace.reInitChromosomes((float)generation / config.maxGeneration);
		}
		/*
		for (size_t i = 0; i < geneticSpace.chromosomes.size(); i++) {
			cout << geneticSpace.chromosomes[i]->fitness << " ";
		}
		cout << endl;
		*/
	} while (!Utils::isStopCondition(generation, config.maxGeneration));
	//cout << "iter Count : " << iterCount << endl;
	//ofile << "file Path: " << config.inputFilePath << endl;
	//ofile << "final max : " << geneticSpace.optimizer->getMaxFitness() << endl;
	//ofile << config << endl << "max fitness : " << geneticSpace.chromosomes[0]->fitness << endl << endl;
	ofile << geneticSpace.optimizer->getMaxFitness()
		<< "," << geneticSpace.chromosomes[population - 1]->fitness
		<< "," << geneticSpace.getAvg() << endl;
	//ofile << *(geneticSpace.chromosomes[0]);

	
	ofstream olocalFile("./results/" + name + "result_local.csv", fstream::out | fstream::app);
	size_t multiStartPopulation = (size_t)iterCount * population * config.childrenRatio;
	vector<int> maxFitnesses;
	vector<float> avgFitnesses;
	vector<int> minFitnesses;
	size_t fitnessesSize = multiStartPopulation / 1000;
	maxFitnesses.reserve(fitnessesSize);
	avgFitnesses.reserve(fitnessesSize);
	minFitnesses.reserve(fitnessesSize);
	for (size_t i = 0; i < fitnessesSize; i++) {
		GeneticSpace multiSpace(1000, &graph);
		for (size_t j = 0; j < 1000; j++) {
			multiSpace.chromosomes[j]->searchToLocal(&graph);
		}
		multiSpace.alignPopulations();
		maxFitnesses.emplace_back(multiSpace.chromosomes[0]->fitness);
		minFitnesses.emplace_back(multiSpace.chromosomes[999]->fitness);
		avgFitnesses.emplace_back(multiSpace.getAvg());
	}
	sort(maxFitnesses.begin(), maxFitnesses.end());
	sort(minFitnesses.begin(), minFitnesses.end());
	float average = accumulate(avgFitnesses.begin(), avgFitnesses.end(), 0.0) / avgFitnesses.size();
	olocalFile << maxFitnesses[fitnessesSize - 1]
		<< "," << minFitnesses[0]
		<< "," << average << endl;

	return 0;
}
