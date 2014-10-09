#include "Process.hpp"

#include <stddef.h>

Process::Process() {
	computationalComplexity = 0;
	numberOfPreviousProcecces = 0;
	previuosProcesses = std::vector<int>();
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
void Process::setComputationalComplexity(int computationalComplexity) {
	this->computationalComplexity = computationalComplexity;
}
int Process::getNumberOfPreviousProcecces() const {
	return numberOfPreviousProcecces;
}
void Process::setNumberOfPreviousProcecces(int numberOfPreviousProcecces) {
	this->numberOfPreviousProcecces = numberOfPreviousProcecces;
	if (numberOfPreviousProcecces != 0) {
		previuosProcesses.resize(numberOfPreviousProcecces);
	}
}
const std::vector<int> &Process::getPreviuosProcesses() const {
	return previuosProcesses;
}
int Process::getPreviuosProcess(int index) const {
	return previuosProcesses[index];
}
void Process::setPreviuosProcesses(std::vector<int> &previuosProcesses) {
	this->previuosProcesses.swap(previuosProcesses);
}
void Process::setPreviuosProcess(int index, int value) {
	previuosProcesses[index] = value;
}
int Process::getProcessor() const {
	return processor;
}
void Process::setProcessor(int processor) {
	this->processor = processor;
}
int Process::getTimeEnd() const {
	return timeEnd;
}
void Process::setTimeEnd(int timeEnd) {
	this->timeEnd = timeEnd;
}
int Process::getTimeStart() const {
	return timeStart;
}
void Process::setTimeStart(int timeStart) {
	this->timeStart = timeStart;
}

void Process::print(std::ostream &out) {
	out << "computationalComplexity = " << computationalComplexity << std::endl;
	out << "numberOfPreviousProcecces = " << numberOfPreviousProcecces
			<< std::endl;
	for (int i = 0; i < numberOfPreviousProcecces; i++) {
		out << previuosProcesses[i] << " ";
	}
	out << std::endl;
	out << "processor = " << processor << std::endl;
	out << "timeStart = " << timeStart << std::endl;
	out << "timeEnd = " << timeEnd << std::endl;
}
