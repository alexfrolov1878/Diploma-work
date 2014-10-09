#include "../GA.hpp"
#include "Population.hpp"

#include <stddef.h>
#include <cmath>

#include "main.hpp"

/*=============================MICROSCOPIC===================================*/
MicroscopicPopulation::MicroscopicPopulation(double value, int idx) {
	solutions.reserve(NUM_SOLUTIONS);
	MicroscopicSolution newSolution;
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		solutions.push_back(newSolution);
	}
	bestSurvivalValue = value;
	bestSolutionIdx = idx;
}
MicroscopicPopulation::MicroscopicPopulation(
		const MicroscopicPopulation& that) {
	solutions = that.solutions;
	bestSurvivalValue = that.bestSurvivalValue;
	bestSolutionIdx = that.bestSolutionIdx;
}
MicroscopicPopulation& MicroscopicPopulation::operator=(
                MicroscopicPopulation that) {
        solutions = that.solutions;
        bestSurvivalValue = that.bestSurvivalValue;
        bestSolutionIdx = that.bestSolutionIdx;
        return *this;
}
MicroscopicPopulation::~MicroscopicPopulation() {
	solutions.clear();
}

const std::vector<MicroscopicSolution> &MicroscopicPopulation::getSolutions() const {
	return solutions;
}
void MicroscopicPopulation::setSolutions(
		std::vector<MicroscopicSolution> &solutions) {
	this->solutions.swap(solutions);
}
int MicroscopicPopulation::getBestSolutionIdx() const {
	return bestSolutionIdx;
}
void MicroscopicPopulation::setBestSolutionIdx(int bestSolutionIdx) {
	this->bestSolutionIdx = bestSolutionIdx;
}
double MicroscopicPopulation::getBestSurvivalValue() const {
	return bestSurvivalValue;
}
void MicroscopicPopulation::setBestSurvivalValue(double bestSurvivalValue) {
	this->bestSurvivalValue = bestSurvivalValue;
}

void MicroscopicPopulation::print(std::ostream &out) {
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		out << "	Solution " << i + 1 << std::endl;
		solutions[i].print(out);
	}
}
void MicroscopicPopulation::generate() {
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		solutions[i].generate();
	}
	bestSurvivalValue = 0.0;
	bestSolutionIdx = 0;
}
int MicroscopicPopulation::crossoverSolutions(SolutionPart part, int firstIdx,
		int secondIdx) {
	int r, tmp1, tmp2;
        std::vector<int> first;
        std::vector<int> second;
        if (part == CROSSOVER_TASK) {
                first = solutions[firstIdx].getTasks();
                second = solutions[secondIdx].getTasks();
        } else if (part == CROSSOVER_PRIO) {
                first = solutions[firstIdx].getPriorities();
                second = solutions[secondIdx].getPriorities();
        } else {
                return -1;
        }

        r = Random::getRandomInt(0, numProcesses);
        for (int i = r + 1; i < numProcesses; i++) {
                tmp1 = first[i];
                tmp2 = second[i];
                first[i] = tmp2;
                second[i] = tmp1;
        }
        return r + 1;
}
void MicroscopicPopulation::mutateSolution(SolutionPart part, int index,
		int offset) {
	solutions[index].mutate(part, offset);
}
void MicroscopicPopulation::countSurvivalValue(int index,
		Process *initProcesses) {
	double Kt, Kr, value;
	solutions[index].buildSchedule(initProcesses);
	Kt = 1.0
			- 1.0 * solutions[index].getTime()
					/ solutions[index].getSumOfComputationalComplexity();
	Kr = 1.0 - 1.0 * solutions[index].getDowntime() / numProcessors;
	value = C1 * Kt + C2 * Kr;
	solutions[index].setSurvivalValue(value);
}
void MicroscopicPopulation::countSurvivalValues(Process *initProcesses) {
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		countSurvivalValue(i, initProcesses);
	}
}
/*===========================================================================*/
/*=============================MACROSCOPIC===================================*/
double MacroscopicPopulation::selectionStrength = 0.0;
std::vector<double> MacroscopicPopulation::weights = std::vector<double>();
MacroscopicPopulation::MacroscopicPopulation() {
	k1 = 0.0;
	k2 = 0.0;
	q = 0.0;
	qMax = 0.0;
	k1Initial = 0.0;
	k2Initial = 0.0;
	solutions = std::vector<MacroscopicSolution>(NUM_SOLUTIONS);
}
MacroscopicPopulation::MacroscopicPopulation(
		const MacroscopicPopulation& that) {
	solutions = that.solutions;
	k1 = that.k1;
	k2 = that.k2;
	q = that.q;
	k1Initial = that.k1Initial;
	k2Initial = that.k2Initial;
	qMax = that.qMax;
}
MacroscopicPopulation::~MacroscopicPopulation() {
	solutions.clear();
}

double MacroscopicPopulation::getSelectionStrength() const {
	return selectionStrength;
}
void MacroscopicPopulation::setSelectionStrength(double selectionStrength) {
	MacroscopicPopulation::selectionStrength = selectionStrength;
}
void MacroscopicPopulation::setWeights(std::vector<double> &weights) {
	MacroscopicPopulation::weights.swap(weights);
}
const std::vector<MacroscopicSolution>& MacroscopicPopulation::getSolutions() const {
	return solutions;
}
void MacroscopicPopulation::setSolutions(
		std::vector<MacroscopicSolution> &solutions) {
	this->solutions.swap(solutions);
}
double MacroscopicPopulation::getK1() const {
	return k1;
}
void MacroscopicPopulation::setK1(double k1) {
	this->k1 = k1;
}
double MacroscopicPopulation::getK2() const {
	return k2;
}
void MacroscopicPopulation::setK2(double k2) {
	this->k2 = k2;
}
double MacroscopicPopulation::getQ() const {
	return q;
}
double MacroscopicPopulation::countQab(int a, int b) {
	double Qab = 0.0;
	for (int i = 0; i < numWeights; i++) {
		Qab += weights[i] * weights[i] * (2 * solutions[a].getIndicator(i) - 1)
				* (2 * solutions[b].getIndicator(i) - 1);
	}
	return Qab / 4 / numWeights;
}
double MacroscopicPopulation::countQ() {
	double result = 0.0;
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		for (int j = 0; j < NUM_SOLUTIONS; j++) {
			if (i == j) {
				continue;
			}
			result += countQab(i, j);
		}
	}
	return result / NUM_SOLUTIONS / (NUM_SOLUTIONS - 1);
}
double MacroscopicPopulation::recountQ() {
	double sum = 0.0;
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		sum += solutions[i].getSurvivalValue();
	}
	double pa[NUM_SOLUTIONS];
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pa[i] = solutions[i].getSurvivalValue() / sum;
	}
	double result = 0.0;
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		result += pa[i] * pa[i] * qMax;
	}
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		for (int j = 0; j < NUM_SOLUTIONS; j++) {
			if (i == j) {
				continue;
			}
			result += pa[i] * pa[j] * countQab(i, j);
		}
	}
	return result;
}
void MacroscopicPopulation::setQ(double q) {
	this->q = q;
}
void MacroscopicPopulation::updateMacroparameters(
		MacroscopicOperation operation) {
	double value1, value2;
	double G = goal;
	double N = numWeights;
	double b = selectionStrength;
	double pc = MACROSCOPIC_CROSSOVER_PROBABILITY;
	double pm = MACROSCOPIC_MUTATION_PROBABILITY;
	switch (operation) {
	case SELECTION:
		value1 = 2 * b * k2 / N;
		value2 = (1 - 1.0 / NUM_SOLUTIONS);
		k1 += value1 * value2 * (G - k1);
		k2 = value2 * k2 - value1 * (1 - 3.0 / NUM_SOLUTIONS) * k2;
		q = recountQ();
		break;
	case CROSSOVER:
		k2 += 2 * pc * (1 - pc)
				* ((1 - 1.0 / NUM_SOLUTIONS) * N * (qMax - q) - k2);
		break;
	case MUTATION:
		k1 += 2 * pm * (k1Initial - k1);
		k2 += 4 * pm * (1 - pm) * (k2Initial - k2);
		q = (1 - 2 * pm) * (1 - 2 * pm) * q;
		break;
	default:
		break;
	}
}

void MacroscopicPopulation::print(std::ostream &out) {
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		out << "	Solution " << i + 1 << std::endl;
		for (int j = 0; j < numProcesses; j++) {
			solutions[i].print(out);
		}
	}
}
void MacroscopicPopulation::generate() {
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		solutions[i].generate();
	}

	double sum1 = 0.0, sum2 = 0.0;
	for (int i = 0; i < numWeights; i++) {
		sum1 += weights[i];
		sum2 += weights[i] * weights[i];
	}

	k1 = sum1 / 2;
	k2 = (1 - 1.0 / NUM_SOLUTIONS) * sum2 / 4;
	q = countQ();
	qMax = sum2 / 4 / numWeights;
	k1Initial = k1;
	k2Initial = k2;
}
void MacroscopicPopulation::crossoverSolutions(int firstIdx, int secondIdx) {
	solutions[firstIdx].crossover(solutions[secondIdx]);
}
void MacroscopicPopulation::mutateSolution(int index) {
	solutions[index].mutate();
}
void MacroscopicPopulation::countSurvivalValue(int index) {
	solutions[index].buildField(weights);
	double N = numWeights;
	double G = goal;
	double h = solutions[index].getField();
	double energy = (h - G) * (h - G) / N;
	double value = std::exp(-1.0 * MacroscopicPopulation::selectionStrength * energy);
	solutions[index].setSurvivalValue(value);
}
void MacroscopicPopulation::countSurvivalValues() {
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		countSurvivalValue(i);
	}
}
double MacroscopicPopulation::getResult() {
	double N = numWeights;
	double G = goal;
	double h, energy, result;
	double min = N + 1;
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		h = solutions[i].getField();
		energy = (h - G) * (h - G) / N;
		if (energy < min) {
			min = energy;
			result = h;
		}
	}
	return result;
}
/*===========================================================================*/
