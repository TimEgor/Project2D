#pragma once

#include <Windows.h>

class UserInterfaces final {
private:
	HWND mainWindow;

	UserInterfaces() : mainWindow(0) {}

	static LRESULT CALLBACK wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	static UserInterfaces& get();

	bool init();
	void release();

	HWND getMainWindow() { return mainWindow; }
};