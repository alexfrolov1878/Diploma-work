#include <algorithm>
#include <cmath>
#include <vector>
#include <memory>

#include "../GA.hpp"
#include "main.hpp"
#include "Population.hpp"
#include "Process.hpp"
#include "Solution.hpp"
#include "CrossoverStrategies.hpp"

using std::unique_ptr;
using std::vector;
using std::ostream;
using std::ifstream;
using std::move;
using std::random_shuffle;
using std::min;

/*========================DERIVED LEVEL 1 CLASSES==============================*/
/*=============================MICROSCOPIC=====================================*/
double MicroscopicGA::bestSeenSurvivalValue = 0.0;
unique_ptr<MicroscopicSolution> MicroscopicGA::bestSeenSolution =
	unique_ptr<MicroscopicSolution>(new MicroscopicSolution());

MicroscopicGA::MicroscopicGA(ifstream &fin,
	CrossoverType _crossoverType, MemoryType _memoryType) {
	int j, tmp;

	fin >> resultTime;
	fin >> numProcessors;
	fin >> numProcesses;
	fin >> numConnections;

	population.reset(new MicroscopicPopulation(_memoryType));

	vector<int> temp(numProcesses);
	initProcesses = vector<Process>(numProcesses);
	for (int i = 0; i < numProcesses; i++) {
		fin >> tmp;
		initProcesses[i].setComputationalComplexity(tmp);
		for (j = 0;; j++) {
			fin >> temp[j];
			if (temp[j] == -1) {
				break;
			}
		}
		initProcesses[i].setNumberOfPreviousProcecces(j);
		if (j != 0) {
			for (int l = 0; l < j; l++) {
				initProcesses[i].setPreviuosProcess(l, temp[l]);
			}
		}
	}
	temp.clear();

	memoryType = _memoryType;
	microscopicMemoryVector.reset(new MicroscopicMemoryVector(_memoryType));

	crossoverType = _crossoverType;
	switch (crossoverType) {
		case ONE_POINT_VECTOR:
			setCrossoverStrategy(unique_ptr<ICrossoverStrategy>(
				new OnePointVectorStrategy())
			);
			break;
		case ONE_POINT_MATRIX_THROWING:
			setCrossoverStrategy(unique_ptr<ICrossoverStrategy>(
				new OnePointMatrixThrowingStrategy())
			);
			break;
		case ONE_POINT_MATRIX_SWAPPING:
			setCrossoverStrategy(unique_ptr<ICrossoverStrategy>(
				new OnePointMatrixSwappingStrategy())
			);
			break;
		case UNIFORM_MATRIX_THROWING:
			setCrossoverStrategy(unique_ptr<ICrossoverStrategy>(
				new UniformMatrixThrowingStrategy())
			);
			break;
		case UNIFORM_MATRIX_SWAPPING:
			setCrossoverStrategy(unique_ptr<ICrossoverStrategy>(
				new UniformMatrixSwappingStrategy())
			);
			break;
		default:
			setCrossoverStrategy(unique_ptr<ICrossoverStrategy>(
				new OnePointMatrixSwappingStrategy())
			);
			break;
	}
}
MicroscopicGA::~MicroscopicGA() {
	initProcesses.clear();
}

double MicroscopicGA::getBestSeenSurvivalValue() {
	return bestSeenSurvivalValue;
}
void MicroscopicGA::setBestSeenSurvivalValue(double bestSeenSurvivalValue) {
	MicroscopicGA::bestSeenSurvivalValue = bestSeenSurvivalValue;
}
unique_ptr<MicroscopicSolution> &MicroscopicGA::getBestSeenSolution() {
	return bestSeenSolution;
}
void MicroscopicGA::setBestSeenSolution(const MicroscopicSolution &solution) {
	MicroscopicGA::bestSeenSolution.reset(new MicroscopicSolution(solution));
}

void MicroscopicGA::generatePopulation() {
	population->generate();
}
void MicroscopicGA::countSurvivalValues() {
	population->countSurvivalValues(initProcesses);
}
void MicroscopicGA::selection() {
	int maxIndex, index;
	double sumOfSurvivalValues, survivalValue, maxValue;
	auto solutions = population->getSolutions();
	vector<MicroscopicSolution> newSolutions;

	maxValue = solutions[0].getSurvivalValue();
	maxIndex = 0;
	sumOfSurvivalValues = 0.0;
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		survivalValue = solutions[i].getSurvivalValue();
		sumOfSurvivalValues += survivalValue;
		if (survivalValue > maxValue) {
			maxValue = survivalValue;
			maxIndex = i;
		}
	}

	population->setBestSurvivalValue(maxValue);
	double curMaxValue = getBestSeenSurvivalValue();
	if (maxValue > curMaxValue) {
		setBestSeenSurvivalValue(maxValue);
		setBestSeenSolution(solutions[maxIndex]);
	}

	vector<double> sectors = vector<double>(NUM_SOLUTIONS);
	double sum = 0.0;
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		sum += 2 * M_PI * solutions[i].getSurvivalValue() / sumOfSurvivalValues;
		sectors[i] = sum;
	}

	unique_ptr<MicroscopicMemoryVector> newMicroscopicMemoryVector;
	if (memoryType != NONE && memoryType != MACROSCOPIC) {
		newMicroscopicMemoryVector.reset(new MicroscopicMemoryVector(memoryType));
	}

	newSolutions.reserve(NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		double rand = Random::getRandomDouble(0, 2 * M_PI);
		index = lower_bound(sectors.begin(), sectors.end(), rand) - sectors.begin();
		newSolutions.push_back(solutions[index]);
		if (newMicroscopicMemoryVector) {
			newMicroscopicMemoryVector->setMutRow(index,
				microscopicMemoryVector->getMutRow(index));
		}
	}
	population->setSolutions(newSolutions);
	microscopicMemoryVector = move(newMicroscopicMemoryVector);

	newSolutions.clear();
	sectors.clear();
}
void MicroscopicGA::crossover() {
	useCrossoverStrategy();
}
void MicroscopicGA::setCrossoverStrategy(unique_ptr<ICrossoverStrategy> _op) {
	operation = move(_op);
}
void MicroscopicGA::useCrossoverStrategy() {
	operation->execute(population, memoryType, initProcesses,
		microscopicMemoryVector);
}
void MicroscopicGA::mutation() {
	double r;
	double prob, before, after;
	auto solutions = population->getSolutions();

	//mutation over tasks
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		for (int j = 0; j < numProcesses; j++) {
			r = Random::getRandomDouble(0, 1);
			if (memoryType == NONE) {
				prob = MUTATION_TASK_PROBABILITY;
			} else {
				prob = microscopicMemoryVector->getElement(MUTATION_TASK, i, j);
			}
			if (r <= prob) {
				before = solutions[i].getSurvivalValue();
				population->mutateSolution(MUTATION_TASK, i, j);
				population->countSurvivalValue(i, initProcesses);
				after = solutions[i].getSurvivalValue();
				if (memoryType == NONE) {
					//do nothing
				} else {
					microscopicMemoryVector->useChangeStrategy(MUTATION_TASK, i, j,
						before, after);
				}
			}
		}
	}

	//mutation over priorities
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		for (int j = 0; j < numProcesses; j++) {
			r = Random::getRandomDouble(0, 1);
			if (memoryType == NONE) {
				prob = MUTATION_PRIO_PROBABILITY;
			} else {
				 prob = microscopicMemoryVector->getElement(MUTATION_PRIO, i, j);
			}
			if (r <= prob) {
				before = solutions[i].getSurvivalValue();
				population->mutateSolution(MUTATION_PRIO, i, j);
				population->countSurvivalValue(i, initProcesses);
				after = solutions[i].getSurvivalValue();
				if (memoryType == NONE) {
					//do nothing
				} else {
					microscopicMemoryVector->useChangeStrategy(MUTATION_PRIO, i, j,
						before, after);
				}
			}
		}
	}
}
void MicroscopicGA::printCurrentPopulation(ostream &out) {
	population->print(out);
}
double MicroscopicGA::getResult() {
	return bestSeenSolution->getTime();
}
/*=============================================================================*/
/*=============================MACROSCOPIC=====================================*/
MacroscopicGA::MacroscopicGA(ifstream &fin, CrossoverType _crossoverType,
		MemoryType _memoryType) {
	fin >> numWeights;
	fin >> goal;
	fin >> selectionStrength;
	weights = vector<double>(numWeights);
	for (int i = 0; i < numWeights; i++) {
		fin >> weights[i];
	}

	population.reset(new MacroscopicPopulation());
	population->setWeights(weights);

	memoryType = _memoryType;
	if (memoryType == MACROSCOPIC) {
		macroscopicMemoryVector.reset(new MacroscopicMemoryVector());
	} else {
		macroscopicMemoryVector.reset(nullptr);
	}

	crossoverType = _crossoverType;
}
MacroscopicGA::~MacroscopicGA() {
	weights.clear();
}

void MacroscopicGA::generatePopulation() {
	population->generate();
}
void MacroscopicGA::countSurvivalValues() {
	population->countSurvivalValues();
}
void MacroscopicGA::selection() {
	int index;
	auto solutions = population->getSolutions();
	vector<MacroscopicSolution> newSolutions;

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
void MacroscopicGA::crossover() {
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
	if (macroscopicMemoryVector) {
		macroscopicMemoryVector->changeElement(0, k1Before, k1After,
			k2Before, k2After, qBefore, qAfter);
	}
}
void MacroscopicGA::mutation() {
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
	if (macroscopicMemoryVector) {
		macroscopicMemoryVector->changeElement(1, k1After, k1Before,
			k2After, k2Before, qAfter, qBefore);
	}
}
void MacroscopicGA::printCurrentPopulation(ostream &out) {
	population->print(out);
}
double MacroscopicGA::getResult() {
	return population->getResult();
}
/*=============================================================================*/
