#ifndef VSCL_EXPORT
#ifdef _WIN32
#define VSCL_EXPORT __declspec( dllimport )
#else
#define VSCL_EXPORT
#endif // _WIN32
#endif // VSCL_EXPORT
