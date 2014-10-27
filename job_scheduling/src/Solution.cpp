#include <vector>

#include "Solution.hpp"
#include "main.hpp"
#include "Process.hpp"
#include "Processor.hpp"

using std::priority_queue;
using std::vector;
using std::ostream;
using std::endl;

int numProcessorsBinaryLength = -1;
int numProcessesBinaryLength = -1;
int sumComplexity = -1;

int getBinaryLength(int num) {
	if (num == numProcessors - 1) {
		if (numProcessorsBinaryLength == -1) {
			int i, tmp;
			for (tmp = 1, i = 0; num >= tmp; i++) {
				tmp *= 2;
			}
			numProcessorsBinaryLength = i;
		}
		return numProcessorsBinaryLength;
	} else if (num == numProcesses - 1) {
		if (numProcessesBinaryLength == -1) {
			int i, tmp;
			for (tmp = 1, i = 0; num >= tmp; i++) {
				tmp *= 2;
			}
			numProcessesBinaryLength = i;
		}
		return numProcessesBinaryLength;
	}
	return -1;
}

int mutateInt(int value, double power, int limit) {
	int binaryLength, n, l;

	binaryLength = getBinaryLength(limit);
	n = int ((binaryLength - 1) * (power + 0.5));
	for (int i = 0; i < n; i++) {
		l = Random::getRandomInt(0, binaryLength);
		value ^= (1 << l);
	}

	if (value < 0) {
		value += limit;
	} else if (value > limit) {
		value -= limit;
	}
	return value;
}

Solution::Solution() {
	tasks = vector<int>(numProcesses, 0);
	priorities = vector<int>(numProcesses, 0);
	time = 0;
	downtime = 0.0;
	survivalValue = 0.0;
	sumOfComputationalComplexity = 0;
}

Solution::Solution(const Solution &that) {
	tasks = that.tasks;
	priorities = that.priorities;
	time = that.time;
	downtime = that.downtime;
	survivalValue = that.survivalValue;
	sumOfComputationalComplexity = that.sumOfComputationalComplexity;
}

Solution& Solution::operator=(Solution that) {
	tasks.swap(that.tasks);
	priorities.swap(that.priorities);
	time = that.time;
	downtime = that.downtime;
	survivalValue = that.survivalValue;
	sumOfComputationalComplexity = that.sumOfComputationalComplexity;
	return *this;
}

Solution::~Solution() {
	tasks.clear();
	priorities.clear();
}

const vector<int> &Solution::getTasks() const {
	return tasks;
}

int Solution::getTask(int index) const {
	return tasks[index];
}

void Solution::setTask(int index, int value) {
	tasks[index] = value;
}

const vector<int> &Solution::getPriorities() const {
	return priorities;
}

int Solution::getPriority(int index) const {
	return priorities[index];
}

void Solution::setPriority(int index, int value) {
	priorities[index] = value;
}

double Solution::getDowntime() const {
	return downtime;
}

void Solution::setDowntime(double _downtime) {
	downtime = _downtime;
}

int Solution::getTime() const {
	return time;
}

void Solution::setTime(int _time) {
	time = _time;
}

double Solution::getSurvivalValue() const {
	return survivalValue;
}

void Solution::setSurvivalValue(double _survivalValue) {
	survivalValue = _survivalValue;
}

int Solution::getSumOfComputationalComplexity() const {
	return sumOfComputationalComplexity;
}

void Solution::setSumOfComputationalComplexity(
		int _sumOfComputationalComplexity) {
	sumOfComputationalComplexity = _sumOfComputationalComplexity;
}

void Solution::print(ostream &out) {
	out << "Task: ";
	for (int i = 0; i < numProcesses; i++) {
		out << tasks[i] << " ";
	}
	out << endl;
	out << "Prio: ";
	for (int i = 0; i < numProcesses; i++) {
		out << priorities[i] << " ";
	}
	out << endl;
	out << "time = " << time << endl;
	out << "downtime = " << downtime << endl;
	out << "survival_value = " << survivalValue << endl;
	out << "sum_of_computational_complexity = " << sumOfComputationalComplexity
			<< endl;
}

void Solution::generate() {
	int randomValue;
	for (int i = 0; i < numProcesses; i++) {
		randomValue = Random::getRandomInt(0, numProcessors);
		tasks[i] = randomValue;
		randomValue = Random::getRandomInt(0, numProcesses);
		priorities[i] = randomValue;
	}
}

void Solution::buildSchedule(vector<Process> &initProcesses) {
	enum State {
		SCHEDULED, HAS_DEPENDENCIES, NO_DEPENDENCIES
	};
	int j, maxEnd, mini, tmp1, tmp2, count, sum;
	int tempEnd, prevProc;
	vector<State> states(numProcesses);
	for (int i = 0; i < numProcesses; i++) {
		states[i] = NO_DEPENDENCIES;
	}

	vector<Process> processes(numProcesses);
	vector<Processor> processors(numProcessors);
	vector<Priority> p;

	for (int i = 0; i < numProcesses; i++) {
		tmp1 = initProcesses[i].getComputationalComplexity();
		processes[i].setComputationalComplexity(tmp1);
		tmp1 = initProcesses[i].getNumberOfPreviousProcecces();
		processes[i].setNumberOfPreviousProcecces(tmp1);
		if (tmp1 == 0) {
			states[i] = NO_DEPENDENCIES;
		} else {
			states[i] = HAS_DEPENDENCIES;
		}
		for (j = 0; j < tmp1; j++) {
			tmp2 = initProcesses[i].getPreviuosProcess(j);
			processes[i].setPreviuosProcess(j, tmp2);
		}
		p.push_back(Priority(priorities[i], i));
	}
	if (sumComplexity == -1) {
		sum = 0;
		for (int i = 0; i < numProcesses; i++) {
			sum += initProcesses[i].getComputationalComplexity();
		}
		sumComplexity = sum;
	}

	Mypq queue;
	for (int i = 0; i < numProcesses; i++) {
		if (states[i] == NO_DEPENDENCIES) {
			queue.push(p[i]);
		}
	}
	count = 0;
	while (count != numProcesses) {
		while (queue.size() > 0) {
			Priority pr = queue.top();
			queue.pop();
			mini = pr.getIndex();
			states[mini] = SCHEDULED;
			processes[mini].setProcessor(tasks[mini]);
			maxEnd = processors[tasks[mini]].getTime();
			for (j = 0; j < processes[mini].getNumberOfPreviousProcecces();
					j++) {
				prevProc = processes[mini].getPreviuosProcess(j);
				tempEnd = processes[prevProc].getTimeEnd();
				if (tempEnd > maxEnd) {
					maxEnd = tempEnd;
				}
			}
			processes[mini].setTimeStart(maxEnd);
			processes[mini].setTimeEnd(
					maxEnd + processes[mini].getComputationalComplexity());
			processors[processes[mini].getProcessor()].setTime(
					processes[mini].getTimeEnd());
			processors[processes[mini].getProcessor()].setProcess(mini, 1);
			count++;
		}
		for (int i = 0; i < numProcesses; i++) {
			if (states[i] == HAS_DEPENDENCIES) {
				for (j = 0; j < processes[i].getNumberOfPreviousProcecces();
						j++) {
					if (states[processes[i].getPreviuosProcess(j)] != SCHEDULED) {
						break;
					}
				}
				if (j == processes[i].getNumberOfPreviousProcecces()) {
					states[i] = NO_DEPENDENCIES;
					queue.push(p[i]);
				}
			}
		}
	}

	maxEnd = -1;
	for (int i = 0; i < numProcessors; i++) {
		if (maxEnd < processors[i].getTime()) {
			maxEnd = processors[i].getTime();
		}
	}
	time = maxEnd;

	int spentTime = numProcessors * time;
	downtime = 1.0 * (spentTime - sumComplexity) / spentTime;

	sumOfComputationalComplexity = sumComplexity;

	states.clear();
	processes.clear();
	processors.clear();
	p.clear();
}

void Solution::mutate(SolutionPart part, int index) {
	int mutated;
	if (part == TASK) {
		mutated = mutateInt(tasks[index], MUTATION_POWER, numProcessors - 1);
		tasks[index] = mutated;
	} else {
		mutated = mutateInt(priorities[index], MUTATION_POWER, numProcesses - 1);
		priorities[index] = mutated;
	}
}
