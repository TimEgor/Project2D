#pragma once

#include <cstdint>

typedef uint32_t CppClassNameHash;

class CppGameLogicEntityComponent;
class CppGameLogicClassManager;

class CppGameLogicClass {
	friend CppGameLogicClassManager;

private:
	CppGameLogicEntityComponent *component;

	void setComponent(CppGameLogicEntityComponent* baseComponent) { component = baseComponent; }

public:
	CppGameLogicClass() = default;

	CppGameLogicEntityComponent* getComponent() { return component; }

	virtual void update(float deltaTime) = 0;
};