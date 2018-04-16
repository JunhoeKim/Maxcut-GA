#include "Config.h"
#define stringify(name) # name

const string replaceTypeStrings[] =
{
	stringify(Genitor),
	stringify(Huristic)
};

const string selectTypeStrings[] = {
	stringify(Rank),
	stringify(Tournament)
};

Config::Config(SelectOption* _selectOption,
	CrossoverOption* _crossoverOption,
	size_t _maxGeneration,
	size_t _population,
	float _childrenRatio,
	string _filePath): 
	selectOption(_selectOption),
	crossoverOption(_crossoverOption),
	maxGeneration(_maxGeneration),
	population(_population),
	childrenRatio(_childrenRatio),
	inputFilePath(_filePath) {
}

Config::Config() {
	selectOption = new SelectOption(Tournament, 2);
	crossoverOption = new CrossoverOption(2);
}
CrossoverOption::CrossoverOption(size_t _pointCount) : pointCount(_pointCount) {

}

SelectOption::SelectOption(SelectType _selectType, size_t _tournamentSize): 
	selectType(_selectType),
	tournamentSize(_tournamentSize) {
}

ReplaceOption::ReplaceOption(ReplaceType type) : replaceType(type) {

}

MutateOption::MutateOption(float ratio) : mutationRatio(ratio) {

}

std::ostream& operator<<(std::ostream& os, const Config& obj) {
	os << "SelectType : " << selectTypeStrings[obj.selectOption->selectType]
		<< ", size: " << obj.selectOption->tournamentSize << std::endl
		<< "Crossover point Count : " << obj.crossoverOption->pointCount << endl
		<< "Population : " << obj.population << endl
		<< "Children ratio : " << obj.childrenRatio << endl
		<< "Replace option type : " << replaceTypeStrings[obj.replaceOption->replaceType] << endl
		<< "Input File Path : " << obj.inputFilePath << endl;
	return os;
}

//string Config::toString() {
//	string result = "";
//	result += "SelectType : ";
//	result += selectOption.selectType;
//	result += ", size: ";
//	return "SelectType : " + selectOption.selectType + ", size: " + selectOption.tournamentSize + "\n"
//		+ "Crossover point Count : " + crossoverOption.pointCount + "\n"
//		+ ""
//}