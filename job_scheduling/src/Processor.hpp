#ifndef PROCESSOR_HPP_
#define PROCESSOR_HPP_

#include <iostream>
#include <vector>

using std::vector;
using std::ostream;

class Processor {
private:
	vector<int> processes;
	int time;
	int downtime;
public:
	Processor();
	~Processor();

	int getProcess(int index) const;
	void setProcess(int index, int value);
	int getTime() const;
	void setTime(int _time);
	int getDowntime() const;
	void setDowntime(int _downtime);

	void print(ostream &out);
};

#endif /* PROCESSOR_HPP_ */
