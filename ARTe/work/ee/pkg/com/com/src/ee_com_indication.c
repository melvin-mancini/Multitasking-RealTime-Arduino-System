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
 * CVS: $Id: ee_com_indication.c,v 1.2 2005/07/17 13:58:36 pj Exp $
 */
 
#include "com/com/inc/ee_comprv.h"

void EE_com_indication (EE_UINT8 *data_id, 
      EE_UINT8 size, EE_UINT8 *data)
{  
  EE_UINT8 found, 
              j;
  MessageIdentifier i, name;
  const struct EE_com_ipdu_ROM_TYPE * IPDU_ROM;
   
  i=0; 
  found = EE_COM_FALSE;
  name = data_id[0] || (data_id[1] << 8);
  while (i<EE_COM_N_IPDU && found==EE_COM_FALSE) 
  { 
    /* Polling on ipdu */
    if (EE_com_ipdu_ROM[i]->name == name) 
      found = EE_COM_TRUE;
    else
      i++;
  }
      
  if (found == EE_COM_TRUE) 
  {
    IPDU_ROM = EE_com_ipdu_ROM[i];    
    
    GetResource (EE_MUTEX_COM_IPDU);
#ifdef __COM_CCC1__
    if ((EE_com_ipdu_RAM[i]->property & 
        (EE_MASK_IPDU_TXRX | EE_MASK_IPDU_DPM)) 
          == (EE_COM_IPDU_RX | EE_COM_IPDU_PER))
    {
      EE_com_remove_PM(i); 
      EE_com_insert_PM(i, IPDU_ROM->reload_PM);
    }
#endif
    ReleaseResource (EE_MUTEX_COM_IPDU);

    /* First of all, IPDU Message Callout */
    if ((IPDU_ROM->ipdu_callout != NULL) && 
          (IPDU_ROM->ipdu_callout() == EE_COM_FALSE)) 
    { /* No further elaboration allowed */         
      return;
    }
    
    for (j=0; j<size; j++) 
      IPDU_ROM->data[j]=data[j];
    
    EE_com_msg_reception(IPDU_ROM->first, IPDU_ROM->data, 
        (EE_UINT16)EE_COM_EXTERNAL);
  } /*if found*/
}          
