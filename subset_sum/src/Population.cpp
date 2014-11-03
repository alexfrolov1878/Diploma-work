#include <stddef.h>
#include <cmath>

#include "Population.hpp"
#include "GA.hpp"
#include "main.hpp"

using std::vector;
using std::ostream;
using std::endl;
using std::exp;

vector<double> Population::weights = vector<double>(numWeights, 0.0);

Population::Population() {
	k1 = 0.0;
	k2 = 0.0;
	q = 0.0;
	qMax = 0.0;
	k1Initial = 0.0;
	k2Initial = 0.0;
	solutions = vector<Solution>(NUM_SOLUTIONS);
}

Population::Population(const Population& that) {
	solutions = that.solutions;
	k1 = that.k1;
	k2 = that.k2;
	q = that.q;
	k1Initial = that.k1Initial;
	k2Initial = that.k2Initial;
	qMax = that.qMax;
}

Population& Population::operator=(Population that) {
	solutions = that.solutions;
	k1 = that.k1;
	k2 = that.k2;
	q = that.q;
	k1Initial = that.k1Initial;
	k2Initial = that.k2Initial;
	qMax = that.qMax;
	return *this;
}

Population::~Population() {
	solutions.clear();
}

void Population::setWeights(vector<double> &weights) {
	Population::weights.swap(weights);
}

vector<Solution>& Population::getSolutions() {
	return solutions;
}

void Population::setSolutions(vector<Solution> &_solutions) {
	solutions.swap(_solutions);
}

double Population::getK1() const {
	return k1;
}

void Population::setK1(double _k1) {
	k1 = _k1;
}

double Population::getK2() const {
	return k2;
}

void Population::setK2(double _k2) {
	k2 = _k2;
}

double Population::getQ() const {
	return q;
}

double Population::countQab(int a, int b) {
	double Qab = 0.0;
	for (int i = 0; i < numWeights; i++) {
		Qab += weights[i] * weights[i] *
				(2 * solutions[a].getIndicator(i) - 1) *
				(2 * solutions[b].getIndicator(i) - 1);
	}
	return Qab / 4.0 / numWeights;
}

double Population::countQ() {
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

double Population::recountQ() {
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

void Population::setQ(double _q) {
	q = _q;
}

void Population::updateMacroparameters(Operation operation) {
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

void Population::print(ostream &out) {
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		out << "	Solution " << i + 1 << endl;
		for (int j = 0; j < numProcesses; j++) {
			solutions[i].print(out);
		}
	}
}

void Population::generate() {
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

void Population::crossoverSolutions(int firstIdx, int secondIdx) {
	solutions[firstIdx].crossover(solutions[secondIdx]);
}

void Population::mutateSolution(int index) {
	solutions[index].mutate();
}

void Population::countSurvivalValue(int index) {
	solutions[index].buildField(weights);
	double N = numWeights;
	double G = goal;
	double h = solutions[index].getField();
	double energy = (h - G) * (h - G) / N;
	double value = exp(-1.0 * selectionStrength * energy);
	solutions[index].setSurvivalValue(value);
}

void Population::countSurvivalValues() {
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		countSurvivalValue(i);
	}
}

double Population::getResult() {
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
