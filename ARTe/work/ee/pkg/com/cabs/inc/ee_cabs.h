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
 * Author: 2002 Paolo Gai
 * CVS: $Id: ee_cabs.h,v 1.1.1.1 2004/11/05 16:03:03 pj Exp $
 */


#ifndef __INCLUDE_CABS_CABS_H__
#define __INCLUDE_CABS_CABS_H__

/* 

Cyclic Asynchronous Buffers (CABs) for the E.R.I.K.A. Project
-------------------------------------------------------------
CABs are a way to exchange data between different threads.

In particular, every thread always read the latest version of the data
that has been put in the CAB.

The number of buffers that have to be allocated into the CAB are given
by the following formula:

1 + number of readers + number of writers

-------------------------------------------------------------

To limit the overall usage of RAM memory, the CAB descriptor 
has been divided in two parts:

- ROM descriptor
	That contains the minimum set of references to the RAM data,
	plus the informations on the dimension and the number of buffers
- RAM descriptor
	That is composed by a set of arrays that contains the data to be
	exchanged, and of a few other variables to store the CAB status.

-------------------------------------------------------------

Usage:

First,  the CAB types have to be defined. These types are:
EE_CAB_INDEX  An integer type that is used to index a single buffer in the CAB
EE_CAB_DATA   A type used for storing data; usually it is a WORD

Every CAB buffer must be defined. There are two ways to define the CAB buffer:

- using the macro DEFINE_CAB
	That macro can be used to declare all the data structures needed.
	Note that after defining it, it must be initialized calling
	cab_init()
- statically initializing the data structure
	At the end of the development process, the macro can be substituted
	with its preprocessor initialization, allowing a proper separation
	between ROM and RAM parts; this way of initializing a CAB do not need
	the call to cab_init, since the data structures can be initialized
	inline.

Then you can use the cabs with the two pairs of functions that can be used to write
a message in the buffer / read the message in the buffer.


-------------------------------------------------------------

Compiling

To compile the CABS you should add __CABS__ in the EEOPT variable of the makefile.

-------------------------------------------------------------

Optimized implementations:

Optimized implementations can be done. As usual, for each optimized version 
the corresponding symbol should be defined.

(Valid symbols are: __PRIVATE_CAB_INIT__ __PRIVATE_CAB_RESERVE__ __PRIVATE_CAB_PUTMES__
                    __PRIVATE_CAB_GETMES__ __PRIVATE_CAB_UNGET__ )

*/

/* types */
#ifndef EE_CAB_INDEX
#define EE_CAB_INDEX  EE_UREG
#endif

#ifndef EE_CAB_DATA
#define EE_CAB_DATA   EE_UREG
#endif


struct EE_cab_RAM_desc {
	EE_CAB_INDEX mrd;		/* most recent data written on the CAB */
	EE_CAB_INDEX free;		/* free buffer on the top of the stack */
};

struct EE_cab_ROM_desc {
  struct EE_cab_RAM_desc *ram_desc;	/* Most recent data / free message pointer */
  EE_CAB_INDEX    *freestk;		/* Free list pointer */
  EE_CAB_INDEX    *used;			/* CAB usage (pointer) */
  EE_CAB_DATA     *msgs;			/* Message buffers (pointer) */

  EE_CAB_INDEX    dim_msg;			/* dimension of each buffer in the CAB */
  EE_CAB_INDEX    n_msg;			/* number of messages in the CAB */
};


#ifndef __PRIVATE_CAB_INIT__
/* These macros can be used to initalize a cab 
   That contains a message. The cab is initialized with a starting
   message (the index 0.
   Note that freestk has a dimension of the number of messages-1,
   since a message (the mrd) is always allocated.
*/

/* use this to define a CAB */
#define EE_DEFINE_CAB(name, nmsg, dimmsg)                        \
	struct EE_cab_RAM_desc name##_CABram = {nmsg-1, nmsg-1}; \
	EE_CAB_INDEX name##_CABfreestk[nmsg-1];	              \
	EE_CAB_INDEX name##_CABused[nmsg];		              \
	EE_CAB_DATA name##_CABmsgs[nmsg*dimmsg];		      \
	const struct EE_cab_ROM_desc name = {&name##_CABram,name##_CABfreestk,name##_CABused,name##_CABmsgs,dimmsg,nmsg}

/* this to refer it as an extern variable */
#define EE_EXTERN_CAB(name) extern const struct EE_cab_ROM_desc name

/* after defining a CAB, you must initialize it */
void EE_cab_init(const struct EE_cab_ROM_desc *c);
#endif

/* Writing a message on a CAB */
#ifndef __PRIVATE_CAB_RESERVE__
void EE_cab_reserve(const struct EE_cab_ROM_desc *c, void **msg, EE_CAB_INDEX *msg_num);
#endif
#ifndef __PRIVATE_CAB_PUTMES__
void EE_cab_putmes(const struct EE_cab_ROM_desc *c, EE_CAB_INDEX msg_num);
#endif



/* Reading a message from a CAB */
#ifndef __PRIVATE_CAB_GETMES__
void EE_cab_getmes(const struct EE_cab_ROM_desc *c, void **msg, EE_CAB_INDEX *msg_num);
#endif
#ifndef __PRIVATE_CAB_UNGET__
void EE_cab_unget(const struct EE_cab_ROM_desc *c, EE_CAB_INDEX msg_num);
#endif

#endif
