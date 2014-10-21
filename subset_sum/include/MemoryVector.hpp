#ifndef MEMORYVECTOR_HPP_
#define MEMORYVECTOR_HPP_

#include <stddef.h>
#include <iostream>
#include <vector>

#include "GA.hpp"

using std::unique_ptr;
using std::vector;
using std::ostream;

typedef vector<vector<double> > MemoryMatrix;

/*========================INTERFACES=============================*/

class IChangeStrategy {
public:
	virtual ~IChangeStrategy() {}
	virtual void changeElement(int row, int index,
			double before, double after,
			MemoryMatrix &mutMem, MemoryMatrix &crMem) = 0;
};

class IChangeContext {
protected:
    unique_ptr<IChangeStrategy> operation;
public: 
	virtual ~IChangeContext() {}
	virtual void setChangeStrategy(unique_ptr<IChangeStrategy> _op) = 0;
	virtual void useChangeStrategy(int row, int index,
			double before, double after) = 0;
};

/*===============================================================*/

class MemoryVector {
private:
	vector<double> c1;
	vector<double> c2;
	vector<double> c3;
	int size;
	vector<double> elements;
public:
	MemoryVector();
	// TODO: ...
	~MemoryVector();

	double getElement();
	void print(ostream &out);
	void changeElement(int index, double k1Before, double k1After,
			double k2Before, double k2After, double qBefore, double qAfter);
};

#endif /* MEMORYVECTOR_HPP_ */