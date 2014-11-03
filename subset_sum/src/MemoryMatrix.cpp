#include <cmath>

#include "MemoryMatrix.hpp"
#include "main.hpp"
#include "ChangeMemoryStrategies.hpp"

using std::vector;
using std::ostream;
using std::endl;

inline void pruneElement(double &element) {
	if (element < MIN_MEM_PARAM_VALUE) {
		element = MIN_MEM_PARAM_VALUE;
	} else if (element > MAX_MEM_PARAM_VALUE) {
		element = MAX_MEM_PARAM_VALUE;
	}
}

MemoryMatrix::MemoryMatrix(const MemoryType &type, double value, int _rows, int _columns) {
	rows = _rows;
	columns = _columns;
	elements = MatrixDouble(rows, vector<double>(columns, value));

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

double MemoryMatrix::getElement(int row, int index) {
	return elements[row][index];
}

void MemoryMatrix::swapElements(int index1, int index2, int start, int end) {
	double tmp;
	for (int i = start; i < end; i++) {
		tmp = elements[index1][i];
		elements[index1][i] = elements[index2][i];
		elements[index2][i] = tmp;
	}
}

void MemoryMatrix::copyElements(int index1, int index2, int start, int end) {
	for (int i = start; i < end; i++) {
		elements[index2][i] = elements[index1][i];
	}
}

void MemoryMatrix::print(ostream &out) {
	out << "Probability values: " << endl;
	for (int i = 0; i < rows; i++) {
		out << "ROW " << i + 1 << ": " << endl;
		for (int j = 0; j < columns; j++) {
			out << elements[i][j] << " ";
		}
		out << endl;
	}
	out << endl;
}

void MemoryMatrix::setChangeStrategy(unique_ptr<IChangeStrategy> _op) {
	operation = move(_op);
}

void MemoryMatrix::useChangeStrategy(int row, int start,
		int end, double before, double after) {
	operation->changeElement(elements, row, start, end, before, after);
}