#ifndef CROSSOVER_STRATEGY_HPP_
#define CROSSOVER_STRATEGY_HPP_

#include <stddef.h>
#include <vector>
#include <memory>

#include "../GA.hpp"
#include "main.hpp"

using std::unique_ptr;
using std::vector;

class Process;
class MicroscopicSolution;
class MicroscopicPopulation;
class MacroscopicPopulation;
class MicroscopicMemoryVector;
class MacroscopicMemoryVector;

class OnePointVectorStrategy : public ICrossoverStrategy {
public:
	virtual ~OnePointVectorStrategy() {}
	virtual void execute(
		unique_ptr<MicroscopicPopulation> &population,
		MemoryType memoryType, vector<Process> &initProcesses,
		unique_ptr<MicroscopicMemoryVector> &microscopicMemoryVector);
};

class OnePointMatrixThrowingStrategy : public ICrossoverStrategy {
public:
	virtual ~OnePointMatrixThrowingStrategy() {}
	virtual void execute(
		unique_ptr<MicroscopicPopulation> &population,
		MemoryType memoryType, vector<Process> &initProcesses,
		unique_ptr<MicroscopicMemoryVector> &microscopicMemoryVector);
};

class OnePointMatrixSwappingStrategy : public ICrossoverStrategy {
public:
	virtual ~OnePointMatrixSwappingStrategy() {}
	virtual void execute(
		unique_ptr<MicroscopicPopulation> &population,
		MemoryType memoryType, vector<Process> &initProcesses,
		unique_ptr<MicroscopicMemoryVector> &microscopicMemoryVector);
};

class UniformMatrixThrowingStrategy : public ICrossoverStrategy {
public:
	virtual ~UniformMatrixThrowingStrategy() {}
	virtual void execute(
		unique_ptr<MicroscopicPopulation> &population,
		MemoryType memoryType, vector<Process> &initProcesses,
		unique_ptr<MicroscopicMemoryVector> &microscopicMemoryVector);
};

class UniformMatrixSwappingStrategy : public ICrossoverStrategy {
public:
	virtual ~UniformMatrixSwappingStrategy() {}
	virtual void execute(
		unique_ptr<MicroscopicPopulation> &population,
		MemoryType memoryType, vector<Process> &initProcesses,
		unique_ptr<MicroscopicMemoryVector> &microscopicMemoryVector);
};

#endif /* CROSSOVER_STRATEGY_HPP_ */
