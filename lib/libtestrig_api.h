#pragma once

#ifndef TESTRIG_API
#if defined(_WIN32) && !defined(__GNUC__)
#ifdef COMPILING_TESTRIG_DLL
#define TESTRIG_API __declspec(dllexport)
#else
#define TESTRIG_API __declspec(dllimport)
#endif // COMPILING_TESTRIG_DLL
#else
#define TESTRIG_API
#endif // _WIN32
#endif // TESTRIG_API
