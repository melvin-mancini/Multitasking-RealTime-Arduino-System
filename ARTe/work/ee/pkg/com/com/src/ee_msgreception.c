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
 * CVS: $Id: ee_msgreception.c,v 1.2 2005/07/17 13:58:36 pj Exp $
 */

#include "com/com/inc/ee_comprv.h"

/*********************************************************
 *       MSG RECEPTION                     
 **********************************************************
 * This function is used to shunt ipdu or internal message data
 **********************************************************
 * IN:
 *    Message: First (identifier) message interested in data
 *    DataRef: Reference to data
 *    rx_from: To inform about an internal or external transfer
 **********************************************************/

#ifndef __PRIVATE_COM_MSG_RECEPTION__
void EE_com_msg_reception (MessageIdentifier Message,
          EE_UINT8 * DataRef,
          EE_UINT16 rx_from)
{
  EE_UINT8 from;  
  EE_UINT8 just_copy;
  struct EE_com_msg_RAM_TYPE * msg_RAM;
  const struct EE_com_msg_ROM_TYPE * msg_ROM;
  
  #if defined(__COM_CCC0__) || defined(__COM_CCC1__)
  EE_UINT8 EE_com_temp_buffer[EE_com_bit2byte(EE_UINT8)];
  #endif

#ifdef __COM_CCC1__
  EE_UINT8 temp;
  EE_UINT8 * punt;
#endif
   
  /* while there are messages in "queue"
   */
  while (Message != EE_COM_NULL) 
  {  
    msg_ROM = EE_com_msg_ROM[Message];
    msg_RAM = EE_com_msg_RAM[Message];
    from = msg_ROM->ipdu_pos;
    just_copy = EE_COM_FALSE;     
    
#if defined(__COM_CCC0_) || defined(__COM_CCC1__)
    if (rx_from == EE_COM_MSG_EXT) 
    {
      if (((msg_RAM->property & EE_MASK_MSG_ORD) != 
            EE_COM_MSG_NORD) || 
            (msg_ROM->filter != F_Always)) 
      {
        EE_com_memo(DataRef, msg_ROM->ipdu_pos, 
              EE_com_temp_buffer, 0, msg_ROM->size);
        just_copy = EE_COM_TRUE;
      }
       
#ifdef __COM_CCC1__
       /*Network message callout*/
      if ((msg_ROM->net_callout != NULL) && 
            (msg_ROM->net_callout() == EE_COM_FALSE))
        return;
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
        EE_com_convert (msg_RAM->property, EE_com_temp_buffer);
       
       
#ifdef __COM_CCC1__
       /*CPU Message Callout*/
      if ((msg_ROM->cpu_callout != NULL) && 
            (msg_ROM->cpu_callout() == EE_COM_FALSE))
        return;
#endif
    }
#endif
     
	  DisableAllInterrupts();
#ifdef __COM_CCC1__
    if (msg_ROM->filter == F_Always) 
      /* I don't want to apply any filter if it is a zero-length message
       * or if it's linked to a F_Always filter
       */
      temp = EE_COM_TRUE;
    else 
    { /* I have to check the filter result */
      if ((msg_RAM->property & EE_MASK_MSG_QUEUNQUE) 
            == EE_COM_MSG_QUE) 
      { /* Queued message */
        register struct EE_COM_msg_queue *temp_reg = 
              (struct EE_COM_msg_queue *)msg_ROM->data;
   
        punt = &(temp_reg->data[temp_reg->last * EE_com_bit2byte(msg_ROM->size)]);
   
        /* Paying attention is necessary because 
         * the queue can be an empty queue
         */
        temp = msg_ROM->filter(EE_com_temp_buffer, 
              punt, msg_ROM->size/8, msg_ROM->filter_data);
      }
      else /* Static message */
        temp = msg_ROM->filter(EE_com_temp_buffer, 
              (EE_UINT8 *)msg_ROM->data, msg_ROM->size/8, msg_ROM->filter_data);
    }
     
    if ((temp == EE_COM_TRUE) && (msg_ROM->size != 0))
    { /* If the filter don't stop, i have to store new data 
       */
#endif 
      
#if defined(__COM_CCCB__) || defined(__COM_CCC1__)
      if ((msg_RAM->property & EE_MASK_MSG_QUEUNQUE) 
            == EE_COM_MSG_QUE)
      /* queued message */
        if (((struct EE_COM_msg_queue *)msg_ROM->data)->tot !=
            ((struct EE_COM_msg_queue *)msg_ROM->data)->max) 
        { /* There is some space in queue */   
          register struct EE_COM_msg_queue *temp_reg = 
                (struct EE_COM_msg_queue *)msg_ROM->data; 
#ifdef __COM_CCC1__
          if (just_copy == EE_COM_TRUE)
            EE_com_memo(EE_com_temp_buffer, 0, 
              &(temp_reg->data[temp_reg->last * EE_com_bit2byte(msg_ROM->size)]), 
              0, msg_ROM->size);
          else 
#endif
            EE_com_memo (DataRef, from, 
              &(temp_reg->data[temp_reg->last * EE_com_bit2byte(msg_ROM->size)]), 
              0, msg_ROM->size);
     
          // temp_reg->last = ++temp_reg->last % temp_reg->max;
          // substituted with an if statement to avoid the mod operation
          temp_reg->last++;
          if (temp_reg->last == temp_reg->max)
            temp_reg->last = 0;
     
          temp_reg->tot++;
          msg_RAM->property &= 0xfff8;  
          msg_RAM->property |= E_OK; 
        } 
        else 
        { /* Full queue */
          msg_RAM->property &= 0xfff8;  
          msg_RAM->property |= E_COM_LIMIT;
        }
      else 
      {
#ifdef __COM_CCC1__
        if ((msg_RAM->property & EE_MASK_MSG_STDY) 
              == EE_COM_MSG_DYN)
        /* I have to store size and then data of a dynamic message
         * (no filter or byte ordering applied) 
         */
          EE_com_memo (DataRef, from, (EE_UINT8 *)msg_ROM->data, 0, 
                msg_ROM->size+sizeof(EE_UINT8));                  
        else
#endif
#endif
#if defined(__COM_CCC0__) || defined(__COM_CCC1__)
        /* I have to store new data into static message */
          if (just_copy == EE_COM_TRUE)
          /* If I have applied a filter or 
           * byte ordering operation 
           */
            EE_com_memo(EE_com_temp_buffer, 0, 
                  (EE_UINT8 *)msg_ROM->data, 
                  0, msg_ROM->size);
          else 
#endif
            EE_com_memo(DataRef, from, 
                  (EE_UINT8 *)msg_ROM->data,
                  0, msg_ROM->size);
            
        msg_RAM->property &= 0xfff8; 
        msg_RAM->property |= E_OK; 
        
#if defined(__COM_CCCB__) || defined(__COM_CCC1__)
      }
#ifdef __COM_CCC1__      
    }
#endif
#endif
    
	  EnableAllInterrupts();
#ifdef __COM_CCC1__
    if (temp == EE_COM_TRUE) 
    { 
#endif
      /* Reception notify */
      EE_com_notify_ok(msg_ROM, msg_RAM);
#ifdef __COM_CCC1__
    }
#endif
     
    /* Select next receiver */
    if (rx_from == EE_COM_MSG_EXT) 
      Message = msg_ROM->next_ipdu;
    else 
      Message = msg_ROM->next;
  } /*while*/
}
#endif
