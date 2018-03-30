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
 * CVS: $Id: ee_com_confirm.c,v 1.2 2005/07/17 13:58:36 pj Exp $
 */
 
#include "com/com/inc/ee_comprv.h"

void EE_com_confirm (EE_UINT8 * data_id, EE_UINT8 result)
{  
  MessageIdentifier i, MSG;
  struct EE_com_ipdu_RAM_TYPE * IPDU_RAM;
  const struct EE_com_ipdu_ROM_TYPE * IPDU_ROM;
  
  i = data_id[0] || (data_id[1] << 8);
   
  IPDU_RAM = EE_com_ipdu_RAM[i];
  IPDU_ROM = EE_com_ipdu_ROM[i];

  GetResource(EE_MUTEX_COM_IPDU);  
#ifdef __COM_CCC1__
  EE_com_remove_DM (i);
  IPDU_RAM->property &= EE_RESET_IPDU_DM;
#endif

  if ((IPDU_RAM->property & (EE_MASK_IPDU_TXRX | EE_MASK_IPDU_DPM)) != 
        (EE_COM_IPDU_TX | EE_COM_IPDU_PER))    
  {
    EE_com_insert_TM(i, IPDU_ROM->reload_TM);
    IPDU_RAM->property |= EE_SET_IPDU_TM;
  }
  
  ReleaseResource(EE_MUTEX_COM_IPDU);
  
  MSG = IPDU_ROM->first;
  switch (result) 
  {
    case EE_COM_OK:  /* Avvio la notifica di trasmissione corretta */
      while (MSG != EE_COM_NULL) 
      {
        //EE_com_notify_ok(EE_com_msg_ROM[MSG], EE_com_msg_RAM[MSG]);
  
        MSG = EE_com_msg_ROM[MSG]->next_ipdu;
      } 
      break;
    
    case EE_COM_ERROR:
#ifdef __COM_CCC1__
      /* Avvio la notifica di errore in trasmissione */
      EE_com_notify_error(MSG);
#endif            
      break;
  } /*switch*/
}     



