#include <algorithm>
#include <cmath>
#include <vector>
#include <memory>

#include "GA.hpp"
#include "main.hpp"
#include "Population.hpp"
#include "Solution.hpp"
#include "CrossoverStrategies.hpp"

using std::unique_ptr;
using std::vector;
using std::ostream;
using std::ifstream;
using std::move;
using std::random_shuffle;
using std::min;

SubsetSumGA::SubsetSumGA(ifstream &fin,
							 CrossoverType _crossoverType,
							 MemoryType _memoryType) {
	fin >> numWeights;
	fin >> goal;
	fin >> selectionStrength;

	weights = vector<double>(numWeights);
	for (int i = 0; i < numWeights; i++) {
		fin >> weights[i];
	}

	population.reset(new Population());
	population->setWeights(weights);

	memoryType = _memoryType;
	if (memoryType == MACROSCOPIC) {
		macroscopicMemoryVector.reset(new MemoryVector());
	} else {
		macroscopicMemoryVector.reset(nullptr);
	}

	crossoverType = _crossoverType;
	// TODO: ...
}
SubsetSumGA::~SubsetSumGA() {
	weights.clear();
}

void SubsetSumGA::generatePopulation() {
	population->generate();
}
void SubsetSumGA::countSurvivalValues() {
	population->countSurvivalValues();
}
void SubsetSumGA::selection() {
	int index;
	auto solutions = population->getSolutions();
	vector<Solution> newSolutions;

	double sumOfSurvivalValues = 0.0;
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		sumOfSurvivalValues += solutions[i].getSurvivalValue();
	}

	vector<double> sectors = vector<double>(NUM_SOLUTIONS);
	double sum = 0.0;
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		sum += 2 * M_PI * solutions[i].getSurvivalValue() / sumOfSurvivalValues;
		sectors[i] = sum;
	}

	newSolutions.reserve(NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		double rand = Random::getRandomDouble(0, 2 * M_PI);
		index = lower_bound(sectors.begin(), sectors.end(), rand) - sectors.begin();
		newSolutions.push_back(solutions[index]);
	}
	population->setSolutions(newSolutions);
	population->updateMacroparameters(SELECTION);

	newSolutions.clear();
	sectors.clear();
}
void SubsetSumGA::crossover() {
	double k1Before = population->getK1();
	double k2Before = population->getK2();
	double qBefore = population->getQ();

	int pairs[NUM_SOLUTIONS];
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		population->crossoverSolutions(pairs[i], pairs[i + 1]);
		population->countSurvivalValue(pairs[i]);
		population->countSurvivalValue(pairs[i + 1]);
	}
	population->updateMacroparameters(CROSSOVER);

	double k1After = population->getK1();
	double k2After = population->getK2();
	double qAfter = population->getQ();
	if (memoryVector) {
		memoryVector->changeElement(0, k1Before, k1After,
			k2Before, k2After, qBefore, qAfter);
	}
}
void SubsetSumGA::mutation() {
	double k1Before = population->getK1();
	double k2Before = population->getK2();
	double qBefore = population->getQ();

	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		population->mutateSolution(i);
		population->countSurvivalValue(i);
	}
	population->updateMacroparameters(MUTATION);

	double k1After = population->getK1();
	double k2After = population->getK2();
	double qAfter = population->getQ();
	if (memoryVector) {
		memoryVector->changeElement(1, k1After, k1Before,
			k2After, k2Before, qAfter, qBefore);
	}
}
void SubsetSumGA::printCurrentPopulation(ostream &out) {
	population->print(out);
}
double SubsetSumGA::getResult() {
	return population->getResult();
}
/*=============================================================================*/
