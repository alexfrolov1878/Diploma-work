#include "../GA.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

#include "main.hpp"
#include "Population.hpp"
#include "Process.hpp"
#include "Solution.hpp"

/*========================DERIVED LEVEL 1 CLASSES==============================*/
/*=============================MICROSCOPIC=====================================*/
double MicroscopicGA::bestSeenSurvivalValue = 0.0;
MicroscopicSolution *MicroscopicGA::bestSeenSolution =
		new MicroscopicSolution();

MicroscopicGA::MicroscopicGA(std::ifstream &fin, MemoryType memoryType) {
	int j, tmp;
	int *temp;

	fin >> resultTime;
	::resultTime = resultTime;
	fin >> numProcessors;
	::numProcessors = numProcessors;
	fin >> numProcesses;
	::numProcesses = numProcesses;
	fin >> numConnections;
	::numConnections = numConnections;

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

	this->memoryType = memoryType;
        microscopicMemoryVector =
		MicroscopicMemoryVectorFactory::newMemoryVector(memoryType);
}
MicroscopicGA::~MicroscopicGA() {
	delete MicroscopicGA::bestSeenSolution;
	delete population;
	delete[] initProcesses;
	delete microscopicMemoryVector;
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

	std::vector<double> sectors = std::vector<double>(NUM_SOLUTIONS);
	double sum = 0.0;
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		sum += 2 * M_PI * solutions[i].getSurvivalValue() / sumOfSurvivalValues;
		sectors[i] = sum;
	}

	MicroscopicMemoryVector *newMicroscopicMemoryVector = NULL;
    if (memoryType != NONE && memoryType != MACROSCOPIC) {
        newMicroscopicMemoryVector =
            MicroscopicMemoryVectorFactory::newMemoryVector(memoryType);
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
	delete microscopicMemoryVector;
        microscopicMemoryVector = newMicroscopicMemoryVector;

	newSolutions.clear();
	sectors.clear();
}
void MicroscopicGA::crossover() {
	double r;
	int pairs[NUM_SOLUTIONS], start;
	double prob, prob1, prob2, before1, before2, after1, after2;
        const std::vector<MicroscopicSolution> solutions =
                        population->getSolutions();

	//crossover over tasks
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	std::random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		r = Random::getRandomDouble(0, 1);
		if (memoryType == NONE) {
                        prob = CROSSOVER_TASK_PROBABILITY;
                } else {
                        prob1 = microscopicMemoryVector->getElement(CROSSOVER_TASK,
                                        pairs[i]);
                        prob2 = microscopicMemoryVector->getElement(CROSSOVER_TASK,
                                        pairs[i + 1]);
                        prob = std::min(prob1, prob2);
                }
		if (r <= prob) {
			before1 = solutions[pairs[i]].getSurvivalValue();
                        before2 = solutions[pairs[i + 1]].getSurvivalValue();
                        start = population->crossoverSolutions(CROSSOVER_TASK, pairs[i],
                                        pairs[i + 1]);
                        population->countSurvivalValue(pairs[i], initProcesses);
                        population->countSurvivalValue(pairs[i + 1], initProcesses);
                        after1 = solutions[pairs[i]].getSurvivalValue();
                        after2 = solutions[pairs[i + 1]].getSurvivalValue();
                        if (memoryType == NONE) {
                                //do nothing
                        } else {
                                microscopicMemoryVector->swapMutElements(CROSSOVER_TASK,
                                                pairs[i], pairs[i + 1], start);
                                microscopicMemoryVector->changeElement(CROSSOVER_TASK, pairs[i],
                                                0, before1, after1);
                                microscopicMemoryVector->changeElement(CROSSOVER_TASK,
                                                pairs[i + 1], 0, before2, after2);
                        }
                }
	}

	//crossover over priorities
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	std::random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		r = Random::getRandomDouble(0, 1);
		if (memoryType == NONE) {
                        prob = CROSSOVER_PRIO_PROBABILITY;
                } else {
                        prob1 = microscopicMemoryVector->getElement(CROSSOVER_PRIO,
                                        pairs[i]);
                        prob2 = microscopicMemoryVector->getElement(CROSSOVER_PRIO,
                                        pairs[i + 1]);
                        prob1 = std::min(prob1, prob2);
                }
                if (r <= prob) {
                        before1 = solutions[pairs[i]].getSurvivalValue();
                        before2 = solutions[pairs[i + 1]].getSurvivalValue();
                        start = population->crossoverSolutions(CROSSOVER_PRIO, pairs[i],
                                        pairs[i + 1]);
                        after1 = solutions[pairs[i]].getSurvivalValue();
                        after2 = solutions[pairs[i + 1]].getSurvivalValue();
                        if (memoryType == NONE) {
                                //do nothing
                        } else {
                                microscopicMemoryVector->swapMutElements(CROSSOVER_PRIO,
                                                pairs[i], pairs[i + 1], start);
                                microscopicMemoryVector->changeElement(CROSSOVER_PRIO, pairs[i],
                                                1, before1, after1);
                                microscopicMemoryVector->changeElement(CROSSOVER_PRIO,
                                                pairs[i + 1], 1, before2, after2);
                        }
                }
	}
}
void MicroscopicGA::mutation() {
	double r;
	double prob, before, after;
        const std::vector<MicroscopicSolution> solutions =
                        population->getSolutions();

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
                                        microscopicMemoryVector->changeElement(MUTATION_TASK, i, j,
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
                                        microscopicMemoryVector->changeElement(MUTATION_PRIO, i, j,
                                                        before, after);
                                }
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
MacroscopicGA::MacroscopicGA(std::ifstream &fin, MemoryType memoryType) {
	fin >> numWeights;
	::numWeights = numWeights;
	fin >> goal;
	::goal = goal;
	fin >> selectionStrength;
	weights = std::vector<double>(numWeights);
	for (int i = 0; i < numWeights; i++) {
		fin >> weights[i];
	}

	population = new MacroscopicPopulation();
	population->setSelectionStrength(selectionStrength);
	population->setWeights(weights);

	this->memoryType = memoryType;
    if (memoryType == MACROSCOPIC) {
        macroscopicMemoryVector = new MacroscopicMemoryVector();
    } else {
        macroscopicMemoryVector = NULL;
    }
}
MacroscopicGA::~MacroscopicGA() {
	delete population;
	weights.clear();
	delete macroscopicMemoryVector;
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

	std::vector<double> sectors = std::vector<double>(NUM_SOLUTIONS);
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
    if (macroscopicMemoryVector) {
        macroscopicMemoryVector->changeElement(0, goal, k1Before, k1After,
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
        macroscopicMemoryVector->changeElement(1, goal, k1After, k1Before,
            k2After, k2Before, qAfter, qBefore);
    }
}
void MacroscopicGA::printCurrentPopulation(std::ostream &out) {
	population->print(out);
}
double MacroscopicGA::getResult() {
    return population->getResult();
}
/*=============================================================================*/
