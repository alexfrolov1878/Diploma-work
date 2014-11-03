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
using std::max;
using std::cout;
using std::endl;

double SubsetSumGA::bestSeenSurvivalValue = 0.0;
unique_ptr<Solution> SubsetSumGA::bestSeenSolution =
	unique_ptr<Solution>(new Solution());

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
	mutationMatr.reset(new MemoryMatrix(memoryType, MUTATION_PROBABILITY,
		NUM_SOLUTIONS, numWeights));
	crossoverMatr.reset(new MemoryMatrix(memoryType, CROSSOVER_PROBABILITY,
		NUM_SOLUTIONS, numWeights));

	crossoverType = _crossoverType;
	switch (crossoverType) {
		case ONE_POINT_VECTOR:
			setCrossoverStrategy(unique_ptr<ICrossoverStrategy>(
				new OnePointVectorStrategy())
			);
			break;
		case ONE_POINT_MATRIX_SWAPPING:
			setCrossoverStrategy(unique_ptr<ICrossoverStrategy>(
				new OnePointMatrixSwappingStrategy())
			);
			break;
		case ONE_POINT_MATRIX_COPYING:
			setCrossoverStrategy(unique_ptr<ICrossoverStrategy>(
				new OnePointMatrixCopyingStrategy())
			);
			break;
		case UNIFORM_MATRIX_SWAPPING:
			setCrossoverStrategy(unique_ptr<ICrossoverStrategy>(
				new UniformMatrixSwappingStrategy())
			);
			break;
		case UNIFORM_MATRIX_COPYING:
			setCrossoverStrategy(unique_ptr<ICrossoverStrategy>(
				new UniformMatrixCopyingStrategy())
			);
			break;
		default:
			setCrossoverStrategy(unique_ptr<ICrossoverStrategy>(
				new OnePointMatrixCopyingStrategy())
			);
			break;
	}
}
SubsetSumGA::~SubsetSumGA() {
	weights.clear();
}

double SubsetSumGA::getBestSeenSurvivalValue() {
	return bestSeenSurvivalValue;
}

void SubsetSumGA::setBestSeenSurvivalValue(double bestSeenSurvivalValue) {
	SubsetSumGA::bestSeenSurvivalValue = bestSeenSurvivalValue;
}

unique_ptr<Solution> &SubsetSumGA::getBestSeenSolution() {
	return bestSeenSolution;
}

void SubsetSumGA::setBestSeenSolution(const Solution &solution) {
	SubsetSumGA::bestSeenSolution.reset(new Solution(solution));
}

void SubsetSumGA::generatePopulation() {
	population->generate();
}

void SubsetSumGA::countSurvivalValues() {
	population->countSurvivalValues();
}

void SubsetSumGA::selection() {
	int maxIndex, index, limit, nSel1, nBestExists, l;
	double sumOfSurvivalValues, survivalValue, averageValue, maxValue;
	auto solutions = population->getSolutions();
	vector<Solution> newSolutions(NUM_SOLUTIONS);
	unique_ptr<MemoryMatrix> newMutMatr, newCrMatr;
	if (memoryType != NONE) {
		newMutMatr.reset(new MemoryMatrix(memoryType, MUTATION_PROBABILITY,
			NUM_SOLUTIONS, numWeights));
		newCrMatr.reset(new MemoryMatrix(memoryType, CROSSOVER_PROBABILITY,
			NUM_SOLUTIONS, numWeights));
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
			newSolutions[l] = solutions[i];
			if (memoryType != NONE) {
				newMutMatr->setRow(l, mutationMatr->getRow(i));
				newCrMatr->setRow(l, crossoverMatr->getRow(i));
			}
			l++;
		}
		nSel1 += limit;
	}

	if (nSel1 == NUM_SOLUTIONS) {
		// set (SELECTION_N_BEST - nBestExists) worst values to maxValue
		int nLeft = SELECTION_N_BEST - nBestExists;
		for (int i = NUM_SOLUTIONS - nLeft - 1; i < NUM_SOLUTIONS; i++) {
			newSolutions[i] = solutions[maxIndex];
			if (memoryType != NONE) {
				newMutMatr->setRow(i, mutationMatr->getRow(maxIndex));
				newCrMatr->setRow(i, crossoverMatr->getRow(maxIndex));
			}
		}
	} else {
		// add (NUM_SOLUTIONS - nSel1 - SELECTION_N_BEST + nBestExists)
		// with the roulette method
		int nLeftBest = max(0, SELECTION_N_BEST - nBestExists);
		int nLeft = NUM_SOLUTIONS - nSel1 - nLeftBest;
		vector<double> sectors = vector<double>(NUM_SOLUTIONS);
		double sum = 0.0;
		for (int i = 0; i < NUM_SOLUTIONS; i++) {
			sum += 2 * M_PI * solutions[i].getSurvivalValue() / sumOfSurvivalValues;
			sectors[i] = sum;
		}
		for (int i = 0; i < nLeft; i++) {
			double rand = Random::getRandomDouble(0, 2 * M_PI);
			index = lower_bound(sectors.begin(), sectors.end(), rand) - sectors.begin();
			newSolutions[l] = solutions[index];
			if (memoryType != NONE) {
				newMutMatr->setRow(l, mutationMatr->getRow(index));
				newCrMatr->setRow(l, crossoverMatr->getRow(index));
			}
			l++;
		}
		// add (SELECTION_N_BEST - nBestExists) maxValues
		if (nLeftBest > 0) {
			for (int i = 0; i < nLeftBest; i++) {
				newSolutions[l] = solutions[maxIndex];
				if (memoryType != NONE) {
					newMutMatr->setRow(l, mutationMatr->getRow(maxIndex));
					newCrMatr->setRow(l, crossoverMatr->getRow(maxIndex));
				}
				l++;
			}
		}
		sectors.clear();
	}

	population->setSolutions(newSolutions);
	mutationMatr = move(newMutMatr);
	crossoverMatr = move(newCrMatr);
	newSolutions.clear();
}

void SubsetSumGA::crossover() {
	useCrossoverStrategy();
}

void SubsetSumGA::setCrossoverStrategy(unique_ptr<ICrossoverStrategy> _op) {
	operation = move(_op);
}

void SubsetSumGA::useCrossoverStrategy() {
	operation->execute(population, memoryType, mutationMatr, crossoverMatr);
}

void SubsetSumGA::mutation() {
	double r;
	double prob, before, after;
	auto &solutions = population->getSolutions();

	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		for (int j = 0; j < numWeights; j++) {
			r = Random::getRandomDouble(0, 1);
			if (memoryType == NONE) {
				prob = MUTATION_PROBABILITY;
			} else {
				prob = mutationMatr->getElement(i, j);
			}
			if (r <= prob) {
				before = solutions[i].getSurvivalValue();
				population->mutateSolution(i, j);
				population->countSurvivalValue(i);
				after = solutions[i].getSurvivalValue();
				if (memoryType == NONE) {
					//do nothing
				} else {
					mutationMatr->useChangeStrategy(i, j, j + 1, after, before);
				}
			}
		}
	}
}

void SubsetSumGA::printCurrentPopulation(ostream &out) {
	population->print(out);
}

double SubsetSumGA::getResult() {
	return bestSeenSolution->getField();
}