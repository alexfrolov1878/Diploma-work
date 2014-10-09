#ifndef GA_HPP_
#define GA_HPP_

#include <stddef.h>
#include <fstream>
#include <iostream>
#include <vector>

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

enum {
	NUM_SOLUTIONS = 100
};

enum AlgorithmType {
	MICROSCOPIC_STANDARD,
	MACROSCOPIC_STANDARD,
	MICROSCOPIC_WITH_MEMORY,
	MACROSCOPIC_WITH_MEMORY
};

enum MemoryType {
	NONE, ABSOLUTE, RELATIVE, FORGETTING, MACROSCOPIC
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

/*======================INTERFACE=================================*/
class IGeneticAlgorithm {
public:
	virtual ~IGeneticAlgorithm() {
	}

	virtual void generatePopulation() = 0;
	virtual void countSurvivalValues() = 0;
	virtual void selection() = 0;
	virtual void crossover() = 0;
	virtual void mutation() = 0;
	virtual void printCurrentPopulation(std::ostream &out) = 0;
	virtual double getResult() = 0;
};
/*================================================================*/

/*==================DERIVED LEVEL 1 CLASSES============================*/
class MicroscopicGA: public IGeneticAlgorithm {
protected:
	static double bestSeenSurvivalValue;
	static MicroscopicSolution *bestSeenSolution;

	int resultTime;
	int numProcessors;
	int numProcesses;
	int numConnections;
	MicroscopicPopulation *population;
	Process *initProcesses;

	MemoryType memoryType;
        MicroscopicMemoryVector *microscopicMemoryVector;
public:
	MicroscopicGA(std::ifstream &fin, MemoryType memoryType = NONE);
	MicroscopicGA(const MicroscopicGA &that);
	MicroscopicGA& operator=(MicroscopicGA that);
	~MicroscopicGA();

	static double getBestSeenSurvivalValue();
	static void setBestSeenSurvivalValue(double bestSeenSurvivalValue);
	static MicroscopicSolution *getBestSeenSolution();
	static void setBestSeenSolution(const MicroscopicSolution &solution);

	virtual void generatePopulation();
	virtual void countSurvivalValues();
	virtual void selection();
	virtual void crossover();
	virtual void mutation();
	virtual void printCurrentPopulation(std::ostream &out);
	virtual double getResult();
};

class MacroscopicGA: public IGeneticAlgorithm {
protected:
	double goal;
	double selectionStrength;
	int numWeights;
	std::vector<double> weights;
	MacroscopicPopulation *population;

	MemoryType memoryType;
	MacroscopicMemoryVector *macroscopicMemoryVector;
public:
	MacroscopicGA(std::ifstream &fin, MemoryType memoryType = NONE);
	MacroscopicGA(const MacroscopicGA &that);
	MacroscopicGA& operator=(MacroscopicGA that);
	~MacroscopicGA();

	virtual void generatePopulation();
	virtual void countSurvivalValues();
	virtual void selection();
	virtual void crossover();
	virtual void mutation();
	virtual void printCurrentPopulation(std::ostream &out);
	virtual double getResult();
};
/*=====================================================================*/

/*==========================FACTORY==================================*/
class GeneticAlgorithmFactory {
public:
	static IGeneticAlgorithm *newGeneticAlgorithm(const AlgorithmType &type,
			MemoryType memoryType, std::ifstream &fin) {
		switch (type) {
		case MICROSCOPIC_STANDARD:
			return new MicroscopicGA(fin);
		case MACROSCOPIC_STANDARD:
			return new MacroscopicGA(fin);
		case MICROSCOPIC_WITH_MEMORY:
			return new MicroscopicGA(fin, memoryType);
		case MACROSCOPIC_WITH_MEMORY:
			return new MacroscopicGA(fin, memoryType);
		default:
			return NULL;
		}
	}
};
/*===================================================================*/

#endif /* GA_HPP_ */
