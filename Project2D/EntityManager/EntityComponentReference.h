#pragma once

#include <EntityManager/EntityComponentHandler.h>

#include <atomic>

class EntityComponentHandler;
class Level;

class EntityComponentReferenceHandler final {
private:
	EntityComponentHandler* componentHandler;
	Level* level;
	std::atomic_size_t refCounter;
	size_t allocatorIndex;

public:
	EntityComponentReferenceHandler(EntityComponentHandler* handler, Level* level, size_t allocatorIndex)
		: componentHandler(handler), level(level), allocatorIndex(allocatorIndex) {}

	EntityComponentHandler* getComponentHandler() { return componentHandler; }
	void setComponentHandler(EntityComponentHandler* newComponentHandler) { componentHandler = newComponentHandler; }

	Level* getLevel() { return level; }

	size_t getRefCounter() { return refCounter; }
	void incrementRefCounter() { ++refCounter; }
	void decrementRefCounter() { --refCounter; }

	size_t getAllocatorIndex() { return allocatorIndex; }
};

class EntityComponentReference final {
private:
	EntityComponentReferenceHandler* handler;

	void releaseReference();

public:
	EntityComponentReference(EntityComponentReferenceHandler* handler);
	EntityComponentReference(const EntityComponentReference& reference);

	~EntityComponentReference();

	EntityComponentReference& operator=(const EntityComponentReference& reference);

	bool isNull() const { return handler == nullptr; }

	template <typename T>
	T& getComponent();
};

template<typename T>
inline T& EntityComponentReference::getComponent() {
	return *(T*)(handler->getComponentHandler()->component);
}
