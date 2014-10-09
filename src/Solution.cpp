#include "Solution.hpp"

#include <vector>

#include "main.hpp"
#include "Process.hpp"
#include "Processor.hpp"

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

/*=============================MICROSCOPIC===================================*/
MicroscopicSolution::MicroscopicSolution() {
	tasks = std::vector<int>(numProcesses, 0);
	priorities = std::vector<int>(numProcesses, 0);
	time = 0;
	downtime = 0.0;
	survivalValue = 0.0;
	sumOfComputationalComplexity = 0;
}
MicroscopicSolution::MicroscopicSolution(const MicroscopicSolution &that) {
	tasks = that.tasks;
	priorities = that.priorities;
	time = that.time;
	downtime = that.downtime;
	survivalValue = that.survivalValue;
	sumOfComputationalComplexity = that.sumOfComputationalComplexity;
}
MicroscopicSolution& MicroscopicSolution::operator=(MicroscopicSolution that) {
    tasks.swap(that.tasks);
	priorities.swap(that.priorities);
    time = that.time;
    downtime = that.downtime;
    survivalValue = that.survivalValue;
    sumOfComputationalComplexity = that.sumOfComputationalComplexity;
    return *this;
}
MicroscopicSolution::~MicroscopicSolution() {
	tasks.clear();
	priorities.clear();
}

const std::vector<int> &MicroscopicSolution::getTasks() const {
	return tasks;
}
int MicroscopicSolution::getTask(int index) const {
	return tasks[index];
}
void MicroscopicSolution::setTask(int index, int value) {
	tasks[index] = value;
}
const std::vector<int> &MicroscopicSolution::getPriorities() const {
	return priorities;
}
int MicroscopicSolution::getPriority(int index) const {
	return priorities[index];
}
void MicroscopicSolution::setPriority(int index, int value) {
	priorities[index] = value;
}
double MicroscopicSolution::getDowntime() const {
	return downtime;
}
void MicroscopicSolution::setDowntime(double downtime) {
	this->downtime = downtime;
}
int MicroscopicSolution::getTime() const {
	return time;
}
void MicroscopicSolution::setTime(int time) {
	this->time = time;
}
double MicroscopicSolution::getSurvivalValue() const {
	return survivalValue;
}
void MicroscopicSolution::setSurvivalValue(double survivalValue) {
	this->survivalValue = survivalValue;
}
int MicroscopicSolution::getSumOfComputationalComplexity() const {
	return sumOfComputationalComplexity;
}
void MicroscopicSolution::setSumOfComputationalComplexity(
		int sumOfComputationalComplexity) {
	this->sumOfComputationalComplexity = sumOfComputationalComplexity;
}

void MicroscopicSolution::print(std::ostream &out) {
	out << "Task: ";
	for (int i = 0; i < numProcesses; i++) {
		out << tasks[i] << " ";
	}
	out << std::endl;
	out << "Prio: ";
	for (int i = 0; i < numProcesses; i++) {
		out << priorities[i] << " ";
	}
	out << std::endl;
	out << "time = " << time << std::endl;
	out << "downtime = " << downtime << std::endl;
	out << "survival_value = " << survivalValue << std::endl;
	out << "sum_of_computational_complexity = " << sumOfComputationalComplexity
			<< std::endl;
}
void MicroscopicSolution::generate() {
	int randomValue;
	for (int i = 0; i < numProcesses; i++) {
		randomValue = Random::getRandomInt(0, numProcessors);
		tasks[i] = randomValue;
		randomValue = Random::getRandomInt(0, numProcesses);
		priorities[i] = randomValue;
	}
}
void MicroscopicSolution::buildSchedule(Process *initProcesses) {
	enum State {
		SCHEDULED, HAS_DEPENDENCIES, NO_DEPENDENCIES
	};
	int j, maxEnd, mini, tmp1, tmp2, count, sum;
	int tempEnd, prevProc;
	State states[numProcesses];
	for (int i = 0; i < numProcesses; i++) {
		states[i] = NO_DEPENDENCIES;
	}

	Process *processes = new Process[numProcesses];
	Processor *processors = new Processor[numProcessors];
	std::vector<Priority> p;

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
		for (int j = 0; j < tmp1; j++) {
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

	delete[] processes;
	delete[] processors;
	p.clear();
}
void MicroscopicSolution::mutate(SolutionPart part, int index) {
	int mutated;
	if (part == MUTATION_TASK) {
		mutated = mutateInt(tasks[index], MUTATION_TASK_POWER,
				numProcessors - 1);
		tasks[index] = mutated;
	} else if (part == MUTATION_PRIO) {
		mutated = mutateInt(priorities[index], MUTATION_PRIO_POWER,
				numProcesses - 1);
		priorities[index] = mutated;
	}
}
/*===========================================================================*/
/*=============================MACROSCOPIC===================================*/
MacroscopicSolution::MacroscopicSolution() {
	indicators = std::vector<int>(numWeights);
	field = 0.0;
	survivalValue = 0.0;
}
MacroscopicSolution::MacroscopicSolution(const MacroscopicSolution& that) {
	indicators = that.indicators;
	field = that.field;
	survivalValue = that.survivalValue;
}
MacroscopicSolution::~MacroscopicSolution() {
	indicators.clear();
}

int MacroscopicSolution::getIndicator(int index) const {
	return indicators[index];
}
double MacroscopicSolution::getField() const {
	return field;
}
void MacroscopicSolution::setField(double field) {
	this->field = field;
}
double MacroscopicSolution::getSurvivalValue() const {
	return survivalValue;
}
void MacroscopicSolution::setSurvivalValue(double survivalValue) {
	this->survivalValue = survivalValue;
}

void MacroscopicSolution::print(std::ostream &out) {
	out << "Indicators: ";
	for (int i = 0; i < numWeights; i++) {
		out << indicators[i] << " ";
	}
	out << std::endl;
}
void MacroscopicSolution::generate() {
	double random_value;
	for (int i = 0; i < numWeights; i++) {
		random_value = Random::getRandomDouble(0, 1);
		indicators[i] = random_value < 0.5;
	}
}
void MacroscopicSolution::buildField(std::vector<double> &weights) {
	double sum = 0.0;
	for (int i = 0; i < numWeights; i++) {
		sum += weights[i] * indicators[i];
	}
	field = sum;
}
void MacroscopicSolution::crossover(MacroscopicSolution &that) {
	double r;
	bool value1, value2;
	for (int i = 0; i < numWeights; i++) {
		r = Random::getRandomDouble(0, 1);
		if (r < MACROSCOPIC_CROSSOVER_PROBABILITY) {
			value1 = this->indicators[i];
			value2 = that.indicators[i];
			this->indicators[i] = value2;
			that.indicators[i] = value1;
		}
	}
}
void MacroscopicSolution::mutate() {
	double r;
	for (int i = 0; i < numWeights; i++) {
		r = Random::getRandomDouble(0, 1);
		if (r < MACROSCOPIC_MUTATION_PROBABILITY) {
			indicators[i] = !indicators[i];
		}
	}
}
/*===========================================================================*/
