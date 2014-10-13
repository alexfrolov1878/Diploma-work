#include <cmath>

#include "MemoryVector.hpp"
#include "main.hpp"
#include "ChangeMemoryStrategies.hpp"

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

/*=======================MICROSCOPIC===================================*/
MicroscopicMemoryVector::MicroscopicMemoryVector(const MemoryType &type) {
	vector<double> tmp;
	rows = NUM_SOLUTIONS;
	columns = 2 * numProcesses;

	mutMem.clear();
	for (int i = 0; i < rows; i++) {
		tmp.clear();
		for (int j = 0; j < numProcesses; j++) {
			tmp.push_back(MUTATION_TASK_PROBABILITY);
		}
		for (int j = 0; j < numProcesses; j++) {
			tmp.push_back(MUTATION_PRIO_PROBABILITY);
		}
		mutMem.push_back(tmp);
	}

	crMem.clear();
	for (int i = 0; i < rows; i++) {
		tmp.clear();
		tmp.push_back(CROSSOVER_TASK_PROBABILITY);
		tmp.push_back(CROSSOVER_PRIO_PROBABILITY);
		crMem.push_back(tmp);
	}

	switch (type) {
		case ABSOLUTE:
			setChangeStrategy(unique_ptr<IChangeStrategy>(
				new AbsoluteStrategy())
			);
			break;
		case RELATIVE:
			setChangeStrategy(unique_ptr<IChangeStrategy>(
				new RelativeStrategy())
			);
			break;
		case FORGETTING:
			setChangeStrategy(unique_ptr<IChangeStrategy>(
				new ForgettingStrategy())
			);
			break;
		default:
			setChangeStrategy(unique_ptr<IChangeStrategy>(
				new AbsoluteStrategy())
			);
			break;
	}
}
MicroscopicMemoryVector::MicroscopicMemoryVector(
		MicroscopicMemoryVector &that) {
	rows = that.rows;
	columns = that.columns;
	mutMem.swap(that.mutMem);
	crMem.swap(that.crMem);
}
MicroscopicMemoryVector::~MicroscopicMemoryVector() {
	mutMem.clear();
	crMem.clear();
}

vector<double> MicroscopicMemoryVector::getMutRow(int index) {
	return mutMem[index];
}

void MicroscopicMemoryVector::setMutRow(int index, vector<double> row) {
	mutMem[index] = row;
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
		tmp = mutMem[index1][i];
		mutMem[index1][i] = mutMem[index2][i];
		mutMem[index2][i] = tmp;
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
	return mutMem[row][index];
}

void MicroscopicMemoryVector::print(ostream &out, int row) {
	out << "Mutation task probabilities: ";
	for (int i = 0; i < numProcesses; i++) {
		out << mutMem[row][i] << " ";
	}
	out << endl;
	out << "Mutation prio probabilities: ";
	for (int i = numProcesses; i < 2 * numProcesses; i++) {
		out << mutMem[row][i] << " ";
	}
	out << endl;
	out << "Crossover task probability: " << crMem[row][0] << endl;
	out << "Crossover prio probability: " << crMem[row][1] << endl;
}

void MicroscopicMemoryVector::setChangeStrategy(
		unique_ptr<IChangeStrategy> _op) {
	operation = move(_op);
}

void MicroscopicMemoryVector::useChangeStrategy(SolutionPart part,
		int row, int index,	double before, double after) {
	operation->changeElement(part, row, index, before, after, mutMem, crMem);
}
/*=====================================================================*/
/*=======================MACROSCOPIC===================================*/
MacroscopicMemoryVector::MacroscopicMemoryVector() {
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
MacroscopicMemoryVector::~MacroscopicMemoryVector() {
	elements.clear();
	c1.clear();
	c2.clear();
	c3.clear();
}

MemoryType MacroscopicMemoryVector::getType() const {
	return type;
}
double MacroscopicMemoryVector::getElement(SolutionPart part) {
	if (part == MUTATION_PRIO || part == MUTATION_PRIO) {
		return elements[0];
	}
	return elements[1];
}
void MacroscopicMemoryVector::changeElement(int index,
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
void MacroscopicMemoryVector::print(ostream &out) {
	out << "Mutation probability: " << elements[0] << endl;
	out << "Crossover probability: " << elements[1] << endl;
}
/*=====================================================================*/
