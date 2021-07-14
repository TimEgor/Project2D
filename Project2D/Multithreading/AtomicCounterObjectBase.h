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

	void incrementCounter();
	void decrementCounter();

public:
	AtomicCounterObjectBase()
		: counter(0), isAlive(true) {}
	virtual ~AtomicCounterObjectBase() {}

	AtomicCounterObjectBase* getReferenceObject();
	void releaseReferenceObject();
};

template <typename T>
class AtomicCounterObjetcBaseReference {
protected:
	T* object = nullptr;

public:
	AtomicCounterObjetcBaseReference() = default;
	AtomicCounterObjetcBaseReference(T* object);
	AtomicCounterObjetcBaseReference(const AtomicCounterObjetcBaseReference<T>& reference);
	virtual ~AtomicCounterObjetcBaseReference() {}

	AtomicCounterObjetcBaseReference& operator=(T* object);
	AtomicCounterObjetcBaseReference& operator=(const AtomicCounterObjetcBaseReference<T>& reference);
	inline bool operator==(const AtomicCounterObjetcBaseReference<T>& reference) const;
	inline bool operator!=(const AtomicCounterObjetcBaseReference<T>& reference) const;

	bool isNull() const { return object == nullptr; }

	T& getObject() { return object; }
	const T& getObject() const { return object; }
};

template<typename T>
inline AtomicCounterObjetcBaseReference<T>::AtomicCounterObjetcBaseReference(T* objectRef) {
	if (objectRef) {
		object = (T*)(objectRef->getReferenceObject());
	}
}

template<typename T>
inline AtomicCounterObjetcBaseReference<T>::AtomicCounterObjetcBaseReference(const AtomicCounterObjetcBaseReference<T>& reference) {
	if (reference.object) {
		object = (T*)(reference.object->getReferenceObject());
	}
}

template<typename T>
inline AtomicCounterObjetcBaseReference<T>& AtomicCounterObjetcBaseReference<T>::operator=(T* objectRef) {
	if (object) {
		object->releaseReferenceObject();
	}

	if (objectRef) {
		object = (T*)(objectRef->getReferenceObject());
	}

	return *this;
}

template<typename T>
inline AtomicCounterObjetcBaseReference<T>& AtomicCounterObjetcBaseReference<T>::operator=(const AtomicCounterObjetcBaseReference<T>& reference) {
	if (object) {
		object->releaseReferenceObject();
	}

	if (reference.object) {
		object = (T*)(reference.object->getReferenceObject());
	}

	return *this;
}

template<typename T>
inline bool AtomicCounterObjetcBaseReference<T>::operator==(const AtomicCounterObjetcBaseReference<T>& reference) const {
	return object == reference.object;
}

template<typename T>
inline bool AtomicCounterObjetcBaseReference<T>::operator!=(const AtomicCounterObjetcBaseReference<T>& reference) const {
	return object != reference.object;;
}
