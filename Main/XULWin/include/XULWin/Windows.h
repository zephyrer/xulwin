#ifndef WINDOWS_H_INCLUDED
#define WINDOWS_H_INCLUDED


// Always include this file instead of <windows.h>

#if !defined(_WIN32_WINNT)
    #define _WIN32_WINNT 0x0501
#endif
#include <windows.h>


#endif // WINDOWS_H_INCLUDED
