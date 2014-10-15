#ifndef CHANGE_MEMORY_STRATEGIES_HPP_
#define CHANGE_MEMORY_STRATEGIES_HPP_

#include <stddef.h>

#include "MemoryVector.hpp"

class AbsoluteStrategy : public IChangeStrategy {
public:
	virtual ~AbsoluteStrategy() {}
	void changeElement(SolutionPart part, int row, int index,
			double before, double after,
			MemoryMatrix &mutMem, MemoryMatrix &crMem);
};
class RelativeStrategy : public IChangeStrategy {
public:
	virtual ~RelativeStrategy() {}
	void changeElement(SolutionPart part, int row, int index,
			double before, double after,
			MemoryMatrix &mutMem, MemoryMatrix &crMem);
};
class ForgettingStrategy : public IChangeStrategy {
public:
	virtual ~ForgettingStrategy() {}
	void changeElement(SolutionPart part, int row, int index,
			double before, double after,
			MemoryMatrix &mutMem, MemoryMatrix &crMem);
};

#endif /* CHANGE_MEMORY_STRATEGIES_HPP_ */
