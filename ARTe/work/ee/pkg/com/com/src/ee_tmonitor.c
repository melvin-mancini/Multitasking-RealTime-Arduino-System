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
 * CVS: $Id: ee_tmonitor.c,v 1.2 2005/07/17 13:58:36 pj Exp $
 */
 
#include "com/com/inc/ee_comprv.h"

/*********************
 * This function is used to monitor the minimum intertransmission 
 * time (so, only direct and mixed ipdu) (see par. 2.3).
 * Every call to this function means that an ipdu is ready to transmit
 * All is made by 1 step
 *  1. Set ipdu to ready and, if there is any pending request, 
 *     it make a transmission to lower layer (), setting new 
 *     deadline monitor alarm
 **
 * WARNING:
 *  An exiting ipdu could be inserted newly in some queue, 
 *  but I can't have more than one pending handler execution.
 *  If I can't guarantee that all previous execution time of 
 *  handler stay within a tick alarm, I could fall in some trouble!
 */
#ifndef __PRIVATE_COM_TIME_MONITOR__
void EE_com_time_monitor (void) 
{
  struct EE_com_ipdu_RAM_TYPE * ipdu_RAM;
  const struct EE_com_ipdu_ROM_TYPE * ipdu_ROM;
  MessageIdentifier ipdu_name, ipdu_old_first;
  
  ipdu_name = ipdu_old_first = EE_com_sys.first_TM;
  
  do
    ipdu_name = EE_com_ipdu_RAM[ipdu_name]->next_TM;
  while ((ipdu_name != EE_COM_NULL) && 
         (EE_com_ipdu_RAM[ipdu_name]->cont_TM == 0));
      
  /* If I have to reinsert the ipdu, the queue head must be consistent
   */       
  EE_com_sys.first_TM = ipdu_name;  
  //ipdu_name = ipdu_old_first;
  
  do
  {
    ipdu_RAM = EE_com_ipdu_RAM[ipdu_name];
    ipdu_ROM = EE_com_ipdu_ROM[ipdu_name];
    ipdu_name = ipdu_RAM->next_TM;        
    
    ipdu_RAM->property &= EE_RESET_IPDU_TM;
    if ((ipdu_RAM->property & EE_MASK_IPDU_PEND) ==
          EE_COM_IPDU_PEND)
    { /* Here the lower layer send command for the ipdu */
      #ifdef __COM_CCC1__
      EE_com_insert_DM (ipdu_ROM->name, ipdu_ROM->reload_DM);
      ipdu_RAM->property |= EE_SET_IPDU_DM;
      #endif
    }
  } while ((ipdu_name != EE_COM_NULL) &&
           (EE_com_ipdu_RAM[ipdu_name]->cont_TM != 0));
}       
#endif      
        
        
          
  
  
