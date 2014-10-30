#include "ChangeMemoryStrategies.hpp"

static void pruneElement(double &element) {
	if (element < MIN_MEM_PARAM_VALUE) {
		element = MIN_MEM_PARAM_VALUE;
	} else if (element > MAX_MEM_PARAM_VALUE) {
		element = MAX_MEM_PARAM_VALUE;
	}
}

void AbsoluteStrategy::changeElement(MatrixDouble &memMatr,
		int row, int start, int end, double before, double after) {
	double add = 0.0;

	if (before < after) {
		add -= MP;
	} else if (before > after) {
		add += MP;
	}

	for (int i = start; i < end; i++) {
		memMatr[row][i] += add;
		pruneElement(memMatr[row][i]);
	}
}

void RelativeStrategy::changeElement(MatrixDouble &memMatr,
		int row, int start, int end, double before, double after) {
	for (int i = start; i < end; i++) {
		memMatr[row][i] = before - after;
		pruneElement(memMatr[row][i]);
	}
}

void ForgettingStrategy::changeElement(MatrixDouble &memMatr,
		int row, int start, int end, double before, double after) {
	for (int i = start; i < end; i++) {
		memMatr[row][i] = REMEMBERING_POWER * memMatr[row][i] + (before - after);
		pruneElement(memMatr[row][i]);
	}	
}
