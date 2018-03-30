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
 * Author: 2003 Francesco Bertozzi
 * CVS: $Id: ee_startcom.c,v 1.2 2005/07/17 13:58:36 pj Exp $
 */

#include "com/com/inc/ee_comprv.h"
#include "com/com/inc/ee_cominit.h"

extern const EE_UINT64 EE_com_msg_init_val[EE_COM_N_MSG];


/************
 * STARTCOM *
 ********************************************
 * starts and initializes the COM subsystem. In particular: 
 * 1. sets to 0 the number of received messages for a queued message 
 * 2. sets to 0 the value of all the unqueued messages 
 * 3. internal and external queued message are initialized 
 * 4. application mode is set to Mode
 * 5. all IPDUs are initialized to 0 
 * 6. Periodic ipdu reception monitor mechanism is started. 
 ****************************************
 * IN     Mode: required startup mode 
 * RETURN status of the operation, E_OK if everything goes ok
 ************
 * NOTE: 
 * Mode is available to StartCOMExtension using GetCOMApplicationMode.
 */


/* 2.9.2.2.1 */
#ifndef __PRIVATE_COM_STARTCOM__
StatusType EE_com_StartCOM(COMApplicationModeType Mode)
{   
  StatusType ret_code;
#if defined(__COM_CCC0__) || defined(__COM_CCC1__)
  EE_UINT8 j;
#endif
  MessageIdentifier i;
#ifdef __COM_HAS_ERRORHOOK__  	
  register EE_FREG flags;
#endif
  register struct EE_com_msg_RAM_TYPE *msg_RAM;
  register const struct EE_com_msg_ROM_TYPE *msg_ROM;
  
#if defined(__COM_CCC0__) || defined(__COM_CCC1__)
  register struct EE_com_ipdu_RAM_TYPE *IPDU_RAM;
  register const struct EE_com_ipdu_ROM_TYPE *IPDU_ROM;
#endif  
  
#ifdef __COM_HAS_ERRORHOOK__
  EE_com_ErrorHook.proc_param.OtherCOM.Message = EE_COM_NULL;
  EE_com_ErrorHook.proc_param.OtherCOM.DataRef = NULL;
  EE_com_ErrorHook.proc_param.OtherCOM.MsgLengthRef = NULL;
  EE_com_ErrorHook.already_executed = EE_COM_FALSE;
#endif
  
#ifdef __EE_COM_EXTENDED__
  if (Mode > EE_COM_N_MODE) 
  {
    EE_com_sys2user.service_error = COMServiceId_StartCOM;
#ifdef __COM_HAS_ERRORHOOK__    
    flags = EE_hal_begin_nested_primitive();        
     EE_com_ErrorHook.proc_param.Mode = Mode;
      if (!EE_com_ErrorHook.already_executed)
      {        
        EE_com_ErrorHook.already_executed = EE_COM_TRUE;
        COMErrorHook(E_COM_ID);    
        EE_com_ErrorHook.already_executed = EE_COM_FALSE;
      }
    EE_hal_end_nested_primitive(flags);        
#endif
    return E_COM_ID;
  }
#endif
  
  EE_com_sys2user.mode = EE_com_validmodes[Mode];
  ret_code = E_OK;
	
  DisableAllInterrupts();
  for (i=0; i<EE_COM_N_MSG; i++) 
  {
    msg_RAM = EE_com_msg_RAM[i];
    if ((msg_RAM->property & EE_MASK_MSG_TXRX) 
          == EE_COM_MSG_RX) 
    {
      /* Receive message */
      msg_ROM = EE_com_msg_ROM[i];
      msg_RAM->property &= EE_SET_COM_MSG_OK;      
#if defined(__COM_CCCB__) || defined(__COM_CCC1__)
      if ((msg_RAM->property & EE_MASK_MSG_QUEUNQUE) 
          == EE_COM_MSG_UNQUE) 
      { /* Unqueued message */
#endif	  
	EE_com_memo((EE_UINT8 *)(&EE_com_msg_init_val[i]), 0, 
				(EE_UINT8 *)msg_ROM->data, 0, msg_ROM->size);

#if defined(__COM_CCCB__) || defined(__COM_CCC1__)
      }
      else 
      { /* Queued message */
        register struct EE_COM_msg_queue *temp_reg = 
        (struct EE_COM_msg_queue *)msg_ROM->data;

        temp_reg->tot = 0;   
        temp_reg->last = 0;  
        temp_reg->first = 0; 
        msg_RAM->property |= E_COM_NOMSG;
      }
#endif
    }
  }

	EnableAllInterrupts();
  
#if defined(__COM_CCC0__) || defined(__COM_CCC1__)
	DisableAllInterrupts();

  for (i=0; i<EE_COM_N_IPDU; i++) 
  {  
    /* I-PDU initialization */
    IPDU_RAM = EE_com_ipdu_RAM[i];
    IPDU_ROM = EE_com_ipdu_ROM[i];
    for (j=0; j < EE_com_bit2byte(IPDU_RAM->size); j++) 
      ((EE_UINT8 *)IPDU_ROM->data)[j] = 0;
    
#ifdef __COM_CCC1__ 
    if ((IPDU_RAM->property & EE_MASK_IPDU_TXRX) 
            == EE_COM_IPDU_RX) 
    { /* Starting DM on all receiving i-pdu (2.5.1, 6th paragraph) */
      IPDU_RAM->property |= EE_SET_IPDU_DM;
      EE_com_insert_DM(i, IPDU_ROM->reload_OFFSET);
    }
    
    if ((IPDU_RAM->property & (EE_MASK_IPDU_TXRX | EE_MASK_IPDU_DPM)) 
            == (EE_COM_IPDU_TX | EE_COM_IPDU_PER))
    { /* Set TM flag in IPDU property */
      IPDU_RAM->property |= EE_SET_IPDU_TM;
    } 
#endif
  }  
	EnableAllInterrupts();
#endif   
   
#ifdef __USE_STARTCOM_EXTENSION__
  ret_code = StartCOMExtension(); /*This is provided by the application*/
#endif
   
#if defined(__COM_CCC0__) || defined(__COM_CCC1__)
  if (ret_code == E_OK) 
    EE_alarm_setrel(EE_COM_ALARM, EE_COM_PERIOD, 1);
#endif
  
  if (ret_code != E_OK) 
  {
    EE_com_sys2user.service_error = COMServiceId_StartCOM;
#ifdef __COM_HAS_ERRORHOOK__    
	flags = EE_hal_begin_nested_primitive();         
      EE_com_ErrorHook.proc_param.Mode = Mode;
      if (!EE_com_ErrorHook.already_executed)
      {        
        EE_com_ErrorHook.already_executed  = EE_COM_TRUE;
        COMErrorHook(E_COM_ID);    
        EE_com_ErrorHook.already_executed = EE_COM_FALSE;
      }
    EE_hal_end_nested_primitive(flags);        
#endif
  }
   
  return ret_code;
}
#endif
