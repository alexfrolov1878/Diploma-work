#include "MemoryVector.hpp"

#include <cmath>

#include "main.hpp"

/*=====================BASE CLASS=================================*/
MemoryVector::MemoryVector() {
	size = 0;
}
MemoryVector::~MemoryVector() {
}

void MemoryVector::setMemoryVector(MemoryVector* memoryVector) {
	size = memoryVector->size;
	elements.swap(memoryVector->elements);
}

void MemoryVector::pruneElement(double &element) {
	if (element < MIN_MEM_PARAM_VALUE) {
		element = MIN_MEM_PARAM_VALUE;
	} else if (element > MAX_MEM_PARAM_VALUE) {
		element = MAX_MEM_PARAM_VALUE;
	}
}
/*================================================================*/

/*==================DERIVED LEVEL 1 CLASSES============================*/
/*=======================MICROSCOPIC===================================*/
MicroscopicMemoryVector::MicroscopicMemoryVector() {
	size = 2 * numProcesses + 2;
	int from1 = 0;
	int from2 = numProcesses;
	int from3 = 2 * numProcesses;
	int from4 = 2 * numProcesses + 1;

	for (int i = from1; i < from2; i++) {
		elements.push_back(MUTATION_TASK_PROBABILITY);
	}
	for (int i = from2; i < from3; i++) {
		elements.push_back(MUTATION_PRIO_PROBABILITY);
	}
	for (int i = from3; i < from4; i++) {
		elements.push_back(CROSSOVER_TASK_PROBABILITY);
	}
	for (int i = from4; i < size; i++) {
		elements.push_back(CROSSOVER_PRIO_PROBABILITY);
	}
}
MicroscopicMemoryVector::MicroscopicMemoryVector(
		MicroscopicMemoryVector &that) {
	size = that.size;
	elements.swap(that.elements);
}
MicroscopicMemoryVector::~MicroscopicMemoryVector() {
	elements.clear();
}

void MicroscopicMemoryVector::print(std::ostream &out) {
	out << "Mutation task probabilities: ";
	for (int i = 0; i < numProcesses; i++) {
		out << elements[i] << " ";
	}
	out << std::endl;
	out << "Mutation prio probabilities: ";
	for (int i = numProcesses; i < 2 * numProcesses; i++) {
		out << elements[i] << " ";
	}
	out << std::endl;
	out << "Crossover task probability: " << elements[2 * numProcesses]
			<< std::endl;
	out << "Crossover prio probability: " << elements[2 * numProcesses + 1]
			<< std::endl;
}
double MicroscopicMemoryVector::getElement(SolutionPart part, int offset) {
	switch (part) {
		case MUTATION_PRIO:
			offset += numProcesses;
			break;
		case CROSSOVER_TASK:
			offset += 2 * numProcesses;
			break;
		case CROSSOVER_PRIO:
			offset += 2 * numProcesses + 1;
			break;
		default:
			break;
	}
	return elements[offset];
}
/*=====================================================================*/
/*=======================MACROSCOPIC===================================*/
MacroscopicMemoryVector::MacroscopicMemoryVector() {
	size = 2;
	elements.push_back(MACROSCOPIC_CROSSOVER_PROBABILITY);
	elements.push_back(MACROSCOPIC_MUTATION_PROBABILITY);
	c1 = new double[size];
	c1[0] = MACROSCOPIC_C1_MUTATION;
	c1[1] = MACROSCOPIC_C1_CROSSOVER;
	c2 = new double[size];
	c2[0] = MACROSCOPIC_C2_MUTATION;
	c2[1] = MACROSCOPIC_C2_CROSSOVER;
	c3 = new double[size];
	c3[0] = MACROSCOPIC_C3_MUTATION;
	c3[1] = MACROSCOPIC_C3_CROSSOVER;
}
MacroscopicMemoryVector::~MacroscopicMemoryVector() {
	elements.clear();
	delete[] c1;
	delete[] c2;
	delete[] c3;
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

/*==================DERIVED LEVEL 2 CLASSES============================*/
MemoryType AbsoluteMemoryVector::getType() const {
	return type;
}
void AbsoluteMemoryVector::changeElement(int index, double before,
		double after) {
	if (before < after) {
		elements[index] -= MP;
	} else if (before > after) {
		elements[index] += MP;
	}

	pruneElement(elements[index]);
}

MemoryType RelativeMemoryVector::getType() const {
	return type;
}
void RelativeMemoryVector::changeElement(int index, double before,
		double after) {
	elements[index] += before - after;
	pruneElement(elements[index]);
}

MemoryType ForgettingMemoryVector::getType() const {
	return type;
}
void ForgettingMemoryVector::changeElement(int index, double before,
		double after) {
	elements[index] = REMEMBERING_POWER * elements[index] + (before - after);
	pruneElement(elements[index]);
}
/*=====================================================================*/
