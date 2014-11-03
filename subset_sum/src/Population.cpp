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
	solutions = vector<Solution>(NUM_SOLUTIONS);
}

Population::Population(const Population& that) {
	solutions = that.solutions;
}

Population& Population::operator=(Population that) {
	solutions = that.solutions;
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

int Population::getBestSolutionIdx() const {
	return bestSolutionIdx;
}

void Population::setBestSolutionIdx(int _bestSolutionIdx) {
	bestSolutionIdx = _bestSolutionIdx;
}

double Population::getBestSurvivalValue() const {
	return bestSurvivalValue;
}

void Population::setBestSurvivalValue(double _bestSurvivalValue) {
	bestSurvivalValue = _bestSurvivalValue;
}

void Population::print(ostream &out) {
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		out << "	Solution " << i + 1 << endl;
		for (int j = 0; j < numWeights; j++) {
			solutions[i].print(out);
		}
	}
}

void Population::generate() {
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		solutions[i].generate();
	}
}

void Population::mutateSolution(int index, int offset) {
	solutions[index].mutate(offset);
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