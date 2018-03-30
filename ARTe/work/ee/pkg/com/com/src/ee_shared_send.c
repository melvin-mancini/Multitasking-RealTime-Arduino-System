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
 * CVS: $Id: ee_shared_send.c,v 1.2 2005/07/17 13:58:36 pj Exp $
 */

#include "ee.h"
#include "com/com/inc/ee_comprv.h"

/*
 * EE_com_send
 * 
 * This function is used by SendMessage, SendDynamicMessage 
 * and SendZeroMessage to send an ipdu
 */
StatusType EE_com_send (MessageIdentifier Message, EE_UINT8 *DataRef, EE_UINT8 size)
{  
  register const struct EE_com_msg_ROM_TYPE *msg_ROM = 
        EE_com_msg_ROM[Message];

#if defined(__COM_CCC0__) || defined(__COM_CCC1__)
  register struct EE_com_msg_RAM_TYPE *msg_RAM = 
    EE_com_msg_RAM[Message];

  EE_UINT8 temp;
  EE_UINT8 EE_com_temp_buffer[EE_com_bit2byte(EE_UINT8)];
#endif

  /*
   * Check and deliver message to internal receivers
   */

  if (msg_ROM->next != EE_COM_NULL) 
  {
    EE_com_msg_reception(msg_ROM->next,
        DataRef, EE_COM_INTERNAL);
  }
    
#if defined(__COM_CCC0__) || defined(__COM_CCC1__)
#ifdef __COM_CCC1__
  if (msg_ROM->filter != F_Always)
  {
    /* If I have to do a filtering operation, I have to bring (?) old 
       ipdu data */
    EE_com_memo (msg_ROM->ipdu_ROM->data, msg_ROM->ipdu_pos,
          EE_com_temp_buffer, 0, msg_ROM->size);
      
    temp = msg_ROM->filter(DataRef, EE_com_temp_buffer,
          msg_ROM->size/8, msg_ROM->filter_data);
  }
  else
    temp = EE_COM_TRUE;
    
  if (temp == EE_COM_TRUE)
#endif
    { 
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
      if ((msg_RAM->property & EE_MASK_MSG_ORD) != 
            EE_COM_MSG_NORD)
      {
        EE_UINT8 i;
        
        for (i=0; i<(msg_ROM->size >> 3); i++)
          EE_com_temp_buffer[i] = DataRef[i];
          
        EE_com_convert (msg_RAM->property, EE_com_temp_buffer);
      }
       
#ifdef __COM_CCC1__
      /*Network message callout*/
      if ((msg_ROM->net_callout != NULL) && 
            (msg_ROM->net_callout() == EE_COM_FALSE))
        return EE_COM_FALSE;
#endif

      /* Store data into ipdu object */
      if ((msg_RAM->property & EE_MASK_MSG_STDY) ==
            EE_COM_MSG_DYN) 
      { /* In dynamic messages, I have to store the message length 
         * in first step and than the message data.
         */
        temp = size;
        EE_com_memo(&temp, 0, msg_ROM->ipdu_ROM->data, 
              msg_ROM->ipdu_pos, sizeof(EE_UINT8)*8 );      
        EE_com_memo(DataRef, 0, msg_ROM->ipdu_ROM->data, 
              msg_ROM->ipdu_pos+sizeof(EE_UINT8)*8, size );
      } 
      else 
      { /* Static length message */
        //extern EE_UINT16 pre;
        //extern EE_UINT16 post;
  
        //pre=*(EE_UINT16 *)0xFE48;
        EE_com_memo(DataRef, 0, msg_ROM->ipdu_ROM->data, 
                msg_ROM->ipdu_pos, size );
        //post=*(EE_UINT16 *)0xFE48;
      }

#ifdef __COM_CCC1__
        /*IPDU Message Callout*/
        if ((msg_ROM->ipdu_ROM->ipdu_callout != NULL) && 
              (msg_ROM->ipdu_ROM->ipdu_callout() == EE_COM_FALSE))
        return EE_COM_FALSE;
#endif      
      /* Check if I have to transmit */
      if ((msg_RAM->property & (EE_MASK_MSG_TXRX | EE_MASK_MSG_TRPD)) == 
            (EE_COM_MSG_TX | EE_COM_MSG_TR)) 
      { /* A trigger message should trigger an ipdu transmission */
        if ((msg_ROM->ipdu_RAM->property & EE_MASK_IPDU_DMTM) == 
            EE_COM_IPDU_DMTM_CLEAR)
        { /* A transmission request go here */
          /* N_USData.request(DIAGNOSTIC, 
                msg_ROM->ipdu_ROM->name, msg_ROM->ipdu_ROM->dest,
                PHYSICAL, msg_ROM->ipdu_ROM->data, 
                EE_com_bit2byte(msg_ROM->ipdu_RAM->size));
           */
#ifdef __COM_CCC1__  
          EE_com_insert_DM(msg_ROM->ipdu_ROM->name, 
                msg_ROM->ipdu_ROM->reload_DM);
          
          msg_ROM->ipdu_RAM->property &= EE_SET_IPDU_DM;       
#endif
        }
        else
        { /* Set pending request */
          msg_ROM->ipdu_RAM->property |= EE_SET_IPDU_PEND;
        }
      } /* if on transmission check */ 
    } /*if (from filter under CCC1)*/
    
  /* resetto i flag di notifica */
  if ((msg_RAM->property & 0x7000) == EE_COM_F_OK)
    *(FlagValue *)msg_ROM->notify_call = EE_COM_FALSE;

  if ((msg_RAM->property & 0x0E00) == EE_COM_F_ER)
    *(FlagValue *)msg_ROM->error_call = EE_COM_FALSE;
  
#endif   
  return E_OK;
}




