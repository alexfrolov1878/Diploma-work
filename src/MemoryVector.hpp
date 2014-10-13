#ifndef MEMORYVECTOR_HPP_
#define MEMORYVECTOR_HPP_

#include <stddef.h>
#include <iostream>
#include <vector>

#include "../GA.hpp"

using std::unique_ptr;
using std::vector;
using std::ostream;

enum SolutionPart {
	MUTATION_TASK = 0,
	MUTATION_PRIO = 1,
	CROSSOVER_TASK = 2,
	CROSSOVER_PRIO = 3
};
typedef vector<vector<double> > MemoryMatrix;

/*========================STRATEGIES=============================*/

class IChangeStrategy {
public:
	virtual ~IChangeStrategy() {}
	virtual void changeElement(SolutionPart part, int row, int index,
			double before, double after,
			MemoryMatrix &mutMem, MemoryMatrix &crMem) = 0;
};

class IChangeContext {
protected:
    unique_ptr<IChangeStrategy> operation;
public: 
	virtual ~IChangeContext() {}
	virtual void setChangeStrategy(unique_ptr<IChangeStrategy> _op) = 0;
	virtual void useChangeStrategy(SolutionPart part, int row, int index,
			double before, double after) = 0;
};

/*=======================BASE CLASSES============================*/

class MicroscopicMemoryVector : public IChangeContext {
protected:
	int rows;
	int columns;
	MemoryMatrix mutMem;
	MemoryMatrix crMem;
public:
	MicroscopicMemoryVector(const MemoryType &type);
	MicroscopicMemoryVector(MicroscopicMemoryVector &that);
	virtual ~MicroscopicMemoryVector();

	vector<double> getMutRow(int index);
	void setMutRow(int index, vector<double> row);
	void swapMutElements(SolutionPart part, int index1, int index2, int start);

	double getElement(SolutionPart part, int row, int column = 0);
	void print(ostream &out, int row);

	virtual void setChangeStrategy(unique_ptr<IChangeStrategy> _operation);
	virtual void useChangeStrategy(SolutionPart part, int row, int index,
			double before, double after);
};

class MacroscopicMemoryVector {
private:
	static const MemoryType type = MACROSCOPIC;
	vector<double> c1;
	vector<double> c2;
	vector<double> c3;
	int size;
	vector<double> elements;
public:
	MacroscopicMemoryVector();
	~MacroscopicMemoryVector();

	double getElement(SolutionPart part);
	void print(ostream &out);

	MemoryType getType() const;
	void changeElement(int index, double k1Before, double k1After,
			double k2Before, double k2After, double qBefore, double qAfter);
};

/*=====================================================================*/

#endif /* MEMORYVECTOR_HPP_ */
