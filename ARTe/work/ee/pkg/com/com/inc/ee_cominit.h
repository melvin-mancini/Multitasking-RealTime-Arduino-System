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
 * Author: 2003 Francesco Bertozzi, Paolo Gai, 2012 Gianluca Franchino.
 * CVS: $Id: ee_cominit.h,v 1.2 2005/07/17 13:58:36 pj Exp $
 */

/* this file should be includede when defining the application data
   structures for the COM Library */

#ifndef __EE_COMINIT_H__
#define __EE_COMINIT_H__

#include "com/ee_com.h"
#include "ee_com.h"

/*************************************************************************
 Internal macros and constants
 *************************************************************************/

/* used only internally to the library */

/* used to call the monitor handler */
void EE_com_monitor_handler(void);
#define EE_COM_MONITOR (EE_ADDR)EE_com_monitor_handler

#define EE_COM_MSG_TO_MSG             1
#define EE_COM_MSG_TO_IPDU            2
#define EE_COM_IPDU_TO_MSG            3

#define EE_COM_MSG_TX            0x0000
#define EE_COM_MSG_RX            0x8000

#define EE_COM_TRIGGERED         0x0000
#define EE_COM_PENDING           0x0040

#define EE_COM_STATIC            0x0000
#define EE_COM_DYNAMIC           0x0000

#define EE_COM_QUEUED            0x0000
#define EE_COM_UNQUEUED          0x0100

#define EE_COM_INTERNAL          0x0100
#define EE_COM_EXTERNAL          0x0000

#define EE_COM_CB_OK             0x0000
#define EE_COM_T_OK              0x1000
#define EE_COM_F_OK              0x2000
#define EE_COM_E_OK              0x3000
#define EE_COM_NO_OK             0x4000

#define EE_COM_CB_ER             0x0000
#define EE_COM_T_ER              0x0200
#define EE_COM_F_ER              0x0400
#define EE_COM_E_ER              0x0600
#define EE_COM_NO_ER             0x0800



#define EE_COM_RX                0x8000

#define EE_COM_DM_QUEUED           0x01

#define EE_COM_cpu_to_be16       0x0000
#define EE_COM_cpu_to_be32       0x0010
#define EE_COM_cpu_to_le16       0x0020
#define EE_COM_cpu_to_le32       0x0030

#define EE_COM_be16_to_cpu       0x0000
#define EE_COM_be32_to_cpu       0x0010
#define EE_COM_le16_to_cpu       0x0020
#define EE_COM_le32_to_cpu       0x0030

#define EE_COM_NO_ORDER          0x0008




/*************************************************************************
 data structure declarations
 *************************************************************************/
 
/* COMErrorHook data structure used to stor info about last 
 * occured error, data structure remapped by COMError_name1_name2 macro
 */
 
#ifdef __COM_HAS_ERRORHOOK__
struct EE_com_ErrorHook_TYPE {
  union 
  {
    COMApplicationModeType Mode;
    struct 
    {
      MessageIdentifier Message;
      ApplicationDataRef DataRef;
      LengthRef MsgLengthRef;
    } OtherCOM;
  } proc_param; 
  
  EE_UINT8 already_executed;
};
#endif
/*
 *
 * IPDU structures 
 *
 */

#if defined(__COM_CCC0__) || defined(__COM_CCC1__) 

/* This is the data structure that contains the static information
   about an IPDU. 

   data: 
     pointer to the I-PDU data
   ipdu_callout: 
     function called before sending the IPDU on the bus
   first: 
     first message that contributes in the insertion of the data in
     the IPDU. These messages receives a notification of the sending
     of the IPDU. The other messages are queued using the message's
     field next_ipdu.
   dest:
     destination node for the IPDU
   reload*:
     timing constants for the IPDU.

*/
struct EE_com_ipdu_ROM_TYPE {
  MessageIdentifier name;
  EE_UINT8 *data;
  StatusType (*ipdu_callout)(void);
  MessageIdentifier first;
  EE_UINT8 reload_DM, 
    reload_PM,          
    reload_TM, 
    reload_OFFSET;
};

/*
 * Property field for IPDUs (in RAM)
 * - TX(0)/RX(1)........................ 1
 * - Direct(00)/Periodic(01)/Mixed(10).. 2
 * - Ric. invio pendente................ 1
 * - DM flag............................ 1
 * - TM flag ........................... 1 
 *
 * This is the bitfield
 * t/r   d/p/m    pend  DM TM  
 * 1     2        1     1  1  - -
 */
struct EE_com_ipdu_RAM_TYPE {
  EE_UINT8 size;                 /* I-PDU size in bit*/
  EE_UINT8 property;             /* see comment above */
   
  EE_UINT8 cont_DM,              /* time variables */
    cont_TM,
    cont_PM;

#ifdef __COM_CCC1__    
  MessageIdentifier next_DM, 
               next_PM; 
#endif  
  MessageIdentifier next_TM; 
};

#endif

/*
 *
 * Message structures 
 *
 */
 
/*
 * message property field:
 *
 * Sending message object properties:
 * ----------------------------------------------------------------------------
 *   tx/rx                                  1 (MUST BE 0)
 *
 *   callback(000b)/task(001b)/flag(010b)
 *   /event(011b)/nothing(100b)             3 notification on correct reception
 *
 *   callback(000b)/task(001b)/flag(010b)
 *   /event(011b)/nothing(100b)             3 notification on erroneous recept.
 *
 *   external/internal                      1 internal message
 *                   
 *   static length/dynamic length           1
 *   triggered/pending                      1
 *
 *   cpu->be16(00b)/cpu->be32(01b)/
 *   cpu->le16(10b)/cpu->le32(11b)          2  byte order
 *                   
 *   byte order/no byte order               1
 *   statusbyte                             3
 * 
 * This is the bitfield
 *    t/r   n_ok  n_er  e/i s/d   t/p   order    no_order    status
 *    1     3     3     1   1     1     2        1           3
 *                   
 * Receiving message object properties:
 * ----------------------------------------------------------------------------
 *   tx/rx                                  1 (MUST BE 1)
 *
 *   callback(000b)/flag(001b)/task(010b)
 *   /event(011b)/nothing(100b)             3 notification on correct reception
 *                   
 *   callback(000b)/flag(001b)/task(010b)
 *   /event(011b)/nothing(100b)             3 notification on erroneous recept.
 *
 *   queued/unqueued                        1
 *                   
 *   static length/dynamic length           1
 *   reserved                               1
 *   be16->cpu(00b)/be32->cpu(01b)/
 *   le16->cpu(10b)/le32->cpu(11b)          2  byte order
 *
 *
 *   no byte order                          1
 *   statusbyte                             3
 *                   
 * This is the bitfield
 *    t/r   n_ok  n_er  q/u   s/d   r     order    no_order    status
 *    1     3     3     1     1     1     2        1           3
 *       
 */ 

struct EE_com_msg_RAM_TYPE {
   EE_UINT16 property;
};

/*
 * size: data length (bit)
 * ipdu_ROM: IPDU to which the data is transmitted (ROM part)
 * ipdu_RAM: IPDU to which the data is transmitted (RAM part)
 * ipdu_pos: which part of the IPDU stores the message
 * filter: filter that must be applied to the message
 * filter_data: data to be passed to the filter
 * notify_call, error_call: notification information - can be a
 *   pointer to a notification , a pointer to a flag, a TID of a
 *   task to be activated or an event mask
 * data: pointer where received messages are stored into. if it is a
 *   queued message it is a pointer to a FIFO queue.
 * next: next message in the list that is interested in the same data. 
 *   In transmission, it refers the internal messages that needs
 *     to receive data. 
 *   In reception, it is the list of the messages that
 *     are interested in IPDU reception.
 * next_ipdu: 
 *  In transmission this is used to inform all messages for 
 *    the result of transmission.
 *  In reception this is the list of messages that are interested in
 *   reception of an IPDU (both correct or erroneous reception).
 */
struct EE_com_msg_ROM_TYPE {
  EE_UINT8 size;
  const struct EE_com_ipdu_ROM_TYPE *ipdu_ROM;
  struct EE_com_ipdu_RAM_TYPE *ipdu_RAM;
  EE_UINT8 ipdu_pos;
  EE_UINT8 (*filter)(void *, void *, EE_UINT8, void *);
  void *filter_data;
  const void *notify_call;
  void *error_call;
  void *data;
  StatusType (*cpu_callout)(void);
  StatusType (*net_callout)(void);
  MessageIdentifier next;
  MessageIdentifier next_ipdu;
};

#if defined(__COM_CCCB__) || defined(__COM_CCC1__)
/* a FIFO queue used for queued messages */
struct EE_COM_msg_queue {
   EE_UINT8 first, last, tot, max;
   EE_UINT8 *data;
};
#endif

/*GF: Start */
struct EE_COM_event_notify {
	TaskType task;
	EventMaskType event;
};
/*GF: End*/


/*************************************************************************
 Data structures defined by/for the user
 *************************************************************************/
#ifdef __COM_HAS_ERRORHOOK__
extern struct EE_com_ErrorHook_TYPE EE_com_ErrorHook;
#endif

/* one value for each possible application mode */
extern COMApplicationModeType EE_com_validmodes[EE_COM_N_MODE];

#if defined(__COM_CCC0__) || defined(__COM_CCC1__) 
/* IPDUs descriptors. One item in rom/ram for each IPDU */
extern const struct EE_com_ipdu_ROM_TYPE *EE_com_ipdu_ROM[EE_COM_N_IPDU];
extern struct EE_com_ipdu_RAM_TYPE * EE_com_ipdu_RAM[EE_COM_N_IPDU];
#endif

/* Message descriptors (rom/ram) */
extern struct EE_com_msg_RAM_TYPE * EE_com_msg_RAM[EE_COM_N_MSG];
extern const struct EE_com_msg_ROM_TYPE * EE_com_msg_ROM[EE_COM_N_MSG];



/*************************************************************************
 IPDU macroes
 *************************************************************************/

/* Note:

   Dynamic messages:
     The IPDU length should include a number of bits at least equal to
     the dimension of the EE_COM_LIMIT type.

   Zero Length messages:
     The IPDU length must not be incremented if some zero length
     messages are present.
*/

#if defined(__COM_CCC0__) || defined(__COM_CCC1__)                             

#define EE_COM_DEFINE_IPDU_PERIODIC(symname,size,ipdu_call,i_tof,i_pdt,i_tout,first)  \
   EE_UINT8 EE_com_data_##symname[EE_com_bit2byte(size)];                        \
   const struct EE_com_ipdu_ROM_TYPE EE_com_ipdu_ROM_##symname = {(symname),EE_com_data_##symname,(ipdu_call),(first),(i_tout),(i_pdt),0,(i_tof) };  \
   struct EE_com_ipdu_RAM_TYPE EE_com_ipdu_RAM_##symname = {(size), 0x20, 0,0,0, EE_COM_NULL,EE_COM_NULL,EE_COM_NULL }

#ifdef __COM_CCC0__
#define EE_COM_DEFINE_IPDU_DIRECT(symname,size,ipdu_call,i_mdt,i_tout,first)  \
   EE_UINT8 EE_com_data_##symname[EE_com_bit2byte(size)];                  \
   const struct EE_com_ipdu_ROM_TYPE EE_com_ipdu_ROM_##symname = {(symname),EE_com_data_##symname,(ipdu_call),(first),(i_tout),0,(i_mdt),0};   \
   struct EE_com_ipdu_RAM_TYPE EE_com_ipdu_RAM_##symname = {(size), 0x00, 0,0,0, EE_COM_NULL}
#else
#define EE_COM_DEFINE_IPDU_DIRECT(symname,size,ipdu_call,i_mdt,i_tout,first)  \
   EE_UINT8 EE_com_data_##symname[EE_com_bit2byte(size)];                  \
   const struct EE_com_ipdu_ROM_TYPE EE_com_ipdu_ROM_##symname = {(symname),EE_com_data_##symname,(ipdu_call),(first),(i_tout),0,(i_mdt),0};   \
   struct EE_com_ipdu_RAM_TYPE EE_com_ipdu_RAM_##symname = {(size), 0x00, 0,0,0, EE_COM_NULL,EE_COM_NULL,EE_COM_NULL}
#endif
   
#define EE_COM_DEFINE_IPDU_MIXED(symname,size,ipdu_call,i_tof,i_mdt,i_pdt,i_tout,first)     \
   EE_UINT8 EE_com_data_##symname[EE_com_bit2byte(size)];                  \
   const struct EE_com_ipdu_ROM_TYPE EE_com_ipdu_ROM_##symname = {(symname),EE_com_data_##symname,(ipdu_call),(first),(i_tout),(i_pdt),(i_mdt),(i_tof)};   \
   struct EE_com_ipdu_RAM_TYPE EE_com_ipdu_RAM_##symname = {(size), 0x40, 0,0,0, EE_COM_NULL,EE_COM_NULL,EE_COM_NULL }
   
#ifdef __COM_CCC0__
#define EE_COM_DEFINE_IPDU_RECEIVE(symname,size,ipdu_call,i_tof,i_tout,first)    \
   EE_UINT8 EE_com_data_##symname[EE_com_bit2byte(size)];                \
   const struct EE_com_ipdu_ROM_TYPE EE_com_ipdu_ROM_##symname = {(symname),EE_com_data_##symname,(ipdu_call),(first),(i_tout),0,0,(i_tof)};  \
   struct EE_com_ipdu_RAM_TYPE EE_com_ipdu_RAM_##symname = {(size), 0x80, 0,0,0, EE_COM_NULL}
#else
#define EE_COM_DEFINE_IPDU_RECEIVE(symname,size,ipdu_call,i_tof,i_tout,first)    \
   EE_UINT8 EE_com_data_##symname[EE_com_bit2byte(size)];                \
   const struct EE_com_ipdu_ROM_TYPE EE_com_ipdu_ROM_##symname = {(symname),EE_com_data_##symname,(ipdu_call),(first),(i_tout),0,0,(i_tof)};  \
   struct EE_com_ipdu_RAM_TYPE EE_com_ipdu_RAM_##symname = {(size), 0x80, 0,0,0, EE_COM_NULL,EE_COM_NULL,EE_COM_NULL}
#endif

   

#define EE_com_ipdu_ROM(symname) &EE_com_ipdu_ROM_##symname
#define EE_com_ipdu_RAM(symname) &EE_com_ipdu_RAM_##symname

#endif






/*************************************************************************
 Message macroes
 *************************************************************************/


#define EE_COM_DEFINE_INTERNAL_MESSAGE(symname,size, receiver) \
   const struct EE_com_msg_ROM_TYPE EE_com_msg_ROM_##symname = {(size), NULL, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL,(receiver),EE_COM_NULL};   \
   struct EE_com_msg_RAM_TYPE EE_com_msg_RAM_##symname = {EE_COM_INTERNAL|E_COM_NOMSG}
   
#define EE_COM_DEFINE_INTERNAL_UNQUEUED_MESSAGE(symname,size,filter,d_filter,n_rx,n_call,next) \
   EE_UINT8 EE_com_unqueued_##symname[EE_com_bit2byte(size)];  \
   const struct EE_com_msg_ROM_TYPE EE_com_msg_ROM_##symname = {(size), NULL, NULL, 0,(filter),(d_filter),(n_call), NULL, EE_com_unqueued_##symname, NULL,NULL,(next),EE_COM_NULL}; \
   struct EE_com_msg_RAM_TYPE EE_com_msg_RAM_##symname = {0x8000|EE_COM_UNQUEUED|(n_rx)|E_COM_NOMSG }

#if defined(__COM_CCCB__) || defined(__COM__CCC1__)
#define EE_COM_DEFINE_INTERNAL_QUEUED_MESSAGE(symname,size,filter,d_filter,n_rx,n_call,length,next)           \
   EE_UINT8 EE_com_buffer_##symname[(length)][EE_com_bit2byte(size)];                  \
   struct EE_COM_msg_queue EE_com_queue_##symname = {0,0,0,(length), EE_com_buffer_##symname[0]};           \
   const struct EE_com_msg_ROM_TYPE EE_com_msg_ROM_##symname = {(size), NULL, NULL, 0,(filter),(d_filter),(n_call),NULL,    \
                              &EE_com_queue_##symname, NULL, NULL,(next),EE_COM_NULL}; \
   struct EE_com_msg_RAM_TYPE EE_com_msg_RAM_##symname = {0x8000|(n_rx)|E_COM_NOMSG}    
#endif

#if defined(__COM_CCC0__) || defined(__COM_CCC1__)  
#define EE_COM_DEFINE_STATIC_MESSAGE(symname,size,ipdu_name,ipdu_pos,tx_prop,filter,d_filter,b_order,n_tx,n_call,e_tx,e_call,cpu_call,net_call,next,next_ipdu)   \
   const struct EE_com_msg_ROM_TYPE EE_com_msg_ROM_##symname = {(size),&EE_com_ipdu_ROM_##ipdu_name, &EE_com_ipdu_RAM_##ipdu_name,(ipdu_pos),(filter),(d_filter),(n_call),(e_call),NULL,(cpu_call),(net_call),(next),(next_ipdu)}; \
   struct EE_com_msg_RAM_TYPE EE_com_msg_RAM_##symname = {(tx_prop)|(b_order)|EE_COM_STATIC|(n_tx)|(e_tx)|E_COM_NOMSG }

#define EE_COM_DEFINE_UNQUEUED_STATIC_MESSAGE(symname,size,ipdu_name,ipdu_pos,filter,d_filter,b_order,n_rx,n_call,e_rx,e_call,cpu_call,net_call,next)               \
   EE_UINT8 EE_com_unqueued_##symname[EE_com_bit2byte(size)];                          \
   const struct EE_com_msg_ROM_TYPE EE_com_msg_ROM_##symname = {(size),&EE_com_ipdu_ROM_##ipdu_name, &EE_com_ipdu_RAM_##ipdu_name,(ipdu_pos),(filter),(d_filter),(n_call),(e_call),EE_com_unqueued_##symname,(cpu_call),(net_call),EE_COM_NULL,(next)}; \
   struct EE_com_msg_RAM_TYPE EE_com_msg_RAM_##symname = { 0x8000|(b_order)|EE_COM_UNQUEUED|(n_rx)|(e_rx)|E_COM_NOMSG }

#define EE_COM_DEFINE_UNQUEUED_DYNAMIC_MESSAGE(symname,size,ipdu_name,ipdu_pos,filter,d_filter,b_order,n_rx,n_call,e_rx,e_call,cpu_call,net_call,next)              \
   EE_UINT8 EE_com_unqueued_##symname[EE_com_bit2byte(size)+sizeof(EE_UINT8)];            \
   const struct EE_com_msg_ROM_TYPE EE_com_msg_ROM_##symname = {(size), &EE_com_ipdu_ROM_##ipdu_name, &EE_com_ipdu_RAM_##ipdu_name,(ipdu_pos),(filter),(d_filter),(n_call),(e_call),EE_com_unqueued_##symname,(cpu_call),(net_call),EE_COM_NULL,(next)};   \
   struct EE_com_msg_RAM_TYPE EE_com_msg_RAM_##symname = { 0x8000|EE_COM_UNQUEUED|EE_COM_DYNAMIC|(b_order)|(n_rx)|(e_rx)|E_COM_NOMSG }

#ifdef __COM_CCC1__
#define EE_COM_DEFINE_DYNAMIC_MESSAGE(symname,maxsize,ipdu_name,ipdu_pos,tx_prop,b_order,n_tx,n_call,e_tx,e_call,cpu_call,net_call,next,next_ipdu)         \
   const struct EE_com_msg_ROM_TYPE EE_com_msg_ROM_##symname = {(maxsize), &EE_com_ipdu_ROM_##ipdu_name, &EE_com_ipdu_RAM_##ipdu_name,(ipdu_pos),F_Always, NULL,(n_call),(e_call),NULL,(cpu_call),(net_call),(next),(next_ipdu)};  \
   struct EE_com_msg_RAM_TYPE EE_com_msg_RAM_##symname = { EE_COM_DYNAMIC|(tx_prop)|(n_tx)|(e_tx)|(b_order)|E_COM_NOMSG }

#define EE_COM_DEFINE_ZERO_MESSAGE_TRANSMIT(symname,ipdu_name,n_tx,n_call,e_tx,e_call,cpu_call,net_call,next,next_ipdu)   \
   const struct EE_com_msg_ROM_TYPE EE_com_msg_ROM_##symname = { 0, &EE_com_ipdu_ROM_##ipdu_name, &EE_com_ipdu_RAM_##ipdu_name,0,F_Always,NULL,(n_call),(e_call),NULL,(cpu_call),(net_call),(next),(next_ipdu)}; \
   struct EE_com_msg_RAM_TYPE EE_com_msg_RAM_##symname = {(n_tx)|EE_COM_no_order|(e_tx)|E_OK }

#define EE_COM_DEFINE_QUEUED_MESSAGE(symname,size,ipdu_name,ipdu_pos,filter,d_filter,b_order,n_rx,n_call,e_rx,e_call,cpu_call,net_call,length,next)           \
   EE_UINT8 EE_com_buffer_##symname[(length)][EE_com_bit2byte(size)];                  \
   struct EE_COM_msg_queue EE_com_queue_##symname = {0, 0, 0,(length),EE_com_buffer_##symname};          \
   const struct EE_com_msg_ROM_TYPE EE_com_msg_ROM_##symname = {(size),&EE_com_ipdu_ROM_##ipdu_name, &EE_com_ipdu_RAM_##ipdu_name,(ipdu_pos),(filter),(d_filter), \
               (n_call),(e_call), &EE_com_queue_##symname,(cpu_call),(net_call),EE_COM_NULL,(next)};  \
   struct EE_com_msg_RAM_TYPE EE_com_msg_RAM_##symname = { 0x8000|(b_order)|(n_rx)|(e_rx)|E_COM_NOMSG }

#define EE_COM_DEFINE_ZERO_MESSAGE_RECEIVE(symname,ipdu_name,n_rx,n_call,e_rx,e_call,cpu_call,net_call,next)  \
   const struct EE_com_msg_ROM_TYPE EE_com_msg_ROM_##symname = { 0, &EE_com_ipdu_ROM_##ipdu_name, &EE_com_ipdu_RAM_##ipdu_name, 0, F_Always, NULL,(n_call),(e_call),NULL,(cpu_call),(net_call),EE_COM_NULL,(next)};  \
   struct EE_com_msg_RAM_TYPE EE_com_msg_RAM_##symname = { 0x8000|(n_rx)|(e_rx)|EE_COM_no_order|E_OK }

#define EE_COM_DEFINE_ZERO_MESSAGE_INTERNAL_RECEIVE(symname,n_rx,n_call,next)   \
   const struct EE_com_msg_ROM_TYPE EE_com_msg_ROM_##symname = { 0,NULL,NULL,0,F_Always,NULL,(n_call),NULL,NULL,NULL,NULL,(next),EE_COM_NULL}; \
   struct EE_com_msg_RAM_TYPE EE_com_msg_RAM_##symname = { 0x8000|(n_rx)|EE_COM_no_order|E_OK }
   
#define EE_COM_DEFINE_INTERNAL_ZERO_MESSAGE(symname,receiver) \
   const struct EE_com_msg_ROM_TYPE EE_com_msg_ROM_##symname = {0, NULL, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL,(receiver),EE_COM_NULL};  \
   struct EE_com_msg_RAM_TYPE EE_com_msg_RAM_##symname = {EE_COM_INTERNAL|E_COM_NOMSG}
   
#endif
#endif

#define EE_com_msg_ROM(symname) &EE_com_msg_ROM_##symname
#define EE_com_msg_RAM(symname) &EE_com_msg_RAM_##symname

#endif
