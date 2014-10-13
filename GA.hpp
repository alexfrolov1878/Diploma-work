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
class MicroscopicSolution;
class MicroscopicPopulation;
class MacroscopicPopulation;
class MicroscopicMemoryVector;
class MacroscopicMemoryVector;

extern int resultTime;
extern int numProcesses;
extern int numProcessors;
extern int numConnections;

extern double goal;
extern int numWeights;
extern double selectionStrength;

enum {
	NUM_SOLUTIONS = 100
};

enum AlgorithmType {
	MICROSCOPIC_STANDARD,
	MACROSCOPIC_STANDARD,
	MICROSCOPIC_WITH_MEMORY,
	MACROSCOPIC_WITH_MEMORY
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
	FORGETTING,
	MACROSCOPIC
};

const double MIN_MEM_PARAM_VALUE = 0.1;
const double MAX_MEM_PARAM_VALUE = 0.9;
const double MP = 0.05;
const double REMEMBERING_POWER = 0.5;

const double MACROSCOPIC_CROSSOVER_PROBABILITY = 0.5;
const double MACROSCOPIC_MUTATION_PROBABILITY = 0.5;
const double MACROSCOPIC_C1_CROSSOVER = 0.5;
const double MACROSCOPIC_C1_MUTATION = 0.5;
const double MACROSCOPIC_C2_CROSSOVER = 0.5;
const double MACROSCOPIC_C2_MUTATION = 0.5;
const double MACROSCOPIC_C3_CROSSOVER = 0.5;
const double MACROSCOPIC_C3_MUTATION = 0.5;
const double G_NEIGHBOURHOOD_RADIUS = 0.2;

const double MUTATION_TASK_PROBABILITY = 0.5;
const double MUTATION_PRIO_PROBABILITY = 0.5;
const double CROSSOVER_TASK_PROBABILITY = 0.5;
const double CROSSOVER_PRIO_PROBABILITY = 0.5;

const double MUTATION_TASK_POWER = 0.25;
const double MUTATION_PRIO_POWER = 0.25;

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
		unique_ptr<MicroscopicPopulation> &population,
		MemoryType memoryType, vector<Process> &initProcesses,
		unique_ptr<MicroscopicMemoryVector> &microscopicMemoryVector) = 0;
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

/*==================DERIVED LEVEL 1 CLASSES============================*/
class MicroscopicGA: public IGeneticAlgorithm, ICrossoverContext {
protected:
	static double bestSeenSurvivalValue;
	static unique_ptr<MicroscopicSolution> bestSeenSolution;

	unique_ptr<MicroscopicPopulation> population;
	vector<Process> initProcesses;
	CrossoverType crossoverType;
	MemoryType memoryType;
    unique_ptr<MicroscopicMemoryVector> microscopicMemoryVector;
public:
	MicroscopicGA(ifstream &fin, CrossoverType crossoverType,
		MemoryType memoryType = NONE);
	MicroscopicGA(const MicroscopicGA &that);
	MicroscopicGA& operator=(MicroscopicGA that);
	~MicroscopicGA();

	static double getBestSeenSurvivalValue();
	static void setBestSeenSurvivalValue(double bestSeenSurvivalValue);
	static unique_ptr<MicroscopicSolution> &getBestSeenSolution();
	static void setBestSeenSolution(const MicroscopicSolution &solution);

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

class MacroscopicGA: public IGeneticAlgorithm {
protected:
	vector<double> weights;
	unique_ptr<MacroscopicPopulation> population;
	CrossoverType crossoverType;
	MemoryType memoryType;
	unique_ptr<MacroscopicMemoryVector> macroscopicMemoryVector;
public:
	MacroscopicGA(ifstream &fin, CrossoverType crossoverType,
		MemoryType memoryType = NONE);
	MacroscopicGA(const MacroscopicGA &that);
	MacroscopicGA& operator=(MacroscopicGA that);
	~MacroscopicGA();

	virtual void generatePopulation();
	virtual void countSurvivalValues();
	virtual void selection();
	virtual void crossover();
	virtual void mutation();
	virtual void printCurrentPopulation(ostream &out);
	virtual double getResult();
};
/*=====================================================================*/

/*==========================FACTORIES==================================*/
class GeneticAlgorithmFactory {
public:
	static unique_ptr<IGeneticAlgorithm>
	newGeneticAlgorithm(const AlgorithmType &algType,
						CrossoverType crType,
						MemoryType memType,
						ifstream &fin) {
		switch (algType) {
		case MICROSCOPIC_STANDARD:
			return unique_ptr<IGeneticAlgorithm>(
				new MicroscopicGA(fin, crType)
			);
		case MACROSCOPIC_STANDARD:
			return unique_ptr<IGeneticAlgorithm>(
				new MacroscopicGA(fin, crType)
			);
		case MICROSCOPIC_WITH_MEMORY:
			return unique_ptr<IGeneticAlgorithm>(
				new MicroscopicGA(fin, crType, memType)
			);
		case MACROSCOPIC_WITH_MEMORY:
			return unique_ptr<IGeneticAlgorithm>(
				new MacroscopicGA(fin, crType, memType)
			);
		default:
			return unique_ptr<IGeneticAlgorithm>(nullptr);
		}
	}
};
/*===================================================================*/

#endif /* GA_HPP_ */
