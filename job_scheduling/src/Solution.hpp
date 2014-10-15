#ifndef SOLUTION_HPP_
#define SOLUTION_HPP_

#include <iostream>
#include <queue>
#include <vector>

#include "../GA.hpp"
#include "MemoryVector.hpp"

using std::priority_queue;
using std::vector;
using std::ostream;

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
	void setIndex(int _index) {
		index = _index;
	}
	int getValue() const {
		return value;
	}
	void setValue(int _value) {
		value = _value;
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

typedef priority_queue<Priority, vector<Priority>, PriorityComparison> Mypq;

class Solution {
private:
	vector<int> tasks;
	vector<int> priorities;
	int time;
	double downtime;
	double survivalValue;
	int sumOfComputationalComplexity;
public:
	Solution();
	Solution(const Solution &that);
	Solution& operator=(Solution that);
	~Solution();

	const vector<int> &getTasks() const;
	int getTask(int index) const;
	void setTask(int index, int value);
	const vector<int> &getPriorities() const;
	int getPriority(int index) const;
	void setPriority(int index, int value);
	double getDowntime() const;
	void setDowntime(double _downtime);
	int getTime() const;
	void setTime(int _time);
	double getSurvivalValue() const;
	void setSurvivalValue(double _survivalValue);
	int getSumOfComputationalComplexity() const;
	void setSumOfComputationalComplexity(int _sumOfComputationalComplexity);
	double getMemoryElement(SolutionPart part, int offset = 0) const;

	void print(ostream &out);
	void generate();
	void buildSchedule(vector<Process> &initProcesses);
	void mutate(SolutionPart part, int index);
};

#endif /* SOLUTION_HPP_ */