#include <cmath>

#include "MemoryMatrix.hpp"
#include "main.hpp"
#include "ChangeMemoryStrategies.hpp"

using std::vector;
using std::ostream;
using std::endl;

MemoryMatrix::MemoryMatrix(const MemoryType &type, int _rows, int _columns) {
	rows = _rows;
	columns = _columns;

	elements = MatrixDouble(rows, vector<double>(columns, 0.0));
	vector<double> tmp(columns);
	for (int i = 0; i < rows; i++) {
		tmp.clear();
		for (int j = 0; j < columns; j++) {
			tmp.push_back(MUTATION_PROBABILITY);
		}
		elements.push_back(tmp);
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
MemoryMatrix::MemoryMatrix(MemoryMatrix &that) {
	rows = that.rows;
	columns = that.columns;
	elements.swap(that.elements);
}
MemoryMatrix::~MemoryMatrix() {
	elements.clear();
}

vector<double> &MemoryMatrix::getRow(int index) {
	return elements[index];
}

void MemoryMatrix::setRow(int index, vector<double> &row) {
	elements[index] = row;
}

double MemoryMatrix::getElement(SolutionPart part, int row, int index) {
	if (part == PRIO) {
		index += numProcesses;
	}
	return elements[row][index];
}

void MemoryMatrix::swapElements(SolutionPart part, int index1, int index2, int start, int end) {
	double tmp;

	if (part == PRIO) {
		start += numProcesses;
		end += numProcesses;
	}

	for (int i = start; i < end; i++) {
		tmp = elements[index1][i];
		elements[index1][i] = elements[index2][i];
		elements[index2][i] = tmp;
	}
}

void MemoryMatrix::copyElements(SolutionPart part, int index1, int index2, int start, int end) {
	if (part == PRIO) {
		start += numProcesses;
		end += numProcesses;
	}

	for (int i = start; i < end; i++) {
		elements[index2][i] = elements[index1][i];
	}
}

void MemoryMatrix::print(ostream &out, int row) {
	out << "Task probabilities: ";
	for (int i = 0; i < numProcesses; i++) {
		out << elements[row][i] << " ";
	}
	out << endl;
	out << "Prio probabilities: ";
	for (int i = numProcesses; i < 2 * numProcesses; i++) {
		out << elements[row][i] << " ";
	}
	out << endl;
}

void MemoryMatrix::setChangeStrategy(unique_ptr<IChangeStrategy> _op) {
	operation = move(_op);
}

void MemoryMatrix::useChangeStrategy(SolutionPart part, int row, int start,
		int end, double before, double after) {
	if (part == PRIO) {
		start += numProcesses;
		end += numProcesses;
	}
	operation->changeElement(elements, row, start, end, before, after);
}