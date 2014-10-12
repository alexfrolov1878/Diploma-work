#include <stddef.h>

#include "Process.hpp"

using std::vector;
using std::ostream;
using std::endl;

Process::Process() {
	computationalComplexity = 0;
	numberOfPreviousProcecces = 0;
	previuosProcesses = vector<int>();
	processor = 0;
	timeStart = 0;
	timeEnd = 0;
}
Process::~Process() {
	previuosProcesses.clear();
}

int Process::getComputationalComplexity() const {
	return computationalComplexity;
}
void Process::setComputationalComplexity(int _computationalComplexity) {
	computationalComplexity = _computationalComplexity;
}
int Process::getNumberOfPreviousProcecces() const {
	return numberOfPreviousProcecces;
}
void Process::setNumberOfPreviousProcecces(int _numberOfPreviousProcecces) {
	numberOfPreviousProcecces = _numberOfPreviousProcecces;
	if (numberOfPreviousProcecces != 0) {
		previuosProcesses.resize(numberOfPreviousProcecces);
	}
}
const vector<int> &Process::getPreviuosProcesses() const {
	return previuosProcesses;
}
int Process::getPreviuosProcess(int index) const {
	return previuosProcesses[index];
}
void Process::setPreviuosProcesses(vector<int> &_previuosProcesses) {
	previuosProcesses.swap(_previuosProcesses);
}
void Process::setPreviuosProcess(int index, int value) {
	previuosProcesses[index] = value;
}
int Process::getProcessor() const {
	return processor;
}
void Process::setProcessor(int _processor) {
	processor = _processor;
}
int Process::getTimeEnd() const {
	return timeEnd;
}
void Process::setTimeEnd(int _timeEnd) {
	timeEnd = _timeEnd;
}
int Process::getTimeStart() const {
	return timeStart;
}
void Process::setTimeStart(int _timeStart) {
	timeStart = _timeStart;
}

void Process::print(ostream &out) {
	out << "computationalComplexity = " << computationalComplexity << endl;
	out << "numberOfPreviousProcecces = " << numberOfPreviousProcecces
			<< endl;
	for (int i = 0; i < numberOfPreviousProcecces; i++) {
		out << previuosProcesses[i] << " ";
	}
	out << endl;
	out << "processor = " << processor << endl;
	out << "timeStart = " << timeStart << endl;
	out << "timeEnd = " << timeEnd << endl;
}
