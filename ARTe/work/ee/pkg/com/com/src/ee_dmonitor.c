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
 * CVS: $Id: ee_dmonitor.c,v 1.2 2005/07/17 13:58:36 pj Exp $
 */
 
#include "com/com/inc/ee_comprv.h"
#include "ee.h"

/*********************
 * This function is used to monitor the deadline (see par. 2.5).
 * Every call to this function means a timeout
 * This are the steps made by this function:
 *  1. Notify the event
 *  2. Set the ipdu sendable (right word?)
 *  3. Check if there is a pending request
 **
 * WARNING:
 *  An exiting ipdu could be inserted newly in DM queue, 
 *  this means that I can't do a new monitor_handler till 
 *  the end of this function. If I can't guarantee that all 
 *  execution time of this function stay within a tick alarm,
 *  all the deadline monitor could fall in some trouble!
 */
#ifndef __PRIVATE_COM_DEADLINE_MONITOR__
void EE_com_deadline_monitor (void) 
{
  register struct EE_com_ipdu_RAM_TYPE *ipdu_RAM;
  register const struct EE_com_ipdu_ROM_TYPE * ipdu_ROM;
  MessageIdentifier ipdu_name, ipdu_old_first;
  
  /* Set new DM head */
  ipdu_name = ipdu_old_first = EE_com_sys.first_DM;
  
  do
    ipdu_name = EE_com_ipdu_RAM[ipdu_name]->next_DM;
  while ((ipdu_name != EE_COM_NULL) && 
         (EE_com_ipdu_RAM[ipdu_name]->cont_DM == 0));
    
  EE_com_sys.first_DM = ipdu_name;  
  /* First three steps */
  ipdu_name = ipdu_old_first;
  
  do
  {
    ipdu_RAM = EE_com_ipdu_RAM[ipdu_name];
    ipdu_ROM = EE_com_ipdu_ROM[ipdu_name];
    ipdu_name = ipdu_RAM->next_DM;
  
    EE_com_notify_error(ipdu_ROM->first);
    
    if ((ipdu_RAM->property & EE_MASK_IPDU_TXRX) == 
          EE_COM_IPDU_RX)
      /* Deadline monitor must be restarted immediatly 
       * on periodic reception (ipdu par. 2.5.1) */
      EE_com_insert_DM (ipdu_ROM->name, ipdu_ROM->reload_DM);
    else 
      if (((ipdu_RAM->property & EE_MASK_IPDU_TXRX) ==   
            EE_COM_IPDU_TX)  && 
          ((ipdu_RAM->property & EE_MASK_IPDU_DPM) == 
            EE_COM_IPDU_DIR) || 
          ((ipdu_RAM->property & EE_MASK_IPDU_DPM) ==   
            EE_COM_IPDU_MIX))
        /* This is a direct/mixed transmission ipdu */
        if ((ipdu_RAM->property & EE_MASK_IPDU_PEND) == 
              EE_COM_IPDU_PEND)
        { /* If there is a pending transmission request 
           * I have to send new data */
           
          /* Lower layer send command for the ipdu */
          EE_com_insert_DM (ipdu_ROM->name, ipdu_ROM->reload_DM);
        }
        else
          ipdu_RAM->property &= EE_RESET_IPDU_DMTM;              
  } while ((ipdu_name != EE_COM_NULL) &&
           (EE_com_ipdu_RAM[ipdu_name]->cont_DM != 0));
}  
#endif
        
