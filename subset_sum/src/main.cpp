#include <cstdlib>
#include <cstring>
#include <time.h>
#include <cmath>
#include <limits>
#include <string>
#include <fstream>
#include <iostream>
#include <memory>

#include "GA.hpp"

using std::numeric_limits;
using std::unique_ptr;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::flush;

double goal;
int numWeights;
double selectionStrength;

string inputFile;
CrossoverType crType;
MemoryType memoryType;
int maxIterations = -1;

inline void printIterationNumber(int i) {
	if (i % 10 == 0) {
		cout << i << " ";
		if (i % 250 == 0 && i != 0) {
			cout << endl;
		}
		cout << flush;
	}
}

inline void printUsage() {
	cout
		<< "Usage: GA.exe input_file cr_type has_memory [memory_type] [max_iter]" << endl
		<< "	input_file: file with input data" << endl
		<< "	cr_type: crossover type (1 - 5)" << endl
		<< "	has_memory: memory usage (\"standard\" or \"memory\")" << endl
		<< "	memory_type: memory type used (\"absolute\", \"relative\" or \"forgetting\")" << endl
		<< "	max_iter: number of maximum iterations without improvement" << endl;
}

inline bool exists(char name[]) {
	ifstream f(name);
	if (f.good()) {
		f.close();
		return true;
	}
	f.close();
	return false;
}

bool parseArguments(int argc, char *argv[]) {
	if (argc != 4 && argc != 5 && argc != 6) {
		cerr << "Wrong number of arguments!" << endl;
		return false;
	}

	if (!exists(argv[1])) {
		cerr << "Specified file \"" << argv[1] << "\" does not exist!" << endl;
		return false;
	}
	inputFile = argv[1];

	int num = atoi(argv[2]);
	if (num < 1 || num > 5) {
		cerr << "Wrong argument 2: \"" << argv[2] << "\"" << endl;
		return false;
	}
	crType = CrossoverType(num - 1);

	char *typeStr = argv[3];
	if (strcmp("standard", typeStr) != 0 && strcmp("memory", typeStr) != 0) {
		cerr << "Wrong argument 3: \"" << typeStr << "\"" << endl;
		return false;
	}

	bool hasMemoryArgument = strcmp("standard", typeStr) != 0;
	if (hasMemoryArgument) {
		char *memoryStr = argv[4];
		if (strcmp("absolute", memoryStr) != 0
				&& strcmp("relative", memoryStr) != 0
				&& strcmp("forgetting", memoryStr) != 0) {
			cerr << "Wrong argument 4: \"" << memoryStr << "\"" << endl;
			return false;
		}
		if (!strcmp("absolute", memoryStr)) {
			memoryType = ABSOLUTE;
		} else if (!strcmp("relative", memoryStr)) {
			memoryType = RELATIVE;
		} else if (!strcmp("forgetting", memoryStr)) {
			memoryType = FORGETTING;
		} else {
			memoryType = NONE;
		}
	}

	if (!hasMemoryArgument && argc == 6) {
		cerr << "Wrong number of arguments!" << endl;
		return false;
	}

	maxIterations = atoi(argv[argc - 1]);
	if (maxIterations <= 0) {
		cerr << "Wrong last argument (" << argc << "): \"" <<
			argv[argc - 1]	<< "\"" << endl;
		return false;
	}
	return true;
}

inline void printResult(unique_ptr<IGeneticAlgorithm> &algorithm,
		double seconds, int iterations) {
	cout << "--------------------------------------------------" << endl;
	cout << "SubsetSumGA ";
	switch (memoryType) {
		case ABSOLUTE:
			cout << "with ABSOLUTE memory";
			break;
		case RELATIVE:
			cout << "with RELATIVE memory";
			break;
		case FORGETTING:
			cout << "with FORGETTING memory";
			break;
		default:
			cout << "STANDARD";
			break;
	}
	cout << " for input file \"" << inputFile << "\"." << endl;

	double result = algorithm->getResult();
	cout << "Result is " << fabs(goal - result) << endl;
	cout << "Sum is " << result	<< ", should be " << goal << "." << endl;

	cout << "It took " << seconds << " seconds to find a solution." << endl;
	cout << "Total number of iterations: " << iterations << "." << endl;
	cout << "--------------------------------------------------" << endl;
}

inline double currentResult(unique_ptr<IGeneticAlgorithm> &algorithm) {
	return goal - algorithm->getResult();
}

int main(int argc, char *argv[]) {
	if (parseArguments(argc, argv) == false) {
		printUsage();
		return EXIT_FAILURE;
	}
	srand(time(NULL));

	int i, iters_without_improvement = 0;
	double seconds, result, bestResult = numeric_limits<double>::max();
	clock_t start, end;
	ifstream fin(inputFile.c_str());
	unique_ptr<IGeneticAlgorithm> algorithm(
		new SubsetSumGA(fin, crType, memoryType)
	);
	fin.close();

	cout << "START" << endl;
	start = clock();
	algorithm->generatePopulation();
	for (i = 0; iters_without_improvement < maxIterations; i++) {
		algorithm->countSurvivalValues();
		algorithm->selection();
		result = currentResult(algorithm);
		if (result < bestResult) {
			cout << "i";
			bestResult = result;
			iters_without_improvement = 0;
		}
		algorithm->crossover();
		algorithm->mutation();
		printIterationNumber(i);
		iters_without_improvement++;
	}
	end = clock();
	cout << endl << "END" << endl;

	seconds = 1.0 * (end - start) / 1000000;
	printResult(algorithm, seconds, i);

	return EXIT_SUCCESS;
}