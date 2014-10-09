#include "../GA.hpp"
#include "Processor.hpp"

#include <algorithm>
#include <vector>

#include "main.hpp"

Processor::Processor() {
	processes = std::vector<int>(numProcesses);
	std::fill(processes.begin(), processes.end(), '\0');
	time = 0;
	downtime = 0;
}
Processor::~Processor() {
	processes.clear();
}

int Processor::getProcess(int index) const {
	return processes[index];
}
void Processor::setProcess(int index, int value) {
	processes[index] = value;
}
int Processor::getTime() const {
	return time;
}
void Processor::setTime(int time) {
	this->time = time;
}
int Processor::getDowntime() const {
	return downtime;
}
void Processor::setDowntime(int downtime) {
	this->downtime = downtime;
}

void Processor::print(std::ostream &out) {
	for (int i = 0; i < numProcesses; i++) {
		if (processes[i] == 1) {
			out << i + 1 << " ";
		}
	}
	out << std::endl;
	out << "time = " << time << std::endl;
	out << "downtime = " << downtime << std::endl;
}
