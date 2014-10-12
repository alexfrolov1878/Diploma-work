#ifndef PROCESS_HPP_
#define PROCESS_HPP_

#include <iostream>
#include <vector>

using std::vector;
using std::ostream;

class Process {
private:
	int computationalComplexity;
	int numberOfPreviousProcecces;
	vector<int> previuosProcesses;
	int processor;
	int timeStart;
	int timeEnd;
public:
	Process();
	~Process();

	int getComputationalComplexity() const;
	void setComputationalComplexity(int _computationalComplexity);
	int getNumberOfPreviousProcecces() const;
	void setNumberOfPreviousProcecces(int _numberOfPreviousProcecces);
	const vector<int> &getPreviuosProcesses() const;
	int getPreviuosProcess(int index) const;
	void setPreviuosProcesses(vector<int> &_previuosProcesses);
	void setPreviuosProcess(int index, int value);
	int getProcessor() const;
	void setProcessor(int _processor);
	int getTimeEnd() const;
	void setTimeEnd(int _timeEnd);
	int getTimeStart() const;
	void setTimeStart(int _timeStart);

	void print(ostream &out);
};

#endif /* PROCESS_HPP_ */
