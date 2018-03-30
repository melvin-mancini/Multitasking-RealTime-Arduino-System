
/* 
 * ARTE *TEST*
 * Author: 2015, Marco Pagani
*/

#include "arte.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifndef __RT_ARDUINO__

void arteLock(void) {}
void arteUnlock(void) {}

// For test purpose
uint8_t arteEnabled(void) {return 0;}
uint8_t lockNestingLevel(void) { return 0;}

#endif


#ifdef __cplusplus
}
#endif
