#ifndef PROCESSOR_HPP_
#define PROCESSOR_HPP_

#include <iostream>

class Processor {
private:
	int *processes;
	int time;
	int downtime;
public:
	Processor();
	~Processor();

	int getProcess(int index) const;
	void setProcess(int index, int value);
	int getTime() const;
	void setTime(int time);
	int getDowntime() const;
	void setDowntime(int downtime);

	void print(std::ostream &out);
};

#endif /* PROCESSOR_HPP_ */
