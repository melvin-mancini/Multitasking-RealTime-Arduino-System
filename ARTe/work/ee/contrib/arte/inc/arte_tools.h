
/** 
 *  @file	arte_tools.h
 *  @brief	ARTE *TEST*
 *  @author	Marco Pagani
 *  @date	2015
 */

#ifndef ARTE_TOOLS_H
#define ARTE_TOOLS_H


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>

void arteLock(void);
void arteUnlock(void);


uint8_t arteEnabled(void);
uint8_t lockNestingLevel(void);

#ifdef __cplusplus
}
#endif

#endif /* ARTE_TOOLS_H */
