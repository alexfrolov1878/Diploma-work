#include <algorithm>
#include <vector>

#include "Processor.hpp"
#include "GA.hpp"
#include "main.hpp"

using std::vector;
using std::fill;
using std::ostream;
using std::endl;

Processor::Processor() {
	processes = vector<int>(numProcesses);
	fill(processes.begin(), processes.end(), '\0');
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
void Processor::setTime(int _time) {
	time = _time;
}
int Processor::getDowntime() const {
	return downtime;
}
void Processor::setDowntime(int _downtime) {
	downtime = _downtime;
}

void Processor::print(ostream &out) {
	for (int i = 0; i < numProcesses; i++) {
		if (processes[i] == 1) {
			out << i + 1 << " ";
		}
	}
	out << endl;
	out << "time = " << time << endl;
	out << "downtime = " << downtime << endl;
}
