#include <vector>

#include "Solution.hpp"
#include "main.hpp"

using std::vector;
using std::ostream;
using std::endl;

Solution::Solution() {
	indicators = vector<int>(numWeights);
	field = 0.0;
	survivalValue = 0.0;
}

Solution::Solution(const Solution& that) {
	indicators = that.indicators;
	field = that.field;
	survivalValue = that.survivalValue;
}

Solution& Solution::operator=(Solution that) {
	indicators.swap(that.indicators);
	field = that.field;
	survivalValue = that.survivalValue;
	return *this;
}

Solution::~Solution() {
	indicators.clear();
}

vector<int> &Solution::getIndicators() {
	return indicators;
}

void Solution::setIndicators(vector<int> &_indicators) {
	indicators.swap(_indicators);
}

int Solution::getIndicator(int index) const {
	return indicators[index];
}

void Solution::setIndicator(int index, int value) {
	indicators[index] = value;
}

double Solution::getField() const {
	return field;
}

void Solution::setField(double _field) {
	field = _field;
}

double Solution::getSurvivalValue() const {
	return survivalValue;
}

void Solution::setSurvivalValue(double _survivalValue) {
	survivalValue = _survivalValue;
}

void Solution::print(ostream &out) {
	out << "Indicators: ";
	for (int i = 0; i < numWeights; i++) {
		out << indicators[i] << " ";
	}
	out << endl;
}

void Solution::generate() {
	double random_value;
	for (int i = 0; i < numWeights; i++) {
		random_value = Random::getRandomDouble(0, 1);
		indicators[i] = random_value < 0.5;
	}
}

void Solution::buildField(vector<double> &weights) {
	double sum = 0.0;
	for (int i = 0; i < numWeights; i++) {
		sum += weights[i] * indicators[i];
	}
	field = sum;
}

void Solution::mutate(int index) {
	double r = Random::getRandomDouble(0, 1);
	if (r < MUTATION_PROBABILITY) {
		indicators[index] = !indicators[index];
	}
}
