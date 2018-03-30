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
 * CVS: $Id: ee_filters.c,v 1.2 2005/07/17 13:58:36 pj Exp $
 */

#include "com/ee_com.h"

/*******************/
/* FILTER ROUTINES */
/*******************/

/********************/
/* F_ALWAYS routine */
/********************/

#ifndef __PRIVATE_F_ALWAYS__
EE_UINT8 F_Always(void *new_data, void *old_data, 
         EE_UINT8 size, void *extra_data)
{ 
  return EE_COM_TRUE;
}
#endif

#ifdef __COM_CCC1__

#ifndef __PRIVATE_F_NEVER__
EE_UINT8 F_Never(void *new_data, void *old_data, 
        EE_UINT8 size, void *extra_data)
{
  return EE_COM_FALSE;
}
#endif

#ifndef __PRIVATE_F_NEWISEQUAL__
EE_UINT8 F_NewIsEqual(void *new_data, void *old_data, 
       EE_UINT8 size, void *extra_data)
{ 
  switch (size) 
  {
    case 2: 
      if ((*(EE_UINT16 *)new_data) == (*(EE_UINT16 *)old_data)) 
        return EE_COM_TRUE;
      else
        return EE_COM_FALSE;
      break;
    
    case 4:
      if ((*(EE_UINT32 *)new_data) == (*(EE_UINT32 *)old_data)) 
        return EE_COM_TRUE;
      else
        return EE_COM_FALSE;     
      break;
  }
}
#endif

#ifndef __PRIVATE_F_NEWISDIFFERENT__
EE_UINT8 F_NewIsDifferent(void *new_data, void *old_data, 
           EE_UINT8 size, void *extra_data)
{ 
  switch (size) 
  {
    case 2: 
      if ((*(EE_UINT16 *)new_data) != (*(EE_UINT16 *)old_data)) 
        return EE_COM_TRUE;
      else
        return EE_COM_FALSE;
      break;
    
    case 4:
      if ((*(EE_UINT32 *)new_data) != (*(EE_UINT32 *)old_data)) 
        return EE_COM_TRUE;
      else
        return EE_COM_FALSE;     
      break;
  }
}
#endif

#ifndef __PRIVATE_F_MASKEDNEWEQUALSX__
EE_UINT8 F_MaskedNewEqualsX(void *new_data, void *old_data, 
             EE_UINT8 size, void *extra_data)
{   
  switch (size) {
  case 2:
    if (((*(EE_UINT16 *)new_data) & ((EE_UINT16 *)extra_data)[0]) == 
  ((EE_UINT16 *)extra_data)[1])
      return EE_COM_TRUE;
    else 
      return EE_COM_FALSE;
    break;
    
  case 4:
    if (((*(EE_UINT32 *)new_data) & ((EE_UINT32 *)extra_data)[0]) == 
  ((EE_UINT32 *)extra_data)[1])
      return EE_COM_TRUE;
    else 
      return EE_COM_FALSE;
    break;
  }
}
#endif

#ifndef __PRIVATE_F_MASKEDNEWDIFFERSX__
EE_UINT8 F_MaskedNewDiffersX(void *new_data, void *old_data, 
        EE_UINT8 size, void *extra_data)
{ 
  switch (size) {
  case 2:
    if (((*(EE_UINT16 *)new_data) & ((EE_UINT16 *)extra_data)[0]) != 
  ((EE_UINT16 *)extra_data)[1])
      return EE_COM_TRUE;
    else 
      return EE_COM_FALSE;
    break;
    
  case 4:
    if (((*(EE_UINT32 *)new_data) & ((EE_UINT32 *)extra_data)[0]) != 
  ((EE_UINT32 *)extra_data)[1])
      return EE_COM_TRUE;
    else 
      return EE_COM_FALSE;
    break;
  }
}
#endif

#ifndef __PRIVATE_F_MASKEDNEWEQUALSMASKEDOLD__
EE_UINT8 F_MaskedNewEqualsMaskedOld(void *new_data, void *old_data, 
               EE_UINT8 size, void *extra_data)
{ 
  switch (size) {
  case 2:
    if (((*(EE_UINT16 *)new_data) & (*(EE_UINT16 *)extra_data)) ==
  ((*(EE_UINT16 *)old_data) & (*(EE_UINT16 *)extra_data))) 
      return EE_COM_TRUE;
    else 
      return EE_COM_FALSE;
    break;
    
  case 4:
    if (((*(EE_UINT32 *)new_data) & (*(EE_UINT32 *)extra_data)) ==
  ((*(EE_UINT32 *)old_data) & (*(EE_UINT32 *)extra_data)))
      return EE_COM_TRUE;
    else 
      return EE_COM_FALSE;
    break;
  }
}
#endif

#ifndef __PRIVATE_F_MASKEDNEWDIFFERSMASKEDOLD__
EE_UINT8 F_MaskedNewDiffersMaskedOld(void *new_data, void *old_data, 
          EE_UINT8 size, void *extra_data)
{ 
  switch (size) {
  case 2:
    if (((*(EE_UINT16 *)new_data) & (*(EE_UINT16 *)extra_data)) !=
  ((*(EE_UINT16 *)old_data) & (*(EE_UINT16 *)extra_data)))
      return EE_COM_TRUE;
    else 
      return EE_COM_FALSE;
    break;
    
  case 4:
    if ((*(EE_UINT32 *)new_data & *(EE_UINT32 *)extra_data) !=
  (*(EE_UINT32 *)old_data & *(EE_UINT32 *)extra_data))
      return EE_COM_TRUE;
    else 
      return EE_COM_FALSE;
    break;
  }
}
#endif

#ifndef __PRIVATE_F_NEWISWHITHIN__
EE_UINT8 F_NewIsWithin(void *new_data, void *old_data, 
        EE_UINT8 size, void *extra_data)
{ 
  switch (size) {
  case 2:
    if (((*(EE_UINT16 *)new_data) >= ((EE_UINT16 *)extra_data)[0]) && 
  ((*(EE_UINT16 *)new_data) <= ((EE_UINT16 *)extra_data)[1]))
      return EE_COM_TRUE;
    else
      return EE_COM_FALSE;
    break;
      
  case 4:
    if (((*(EE_UINT32 *)new_data) >= ((EE_UINT32 *)extra_data)[0]) && 
    ((*(EE_UINT32 *)new_data) <= ((EE_UINT32 *)extra_data)[1]))
      return EE_COM_TRUE;
    else
      return EE_COM_FALSE;
    break;
  }
}
#endif

#ifndef __PRIVATE_F_NEWISOUTSIDE__
EE_UINT8 F_NewIsOutside(void *new_data, void *old_data, 
         EE_UINT8 size, void *extra_data)
{ 
  switch (size) {
  case 2:
    if (((*(EE_UINT16 *)new_data) <= ((EE_UINT16 *)extra_data)[0]) ||
  ((*(EE_UINT16 *)new_data) >= ((EE_UINT16 *)extra_data)[1]))
      return EE_COM_TRUE;
    else
      return EE_COM_FALSE;
    break;
    
  case 4:
    if (((*(EE_UINT32 *)new_data) <= ((EE_UINT32 *)extra_data)[0]) ||
  ((*(EE_UINT32 *)new_data) >= ((EE_UINT32 *)extra_data)[1]))
  return EE_COM_TRUE;
    else
      return EE_COM_FALSE;
    break;
  }
}
#endif

#ifndef __PRIVATE_F_NEWISGREATER__
EE_UINT8 F_NewIsGreater(void *new_data, void *old_data, 
         EE_UINT8 size, void *extra_data)
{ 
  switch (size) {
  case 2: 
    if ((*(EE_UINT16 *)new_data) > (*(EE_UINT16 *)old_data)) 
      return EE_COM_TRUE;
    else
      return EE_COM_FALSE;
    break;
    
  case 4:
    if ((*(EE_UINT32 *)new_data) > (*(EE_UINT32 *)old_data)) 
      return EE_COM_TRUE;
      else
  return EE_COM_FALSE;     
    break;
  }
}
#endif

#ifndef __PRIVATE_F_NEWISLESSOREQUAL__
EE_UINT8 F_NewIsLessOrEqual(void *new_data, void *old_data, 
             EE_UINT8 size, void *extra_data)
{ 
  switch (size) {
  case 2: 
    if ((*(EE_UINT16 *)new_data) <= (*(EE_UINT16 *)old_data)) 
      return EE_COM_TRUE;
    else
      return EE_COM_FALSE;
    break;
    
  case 4:
    if ((*(EE_UINT32 *)new_data) <= (*(EE_UINT32 *)old_data)) 
  return EE_COM_TRUE;
    else
      return EE_COM_FALSE;     
    break;
  }
}
#endif

#ifndef __PRIVATE_F_NEWISLESS__
EE_UINT8 F_NewIsLess(void *new_data, void *old_data, 
      EE_UINT8 size, void *extra_data)
{ 
  switch (size) {
  case 2: 
    if ((*(EE_UINT16 *)new_data) < (*(EE_UINT16 *)old_data)) 
      return EE_COM_TRUE;
    else
      return EE_COM_FALSE;
    break;
    
  case 4:
    if ((*(EE_UINT32 *)new_data) < (*(EE_UINT32 *)old_data)) 
  return EE_COM_TRUE;
    else
      return EE_COM_FALSE;     
    break;
  }
}
#endif

#ifndef __PRIVATE_F_NEWISGREATEROREQUAL__
EE_UINT8 F_NewIsGreaterOrEqual(void *new_data, void *old_data, 
          EE_UINT8 size, void *extra_data)
{ 
  switch (size) {
  case 2: 
    if ((*(EE_UINT16 *)new_data) >= (*(EE_UINT16 *)old_data)) 
      return EE_COM_TRUE;
    else
      return EE_COM_FALSE;
    break;
      
  case 4:
    if ((*(EE_UINT32 *)new_data) >= (*(EE_UINT32 *)old_data)) 
      return EE_COM_TRUE;
    else
      return EE_COM_FALSE;     
    break;
  }
}
#endif

#ifndef __PRIVATE_F_ONEEVERYN__
EE_UINT8 F_OneEveryN(void *new_data, void *old_data, 
      EE_UINT8 size, void *extra_data)
{ 
  ((EE_UINT16 *)extra_data)[2]++;
    
  switch (size) {
  case 2: 
    if ((((EE_UINT16 *)extra_data)[2] % ((EE_UINT16 *)extra_data)[0]) 
          == ((EE_UINT16 *)extra_data)[1])
      return EE_COM_TRUE;
    else
      return EE_COM_FALSE;
    break;
    
  case 4: 
    if ((((EE_UINT32 *)extra_data)[2] % ((EE_UINT32 *)extra_data)[0]) == 
  ((EE_UINT32 *)extra_data)[1])
      return EE_COM_TRUE;
    else
      return EE_COM_FALSE;
    break;
  }
}
#endif

#endif
