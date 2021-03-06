#pragma once

#include <Windows.h>

enum class KeysMap : BYTE {
	MouseRight = VK_RBUTTON,
	MouseLeft = VK_LBUTTON,
	MouseMiddle = VK_MBUTTON,
	Backspace = VK_BACK,
	Delete = VK_DELETE,
	Tab = VK_TAB,
	Clear = VK_CLEAR,
	Return = VK_RETURN,
	Pause = VK_PAUSE,
	Esc = VK_ESCAPE,
	Space = VK_SPACE,
	KeyPad0 = VK_NUMPAD0,
	KeyPad1,
	KeyPad2,
	KeyPad3,
	KeyPad4,
	KeyPad5,
	KeyPad6,
	KeyPad7,
	KeyPad8,
	KeyPad9,
	Dot = VK_OEM_PERIOD,
	Comma = VK_OEM_COMMA,
	Divide = VK_DIVIDE,
	Multiply = VK_MULTIPLY,
	Minus = VK_SUBTRACT,
	Plus = VK_ADD,
	Enter = VK_RETURN,
	UpArrow = VK_UP,
	DownArrow = VK_DOWN,
	RightArrow = VK_RIGHT,
	LeftArrow = VK_LEFT,
	Insert = VK_INSERT,
	Home = VK_HOME,
	End = VK_END,
	PageUP = VK_PRIOR,
	PageDown = VK_NEXT,
	F1 = VK_F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	F13,
	F14,
	F15,
	Alpha0 = 0x30,
	Alpha1,
	Alpha2,
	Alpha3,
	Alpha4,
	Alpha5,
	Alpha6,
	Alpha7,
	Alpha8,
	Alpha9,
	A = 0x41,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	Colons = VK_OEM_1,
	RightBrackets = VK_OEM_4,
	LeftBrackets = VK_OEM_6,
	BackSlash = VK_OEM_5,
	Quotes = VK_OEM_7,
	Question = VK_OEM_2,
	Tilde = VK_OEM_3,
	NumLock = VK_NUMLOCK,
	CapsLock = VK_CAPITAL,
	ScrollLock = VK_SCROLL,
	RightShift = VK_RSHIFT,
	LeftShift = VK_LSHIFT,
	RightControl = VK_RCONTROL,
	LeftControl = VK_LCONTROL,
	RightAlt = VK_RMENU,
	LeftAlt = VK_LMENU,
	RightSystem = VK_RWIN,
	LeftSystem = VK_LWIN,
	Help = VK_HELP,
	Print = VK_PRINT,
	SnapShot = VK_SNAPSHOT
};

class Input final {
private:
	BYTE* keys;
	POINT mousePos;
	POINT oldMousePos;

public:
	Input() = default;
	~Input() { release(); }

	bool init();
	void release();

	void update();

	bool isKeyDown(KeysMap key) const;
	bool isKeyUp(KeysMap key) const;

	POINT getMousePosition() const;
	POINT getOldMousePosition() const;
	POINT getMouseMovment() const;
	bool isMouseMoved() const;
};