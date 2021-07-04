#include "LockState.h"

#include <thread>

bool LockState::tryLock() {
	bool expectedState = false;
	if (state.compare_exchange_strong(expectedState, true)) {
		return true;
	}

	return false;
}

void LockState::lock() {
	size_t tryingCount = 0;

	while (!tryLock()) {
		++tryingCount;

		if (tryingCount >= 256) {
			std::this_thread::yield();
		}
	}
}

void LockState::unlock() {
	state = false;
}
