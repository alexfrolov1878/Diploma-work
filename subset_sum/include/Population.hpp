#ifndef POPULATION_HPP_
#define POPULATION_HPP_

#include <iostream>
#include <vector>

#include "MemoryMatrix.hpp"
#include "Solution.hpp"

using std::vector;
using std::ostream;

class Population {
protected:
	static vector<double> weights;
private:
	vector<Solution> solutions;
	double bestSurvivalValue;
	int bestSolutionIdx;
public:
	Population();
	Population(const Population& that);
	Population &operator=(Population that);
	~Population();

	void setWeights(vector<double> &weights);
	vector<Solution>& getSolutions();
	void setSolutions(vector<Solution> &_solutions);
	int getBestSolutionIdx() const;
	void setBestSolutionIdx(int _bestSolutionIdx);
	double getBestSurvivalValue() const;
	void setBestSurvivalValue(double _bestSurvivalValue);

	void print(ostream &out);
	void generate();
	void mutateSolution(int index, int offset);
	void countSurvivalValue(int index);
	void countSurvivalValues();
	double getResult();
};

#endif /* POPULATION_HPP_ */