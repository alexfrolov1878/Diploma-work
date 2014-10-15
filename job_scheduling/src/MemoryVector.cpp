#include <cmath>

#include "MemoryVector.hpp"
#include "main.hpp"
#include "ChangeMemoryStrategies.hpp"

using std::vector;
using std::ostream;
using std::move;
using std::endl;

/*
static void pruneElement(double &element) {
	if (element < MIN_MEM_PARAM_VALUE) {
		element = MIN_MEM_PARAM_VALUE;
	} else if (element > MAX_MEM_PARAM_VALUE) {
		element = MAX_MEM_PARAM_VALUE;
	}
}
*/

MemoryVector::MemoryVector(const MemoryType &type) {
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
MemoryVector::MemoryVector(MemoryVector &that) {
	rows = that.rows;
	columns = that.columns;
	mutMem.swap(that.mutMem);
	crMem.swap(that.crMem);
}
MemoryVector::~MemoryVector() {
	mutMem.clear();
	crMem.clear();
}

vector<double> MemoryVector::getMutRow(int index) {
	return mutMem[index];
}

void MemoryVector::setMutRow(int index, vector<double> row) {
	mutMem[index] = row;
}

void MemoryVector::swapMutElements(SolutionPart part, int index1,
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

double MemoryVector::getElement(SolutionPart part, int row, int index) {
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

void MemoryVector::print(ostream &out, int row) {
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

void MemoryVector::setChangeStrategy(unique_ptr<IChangeStrategy> _op) {
	operation = move(_op);
}

void MemoryVector::useChangeStrategy(SolutionPart part, int row, int index,
		double before, double after) {
	operation->changeElement(part, row, index, before, after, mutMem, crMem);
}