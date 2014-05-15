#include "../library/GA.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

#include "main.hpp"
#include "Population.hpp"
#include "Process.hpp"

int findLowerBound(double *a, int size, double key) {
	int indexLeft, indexRight, indexMiddle;

	indexLeft = 0;
	indexRight = size - 1;
	while (indexLeft < indexRight) {
		indexMiddle = indexLeft + (indexRight - indexLeft) / 2;
		if (a[indexMiddle] < key) {
			indexLeft = indexMiddle + 1;
		} else {
			indexRight = indexMiddle;
		}
	}

	return indexLeft - 1;
}
/*========================DERIVED LEVEL 1 CLASSES==============================*/
/*=============================MICROSCOPIC=====================================*/
double MicroscopicGA::bestSeenSurvivalValue = 0.0;
MicroscopicSolution *MicroscopicGA::bestSeenSolution = new MicroscopicSolution(
		NONE);

MicroscopicGA::MicroscopicGA(std::ifstream &fin, MemoryType memoryType) {
	int j, tmp;
	int *temp;

	fin >> tmp;
	resultTime = tmp;
	::resultTime = resultTime;
	fin >> tmp;
	numProcessors = tmp;
	::numProcessors = tmp;
	fin >> tmp;
	numProcesses = tmp;
	::numProcesses = tmp;
	fin >> tmp;
	numConnections = tmp;
	::numConnections = tmp;

	population = new MicroscopicPopulation(memoryType);

	temp = new int[numProcesses];
	initProcesses = new Process[numProcesses];
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
	delete[] temp;
}
MicroscopicGA::~MicroscopicGA() {
	delete MicroscopicGA::bestSeenSolution;
	delete population;
	delete[] initProcesses;
}

double MicroscopicGA::getBestSeenSurvivalValue() {
	return bestSeenSurvivalValue;
}
void MicroscopicGA::setBestSeenSurvivalValue(double bestSeenSurvivalValue) {
	MicroscopicGA::bestSeenSurvivalValue = bestSeenSurvivalValue;
}
MicroscopicSolution *MicroscopicGA::getBestSeenSolution() {
	return bestSeenSolution;
}
void MicroscopicGA::setBestSeenSolution(const MicroscopicSolution &solution) {
	delete MicroscopicGA::bestSeenSolution;
	MicroscopicGA::bestSeenSolution = new MicroscopicSolution(solution);
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
	const std::vector<MicroscopicSolution> solutions =
			population->getSolutions();
	std::vector<MicroscopicSolution> newSolutions;

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

	double *sectors = new double[NUM_SOLUTIONS + 1];
	double sum = 0.0;
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		sectors[i] = sum;
		sum += 2 * M_PI * solutions[i].getSurvivalValue() / sumOfSurvivalValues;
	}
	sectors[NUM_SOLUTIONS] = sum;

	newSolutions.reserve(NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		double rand = Random::getRandomDouble(0, 2 * M_PI);
		index = findLowerBound(sectors, NUM_SOLUTIONS + 1, rand);
		newSolutions.push_back(solutions[index]);
	}
	population->setSolutions(newSolutions);

	newSolutions.clear();
	delete[] sectors;
}
void MicroscopicGA::crossover() {
	double r;
	int pairs[NUM_SOLUTIONS];

	//crossover over tasks
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	std::random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		r = Random::getRandomDouble(0, 1);
		if (r <= CROSSOVER_TASK_PROBABILITY) {
			population->crossoverSolutions(CROSSOVER_TASK, pairs[i],
					pairs[i + 1]);
			population->countSurvivalValue(pairs[i], initProcesses);
			population->countSurvivalValue(pairs[i + 1], initProcesses);
		}
	}

	//crossover over priorities
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	std::random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		r = Random::getRandomDouble(0, 1);
		if (r <= CROSSOVER_PRIO_PROBABILITY) {
			population->crossoverSolutions(CROSSOVER_PRIO, pairs[i],
					pairs[i + 1]);
			population->countSurvivalValue(pairs[i], initProcesses);
			population->countSurvivalValue(pairs[i + 1], initProcesses);
		}
	}
}
void MicroscopicGA::mutation() {
	double r;

	//mutation over tasks
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		for (int j = 0; j < numProcesses; j++) {
			r = Random::getRandomDouble(0, 1);
			if (r <= MUTATION_TASK_PROBABILITY) {
				population->mutateSolution(MUTATION_TASK, i, j);
				population->countSurvivalValue(i, initProcesses);
			}
		}
	}

	//mutation over priorities
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		for (int j = 0; j < numProcesses; j++) {
			r = Random::getRandomDouble(0, 1);
			if (r <= MUTATION_PRIO_PROBABILITY) {
				population->mutateSolution(MUTATION_PRIO, i, j);
				population->countSurvivalValue(i, initProcesses);
			}
		}
	}
}
void MicroscopicGA::printCurrentPopulation(std::ostream &out) {
	population->print(out);
}
double MicroscopicGA::getResult() {
        return bestSeenSolution->getTime();
}
/*=============================================================================*/
/*=============================MACROSCOPIC=====================================*/
MacroscopicGA::MacroscopicGA(std::ifstream &fin) {
	int tmpInt;
	double tmpDouble;

	fin >> tmpInt;
	numWeights = tmpInt;
	::numWeights = tmpInt;
	fin >> tmpDouble;
	goal = tmpDouble;
	::goal = goal;
	fin >> tmpDouble;
	selectionStrength = tmpDouble;
	weights = new double[numWeights];
	for (int i = 0; i < numWeights; i++) {
		fin >> tmpDouble;
		weights[i] = tmpDouble;
	}

	population = new MacroscopicPopulation();
	population->setSelectionStrength(selectionStrength);
	population->setWeights(weights);
}
MacroscopicGA::~MacroscopicGA() {
	delete population;
	delete[] weights;
}

void MacroscopicGA::generatePopulation() {
	population->generate();
}
void MacroscopicGA::countSurvivalValues() {
	population->countSurvivalValues();
}
void MacroscopicGA::selection() {
	int index;
	const std::vector<MacroscopicSolution> solutions =
			population->getSolutions();
	std::vector<MacroscopicSolution> newSolutions;

	double sumOfSurvivalValues = 0.0;
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		sumOfSurvivalValues += solutions[i].getSurvivalValue();
	}
	double *sectors = new double[NUM_SOLUTIONS + 1];
	double sum = 0;
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		sectors[i] = sum;
		sum += 2 * M_PI * solutions[i].getSurvivalValue() / sumOfSurvivalValues;
	}
	sectors[NUM_SOLUTIONS] = sum;

	newSolutions.reserve(NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		double rand = Random::getRandomDouble(0, 2 * M_PI);
		index = findLowerBound(sectors, NUM_SOLUTIONS + 1, rand);
		newSolutions.push_back(solutions[index]);
	}
	population->setSolutions(newSolutions);
	population->updateMacroparameters(SELECTION);

	newSolutions.clear();
	delete[] sectors;
}
void MacroscopicGA::crossover() {
	int pairs[NUM_SOLUTIONS];
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	std::random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		population->crossoverSolutions(pairs[i], pairs[i + 1]);
		population->countSurvivalValue(pairs[i]);
		population->countSurvivalValue(pairs[i + 1]);
	}
	population->updateMacroparameters(CROSSOVER);
}
void MacroscopicGA::mutation() {
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		population->mutateSolution(i);
		population->countSurvivalValue(i);
	}
	population->updateMacroparameters(MUTATION);
}
void MacroscopicGA::printCurrentPopulation(std::ostream &out) {
	population->print(out);
}
double MacroscopicGA::getResult() {
        return population->getResult();
}
/*=============================================================================*/

/*======================DERIVED LEVEL 2 CLASSES================================*/
/*=============================MICROSCOPIC=====================================*/
MicroscopicMemoryGA::MicroscopicMemoryGA(std::ifstream &in,
		MemoryType memoryType) :
		MicroscopicGA(in, memoryType) {
	this->memoryType = memoryType;
	macroscopicMemoryVector = new MacroscopicMemoryVector();
}
MicroscopicMemoryGA::~MicroscopicMemoryGA() {
	delete macroscopicMemoryVector;
}

void MicroscopicMemoryGA::crossover() {
	double r, prob1, prob2, before1, before2, after1, after2;
	const std::vector<MicroscopicSolution> solutions =
			population->getSolutions();
	int pairs[NUM_SOLUTIONS];

	//crossover over tasks
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	std::random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		r = Random::getRandomDouble(0, 1);
		if (memoryType == MACROSCOPIC) {
			prob1 = prob2 = macroscopicMemoryVector->getElement(CROSSOVER_TASK);
		} else {
			prob1 = solutions[pairs[i]].getMemoryElement(CROSSOVER_TASK);
			prob2 = solutions[pairs[i + 1]].getMemoryElement(CROSSOVER_TASK);
		}
		prob1 = std::min(prob1, prob2);
		if (r <= prob1) {
			before1 = solutions[pairs[i]].getSurvivalValue();
			before2 = solutions[pairs[i + 1]].getSurvivalValue();
			population->crossoverSolutions(CROSSOVER_TASK, pairs[i],
					pairs[i + 1]);
			population->countSurvivalValue(pairs[i], initProcesses);
			population->countSurvivalValue(pairs[i + 1], initProcesses);
			after1 = solutions[pairs[i]].getSurvivalValue();
			after2 = solutions[pairs[i + 1]].getSurvivalValue();
			population->updateMemoryElement(CROSSOVER_TASK, pairs[i], before1,
					after1);
			population->updateMemoryElement(CROSSOVER_TASK, pairs[i + 1],
					before2, after2);
		}
	}

	//crossover over priorities
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	std::random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		r = Random::getRandomDouble(0, 1);
		if (memoryType == MACROSCOPIC) {
			prob1 = prob2 = macroscopicMemoryVector->getElement(CROSSOVER_TASK);
		} else {
			prob1 = solutions[pairs[i]].getMemoryElement(CROSSOVER_TASK);
			prob2 = solutions[pairs[i + 1]].getMemoryElement(CROSSOVER_TASK);
		}
		prob1 = std::min(prob1, prob2);
		if (r <= prob1) {
			before1 = solutions[pairs[i]].getSurvivalValue();
			before2 = solutions[pairs[i + 1]].getSurvivalValue();
			population->crossoverSolutions(CROSSOVER_PRIO, pairs[i],
					pairs[i + 1]);
			after1 = solutions[pairs[i]].getSurvivalValue();
			after2 = solutions[pairs[i + 1]].getSurvivalValue();
			population->updateMemoryElement(CROSSOVER_PRIO, pairs[i], before1,
					after1);
			population->updateMemoryElement(CROSSOVER_PRIO, pairs[i + 1],
					before2, after2);
		}
	}
}
void MicroscopicMemoryGA::mutation() {
	double r, prob1, before, after;
	const std::vector<MicroscopicSolution> solutions =
			population->getSolutions();

	//mutation over tasks
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		for (int j = 0; j < numProcesses; j++) {
			r = Random::getRandomDouble(0, 1);
			if (memoryType == MACROSCOPIC) {
				prob1 = macroscopicMemoryVector->getElement(MUTATION_TASK, j);
			} else {
				prob1 = solutions[i].getMemoryElement(MUTATION_TASK, j);
			}
			if (r <= prob1) {
				before = solutions[i].getSurvivalValue();
				population->mutateSolution(MUTATION_TASK, i, j);
				population->countSurvivalValue(i, initProcesses);
				after = solutions[i].getSurvivalValue();
				population->updateMemoryElement(MUTATION_TASK, i, after, before,
						j);
			}
		}
	}

	//mutation over priorities
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		for (int j = 0; j < numProcesses; j++) {
			r = Random::getRandomDouble(0, 1);
			if (memoryType == MACROSCOPIC) {
				prob1 = macroscopicMemoryVector->getElement(MUTATION_TASK, j);
			} else {
				prob1 = solutions[i].getMemoryElement(MUTATION_TASK, j);
			}
			if (r <= prob1) {
				before = solutions[i].getSurvivalValue();
				population->mutateSolution(MUTATION_PRIO, i, j);
				population->countSurvivalValue(i, initProcesses);
				after = solutions[i].getSurvivalValue();
				population->updateMemoryElement(MUTATION_PRIO, i, after, before,
						j);
			}
		}
	}
}
/*=============================================================================*/
/*=============================MACROSCOPIC=====================================*/
MacroscopicMemoryGA::MacroscopicMemoryGA(std::ifstream &in,
		MemoryType memoryType) :
		MacroscopicGA(in) {
	this->memoryType = memoryType;
	macroscopicMemoryVector = new MacroscopicMemoryVector();
}
MacroscopicMemoryGA::~MacroscopicMemoryGA() {
	delete macroscopicMemoryVector;
}
void MacroscopicMemoryGA::crossover() {
	double k1Before = population->getK1();
	double k2Before = population->getK2();
	double qBefore = population->getQ();

	int pairs[NUM_SOLUTIONS];
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	std::random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		population->crossoverSolutions(pairs[i], pairs[i + 1]);
		population->countSurvivalValue(pairs[i]);
		population->countSurvivalValue(pairs[i + 1]);
	}

	population->updateMacroparameters(CROSSOVER);
	double k1After = population->getK1();
	double k2After = population->getK2();
	double qAfter = population->getQ();
	macroscopicMemoryVector->changeElement(0, goal, k1Before, k1After, k2Before,
			k2After, qBefore, qAfter);
}
void MacroscopicMemoryGA::mutation() {
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
	macroscopicMemoryVector->changeElement(1, goal, k1After, k1Before, k2After,
			k2Before, qAfter, qBefore);
}
/*=============================================================================*/
