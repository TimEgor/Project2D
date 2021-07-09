#include "AtomicCounterObjectBase.h"

#include <cassert>

void AtomicCounterObjectBase::tryDestroy() {
	LockStateGuard guard(lockState);

	if (counter == 0) {
		isAlive = false;
		selfDestroy();
	}

	guard.unlock();
}

void AtomicCounterObjectBase::incrementCounter() {
	++counter;
}

void AtomicCounterObjectBase::decrementCounter() {
	if (--counter) {
		tryDestroy();
	}
}

AtomicCounterObjectBase* AtomicCounterObjectBase::getReferenceObject() {
	LockStateGuard guard(lockState);

	if (counter > 0 || isAlive) {
		incrementCounter();
		return this;
	}

	guard.unlock();

	return nullptr;
}

void AtomicCounterObjectBase::releaseReferenceObject() {
	decrementCounter();
}
