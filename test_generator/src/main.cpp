#include "main.hpp"

#include <cstring>
#include <ctime>
#include <vector>
#include <fstream>
#include <iostream>

using std::vector;
using std::ofstream;
using std::cerr;
using std::cout;
using std::endl;
using std::flush;

AlgorithmType algType;
char *outputFile;

int numProcesses;
int numProcessors;
int minComplexity;
int maxComplexity;
double connectionsRatio;

int numWeights;
double goalRatio;
double selectionStrength;

inline void printUsage() {
	cout << "Usage: GA_test_generator.exe alg_type "
			<< "[num_proc proc_ratio min_comp max_comp [conn_ratio] | "
			<< "num_weights goal_ratio sel_strength] output_file" << endl
			<< "  alg_type: type of genetic algorithm (\"microscopic\" or \"macroscopic\")"
			<< endl << "  For microscopic only:" << endl
			<< "	num_proc: number of processes" << endl
			<< "	proc_ratio: ratio between processes and processors" << endl
			<< "	min_comp: minimum process complexity" << endl
			<< "	min_comp: maximum process complexity" << endl
			<< "	conn_ratio: ratio between processes and connections" << endl
			<< "  For macroscopic only:"  << endl << "	num_weights: number of weights"
			<< endl << "	goal_ratio: ratio between desired goal and num_weights"
			<< endl	<< "	sel_strength: selection strength" << endl
			<< "  output_file: output file with test info" << endl;
}

bool parseArguments(int argc, char *argv[]) {
	if (argc != 6 && argc != 7 && argc != 8) {
		cerr << "Wrong number of arguments!" << endl;
		return false;
	}

	char *algStr = argv[1];
	if (strcmp("microscopic", algStr) == 0) {
		algType = MICROSCOPIC;
	} else if (strcmp("macroscopic", algStr) == 0) {
		algType = MACROSCOPIC;
	} else {
		cerr << "Wrong argument 2: \"" << algStr << "\"" << endl;
		return false;
	}

	if (algType == MICROSCOPIC) {
		if (argc != 7 && argc != 8) {
			cerr << "Wrong number of arguments!" << endl;
			return false;
		}
		numProcesses = atoi(argv[2]);
		if (numProcesses <= 0) {
			cerr << "Wrong argument 2: \"" << argv[2] << "\"" << endl;
			return false;
		}
		double processorsRatio = atof(argv[3]);
		if (processorsRatio <= 0 || processorsRatio >= 1) {
			cerr << "Wrong argument 3: \"" << argv[3] << "\"" << endl;
			return false;
		}
		numProcessors = processorsRatio * numProcesses;
		minComplexity = atoi(argv[4]);
		if (minComplexity <= 0) {
			cerr << "Wrong argument 4: \"" << argv[4] << "\"" << endl;
			return false;
		}
		maxComplexity = atoi(argv[5]);
		if (maxComplexity <= 0) {
			cerr << "Wrong argument 5: \"" << argv[5] << "\"" << endl;
			return false;
		}
		if (argc == 8) {
			connectionsRatio = atof(argv[6]);
			if (connectionsRatio <= 0 || connectionsRatio >= 1) {
				cerr << "Wrong argument 6: \"" << argv[6] << "\"" << endl;
				return false;
			}
		} else {
			connectionsRatio = Random::getRandomDouble(CON_RATIO_MIN,
					CON_RATIO_MAX);
		}
		outputFile = argv[7];
	} else {
		if (argc != 6) {
			cerr << "Wrong number of arguments!" << endl;
			return false;
		}
		numWeights = atoi(argv[2]);
		if (numWeights <= 0) {
			cerr << "Wrong argument 2: \"" << argv[2] << "\"" << endl;
			return false;
		}
		goalRatio = atof(argv[3]);
		if (goalRatio < 0 || goalRatio > 1) {
			cerr << "Wrong argument 3: \"" << argv[3] << "\"" << endl;
			return false;
		}
		selectionStrength = atof(argv[4]);
		if (selectionStrength <= 0 || selectionStrength >= 1) {
			cerr << "Wrong argument 4: \"" << argv[4] << "\"" << endl;
			return false;
		}
		outputFile = argv[5];
	}
	return true;
}

void printMicriscopicTest(ofstream &fout) {
	int pr1, pr2, p1, p2;
	int i, j, k, t, actualNumProccesses, resultTime, numConnections;

	vector<Process> processes(numProcesses);
	vector<Processor> processors(numProcessors);
	resultTime = 1.0 * numProcesses / numProcessors * (maxComplexity - minComplexity) / 1.5;

	for (i = j = k = 0; i < numProcessors; i++) {
		t = 0;
		while (resultTime != t) {
			if (resultTime - t > maxComplexity) {
				processes[j].comp_complexity = Random::getRandomInt(
						minComplexity, maxComplexity);
			} else {
				processes[j].comp_complexity = resultTime - t;
			}
			processes[j].num_prev_proc = 0;
			processes[j].prev_processes = vector<int>();
			processes[j].processor = i;
			processes[j].time_start = t;
			processes[j].time_end = t + processes[j].comp_complexity;
			t = processes[j].time_end;
			processors[i].processes.push_back(j);
			processors[i].num_proc++;
			j++;
			if (j == numProcesses) {
				processes.push_back(Process());
			}
		}
	}
	actualNumProccesses = j;

	numConnections = Random::getRandomInt(actualNumProccesses * CON_RATIO_MIN,
			actualNumProccesses * CON_RATIO_MAX + 1);
	numConnections = actualNumProccesses * connectionsRatio;
	for (i = 0; i < numConnections; i++) {
		while (true) {
			pr1 = Random::getRandomInt(0, numProcessors);
			p1 = Random::getRandomInt(0, processors[pr1].num_proc - 1);
			p1 = processors[pr1].processes[p1];
			do {
				pr2 = Random::getRandomInt(0, numProcessors);
			} while (pr1 == pr2);
			p2 = Random::getRandomInt(1, processors[pr2].num_proc);
			p2 = processors[pr2].processes[p2];

			if (processes[p1].time_end <= processes[p2].time_start) {
				processes[p2].prev_processes.push_back(p1);
				processes[p2].num_prev_proc++;
				break;
			}
		}
	}

	fout << resultTime << " " << numProcessors << " " << actualNumProccesses
			<< " " << numConnections << endl;
	for (i = 0; i < actualNumProccesses; i++) {
		fout << processes[i].comp_complexity << " ";
		for (j = 0; j < processes[i].num_prev_proc; j++) {
			fout << processes[i].prev_processes[j] << " ";
		}
		fout << "-1" << endl;
	}
	fout << flush;

	for (i = 0; i < numProcesses; i++) {
		processes[i].prev_processes.clear();
	}
	for (i = 0; i < numProcessors; i++) {
		processors[i].processes.clear();
	}
	processes.clear();
	processors.clear();
}

void printMacriscopicTest(ofstream &fout) {
	vector<double> weights(numWeights);
	double sum = 0.0;
	for (int i = 0; i < numWeights; i++) {
		weights[i] = Random::getRandomDouble(0, 1);
		sum += weights[i];
	}
	double goal = goalRatio * sum;
	fout << numWeights << " " << goal << " " << selectionStrength << endl;
	for (int i = 0; i < numWeights; i++) {
		fout << weights[i] << endl;
	}
	fout << flush;
}

int main(int argc, char *argv[]) {
	if (parseArguments(argc, argv) == false) {
		printUsage();
		return EXIT_FAILURE;
	}
	srand(time(NULL));
	ofstream fout(outputFile);

	if (algType == MICROSCOPIC) {
		printMicriscopicTest(fout);
	} else {
		printMacriscopicTest(fout);
	}

	fout.flush();
	fout.close();
	return EXIT_SUCCESS;
}
