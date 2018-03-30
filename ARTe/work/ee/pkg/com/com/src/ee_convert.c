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
 * CVS: $Id: ee_convert.c,v 1.2 2005/07/17 13:58:36 pj Exp $
 */

#include "com/com/inc/ee_comprv.h"
#include "com/byteorder.h"

#ifndef __PRIVATE_COM_CONVERT__
void EE_com_convert (EE_UINT16 property, void *data)
{
  switch (property & EE_MASK_MSG_BORD)
    { /* Data IN */
    case EE_COM_MSG_BE16TOCPU:  
      *(EE_UINT16 *)data = EE_COM_be16_to_cpu(*(EE_UINT16 *)data);
      break;

    case EE_COM_MSG_BE32TOCPU:  
      *(EE_UINT32 *)data = EE_COM_be32_to_cpu(*(EE_UINT32 *)data);
      break;

    case EE_COM_MSG_LE16TOCPU:
      *(EE_UINT16 *)data = EE_COM_le16_to_cpu(*(EE_UINT16 *)data);
      break;

    case EE_COM_MSG_LE32TOCPU:
      *(EE_UINT32 *)data = EE_COM_le32_to_cpu(*(EE_UINT32 *)data);
      break;

      /* Data OUT */
    case EE_COM_MSG_CPUTOBE16:
      *(EE_UINT16 *)data = EE_COM_cpu_to_be16(*(EE_UINT16 *)data);
      break;

    case EE_COM_MSG_CPUTOBE32:
      *(EE_UINT32 *)data = EE_COM_cpu_to_be32(*(EE_UINT32 *)data);
      break;

    case EE_COM_MSG_CPUTOLE16:
      *(EE_UINT16 *)data = EE_COM_cpu_to_le16(*(EE_UINT16 *)data);
      break;

    case EE_COM_MSG_CPUTOLE32:
      *(EE_UINT32 *)data = EE_COM_cpu_to_le32(*(EE_UINT32 *)data);
      break;
    }
} 
#endif
