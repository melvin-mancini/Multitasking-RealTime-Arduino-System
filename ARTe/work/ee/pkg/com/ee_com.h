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
 * CVS: $Id: ee_com.h,v 1.2 2005/07/16 13:19:58 pj Exp $
 */

#ifndef __EE_COM_H__
#define __EE_COM_H__

/* Definisco le strutture ed i parametri necessari per il passaggio di
   informazioni tra i livelli di rete */
#include "ee.h"
#include "ee_internal.h"

#define EE_COM_OK 0
#define EE_COM_ERROR 1
#define EE_COM_TRUE 1
#define EE_COM_FALSE 0
#define EE_COM_OVFL 4
#define EE_COM_EMPTY 5
#define EE_COM_ILLEGAL 6
#define EE_COM_NULL 255
#define EE_COM_NULL16 0xFFFF
#define EE_COM_BOOLEAN EE_UINT8
#define EE_COM_ENUM EE_UINT8

#include "com/ee_byteorder.h"
    
/****************************************/

#ifdef __ISO15765__

  #include "com/iso15765/iso15765.h"
  #include "alarms/alarms.h"
  
#endif  /*iso*/

/****************************************/

#ifdef __DLC_STANDARD__

  #include "com/dlc/canst10.h"
  #include "com/dlc/inline.c"

#endif  /* standard data link layer function */

/****************************************/

#ifdef __RT_DLC_STANDARD__

  #include "com/rtdlc/canst10.h"
  #include "com/rtdlc/inline.c"

#endif  /* standard data link layer function */

/*****************************************/

#ifdef __PSA_MASTER__

   #include "com/psa/master.h"
   #include "alarms/alarms.h"

#endif

#ifdef __PSA_SLAVE__

   #include "com/psa/slave.h"

#endif
   
/****************************************/

#if defined(__COM_CCCA__) || defined(__COM_CCCB__) || defined(__COM_CCC0__) || defined(__COM_CCC1__)

  #include "com/com/inc/ee_com.h"
  
  #if defined(__COM_CCC0__) || defined(__COM_CCC1__)
  #include "alarms/alarms.h"
  #endif
  
#endif

#endif

