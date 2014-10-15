#include "ChangeMemoryStrategies.hpp"

static void pruneElement(double &element) {
	if (element < MIN_MEM_PARAM_VALUE) {
		element = MIN_MEM_PARAM_VALUE;
	} else if (element > MAX_MEM_PARAM_VALUE) {
		element = MAX_MEM_PARAM_VALUE;
	}
}

void AbsoluteStrategy::changeElement(SolutionPart part, int row, int index,
		double before, double after, MemoryMatrix &mutMem, MemoryMatrix &crMem) {
	switch (part) {
		case MUTATION_TASK:
		case MUTATION_PRIO:
			if (before < after) {
				mutMem[row][index] -= MP;
			} else if (before > after) {
				mutMem[row][index] += MP;
			}
			pruneElement(mutMem[row][index]);
			break;
		case CROSSOVER_TASK:
		case CROSSOVER_PRIO:
			if (before < after) {
				crMem[row][index] -= MP;
			} else if (before > after) {
				crMem[row][index] += MP;
			}
			pruneElement(crMem[row][index]);
			break;
		default:
			break;
	}
}

void RelativeStrategy::changeElement(SolutionPart part, int row, int index,
		double before, double after, MemoryMatrix &mutMem, MemoryMatrix &crMem) {
	switch (part) {
		case MUTATION_TASK:
		case MUTATION_PRIO:
			mutMem[row][index] += before - after;
			pruneElement(mutMem[row][index]);
			break;
		case CROSSOVER_TASK:
		case CROSSOVER_PRIO:
			crMem[row][index] += before - after;
			pruneElement(crMem[row][index]);
			break;
		default:
			break;
	}
}

void ForgettingStrategy::changeElement(SolutionPart part, int row, int index,
		double before, double after, MemoryMatrix &mutMem, MemoryMatrix &crMem) {
	switch (part) {
		case MUTATION_TASK:
		case MUTATION_PRIO:
			mutMem[row][index] = REMEMBERING_POWER * mutMem[row][index]
					+ (before - after);
			pruneElement(mutMem[row][index]);
			break;
		case CROSSOVER_TASK:
		case CROSSOVER_PRIO:
			crMem[row][index] = REMEMBERING_POWER * crMem[row][index]
					+ (before - after);
			pruneElement(crMem[row][index]);
			break;
		default:
			break;
	}
}
