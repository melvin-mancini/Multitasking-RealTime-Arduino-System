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
 * Author 2003 Francesco Bertozzi, Paolo Gai, 2012 Gianluca Franchino.
 * CVS: $Id: ee_comprv.h,v 1.3 2006/04/08 21:15:23 pj Exp $
 */

#ifndef __EE_COMPRV_H__
#define __EE_COMPRV_H__

//#include "ee.h"

#include "com/ee_com.h"
#include "ee_callout.h"
#include "ee_cominit.h"

/*************************************************************************
 Internal data defined by the system
 *************************************************************************/

/* a temporary buffer used inside the COM Library. It is defined into
  * src/com/com/inc/ee_com.c. 
  */
extern EE_UINT16 EE_com_temp_buffer[];

#if defined(__COM_CCC0__) || defined(__COM_CCC1__)
extern struct EE_com_sys_TYPE {
#ifdef __COM_CCC1__
  MessageIdentifier first_DM;
  MessageIdentifier first_PM;
#endif
  MessageIdentifier first_TM;
} EE_com_sys;
#endif


/*************************************************************************
 Functions called internally by the library
 *************************************************************************/

#ifndef __PRIVATE_COM_MEMO__
void EE_com_memo(EE_UINT8 *source, EE_UINT8 s_from, 
			EE_UINT8 *dest, EE_UINT8 d_from, 
			EE_UINT8 length);
#endif

#ifndef __PRIVATE_COM_MSG_RECEPTION__
void EE_com_msg_reception (MessageIdentifier Message, 
				  EE_UINT8 *ipdu_data, 
				  EE_UINT16 rx_from);
#endif

#ifndef __PRIVATE_COM_SEND__
StatusType EE_com_send (MessageIdentifier Message, EE_UINT8 *DataRef, 
			       EE_UINT8 size);
#endif

#ifndef __PRIVATE_COM_DEADLINE_MONITOR__
void EE_com_monitor_handler (void);
#endif

#ifndef __PRIVATE_COM_DEADLINE_MONITOR__
void EE_com_period_monitor (void);
#endif

#ifndef __PRIVATE_COM_DEADLINE_MONITOR__
void EE_com_deadline_monitor (void);
#endif

#ifndef __PRIVATE_COM_DEADLINE_MONITOR__
void EE_com_time_monitor (void);
#endif

#ifdef __COM_CCC1__
#ifndef __PRIVATE_COM_NOTIFY_ERROR__
void EE_com_notify_error (MessageIdentifier MSG);
#endif
#endif

#ifndef __PRIVATE_COM_CONVERT__
void EE_com_convert (EE_UINT16 property, void *data);
#endif

#ifndef __PRIVATE_COM_CONFIRM__
void EE_com_confirm (EE_UINT8 * data_id, EE_UINT8 result);
#endif

/* #ifndef __PRIVATE_COM_GET_IPDU__
void EE_com_get_ipdu (EE_UINT8 MType, EE_UINT8 N_SA, 
           EE_UINT8 N_TA, EE_UINT8 N_TAtype, 
           EE_UINT8 *MessageData, EE_UINT16 length, 
           EE_UINT8 result);
#endif
*/

void EE_com_indication (EE_UINT8 *data_id, 
      EE_UINT8 size, EE_UINT8 *data);

#ifndef __PRIVATE_COM_INSERT_DM__
void EE_com_insert_DM (MessageIdentifier ipdu_num, EE_UINT8 reload);
#endif

#ifndef __PRIVATE_COM_CANCEL_DM__
void EE_com_remove_DM (MessageIdentifier num);
#endif

#ifndef __PRIVATE_COM_INSERT_TM__
void EE_com_insert_TM (MessageIdentifier ipdu_num, EE_UINT8 reload);
#endif

#ifndef __PRIVATE_COM_CANCEL_TM__
void EE_com_remove_TM (MessageIdentifier num);
#endif

#ifndef __PRIVATE_COM_INSERT_PM__
void EE_com_insert_PM (MessageIdentifier ipdu_num, EE_UINT8 reload);
#endif

#ifndef __PRIVATE_COM_CANCEL_PM__
void EE_com_remove_PM (MessageIdentifier num);
#endif

/*************************************************************************
 Masks
 *************************************************************************/
#define EE_MASK_IPDU_TXRX 0x80
#define EE_MASK_IPDU_DPM   0x60
#define EE_MASK_IPDU_PEND  0x10
#define EE_MASK_IPDU_DM    0x08
#define EE_MASK_IPDU_TM    0x04
#define EE_MASK_IPDU_DMTM  0x0C

#define EE_MASK_MSG_TXRX  0x8000
#define EE_MASK_MSG_N_OK  0x7000
#define EE_MASK_MSG_N_ER  0x0E00
#define EE_MASK_MSG_INTEXT   0x0100
#define EE_MASK_MSG_QUEUNQUE   0x0100
#define EE_MASK_MSG_STDY  0x0080
#define EE_MASK_MSG_TRPD  0x0040
#define EE_MASK_MSG_BORD  0x0030
#define EE_MASK_MSG_ORD  0x0008
#define EE_MASK_MSG_STAT  0x0007


/*
#define EE_MASK_IPDU_TYPE              0xE0 
#define EE_MASK_IPDU_PENDING           0x10
#define EE_MASK_IPDU_DMTM_STATUS       0x0C
#define EE_MASK_IPDU_DM_STATUS         0x08
#define EE_MASK_IPDU_DIRECTION         0x80

#define EE_MASK_MSG_STATUS           0x0007
#define EE_MASK_MSG_ORDER            0x8038
#define EE_MASK_MSG_LENGTH_TYPE      0x0080

#define EE_MASK_MSG_TX_TYPE          0x8140

#define EE_MASK_MSG_DIRECTION        0x8000
*/
/* Usefull ? *//* 
//#define EE_MASK_IPDU_ACTIVE            0x02

#define EE_MASK_IPDU_STATUS            0x0C

#define EE_MASK_DM_QUEUED              0x01

#define EE_MASK_MSG_QUEUE_TYPE       0x0100

#define EE_MASK_ERROR_NOTIFY         0x0E00
#define EE_MASK_OK_NOTIFY            0x7000
*/
/*************************************************************************
 set/reset and result
 *************************************************************************/
#define EE_SET_IPDU_TX   0x00
#define EE_SET_IPDU_RX   0x80
#define EE_SET_IPDU_DIR  0x00
#define EE_SET_IPDU_PER  0x20
#define EE_SET_IPDU_MIX  0x40
#define EE_SET_IPDU_PEND 0x10
#define EE_SET_IPDU_DM   0x08
#define EE_SET_IPDU_TM   0x04

#define EE_RESET_IPDU_DM   0xF7
#define EE_RESET_IPDU_TM   0xFB
#define EE_RESET_IPDU_DMTM 0xF3

#define EE_SET_COM_MSG_OK        0xFFF8

#define EE_COM_IPDU_TX   0x00
#define EE_COM_IPDU_RX   0x80
#define EE_COM_IPDU_DIR  0x00
#define EE_COM_IPDU_PER  0x20
#define EE_COM_IPDU_MIX  0x40
#define EE_COM_IPDU_PEND 0x10
#define EE_COM_IPDU_DM_SET   0x08
#define EE_COM_IPDU_DM_CLEAR   0x00
#define EE_COM_IPDU_TM_SET   0x04
#define EE_COM_IPDU_TM_CLEAR   0x00
#define EE_COM_IPDU_DMTM_CLEAR   0x00


#define EE_COM_MSG_TX       0x0000
#define EE_COM_MSG_RX       0x8000
#define EE_COM_MSG_CBOK     0x0000
#define EE_COM_MSG_TSOK     0x1000
#define EE_COM_MSG_FLOK     0x2000
#define EE_COM_MSG_EVOK     0x3000
#define EE_COM_MSG_NTOK     0x4000
#define EE_COM_MSG_CBER     0x0000
#define EE_COM_MSG_TSER     0x0200
#define EE_COM_MSG_FLER     0x0400
#define EE_COM_MSG_EVER     0x0600
#define EE_COM_MSG_NTER     0x0800
#define EE_COM_MSG_INT      0x0100
#define EE_COM_MSG_EXT      0x0000
#define EE_COM_MSG_QUE      0x0000
#define EE_COM_MSG_UNQUE    0x0100
#define EE_COM_MSG_STA      0x0000
#define EE_COM_MSG_DYN      0x0080
#define EE_COM_MSG_TR       0x0000
#define EE_COM_MSG_PD       0x0040
#define EE_COM_MSG_CPUTOBE16 0x0000
#define EE_COM_MSG_CPUTOBE32 0x0010
#define EE_COM_MSG_CPUTOLE16 0x0020
#define EE_COM_MSG_CPUTOLE32 0x0030
#define EE_COM_MSG_BE16TOCPU 0x8000
#define EE_COM_MSG_BE32TOCPU 0x8010
#define EE_COM_MSG_LE16TOCPU 0x8020
#define EE_COM_MSG_LE32TOCPU 0x8030
#define EE_COM_MSG_ORD     0x0000
#define EE_COM_MSG_NORD     0x0008





 
/* #define EE_COM_SET_DM              0x08
#define EE_COM_RESET_DM            0xF7
#define EE_COM_SET_TM              0x04
#define EE_COM_RESET_TM            0xFB
#define EE_COM_RESET_DMTM          0xF3
#define EE_COM_SET_PENDING         0x10
#define EE_COM_RESET_PENDING       0xEF
#define EE_COM_SET_PENDING         0x10
*/

/* Usefull ? */
/*
#define EE_COM_IPDU_RESET_ACTIVE   0xFD
#define EE_COM_IPDU_SET_ACTIVE     0x02
*/

/*
 * Result
 */
/*
#define EE_COM_DMTM_CLEAR          0x00
#define EE_COM_DM_CLEAR            0x00
#define EE_COM_DM_SET              0x80
#define EE_COM_TM_SET              0x40
#define EE_COM_IPDU_TX_DIRECT      0x00
#define EE_COM_IPDU_TX_MIXED       0x40
#define EE_COM_IPDU_TX_PERIODIC    0x20
#define EE_COM_IPDU_RX             0x80
#define EE_COM_IPDU_RX_PERIODIC    0x80
#define EE_COM_IPDU_PENDING        0x10

#define EE_COM_STATIC            0x0000
#define EE_COM_DYNAMIC           0x0080

#define EE_COM_MSG_TX            0x0000
#define EE_COM_EXTERNAL          0x0000

#define EE_COM_BE16TOCPU         0x8000
#define EE_COM_BE32TOCPU         0x8010
#define EE_COM_LE16TOCPU         0x8020
#define EE_COM_LE32TOCPU         0x8030
#define EE_COM_CPUTOBE16         0x0000
#define EE_COM_CPUTOBE32         0x0010
#define EE_COM_CPUTOLE16         0x0020
#define EE_COM_CPUTOLE32         0x0030

#define EE_COM_MSG_TX_TR         0x0000
#define EE_COM_MSG_TX_EX         0x0000
*/

/*
 * Inline functions
 */


#ifndef __PRIVATE_COM_NOTIFY_OK__
__INLINE__ 
void __ALWAYS_INLINE__ EE_com_notify_ok(const struct EE_com_msg_ROM_TYPE *msg_ROM, 
           struct EE_com_msg_RAM_TYPE *msg_RAM)
{
  switch (msg_RAM->property & EE_MASK_MSG_N_OK) {

    /* call an user defined function */
    case EE_COM_MSG_CBOK:
    ((void (*)(void))msg_ROM->notify_call)(); 
    break;   
    
#ifndef __COM_CCCA__
    /* Flag set */
    case EE_COM_MSG_FLOK:
    *(FlagValue *)msg_ROM->notify_call = (FlagValue)EE_COM_TRUE; 
    break;  
#endif
    
    /* activate a task */
    case EE_COM_MSG_TSOK:
		 ActivateTask(*((EE_TID *)msg_ROM->notify_call));
    break; 
#if defined( __OO_ECC1__) || defined (__OO_ECC2__)
    /* Setting an event */
    case EE_COM_MSG_EVOK:
		SetEvent( ((struct EE_COM_event_notify *)msg_ROM->notify_call)->task, 
			((struct EE_COM_event_notify *)(msg_ROM->notify_call))->event );
    break;
#endif	
  }
}
#endif

#endif
