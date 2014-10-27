#ifndef CHANGE_MEMORY_STRATEGIES_HPP_
#define CHANGE_MEMORY_STRATEGIES_HPP_

#include <stddef.h>

#include "MemoryMatrix.hpp"

class AbsoluteStrategy : public IChangeStrategy {
public:
	virtual ~AbsoluteStrategy() {}
	void changeElement(MatrixDouble &memMatr, int row, int index,
		double before, double after);
};
class RelativeStrategy : public IChangeStrategy {
public:
	virtual ~RelativeStrategy() {}
	void changeElement(MatrixDouble &memMatr, int row, int index,
		double before, double after);
};
class ForgettingStrategy : public IChangeStrategy {
public:
	virtual ~ForgettingStrategy() {}
	void changeElement(MatrixDouble &memMatr, int row, int index,
		double before, double after);
};

#endif /* CHANGE_MEMORY_STRATEGIES_HPP_ */
