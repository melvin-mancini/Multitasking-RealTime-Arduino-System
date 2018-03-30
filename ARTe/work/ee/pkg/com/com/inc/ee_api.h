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
 * Author: Paolo Gai
 * CVS: $Id: ee_api.h,v 1.2 2005/07/17 13:58:36 pj Exp $
 */


#ifndef __INCLUDE_KERNEL_COM_EE_API__
#define __INCLUDE_KERNEL_COM_EE_API__

#if defined(__COM_CCCA__) || defined(__COM_CCCB__) || defined(__COM_CCC0__) || defined(__COM_CCC1__)

#ifndef StartCOM
#define StartCOM                 EE_com_StartCOM
#endif

#ifndef StopCOM
#define StopCOM                  EE_com_StopCOM
#endif

#ifndef GetCOMApplicationMode
#define GetCOMApplicationMode    EE_com_GetCOMApplicationMode
#endif

#ifndef InitMessage
#define InitMessage              EE_com_InitMessage
#endif

#ifndef StartPeriodic
#define StartPeriodic            EE_com_StartPeriodic
#endif

#ifndef StopPeriodic
#define StopPeriodic             EE_com_StopPeriodic
#endif

#ifndef SendMessage
#define SendMessage              EE_com_SendMessage
#endif

#ifndef ReceiveMessage
#define ReceiveMessage           EE_com_ReceiveMessage
#endif

#ifndef SendDynamicMessage
#define SendDynamicMessage       EE_com_SendDynamicMessage
#endif

#ifndef ReceiveDynamicMessage
#define ReceiveDynamicMessage    EE_com_ReceiveDynamicMessage
#endif

#ifndef SendZeroMessage
#define SendZeroMessage          EE_com_SendZeroMessage
#endif

#if defined(__COM_CCCB__) || defined(__COM_CCC1__)
#ifndef GetMessageStatus
#define GetMessageStatus         EE_com_GetMessageStatus
#endif
#endif

#ifndef COMErrorGetServiceId
#define COMErrorGetServiceId     EE_com_COMErrorGetServiceId
#endif

#endif

#ifdef __COM_HAS_ERRORHOOK__

/* COMErrorHook MACRO
 * Used in COMErrorHook to access the parameters of the calling function
 */
#define COMError_StartCOM_Mode()          EE_com_ErrorHook.proc_param.Mode
#define COMError_StopCOM_Mode()           EE_com_ErrorHook.proc_param.Mode
#define COMError_InitMessage_Message()    EE_com_ErrorHook.proc_param.OtherCOM.Message
#define COMError_InitMessage_DataRef()    EE_com_ErrorHook.proc_param.OtherCOM.DataRef
#define COMError_SendMessage_Message()    EE_com_ErrorHook.proc_param.OtherCOM.Message
#define COMError_SendMessage_DataRef()    EE_com_ErrorHook.proc_param.OtherCOM.DataRef
#define COMError_ReceiveMessage_Message()    EE_com_ErrorHook.proc_param.OtherCOM.Message
#define COMError_ReceiveMessage_DataRef()    EE_com_ErrorHook.proc_param.OtherCOM.DataRef
#define COMError_SendDynamicMessage_Message()    EE_com_ErrorHook.proc_param.OtherCOM.Message
#define COMError_SendDynamicMessage_DataRef()    EE_com_ErrorHook.proc_param.OtherCOM.DataRef
#define COMError_SendDynamicMessage_LengthRef()  EE_com_ErrorHook.proc_param.OtherCOM.MsgLengthRef
#define COMError_ReceiveDynamicMessage_Message()    EE_com_ErrorHook.proc_param.OtherCOM.Message
#define COMError_ReceiveDynamicMessage_DataRef()    EE_com_ErrorHook.proc_param.OtherCOM.DataRef
#define COMError_ReceiveDynamicMessage_LengthRef()  EE_com_ErrorHook.proc_param.OtherCOM.MsgLengthRef
#define COMError_SendZeroMessage_Message()    EE_com_ErrorHook.proc_param.OtherCOM.Message
#define COMError_GetMessageStatus_Message()    EE_com_ErrorHook.proc_param.OtherCOM.Message

#endif


#define COM_TRUE EE_COM_TRUE 
#define COM_FALSE EE_COM_FALSE 

#endif
