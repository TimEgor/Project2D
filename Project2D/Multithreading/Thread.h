#pragma once

#include <Windows.h>

#include <cstdint>

class Thread;
typedef void (*ThreadFunction)(Thread* thread, void* args);

class Thread final {
private:
	HANDLE handle;
	uint32_t threadID;

public:
	Thread() : handle(NULL) {}

	bool init(ThreadFunction *function, void *args);
	void release();

	void join();

	uint32_t getThreadID() { return threadID; }
};