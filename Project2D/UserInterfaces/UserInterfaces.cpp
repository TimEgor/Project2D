#include "UserInterfaces.h"

#include <UserInterfaces/Input.h>

UserInterfaces::UserInterfaces() :
    mainWindow(0), input(nullptr) {}

UserInterfaces& UserInterfaces::get() {
    static UserInterfaces uniqueUserInterfaces;
    return uniqueUserInterfaces;
}

bool UserInterfaces::init() {
    if (!initMainWindow()) {
        release();
        return false;
    }

    input = new Input();
    if (!input->init()) {
        release();
        return false;
    }

    return true;
}

void UserInterfaces::release() {
    if (mainWindow) {
        DestroyWindow(mainWindow);
        mainWindow = NULL;
    }

    if (input) {
        input->release();
        delete input;
        input = nullptr;
    }
}

bool UserInterfaces::initMainWindow() {
    HINSTANCE instance = GetModuleHandle(0);

    WNDCLASS wndClass;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hbrBackground = (HBRUSH)(GetStockObject(BLACK_BRUSH));
    wndClass.hCursor = LoadCursor(0, IDC_ARROW);
    wndClass.hIcon = LoadIcon(0, IDI_APPLICATION);
    wndClass.hInstance = instance;
    wndClass.lpfnWndProc = UserInterfaces::wndProc;
    wndClass.lpszClassName = "Project2D";
    wndClass.lpszMenuName = 0;
    wndClass.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&wndClass);

    mainWindow = CreateWindow("Project2D", "Project2D", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 800, NULL, NULL, instance, NULL);
    if (mainWindow == NULL) {
        return false;
    }

    ShowWindow(mainWindow, SW_NORMAL);
    UpdateWindow(mainWindow);

    return true;
}

LRESULT UserInterfaces::wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}