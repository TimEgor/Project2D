#pragma once

#include <Windows.h>

class Input;

class UserInterfaces final {
private:
	HWND mainWindow;
	Input* input;

	UserInterfaces();

	bool initMainWindow();
	static LRESULT CALLBACK wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	static UserInterfaces& get();

	bool init();
	void release();

	Input* getInput() { return input; }

	HWND getMainWindow() { return mainWindow; }
};