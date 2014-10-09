#ifndef POPULATION_HPP_
#define POPULATION_HPP_

#include <iostream>
#include <vector>

#include "MemoryVector.hpp"
#include "Solution.hpp"

enum MacroscopicOperation {
	SELECTION, CROSSOVER, MUTATION
};

/*=============================MICROSCOPIC===================================*/

class MicroscopicPopulation {
private:
	std::vector<MicroscopicSolution> solutions;
	double bestSurvivalValue;
	int bestSolutionIdx;
public:
	MicroscopicPopulation(double value = 0.0, int idx = 0);
	MicroscopicPopulation(const MicroscopicPopulation& that);
	MicroscopicPopulation& operator=(MicroscopicPopulation that);
	~MicroscopicPopulation();

	const std::vector<MicroscopicSolution> &getSolutions() const;
	void setSolutions(std::vector<MicroscopicSolution> &solutions);
	int getBestSolutionIdx() const;
	void setBestSolutionIdx(int bestSolutionIdx);
	double getBestSurvivalValue() const;
	void setBestSurvivalValue(double bestSurvivalValue);

	void print(std::ostream &out);
	void generate();
	int crossoverSolutions(SolutionPart part, int firstIdx, int secondIdx);
	void mutateSolution(SolutionPart part, int index, int offset);
	void countSurvivalValue(int index, Process *initProcesses);
	void countSurvivalValues(Process *init_processes);
};

/*=============================MACROSCOPIC===================================*/

class MacroscopicPopulation {
protected:
	static double selectionStrength;
	static std::vector<double> weights;
private:
	std::vector<MacroscopicSolution> solutions;
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

	double getSelectionStrength() const;
	void setSelectionStrength(double selectionStrength);
	void setWeights(std::vector<double> &weights);
	const std::vector<MacroscopicSolution>& getSolutions() const;
	void setSolutions(std::vector<MacroscopicSolution> &solutions);
	double getK1() const;
	void setK1(double k1);
	double getK2() const;
	void setK2(double k2);
	double getQ() const;
	double countQab(int a, int b);
	double countQ();
	double recountQ();
	void setQ(double q);
	void updateMacroparameters(MacroscopicOperation operation);

	void print(std::ostream &out);
	void generate();
	void crossoverSolutions(int firstIdx, int secondIdx);
	void mutateSolution(int index);
	void countSurvivalValue(int index);
	void countSurvivalValues();
	double getResult();
};

#endif /* POPULATION_HPP_ */
