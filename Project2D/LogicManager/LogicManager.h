#pragma once

class CPP_Logic;

class LogicManager final {
private:
	CPP_Logic* cppLogic;

	LogicManager() : cppLogic(nullptr) {}

public:
	static LogicManager& get();

	bool init();
	void release();
};