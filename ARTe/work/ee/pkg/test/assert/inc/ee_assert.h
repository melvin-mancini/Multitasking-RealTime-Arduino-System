/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2008  Evidence Srl
 *
 * This file is part of ERIKA Enterprise.
 *
 * ERIKA Enterprise is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation, 
 * (with a special exception described below).
 *
 * Linking this code statically or dynamically with other modules is
 * making a combined work based on this code.  Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * As a special exception, the copyright holders of this library give you
 * permission to link this code with independent modules to produce an
 * executable, regardless of the license terms of these independent
 * modules, and to copy and distribute the resulting executable under
 * terms of your choice, provided that you also meet, for each linked
 * independent module, the terms and conditions of the license of that
 * module.  An independent module is a module which is not derived from
 * or based on this library.  If you modify this code, you may extend
 * this exception to your version of the code, but you are not
 * obligated to do so.  If you do not wish to do so, delete this
 * exception statement from your version.
 *
 * ERIKA Enterprise is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License version 2 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with ERIKA Enterprise; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 * ###*E*### */

/*
 * Author: 2003 Paolo Gai
 * CVS: $Id: ee_assert.h,v 1.1.1.1 2004/11/05 16:03:03 pj Exp $
 */

/* EE Test Assertions
 *
 * This component provides a set of macros/functions which allow to
 * simplify the debug of a generic EE component using an automatic
 * tool. The paradigm of the testing is a "black box" testing. That
 * is, assertions are inserted into the code OUTSIDE the component
 * code, and the test will test the behavior of the system seen by an
 * external observer.
 *
 * To debug a component, a set of test cases have to be produced and
 * verified.
 *
 * Each test case is composed by:
 * 
 * - a set of tasks, resources, ..., with their properties and init values
 *
 * - a sequence of calls, ISRs and assertions that constitute a
 *   typical execution of the test case
 *
 * Definition: The test case is said to be successful if a run
 * of the application respects the sequence.
 *
 * To check if a test case is successful, the automatic tool runs the
 * test case. When the test case ends (i.e., it stops at a predefined
 * breackpoint), the debugger (or the user) have to check that the
 * final assertion is verified. If it is verified, the test is passed.
 *
 * Implementation details
 * ----------------------
 *
 * Assertion internal structure:
 *
 * Assertion Data
 *
 * - Assertions are identified by an identifier (index in an
 *   array). Index 0 is reserved for the final result that says if the
 *   test case is successful or not.
 *
 * - The data structure for an assertion is an array of integers
 *   filled with 0s. The array contains a structure with a field
 *   "value" that is 0 if the assertion has not yet been evaluated, ASSERT_NO
 *   if the assertion has failed, ASSERT_YES if the assertion was
 *   successful. 
 *
 * Assertion functions 
 *
 * - Assertions can run with interrupts enabled. Please notice that if a
 *   preemption by an interrupt or another task occurs during the execution of
 *   an assertion, and the preempting task/interrupt modifies the same
 *   assertion, the check for a double use of an assertion may fail.
 *
 * - The assertion functions should not work as a normal
 *   primitive. Every assertion basically should work with a dedicated
 *   entry in the array. That is, there can not exist two assertions with
 *   the same identifier.
 *
 * - Assertions can rely on other assertion identifiers to implement
 *   precedence constraints. It is guaranteed that an assertion function
 *   will read the value field only once, to avoid problems due to the
 *   fact that assertion runs with interrupts enabled.
 * 
 * Types that have to be defined in types.h
 *
 * - EE_TYPEASSERT is the type used for the assertion index. 
 *   MUST BE SIGNED!!! 
 *
 * - EE_TYPEASSERTVALUE contains at least 3 values: 
 *   0, EE_ASSERT_NO, EE_ASSERT_YES
 *   The type EE_INT8 should work for most architectures.  */

#ifndef __ASSERT_ASSERT_H__
#define __ASSERT_ASSERT_H__

#include "ee.h"


/* types.h */
#ifndef EE_TYPEASSERT
#define EE_TYPEASSERT EE_SREG
#endif

#ifndef EE_TYPEASSERTVALUE
#define EE_TYPEASSERTVALUE EE_INT8
#endif




/* this is an invalid value for the index */
#define EE_ASSERT_NIL      ((EE_TYPEASSERT)(-1))

/* these are the results of each assertion test */
#define EE_ASSERT_INITVALUE   ((EE_TYPEASSERTVALUE)0)
#define EE_ASSERT_YES         ((EE_TYPEASSERTVALUE)1)
#define EE_ASSERT_NO          ((EE_TYPEASSERTVALUE)2)
#define EE_ASSERT_ALREADYUSED ((EE_TYPEASSERTVALUE)3)

/* the assertion array */
extern EE_TYPEASSERTVALUE EE_assertions[];

/* This is the simplest assertion that can be made: 
 *
 * the assertion "id" become YES if the test is true and the prev
 * assertion is YES. 
 *
 * If no PREVious assertion has to be checked, the prev parameter
 * should be EE_ASSERT_NIL.  
 *
 * The return value is either YES or NO depending on the result.
 */
#ifndef __PRIVATE_ASSERT__
EE_TYPEASSERTVALUE EE_assert(EE_TYPEASSERT id, 
			     int test, 
			     EE_TYPEASSERT prev);
#endif

/* These are a simple or/and assertion:
 *
 * the assertion "id" become YES if the prev1 or/and prev2
 * assertions are YES.
 */
#ifndef __PRIVATE_ASSERT_OR__
EE_TYPEASSERTVALUE EE_assert_or(EE_TYPEASSERT id, 
				EE_TYPEASSERT prev1,
				EE_TYPEASSERT prev2);
#endif

#ifndef __PRIVATE_ASSERT_AND__
EE_TYPEASSERTVALUE EE_assert_and(EE_TYPEASSERT id, 
				 EE_TYPEASSERT prev1,
				 EE_TYPEASSERT prev2);
#endif

/* This is a range assertion, typically used as last assertion.
 *
 * the assertion "id" become YES if ALL the assertions between begin
 * and end are YES.
 */

#ifndef __PRIVATE_ASSERT_RANGE__
EE_TYPEASSERTVALUE EE_assert_range(EE_TYPEASSERT id,
				   EE_TYPEASSERT begin,
				   EE_TYPEASSERT end);
#endif

/* This is the last assertion. It simply does nothing, and must be
 * included in all the examples with the purpose of setting a
 * breakpoint there.
 */
#ifndef __PRIVATE_ASSERT_LAST__
EE_TYPEASSERTVALUE EE_assert_last(void);
#endif

#endif
