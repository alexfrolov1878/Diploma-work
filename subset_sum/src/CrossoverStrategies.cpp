#include <algorithm>
#include <vector>
#include <memory>

#include "CrossoverStrategies.hpp"
#include "Population.hpp"

using std::unique_ptr;
using std::vector;
using std::min;

void OnePointVectorStrategy::execute(
		unique_ptr<Population> &population,
		MemoryType memoryType,
		unique_ptr<MemoryVector> &memoryVector) {

	// TODO: implement correct crossover operation
	
}

void OnePointMatrixThrowingStrategy::execute(
		unique_ptr<Population> &population,
		MemoryType memoryType,
		unique_ptr<MemoryVector> &memoryVector) {

	// TODO: implement correct crossover operation
	
}

void OnePointMatrixSwappingStrategy::execute(
		unique_ptr<Population> &population,
		MemoryType memoryType,
		unique_ptr<MemoryVector> &memoryVector) {

	// TODO: implement correct crossover operation
	
}

void UniformMatrixThrowingStrategy::execute(
		unique_ptr<Population> &population,
		MemoryType memoryType,
		unique_ptr<MemoryVector> &memoryVector) {

	// TODO: implement correct crossover operation
	
}

void UniformMatrixSwappingStrategy::execute(
		unique_ptr<Population> &population,
		MemoryType memoryType,
		unique_ptr<MemoryVector> &memoryVector) {

	// TODO: implement correct crossover operation
	
}