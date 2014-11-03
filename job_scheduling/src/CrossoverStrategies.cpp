#include <algorithm>
#include <vector>
#include <memory>

#include "CrossoverStrategies.hpp"
#include "Population.hpp"

using std::unique_ptr;
using std::vector;
using std::random_shuffle;
using std::min;

inline void onePointSwap(vector<int> &first, vector<int> &second, int start, int end) {
	int tmp1, tmp2;
	for (int i = start; i < end; i++) {
		tmp1 = first[i];
		tmp2 = second[i];
		first[i] = tmp2;
		second[i] = tmp1;
	}
}

inline void onePointCopy(vector<int> &first, vector<int> &second, int start, int end) {
	for (int i = start; i < end; i++) {
		second[i] = first[i];
	}
}

//=========================================================================
//=========================================================================

void OnePointVectorStrategy::execute(
		unique_ptr<Population> &population,
		MemoryType memoryType,
		vector<Process> &initProcesses,
		unique_ptr<MemoryMatrix> &mutMatr,
		unique_ptr<MemoryMatrix> &crMatr) {
	double r;
	int pairs[NUM_SOLUTIONS], point;
	double prob, prob1, prob2, before1, before2, after1, after2;
	auto &solutions = population->getSolutions();

	//crossover over tasks
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		r = Random::getRandomDouble(0, 1);
		point = Random::getRandomInt(0, numProcesses);
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
			onePointSwap(solutions[pairs[i]].getTasks(),
				solutions[pairs[i + 1]].getTasks(), point, numProcesses);
			population->countSurvivalValue(pairs[i], initProcesses);
			population->countSurvivalValue(pairs[i + 1], initProcesses);
			after1 = solutions[pairs[i]].getSurvivalValue();
			after2 = solutions[pairs[i + 1]].getSurvivalValue();
			if (memoryType == NONE) {
				//do nothing
			} else {
				mutMatr->swapElements(TASK, pairs[i], pairs[i + 1],
					point, numProcesses);
				crMatr->swapElements(TASK, pairs[i], pairs[i + 1],
					point, numProcesses);
				crMatr->useChangeStrategy(TASK, pairs[i],
					0, 1, before1, after1);
				crMatr->useChangeStrategy(TASK, pairs[i + 1],
					0, 1, before2, after2);
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
		point = Random::getRandomInt(0, numProcesses);
		if (memoryType == NONE) {
			prob = CROSSOVER_PROBABILITY;
		} else {
			prob1 = crMatr->getElement(PRIO, pairs[i], 0);
			prob2 = crMatr->getElement(PRIO, pairs[i + 1], 0);
			prob = min(prob1, prob2);
		}
		if (r <= prob) {
			before1 = solutions[pairs[i]].getSurvivalValue();
			before2 = solutions[pairs[i + 1]].getSurvivalValue();
			onePointSwap(solutions[pairs[i]].getPriorities(),
				solutions[pairs[i + 1]].getPriorities(), point, numProcesses);
			population->countSurvivalValue(pairs[i], initProcesses);
			population->countSurvivalValue(pairs[i + 1], initProcesses);
			after1 = solutions[pairs[i]].getSurvivalValue();
			after2 = solutions[pairs[i + 1]].getSurvivalValue();
			if (memoryType == NONE) {
				//do nothing
			} else {
				mutMatr->swapElements(PRIO, pairs[i], pairs[i + 1],
					point, numProcesses);
				crMatr->swapElements(PRIO, pairs[i], pairs[i + 1],
					point, numProcesses);
				crMatr->useChangeStrategy(PRIO, pairs[i],
					0, 1, before1, after1);
				crMatr->useChangeStrategy(PRIO, pairs[i + 1],
					0, 1, before2, after2);
			}
		}
	}
}

//=========================================================================
//=========================================================================

void OnePointMatrixSwappingStrategy::execute(
		unique_ptr<Population> &population,
		MemoryType memoryType,
		vector<Process> &initProcesses,
		unique_ptr<MemoryMatrix> &mutMatr,
		unique_ptr<MemoryMatrix> &crMatr) {
	double r;
	int pairs[NUM_SOLUTIONS], point;
	double prob, prob1, prob2, before1, before2, after1, after2;
	auto &solutions = population->getSolutions();

	//crossover over tasks
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		r = Random::getRandomDouble(0, 1);
		point = Random::getRandomInt(0, numProcesses);
		if (memoryType == NONE) {
			prob = CROSSOVER_PROBABILITY;
		} else {
			prob1 = 0.0;
			for (int j = point; j < numProcesses; j++) {
				prob1 += crMatr->getElement(TASK, pairs[i], j);
			}
			prob1 /= (numProcesses - point);
			prob2 = 0.0;
			for (int j = point; j < numProcesses; j++) {
				prob2 += crMatr->getElement(TASK, pairs[i + 1], j);
			}
			prob2 /= (numProcesses - point);
			prob = min(prob1, prob2);
		}
		if (r <= prob) {
			before1 = solutions[pairs[i]].getSurvivalValue();
			before2 = solutions[pairs[i + 1]].getSurvivalValue();
			onePointSwap(solutions[pairs[i]].getTasks(),
				solutions[pairs[i + 1]].getTasks(), point, numProcesses);
			population->countSurvivalValue(pairs[i], initProcesses);
			population->countSurvivalValue(pairs[i + 1], initProcesses);
			after1 = solutions[pairs[i]].getSurvivalValue();
			after2 = solutions[pairs[i + 1]].getSurvivalValue();
			if (memoryType == NONE) {
				//do nothing
			} else {
				mutMatr->swapElements(TASK, pairs[i], pairs[i + 1],
					point, numProcesses);
				crMatr->swapElements(TASK, pairs[i], pairs[i + 1],
					point, numProcesses);
				crMatr->useChangeStrategy(TASK, pairs[i],
					point, numProcesses, before1, after1);
				crMatr->useChangeStrategy(TASK, pairs[i + 1],
					point, numProcesses, before2, after2);
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
		point = Random::getRandomInt(0, numProcesses);
		if (memoryType == NONE) {
			prob = CROSSOVER_PROBABILITY;
		} else {
			prob1 = 0.0;
			for (int j = point; j < numProcesses; j++) {
				prob1 += crMatr->getElement(PRIO, pairs[i], j);
			}
			prob1 /= (numProcesses - point);
			prob2 = 0.0;
			for (int j = point; j < numProcesses; j++) {
				prob2 += crMatr->getElement(PRIO, pairs[i + 1], j);
			}
			prob2 /= (numProcesses - point);
			prob = min(prob1, prob2);
		}
		if (r <= prob) {
			before1 = solutions[pairs[i]].getSurvivalValue();
			before2 = solutions[pairs[i + 1]].getSurvivalValue();
			onePointSwap(solutions[pairs[i]].getPriorities(),
				solutions[pairs[i + 1]].getPriorities(), point, numProcesses);
			population->countSurvivalValue(pairs[i], initProcesses);
			population->countSurvivalValue(pairs[i + 1], initProcesses);
			after1 = solutions[pairs[i]].getSurvivalValue();
			after2 = solutions[pairs[i + 1]].getSurvivalValue();
			if (memoryType == NONE) {
				//do nothing
			} else {
				mutMatr->swapElements(PRIO, pairs[i], pairs[i + 1],
					point, numProcesses);
				crMatr->swapElements(PRIO, pairs[i], pairs[i + 1],
					point, numProcesses);
				crMatr->useChangeStrategy(PRIO, pairs[i],
					point, numProcesses, before1, after1);
				crMatr->useChangeStrategy(PRIO, pairs[i + 1],
					point, numProcesses, before2, after2);
			}
		}
	}
}

//=========================================================================
//=========================================================================

void OnePointMatrixCopyingStrategy::execute(
		unique_ptr<Population> &population,
		MemoryType memoryType,
		vector<Process> &initProcesses,
		unique_ptr<MemoryMatrix> &mutMatr,
		unique_ptr<MemoryMatrix> &crMatr) {
	double r;
	int pairs[NUM_SOLUTIONS], point;
	double prob, prob1, prob2, before1, before2, after1, after2;
	auto &solutions = population->getSolutions();

	//crossover over tasks
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		r = Random::getRandomDouble(0, 1);
		point = Random::getRandomInt(0, numProcesses);
		if (memoryType == NONE) {
			prob = CROSSOVER_PROBABILITY;
		} else {
			prob1 = 0.0;
			for (int j = point; j < numProcesses; j++) {
				prob1 += crMatr->getElement(TASK, pairs[i], j);
			}
			prob1 /= (numProcesses - point);
			prob2 = 0.0;
			for (int j = point; j < numProcesses; j++) {
				prob2 += crMatr->getElement(TASK, pairs[i + 1], j);
			}
			prob2 /= (numProcesses - point);
			prob = min(prob1, prob2);
		}
		if (memoryType != NONE && prob1 > GOOD_CROSSOVER_PROBABILITY &&
				prob2 < BAD_CROSSOVER_PROBABILITY) {
			before1 = solutions[pairs[i]].getSurvivalValue();
			before2 = solutions[pairs[i + 1]].getSurvivalValue();
			onePointCopy(solutions[pairs[i]].getTasks(),
				solutions[pairs[i + 1]].getTasks(), point, numProcesses);
			population->countSurvivalValue(pairs[i], initProcesses);
			population->countSurvivalValue(pairs[i + 1], initProcesses);
			after1 = solutions[pairs[i]].getSurvivalValue();
			after2 = solutions[pairs[i + 1]].getSurvivalValue();
			mutMatr->copyElements(TASK, pairs[i], pairs[i + 1],
				point, numProcesses);
			crMatr->copyElements(TASK, pairs[i], pairs[i + 1],
				point, numProcesses);
			crMatr->useChangeStrategy(TASK, pairs[i],
				point, numProcesses, before1, after1);
			crMatr->useChangeStrategy(TASK, pairs[i + 1],
				point, numProcesses, before2, after2);
		} else if (memoryType != NONE && prob2 > GOOD_CROSSOVER_PROBABILITY &&
				prob1 < BAD_CROSSOVER_PROBABILITY) {
			before1 = solutions[pairs[i]].getSurvivalValue();
			before2 = solutions[pairs[i + 1]].getSurvivalValue();
			onePointCopy(solutions[pairs[i + 1]].getTasks(),
				solutions[pairs[i]].getTasks(), point, numProcesses);
			population->countSurvivalValue(pairs[i], initProcesses);
			population->countSurvivalValue(pairs[i + 1], initProcesses);
			after1 = solutions[pairs[i]].getSurvivalValue();
			after2 = solutions[pairs[i + 1]].getSurvivalValue();
			mutMatr->copyElements(TASK, pairs[i + 1], pairs[i],
				point, numProcesses);
			crMatr->copyElements(TASK, pairs[i + 1], pairs[i],
				point, numProcesses);
			crMatr->useChangeStrategy(TASK, pairs[i],
				point, numProcesses, before1, after1);
			crMatr->useChangeStrategy(TASK, pairs[i + 1],
				point, numProcesses, before2, after2);
		} else if (r <= prob) {
			before1 = solutions[pairs[i]].getSurvivalValue();
			before2 = solutions[pairs[i + 1]].getSurvivalValue();
			onePointSwap(solutions[pairs[i]].getTasks(),
				solutions[pairs[i + 1]].getTasks(), point, numProcesses);
			population->countSurvivalValue(pairs[i], initProcesses);
			population->countSurvivalValue(pairs[i + 1], initProcesses);
			after1 = solutions[pairs[i]].getSurvivalValue();
			after2 = solutions[pairs[i + 1]].getSurvivalValue();
			if (memoryType == NONE) {
				//do nothing
			} else {
				mutMatr->swapElements(TASK, pairs[i], pairs[i + 1],
					point, numProcesses);
				crMatr->swapElements(TASK, pairs[i], pairs[i + 1],
					point, numProcesses);
				crMatr->useChangeStrategy(TASK, pairs[i],
					point, numProcesses, before1, after1);
				crMatr->useChangeStrategy(TASK, pairs[i + 1],
					point, numProcesses, before2, after2);
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
		point = Random::getRandomInt(0, numProcesses);
		if (memoryType == NONE) {
			prob = CROSSOVER_PROBABILITY;
		} else {
			prob1 = 0.0;
			for (int j = point; j < numProcesses; j++) {
				prob1 += crMatr->getElement(PRIO, pairs[i], j);
			}
			prob1 /= (numProcesses - point);
			prob2 = 0.0;
			for (int j = point; j < numProcesses; j++) {
				prob2 += crMatr->getElement(PRIO, pairs[i + 1], j);
			}
			prob2 /= (numProcesses - point);
			prob = min(prob1, prob2);
		}
		if (memoryType != NONE && prob1 > GOOD_CROSSOVER_PROBABILITY &&
				prob2 < BAD_CROSSOVER_PROBABILITY) {
			before1 = solutions[pairs[i]].getSurvivalValue();
			before2 = solutions[pairs[i + 1]].getSurvivalValue();
			onePointCopy(solutions[pairs[i]].getPriorities(),
				solutions[pairs[i + 1]].getPriorities(), point, numProcesses);
			population->countSurvivalValue(pairs[i], initProcesses);
			population->countSurvivalValue(pairs[i + 1], initProcesses);
			after1 = solutions[pairs[i]].getSurvivalValue();
			after2 = solutions[pairs[i + 1]].getSurvivalValue();
			mutMatr->copyElements(PRIO, pairs[i], pairs[i + 1],
				point, numProcesses);
			crMatr->copyElements(PRIO, pairs[i], pairs[i + 1],
				point, numProcesses);
			crMatr->useChangeStrategy(PRIO, pairs[i],
				point, numProcesses, before1, after1);
			crMatr->useChangeStrategy(PRIO, pairs[i + 1],
				point, numProcesses, before2, after2);
		} else if (memoryType != NONE && prob2 > GOOD_CROSSOVER_PROBABILITY &&
				prob1 < BAD_CROSSOVER_PROBABILITY) {
			before1 = solutions[pairs[i]].getSurvivalValue();
			before2 = solutions[pairs[i + 1]].getSurvivalValue();
			onePointCopy(solutions[pairs[i + 1]].getPriorities(),
				solutions[pairs[i]].getPriorities(), point, numProcesses);
			population->countSurvivalValue(pairs[i], initProcesses);
			population->countSurvivalValue(pairs[i + 1], initProcesses);
			after1 = solutions[pairs[i]].getSurvivalValue();
			after2 = solutions[pairs[i + 1]].getSurvivalValue();
			mutMatr->copyElements(PRIO, pairs[i + 1], pairs[i],
				point, numProcesses);
			crMatr->copyElements(PRIO, pairs[i + 1], pairs[i],
				point, numProcesses);
			crMatr->useChangeStrategy(PRIO, pairs[i],
				point, numProcesses, before1, after1);
			crMatr->useChangeStrategy(PRIO, pairs[i + 1],
				point, numProcesses, before2, after2);
		} else if (r <= prob) {
			before1 = solutions[pairs[i]].getSurvivalValue();
			before2 = solutions[pairs[i + 1]].getSurvivalValue();
			onePointSwap(solutions[pairs[i]].getPriorities(),
				solutions[pairs[i + 1]].getPriorities(), point, numProcesses);
			population->countSurvivalValue(pairs[i], initProcesses);
			population->countSurvivalValue(pairs[i + 1], initProcesses);
			after1 = solutions[pairs[i]].getSurvivalValue();
			after2 = solutions[pairs[i + 1]].getSurvivalValue();
			if (memoryType == NONE) {
				//do nothing
			} else {
				mutMatr->swapElements(PRIO, pairs[i], pairs[i + 1],
					point, numProcesses);
				crMatr->swapElements(PRIO, pairs[i], pairs[i + 1],
					point, numProcesses);
				crMatr->useChangeStrategy(PRIO, pairs[i],
					point, numProcesses, before1, after1);
				crMatr->useChangeStrategy(PRIO, pairs[i + 1],
					point, numProcesses, before2, after2);
			}
		}
	}
}

//=========================================================================
//=========================================================================

void UniformMatrixSwappingStrategy::execute(
		unique_ptr<Population> &population,
		MemoryType memoryType,
		vector<Process> &initProcesses,
		unique_ptr<MemoryMatrix> &mutMatr,
		unique_ptr<MemoryMatrix> &crMatr) {
	double r;
	int pairs[NUM_SOLUTIONS];
	double prob, prob1, prob2, before1, before2, after1, after2;
	auto &solutions = population->getSolutions();
	vector<int> v;

	//crossover over tasks
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		before1 = solutions[pairs[i]].getSurvivalValue();
		before2 = solutions[pairs[i + 1]].getSurvivalValue();
		for (int j = 0; j < numProcesses; j++) {
			r = Random::getRandomDouble(0, 1);
			if (memoryType == NONE) {
				prob = CROSSOVER_PROBABILITY;
			} else {
				prob1 = crMatr->getElement(TASK, pairs[i], j);
				prob2 = crMatr->getElement(TASK, pairs[i + 1], j);
				prob = min(prob1, prob2);
			}
			if (r <= prob) {
				onePointSwap(solutions[pairs[i]].getTasks(),
					solutions[pairs[i + 1]].getTasks(), j, j + 1);
				if (memoryType != NONE) {
					v.push_back(j);
				}
			}
		}
		population->countSurvivalValue(pairs[i], initProcesses);
		population->countSurvivalValue(pairs[i + 1], initProcesses);
		after1 = solutions[pairs[i]].getSurvivalValue();
		after2 = solutions[pairs[i + 1]].getSurvivalValue();
		if (memoryType == NONE) {
			//do nothing
		} else {
			for (int j = 0, size = v.size(); j < size; j++) {
				mutMatr->swapElements(TASK, pairs[i], pairs[i + 1],
					v[j], v[j] + 1);
				crMatr->swapElements(TASK, pairs[i], pairs[i + 1],
					v[j], v[j] + 1);
				crMatr->useChangeStrategy(TASK, pairs[i],
					v[j], v[j] + 1,	before1, after1);
				crMatr->useChangeStrategy(TASK, pairs[i + 1],
					v[j], v[j] + 1,	before2, after2);
			}
			v.clear();
		}
	}

	//crossover over priorities
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		before1 = solutions[pairs[i]].getSurvivalValue();
		before2 = solutions[pairs[i + 1]].getSurvivalValue();
		for (int j = 0; j < numProcesses; j++) {
			r = Random::getRandomDouble(0, 1);
			if (memoryType == NONE) {
				prob = CROSSOVER_PROBABILITY;
			} else {
				prob1 = crMatr->getElement(PRIO, pairs[i], j);
				prob2 = crMatr->getElement(PRIO, pairs[i + 1], j);
				prob = min(prob1, prob2);
			}
			if (r <= prob) {
				onePointSwap(solutions[pairs[i]].getPriorities(),
					solutions[pairs[i + 1]].getPriorities(), j, j + 1);
				if (memoryType != NONE) {
					v.push_back(j);
				}
			}
		}
		population->countSurvivalValue(pairs[i], initProcesses);
		population->countSurvivalValue(pairs[i + 1], initProcesses);
		after1 = solutions[pairs[i]].getSurvivalValue();
		after2 = solutions[pairs[i + 1]].getSurvivalValue();
		if (memoryType == NONE) {
			//do nothing
		} else {
			for (int j = 0, size = v.size(); j < size; j++) {
				mutMatr->swapElements(PRIO, pairs[i], pairs[i + 1],
					v[j], v[j] + 1);
				crMatr->swapElements(PRIO, pairs[i], pairs[i + 1],
					v[j], v[j] + 1);
				crMatr->useChangeStrategy(PRIO, pairs[i],
					v[j], v[j] + 1,	before1, after1);
				crMatr->useChangeStrategy(PRIO, pairs[i + 1],
					v[j], v[j] + 1,	before2, after2);
			}
			v.clear();
		}
	}
}

//=========================================================================
//=========================================================================

void UniformMatrixCopyingStrategy::execute(
		unique_ptr<Population> &population,
		MemoryType memoryType,
		vector<Process> &initProcesses,
		unique_ptr<MemoryMatrix> &mutMatr,
		unique_ptr<MemoryMatrix> &crMatr) {
	double r;
	int pairs[NUM_SOLUTIONS];
	double prob, prob1, prob2, before1, before2, after1, after2;
	auto &solutions = population->getSolutions();
	vector<int> v1, v2, v3;

	//crossover over tasks
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		before1 = solutions[pairs[i]].getSurvivalValue();
		before2 = solutions[pairs[i + 1]].getSurvivalValue();
		for (int j = 0; j < numProcesses; j++) {
			r = Random::getRandomDouble(0, 1);
			if (memoryType == NONE) {
				prob = CROSSOVER_PROBABILITY;
			} else {
				prob1 = crMatr->getElement(TASK, pairs[i], j);
				prob2 = crMatr->getElement(TASK, pairs[i + 1], j);
				prob = min(prob1, prob2);
			}
			if (memoryType != NONE && prob1 > GOOD_CROSSOVER_PROBABILITY &&
					prob2 < BAD_CROSSOVER_PROBABILITY) {
				onePointCopy(solutions[pairs[i]].getTasks(),
					solutions[pairs[i + 1]].getTasks(), j, j + 1);
				if (memoryType != NONE) {
					v1.push_back(j);
				}
			} else if (memoryType != NONE && prob1 > GOOD_CROSSOVER_PROBABILITY &&
					prob2 < BAD_CROSSOVER_PROBABILITY) {
				onePointCopy(solutions[pairs[i + 1]].getTasks(),
					solutions[pairs[i]].getTasks(), j, j + 1);
				if (memoryType != NONE) {
					v2.push_back(j);
				}
			} else if (r <= prob) {
				onePointSwap(solutions[pairs[i]].getTasks(),
					solutions[pairs[i + 1]].getTasks(), j, j + 1);
				if (memoryType != NONE) {
					v3.push_back(j);
				}
			}
		}
		population->countSurvivalValue(pairs[i], initProcesses);
		population->countSurvivalValue(pairs[i + 1], initProcesses);
		after1 = solutions[pairs[i]].getSurvivalValue();
		after2 = solutions[pairs[i + 1]].getSurvivalValue();
		if (memoryType == NONE) {
			//do nothing
		} else {
			for (int j = 0, size = v1.size(); j < size; j++) {
				mutMatr->copyElements(TASK, pairs[i], pairs[i + 1],
					v1[j], v1[j] + 1);
				crMatr->copyElements(TASK, pairs[i], pairs[i + 1],
					v1[j], v1[j] + 1);
				crMatr->useChangeStrategy(TASK, pairs[i],
					v1[j], v1[j] + 1, before1, after1);
				crMatr->useChangeStrategy(TASK, pairs[i + 1],
					v1[j], v1[j] + 1, before2, after2);
			}
			v1.clear();
			for (int j = 0, size = v2.size(); j < size; j++) {
				mutMatr->copyElements(TASK, pairs[i + 1], pairs[i],
					v2[j], v2[j] + 1);
				crMatr->copyElements(TASK, pairs[i + 1], pairs[i],
					v2[j], v2[j] + 1);
				crMatr->useChangeStrategy(TASK, pairs[i],
					v2[j], v2[j] + 1, before1, after1);
				crMatr->useChangeStrategy(TASK, pairs[i + 1],
					v2[j], v2[j] + 1, before2, after2);
			}
			v2.clear();
			for (int j = 0, size = v3.size(); j < size; j++) {
				mutMatr->swapElements(TASK, pairs[i], pairs[i + 1],
					v3[j], v3[j] + 1);
				crMatr->swapElements(TASK, pairs[i], pairs[i + 1],
					v3[j], v3[j] + 1);
				crMatr->useChangeStrategy(TASK, pairs[i],
					v3[j], v3[j] + 1, before1, after1);
				crMatr->useChangeStrategy(TASK, pairs[i + 1],
					v3[j], v3[j] + 1, before2, after2);
			}
			v3.clear();
		}
	}

	//crossover over priorities
	for (int i = 0; i < NUM_SOLUTIONS; i++) {
		pairs[i] = i;
	}
	random_shuffle(pairs, pairs + NUM_SOLUTIONS);
	for (int i = 0; i < NUM_SOLUTIONS; i += 2) {
		before1 = solutions[pairs[i]].getSurvivalValue();
		before2 = solutions[pairs[i + 1]].getSurvivalValue();
		for (int j = 0; j < numProcesses; j++) {
			r = Random::getRandomDouble(0, 1);
			if (memoryType == NONE) {
				prob = CROSSOVER_PROBABILITY;
			} else {
				prob1 = crMatr->getElement(PRIO, pairs[i], j);
				prob2 = crMatr->getElement(PRIO, pairs[i + 1], j);
				prob = min(prob1, prob2);
			}
			if (memoryType != NONE && prob1 > GOOD_CROSSOVER_PROBABILITY &&
					prob2 < BAD_CROSSOVER_PROBABILITY) {
				onePointCopy(solutions[pairs[i]].getPriorities(),
					solutions[pairs[i + 1]].getPriorities(), j, j + 1);
				if (memoryType != NONE) {
					v1.push_back(j);
				}
			} else if (memoryType != NONE && prob1 > GOOD_CROSSOVER_PROBABILITY &&
					prob2 < BAD_CROSSOVER_PROBABILITY) {
				onePointCopy(solutions[pairs[i + 1]].getPriorities(),
					solutions[pairs[i]].getPriorities(), j, j + 1);
				if (memoryType != NONE) {
					v2.push_back(j);
				}
			} else if (r <= prob) {
				onePointSwap(solutions[pairs[i]].getPriorities(),
					solutions[pairs[i + 1]].getPriorities(), j, j + 1);
				if (memoryType != NONE) {
					v3.push_back(j);
				}
			}
		}
		population->countSurvivalValue(pairs[i], initProcesses);
		population->countSurvivalValue(pairs[i + 1], initProcesses);
		after1 = solutions[pairs[i]].getSurvivalValue();
		after2 = solutions[pairs[i + 1]].getSurvivalValue();
		if (memoryType == NONE) {
			//do nothing
		} else {
			for (int j = 0, size = v1.size(); j < size; j++) {
				mutMatr->copyElements(PRIO, pairs[i], pairs[i + 1],
					v1[j], v1[j] + 1);
				crMatr->copyElements(PRIO, pairs[i], pairs[i + 1],
					v1[j], v1[j] + 1);
				crMatr->useChangeStrategy(PRIO, pairs[i],
					v1[j], v1[j] + 1, before1, after1);
				crMatr->useChangeStrategy(PRIO, pairs[i + 1],
					v1[j], v1[j] + 1, before2, after2);
			}
			v1.clear();
			for (int j = 0, size = v2.size(); j < size; j++) {
				mutMatr->copyElements(PRIO, pairs[i + 1], pairs[i],
					v2[j], v2[j] + 1);
				crMatr->copyElements(PRIO, pairs[i + 1], pairs[i],
					v2[j], v2[j] + 1);
				crMatr->useChangeStrategy(PRIO, pairs[i],
					v2[j], v2[j] + 1, before1, after1);
				crMatr->useChangeStrategy(PRIO, pairs[i + 1],
					v2[j], v2[j] + 1, before2, after2);
			}
			v2.clear();
			for (int j = 0, size = v3.size(); j < size; j++) {
				mutMatr->swapElements(PRIO, pairs[i], pairs[i + 1],
					v3[j], v3[j] + 1);
				crMatr->swapElements(PRIO, pairs[i], pairs[i + 1],
					v3[j], v3[j] + 1);
				crMatr->useChangeStrategy(PRIO, pairs[i],
					v3[j], v3[j] + 1, before1, after1);
				crMatr->useChangeStrategy(PRIO, pairs[i + 1],
					v3[j], v3[j] + 1, before2, after2);
			}
			v3.clear();
		}
	}
}