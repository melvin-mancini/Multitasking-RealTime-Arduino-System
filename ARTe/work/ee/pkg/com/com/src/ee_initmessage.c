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
 * CVS: $Id: ee_initmessage.c,v 1.2 2005/07/17 13:58:36 pj Exp $
 */

#include "com/com/inc/ee_comprv.h"

/****************
 * INIT_MESSAGE *
 ****************
 * I have to make an initialization of all transmit/receive messages
 * The initialization of an external message was mirrored in i-pdu data area
 * On queued messages the number of received messages was set to 0
 ***************************************
 * IN 
 *    Message: Message identifier
 *    DataRef: Reference to data to use in initialization
 * RETURN: 
 *    Return state
 */

/* 2.9.2.2.4 */
#ifndef __PRIVATE_COM_INITMESSAGE__
StatusType EE_com_InitMessage(MessageIdentifier Message,
                 ApplicationDataRef DataRef)
{
  register struct EE_com_msg_RAM_TYPE * msg_RAM;
  register const struct EE_com_msg_ROM_TYPE * msg_ROM;
#ifdef __COM_HAS_ERRORHOOK__  	
	register EE_FREG flags;
#endif
  
  #if defined(__COM_CCC0__) || defined(__COM_CCC1__)
  EE_UINT8 EE_com_temp_buffer[EE_com_bit2byte(EE_UINT8)];
  #endif
       
#ifdef __EE_COM_EXTENDED__
  if ((Message > EE_COM_N_MSG) || 
      (EE_com_msg_ROM[Message]->size == 0) || 
      ((EE_com_msg_RAM[Message]->property & 0x8100) == 0x0100))
  {
    EE_com_sys2user.service_error = COMServiceId_InitMessage;
#ifdef __COM_HAS_ERRORHOOK__ 
    flags = EE_hal_begin_nested_primitive();    
      EE_com_ErrorHook.proc_param.OtherCOM.Message = Message;
      EE_com_ErrorHook.proc_param.OtherCOM.DataRef = DataRef;
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

  msg_ROM = EE_com_msg_ROM[Message];
  msg_RAM = EE_com_msg_RAM[Message];

  DisableAllInterrupts();

  msg_RAM->property &= EE_SET_COM_MSG_OK;   /* set status to E_OK */
      
#if defined(__COM_CCC0__) || defined(__COM_CCC1__)    
  if ((msg_RAM->property & (EE_MASK_MSG_TXRX | EE_MASK_MSG_INTEXT))
        == (EE_COM_MSG_TX | EE_COM_MSG_EXT)) 
  { /* External transmit message (CCC0 or CCC1) */
    /* Note: for internal transmit messages, no initialization takes
       place (p. 33, line 13) */

#ifdef __COM_CCC1__
    /* CPU Message Callout */
    if ((msg_ROM->cpu_callout != NULL) && 
          (msg_ROM->cpu_callout() == EE_COM_FALSE))
      return EE_COM_FALSE;
#endif
      
    /*
     * BYTE ORDERING CONVERSION
     **
     * NOTE:
     *     byte ordering must be done considering the data
     *     interpreted as unsigned integer (pag. 24).
     */
    if ((msg_RAM->property & EE_MASK_MSG_ORD) != EE_COM_MSG_NORD)
    {
      EE_UINT8 i;
      
      for (i=0; i<(msg_ROM->size >> 3); i++)
        EE_com_temp_buffer[i] = DataRef[i];
        
      EE_com_convert (msg_RAM->property, EE_com_temp_buffer);
    }
         
#ifdef __COM_CCC1__
    /* Network message callout */
    if ((msg_ROM->net_callout != NULL) && 
          (msg_ROM->net_callout() == EE_COM_FALSE))
      return EE_COM_FALSE;
#endif
    /* Store initialization data into I-PDU */
   
	DisableAllInterrupts();
	  
	  if ((msg_RAM->property & EE_MASK_MSG_STDY) ==
          EE_COM_MSG_DYN) 
    {  
      /* On dynamic length messages, first of all I have to store its
       * length and than the effective value
       */
      EE_com_memo(&(msg_ROM->size), 0, msg_ROM->ipdu_ROM->data, 
          msg_ROM->ipdu_pos, sizeof(EE_UINT8)*8 );      
      EE_com_memo(DataRef, 0, msg_ROM->ipdu_ROM->data, 
          msg_ROM->ipdu_pos+sizeof(EE_UINT8)*8,
          msg_ROM->size );
    } 
    else 
      /* Static length message */
      if ((msg_RAM->property & EE_MASK_MSG_ORD) != EE_COM_MSG_NORD)
        EE_com_memo(DataRef, 0, msg_ROM->ipdu_ROM->data, 
              msg_ROM->ipdu_pos, msg_ROM->size);
      else 
        EE_com_memo(EE_com_temp_buffer, 0, 
              msg_ROM->ipdu_ROM->data, msg_ROM->ipdu_pos, msg_ROM->size);
            
	  EnableAllInterrupts();
  } 
  else 
#endif
  {  
    /* Receive message */

#if defined(__COM_CCCB__) || defined(__COM_CCC1__)
    if ((msg_RAM->property & EE_MASK_MSG_QUEUNQUE) ==
          EE_COM_MSG_QUE) 
    { /* Queued message 
       * InitMessage sets the number of received messages 
       * to 0. (page 33, line 18) 
       */
      register struct EE_COM_msg_queue *temp = 
            (struct EE_COM_msg_queue *)msg_ROM->data;
         
      temp->tot=0;
      temp->last=0;
      temp->first=0;
      msg_RAM->property &= EE_SET_COM_MSG_OK;
      msg_RAM->property |= E_COM_NOMSG;
    } 
    else 
#endif
#ifdef __COM_CCC1__
      if ((msg_RAM->property & EE_MASK_MSG_STDY) ==
            EE_COM_MSG_DYN) 
      { /* Unqueued dynamic length message */
        EE_com_memo (&(msg_ROM->size), 0, msg_ROM->data, 
              0, sizeof(EE_UINT8)*8);
        EE_com_memo (DataRef, 0, msg_ROM->data, 
              sizeof(EE_UINT8)*8, msg_ROM->size);
      }
      else
#endif     
        /* Unqueued static length message */
        EE_com_memo (DataRef, 0, msg_ROM->data, 0, msg_ROM->size);

      /* Usefull ? */         
/* #if defined(__COM_CCC0__) || defined (__COM_CCC1__)
      if (msg_ROM->ipdu_ROM != NULL) 
      {
   / aggiorno anche l'ipdu collegata (superfluo per i messaggi dinamici) /
   GetResource(EE_MUTEX_COM_IPDU);   
   EE_com_memo(DataRef, 0, msg_ROM->ipdu_ROM->data, 
            msg_ROM->ipdu_pos, msg_ROM->size);
   ReleaseResource(EE_MUTEX_COM_IPDU);   
      }
#endif
*/
  }
   
	EnableAllInterrupts();	
	return E_OK;
}
#endif
