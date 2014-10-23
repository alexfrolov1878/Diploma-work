#include <algorithm>
#include <cmath>
#include <vector>
#include <memory>

#include "GA.hpp"
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

double JobSchedulingGA::bestSeenSurvivalValue = 0.0;
unique_ptr<Solution> JobSchedulingGA::bestSeenSolution =
	unique_ptr<Solution>(new Solution());

JobSchedulingGA::JobSchedulingGA(ifstream &fin,
								 CrossoverType _crossoverType,
								 MemoryType _memoryType) {
	int j, tmp;

	fin >> resultTime;
	fin >> numProcessors;
	fin >> numProcesses;
	fin >> numConnections;

	population.reset(new Population(_memoryType));

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
	memoryVector.reset(new MemoryVector(_memoryType));

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
JobSchedulingGA::~JobSchedulingGA() {
	initProcesses.clear();
}

double JobSchedulingGA::getBestSeenSurvivalValue() {
	return bestSeenSurvivalValue;
}
void JobSchedulingGA::setBestSeenSurvivalValue(double bestSeenSurvivalValue) {
	JobSchedulingGA::bestSeenSurvivalValue = bestSeenSurvivalValue;
}
unique_ptr<Solution> &JobSchedulingGA::getBestSeenSolution() {
	return bestSeenSolution;
}
void JobSchedulingGA::setBestSeenSolution(const Solution &solution) {
	JobSchedulingGA::bestSeenSolution.reset(new Solution(solution));
}

void JobSchedulingGA::generatePopulation() {
	population->generate();
}
void JobSchedulingGA::countSurvivalValues() {
	population->countSurvivalValues(initProcesses);
}
void JobSchedulingGA::selection() {
	int maxIndex, index, limit, nSel1, nBestExists, l;
	double sumOfSurvivalValues, survivalValue, averageValue, maxValue;
	auto solutions = population->getSolutions();
	vector<Solution> newSolutions(NUM_SOLUTIONS);
	unique_ptr<MemoryVector> newMemoryVector;
	if (memoryType != NONE) {
		newMemoryVector.reset(new MemoryVector(memoryType));
	}

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
	averageValue = sumOfSurvivalValues / NUM_SOLUTIONS;

	population->setBestSurvivalValue(maxValue);
	double curMaxValue = getBestSeenSurvivalValue();
	if (maxValue > curMaxValue) {
		setBestSeenSurvivalValue(maxValue);
		setBestSeenSolution(solutions[maxIndex]);
	}

	nBestExists = 0;
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		if (fabs(solutions[i].getSurvivalValue() - maxValue) < 1e-6) {
			nBestExists++;
		}
	}

	nSel1 = 0;
	l = 0;
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		limit = solutions[i].getSurvivalValue() / averageValue;
		for (int j = 0; j < limit; j++) {
			newSolutions.push_back(solutions[i]);
			if (newMemoryVector) {
				newMemoryVector->setMutRow(l++, memoryVector->getMutRow(i));
			}
		}
		nSel1 += limit;
	}

	if (nSel1 == NUM_SOLUTIONS) {
		// set (SELECTION_N_BEST - nBestExists) worst values to maxValue
		int nLeft = SELECTION_N_BEST - nBestExists;
		for (int i = NUM_SOLUTIONS - nLeft - 1; i < NUM_SOLUTIONS; i++) {
			newSolutions[i] = solutions[maxIndex];
			if (newMemoryVector) {
				newMemoryVector->setMutRow(i, memoryVector->getMutRow(maxIndex));
			}
		}
	} else {
		// add (NUM_SOLUTIONS - nSel1 - SELECTION_N_BEST + nBestExists)
		// with the roulette method
		int nLeft = NUM_SOLUTIONS - nSel1 - (SELECTION_N_BEST - nBestExists);
		vector<double> sectors = vector<double>(NUM_SOLUTIONS);
		double sum = 0.0;
		for (int i = 0; i < NUM_SOLUTIONS; i++) {
			sum += 2 * M_PI * solutions[i].getSurvivalValue() / sumOfSurvivalValues;
			sectors[i] = sum;
		}
		for (int i = 0; i < nLeft; i++) {
			double rand = Random::getRandomDouble(0, 2 * M_PI);
			index = lower_bound(sectors.begin(), sectors.end(), rand) - sectors.begin();
			newSolutions.push_back(solutions[index]);
			if (newMemoryVector) {
				newMemoryVector->setMutRow(l++, memoryVector->getMutRow(index));
			}
		}
		// add (SELECTION_N_BEST - nBestExists) maxValues
		nLeft = SELECTION_N_BEST - nBestExists;
		if (nLeft > 0) {
			for (int i = 0; i < nLeft; i++) {
				newSolutions.push_back(solutions[maxIndex]);
				if (newMemoryVector) {
					newMemoryVector->setMutRow(l++, memoryVector->getMutRow(maxIndex));
				}
			}
		}
		sectors.clear();
	}	

	population->setSolutions(newSolutions);
	memoryVector = move(newMemoryVector);
	newSolutions.clear();
}
void JobSchedulingGA::crossover() {
	useCrossoverStrategy();
}
void JobSchedulingGA::setCrossoverStrategy(unique_ptr<ICrossoverStrategy> _op) {
	operation = move(_op);
}
void JobSchedulingGA::useCrossoverStrategy() {
	operation->execute(population, memoryType, initProcesses, memoryVector);
}
void JobSchedulingGA::mutation() {
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
				prob = memoryVector->getElement(MUTATION_TASK, i, j);
			}
			if (r <= prob) {
				before = solutions[i].getSurvivalValue();
				population->mutateSolution(MUTATION_TASK, i, j);
				population->countSurvivalValue(i, initProcesses);
				after = solutions[i].getSurvivalValue();
				if (memoryType == NONE) {
					//do nothing
				} else {
					memoryVector->useChangeStrategy(MUTATION_TASK, i, j,
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
				 prob = memoryVector->getElement(MUTATION_PRIO, i, j);
			}
			if (r <= prob) {
				before = solutions[i].getSurvivalValue();
				population->mutateSolution(MUTATION_PRIO, i, j);
				population->countSurvivalValue(i, initProcesses);
				after = solutions[i].getSurvivalValue();
				if (memoryType == NONE) {
					//do nothing
				} else {
					memoryVector->useChangeStrategy(MUTATION_PRIO, i, j,
						before, after);
				}
			}
		}
	}
}
void JobSchedulingGA::printCurrentPopulation(ostream &out) {
	population->print(out);
}
double JobSchedulingGA::getResult() {
	return bestSeenSolution->getTime();
}