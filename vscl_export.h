#ifndef VSCL_API
#ifdef _WIN32
#define VSCL_API __declspec( dllimport )
#else
#define VSCL_API
#endif // _WIN32
#endif // VSCL_API
