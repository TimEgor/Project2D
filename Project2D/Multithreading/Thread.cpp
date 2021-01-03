#include "Thread.h"

#include <cassert>

bool Thread::init(ThreadFunction* function, void* args) {
	handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(function), args, NULL, (LPDWORD)(&threadID));
	return handle;
}

void Thread::release() {
	join();
	CloseHandle(handle);
}

void Thread::join() {
	WaitForSingleObject(handle, INFINITE);
}
