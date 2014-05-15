#ifndef SOLUTION_HPP_
#define SOLUTION_HPP_

#include <iostream>
#include <queue>
#include <vector>

#include "../library/GA.hpp"
#include "MemoryVector.hpp"

class Process;

class Priority {
private:
	int value;
	int index;
public:
	Priority(int v, int i)
			: value(v), index(i) {

	}
	int getIndex() const {
		return index;
	}
	void setIndex(int index) {
		this->index = index;
	}
	int getValue() const {
		return value;
	}
	void setValue(int value) {
		this->value = value;
	}
};
class PriorityComparison {
public:
	PriorityComparison() {
	}
	bool operator()(const Priority &p1, const Priority &p2) const {
		return p1.getValue() > p2.getValue();
	}
};
typedef std::priority_queue<Priority, std::vector<Priority>, PriorityComparison> Mypq;

/*=============================MICROSCOPIC===================================*/

class MicroscopicSolution {
private:
	int *tasks;
	int *priorities;
	int time;
	double downtime;
	double survivalValue;
	int sumOfComputationalComplexity;
	MemoryType memoryType;
	MicroscopicMemoryVector *microscopicMemoryVector;
public:
	MicroscopicSolution(MemoryType memoryType);
	MicroscopicSolution(const MicroscopicSolution &that);
	~MicroscopicSolution();

	int *getTasks() const;
	int getTask(int index) const;
	void setTask(int index, int value);
	int* getPriorities() const;
	int getPriority(int index) const;
	void setPriority(int index, int value);
	double getDowntime() const;
	void setDowntime(double downtime);
	int getTime() const;
	void setTime(int time);
	double getSurvivalValue() const;
	void setSurvivalValue(double survivalValue);
	int getSumOfComputationalComplexity() const;
	void setSumOfComputationalComplexity(int sumOfComputationalComplexity);
	double getMemoryElement(SolutionPart part, int offset = 0) const;

	void print(std::ostream &out);
	void generate();
	void buildSchedule(Process *initProcesses);
	void mutate(SolutionPart part, int index);
	void updateMemoryElement(SolutionPart part, double before, double after,
			int offset = 0);
};

/*=============================MACROSCOPIC===================================*/

class MacroscopicSolution {
private:
	int *indicators;
	double field;
	double survivalValue;
public:
	MacroscopicSolution();
	MacroscopicSolution(const MacroscopicSolution &that);
	~MacroscopicSolution();

	int getIndicator(int index) const;
	double getField() const;
	void setField(double field);
	double getSurvivalValue() const;
	void setSurvivalValue(double survivalValue);

	void print(std::ostream &out);
	void generate();
	void buildField(double *weights);
	void crossover(MacroscopicSolution &that);
	void mutate();
};

#endif /* SOLUTION_HPP_ */
