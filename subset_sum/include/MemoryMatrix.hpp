#ifndef MEMORYVECTOR_HPP_
#define MEMORYVECTOR_HPP_

#include <stddef.h>
#include <iostream>
#include <vector>

#include "GA.hpp"

using std::vector;
using std::ostream;

typedef vector<vector<double> > MatrixDouble;

/*========================INTERFACES=============================*/

class IMemoryMatrix {
public:
	virtual ~IMemoryMatrix() {}
	
	virtual vector<double> &getRow(int index) = 0;
	virtual void setRow(int index, vector<double> &row) = 0;
	virtual double getElement(int row, int column) = 0;
	virtual void swapElements(int index1, int index2, int start, int end) = 0;
	virtual void copyElements(int index1, int index2, int start, int end) = 0;
	virtual void print(ostream &out) = 0;
};

class IChangeStrategy {
public:
	virtual ~IChangeStrategy() {}
	virtual void changeElement(MatrixDouble &memMatr, int row, int start,
		int end, double before, double after) = 0;
};

class IChangeContext {
protected:
    unique_ptr<IChangeStrategy> operation;
public: 
	virtual ~IChangeContext() {}
	virtual void setChangeStrategy(unique_ptr<IChangeStrategy> _op) = 0;
	virtual void useChangeStrategy(int row, int start, int end,
		double before, double after) = 0;
};

/*===============================================================*/

class MemoryMatrix : public IMemoryMatrix, IChangeContext {
protected:
	int rows;
	int columns;
	MatrixDouble elements;
public:
	MemoryMatrix(const MemoryType &type, double value, int rows, int columns);
	MemoryMatrix(MemoryMatrix &that);
	virtual ~MemoryMatrix();

	virtual vector<double> &getRow(int index);
	virtual void setRow(int index, vector<double> &row);
	virtual double getElement(int row, int column);
	virtual void swapElements(int index1, int index2, int start, int end);
	virtual void copyElements(int index1, int index2, int start, int end);
	virtual void print(ostream &out);

	virtual void setChangeStrategy(unique_ptr<IChangeStrategy> _operation);
	virtual void useChangeStrategy(int row, int start, int end,
		double before, double after);
};

#endif /* MEMORYVECTOR_HPP_ */
