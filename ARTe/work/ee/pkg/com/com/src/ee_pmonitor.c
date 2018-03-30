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
 * CVS: $Id: ee_pmonitor.c,v 1.2 2005/07/17 13:58:36 pj Exp $
 */
 
#include "com/com7inc/ee_comprv.h"
#include "ee.h"

/*********************
 * This function is used to monitor the periodic transmission 
 * (only periodic and mixed ipdu) (see par. 2.3).
 * All this is made in 2 steps
 *  1. For periodic ipdu a new transmission request is made and 
 *     for mixed ipdu, a new transmission request is made only if
 *     DM and TM flag are clear (ipdu ready to transmit)
 **
 * WARNING:
 *  An exiting ipdu could be inserted newly in DM queue, 
 *  this means that I can't do a new monitor_handler till 
 *  the end of this function. If I can't guarantee that all 
 *  execution time of this function stay within a tick alarm,
 *  all the deadline monitor could fall in some trouble!
 */
#ifndef __PRIVATE_COM_PERIOD_MONITOR__
void EE_com_period_monitor (void) 
{
  register struct EE_com_ipdu_RAM_TYPE * ipdu_RAM;
  register const struct EE_com_ipdu_ROM_TYPE * ipdu_ROM;
  MessageIdentifier ipdu_name, ipdu_old_first;
  
  ipdu_name = ipdu_old_first = EE_com_sys.first_PM;
  
  do
    ipdu_name = EE_com_ipdu_RAM[ipdu_name]->next_PM;
  while ((ipdu_name != EE_COM_NULL) && 
         (EE_com_ipdu_RAM[ipdu_name]->cont_DM == 0));
      
  EE_com_sys.first_PM = ipdu_name;  
  ipdu_name = ipdu_old_first;
  
  do
  {
    ipdu_RAM = EE_com_ipdu_RAM[ipdu_name];
    ipdu_ROM = EE_com_ipdu_ROM[ipdu_name];
    ipdu_name = ipdu_RAM->next_PM;        
    
    if ((ipdu_RAM->property & (EE_MASK_IPDU_TXRX | EE_MASK_IPDU_DPM)) ==
          (EE_COM_IPDU_TX | EE_COM_IPDU_PER))
    { /* Here the lower layer send command for the ipdu */
      #ifdef __COM_CCC1__
      if ((ipdu_RAM->property & EE_MASK_IPDU_DM) ==
            EE_COM_IPDU_DM_CLEAR)
      { 
        EE_com_insert_DM (ipdu_ROM->name, ipdu_ROM->reload_DM);
        ipdu_RAM->property |= EE_SET_IPDU_DM;        
      }
      #endif
    }
    else
      /* it's a mixed ipdu */
      if ((ipdu_RAM->property & EE_MASK_IPDU_DMTM) ==
            EE_COM_IPDU_DMTM_CLEAR)
      { /* Here the lower layer send command for the ipdu */
        #ifdef __COM_CCC1__
        EE_com_insert_DM (ipdu_ROM->name, ipdu_ROM->reload_DM);
        ipdu_RAM->property |= EE_SET_IPDU_DM;
        #endif
      }
      else
        ipdu_RAM->property |= EE_SET_IPDU_PEND;
        
    EE_com_insert_PM (ipdu_ROM->name, ipdu_ROM->reload_PM);
  } while ((ipdu_name != EE_COM_NULL) &&
           (EE_com_ipdu_RAM[ipdu_name]->cont_PM != 0));
}
#endif
