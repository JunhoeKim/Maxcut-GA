#include <iostream>
#include <fstream>
#include <ctime>
#include "Graph.h"
#include "GeneticSpace.h"
#include "Utils.h"
#include "Config.h"

using namespace std;

void process(const Config& config);
int main() {
	srand((unsigned int)time(NULL));
	//srand(0);
	vector<size_t> tournamentCounts = { 2, 3, 5 };
	vector<size_t> pointCounts = { 2, 4, 8 };
	size_t maxGeneration = 60;
	vector<size_t> populations = { 300, 700, 1000};
	vector<string> paths = { "unweighted_50.txt", "unweighted_100.txt", "weighted_500.txt" };
	vector<float> childrenRatios = { 0.03f, 0.05f, 0.07f };
	vector<ReplaceType> replaceTypes = { Genitor, Huristic };
	Config config;
	SelectOption* selectOption = new SelectOption(Tournament, tournamentCounts[0]);
	ReplaceOption* replaceOption = new ReplaceOption(Huristic);
	MutateOption* mutateOption = new MutateOption(0.01f);
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
	config.population = 1000;
	config.inputFilePath = "weighted_chimera_297.txt";
	config.childrenRatio = 0.03f;
	process(config);

	int x;
	cin >> x;
	return 0;
}


void process(const Config& config) {
	auto start = clock();
	int vCount = 0, eCount = 0;
	int first = 0, second = 0, weight = 0;

	ifstream ifile(config.inputFilePath, ifstream::in);
	ofstream ofile("result2.txt", fstream::out | fstream::app);
	ifile >> vCount >> eCount;
	//cout << "|V| : " << vCount << " |E| : " << eCount << endl;
	Graph graph(vCount, eCount);

	while (ifile >> first >> second >> weight) {
		graph.addEdge(first, second, weight);
	}

	GeneticSpace geneticSpace(config.population, &graph);
	const size_t K = (size_t)(config.population * config.childrenRatio);
	size_t generation = 0;
	auto debugStart = clock();
	int debugGeneration = 0;
	geneticSpace.initWeights();
	size_t iterCount = 0;
	do {
		vector<Chromosome> replaceElems;
		replaceElems.reserve(K);
		for (size_t i = 0; i < K; i++) {
			auto selectedPair = geneticSpace.select(*config.selectOption);
			auto processed = geneticSpace.crossover(selectedPair.first, selectedPair.second, *config.crossoverOption);
			processed.mutate(generation, config.maxGeneration, config.mutateOption->mutationRatio, &graph);
			replaceElems.emplace_back(processed);
		}
		auto end = clock();
		generation = (end - start) / CLOCKS_PER_SEC;
		debugGeneration = (end - debugStart) / CLOCKS_PER_SEC;
		if (debugGeneration >= 1) {
			debugStart = clock();
			debugGeneration = -1;
		}
		auto replaceIndices = geneticSpace.replace(replaceElems, generation, config.maxGeneration, config.replaceOption);
		geneticSpace.updateWeights(replaceIndices);
		if (debugGeneration == -1) {
			cout << " max fitness : " << geneticSpace.chromosomes[0]->fitness << endl;
		}
		iterCount++;
	} while (!Utils::isStopCondition(generation, config.maxGeneration));
	//geneticSpace.printElems();
	cout << "iter Count : " << iterCount << endl;
	cout << " x, y : " << graph.x << " " << graph.y << endl;
	ofile << config << endl << "max fitness : " << geneticSpace.chromosomes[0]->fitness << endl << endl;

}