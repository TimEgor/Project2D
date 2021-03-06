#include "Input.h"

bool Input::init() {
    keys = new BYTE[255];

    return true;
}

void Input::release() {
    if (keys) {
        delete[] keys;
        keys = nullptr;
    }
}

void Input::update() {
    GetKeyboardState(keys);

    oldMousePos = mousePos;
    GetCursorPos(&mousePos);
}

bool Input::isKeyDown(KeysMap key) const {
    return (keys[(BYTE)(key)] & 0x80);
}

bool Input::isKeyUp(KeysMap key) const {
    return !(keys[(BYTE)(key)] & 0x80);
}

POINT Input::getMousePosition() const {
    return mousePos;
}

POINT Input::getOldMousePosition() const {
    return oldMousePos;
}

POINT Input::getMouseMovment() const {
    return POINT{ mousePos.x - oldMousePos.x, mousePos.y - oldMousePos.y };
}

bool Input::isMouseMoved() const {
    return ((mousePos.x != oldMousePos.x) || (mousePos.y != oldMousePos.y));
}
