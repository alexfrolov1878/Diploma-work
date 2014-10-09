#ifndef PROCESS_HPP_
#define PROCESS_HPP_

#include <iostream>
#include <vector>

class Process {
private:
	int computationalComplexity;
	int numberOfPreviousProcecces;
	std::vector<int> previuosProcesses;
	int processor;
	int timeStart;
	int timeEnd;
public:
	Process();
	~Process();

	int getComputationalComplexity() const;
	void setComputationalComplexity(int computationalComplexity);
	int getNumberOfPreviousProcecces() const;
	void setNumberOfPreviousProcecces(int numberOfPreviousProcecces);
	const std::vector<int> &getPreviuosProcesses() const;
	int getPreviuosProcess(int index) const;
	void setPreviuosProcesses(std::vector<int> &previuosProcesses);
	void setPreviuosProcess(int index, int value);
	int getProcessor() const;
	void setProcessor(int processor);
	int getTimeEnd() const;
	void setTimeEnd(int timeEnd);
	int getTimeStart() const;
	void setTimeStart(int timeStart);

	void print(std::ostream &out);
};

#endif /* PROCESS_HPP_ */
