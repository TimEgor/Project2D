#pragma once

#include <Windows.h>

class CPP_Logic final {
private:
	HMODULE logicLibrary;

public:
	CPP_Logic() = default;

	bool init();
	void release();
};