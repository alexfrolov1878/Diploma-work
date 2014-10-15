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
class MemoryVector;

extern int resultTime;
extern int numProcesses;
extern int numProcessors;
extern int numConnections;

enum {
	NUM_SOLUTIONS = 100
};

enum CrossoverType {
	ONE_POINT_VECTOR,
	ONE_POINT_MATRIX_THROWING,
	ONE_POINT_MATRIX_SWAPPING,
	UNIFORM_MATRIX_THROWING,
	UNIFORM_MATRIX_SWAPPING
};

enum MemoryType {
	NONE,
	ABSOLUTE,
	RELATIVE,
	FORGETTING
};

const double MUTATION_TASK_PROBABILITY = 0.5;
const double MUTATION_PRIO_PROBABILITY = 0.5;
const double CROSSOVER_TASK_PROBABILITY = 0.5;
const double CROSSOVER_PRIO_PROBABILITY = 0.5;
const double MUTATION_TASK_POWER = 0.25;
const double MUTATION_PRIO_POWER = 0.25;

const double MIN_MEM_PARAM_VALUE = 0.1;
const double MAX_MEM_PARAM_VALUE = 0.9;
const double MP = 0.05;
const double REMEMBERING_POWER = 0.5;

const double C1 = 0.3;
const double C2 = 0.7;

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
		unique_ptr<MemoryVector> &memoryVector) = 0;
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
    unique_ptr<MemoryVector> memoryVector;
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
	virtual void setCrossoverStrategy(unique_ptr<ICrossoverStrategy> _op);
	virtual void useCrossoverStrategy();
	virtual void crossover();
	virtual void mutation();
	virtual void printCurrentPopulation(ostream &out);
	virtual double getResult();
};

#endif /* GA_HPP_ */