/** @file	ee_mcu_common_e2p.h
 *
 *  @brief	Erika Enterprise - E2P Driver - E2P Common Header File.
 *
 *  <!--
 *    ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 *    Copyright (C) 2002-2013  Evidence Srl
 *
 *    This file is part of ERIKA Enterprise.
 *
 *    ERIKA Enterprise is free software; you can redistribute it
 *    and/or modify it under the terms of the GNU General Public License
 *    version 2 as published by the Free Software Foundation, 
 *    (with a special exception described below).
 *
 *    Linking this code statically or dynamically with other modules is
 *    making a combined work based on this code.  Thus, the terms and
 *    conditions of the GNU General Public License cover the whole
 *    combination.
 *
 *    As a special exception, the copyright holders of this library give you
 *    permission to link this code with independent modules to produce an
 *    executable, regardless of the license terms of these independent
 *    modules, and to copy and distribute the resulting executable under
 *    terms of your choice, provided that you also meet, for each linked
 *    independent module, the terms and conditions of the license of that
 *    module.  An independent module is a module which is not derived from
 *    or based on this library.  If you modify this code, you may extend
 *    this exception to your version of the code, but you are not
 *    obligated to do so.  If you do not wish to do so, delete this
 *    exception statement from your version.
 *
 *    ERIKA Enterprise is distributed in the hope that it will be
 *    useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 *    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License version 2 for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    version 2 along with ERIKA Enterprise; if not, write to the
 *    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *    Boston, MA 02110-1301 USA.
 *  -->
 *
 *  The E2P driver provides services for reading, writing, erasing to/from an
 *  EEPROM. It also provides a service for comparing a data block in the EEPROM
 *  with a data block in the memory (e.g. RAM).
 *
 *  The behaviour of those services can be synchronous or asynchronous.
 *
 *  There are two classes of E2P drivers: 
 *  - EEPROM drivers for internal EEPROM.
 *  - EEPROM drivers for external EEPROM devices.
 *
 *  A driver for an internal EEPROM accesses the microcontroller hardware
 *  directly and is located in the Microcontroller Abstraction Layer.
 *
 *  A driver for an external EEPROM uses handlers (SPI in most cases) or drivers
 *  to access the external EEPROM device.
 *  It is located in the ECU Abstraction Layer. 
 *
 *  @note	The source code of external EEPROM drivers shall be independent
 *  		of the microcontroller platform.
 *
 *  The functional requirements and the functional scope are the same for both
 *  types of drivers. Hence the API is semantically identical.
 *
 *  The internal EEPROM may depend on the system clock, prescaler(s) and PLL.
 *  Thus, changes of the system clock (e.g. PLL on -> PLL off) may also affect
 *  the clock settings of the EEPROM hardware. Module E2P Driver do not take
 *  care of setting the registers which configure the clock, prescaler(s) and
 *  PLL in its init function. This has to be done by the MCU module.
 *
 *  A driver for an external EEPROM depends on the API and capabilities of the
 *  used onboard communication handler (e.g. SPI Handler/Driver).
 *
 *  The E2P driver does not provide mechanisms for providing data integrity
 *  (e.g. checksums, redundant storage, etc.).
 *
 *  The setting of the EEPROM write protection is not provided.
 *
 *  The E2P module shall not buffer jobs. The Eep module shall accept only one
 *  job at a time. During job processing, the Eep module shall accept no other
 *  job.
 *
 *  @note	When running in production mode it isassumed that the E2p user
 *  		will never issue jobs concurrently; therefore error handling for
 *		this requirement is restricted to development.
 *
 *  The E2P module shall not buffer data to be read or written. The E2P module
 *  shall use application data buffers thatare referenced by a pointer passed
 *  via the API.
 *
 *  The E2P module shall allow to be configured for interrupt or polling
 *  controlled job processing (if this is supported by the EEPROM hardware)
 *  through the OIL configuration.
 *
 *  If interrupt controlled job processing is supported and enabled, the
 *  external interrupt service routine shall call an additional job processing
 *  function.
 *
 *  Hint:	The function <tt>EE_E2p_MainFunction()</tt> is still required
 *  		for processing of jobs without hardware interrupt support (e.g.
 *  		for read and compare jobs) and for timeout supervision.
 *
 *  @author	Riccardo Schiavi
 *  @author	Giuseppe Serano
 *  @version	0.1
 *  @date	2013
 */

#ifndef	__EE_MCU_COMMON_E2P_H__
#define	__EE_MCU_COMMON_E2P_H__

/** @brief	E2P Status Type
 *
 *  The type <tt>EE_TYPEE2PSTAT</tt> is the type of the return value of the
 *  function <tt>EE_E2p_GetStatus()</tt>.
 *
 *  The type of <tt>EE_TYPEE2PSTAT</tt> is an enumeration describing the status
 *  of the related EEPROM and last job performed..
 */
typedef enum {
  EE_E2P_UNINIT,	/**< "Un-Initialized" State.			      */
  EE_E2P_OPERATIONAL,	/**< Normal operation; the related EEPROM is ready to
  			  *  perform jobs. (e.g. after initialization) 	      */
  EE_E2P_BUSY,		/**< Ongoing EEPROM job.			      */
  EE_E2P_ERROR,		/**< Erroneous EEPROM job.			      */
  EE_E2P_CANCELED	/**< Canceled EEPROM job.			      */
} EE_TYPEE2PSTAT;

/*
 * Return Values: Extended Status Only.
 */
#ifdef	__E2P_EXTENDED_STATUS__

/** @brief	Invalid E2P De-Initialization Flags. */
#define	E_E2P_PARAM_FLAGS	0xFE

/** @brief	Invalid Address. */
#define	E_E2P_PARAM_ADDRESS	0xFD

/** @brief	Invalid Size. */
#define	E_E2P_PARAM_SIZE	0xFC

/** @brief	Driver Busy. */
#define	E_E2P_BUSY		0xFB

/** @brief	Pointer is NULL. */
#define	E_E2P_PARAM_NULL	0xFA

#endif	/* __E2P_EXTENDED_STATUS__ */

/** @brief	E2P Configuration Flags.
 *
 *  Type for setting-up a E2P Driver Configuration.
 *
 *  Implementation specific. But not all values may be valid within this type.
 *
 *  This type shall be chosen in order to have the most efficient implementation
 *  on a specific micro controller platform.
 *
 *  For parameter values of type <tt>EE_TYPEE2PFLAGS</tt>, the E2p's user shall
 *  use the symbolic names provided by the driver implementation.
 *
 *  The type shall be EE_UREG depending on uC platform.
 * 
 */
#ifndef	EE_TYPEE2PFLAGS
#define	EE_TYPEE2PFLAGS	EE_UREG
#endif

/** @brief	E2P Driver Default Configuration Flag. */
#define	EE_E2P_FLAG_DEFAULT	0x00U

/** @brief	E2P Write Cycle Reduction Flag.
 *
 *  Switches to activate or deactivate write cycle reduction:
 *  EEPROM value is read and compared before being overwritten.
 */
#define	EE_E2P_FLAG_WCR		0x01U

/** @brief	E2P Address Type.
 *
 *  Used as address offset from the configured EEPROM base address to access a 
 *  certain EEPROM memory area.
 *
 *  This type shall have 0 as lower limit for each EEPROM device.
 *
 *  Implementation specific. But not all values may be valid within this type.
 *
 *  This type shall be chosen in order to have the most efficient implementation
 *  on a specific micro controller platform.
 *
 *  Type is <tt>EE_UINT8</tt>, <tt>EE_UINT16</tt>, <tt>EE_UINT32</tt> and Range
 *  is 8 to 32 bit.
 *
 *  It shall be provided for external use.
 */
#ifndef	EE_TYPEE2PADDR
#define	EE_TYPEE2PADDR	EE_UINT16
#endif

/** @brief	E2P Data Type.
 *
 *  Implementation specific. But not all values may be valid within this type.
 *
 *  This type shall be chosen in order to have the most efficient implementation
 *  on a specific micro controller platform.
 *
 *  Type is <tt>EE_UINT8</tt>, <tt>EE_UINT16</tt>, <tt>EE_UINT32</tt> and Range
 *  is 8 to 32 bit.
 *
 *  It shall be provided for external use.
 */
#ifndef	EE_TYPEE2PDATA
#define	EE_TYPEE2PDATA	EE_UINT8
#endif

/** @brief	E2P Size.
 *
 *  Implementation specific. But not all values may be valid within this type.
 *
 *  This type shall be chosen in order to have the most efficient implementation
 *  on a specific micro controller platform.
 *
 *  Type is <tt>EE_UINT8</tt>, <tt>EE_UINT16</tt>, <tt>EE_UINT32</tt> and Range
 *  is 8 to 32 bit.
 *
 *  It shall be provided for external use.
 */
#ifndef	EE_TYPEE2PSIZE
#define	EE_TYPEE2PSIZE	EE_UINT16
#endif

#ifdef	__E2P_GETSTATUS_API__
/** @brief	EEPROM and job Status Retrieval.
 *  @return	
 *  - <tt>EE_E2P_UNINIT</tt>:		"Un-Initialized" State.
 *  - <tt>EE_E2P_OPERATIONAL</tt>:	Normal operation; the related EEPROM is
 *  					ready to perform jobs.
 *  					(e.g. after initialization)
 *  - <tt>EE_E2P_BUSY</tt>:		Ongoing EEPROM job.
 *  - <tt>EE_E2P_ERROR</tt>:		Erroneous EEPROM job.
 *  - <tt>EE_E2P_CANCELED:		Canceled EEPROM job.
 *
 *  The service gets the status of the related EEPROM and last job performed.
 *
 *  The function <tt>EE_E2p_GetStatus()</tt> shall return the EEPROM and last
 *  job status synchronously.
 *
 *  The return states <tt>EE_E2P_BUSY</tt>, <tt>EE_E2P_ERROR</tt> and
 *  <tt>EE_E2P_CANCELED</tt> are substates of the state
 *  <tt>EE_E2P_OPERATIONAL</tt>.
 */
EE_TYPEE2PSTAT EE_E2p_GetStatus( void );
#endif	/* __E2P_GETSTATUS_API__ */

/** @brief	E2P Driver Initialization.
 *  @param[in]	Flags	E2P Driver Initialization Configuration Flags.
 *  @param[in]	NotifCB	E2P Notification Call-Back Function Pointer.
 *  @return	Extended Status Only:
 *  	- E_OK:			E2P Driver Initializion Success.
 *  	- E_NOT_OK:		E2P Driver Already Initialized.
 *  	- E_E2P_PARAM_FLAGS:	E2P Configuration Flags Invalid.
 *
 *  This service initializes the E2P driver.
 *
 *  The function <tt>EE_E2p_Init()</tt> shall initialize the E2P module.
 *
 *  After having finished the module initialization, the function
 *  <tt>EE_E2p_Init()</tt> shall set the EEPROM state to "OPERATIONAL".
 *
 *  The E2P module's implementer shall apply the following rules regarding
 *  initialization of controller registers within the function
 *  <tt>EE_E2p_Init()</tt>:
 *  	- If the hardware allows for only one usage of the register, the driver
 *  	  module implementing that functionality is responsible for initializing
 *  	  the register.
 *  	- If the register can affect several hardware modules and if it is an
 *  	  I/O register, it shall be initialised by the GPIO driver.
 *  	- If the register can affect several hardware modules and if it is not
 *  	  an I/O register, it shall be initialised by this MCU Driver.
 *  	- One-time writable registers that require initialisation directly after
 *  	  reset shall be initialised by the startup code.
 *  	- All other registers not mentioned before shall be initialised by the
 *  	  start-up code.
 *
 *  If Extended Status for the E2P module is enabled: If the E2P Driver has
 *  ALREADY been initialized, the function <tt>EE_E2p_Init()</tt> shall return
 *  the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the E2P module is enabled: If the parameter
 *  <tt>Flags</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_E2p_Init()</tt> shall return the
 *  development error <tt>E_E2P_PARAM_FLAGS</tt>.
 *
 *  A re-initialization of the E2P Driver by executing the
 *  <tt>EE_E2p_Init()</tt> function requires a de-initialization before by
 *  executing a <tt>EE_E2p_DeInit</tt>.
 */
#ifdef	__E2P_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_E2p_Init(
  EE_TYPEE2PFLAGS	Flags,
  EE_ISR_callback	NotifCB
);

#ifdef	__E2P_DEINIT_API__
/** @brief	E2P Driver De-Initialization.
 *  @param[in]	Flags	E2P Driver De-Initialization Configuration Flags.
 *  @return	Extended Status Only:
 *  	- E_OK:			E2P Driver De-Initializion Success.
 *  	- E_NOT_OK:		E2P Driver NOT Initialized.
 *  	- E_E2P_PARAM_FLAGS:	E2P Configuration Flags Invalid.
 *  	- E_E2P_BUSY:		E2P Busy.
 *
 *  This service de-initializes the E2P Driver.
 *
 *  The function <tt>EE_E2p_DeInit()</tt> shall deinitialize the E2P Module.
 *  Values of registers which are not writeable are excluded. It's the
 *  responsibility of the hardware design that the state does not lead to
 *  undefined activities in the uC.
 *
 *  If Extended Status for the E2P module is enabled: If the E2P Driver is NOT
 *  initialized, the function <tt>EE_E2p_DeInit()</tt> shall return the
 *  development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the E2P module is enabled: If the parameter
 *  <tt>Flags</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_E2p_DeInit()</tt> shall return the
 *  development error <tt>E_E2P_PARAM_FLAGS</tt>.
 *
 *  If Extended Status for the E2P module is enabled: If the EEPROM is in state
 *  "BUSY", the function <tt>EE_E2p_DeInit()</tt> shall return the
 *  development error <tt>E_E2P_BUSY</tt>.
 */
#ifdef	__E2P_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_E2p_DeInit(
  EE_TYPEE2PFLAGS	Flags
);
#endif	/* __E2P_DEINIT_API__ */

#ifdef __E2P_READ_API__
/** @brief	Reads from EEPROM.
 *  @param[in]	Address		Address offset in EEPROM
 *  				(will be added to the EEPROM base address).
 *  				Min.: 0, Max.: <tt>EE_E2P_SIZE</tt> - 1.
 *  @param[in]	BufferPtr	Pointer to destination data buffer in RAM.
 *  @param[in]	Size		Number of bytes to read
 *  				Min.: 1,
 *  				Max.: <tt>EE_E2P_SIZE</tt> - <tt>Address</tt>.
 *  @return	Extended Status Only:
 *  	- E_OK:			Read command has been accepted.
 *  	- E_NOT_OK:		E2P Driver NOT Initialized.
 *  	- E_E2P_PARAM_ADDRESS:	Invalid Address.
 *  	- E_E2P_PARAM_NULL:	Buffer Pointer is NULL.
 *  	- E_E2P_PARAM_SIZE:	Invalid Size.
 *  	- E_E2P_BUSY:		Driver Busy.
 *
 *  This service Reads Data from EEPROM.
 *
 *  The function <tt>EE_E2p_Read()</tt> shall copy the given parameters,
 *  initiate a read job, set the EEPROM status to "BUSY" and return.
 *
 *  The E2P module shall execute the read job asynchronously within the E2P
 *  module's job processing function. During job processing the E2P module shall
 *  read a data block of size <tt>Size</tt> from <tt>Address</tt> + EEPROM Base
 *  Address to *<tt>BufferPtr</tt>.
 *
 *  The E2P's user shall only call <tt>EE_E2p_Read()</tt> after the E2P module
 *  has been initialized using the function <tt>EE_E2p_Init()</tt>.
 *
 *  If Extended Status for the E2P module is enabled: If the E2P Driver is NOT
 *  initialized, the function <tt>EE_E2p_Read()</tt> shall return with
 *  development error <tt>E_NOT_OK</tt>.
 *
 *  The E2P's user shall not call the function <tt>EE_E2p_Read()</tt> during a
 *  running E2P module job (read/write/erase/compare).
 *
 *  If Extended Status for the module E2P is enabled: the function
 *  <tt>EE_E2p_Read()</tt> shall check the EEPROM state for being NOT "BUSY".
 *  If the EEPROM state is "BUSY", the called function shall return with
 *  development error E_E2P_BUSY.
 *
 *  If Extended Status for the module E2P is enabled: the function
 *  <tt>EE_E2p_Read()</tt> shall check that <tt>BufferPtr</tt> is not
 *  <tt>NULL</tt>.
 *  If <tt>BufferPtr</tt> is <tt>NULL</tt>, the called function shall return
 *  with development error <tt>E_E2P_PARAM_NULL</tt>.
 *
 *  If Extended Status for the module E2P is enabled: the function
 *  <tt>EE_E2p_Read()</tt> shall check that <tt>Address</tt> is valid.
 *  If <tt>Address</tt> is not within the valid EEPROM address range, the called
 *  function shall return with development error <tt>E_E2P_PARAM_ADDRESS</tt>.
 *
 *  If Extended Status for the module E2P is enabled: the function
 *  <tt>EE_E2p_Read()</tt> check that the parameter <tt>Size</tt> is within the
 *  specified minimum and maximum values: Min.:1, Max.: <tt>EE_E2P_SIZE</tt> -
 *  <tt>Address</tt>.
 *  If the parameter <tt>Size</tt> is not within the specified minimum and
 *  maximum values, the called function shall return with development error
 *  <tt>E_E2P_PARAM_SIZE</tt>.
 */
#ifdef	__E2P_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_E2p_Read(
  EE_TYPEE2PADDR	Address,
  EE_TYPEE2PDATA	*BufferPtr,
  EE_TYPEE2PSIZE	Size
);
#endif	/* __E2P_READ_API__ */

#ifdef __E2P_WRITE_API__
/** @brief	Writes to EEPROM.
 *  @param[in]	Address		Address offset in EEPROM
 *  				(will be added to the EEPROM base address).
 *  				Min.: 0, Max.: <tt>EE_E2P_SIZE</tt> - 1.
 *  @param[in]	BufferPtr	Pointer to source data buffer in RAM.
 *  @param[in]	Size		Number of bytes to write
 *  				Min.: 1,
 *  				Max.: <tt>EE_E2P_SIZE</tt> - <tt>Address</tt>.
 *  @return	Extended Status Only:
 *  	- E_OK:			Write command has been accepted.
 *  	- E_NOT_OK:		E2P Driver NOT Initialized.
 *  	- E_E2P_PARAM_ADDRESS:	Invalid Address.
 *  	- E_E2P_PARAM_NULL:	Buffer Pointer is NULL.
 *  	- E_E2P_PARAM_SIZE:	Invalid Size.
 *  	- E_E2P_BUSY:		Driver Busy.
 *
 *  This service Writes Data to EEPROM.
 *
 *  The function <tt>EE_E2p_Write()</tt> shall copy the given parameters,
 *  initiate a write job, set the EEPROM status to "BUSY" and return.
 *
 *  The E2P module shall execute the read job asynchronously within the E2P
 *  module's job processing function. During job processing the E2P module shall
 *  write a data block of size <tt>Size</tt> from <tt>Address</tt> + EEPROM Base
 *  Address to *<tt>BufferPtr</tt>.
 *
 *  The E2P's user shall only call <tt>EE_E2p_Write()</tt> after the E2P module
 *  has been initialized using the function <tt>EE_E2p_Init()</tt>.
 *
 *  If Extended Status for the E2P module is enabled: If the E2P Driver is NOT
 *  initialized, the function <tt>EE_E2p_Write()</tt> shall return with
 *  development error <tt>E_NOT_OK</tt>.
 *
 *  The E2P's user shall not call the function <tt>EE_E2p_Write()</tt> during a
 *  running E2P module job (read/write/erase/compare).
 *
 *  If Extended Status for the module E2P is enabled: the function
 *  <tt>EE_E2p_Write()</tt> shall check the EEPROM state for being NOT "BUSY".
 *  If the EEPROM state is "BUSY", the called function shall return with
 *  development error E_E2P_BUSY.
 *
 *  If Extended Status for the module E2P is enabled: the function
 *  <tt>EE_E2p_Write()</tt> shall check that <tt>BufferPtr</tt> is not
 *  <tt>NULL</tt>.
 *  If <tt>BufferPtr</tt> is <tt>NULL</tt>, the called function shall return
 *  with development error <tt>E_E2P_PARAM_NULL</tt>.
 *
 *  If Extended Status for the module E2P is enabled: the function
 *  <tt>EE_E2p_Write()</tt> shall check that <tt>Address</tt> is valid.
 *  If <tt>Address</tt> is not within the valid EEPROM address range, the called
 *  function shall return with development error <tt>E_E2P_PARAM_ADDRESS</tt>.
 *
 *  If Extended Status for the module E2P is enabled: the function
 *  <tt>EE_E2p_Write()</tt> check that the parameter <tt>Size</tt> is within the
 *  specified minimum and maximum values: Min.:1, Max.: <tt>EE_E2P_SIZE</tt> -
 *  <tt>Address</tt>.
 *  If the parameter <tt>Size</tt> is not within the specified minimum and
 *  maximum values, the called function shall return with development error
 *  <tt>E_E2P_PARAM_SIZE</tt>.
 */
#ifdef	__E2P_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_E2p_Write(
  EE_TYPEE2PADDR	Address,
  EE_TYPEE2PDATA	*BufferPtr,
  EE_TYPEE2PSIZE	Size
);
#endif	/* __E2P_WRITE_API__ */

#ifdef __E2P_ERASE_API__
/** @brief	Erases EEPROM.
 *  @param[in]	Address		Address offset in EEPROM
 *  				(will be added to the EEPROM base address).
 *  				Min.: 0, Max.: <tt>EE_E2P_SIZE</tt> - 1.
 *  @param[in]	Size		Number of bytes to Erase
 *  				Min.: 1,
 *  				Max.: <tt>EE_E2P_SIZE</tt> - <tt>Address</tt>.
 *  @return	Extended Status Only:
 *  	- E_OK:			Erase command has been accepted.
 *  	- E_NOT_OK:		E2P Driver NOT Initialized.
 *  	- E_E2P_PARAM_ADDRESS:	Invalid Address.
 *  	- E_E2P_PARAM_NULL:	Buffer Pointer is NULL.
 *  	- E_E2P_PARAM_SIZE:	Invalid Size.
 *  	- E_E2P_BUSY:		Driver Busy.
 *
 *  This service erases EEPROM sections.
 *
 *  The function <tt>EE_E2p_Erase()</tt> shall copy the given parameters,
 *  initiate an erase job, set the EEPROM status to "BUSY" and return.
 *
 *  The E2P module shall execute the erase job asynchronously within the E2P
 *  module's job processing function. During job processing the E2P module shall
 *  erase an EEPROM block of size <tt>Size</tt> starting from <tt>Address</tt> +
 *  EEPROM Base Address.
 *
 *  The E2P's user shall only call <tt>EE_E2p_Erase()</tt> after the E2P module
 *  has been initialized using the function <tt>EE_E2p_Init()</tt>.
 *
 *  If Extended Status for the E2P module is enabled: If the E2P Driver is NOT
 *  initialized, the function <tt>EE_E2p_Erase()</tt> shall return with
 *  development error <tt>E_NOT_OK</tt>.
 *
 *  The E2P's user shall not call the function <tt>EE_E2p_Erase()</tt> during a
 *  running E2P module job (read/write/erase/compare).
 *
 *  If Extended Status for the module E2P is enabled: the function
 *  <tt>EE_E2p_Erase()</tt> shall check the EEPROM state for being NOT "BUSY".
 *  If the EEPROM state is "BUSY", the called function shall return with
 *  development error E_E2P_BUSY.
 *
 *  If Extended Status for the module E2P is enabled: the function
 *  <tt>EE_E2p_Erase()</tt> shall check that <tt>Address</tt> is valid.
 *  If <tt>Address</tt> is not within the valid EEPROM address range, the called
 *  function shall return with development error <tt>E_E2P_PARAM_ADDRESS</tt>.
 *
 *  If Extended Status for the module E2P is enabled: the function
 *  <tt>EE_E2p_Erase()</tt> check that the parameter <tt>Size</tt> is within the
 *  specified minimum and maximum values: Min.:1, Max.: <tt>EE_E2P_SIZE</tt> -
 *  <tt>Address</tt>.
 *  If the parameter <tt>Size</tt> is not within the specified minimum and
 *  maximum values, the called function shall return with development error
 *  <tt>E_E2P_PARAM_SIZE</tt>.
 */
#ifdef	__E2P_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_E2p_Erase(
  EE_TYPEE2PADDR	Address,
  EE_TYPEE2PSIZE	Size
);
#endif	/* __E2P_ERASE_API__ */

#ifdef __E2P_COMPARE_API__
/** @brief	Compares with EEPROM.
 *  @param[in]	Address		Address offset in EEPROM
 *  				(will be added to the EEPROM base address).
 *  				Min.: 0, Max.: <tt>EE_E2P_SIZE</tt> - 1.
 *  @param[in]	BufferPtr	Pointer to data buffer in RAM (compare data).
 *  @param[in]	Size		Number of bytes to write
 *  				Min.: 1,
 *  				Max.: <tt>EE_E2P_SIZE</tt> - <tt>Address</tt>.
 *  @return	Extended Status Only:
 *  	- E_OK:			Compare command has been accepted.
 *  	- E_NOT_OK:		E2P Driver NOT Initialized.
 *  	- E_E2P_PARAM_ADDRESS:	Invalid Address.
 *  	- E_E2P_PARAM_NULL:	Buffer Pointer is NULL.
 *  	- E_E2P_PARAM_SIZE:	Invalid Size.
 *  	- E_E2P_BUSY:		Driver Busy.
 *
 *  This service Compares a data block in EEPROM with an EEPROM block in the
 *  memory.
 *
 *  The function <tt>EE_E2p_Compare()</tt> shall copy the given parameters,
 *  initiate a compare job, set the EEPROM status to "BUSY" and return.
 *
 *  The E2P module shall execute the compare job asynchronously within the E2P
 *  module's job processing function. During job processing the E2P module shall
 *  compare the EEPROM data block at <tt>Address</tt> + EEPROM Base Address of
 *  size <tt>Size</tt> with the data block at *<tt>BufferPtr</tt> of the same
 *  size.
 *
 *  The E2P's user shall only call <tt>EE_E2p_Compare()</tt> after the E2P
 *  module has been initialized using the function <tt>EE_E2p_Init()</tt>.
 *
 *  If Extended Status for the E2P module is enabled: If the E2P Driver is NOT
 *  initialized, the function <tt>EE_E2p_Compare()</tt> shall return with
 *  development error <tt>E_NOT_OK</tt>.
 *
 *  The E2P's user shall not call the function <tt>EE_E2p_Compare()</tt> during
 *  a running E2P module job (read/write/erase/compare).
 *
 *  If Extended Status for the module E2P is enabled: the function
 *  <tt>EE_E2p_Compare()</tt> shall check the EEPROM state for being NOT "BUT".
 *  If the EEPROM state is "BUSY", the called function shall return with
 *  development error E_E2P_BUSY.
 *
 *  If Extended Status for the module E2P is enabled: the function
 *  <tt>EE_E2p_Compare()</tt> shall check that <tt>BufferPtr</tt> is not
 *  <tt>NULL</tt>.
 *  If <tt>BufferPtr</tt> is <tt>NULL</tt>, the called function shall return
 *  with development error <tt>E_E2P_PARAM_NULL</tt>.
 *
 *  If Extended Status for the module E2P is enabled: the function
 *  <tt>EE_E2p_Compare()</tt> shall check that <tt>Address</tt> is valid.
 *  If <tt>Address</tt> is not within the valid EEPROM address range, the called
 *  function shall return with development error <tt>E_E2P_PARAM_ADDRESS</tt>.
 *
 *  If Extended Status for the module E2P is enabled: the function
 *  <tt>EE_E2p_Compare()</tt> check that the parameter <tt>Size</tt> is within the
 *  specified minimum and maximum values: Min.:1, Max.: <tt>EE_E2P_SIZE</tt> -
 *  <tt>Address</tt>.
 *  If the parameter <tt>Size</tt> is not within the specified minimum and
 *  maximum values, the called function shall return with development error
 *  <tt>E_E2P_PARAM_SIZE</tt>.
 */
#ifdef	__E2P_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_E2p_Compare(
  EE_TYPEE2PADDR	Address,
  EE_TYPEE2PDATA	*BufferPtr,
  EE_TYPEE2PSIZE	Size
);
#endif	/* __E2P_COMPARE_API__ */

#if	( defined(__E2P_CANCEL_API__) && !defined(__E2P_SYNCHRONOUS__) )
/** @brief	Cancels a running job
 *  @return	Extended Status Only:
 *  	- E_OK:			Cancel command successful.
 *  	- E_NOT_OK:		E2P Driver NOT Initialized.
 *
 *  This service Cancels a running EEPROM job (read/write/erase/compare).
 *
 *  The function <tt>EE_E2p_Cancel()</tt> shall cancel an ongoing EEPROM read,
 *  write, erase or compare job.
 *
 *  The function <tt>EE_E2p_Cancel()</tt> shall abort a running job
 *  synchronously so that directly after returning from this function a new job
 *  can be requested by the upper layer.
 *
 *  @note	The function <tt>EE_E2p_Cancel()</tt> is synchronous in its
 *  		behavior but at the same time asynchronous w.r.t. the underlying
 *  		hardware. The job of the <tt>EE_E2p_Cancel()</tt> function (i.e.
 *  		make the module ready for a new job request) is finished when it
 *  		returns to the caller (hence it is synchronous), but on the
 *  		other hand e.g. an erase job might still be ongoing in the
 *  		hardware device (hence it is asynchronousw.r.t. the hardware).
 *
 *  The function <tt>EE_E2p_Cancel()</tt> shall set the E2P module state to NOT
 *  "BUSY".
 *
 *  If configured, <tt>EE_E2p_Cancel()</tt> shall call the notification callback
 *  in order to inform the caller about the cancelation of a job.
 *
 *  The function <tt>EE_E2p_Cancel()</tt> shall set the status of the current
 *  running job to <tt>EE_E2P_CANCELED</tt> if the EEPROM status currently has
 *  the value  <tt>EE_E2P_BUSY</tt>. Otherwise it shall leave the status
 *  unchanged.
 *
 *  @note	The states and data of the affected EEPROM cells will be
 *  		undefined when canceling an ongoing write orerase job with the
 *  		function <tt>EE_E2p_Cancel()</tt>. 
 *
 *  Canceling any job on-going with the service <tt>EE_E2p_Cancel()</tt> in an
 *  external EEPROM device might set this one in a blocking state.
 *
 *  The E2P's user shall only call <tt>EE_E2p_Cancel()</tt> after the E2P
 *  module has been initialized using the function <tt>EE_E2p_Init()</tt>.
 *
 *  If Extended Status for the E2P module is enabled: If the E2P Driver is NOT
 *  initialized, the function <tt>EE_E2p_Cancel()</tt> shall return with
 *  development error <tt>E_NOT_OK</tt>.
 */
#ifdef	__E2P_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_E2p_Cancel(
  void
);
#endif	/* __E2P_CANCEL_API__ && !__E2P_SYNCHRONOUS__ */

#if	( !defined(__E2P_INTERRUPT__) && !defined(__E2P_SYNCHRONOUS__) )
/** @brief	Main Function
 *
 *  Service to perform the processing of the EEPROM jobs
 *  (read/write/erase/compare).
 *
 *  The function <tt>EE_E2p_MainFunction()</tt> shall perform the processing of
 *  the EEPROM read, write, erase and compare jobs.
 *
 *  When a job has been initiated, the E2P's user shall call the function
 *  <tt>EE_E2p_MainFunction()</tt> cyclically until the job is finished.
 *
 *  @note	The function <tt>EE_E2p_MainFunction()</tt> may also be called
 *  		cyclically if no job is currently pending.
 *
 *  The configuration parameter <tt>EE_E2P_JOB_CALL_CYCLE</tt> shall be used for
 *  internal timing of the EEPROM driver (deadline monitoring, write and erase
 *  timing etc.) if needed by the implementation and/or the underlying hardware.
 *
 *  The function <tt>EE_E2p_MainFunction()</tt> shall return without action if
 *  no job is pending.
 *
 *  The function <tt>EE_E2p_MainFunction()</tt> shall set the internal status to
 *  <tt>EE_E2P_ERROR</tt> if an EEPROM erase job fails due to a hardware error.
 *
 *  The function <tt>EE_E2p_MainFunction()</tt> shall set the internal status to
 *  <tt>EE_E2P_ERROR</tt> if an EEPROM write job fails due to a hardware error.
 *
 *  The function <tt>EE_E2p_MainFunction()</tt> shall set the internal status to
 *  <tt>EE_E2P_ERROR</tt> if an EEPROM read job fails due to a hardware error.
 *
 *  The function <tt>EE_E2p_MainFunction()</tt> shall set the internal status to
 *  <tt>EE_E2P_ERROR</tt> if an EEPROM compare job fails due to a hardware
 *  error.
 */
void EE_E2p_MainFunction( void );
#endif	/* !__E2P_INTERRUPT__ && !__E2P_SYNCHRONOUS__ */

#endif	/* __EE_MCU_COMMON_E2P_H__ */
