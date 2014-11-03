#ifndef GA_HPP_
#define GA_HPP_

#include <stddef.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>

using std::unique_ptr;
using std::vector;
using std::ostream;
using std::ifstream;

class Process;
class Solution;
class Population;
class MemoryMatrix;

extern int resultTime;
extern int numProcesses;
extern int numProcessors;
extern int numConnections;

enum {
	NUM_SOLUTIONS = 100
};

enum CrossoverType {
	ONE_POINT_VECTOR,
	ONE_POINT_MATRIX_SWAPPING,
	ONE_POINT_MATRIX_COPYING,
	UNIFORM_MATRIX_SWAPPING,
	UNIFORM_MATRIX_COPYING,
};

enum MemoryType {
	NONE,
	ABSOLUTE,
	RELATIVE,
	FORGETTING
};

enum SolutionPart {
	TASK,
	PRIO
};

const int SELECTION_N_BEST = 0.05 * NUM_SOLUTIONS;

const double CROSSOVER_PROBABILITY = 0.5;
const double GOOD_CROSSOVER_PROBABILITY = 0.66;
const double BAD_CROSSOVER_PROBABILITY = 0.33;
const double MUTATION_PROBABILITY = 0.5;
const double MUTATION_POWER = 0.25;

const double MIN_MEM_PARAM_VALUE = 0.1;
const double MAX_MEM_PARAM_VALUE = 0.9;
const double MP = 0.1;
const double RELATIVE_DIFF_FACTOR = 7.5;
const double REMEMBERING_POWER = 0.5;

const double C1 = 0.7;
const double C2 = 0.3;

/*======================INTERFACES================================*/
class IGeneticAlgorithm {
public:
	virtual ~IGeneticAlgorithm() {}

	virtual void generatePopulation() = 0;
	virtual void countSurvivalValues() = 0;
	virtual void selection() = 0;
	virtual void crossover() = 0;
	virtual void mutation() = 0;
	virtual void printCurrentPopulation(ostream &out) = 0;
	virtual double getResult() = 0;
};

class ICrossoverStrategy {
public:
	virtual ~ICrossoverStrategy() {}

	virtual void execute(
		unique_ptr<Population> &population,
		MemoryType memoryType,
		vector<Process> &initProcesses,
		unique_ptr<MemoryMatrix> &mutMatr,
		unique_ptr<MemoryMatrix> &crMatr) = 0;
};

class ICrossoverContext {
protected:
    unique_ptr<ICrossoverStrategy> operation;
public:
	virtual ~ICrossoverContext() {}
	virtual void setCrossoverStrategy(unique_ptr<ICrossoverStrategy> _op) = 0;
	virtual void useCrossoverStrategy() = 0;
};
/*==============================================================*/

class JobSchedulingGA: public IGeneticAlgorithm, ICrossoverContext {
protected:
	static double bestSeenSurvivalValue;
	static unique_ptr<Solution> bestSeenSolution;

	CrossoverType crossoverType;
	MemoryType memoryType;

	unique_ptr<Population> population;
	vector<Process> initProcesses;
    unique_ptr<MemoryMatrix> mutationMatr;
    unique_ptr<MemoryMatrix> crossoverMatr;
public:
	JobSchedulingGA(ifstream &fin,
					CrossoverType crossoverType,
					MemoryType memoryType);
	JobSchedulingGA(const JobSchedulingGA &that);
	JobSchedulingGA& operator=(JobSchedulingGA that);
	~JobSchedulingGA();

	static double getBestSeenSurvivalValue();
	static void setBestSeenSurvivalValue(double bestSeenSurvivalValue);
	static unique_ptr<Solution> &getBestSeenSolution();
	static void setBestSeenSolution(const Solution &solution);

	virtual void generatePopulation();
	virtual void countSurvivalValues();
	virtual void selection();
	virtual void crossover();
	virtual void mutation();
	virtual void printCurrentPopulation(ostream &out);
	virtual double getResult();

	virtual void setCrossoverStrategy(unique_ptr<ICrossoverStrategy> _op);
	virtual void useCrossoverStrategy();
};

#endif /* GA_HPP_ */