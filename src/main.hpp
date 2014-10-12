#ifndef MAIN_HPP_
#define MAIN_HPP_

#include <cstdlib>

class Random {
public:
	static int getRandomInt(int a, int b) {
		return int(1.0 * rand() / (RAND_MAX + 1.0) * (b - a) + a);
	}
	static double getRandomDouble(double a, double b) {
		return 1.0 * rand() / (RAND_MAX + 1.0) * (b - a) + a;
	}
};

#endif /* MAIN_HPP_ */

