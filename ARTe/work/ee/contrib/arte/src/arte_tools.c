
/** 
 *  @file	arte_tools.c
 *  @brief	ARTE *TEST*
 *  @author	Marco Pagani
 *  @date	2015
 */

 
#ifdef __LIB_ARTE__

#include "arte_tools.h"

#include "ee.h"
#include "ee_irq.h"

#ifdef __cplusplus
extern "C" {
#endif

static const uint8_t max_nesting_level_ = 1;
static uint8_t nesting_level_ = 0;

void arteLock(void)
{
	EE_hal_disableIRQ();
	
	if (++nesting_level_ <= max_nesting_level_)
		GetResource(RES_SCHEDULER);
	
	EE_hal_enableIRQ();
}

void arteUnlock(void)
{
	EE_hal_disableIRQ();
	
	if (nesting_level_ > 0) {
		if (--nesting_level_ < max_nesting_level_)
			ReleaseResource(RES_SCHEDULER);
	}
	
	EE_hal_enableIRQ();
}


uint8_t arteEnabled(void)
{
	return 1;
}

uint8_t lockNestingLevel(void)
{
	return nesting_level_;
}

#ifdef __cplusplus
}
#endif

#endif

