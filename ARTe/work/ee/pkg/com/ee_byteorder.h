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
 * CVS: $Id: ee_byteorder.h,v 1.1.1.1 2004/11/05 16:03:03 pj Exp $
 */

#ifndef __EE_BYTEORDER_C__
#define __EE_BYTEORDER_C__

/* byte ordering conversion 
 * function name EE_COM_xx_to_yyzz where:
 *    xx is cpu, be(big endian), le(little endian)
 *    yy is cpu, be(big endian), le(little endian)
 *    zz is 16, 32
 *    
 * examples: 
 *
 * EE_COM_cpu_to_be16, EE_COM_cpu_to_be32,
 * EE_COM_cpu_to_le16, EE_COM_cpu_to_le32, EE_COM_le16_to_cpu,
 * EE_COM_le32_to_cpu, EE_COM_be16_to_cpu, EE_COM_be32_to_cpu
 */

#ifdef __LITTLE__
/* This micro has an internal little endian byte organization and this
 * means that a 32 byte unsigned integer like 0x12345678 is stored in
 * memory like 78 56 34 12
 */

#define EE_COM_cpu_to_be16(x) \
   (EE_UINT16)((((EE_UINT16)(x) & 0x00ff) << 8) |  \
                  (((EE_UINT16)(x) & 0xff00) >> 8))
              
#define EE_COM_cpu_to_be32(x) \
   (EE_UINT32)((((EE_UINT32)(x) & 0x000000ff) << 24) |   \
                  (((EE_UINT32)(x) & 0x0000ff00) << 8) |    \
                  (((EE_UINT32)(x) & 0x00ff0000) >> 8) |    \
                  (((EE_UINT32)(x) & 0xff000000) >> 24))
                 
#define EE_COM_cpu_to_le16(x) (EE_UINT16)x

#define EE_COM_cpu_to_le32(x) (EE_UINT32)x

#define EE_COM_le16_to_cpu(x) (EE_UINT16)x

#define EE_COM_le32_to_cpu(x) (EE_UINT32)x

#define EE_COM_be16_to_cpu(x) EE_COM_cpu_to_be16(x)

#define EE_COM_be32_to_cpu(x) EE_COM_cpu_to_be32(x)

/* To do even in big endian part
 */

#define EE_COM_be16_store(x,to)  \
   *(EE_UINT8 *)(to) = ((EE_UINT16)(x) >> 8);  \
   *(EE_UINT8 *)((to) + 1) = ((EE_UINT16)(x) & 0x00FF)

#define EE_COM_be16_get(from,x)  \
   x = *(EE_UINT8 *)(from) << 8 | *(EE_UINT8 *)((from)+1)
   
#define EE_COM_le16_store(x,to)  \
   *(EE_UINT8 *)to = (EE_UINT8)((EE_UINT16)(x) & 0x00FF);   \
   *(EE_UINT8 *)((to) + 1) = (EE_UINT8)((EE_UINT16)(x) >> 8)
   
#define EE_COM_le16_get(from,x)  \
   x = *(EE_UINT8 *)(from) | (*(EE_UINT8 *)((from) + 1) << 8)
   
#endif   /* Little endian */

#ifdef __BIG__
/* This micro has an internal little endian byte organization and this
 * means that a 32 byte unsigned integer like 0x12345678 is stored in
 * memory like 12 34 56 78
 */

#define EE_COM_cpu_to_le16(x) \
   (EE_UINT16)((((EE_UINT16)(x) & 0x00ff) << 8) |  \
                  (((EE_UINT16)(x) & 0xff00) >> 8))

#define EE_COM_cpu_to_le32(x) \
   (EE_UINT32)((((EE_UINT32)(x) & 0x000000ff) << 24) |   \
                  (((EE_UINT32)(x) & 0x0000ff00) << 8) |    \
                  (((EE_UINT32)(x) & 0x00ff0000) >> 8) |    \
                  (((EE_UINT32)(x) & 0xff000000) >> 24))
         
#define EE_COM_cpu_to_be16(x) (EE_UINT16)x

#define EE_COM_cpu_to_be32(x) (EE_UINT32)x

#define EE_COM_be16_to_cpu(x) (EE_UINT16)x

#define EE_COM_be32_to_cpu(x)	(EE_UINT32)x

#define EE_COM_le16_to_cpu(x)	EE_COM_cpu_to_le16(x)

#define EE_COM_le32_to_cpu(x)	EE_COM_cpu_to_le32(x)

#endif	/* Big endian */

#endif
