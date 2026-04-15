#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "libtestrig_api.h"

/*
 *	Socket read flag
 *
 *	When receiving HEAD_DC, disconnect right after the client does.
 */
TESTRIG_API extern const int DC_WITH_CLIENT;

/*
 *	For use with socket handler functions.
 *	TODO (post-refactor): Do I actually need this?
 */
TESTRIG_API enum HEADER_RET {
	// When receiving HEAD_DC, disconnect right after the client does.
	HANDLER_RET_STAY = 1,
	HANDLER_RET_DC = 2
};

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
