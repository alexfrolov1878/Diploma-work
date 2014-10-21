#include <stddef.h>
#include <cmath>

#include "Population.hpp"
#include "GA.hpp"
#include "main.hpp"

using std::vector;
using std::ostream;
using std::endl;
using std::exp;

Population::Population(double value, int idx) {
	solutions = vector<Solution>(NUM_SOLUTIONS);
	bestSurvivalValue = value;
	bestSolutionIdx = idx;
}
Population::Population(const Population& that) {
	solutions = that.solutions;
	bestSurvivalValue = that.bestSurvivalValue;
	bestSolutionIdx = that.bestSolutionIdx;
}
Population& Population::operator=(Population that) {
	solutions = that.solutions;
	bestSurvivalValue = that.bestSurvivalValue;
	bestSolutionIdx = that.bestSolutionIdx;
	return *this;
}
Population::~Population() {
	solutions.clear();
}

const vector<Solution> &Population::getSolutions() const {
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
		solutions[i].print(out);
	}
}
void Population::generate() {
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		solutions[i].generate();
	}
	bestSurvivalValue = 0.0;
	bestSolutionIdx = 0;
}
int Population::crossoverSolutions(SolutionPart part,
		int firstIdx, int secondIdx) {
	int r, tmp1, tmp2;
	vector<int> first;
	vector<int> second;
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
void Population::mutateSolution(SolutionPart part, int index, int offset) {
	solutions[index].mutate(part, offset);
}
void Population::countSurvivalValue(int index, vector<Process> &initProcesses) {
	double Kt, Kr, value;
	solutions[index].buildSchedule(initProcesses);
	Kt = 1.0 - 1.0 * solutions[index].getTime()
			/ solutions[index].getSumOfComputationalComplexity();
	Kr = 1.0 - 1.0 * solutions[index].getDowntime() / numProcessors;
	value = C1 * Kt + C2 * Kr;
	solutions[index].setSurvivalValue(value);
}
void Population::countSurvivalValues(vector<Process> &initProcesses) {
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		countSurvivalValue(i, initProcesses);
	}
}