#ifndef MAIN_HPP_
#define MAIN_HPP_

#include <cstdlib>
#include <vector>

using std::vector;

const double CON_RATIO_MIN = 0.1;
const double CON_RATIO_MAX = 0.5;

enum AlgorithmType {
	MICROSCOPIC, MACROSCOPIC
};

class Random {
public:
	static int getRandomInt(int a, int b) {
		return 1.0 * rand() / (RAND_MAX + 1.0) * (b - a) + a;
	}
	static double getRandomDouble(double a, double b) {
		return 1.0 * rand() / (RAND_MAX + 1.0) * (b - a) + a;
	}
};

typedef struct {
	int num_proc;
	vector<int> processes;
} Processor;

typedef struct {
	int comp_complexity;
	int num_prev_proc;
	vector<int> prev_processes;
	int processor;
	int time_start;
	int time_end;
} Process;

extern int numProcesses;

#endif /* MAIN_HPP_ */
