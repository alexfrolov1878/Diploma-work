#ifndef POPULATION_HPP_
#define POPULATION_HPP_

#include <iostream>
#include <vector>

#include "MemoryMatrix.hpp"
#include "Solution.hpp"

using std::vector;
using std::ostream;

class Population {
private:
	vector<Solution> solutions;
	double bestSurvivalValue;
	int bestSolutionIdx;
public:
	Population(double value = 0.0, int idx = 0);
	Population(const Population& that);
	Population &operator=(Population that);
	~Population();

	vector<Solution> &getSolutions();
	void setSolutions(vector<Solution> &_solutions);
	int getBestSolutionIdx() const;
	void setBestSolutionIdx(int _bestSolutionIdx);
	double getBestSurvivalValue() const;
	void setBestSurvivalValue(double _bestSurvivalValue);

	void print(ostream &out);
	void generate();
	void mutateSolution(SolutionPart part, int index, int offset);
	void countSurvivalValue(int index, vector<Process> &initProcesses);
	void countSurvivalValues(vector<Process> &initProcesses);
};

#endif /* POPULATION_HPP_ */