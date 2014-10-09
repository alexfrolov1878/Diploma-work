#ifndef MEMORYVECTOR_HPP_
#define MEMORYVECTOR_HPP_

#include <stddef.h>
#include <iostream>
#include <vector>

#include "../GA.hpp"

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
	std::vector<std::vector<double> > mutMV;
	std::vector<std::vector<double> > crMV;
public:
	MicroscopicMemoryVector();
	MicroscopicMemoryVector(MicroscopicMemoryVector &that);
	virtual ~MicroscopicMemoryVector();

	std::vector<double> getMutRow(int index);
	void setMutRow(int index, std::vector<double> row);
	void swapMutElements(SolutionPart part, int index1, int index2, int start);

	double getElement(SolutionPart part, int row, int column = 0);
	void print(std::ostream &out, int row);

	virtual MemoryType getType() const= 0;
	virtual void changeElement(SolutionPart part, int row, int index,
		double before, double after) = 0;
};

class MacroscopicMemoryVector {
private:
	static const MemoryType type = MACROSCOPIC;
	std::vector<double> c1;
	std::vector<double> c2;
	std::vector<double> c3;
	int size;
	std::vector<double> elements;
public:
	MacroscopicMemoryVector();
	~MacroscopicMemoryVector();

	double getElement(SolutionPart part, int offset = 0);
	void print(std::ostream &out);

	MemoryType getType() const;
	void changeElement(int index, double goal, double k1Before, double k1After,
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
	static MicroscopicMemoryVector *newMemoryVector(const MemoryType &type) {
		switch (type) {
			case ABSOLUTE:
				return new AbsoluteMemoryVector();
			case RELATIVE:
				return new RelativeMemoryVector();
			case FORGETTING:
				return new ForgettingMemoryVector();
			default:
				return NULL;
		}
	}
};
/*===============================================================*/

#endif /* MEMORYVECTOR_HPP_ */
