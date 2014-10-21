#ifndef SOLUTION_HPP_
#define SOLUTION_HPP_

#include <iostream>
#include <vector>

#include "GA.hpp"
#include "MemoryVector.hpp"

using std::vector;
using std::ostream;

class Solution {
private:
	vector<int> indicators;
	double field;
	double survivalValue;
public:
	Solution();
	Solution(const Solution &that);
	// TODO: ...
	~Solution();

	int getIndicator(int index) const;
	double getField() const;
	void setField(double _field);
	double getSurvivalValue() const;
	void setSurvivalValue(double _survivalValue);

	void print(ostream &out);
	void generate();
	void buildField(vector<double> &weights);
	void crossover(Solution &that);
	void mutate();
};

#endif /* SOLUTION_HPP_ */