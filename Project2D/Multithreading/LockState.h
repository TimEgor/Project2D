#pragma once

#include <atomic>

class LockState final {
private:
	std::atomic_bool state;

	bool tryLock();

public:
	LockState(bool isInitialLock = false)
		: state(isInitialLock) {}

	void lock();
	void unlock();
};

class LockStateGuard final {
private:
	LockState& state;

public:
	LockStateGuard(LockState& state)
		: state(state) {
		lock();
	}

	~LockStateGuard() { unlock(); }

	void lock() { state.lock(); }
	void unlock() { state.unlock(); }
};