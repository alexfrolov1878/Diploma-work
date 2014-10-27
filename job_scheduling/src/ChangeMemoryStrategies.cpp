#include "ChangeMemoryStrategies.hpp"

static void pruneElement(double &element) {
	if (element < MIN_MEM_PARAM_VALUE) {
		element = MIN_MEM_PARAM_VALUE;
	} else if (element > MAX_MEM_PARAM_VALUE) {
		element = MAX_MEM_PARAM_VALUE;
	}
}

void AbsoluteStrategy::changeElement(MatrixDouble &memMatr,
		int row, int index, double before, double after) {
	if (before < after) {
		memMatr[row][index] -= MP;
	} else if (before > after) {
		memMatr[row][index] += MP;
	}
	pruneElement(memMatr[row][index]);
}

void RelativeStrategy::changeElement(MatrixDouble &memMatr,
		int row, int index, double before, double after) {
	memMatr[row][index] += before - after;
	pruneElement(memMatr[row][index]);
}

void ForgettingStrategy::changeElement(MatrixDouble &memMatr,
		int row, int index, double before, double after) {
	memMatr[row][index] = REMEMBERING_POWER * memMatr[row][index] + (before - after);
	pruneElement(memMatr[row][index]);
}
