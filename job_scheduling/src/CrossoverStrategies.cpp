#include <algorithm>
#include <vector>
#include <memory>

#include "CrossoverStrategies.hpp"
#include "Population.hpp"

using std::unique_ptr;
using std::vector;
using std::random_shuffle;
using std::min;

int OnePointVectorStrategy::crossoverSolutions(unique_ptr<Population> &population,
		SolutionPart part, int firstIdx, int secondIdx) {
	int r, tmp1, tmp2;
	vector<int> first;
	vector<int> second;
	auto solutions = population->getSolutions();
	if (part == TASK) {
		first = solutions[firstIdx].getTasks();
		second = solutions[secondIdx].getTasks();
	} else if (part == PRIO) {
		first = solutions[firstIdx].getPriorities();
		second = solutions[secondIdx].getPriorities();
	} else {
		return -1;
	}

	r = Random::getRandomInt(0, numProcesses);
	for (int i = r + 1; i < numProcesses; i++) {
		tmp1 = first[i];
		tmp2 = second[i];
		first[i] = tmp2;
		second[i] = tmp1;
	}
	return r + 1;
}

void OnePointVectorStrategy::execute(
		unique_ptr<Population> &population,
		MemoryType memoryType,
		vector<Process> &initProcesses,
		unique_ptr<MemoryMatrix> &mutMatr,
		unique_ptr<MemoryMatrix> &crMatr) {
	double r;
	int pairs[NUM_SOLUTIONS], start;
	double prob, prob1, prob2, before1, before2, after1, after2;
	auto solutions = population->getSolutions();

	//crossover over tasks
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		r = Random::getRandomDouble(0, 1);
		if (memoryType == NONE) {
			prob = CROSSOVER_PROBABILITY;
		} else {
			prob1 = crMatr->getElement(TASK, pairs[i], 0);
			prob2 = crMatr->getElement(TASK, pairs[i + 1], 0);
			prob = min(prob1, prob2);
		}
		if (r <= prob) {
			before1 = solutions[pairs[i]].getSurvivalValue();
			before2 = solutions[pairs[i + 1]].getSurvivalValue();
			start = crossoverSolutions(population, TASK, pairs[i], pairs[i + 1]);
			population->countSurvivalValue(pairs[i], initProcesses);
			population->countSurvivalValue(pairs[i + 1], initProcesses);
			after1 = solutions[pairs[i]].getSurvivalValue();
			after2 = solutions[pairs[i + 1]].getSurvivalValue();
			if (memoryType == NONE) {
				//do nothing
			} else {
				mutMatr->swapElements(TASK, pairs[i], pairs[i + 1], start);
				crMatr->swapElements(TASK, pairs[i], pairs[i + 1], start);
				crMatr->useChangeStrategy(TASK, pairs[i], 0, before1, after1);
				crMatr->useChangeStrategy(TASK, pairs[i + 1], 0, before2, after2);
			}
		}
	}

	//crossover over priorities
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		r = Random::getRandomDouble(0, 1);
		if (memoryType == NONE) {
			prob = CROSSOVER_PROBABILITY;
		} else {
			prob1 = crMatr->getElement(PRIO, pairs[i], 0);
			prob2 = crMatr->getElement(PRIO, pairs[i + 1], 0);
			prob1 = min(prob1, prob2);
		}
		if (r <= prob) {
			before1 = solutions[pairs[i]].getSurvivalValue();
			before2 = solutions[pairs[i + 1]].getSurvivalValue();
			start = crossoverSolutions(population, TASK, pairs[i], pairs[i + 1]);
			after1 = solutions[pairs[i]].getSurvivalValue();
			after2 = solutions[pairs[i + 1]].getSurvivalValue();
			if (memoryType == NONE) {
				//do nothing
			} else {
				mutMatr->swapElements(PRIO, pairs[i], pairs[i + 1], start);
				crMatr->swapElements(PRIO, pairs[i], pairs[i + 1], start);
				crMatr->useChangeStrategy(PRIO, pairs[i], 1, before1, after1);
				crMatr->useChangeStrategy(PRIO, pairs[i + 1], 1, before2, after2);
			}
		}
	}
}

/*

//=========================================================================
//=========================================================================

int OnePointMatrixThrowingStrategy::crossoverSolutions(SolutionPart part,
		int firstIdx, int secondIdx) {
	int r, tmp1, tmp2;
	vector<int> first;
	vector<int> second;
	auto solutions = population->getSolutions();
	if (part == TASK) {
		first = solutions[firstIdx].getTasks();
		second = solutions[secondIdx].getTasks();
	} else if (part == PRIO) {
		first = solutions[firstIdx].getPriorities();
		second = solutions[secondIdx].getPriorities();
	} else {
		return -1;
	}

	r = Random::getRandomInt(0, numProcesses);
	for (int i = r + 1; i < numProcesses; i++) {
		tmp1 = first[i];
		tmp2 = second[i];
		first[i] = tmp2;
		second[i] = tmp1;
	}
	return r + 1;
}

void OnePointMatrixThrowingStrategy::execute(
		unique_ptr<Population> &population,
		MemoryType memoryType,
		vector<Process> &initProcesses,
		unique_ptr<MemoryMatrix> &mutMatr,
		unique_ptr<MemoryMatrix> &crMatr) {
	double r;
	int pairs[NUM_SOLUTIONS], start;
	double prob, prob1, prob2, before1, before2, after1, after2;
	auto solutions = population->getSolutions();

	//crossover over tasks
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		r = Random::getRandomDouble(0, 1);
		if (memoryType == NONE) {
			prob = CROSSOVER_PROBABILITY;
		} else {
			prob1 = crMatr->getElement(TASK, pairs[i]);
			prob2 = crMatr->getElement(TASK, pairs[i + 1]);
			prob = min(prob1, prob2);
		}
		if (r <= prob) {
			before1 = solutions[pairs[i]].getSurvivalValue();
			before2 = solutions[pairs[i + 1]].getSurvivalValue();
			start = crossoverSolutions(TASK, pairs[i], pairs[i + 1]);
			population->countSurvivalValue(pairs[i], initProcesses);
			population->countSurvivalValue(pairs[i + 1], initProcesses);
			after1 = solutions[pairs[i]].getSurvivalValue();
			after2 = solutions[pairs[i + 1]].getSurvivalValue();
			if (memoryType == NONE) {
				//do nothing
			} else {
				mutMatr->swapElements(TASK, pairs[i], pairs[i + 1], start);
				crMatr->swapElements(TASK, pairs[i], pairs[i + 1], start);
				crMatr->useChangeStrategy(pairs[i], 0, before1, after1);
				crMatr->useChangeStrategy(pairs[i + 1], 0, before2, after2);
			}
		}
	}

	//crossover over priorities
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		r = Random::getRandomDouble(0, 1);
		if (memoryType == NONE) {
			prob = CROSSOVER_PROBABILITY;
		} else {
			prob1 = crMatr->getElement(PRIO, pairs[i]);
			prob2 = crMatr->getElement(PRIO, pairs[i + 1]);
			prob1 = min(prob1, prob2);
		}
		if (r <= prob) {
			before1 = solutions[pairs[i]].getSurvivalValue();
			before2 = solutions[pairs[i + 1]].getSurvivalValue();
			start = population->crossoverSolutions(PRIO, pairs[i], pairs[i + 1]);
			after1 = solutions[pairs[i]].getSurvivalValue();
			after2 = solutions[pairs[i + 1]].getSurvivalValue();
			if (memoryType == NONE) {
				//do nothing
			} else {
				mutMatr->swapElements(PRIO, pairs[i], pairs[i + 1], start);
				crMatr->swapElements(PRIO, pairs[i], pairs[i + 1], start);
				crMatr->useChangeStrategy(pairs[i], 1, before1, after1);
				crMatr->useChangeStrategy(pairs[i + 1], 1, before2, after2);
			}
		}
	}
}

//=========================================================================
//=========================================================================

int OnePointMatrixSwappingStrategy::crossoverSolutions(SolutionPart part,
		int firstIdx, int secondIdx) {
	int r, tmp1, tmp2;
	vector<int> first;
	vector<int> second;
	auto solutions = population->getSolutions();
	if (part == TASK) {
		first = solutions[firstIdx].getTasks();
		second = solutions[secondIdx].getTasks();
	} else if (part == PRIO) {
		first = solutions[firstIdx].getPriorities();
		second = solutions[secondIdx].getPriorities();
	} else {
		return -1;
	}

	r = Random::getRandomInt(0, numProcesses);
	for (int i = r + 1; i < numProcesses; i++) {
		tmp1 = first[i];
		tmp2 = second[i];
		first[i] = tmp2;
		second[i] = tmp1;
	}
	return r + 1;
}

void OnePointMatrixSwappingStrategy::execute(
		unique_ptr<Population> &population,
		MemoryType memoryType,
		vector<Process> &initProcesses,
		unique_ptr<MemoryMatrix> &mutMatr,
		unique_ptr<MemoryMatrix> &crMatr) {
	double r;
	int pairs[NUM_SOLUTIONS], start;
	double prob, prob1, prob2, before1, before2, after1, after2;
	auto solutions = population->getSolutions();

	//crossover over tasks
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		r = Random::getRandomDouble(0, 1);
		if (memoryType == NONE) {
			prob = CROSSOVER_PROBABILITY;
		} else {
			prob1 = crMatr->getElement(TASK, pairs[i]);
			prob2 = crMatr->getElement(TASK, pairs[i + 1]);
			prob = min(prob1, prob2);
		}
		if (r <= prob) {
			before1 = solutions[pairs[i]].getSurvivalValue();
			before2 = solutions[pairs[i + 1]].getSurvivalValue();
			start = crossoverSolutions(TASK, pairs[i], pairs[i + 1]);
			population->countSurvivalValue(pairs[i], initProcesses);
			population->countSurvivalValue(pairs[i + 1], initProcesses);
			after1 = solutions[pairs[i]].getSurvivalValue();
			after2 = solutions[pairs[i + 1]].getSurvivalValue();
			if (memoryType == NONE) {
				//do nothing
			} else {
				mutMatr->swapElements(TASK, pairs[i], pairs[i + 1], start);
				crMatr->swapElements(TASK, pairs[i], pairs[i + 1], start);
				crMatr->useChangeStrategy(pairs[i], 0, before1, after1);
				crMatr->useChangeStrategy(pairs[i + 1], 0, before2, after2);
			}
		}
	}

	//crossover over priorities
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		r = Random::getRandomDouble(0, 1);
		if (memoryType == NONE) {
			prob = CROSSOVER_PROBABILITY;
		} else {
			prob1 = crMatr->getElement(PRIO, pairs[i]);
			prob2 = crMatr->getElement(PRIO, pairs[i + 1]);
			prob1 = min(prob1, prob2);
		}
		if (r <= prob) {
			before1 = solutions[pairs[i]].getSurvivalValue();
			before2 = solutions[pairs[i + 1]].getSurvivalValue();
			start = population->crossoverSolutions(PRIO, pairs[i], pairs[i + 1]);
			after1 = solutions[pairs[i]].getSurvivalValue();
			after2 = solutions[pairs[i + 1]].getSurvivalValue();
			if (memoryType == NONE) {
				//do nothing
			} else {
				mutMatr->swapElements(PRIO, pairs[i], pairs[i + 1], start);
				crMatr->swapElements(PRIO, pairs[i], pairs[i + 1], start);
				crMatr->useChangeStrategy(pairs[i], 1, before1, after1);
				crMatr->useChangeStrategy(pairs[i + 1], 1, before2, after2);
			}
		}
	}
}

//=========================================================================
//=========================================================================

int UniformMatrixThrowingStrategy::crossoverSolutions(SolutionPart part,
		int firstIdx, int secondIdx) {
	int r, tmp1, tmp2;
	vector<int> first;
	vector<int> second;
	auto solutions = population->getSolutions();
	if (part == TASK) {
		first = solutions[firstIdx].getTasks();
		second = solutions[secondIdx].getTasks();
	} else if (part == PRIO) {
		first = solutions[firstIdx].getPriorities();
		second = solutions[secondIdx].getPriorities();
	} else {
		return -1;
	}

	r = Random::getRandomInt(0, numProcesses);
	for (int i = r + 1; i < numProcesses; i++) {
		tmp1 = first[i];
		tmp2 = second[i];
		first[i] = tmp2;
		second[i] = tmp1;
	}
	return r + 1;
}

void UniformMatrixThrowingStrategy::execute(
		unique_ptr<Population> &population,
		MemoryType memoryType,
		vector<Process> &initProcesses,
		unique_ptr<MemoryMatrix> &mutMatr,
		unique_ptr<MemoryMatrix> &crMatr) {
	double r;
	int pairs[NUM_SOLUTIONS], start;
	double prob, prob1, prob2, before1, before2, after1, after2;
	auto solutions = population->getSolutions();

	//crossover over tasks
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		r = Random::getRandomDouble(0, 1);
		if (memoryType == NONE) {
			prob = CROSSOVER_PROBABILITY;
		} else {
			prob1 = crMatr->getElement(TASK, pairs[i]);
			prob2 = crMatr->getElement(TASK, pairs[i + 1]);
			prob = min(prob1, prob2);
		}
		if (r <= prob) {
			before1 = solutions[pairs[i]].getSurvivalValue();
			before2 = solutions[pairs[i + 1]].getSurvivalValue();
			start = crossoverSolutions(TASK, pairs[i], pairs[i + 1]);
			population->countSurvivalValue(pairs[i], initProcesses);
			population->countSurvivalValue(pairs[i + 1], initProcesses);
			after1 = solutions[pairs[i]].getSurvivalValue();
			after2 = solutions[pairs[i + 1]].getSurvivalValue();
			if (memoryType == NONE) {
				//do nothing
			} else {
				mutMatr->swapElements(TASK, pairs[i], pairs[i + 1], start);
				crMatr->swapElements(TASK, pairs[i], pairs[i + 1], start);
				crMatr->useChangeStrategy(pairs[i], 0, before1, after1);
				crMatr->useChangeStrategy(pairs[i + 1], 0, before2, after2);
			}
		}
	}

	//crossover over priorities
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		r = Random::getRandomDouble(0, 1);
		if (memoryType == NONE) {
			prob = CROSSOVER_PROBABILITY;
		} else {
			prob1 = crMatr->getElement(PRIO, pairs[i]);
			prob2 = crMatr->getElement(PRIO, pairs[i + 1]);
			prob1 = min(prob1, prob2);
		}
		if (r <= prob) {
			before1 = solutions[pairs[i]].getSurvivalValue();
			before2 = solutions[pairs[i + 1]].getSurvivalValue();
			start = population->crossoverSolutions(PRIO, pairs[i], pairs[i + 1]);
			after1 = solutions[pairs[i]].getSurvivalValue();
			after2 = solutions[pairs[i + 1]].getSurvivalValue();
			if (memoryType == NONE) {
				//do nothing
			} else {
				mutMatr->swapElements(PRIO, pairs[i], pairs[i + 1], start);
				crMatr->swapElements(PRIO, pairs[i], pairs[i + 1], start);
				crMatr->useChangeStrategy(pairs[i], 1, before1, after1);
				crMatr->useChangeStrategy(pairs[i + 1], 1, before2, after2);
			}
		}
	}
}

//=========================================================================
//=========================================================================

int UniformMatrixSwappingStrategy::crossoverSolutions(SolutionPart part,
		int firstIdx, int secondIdx) {
	int r, tmp1, tmp2;
	vector<int> first;
	vector<int> second;
	auto solutions = population->getSolutions();
	if (part == TASK) {
		first = solutions[firstIdx].getTasks();
		second = solutions[secondIdx].getTasks();
	} else if (part == PRIO) {
		first = solutions[firstIdx].getPriorities();
		second = solutions[secondIdx].getPriorities();
	} else {
		return -1;
	}

	r = Random::getRandomInt(0, numProcesses);
	for (int i = r + 1; i < numProcesses; i++) {
		tmp1 = first[i];
		tmp2 = second[i];
		first[i] = tmp2;
		second[i] = tmp1;
	}
	return r + 1;
}

void UniformMatrixSwappingStrategy::execute(
		unique_ptr<Population> &population,
		MemoryType memoryType,
		vector<Process> &initProcesses,
		unique_ptr<MemoryMatrix> &mutMatr,
		unique_ptr<MemoryMatrix> &crMatr) {
	double r;
	int pairs[NUM_SOLUTIONS], start;
	double prob, prob1, prob2, before1, before2, after1, after2;
	auto solutions = population->getSolutions();

	//crossover over tasks
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		r = Random::getRandomDouble(0, 1);
		if (memoryType == NONE) {
			prob = CROSSOVER_PROBABILITY;
		} else {
			prob1 = crMatr->getElement(TASK, pairs[i]);
			prob2 = crMatr->getElement(TASK, pairs[i + 1]);
			prob = min(prob1, prob2);
		}
		if (r <= prob) {
			before1 = solutions[pairs[i]].getSurvivalValue();
			before2 = solutions[pairs[i + 1]].getSurvivalValue();
			start = crossoverSolutions(TASK, pairs[i], pairs[i + 1]);
			population->countSurvivalValue(pairs[i], initProcesses);
			population->countSurvivalValue(pairs[i + 1], initProcesses);
			after1 = solutions[pairs[i]].getSurvivalValue();
			after2 = solutions[pairs[i + 1]].getSurvivalValue();
			if (memoryType == NONE) {
				//do nothing
			} else {
				mutMatr->swapElements(TASK, pairs[i], pairs[i + 1], start);
				crMatr->swapElements(TASK, pairs[i], pairs[i + 1], start);
				crMatr->useChangeStrategy(pairs[i], 0, before1, after1);
				crMatr->useChangeStrategy(pairs[i + 1], 0, before2, after2);
			}
		}
	}

	//crossover over priorities
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		r = Random::getRandomDouble(0, 1);
		if (memoryType == NONE) {
			prob = CROSSOVER_PROBABILITY;
		} else {
			prob1 = crMatr->getElement(PRIO, pairs[i]);
			prob2 = crMatr->getElement(PRIO, pairs[i + 1]);
			prob1 = min(prob1, prob2);
		}
		if (r <= prob) {
			before1 = solutions[pairs[i]].getSurvivalValue();
			before2 = solutions[pairs[i + 1]].getSurvivalValue();
			start = population->crossoverSolutions(PRIO, pairs[i], pairs[i + 1]);
			after1 = solutions[pairs[i]].getSurvivalValue();
			after2 = solutions[pairs[i + 1]].getSurvivalValue();
			if (memoryType == NONE) {
				//do nothing
			} else {
				mutMatr->swapElements(PRIO, pairs[i], pairs[i + 1], start);
				crMatr->swapElements(PRIO, pairs[i], pairs[i + 1], start);
				crMatr->useChangeStrategy(pairs[i], 1, before1, after1);
				crMatr->useChangeStrategy(pairs[i + 1], 1, before2, after2);
			}
		}
	}
}

*/