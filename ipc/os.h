#pragma once

#ifdef __cplusplus
namespace VSCL::_detail {
extern "C" {
#endif // __cplusplus

#ifndef _WIN32
#define INVALID_SOCKET -1
#endif

int GetSockDestination(char* dest);

#ifdef __cplusplus
} // extern "C"
} // namespace VSCL::_detail
#endif // __cplusplus
