#pragma once

#include <MemoryManager/MemoryCore.h>
#include <GameLogic/CPP/CppGameLogicClass.h>

#include <unordered_map>
#include <string>

typedef CppGameLogicClass* (*CppGameLogicEntityComponentCreatingFunc)();

class CppGameLogicClassManager final {
	template <typename ClassType>
	friend class ClassRegister;

private:
	std::unordered_map<CppClassNameHash, CppGameLogicEntityComponentCreatingFunc> classes;

	CppGameLogicClassManager() = default;
	bool registryNewCreatingFunction(const char* className, CppGameLogicEntityComponentCreatingFunc function);

public:
	~CppGameLogicClassManager() { release(); }

	static CppGameLogicClassManager& get();

	bool init();
	void release();

	CppGameLogicClass* createComponent(const char* className, CppGameLogicEntityComponent* component);
	CppGameLogicClass* createComponent(CppClassNameHash classNameHash, CppGameLogicEntityComponent* component);
};

template <typename ClassType>
CppGameLogicClass* createCppLogicClass() {
	return new (memAllocate(sizeof(ClassType))) ClassType();
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

#define CPPGameLogicClassRegistry(ClassName, ClassType) const ClassRegister<ClassType>& classRegister_##ClassType = ClassRegister<ClassType>::registry(ClassName);
