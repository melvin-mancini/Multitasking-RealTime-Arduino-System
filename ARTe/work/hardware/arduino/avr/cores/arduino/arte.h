
/* 
 * ARTE *TEST*
 * Author: 2015, Marco Pagani
*/

#ifndef ARTE_H
#define ARTE_H

#ifdef __cplusplus
extern "C" {
#endif

// C99 header
#include <stdint.h>

void arteLock(void);
void arteUnlock(void);

// For test purpose
uint8_t arteEnabled(void);
uint8_t lockNestingLevel(void);

#ifdef __cplusplus
}
#endif

#endif /* ARTE_H */
