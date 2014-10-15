#include <cmath>

#include "MemoryVector.hpp"
#include "main.hpp"

using std::vector;
using std::ostream;
using std::move;
using std::endl;

static void pruneElement(double &element) {
	if (element < MIN_MEM_PARAM_VALUE) {
		element = MIN_MEM_PARAM_VALUE;
	} else if (element > MAX_MEM_PARAM_VALUE) {
		element = MAX_MEM_PARAM_VALUE;
	}
}

MemoryVector::MemoryVector() {
	size = 2;
	elements.push_back(MACROSCOPIC_CROSSOVER_PROBABILITY);
	elements.push_back(MACROSCOPIC_MUTATION_PROBABILITY);
	c1 = vector<double>(size);
	c1[0] = MACROSCOPIC_C1_MUTATION;
	c1[1] = MACROSCOPIC_C1_CROSSOVER;
	c2 = vector<double>(size);
	c2[0] = MACROSCOPIC_C2_MUTATION;
	c3[1] = MACROSCOPIC_C2_CROSSOVER;
	c1 = vector<double>(size);
	c3[0] = MACROSCOPIC_C3_MUTATION;
	c3[1] = MACROSCOPIC_C3_CROSSOVER;
}
MemoryVector::~MemoryVector() {
	elements.clear();
	c1.clear();
	c2.clear();
	c3.clear();
}

double MemoryVector::getElement(SolutionPart part) {
	if (part == MUTATION_PRIO || part == MUTATION_PRIO) {
		return elements[0];
	}
	return elements[1];
}
void MemoryVector::changeElement(int index,
		double k1Before, double k1After,
		double k2Before, double k2After,
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
void MemoryVector::print(ostream &out) {
	out << "Mutation probability: " << elements[0] << endl;
	out << "Crossover probability: " << elements[1] << endl;
}