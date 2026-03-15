#ifndef VSCL_API
#if defined(_WIN32) && !defined(__GNUC__)
#define VSCL_API __declspec( dllimport )
#else
#define VSCL_API
#endif // _WIN32
#endif // VSCL_API
