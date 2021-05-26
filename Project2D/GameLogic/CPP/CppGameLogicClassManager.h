#pragma once

#include <MemoryManager/Heap.h>

#include <unordered_map>
#include <string>

class Heap;
class CppGameLogicEntityComponent;

typedef CppGameLogicEntityComponent* (*CppGameLogicEntityComponentCreatingFunc)(Heap*);

class CppGameLogicClassManager final {
	template <typename ClassType>
	friend class ClassRegister;

	typedef uint32_t ClassNameHash;

private:
	std::unordered_map<ClassNameHash, CppGameLogicEntityComponentCreatingFunc> classes;

	CppGameLogicClassManager() = default;
	bool registryNewCreatingFunction(const char* className, CppGameLogicEntityComponentCreatingFunc function);

public:
	~CppGameLogicClassManager() { release(); }

	static CppGameLogicClassManager& get();

	bool init();
	void release();

	CppGameLogicEntityComponent* createComponent(const char* className, Heap* heap);
	CppGameLogicEntityComponent* createComponent(ClassNameHash classNameHash, Heap* heap);
};

template <typename ClassType>
CppGameLogicEntityComponent* createCppLogicClass(Heap* heap) {
	return (CppGameLogicEntityComponent*)(heap->allocate(sizeof(ClassType)));
}

template <typename ClassType>
class ClassRegister {
private:
	ClassRegister(const char* className);
	ClassRegister(const ClassRegister<ClassType>&) = delete;
	ClassRegister(ClassRegister<ClassType>&&) = delete;

public:
	static ClassRegister<ClassType>& registry(const char* className);
};

template<typename ClassType>
inline ClassRegister<ClassType>::ClassRegister(const char* className) {
	CppGameLogicClassManager& classManager = CppGameLogicClassManager::get();
	classManager.registryNewCreatingFunction(className, createCppLogicClass<ClassType>);
}

template<typename ClassType>
inline ClassRegister<ClassType>& ClassRegister<ClassType>::registry(const char* className) {
	static ClassRegister<ClassType> uniqueClassRegister(className);
	return uniqueClassRegister;
}

#define CPPGameLogicClassRegistry(ClassName, ClassType) const ClassRegister<ClassType>& classRegister = ClassRegister<ClassType>::registry(ClassName);
