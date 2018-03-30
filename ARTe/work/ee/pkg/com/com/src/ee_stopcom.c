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
 * CVS: $Id: ee_stopcom.c,v 1.2 2005/07/17 13:58:36 pj Exp $
 */

#include "com/com/inc/ee_comprv.h"

/***********
 * STOPCOM *
 ********************************************
 * This call stops all COM activities.
 * 1. IPDUs are disabled 
 * 2. the timer is stopped
 ****************************************
 * IN      Mode: required shutdown mode (can be only COM_SHUTDOWN_IMMEDIATE) 
 * RETURN  return status value
 ************/

/* 2.9.2.2.2 */
#ifndef __PRIVATE_COM_STOPCOM__
StatusType EE_com_StopCOM(COMShutdownModeType Mode)
{
#ifdef __COM_HAS_ERRORHOOK__	
	register EE_FREG flags;
#endif
#ifdef __EE_COM_EXTENDED__
  if (Mode != COM_SHUTDOWN_IMMEDIATE) 
  {
      EE_com_sys2user.service_error = COMServiceId_StopCOM;
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
    return E_COM_ID;
  }
#endif         

#if defined(__COM_CCC0__) || defined(__COM_CCC1__)
  
  EE_alarm_cancel(EE_COM_ALARM);
  
  GetResource (EE_MUTEX_COM_IPDU);
  
  EE_com_sys.first_TM = EE_COM_NULL;
#ifdef __COM_CCC1__  
  EE_com_sys.first_DM = EE_COM_NULL; 
  EE_com_sys.first_PM = EE_COM_NULL;
#endif

  ReleaseResource(EE_MUTEX_COM_IPDU);
#endif
   
  return E_OK;
}
#endif
