#ifndef MEMORYVECTOR_HPP_
#define MEMORYVECTOR_HPP_

#include <stddef.h>
#include <iostream>
#include <vector>

#include "../library/GA.hpp"

//const double MIN_MEM_PARAM_VALUE = 0.1;
//const double MAX_MEM_PARAM_VALUE = 0.9;
//const double MP = 0.05;
//const double REMEMBERING_POWER = 0.5;

//const double MACROSCOPIC_CROSSOVER_PROBABILITY = 0.5;
//const double MACROSCOPIC_MUTATION_PROBABILITY = 0.5;
//const double MACROSCOPIC_C1_CROSSOVER = 0.5;
//const double MACROSCOPIC_C1_MUTATION = 0.5;
//const double MACROSCOPIC_C2_CROSSOVER = 0.5;
//const double MACROSCOPIC_C2_MUTATION = 0.5;
//const double MACROSCOPIC_C3_CROSSOVER = 0.5;
//const double MACROSCOPIC_C3_MUTATION = 0.5;
//const double G_NEIGHBOURHOOD_RADIUS = 0.2;

enum SolutionPart {
	MUTATION_TASK = 0, MUTATION_PRIO = 1, CROSSOVER_TASK = 2, CROSSOVER_PRIO = 3
};

/*=====================BASE CLASS=================================*/
class MemoryVector {
protected:
	int size;
	std::vector<double> elements;
public:
	MemoryVector();
	virtual ~MemoryVector();

	void setMemoryVector(MemoryVector *memoryVector);

	void pruneElement(double &element);
	virtual void print(std::ostream &out) = 0;
	virtual MemoryType getType() const = 0;
};
/*================================================================*/

/*==================DERIVED LEVEL 1 CLASSES============================*/
class MicroscopicMemoryVector: public MemoryVector {
public:
	MicroscopicMemoryVector();
	MicroscopicMemoryVector(MicroscopicMemoryVector &that);
	virtual ~MicroscopicMemoryVector();

	virtual void print(std::ostream &out);
	double getElement(SolutionPart part, int offset = 0);
	virtual void changeElement(int index, double before, double after) = 0;
};

class MacroscopicMemoryVector: public MemoryVector {
private:
	static const MemoryType type = MACROSCOPIC;
	double *c1;
	double *c2;
	double *c3;
public:
	MacroscopicMemoryVector();
	~MacroscopicMemoryVector();

	virtual MemoryType getType() const;
	void print(std::ostream &out);
	double getElement(SolutionPart part, int offset = 0);
	void changeElement(int index, double goal, double k1Before, double k1After,
			double k2Before, double k2After, double qBefore, double qAfter);
};
/*=====================================================================*/

/*==================DERIVED LEVEL 2 CLASSES============================*/
class AbsoluteMemoryVector: public MicroscopicMemoryVector {
private:
	static const MemoryType type = ABSOLUTE;
public:
	virtual MemoryType getType() const;
	virtual void changeElement(int index, double before, double after);
};

class RelativeMemoryVector: public MicroscopicMemoryVector {
private:
	static const MemoryType type = RELATIVE;
public:
	virtual MemoryType getType() const;
	virtual void changeElement(int index, double before, double after);
};

class ForgettingMemoryVector: public MicroscopicMemoryVector {
private:
	static const MemoryType type = FORGETTING;
public:
	virtual MemoryType getType() const;
	virtual void changeElement(int index, double before, double after);
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
