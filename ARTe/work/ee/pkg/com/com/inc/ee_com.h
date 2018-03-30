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
 * Author: 2003 Francesco Bertozzi, Paolo Gai. 2012 Gianluca Franchino.
 * CVS: $Id: ee_com.h,v 1.2 2005/07/17 13:58:36 pj Exp $
 */

/* This file contains the COM user interface. If you need to
   initialize the application COM data structures, include the
   cominit.h file instead of this one. */

#ifndef __EE_COM_COM_H__
#define __EE_COM_COM_H__

#include "ee.h"
#include "com/ee_com.h"
#include "ee_callout.h"


#if !( defined(__OO_BCC1__) || defined(__OO_BCC2__) || defined(__OO_ECC1__) || \
defined(__OO_ECC2__) || defined(__AS_SC4__) )

__INLINE__ void __ALWAYS_INLINE__ DisableAllInterrupts(void)
{
	EE_hal_disableIRQ();
}

__INLINE__ void __ALWAYS_INLINE__ EnableAllInterrupts(void)
{
	EE_hal_enableIRQ();
}
#endif

/*************************************************************************
 macros and constants
 *************************************************************************/

/* returns the number of bytes used by a bit stream */
#define EE_com_bit2byte(bit) ((((bit)-1) >> 3)+1)


/*GF: EE_COM_NULL already defined in con/ee_com.h as EE_COM_NULL 255 */
#ifdef EE_COM_NULL
#undef EE_COM_NULL
#endif

/* to end a message list into the next / next_ipdu field */
#define EE_COM_NULL (MessageIdentifier)~0

/* used to specify that a message should not be filtered (the filter
   is mapped to F_Always) */
#define EE_COM_NO_FILTER F_Always

/*************************************************************************
 types that can be redefined by the application
 *************************************************************************/

/* status type */
/* the status type, that usually was an used configurable type, is set
   by the Standard (Bindings specification, paragraph 3.2) to be
   unsigned char. */
#ifndef EE_TYPESTATUS
#define EE_TYPESTATUS unsigned char
#endif

#ifndef EE_TYPESYMBOLICNAME
#define EE_TYPESYMBOLICNAME EE_UINT16
#endif

/* MUST BE UNSIGNED */
#ifndef EE_TYPECOMAPPMODE
#define EE_TYPECOMAPPMODE EE_UINT8
#endif

#ifndef EE_TYPECOMSHUTDOWNMODE
#define EE_TYPECOMSHUTDOWNMODE EE_UINT8 
#endif



/*************************************************************************
 COM API
 *************************************************************************/

/***************************************************************************
 * 2.2.2 Reception Filtering
 ***************************************************************************/


#ifndef __PRIVATE_F_ALWAYS__
EE_UINT8 F_Always(void *new_data, void *old_data, 
		     EE_UINT8 size, void *extra_data);
#endif

#ifdef __COM_CCC1__
#ifndef __PRIVATE_F_NEVER__
EE_UINT8 F_Never(void *new_data, void *old_data,
		    EE_UINT8 size, void *extra_data);
#endif

#ifndef __PRIVATE_F_MASKEDNEWEQUALX__
EE_UINT8 F_MaskedNewEqualX(void *new_data, void *old_data,
			      EE_UINT8 size, void *extra_data);
#endif

#ifndef __PRIVATE_F_NEWISEQUAL__
EE_UINT8 F_NewIsEqual(void *new_data, void *old_data,
			 EE_UINT8 size, void *extra_data);
#endif

#ifndef __PRIVATE_F_NEWISDIFFERENT__
EE_UINT8 F_NewIsDifferent(void *new_data, void *old_data, 
			     EE_UINT8 size, void *extra_data);
#endif

#endif


/***************************************************************************
 * 2.6.3 Callbacks
 ***************************************************************************/

/* 2.6.3: used when defining a COM Callback */
#define COMCallback(name) void (name)(void)

/* should be used when declaring a callback as extern;
   COMCallback can only be used in the definition of a callback. */
#define EE_COM_DECLARE_CALLBACK(name) void (name)(void)


/***************************************************************************
 * 2.9.1 Interface to Indirect NM
 ***************************************************************************/

/* 2.9.1.1 I-PDU transfer indication                                       */
/* ----------------------------------------------------------------------- */

/* not implemented */

/* 2.9.1.2 I-PDU time-out indication                                       */
/* ----------------------------------------------------------------------- */

/* not implemented */


/***************************************************************************
 * 2.9.2 Application Program Interface (API)
 ***************************************************************************/

/* 2.9.2.1 Service parameter types                                         */
/* ----------------------------------------------------------------------- */

/* 2.9.2.1.1, 3.1 (Bindings), 3.2 (Bindings) */
#ifndef STATUSTYPEDEFINED
#define STATUSTYPEDEFINED
typedef EE_TYPESTATUS StatusType;
#define E_OK                                 0
#endif
/*GF: Since the message status is given by the first 3 bits in 
* EE_com_msg_RAM_TYPE->property, it's better to use values in
* the range [1; 7]. 
*/
/*
#define E_COM_ID                            32
#define E_COM_LENGTH                        33
#define E_COM_LIMIT                         34
#define E_COM_NOMSG                         35
#define E_COM_SYS_DISCONNECTED              36
*/
/*GF: looking at Bertozzi'thesis, the following should be the corret values. */
#define E_COM_ID                            1
#define E_COM_LENGTH                        2
#define E_COM_LIMIT                         3
#define E_COM_NOMSG                         4
#define E_COM_SYS_DISCONNECTED              5


/* 2.9.2.1.2 */
typedef EE_TYPESYMBOLICNAME MessageIdentifier;

/* 2.9.2.1.3 - This implementation of COM uses pointer to
           bytes. For that reason, EE_ADDR CANNOT be used. */
typedef void *ApplicationDataRef;

/* 2.9.2.1.4 */
typedef EE_UINT8 *LengthRef;

/* 2.9.2.1.5 */
enum FlagValue_enum { 
  FlagValueF=EE_COM_FALSE, 
  FlagValueT=EE_COM_TRUE 
};

typedef enum FlagValue_enum FlagValue;

/* 2.9.2.1.6 */
typedef EE_TYPECOMAPPMODE COMApplicationModeType;

/* 2.9.2.1.7 */
typedef EE_TYPECOMSHUTDOWNMODE COMShutdownModeType;

#define COM_SHUTDOWN_IMMEDIATE 0

/* 2.9.2.1.8 */
enum CalloutReturnType_enum { 
  CalloutReturnTypeF=EE_COM_FALSE, 
  CalloutReturnTypeT=EE_COM_TRUE
};

typedef enum CalloutReturnType_enum CalloutReturnType;

/* 2.9.2.1.9 */
enum COMServiceIdType_enum {
  COMServiceId_NoError = 0,
  COMServiceId_StartCOM,
  COMServiceId_StopCOM,
  COMServiceId_GetCOMApplicationMode,
  COMServiceId_InitMessage,
  COMServiceId_StartPeriodic,
  COMServiceId_StopPeriodic,
  COMServiceId_ReadFlag,
  COMServiceId_ResetFlag,
  COMServiceId_SendMessage,
  COMServiceId_ReceiveMessage,
  COMServiceId_SendDynamicMessage,
  COMServiceId_ReceiveDynamicMessage,
  COMServiceId_SendZeroMessage,
  COMServiceId_GetMessageStatus,
  COMServiceId_COMErrorGetServiceId
};

typedef enum COMServiceIdType_enum COMServiceIdType;

extern struct EE_com_sys2user_TYPE {
  COMApplicationModeType mode;
  COMServiceIdType service_error;
} EE_com_sys2user;

/* 2.9.2.2 Start-up services                                               */
/* ----------------------------------------------------------------------- */

/* 2.9.2.2.1 */
#ifndef __PRIVATE_COM_STARTCOM__
StatusType EE_com_StartCOM(COMApplicationModeType Mode);
#endif

/* 2.9.2.2.2 */
#ifndef __PRIVATE_COM_STOPCOM__
StatusType EE_com_StopCOM(COMShutdownModeType Mode);
#endif

/* 2.9.2.2.3 */
#ifndef __PRIVATE_COM_GETCOMAPPLICATIONMODE__
#define EE_com_GetCOMApplicationMode() \
  EE_com_sys2user.mode
#endif

/* 2.9.2.2.4 */
#ifndef __PRIVATE_COM_INITMESSAGE__
StatusType EE_com_InitMessage(MessageIdentifier Message,
							  ApplicationDataRef DataRef);
#endif


/* 2.9.2.2.5 */
#ifdef __COM_CCC1__
#ifndef __PRIVATE_COM_STARTPERIODIC__
StatusType EE_com_StartPeriodic(void);
#endif
#endif

/* 2.9.2.2.6 */
#ifdef __COM_CCC1__
#ifndef __PRIVATE_COM_STOPPERIODIC__
StatusType EE_com_StopPeriodic(void);
#endif
#endif

/* 2.9.2.3 Notification mechanism support services                         */
/* ----------------------------------------------------------------------- */

#ifndef __COM_CCCA__

/* used to declare a flag that have to be used in the application */
#define EE_COM_DECLARE_FLAG(name) \
   FlagValue ReadFlag_##name(void); \
   void ResetFlag_##name(void)

/* used to define a flag that have to be used by the application */
#define EE_COM_DEFINE_FLAG(name)   \
   extern FlagValue (name); \
   FlagValue ReadFlag_##name(void) { return (name); } \
   void ResetFlag_##name(void) { (name)=EE_COM_FALSE; }   \
   FlagValue (name)=EE_COM_FALSE
#endif

/* 2.9.2.4 Communication services                                          */
/* ----------------------------------------------------------------------- */

/* 2.9.2.4.1 */
#ifndef __PRIVATE_COM_SENDMESSAGE__
StatusType EE_com_SendMessage(MessageIdentifier Message, 
				     ApplicationDataRef DataRef);
#endif

/* 2.9.2.4.2 */
#ifndef __PRIVATE_COM_RECEIVEMESSAGE__
StatusType EE_com_ReceiveMessage(MessageIdentifier Message,
					ApplicationDataRef DataRef);
#endif

/* 2.9.2.4.3 */
#ifdef __COM_CCC1__
#ifndef __PRIVATE_COM_SENDDYNAMICMESSAGE__
StatusType EE_com_SendDynamicMessage(MessageIdentifier Message, 
					    ApplicationDataRef DataRef, 
					    LengthRef msg_length);
#endif
#endif

/* 2.9.2.4.4 */
#ifdef __COM_CCC1__
#ifndef __PRIVATE_COM_RECEIVEDYNAMICMESSAGE__
StatusType EE_com_ReceiveDynamicMessage(MessageIdentifier Message,
					       ApplicationDataRef DataRef,
					       LengthRef msg_length);
#endif
#endif

/* 2.9.2.4.5 */
#ifdef __COM_CCC1__
#ifndef __PRIVATE_COM_SENDZEROMESSAGE__
StatusType EE_com_SendZeroMessage(MessageIdentifier Message);
#endif
#endif

/* 2.9.2.4.6 */
#if defined(__COM_CCCB__) || defined(__COM_CCC1__)
#ifndef __PRIVATE_COM_GETMESSAGESTATUS__
StatusType EE_com_GetMessageStatus(MessageIdentifier Message);
#endif
#endif

/* 2.9.2.4.7 */
#ifndef __PRIVATE_COM_COMERRORGETSERVICEID__
#define EE_com_COMErrorGetServiceId() \
  EE_com_sys2user.service_error
#endif

/***************************************************************************
 * 2.9.3 Routines provided by the application
 ***************************************************************************/

/* 2.9.3.1 */
/* this routine is defined by the application */
StatusType StartCOMExtension(void);

/* 2.9.3.2 */
#define EE_COM_DECLARE_CALLOUT(CalloutRoutineName) CalloutReturnType CalloutRoutineName(void)

#define COMCallout(CalloutRoutineName) CalloutReturnType CalloutRoutineName(void)

/* 2.9.3.3 */
#ifdef __COM_HAS_ERRORHOOK__
void COMErrorHook (StatusType Error);
#endif

/* 2.9.3.4 */
/* See in cominit.h
 */

#endif
