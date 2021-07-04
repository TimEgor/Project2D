#pragma once

#include <Multithreading/LockState.h>

class AtomicCounterObjectBase {
private:
	std::atomic_size_t counter;
	LockState lockState;
	bool isAlive;

	void tryDestroy();

protected:
	virtual void selfDestroy() = 0; 

public:
	AtomicCounterObjectBase()
		: counter(0), isAlive(true) {}
	virtual ~AtomicCounterObjectBase() {}

	AtomicCounterObjectBase* getObject();

	void incrementCounter();
	void decrementCounter(); //returning true when counter is null
};

class AtomicCounterObjetcBaseReference {
private:
	AtomicCounterObjectBase* object = nullptr;

public:
	AtomicCounterObjetcBaseReference() = default;
	AtomicCounterObjetcBaseReference(AtomicCounterObjectBase* object);
	AtomicCounterObjetcBaseReference(const AtomicCounterObjetcBaseReference& reference);
	virtual ~AtomicCounterObjetcBaseReference() {}

	AtomicCounterObjetcBaseReference& operator=(AtomicCounterObjectBase* object);
	AtomicCounterObjetcBaseReference& operator=(const AtomicCounterObjetcBaseReference& reference);

	bool isNull() const { return object == nullptr; }

	template <typename T>
	T& getObject();

	template <typename T>
	T& getObject() const;
};

template<typename T>
inline T& AtomicCounterObjetcBaseReference::getObject() {
	return *(T*)(object);
}

template<typename T>
inline T& AtomicCounterObjetcBaseReference::getObject() const {
	return *(T*)(object);
}
