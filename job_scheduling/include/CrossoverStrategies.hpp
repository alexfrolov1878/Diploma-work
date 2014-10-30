#ifndef CROSSOVER_STRATEGY_HPP_
#define CROSSOVER_STRATEGY_HPP_

#include <stddef.h>
#include <vector>
#include <memory>

#include "GA.hpp"
#include "main.hpp"
#include "MemoryMatrix.hpp"

using std::unique_ptr;
using std::vector;

class Process;
class Solution;
class Population;
class MemoryMatrix;

void onePointSwap(vector<int> &first, vector<int> &second, int start, int end);
void onePointCopy(vector<int> &first, vector<int> &second, int start, int end);

class OnePointVectorStrategy : public ICrossoverStrategy {
public:
	virtual ~OnePointVectorStrategy() {}
	virtual void execute(
		unique_ptr<Population> &population,
		MemoryType memoryType,
		vector<Process> &initProcesses,
		unique_ptr<MemoryMatrix> &mutMatr,
		unique_ptr<MemoryMatrix> &crMatr
	);
};

class OnePointMatrixSwappingStrategy : public ICrossoverStrategy {
public:
	virtual ~OnePointMatrixSwappingStrategy() {}
	virtual void execute(
		unique_ptr<Population> &population,
		MemoryType memoryType,
		vector<Process> &initProcesses,
		unique_ptr<MemoryMatrix> &mutMatr,
		unique_ptr<MemoryMatrix> &crMatr
	);
};


class OnePointMatrixCopyingStrategy : public ICrossoverStrategy {
public:
	virtual ~OnePointMatrixCopyingStrategy() {}
	virtual void execute(
		unique_ptr<Population> &population,
		MemoryType memoryType,
		vector<Process> &initProcesses,
		unique_ptr<MemoryMatrix> &mutMatr,
		unique_ptr<MemoryMatrix> &crMatr
	);
};

class UniformMatrixSwappingStrategy : public ICrossoverStrategy {
public:
	virtual ~UniformMatrixSwappingStrategy() {}
	virtual void execute(
		unique_ptr<Population> &population,
		MemoryType memoryType,
		vector<Process> &initProcesses,
		unique_ptr<MemoryMatrix> &mutMatr,
		unique_ptr<MemoryMatrix> &crMatr);
};

class UniformMatrixCopyingStrategy : public ICrossoverStrategy {
public:
	virtual ~UniformMatrixCopyingStrategy() {}
	virtual void execute(
		unique_ptr<Population> &population,
		MemoryType memoryType,
		vector<Process> &initProcesses,
		unique_ptr<MemoryMatrix> &mutMatr,
		unique_ptr<MemoryMatrix> &crMatr);
};

#endif /* CROSSOVER_STRATEGY_HPP_ */
