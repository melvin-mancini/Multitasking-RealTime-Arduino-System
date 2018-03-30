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
 * Author: 2001,2002 Paolo Gai
 *
 * CVS: $Id: ee_kernel.h,v 1.10 2006/12/05 21:18:57 pj Exp $
 */

#ifndef __INCLUDE_FP_KERN_H__
#define __INCLUDE_FP_KERN_H__

#include "kernel/fp/inc/ee_common.h"

/*************************************************************************
 Public Kernel Types and Constants
 *************************************************************************/

#define INVALID_TASK EE_NIL

typedef EE_TID TaskType;
typedef EE_TYPERESOURCE ResourceType;


/*************************************************************************
 Kernel Primitives
 *************************************************************************/


/* This macros are used to define a task */
#define DeclareTask(t) void Func##t(void)
#define TASK(t) void Func##t(void)


/* This primitive activates a task once and then execute the
   preemption test */
#ifndef __PRIVATE_ACTIVATETASK__
void EE_fp_ActivateTask(TaskType t);
#endif

#ifndef __PRIVATE_SCHEDULER__
void EE_fp_Schedule(void);
#endif


#ifndef __FP_NO_RESOURCE__

#ifndef __PRIVATE_GETRESOURCE__
void EE_fp_GetResource(ResourceType m);
#endif

#ifndef __PRIVATE_RELEASERESOURCE__
void EE_fp_ReleaseResource(ResourceType m);
#endif

#endif

#endif
