#ifndef POPULATION_HPP_
#define POPULATION_HPP_

#include <iostream>
#include <vector>

#include "MemoryVector.hpp"
#include "Solution.hpp"

using std::vector;
using std::ostream;

enum MacroscopicOperation {
	SELECTION, CROSSOVER, MUTATION
};

/*=============================MICROSCOPIC===================================*/

class MicroscopicPopulation {
private:
	vector<MicroscopicSolution> solutions;
	double bestSurvivalValue;
	int bestSolutionIdx;
public:
	MicroscopicPopulation(double value = 0.0, int idx = 0);
	MicroscopicPopulation(const MicroscopicPopulation& that);
	MicroscopicPopulation& operator=(MicroscopicPopulation that);
	~MicroscopicPopulation();

	const vector<MicroscopicSolution> &getSolutions() const;
	void setSolutions(vector<MicroscopicSolution> &_solutions);
	int getBestSolutionIdx() const;
	void setBestSolutionIdx(int _bestSolutionIdx);
	double getBestSurvivalValue() const;
	void setBestSurvivalValue(double _bestSurvivalValue);

	void print(ostream &out);
	void generate();
	int crossoverSolutions(SolutionPart part, int firstIdx, int secondIdx);
	void mutateSolution(SolutionPart part, int index, int offset);
	void countSurvivalValue(int index, vector<Process> &initProcesses);
	void countSurvivalValues(vector<Process> &initProcesses);
};

/*=============================MACROSCOPIC===================================*/

class MacroscopicPopulation {
protected:
	static vector<double> weights;
private:
	vector<MacroscopicSolution> solutions;
	double k1;
	double k2;
	double q;
	double k1Initial;
	double k2Initial;
	double qMax;
public:
	MacroscopicPopulation();
	MacroscopicPopulation(const MacroscopicPopulation& that);
	~MacroscopicPopulation();

	void setWeights(vector<double> &weights);
	const vector<MacroscopicSolution>& getSolutions() const;
	void setSolutions(vector<MacroscopicSolution> &_solutions);
	double getK1() const;
	void setK1(double _k1);
	double getK2() const;
	void setK2(double _k2);
	double getQ() const;
	double countQab(int a, int b);
	double countQ();
	double recountQ();
	void setQ(double _q);
	void updateMacroparameters(MacroscopicOperation operation);

	void print(ostream &out);
	void generate();
	void crossoverSolutions(int firstIdx, int secondIdx);
	void mutateSolution(int index);
	void countSurvivalValue(int index);
	void countSurvivalValues();
	double getResult();
};

#endif /* POPULATION_HPP_ */
