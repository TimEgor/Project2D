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

AtomicCounterObjectBase* AtomicCounterObjectBase::getObject() {
	LockStateGuard guard(lockState);

	if (counter > 0 || isAlive) {
		incrementCounter();
		return this;
	}

	guard.unlock();

	return nullptr;
}

void AtomicCounterObjectBase::incrementCounter() {
	++counter;
}

void AtomicCounterObjectBase::decrementCounter() {
	if (--counter) {
		tryDestroy();
	}
}

AtomicCounterObjetcBaseReference::AtomicCounterObjetcBaseReference(AtomicCounterObjectBase* objectRef) {
	if (objectRef) {
		object = objectRef->getObject();
	}
}

AtomicCounterObjetcBaseReference::AtomicCounterObjetcBaseReference(const AtomicCounterObjetcBaseReference& reference) {
	if (reference.object) {
		object = reference.object->getObject();
	}
}

AtomicCounterObjetcBaseReference& AtomicCounterObjetcBaseReference::operator=(AtomicCounterObjectBase* objectRef) {
	if (object) {
		object->decrementCounter();
	}

	if (objectRef) {
		object = objectRef->getObject();
	}
}

AtomicCounterObjetcBaseReference& AtomicCounterObjetcBaseReference::operator=(const AtomicCounterObjetcBaseReference& reference) {
	if (object) {
		object->decrementCounter();
	}

	if (reference.object) {
		object = reference.object->getObject();
	}
}
