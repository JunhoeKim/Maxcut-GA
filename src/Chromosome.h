#ifndef CHROMOSOME_H_
#define CHROMOSOME_H_

#include <vector>
#include <memory>
#include <iostream>

using namespace std;

class Chromosome {
public:
	Chromosome();
	Chromosome(size_t size);
	vector<int> genes;
	void mutate(int generation, int maxGeneration);
};
#endif
