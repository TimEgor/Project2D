#include "CPP_Logic.h"

bool CPP_Logic::init() {
//#ifdef CPP_LOGIC
//    const char* cppLogicLibName = (const char*)(CPP_LOGIC);
//    logicLibrary = LoadLibraryA(cppLogicLibName);
//    GetProcAddress(logicLibrary, "getLogicAPI");
//#endif

    return true;
}

void CPP_Logic::release() {
    FreeLibrary(logicLibrary);
}
