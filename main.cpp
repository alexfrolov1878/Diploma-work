#include <cstdlib>
#include <cstring>
#include <time.h>
#include <cmath>
#include <limits>
#include <string>
#include <fstream>
#include <iostream>

#include "GA.hpp"

using namespace std;

int resultTime;
int numProcesses;
int numProcessors;
int numConnections;

double goal;
int numWeights;

string inputFile;
AlgorithmType algType;
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
		<< "Usage: GA.exe input_file alg_type has_memory [memory_type] [max_iter]"
		<< endl << "	input_file: file with input data" << endl
		<< "	alg_type: algorithm type (\"microscopic\" or \"macroscopic\")"
		<< endl << "	has_memory: memory usage (\"standard\" or \"memory\")"
		<< endl
		<< "	memory_type: memory type used (\"absolute\", \"relative\" or \"forgetting\")"
		<< endl << "	max_iter: number of maximum iterations without improvement"
		<< endl;
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

	char *algStr = argv[2];
	if (strcmp("microscopic", algStr) != 0
			&& strcmp("macroscopic", algStr) != 0) {
		cerr << "Wrong argument 2: \"" << algStr << "\"" << endl;
		return false;
	}
	char *typeStr = argv[3];
	if (strcmp("standard", typeStr) != 0 && strcmp("memory", typeStr) != 0) {
		cerr << "Wrong argument 3: \"" << typeStr << "\"" << endl;
		return false;
	}
	if (!strcmp("microscopic", algStr) && !strcmp("standard", typeStr)) {
		algType = MICROSCOPIC_STANDARD;
		memoryType = NONE;
	} else if (!strcmp("microscopic", algStr) && !strcmp("memory", typeStr)) {
		algType = MICROSCOPIC_WITH_MEMORY;
	} else if (!strcmp("macroscopic", algStr) && !strcmp("standard", typeStr)) {
		algType = MACROSCOPIC_STANDARD;
		memoryType = NONE;
	} else {
		algType = MACROSCOPIC_WITH_MEMORY;
		memoryType = MACROSCOPIC;
	}

	bool hasMemoryArgument = algType == MICROSCOPIC_WITH_MEMORY;
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
		} else {
			memoryType = FORGETTING;
		}
	}

	if (!hasMemoryArgument && argc == 6) {
		cerr << "Wrong number of arguments!" << endl;
		return false;
	}

	int offset = hasMemoryArgument == true;
	int lastArg = 4 + offset;
	if (argc == lastArg + 1) {
		maxIterations = atoi(argv[lastArg]);
		if (maxIterations <= 0) {
			cerr << "Wrong argument " << lastArg << ": \"" << argv[lastArg]
					<< "\"" << endl;
			return false;
		}
	}
	return true;
}

inline void printResult(IGeneticAlgorithm *algorithm, double seconds, int iterations) {
	cout << "--------------------------------------------------" << endl;
	if (algType == MICROSCOPIC_STANDARD || algType == MICROSCOPIC_WITH_MEMORY) {
		cout << "MICROSCOPIC ";
	} else {
		cout << "MACROSCOPIC ";
	}
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
		case MACROSCOPIC:
			cout << "with MACROSCOPIC memory";
			break;
		default:
			cout << "standard";
			break;
	}
	cout << " for input file \"" << inputFile << "\"." << endl;

	if (algType == MICROSCOPIC_STANDARD || algType == MICROSCOPIC_WITH_MEMORY) {
		int time = (int) algorithm->getResult();
		cout << "Result is " << time << ", which is ";
		if (time == resultTime) {
			cout << "CORRECT." << endl;
		} else {
			cout << "INCORRECT (should be " << resultTime << ")." << endl;
		}
		if (time < resultTime) {
			cerr << "ERROR: incorrect result - better than optimal!" << endl;
		}
	} else {
		double result = algorithm->getResult();
		cout << "Result is " << fabs(goal - result) << ", sum is " << result
				<< ", should be " << goal << "." << endl;
	}

        cout << "It took " << seconds << " seconds to find a solution." << endl;
        cout << "Total number of iterations: " << iterations << "." << endl;
	cout << "--------------------------------------------------" << endl;
}

inline double currentResult(IGeneticAlgorithm *algorithm) {
	double result = algorithm->getResult();
	if (algType == MACROSCOPIC_STANDARD || algType == MACROSCOPIC_WITH_MEMORY) {
		result = goal - result;
	}
	return result;
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
	IGeneticAlgorithm *algorithm = GeneticAlgorithmFactory::newGeneticAlgorithm(
			algType, memoryType, fin);
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

	delete algorithm;
	return EXIT_SUCCESS;
}
