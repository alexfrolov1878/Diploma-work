#include "MemoryVector.hpp"

#include <cmath>

#include "main.hpp"

void pruneElement(double &element) {
	if (element < MIN_MEM_PARAM_VALUE) {
		element = MIN_MEM_PARAM_VALUE;
	} else if (element > MAX_MEM_PARAM_VALUE) {
		element = MAX_MEM_PARAM_VALUE;
	}
}

/*======================BASE CLASSES============================*/
/*=======================MICROSCOPIC===================================*/
MicroscopicMemoryVector::MicroscopicMemoryVector() {
	std::vector<double> tmp;
	rows = NUM_SOLUTIONS;
	columns = 2 * numProcesses;

	mutMV.clear();
	for (int i = 0; i < rows; i++) {
		tmp.clear();
		for (int j = 0; j < numProcesses; j++) {
			tmp.push_back(MUTATION_TASK_PROBABILITY);
		}
		for (int j = 0; j < numProcesses; j++) {
			tmp.push_back(MUTATION_PRIO_PROBABILITY);
		}
		mutMV.push_back(tmp);
	}

	crMV.clear();
	for (int i = 0; i < rows; i++) {
		tmp.clear();
		tmp.push_back(CROSSOVER_TASK_PROBABILITY);
		tmp.push_back(CROSSOVER_PRIO_PROBABILITY);
		crMV.push_back(tmp);
	}
}
MicroscopicMemoryVector::MicroscopicMemoryVector(
		MicroscopicMemoryVector &that) {
	rows = that.rows;
	columns = that.columns;
	mutMV.swap(that.mutMV);
	crMV.swap(that.crMV);
}
MicroscopicMemoryVector::~MicroscopicMemoryVector() {
	mutMV.clear();
	crMV.clear();
}

std::vector<double> MicroscopicMemoryVector::getMutRow(int index) {
	return mutMV[index];
}

void MicroscopicMemoryVector::setMutRow(int index, std::vector<double> row) {
	mutMV[index] = row;
}

void MicroscopicMemoryVector::swapMutElements(SolutionPart part, int index1,
		int index2, int start) {
	int end;
	double tmp;

	if (part == CROSSOVER_TASK) {
		end = numProcesses;
	} else {
		end = 2 * numProcesses;
	}

	for (int i = start; i < end; i++) {
		tmp = mutMV[index1][i];
		mutMV[index1][i] = mutMV[index2][i];
		mutMV[index2][i] = tmp;
	}
}

double MicroscopicMemoryVector::getElement(SolutionPart part, int row,
		int index) {
	switch (part) {
		case MUTATION_PRIO:
			index += numProcesses;
			break;
		case CROSSOVER_PRIO:
			index += 1;
			break;
		default:
			break;
	}
	return mutMV[row][index];
}

void MicroscopicMemoryVector::print(std::ostream &out, int row) {
	out << "Mutation task probabilities: ";
	for (int i = 0; i < numProcesses; i++) {
		out << mutMV[row][i] << " ";
	}
	out << std::endl;
	out << "Mutation prio probabilities: ";
	for (int i = numProcesses; i < 2 * numProcesses; i++) {
		out << mutMV[row][i] << " ";
	}
	out << std::endl;
	out << "Crossover task probability: " << crMV[row][0] << std::endl;
	out << "Crossover prio probability: " << crMV[row][1] << std::endl;
}
/*=====================================================================*/
/*=======================MACROSCOPIC===================================*/
MacroscopicMemoryVector::MacroscopicMemoryVector() {
	size = 2;
	elements.push_back(MACROSCOPIC_CROSSOVER_PROBABILITY);
	elements.push_back(MACROSCOPIC_MUTATION_PROBABILITY);
	c1 = std::vector<double>(size);
	c1[0] = MACROSCOPIC_C1_MUTATION;
	c1[1] = MACROSCOPIC_C1_CROSSOVER;
	c2 = std::vector<double>(size);
	c2[0] = MACROSCOPIC_C2_MUTATION;
	c3[1] = MACROSCOPIC_C2_CROSSOVER;
	c1 = std::vector<double>(size);
	c3[0] = MACROSCOPIC_C3_MUTATION;
	c3[1] = MACROSCOPIC_C3_CROSSOVER;
}
MacroscopicMemoryVector::~MacroscopicMemoryVector() {
	elements.clear();
	c1.clear();
	c2.clear();
	c3.clear();
}

MemoryType MacroscopicMemoryVector::getType() const {
	return type;
}
double MacroscopicMemoryVector::getElement(SolutionPart part, int offset) {
	if (part == MUTATION_PRIO || part == MUTATION_PRIO) {
		return elements[0];
	}
	return elements[1];
}
void MacroscopicMemoryVector::changeElement(int index, double goal,
		double k1Before, double k1After, double k2Before, double k2After,
		double qBefore, double qAfter) {
	double s;
	if (fabs(k1After - goal) < G_NEIGHBOURHOOD_RADIUS * goal) {
		s = c2[index] * (k2Before - k2After) / numWeights / numWeights;
	} else {
		s = c3[index] * (qBefore - qAfter);
	}
	double t = c1[index] * (fabs(k1Before - goal) - fabs(k1After - goal))
			/ numWeights + s;
	elements[index] += t;
	pruneElement(elements[index]);
}
void MacroscopicMemoryVector::print(std::ostream &out) {
	out << "Mutation probability: " << elements[0] << std::endl;
	out << "Crossover probability: " << elements[1] << std::endl;
}
/*=====================================================================*/

/*==================DERIVED LEVEL 1 CLASSES============================*/
AbsoluteMemoryVector& AbsoluteMemoryVector::operator=(
		AbsoluteMemoryVector that) {
	rows = that.rows;
	columns = that.columns;
	mutMV.swap(that.mutMV);
	crMV.swap(that.crMV);
	return *this;
}
MemoryType AbsoluteMemoryVector::getType() const {
	return type;
}
void AbsoluteMemoryVector::changeElement(SolutionPart part, int row, int index,
		double before, double after) {
	switch (part) {
		case MUTATION_TASK:
		case MUTATION_PRIO:
			if (before < after) {
				mutMV[row][index] -= MP;
			} else if (before > after) {
				mutMV[row][index] += MP;
			}
			pruneElement(mutMV[row][index]);
			break;
		case CROSSOVER_TASK:
		case CROSSOVER_PRIO:
			if (before < after) {
				crMV[row][index] -= MP;
			} else if (before > after) {
				crMV[row][index] += MP;
			}
			pruneElement(crMV[row][index]);
			break;
		default:
			break;
	}
}

RelativeMemoryVector& RelativeMemoryVector::operator=(
		RelativeMemoryVector that) {
	rows = that.rows;
	columns = that.columns;
	mutMV.swap(that.mutMV);
	crMV.swap(that.crMV);
	return *this;
}
MemoryType RelativeMemoryVector::getType() const {
	return type;
}
void RelativeMemoryVector::changeElement(SolutionPart part, int row, int index,
		double before, double after) {
	switch (part) {
		case MUTATION_TASK:
		case MUTATION_PRIO:
			mutMV[row][index] += before - after;
			pruneElement(mutMV[row][index]);
			break;
		case CROSSOVER_TASK:
		case CROSSOVER_PRIO:
			crMV[row][index] += before - after;
			pruneElement(crMV[row][index]);
			break;
		default:
			break;
	}
}

ForgettingMemoryVector& ForgettingMemoryVector::operator=(
		ForgettingMemoryVector that) {
	rows = that.rows;
	columns = that.columns;
	mutMV.swap(that.mutMV);
	crMV.swap(that.crMV);
	return *this;
}
MemoryType ForgettingMemoryVector::getType() const {
	return type;
}
void ForgettingMemoryVector::changeElement(SolutionPart part, int row,
		int index, double before, double after) {
	switch (part) {
		case MUTATION_TASK:
		case MUTATION_PRIO:
			mutMV[row][index] = REMEMBERING_POWER * mutMV[row][index]
					+ (before - after);
			pruneElement(mutMV[row][index]);
			break;
		case CROSSOVER_TASK:
		case CROSSOVER_PRIO:
			crMV[row][index] = REMEMBERING_POWER * crMV[row][index]
					+ (before - after);
			pruneElement(crMV[row][index]);
			break;
		default:
			break;
	}
}
/*=====================================================================*/
