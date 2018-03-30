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
 * CVS: $Id: ee_sendzero.c,v 1.2 2005/07/17 13:58:36 pj Exp $
 */

#include "com/com/inc/ee_comprv.h"
   
/* 2.9.2.4.5 */
#ifdef __COM_CCC1__
#ifndef __PRIVATE_COM_SENDZEROMESSAGE__
StatusType EE_com_SendZeroMessage (MessageIdentifier Message)
{
  StatusType code;
#ifdef __COM_HAS_ERRORHOOK__  	
  register EE_FREG flags;
#endif
	
#ifdef EE_COM_EXTENDED
  if ((Message > MAX_MSG) || (Message == 0) || 
      (EE_COM_msg_ROM[Message] == NULL) || 
      (EE_COM_msg_ROM[Message]->size != 0)) 
  {
    EE_com_sys2user.service_error = COMServiceId_SendZeroMessage;
#ifdef __COM_HAS_ERRORHOOK__   
    flags = EE_hal_begin_nested_primitive(); 
      COMError_SendZeroMessage_Message = Message;
      
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

  GetResource(EE_MUTEX_COM_IPDU);
  
  code = EE_com_send(Message, NULL, 0);
  
  ReleaseResource(EE_MUTEX_COM_IPDU);
   
  if (code != E_OK)
  {
    EE_com_sys2user.service_error = COMServiceId_SendZeroMessage;
#ifdef __COM_HAS_ERRORHOOK__   
    flags = EE_hal_begin_nested_primitive(); 
      COMError_SendZeroMessage_Message = Message;      
      
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
  
  return code;
}  
#endif
#endif
