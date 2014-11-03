#ifndef SOLUTION_HPP_
#define SOLUTION_HPP_

#include <iostream>
#include <vector>

#include "GA.hpp"

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
	Solution &operator=(Solution that);
	~Solution();

	vector<int> &getIndicators();
	void setIndicators(vector<int> &_indicators);
	int getIndicator(int index) const;
	void setIndicator(int index, int value);
	double getField() const;
	void setField(double _field);
	double getSurvivalValue() const;
	void setSurvivalValue(double _survivalValue);

	void print(ostream &out);
	void generate();
	void buildField(vector<double> &weights);
	void mutate(int index);
};

#endif /* SOLUTION_HPP_ */