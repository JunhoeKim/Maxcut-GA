#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <iostream>
using namespace std;
enum SelectType {
	Rank, Tournament
};
enum ReplaceType {
	Genitor, Huristic
};
class SelectOption {
public:
	SelectOption(SelectType, size_t tournamentSize);
	SelectType selectType;
	size_t tournamentSize;
};

class CrossoverOption {
public:
	CrossoverOption(size_t pointCount);
	size_t pointCount;
};

class ReplaceOption {
public:
	ReplaceOption(ReplaceType);
	ReplaceType replaceType;
};

class MutateOption {
public:
	MutateOption(float);
	float mutationRatio;
};

class Config {
public:
	Config(SelectOption*, CrossoverOption*, size_t maxGeneration, size_t, float, string);
	Config();
	SelectOption* selectOption;
	CrossoverOption* crossoverOption;
	ReplaceOption* replaceOption;
	MutateOption* mutateOption;
	size_t maxGeneration;
	size_t population;
	float childrenRatio;
	string inputFilePath;
	friend ostream& operator<<(std::ostream& os, const Config& obj);
};

#endif