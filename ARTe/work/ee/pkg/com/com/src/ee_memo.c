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
 * CVS: $Id: ee_memo.c,v 1.2 2005/07/17 13:58:36 pj Exp $
 */

#include "com/ee_com.h"

/************************************************************************
 * MEMO                                                                 *
 ************************************************************************
 * Take all bits from source (starting from bit s_from) and store it to
 * destination (starting from bit d_from). All this was made by length 
 * consecutive bits.
 ************************************************************************
 * IN 
 *    source: Reference to source data
 *    s_from: starting source bit
 *    dest: Reference to destination area
 *    d_from: starting destination bit
 *    length: How many bit must be transferred
 ************************************************************************/
#ifndef __PRIVATE_COM_MEMO__
void EE_com_memo(EE_UINT8 *source, EE_UINT8 s_from, 
      EE_UINT8 *dest, EE_UINT8 d_from, 
      EE_UINT8 length) 
{
  register EE_UINT8 data_bit, temp_bit;
  EE_UINT8 i, temp, a, b;  

  if (((s_from | d_from | length) & 0x07) == 0) { 
    /* Posso eseguire un trasferimento a byte */
   
    a = s_from >> 3;
    b = a + (length >> 3);
    temp = d_from >> 3;

    for (i=a; i < b; i++) {
      dest[temp] = source[i];
      temp++;
    }      
  }    
  else {
    temp = d_from;
    a = s_from;
    b = s_from + length;
      
    for (i=a; i < b; i++) {
      // bits of interest are isolated 
      data_bit = (source[i>>3] >> (i & 7)) & 0x01;
      temp_bit = 1 << (temp & 7);
   
      if (data_bit == 0) {
  temp_bit = ~temp_bit;
  dest[temp>>3] &= temp_bit;
      } 
      else
  dest[temp>>3] |= temp_bit;
               
      temp++;
    }
  }
}
#endif
