#ifndef POPULATION_HPP_
#define POPULATION_HPP_

#include <iostream>
#include <vector>

#include "MemoryVector.hpp"
#include "Solution.hpp"

using std::vector;
using std::ostream;

enum Operation {
	SELECTION, CROSSOVER, MUTATION
};

class Population {
protected:
	static vector<double> weights;
private:
	vector<Solution> solutions;
	double k1;
	double k2;
	double q;
	double k1Initial;
	double k2Initial;
	double qMax;
public:
	Population();
	Population(const Population& that);
	Population &operator=(Population that);
	~Population();

	void setWeights(vector<double> &weights);
	const vector<Solution>& getSolutions() const;
	void setSolutions(vector<Solution> &_solutions);
	double getK1() const;
	void setK1(double _k1);
	double getK2() const;
	void setK2(double _k2);
	double getQ() const;
	double countQab(int a, int b);
	double countQ();
	double recountQ();
	void setQ(double _q);
	void updateMacroparameters(Operation operation);

	void print(ostream &out);
	void generate();
	void crossoverSolutions(int firstIdx, int secondIdx);
	void mutateSolution(int index);
	void countSurvivalValue(int index);
	void countSurvivalValues();
	double getResult();
};

#endif /* POPULATION_HPP_ */