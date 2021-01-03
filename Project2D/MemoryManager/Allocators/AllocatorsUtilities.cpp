#include "AllocatorsUtilities.h"

size_t getAdjustment(const size_t& size, const size_t& alignment) {
	return alignment - (size & (alignment - 1));
}

void align(size_t& size, const size_t& alignment) {
	size_t adjustment = getAdjustment(size, alignment);
	if (adjustment != alignment)
		size += adjustment;
}
