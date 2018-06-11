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

int process(ofstream& ofile, const Config& config);
float avg(vector<int>& fitnesses);
int max(vector<int>& fitnesses);
float stdev(vector<int>& fitnesses);

int main() {
	srand((unsigned int)time(NULL));
	vector<size_t> tournamentCounts = { 2, 3, 5 };
	vector<size_t> pointCounts = { 2, 4, 8 };
	size_t maxGeneration = 60;
	vector<size_t> populations = { 300, 700, 1000 };
	vector<string> paths = { "planar_layered_1600.txt", "toroidal_overlapped_3000.txt", "planar_2000.txt" , "cubic_2744.txt", "treecone_3000.txt", "treecone_overlapped_3000.txt" };
	vector<string> names = { "planar_layered_", "toroidal_overlapped_", "planar_", "cubic_", "treecone_", "treecone_overlapped_" };
	//vector<string> paths = { "cubic_2744.txt", "planar_2000.txt", "treecone_overlapped_3000.txt", "toroidal_overlapped_3000.txt", "planar_amplified_3000.txt", "planar_layered_1600.txt", "random_2000.txt", "toroidal_2000.txt",
	//	 "treecone_3000.txt" };
	//vector<string> names = { "cubic_", "planar_", "treecone_overlapped_", "toroidal_overlapped_", "planar_amplified_", "planar_layered_", "random_", "toroidal_",  "trecone_" };
	vector<float> childrenRatios = { 0.03f, 0.05f, 0.07f };
	vector<ReplaceType> replaceTypes = { Genitor, ReplaceRandom };
	Config config;
	SelectOption* selectOption = new SelectOption(Tournament, tournamentCounts[0]);
	ReplaceOption* replaceOption = new ReplaceOption(Genitor);
	MutateOption* mutateOption = new MutateOption(Uniform, 0.02f);
	config.selectOption = selectOption;
	config.replaceOption = replaceOption;
	config.maxGeneration = maxGeneration;
	config.mutateOption = mutateOption;
	config.selectOption = new SelectOption(Tournament, 2);
	config.crossoverOption = new CrossoverOption(3);
	config.maxGeneration = 495;
	config.population = 1000;
	config.inputFilePath = "./maxcut.in";
	config.childrenRatio = 0.07f;

	ofstream ofile("./maxcut.out", fstream::out);
	process(ofile, config);
	//for (size_t i = 0; i < 6; i++) {
	//	config.inputFilePath = "../proj3_instances/" + paths[i];
	//	for (size_t j = 0; j < 4; j++) {
	//		cout << config.inputFilePath << endl;
	//		process(ofile, config, names[i]);
	//	}
	//}

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

int process(ofstream& ofile, const Config& config) {

	auto start = clock();
	size_t vCount = 0, eCount = 0;
	int first = 0, second = 0, weight = 0;

	ifstream ifile(config.inputFilePath, ifstream::in);
	//ofstream ofile("./results/" + name + "result_by_generation.csv", fstream::out | fstream::app);
	ifile >> vCount >> eCount;
	Graph graph(vCount, eCount);

	while (ifile >> first >> second >> weight) {
		graph.addEdge(first, second, weight);
	}
	const size_t K = (size_t)(config.population * config.childrenRatio);
	GeneticSpace geneticSpace(config.population, &graph, K);
	size_t generation = 0;
	auto debugStart = clock();
	int debugGeneration = 0;
	//cout << " max: " << geneticSpace.chromosomes[0]->fitness << " mean: " << geneticSpace.getAvg() << endl;
	//ofile << "Generation,Global Maximum Fitness,Local Maximum Fitness,Local Minimum Fitness,Average" << endl;
	//ofile << "0," << geneticSpace.chromosomes[0]->fitness
	//	<< "," << geneticSpace.chromosomes[0]->fitness
	//	<< "," << geneticSpace.chromosomes[config.population -1]->fitness
	//	<< "," << geneticSpace.getAvg() << endl;

	size_t iterCount = 0;
	do {
		const size_t K = (size_t)(geneticSpace.chromosomes.size() * config.childrenRatio);

		for (size_t i = 0; i < K; i++) {
			auto selectedPair = geneticSpace.select(*config.selectOption);
			geneticSpace.crossover(selectedPair.first, selectedPair.second, *config.crossoverOption, i);
			geneticSpace.tempChromosomes[i].mutate(config.mutateOption, &graph, iterCount);
			geneticSpace.tempChromosomes[i].searchToLocal(&graph);
		}

		auto end = clock();
		generation = (end - start) / CLOCKS_PER_SEC;
		debugGeneration = (end - debugStart) / CLOCKS_PER_SEC;

		if (debugGeneration >= 1) {
			debugStart = clock();
			debugGeneration = -1;
		}

		geneticSpace.replace(generation, config.maxGeneration, config.replaceOption);
		geneticSpace.alignPopulations();

		if (debugGeneration == -1) {
			//cout << " max: " << geneticSpace.optimizer->getMaxFitness()
			//	<< " " << geneticSpace.chromosomes[0]->fitness
			//	<< " " << geneticSpace.chromosomes[1]->fitness
			//	<< " " << geneticSpace.chromosomes[2]->fitness
			//	<< " " << geneticSpace.chromosomes[geneticSpace.chromosomes.size() / 2]->fitness
			//	<< " mean: " << geneticSpace.getAvg()
			//	<< " size: " << geneticSpace.chromosomes.size()
			//	<< " iter count: " << iterCount << endl;
			//ofile << iterCount
			//	<< "," << geneticSpace.optimizer->getMaxFitness()
			//	<< "," << geneticSpace.chromosomes[0]->fitness
			//	<< "," << geneticSpace.chromosomes[config.population - 1]->fitness
			//	<< "," << geneticSpace.getAvg() << endl;
		}

		iterCount++;

		// RE INIT
		if (geneticSpace.optimizer->isReInitCondition(iterCount, generation)) {
			geneticSpace.reInitChromosomes(generation);
		}

	} while (!Utils::isStopCondition(generation, config.maxGeneration));
	for (size_t i = 0; i < vCount; i++) {
		if (geneticSpace.optimizer->maxChromosome->genes[i] == 0) {
			ofile << i + 1 << " ";
		}
	}
	//ofstream olocalMaxFile("./results/" + name + "result_max.csv", fstream::out | fstream::app);
	//vector<pair<size_t, int>> localMaxFitnesses = geneticSpace.optimizer->tempMaxFitnesses;
	//cout << localMaxFitnesses.size() << endl;
	//olocalMaxFile << "Generation,Fitness,Average" << endl;
	//for (size_t i = 0; i < localMaxFitnesses.size(); i++) { 
	//	cout << localMaxFitnesses[i].first << "," << localMaxFitnesses[i].second << endl;
	//	olocalMaxFile << localMaxFitnesses[i].first << "," << localMaxFitnesses[i].second
	//		<< "," << geneticSpace.optimizer->tempAverages[i] << endl;
	//}
	//ofile << "file Path: " << config.inputFilePath << endl;
	//ofile << "iter Count : " << iterCount << endl;
	//ofile << "final max : " << geneticSpace.optimizer->getMaxFitness() << endl;
	//ofile << config << endl << "max fitness : " << geneticSpace.chromosomes[0]->fitness << endl << endl;
	//ofile << geneticSpace.optimizer->getMaxFitness();
	//	<< "," << geneticSpace.chromosomes[population - 1]->fitness
	//	<< "," << geneticSpace.getAvg() << endl;
	//ofile << *(geneticSpace.chromosomes[0]);


	//ofstream olocalFile("./results/" + name + "result_local.csv", fstream::out | fstream::app);
	//size_t multiStartPopulation = (size_t)iterCount * population * config.childrenRatio;
	//vector<int> maxFitnesses;
	//vector<float> avgFitnesses;
	//vector<int> minFitnesses;
	//size_t fitnessesSize = multiStartPopulation / 1000;
	//maxFitnesses.reserve(fitnessesSize);
	//avgFitnesses.reserve(fitnessesSize);
	//minFitnesses.reserve(fitnessesSize);
	//for (size_t i = 0; i < fitnessesSize; i++) {
	//	GeneticSpace multiSpace(1000, &graph);
	//	for (size_t j = 0; j < 1000; j++) {
	//		multiSpace.chromosomes[j]->searchToLocal(&graph);
	//	}
	//	multiSpace.alignPopulations();
	//	maxFitnesses.emplace_back(multiSpace.chromosomes[0]->fitness);
	//	minFitnesses.emplace_back(multiSpace.chromosomes[999]->fitness);
	//	avgFitnesses.emplace_back(multiSpace.getAvg());
	//}
	//sort(maxFitnesses.begin(), maxFitnesses.end());
	//sort(minFitnesses.begin(), minFitnesses.end());
	//float average = accumulate(avgFitnesses.begin(), avgFitnesses.end(), 0.0) / avgFitnesses.size();
	//olocalFile << maxFitnesses[fitnessesSize - 1]
	//	<< "," << minFitnesses[0]
	//	<< "," << average << endl;

	return 0;
}
