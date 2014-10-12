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

/*=======================BASE CLASSES============================*/
class MicroscopicMemoryVector {
protected:
	int rows;
	int columns;
	vector<vector<double> > mutMV;
	vector<vector<double> > crMV;
public:
	MicroscopicMemoryVector();
	MicroscopicMemoryVector(MicroscopicMemoryVector &that);
	virtual ~MicroscopicMemoryVector();

	vector<double> getMutRow(int index);
	void setMutRow(int index, vector<double> row);
	void swapMutElements(SolutionPart part, int index1, int index2, int start);

	double getElement(SolutionPart part, int row, int column = 0);
	void print(ostream &out, int row);

	virtual MemoryType getType() const= 0;
	virtual void changeElement(SolutionPart part, int row, int index,
			double before, double after) = 0;
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

/*==================DERIVED LEVEL 1 CLASSES============================*/
class AbsoluteMemoryVector: public MicroscopicMemoryVector {
private:
	static const MemoryType type = ABSOLUTE;
public:
	AbsoluteMemoryVector& operator=(AbsoluteMemoryVector that);
	virtual MemoryType getType() const;
	virtual void changeElement(SolutionPart part, int row, int index,
			double before, double after);
};

class RelativeMemoryVector: public MicroscopicMemoryVector {
private:
	static const MemoryType type = RELATIVE;
public:
	RelativeMemoryVector& operator=(RelativeMemoryVector that);
	virtual MemoryType getType() const;
	virtual void changeElement(SolutionPart part, int row, int index,
			double before, double after);
};

class ForgettingMemoryVector: public MicroscopicMemoryVector {
private:
	static const MemoryType type = FORGETTING;
public:
	ForgettingMemoryVector& operator=(ForgettingMemoryVector that);
	virtual MemoryType getType() const;
	virtual void changeElement(SolutionPart part, int row, int index,
			double before, double after);
};
/*=====================================================================*/

/*=====================FACTORIES=================================*/
class MicroscopicMemoryVectorFactory {
public:
	static unique_ptr<MicroscopicMemoryVector>
	newMemoryVector(const MemoryType &type) {
		switch (type) {
			case ABSOLUTE:
				return unique_ptr<MicroscopicMemoryVector>(
					new AbsoluteMemoryVector()
				);
			case RELATIVE:
				return unique_ptr<MicroscopicMemoryVector>(
					new RelativeMemoryVector()
				);
			case FORGETTING:
				return unique_ptr<MicroscopicMemoryVector>(
				 	new ForgettingMemoryVector()
				 );
			default:
				return unique_ptr<MicroscopicMemoryVector>(nullptr);
		}
	}
};
/*===============================================================*/

#endif /* MEMORYVECTOR_HPP_ */
