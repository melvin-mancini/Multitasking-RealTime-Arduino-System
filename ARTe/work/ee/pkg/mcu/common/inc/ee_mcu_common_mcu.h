/** @file	ee_mcu_common_mcu.h
 *
 *  @brief	Erika Enterprise - MCU Driver - MCU Common Header File.
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
 *  The MCU driver provides MCU services for Clock and RAM initialization.
 *  In the MCU configuration set, the MCU specific settings for the Clock
 *  (i.e. PLL setting) and RAM (i.e. section base address and size) shall be
 *  configured.
 *
 *  The MCU module shall provide a service to provide software triggering of a
 *  hardware reset.
 *
 *  @note	Only an authorized user shall be able to call this reset service
 *  		function.
 *
 *  The MCU module shall provide services to get the reset reason of the last
 *  reset if the hardware supports such a feature.
 *
 *  @note	In an ECU, there are several sources which can cause a reset.
 *  		Depending on the reset reason, several application scenarios
 *  		might be necessary after reinitialization of the MCU.
 * 
 *  @author	Riccardo Schiavi
 *  @author	Giuseppe Serano
 *  @version	0.1
 *  @date	2013
 */

#ifndef	__EE_MCU_COMMON_MCU_H__
#define	__EE_MCU_COMMON_MCU_H__

/*
 * Return Values: Extended Status Only.
 */
#ifdef	__MCU_EXTENDED_STATUS__

/** @brief	Invalid MCU De-Initialization Flags. */
#define	E_MCU_PARAM_INIT_FLAGS		0xFE

/** @brief	Invalid Clock Sources Flags. */
#define	E_MCU_PARAM_CLKSRCS_FLAGS	0xFD

/** @brief	Invalid Clock Multiplexer Flags. */
#define	E_MCU_PARAM_CLKMUX_FLAGS	0xFC

/** @brief	Invalid Clock Divisor Flags. */
#define	E_MCU_PARAM_CLKDIV_FLAGS	0xFB

/** @brief	Invalid MCU Mode Flags. */
#define	E_MCU_PARAM_MODE_FLAGS		0xFA

/** @brief	Pointer is NULL. */
#define	E_MCU_PARAM_NULL		0xF9

#endif	/* __MCU_EXTENDED_STATUS__ */

/** @brief	MCU Configuration Flags.
 *
 *  Type for setting-up a MCU Driver Configuration.
 *
 *  Implementation specific. But not all values may be valid within this type.
 *
 *  This type shall be chosen in order to have the most efficient implementation
 *  on a specific micro controller platform.
 *
 *  For parameter values of type <tt>EE_TYPEMCUFLAGS</tt>, the Mcu's user shall
 *  use the symbolic names provided by the driver implementation.
 *
 *  The type shall be EE_UREG depending on uC platform.
 * 
 */
#ifndef	EE_TYPEMCUFLAGS
#define	EE_TYPEMCUFLAGS	EE_UREG
#endif

/** @brief	MCU Driver Default Configuration Flag. */
#define	EE_MCU_FLAG_DEFAULT	0x00U

/** @brief	Milliseconds to Ticks.
 *  @param[in]	_MS	Milliseconds to convert.
 *  @param[in]	_FHZ	Reference Frequency in Hz.
 *  @return	Number of Ticks.
 *
 *  Utility Macro that convert an amount of ms in number of ticks of a given
 *  frequency in Hz.
 */
#define	EE_MCU_MS_2_TICKS(_MS, _FHZ)	((_MS) * ((_FHZ) / 1000U))

/** @brief	Microseconds to Ticks.
 *  @param[in]	_US	Microseconds to convert.
 *  @param[in]	_FHZ	Reference Frequency in Hz.
 *  @return	Number of Ticks.
 *
 *  Utility Macro that convert an amount of us in number of ticks of a given
 *  frequency in Hz.
 */
#define	EE_MCU_US_2_TICKS(_US, _FHZ)	( 			\
	((_US) / 1000UL)?					\
		(EE_MCU_MS_2_TICKS(((_US) / 1000UL), (_FHZ))):	\
		(EE_MCU_MS_2_TICKS((_US), (_FHZ)) / 1000U)	\
)

/** @brief	Clock Status Type
 *
 *  The type <tt>EE_TYPEMCUCLKSTATUS</tt> is the type of the return value of the
 *  function <tt>EE_Mcu_GetClockStatus()</tt>.
 *
 *  The type of <tt>EE_TYPEMCUCLKSTATUS</tt> is an enumeration with the
 *  following values:
 *  - <tt>EE_MCU_CLK_STABLE</tt>
 *  - <tt>EE_MCU_CLK_UNSTABLE</tt>
 *  - <tt>EE_MCU_CLK_STATUS_UNDEFINED</tt>.
 */
typedef	enum {
  EE_MCU_CLK_STABLE,		/**< Clock Sources are now Stable.	      */
  EE_MCU_CLK_UNSTABLE,		/**< Clock Sources are still unstable.	      */
  EE_MCU_CLK_STATUS_UNDEFINED,	/**< Clock Sources Status is unknown.	      */
} EE_TYPEMCUCLKSTATUS;

/** @brief	MCU Clock Sources Configuration Flags.
 *
 *  Type for setting-up a MCU Clock Sources.
 *
 *  Implementation specific. But not all values may be valid within this type.
 *
 *  This type shall be chosen in order to have the most efficient implementation
 *  on a specific micro controller platform.
 *
 *  For parameter values of type <tt>EE_TYPEMCUCLKSRCSFLAGS</tt>, the Mcu's user
 *  shall use the symbolic names provided by the driver implementation.
 *
 *  The type shall be EE_UREG depending on uC platform.
 * 
 */
#ifndef	EE_TYPEMCUCLKSRCSFLAGS
#define	EE_TYPEMCUCLKSRCSFLAGS	EE_UREG
#endif

/*
 * MCU Clock Source Configuration Flags.
 */
/** @brief	MCU Default Clock Sources Configuration Flag. */
#define	EE_MCU_CLK_SRCS_FLAG_DEFAULT	0x00U

/** @brief	MCU Clock Multiplexer Configuration Flags.
 *
 *  Type for setting-up a MCU Clock Multiplexer.
 *
 *  Implementation specific. But not all values may be valid within this type.
 *
 *  This type shall be chosen in order to have the most efficient implementation
 *  on a specific micro controller platform.
 *
 *  For parameter values of type <tt>EE_TYPEMCUCLKMUXFLAGS</tt>, the Mcu's user
 *  shall use the symbolic names provided by the driver implementation.
 *
 *  The type shall be EE_UREG depending on uC platform.
 * 
 */
#ifndef	EE_TYPEMCUCLKMUXFLAGS
#define	EE_TYPEMCUCLKMUXFLAGS	EE_UREG
#endif

/*
 * MCU Clock Multiplexer Configuration Flags.
 */
/** @brief	MCU Default Clock Multiplexer Configuration Flag. */
#define	EE_MCU_CLK_MUX_FLAG_DEFAULT	0x00U

/** @brief	MCU Clock Divisor Configuration Flags.
 *
 *  Type for setting-up a MCU Clock Divisor.
 *
 *  Implementation specific. But not all values may be valid within this type.
 *
 *  This type shall be chosen in order to have the most efficient implementation
 *  on a specific micro controller platform.
 *
 *  For parameter values of type <tt>EE_TYPEMCUCLKDIVFLAGS</tt>, the Mcu's user
 *  shall use the symbolic names provided by the driver implementation.
 *
 *  The type shall be EE_UREG depending on uC platform.
 * 
 */
#ifndef	EE_TYPEMCUCLKDIVFLAGS
#define	EE_TYPEMCUCLKDIVFLAGS		EE_UREG
#endif

/*
 * MCU Clock Divisor Configuration Flags.
 */
/** @brief	MCU Default Clock Divisor Configuration Flag. */
#define	EE_MCU_CLK_DIV_FLAG_DEFAULT	0x00U


/** @brief	MCU Clock Divisor Configuration Flags.
 *
 *  Type for setting-up a MCU Clock Divisor.
 *
 *  Implementation specific. But not all values may be valid within this type.
 *
 *  This type shall be chosen in order to have the most efficient implementation
 *  on a specific micro controller platform.
 *
 *  For parameter values of type <tt>EE_TYPEMCUCLKDIVFLAGS</tt>, the Mcu's user
 *  shall use the symbolic names provided by the driver implementation.
 *
 *  The type shall be EE_UREG depending on uC platform.
 * 
 */
#ifndef	EE_TYPEMCUCLKDIVFLAGS
#define	EE_TYPEMCUCLKDIVFLAGS	EE_UREG
#endif

/*
 * MCU Clock Divisor Configuration Flags.
 */
/** @brief	MCU Default Clock Divisor Configuration Flag. */
#define	EE_MCU_CLK_DIV_FLAG_DEFAULT	0x00U

/** @brief	MCU Mode Configuration Flags.
 *
 *  Type for setting-up a MCU Modes.
 *
 *  Implementation specific. But not all values may be valid within this type.
 *
 *  This type shall be chosen in order to have the most efficient implementation
 *  on a specific micro controller platform.
 *
 *  For parameter values of type <tt>EE_TYPEMCUMODEFLAGS</tt>, the Mcu's user
 *  shall use the symbolic names provided by the driver implementation.
 *
 *  The type shall be EE_UREG depending on uC platform.
 * 
 */
#ifndef	EE_TYPEMCUMODEFLAGS
#define	EE_TYPEMCUMODEFLAGS	EE_UREG
#endif

/*
 * MCU Mode Configuration Flags.
 */
/** @brief	MCU Default Mode Configuration Flag. */
#define	EE_MCU_MODE_FLAG_DEFAULT	0x00U

/** @brief	MCU Reset Reason Flags.
 *
 *  Type for retrieving MCU reset reason.
 *
 *  Implementation specific. But not all values may be valid within this type.
 *
 *  This type shall be chosen in order to have the most efficient implementation
 *  on a specific micro controller platform.
 *
 *  For parameter values of type <tt>EE_TYPEMCURESETFLAGS</tt>, the Mcu's user
 *  shall use the symbolic names provided by the driver implementation.
 *
 *  The type shall be EE_UREG depending on uC platform.
 * 
 */
#ifndef	EE_TYPEMCURESETFLAGS
#define	EE_TYPEMCURESETFLAGS	EE_UREG
#endif

/*
 * MCU Reset Reason Flags.
 */
/** @brief	MCU Undefined Reset Flag. */
#define	EE_MCU_RESET_FLAG_UNDEFINED	0x00U

/** @brief	MCU Driver Initialization.
 *  @param[in]	Flags	MCU Driver Initialization Configuration Flags.
 *  @return	Extended Status Only:
 *  	- E_OK:				MCU Driver Initializion Success.
 *  	- E_NOT_OK:			MCU Driver Already Initialized.
 *  	- E_MCU_PARAM_INIT_FLAGS:	MCU Configuration Flags Invalid.
 *
 *  This service initializes the MCU driver.
 *
 *  The function <tt>EE_Mcu_Init()</tt> shall initialize the MCU module.
 *
 *  The MCU module's implementer shall apply the following rules regarding
 *  initialization of controller registers within the function
 *  <tt>EE_Mcu_Init()</tt>:
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
 *  If Extended Status for the MCU module is enabled: If the MCU Driver has
 *  ALREADY been initialized, the function <tt>EE_Mcu_Init()</tt> shall return
 *  the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the MCU module is enabled: If the parameter
 *  <tt>Flags</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Mcu_Init()</tt> shall return the
 *  development error <tt>E_MCU_PARAM_FLAGS</tt>.
 *
 *  A re-initialization of the MCU Driver by executing the
 *  <tt>EE_Mcu_Init()</tt> function requires a de-initialization before by
 *  executing a <tt>EE_Mcu_DeInit</tt>.
 */
#ifdef	__MCU_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Mcu_Init(
  EE_TYPEMCUFLAGS	Flags
);

#ifdef	__MCU_DEINIT_API__
/** @brief	MCU Driver De-Initialization.
 *  @param[in]	Flags	MCU Driver De-Initialization Configuration Flags.
 *  @return	Extended Status Only:
 *  	- E_OK:				MCU Driver De-Initializion Success.
 *  	- E_NOT_OK:			MCU Driver NOT Initialized.
 *  	- E_MCU_PARAM_INIT_FLAGS:	MCU Configuration Flags Invalid.
 *
 *  This service de-initializes the Mcu Driver.
 *
 *  The function <tt>EE_Mcu_DeInit()</tt> shall deinitialize the MCU Module.
 *  Values of registers which are not writeable are excluded. It's the
 *  responsibility of the hardware design that the state does not lead to
 *  undefined activities in the uC.
 *
 *  If Extended Status for the MCU module is enabled: If the MCU Driver is NOT
 *  initialized, the function <tt>EE_Mcu_DeInit()</tt> shall return the
 *  development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the MCU module is enabled: If the parameter
 *  <tt>Flags</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Mcu_DeInit()</tt> shall return the
 *  development error <tt>E_MCU_PARAM_FLAGS</tt>.
 */
#ifdef	__MCU_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Mcu_DeInit(
  EE_TYPEMCUFLAGS	Flags
);
#endif	/* __MCU_DEINIT_API__ */

#ifdef __MCU_INITCLOCK_API__
/** @brief	Clock Initialization.
 *  @param[in]	SrcsFlags	Clock Sources Flags.
 *  @return	Extended Status Only:
 *  	- E_OK:				Clock Initializion Success.
 *  	- E_NOT_OK:			MCU Driver Not Initialized.
 *  	- E_MCU_PARAM_CLKSRCS_FLAGS:	Clock Sources Flags Invalid.
 *
 *  This service initializes MCU specific clock sources.
 *
 *  The function <tt>EE_Mcu_InitClock()</tt> shall initialize MCU specific clock
 *  sources. The clock sources parameters are provided via the Clock Sources
 *  Flags <tt>SrcsFlags</tt>.
 *
 *  The function <tt>EE_Mcu_InitClock()</tt> shall start the PLL lock procedure
 *  (if PLL shall be configured and initialized) and shall return without
 *  waiting until the PLL is locked.
 *
 *  The MCU module's environment shall only call the function
 *  <tt>EE_Mcu_InitClock()</tt> after the MCU module has been initialized using
 *  the function <tt>EE_Mcu_Init()</tt>.
 *
 *  If Extended Status for the MCU module is enabled: If the MCU Driver is NOT
 *  initialized, the function <tt>EE_Mcu_InitClock()</tt> shall return the
 *  development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the MCU module is enabled: If the parameter
 *  <tt>SrcsFlags</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Mcu_InitClock()</tt> shall return the
 *  development error <tt>E_MCU_PARAM_CLKSRCS_FLAGS</tt>.
 *
 *  If Extended Status for the MCU module is enabled: If the parameter
 *  <tt>MuxFlags</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Mcu_InitClock()</tt> shall return the
 *  development error <tt>E_MCU_PARAM_CLKMUX_FLAGS</tt>.
 *
 *  If Extended Status for the MCU module is enabled: If the parameter
 *  <tt>DivFlags</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Mcu_InitClock()</tt> shall return the
 *  development error <tt>E_MCU_PARAM_CLKDIV_FLAGS</tt>.
 */
#ifdef	__MCU_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Mcu_InitClock(
  EE_TYPEMCUCLKSRCSFLAGS	SrcsFlags
);
#endif	/* __MCU_INITCLOCK_API__ */

#ifdef __MCU_DEINITCLOCK_API__
/** @brief	Clock De-Initialization.
 *  @param[in]	SrcsFlags	Clock Sources Flags.
 *  @return	Extended Status Only:
 *  	- E_OK:				Clock Initializion Success.
 *  	- E_NOT_OK:			MCU Driver Not Initialized.
 *  	- E_MCU_PARAM_CLKSRCS_FLAGS:	Clock Source Flags Invalid.
 *
 *  This service de-initializes MCU specific clock sources.
 *
 *  The function <tt>EE_Mcu_DeInitClock()</tt> shall de-initialize MCU specific
 *  clock sources. The clock sources parameters are provided via the Clock
 *  Sources Flags <tt>SrcsFlags</tt>.
 *
 *  The MCU module's environment shall only call the function
 *  <tt>EE_Mcu_DeInitClock()</tt> after the MCU module has been initialized
 *  using the function <tt>EE_Mcu_Init()</tt>.
 *
 *  If Extended Status for the MCU module is enabled: If the MCU Driver is NOT
 *  initialized, the function <tt>EE_Mcu_DeInitClock()</tt> shall return the
 *  development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the MCU module is enabled: If the parameter
 *  <tt>SrcsFlags</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Mcu_DeInitClock()</tt> shall return the
 *  development error <tt>E_MCU_PARAM_CLKSRCS_FLAGS</tt>.
 */
#ifdef	__MCU_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Mcu_DeInitClock(
  EE_TYPEMCUCLKSRCSFLAGS	SrcsFlags
);
#endif	/* __MCU_DEINITCLOCK_API__ */

#ifdef __MCU_GETCLOCKSTATUS_API__
/** @brief	Clock Sources Status Retrieval.
 *  @param[in]	SrcsFlags	Clock Sources Flags.
 *  @param[out]	Status		Clock Status to Retrieve.
 *  @return	Extended Status Only:
 *  	- E_OK:				Clock Sources Status Retrieval Success.
 *  	- E_NOT_OK:			MCU Driver Not Initialized.
 *  	- E_MCU_PARAM_CLKSRCS_FLAGS:	Clock Source Flags Invalid.
 *  	- E_MCU_PARAM_NULL:		Clock Status Pointer is <tt>NULL</tt>.
 *
 *  This service provides the status of the clock sources provided via the Clock
 *  Sources Flags.
 *
 *  The function <tt>EE_Mcu_GetClockStatus()</tt> shall return the status of
 *  the provided clock sources.
 *
 *  The function <tt>Mcu_GetClockStatus()</tt> shall return the status
 *  <tt>EE_MCU_CLK_STATUS_UNDEFINED</tt> if this function is called prior to
 *  calling of the function <tt>EE_Mcu_Init()</tt>.
 *
 *  If Extended Status for the MCU module is enabled: If the MCU Driver is NOT
 *  initialized, the function <tt>EE_Mcu_GetClockStatus()</tt> shall return the
 *  development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the MCU module is enabled: If the parameter
 *  <tt>SrcsFlags</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Mcu_InitClock()</tt> shall return the
 *  development error <tt>E_MCU_PARAM_CLKSRCS_FLAGS</tt>.
 *
 *  If Extended Status for the MCU module is enabled: If the parameter pointer
 *  <tt>Status</tt> is <tt>NULL</tt>, the function
 *  <tt>EE_Mcu_GetClockStatus()</tt> shall return the development error
 *  <tt>E_MCU_PARAM_NULL</tt>.
 */
#ifdef	__MCU_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Mcu_GetClockStatus(
  EE_TYPEMCUCLKSRCSFLAGS	SrcsFlags,
  EE_TYPEMCUCLKSTATUS		*Status
);
#endif	/* __MCU_GETCLOCKSTATUS_API__ */

#ifdef __MCU_DISTRIBUTECLOCK_API__
/** @brief	Clock Distribution.
 *  @param[in]	MuxFlags	Clock Multiplexer Flags.
 *  @param[in]	DivFlags	Clock Divisor Flags.
 *  @return	Extended Status Only:
 *  	- E_OK:				Clock Distribution Success.
 *  	- E_NOT_OK:			MCU Driver Not Initialized.
 *  	- E_MCU_PARAM_CLKMUX_FLAGS:	Clock Multiplexer Flags Invalid.
 *  	- E_MCU_PARAM_CLKDIV_FLAGS:	Clock Divisor Flags Invalid.
 *
 *  This service activates the MCU clock distribution.
 *
 *  The function <tt>EE_Mcu_DistributeClock()</tt> shall activate the MCU clock
 *  distribution.
 *
 *  The MCU module's environment shall execute the function
 *  <tt>EE_Mcu_DistributeClock()</tt> if the MCU module needs a separate
 *  request to activate clock after the clock is stable and ready. In this case,
 *  the function shall remove the current clock source (for example internal
 *  oscillator clock) from MCU clock distribution.
 *
 *  The function <tt>EE_Mcu_DistributeClock()</tt> shall return without
 *  affecting the MCU hardware if the clock has been automatically activated by
 *  the MCU hardware.
 *
 *  The MCU module's environment shall only call the function
 *  <tt>EE_Mcu_DistributeClock()</tt> after the status of the clock source has
 *  been detected as stable and ready by the function
 *  <tt>Mcu_GetClockStatus()</tt>.
 *
 *  If Extended Status for the MCU module is enabled: If the MCU Driver is NOT
 *  initialized, the function <tt>EE_Mcu_GetClockStatus()</tt> shall return the
 *  development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the MCU module is enabled: If the parameter
 *  <tt>MuxFlags</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Mcu_DistributeClock()</tt> shall return
 *  the development error <tt>E_MCU_PARAM_CLKMUX_FLAGS</tt>.
 *
 *  If Extended Status for the MCU module is enabled: If the parameter
 *  <tt>DivFlags</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Mcu_DistributeClock()</tt> shall return
 *  the development error <tt>E_MCU_PARAM_CLKDIV_FLAGS</tt>.
 */
#ifdef	__MCU_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Mcu_DistributeClock(
  EE_TYPEMCUCLKMUXFLAGS	MuxFlags,
  EE_TYPEMCUCLKDIVFLAGS	DivFlags
);
#endif	/* __MCU_DISTRIBUTECLOCK_API__ */

#ifdef	__MCU_SETMODE_API__
/** @brief	Mode Setting.
 *  @param[in]	ModeFlags	MCU Power Mode Flags.
 *  @return	Extended Status Only:
 *  	- E_OK:				Clock Distribution Success.
 *  	- E_NOT_OK:			MCU Driver Not Initialized.
 *  	- E_MCU_PARAM_MODE_FLAGS:	MCU Power Mode Flags Invalid.
 *
 *  This service activates the MCU power modes.
 *
 *  The function <tt>EE_Mcu_SetMode()</tt> shall set the MCU power mode.
 *  In case of CPU power down mode, the function <tt>EE_Mcu_SetMode()</tt>
 *  returns after it has performed a wake-up.
 *
 *  The MCU module's environment shall only call the function
 *  <tt>EE_Mcu_SetMode()</tt> after the MCU module has been initialized by the
 *  function <tt>EE_Mcu_Init()</tt>.
 *
 *  If Extended Status for the MCU module is enabled: If the MCU Driver is NOT
 *  initialized, the function <tt>EE_Mcu_SetMode()</tt> shall return the
 *  development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the MCU module is enabled: If the parameter
 *  <tt>ModeFlags</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Mcu_SetMode()</tt> shall return
 *  the development error <tt>E_MCU_PARAM_MODE_FLAGS</tt>.
 */
#ifdef	__MCU_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Mcu_SetMode(
  EE_TYPEMCUMODEFLAGS ModeFlags
);
#endif	/* __MCU_SETMODE_API__ */

#ifdef	__MCU_GETRESETREASON_API__
/** @brief	Reset Reason Retrieval.
 *  @param[out]	ResetFlags	Reset Reason Flags to Retrieve.
 *  @return	Extended Status Only:
 *  	- E_OK:			Retrieval Operartion Success.
 *  	- E_NOT_OK:		MCU Driver Not Initialized.
 *  	- E_MCU_PARAM_NULL	Reset Reason Flag Pointer is <tt>NULL</tt>.
 *
 *  The service reads the reset type from the hardware, if supported.
 *
 *  The function <tt>EE_Mcu_GetResetReason()</tt> shall read the reset reason
 *  from the hardware and return this reason if supported by the hardware.
 *  If the hardware does not support the hardware detection of the reset reason,
 *  the return value from the function <tt>EE_Mcu_GetResetReason()</tt> shall
 *  always be <tt>EE_MCU_RESET_FLAG_UNDEFINED</tt>.
 *
 *  The function <tt>EE_Mcu_GetResetReason()</tt> shall return
 *  <tt>EE_MCU_RESET_FLAG_UNDEFINED</tt> if this function is called prior to
 *  calling of the function <tt>EE_Mcu_Init()</tt>, and if supported by the
 *  hardware.
 *
 *  The function <tt>Mcu_GetResetReason()</tt> shall return an implementation
 *  specific value which correspond to a valid value of the reset status
 *  register if supported by the hardware.
 *
 *  @note	The User should ensure that the reset reason is cleared once it
 *  		has been read out to avoid multiple reset reasons.
 *
 *  If Extended Status for the MCU module is enabled: If the MCU Driver is NOT
 *  initialized, the function <tt>EE_Mcu_SetMode()</tt> shall return the
 *  development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the MCU module is enabled: If the parameter pointer
 *  <tt>ResetFlags</tt> is <tt>NULL</tt>, the function
 *  <tt>EE_Mcu_GetResetReason()</tt> shall return the development error
 *  <tt>E_MCU_PARAM_NULL</tt>.
 */
#ifdef	__MCU_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Mcu_GetResetReason(
  EE_TYPEMCUMODEFLAGS	*ResetFlags
);
#endif	/* __MCU_GETRESETREASON_API__ */

#ifdef	__MCU_PERFORMRESET_API__
/** @brief	Microcontroller Reset.
 *  @return	Extended Status Only:
 *  	- E_NOT_OK:	MCU Driver Not Initialized.
 *
 *  The service performs a microcontroller reset.
 *
 *  The function <tt>EE_Mcu_PerformReset()</tt> shall perform a
 *  microcontroller reset by using the hardware feature of the microcontroller.
 *
 *  The function <tt>EE_Mcu_PerformReset()</tt> shall perform the reset type
 *  which is configured in the initialization.
 *
 *  The MCU module's environment shall only call the function
 *  <tt>EE_Mcu_PerformReset</tt> after the MCU module has been initialized by
 *  the function <tt>EE_Mcu_Init</tt>.
 *
 *  If Extended Status for the MCU module is enabled: If the MCU Driver is NOT
 *  initialized, the function <tt>EE_Mcu_PerformReset()</tt> shall return the
 *  development error <tt>E_NOT_OK</tt>.
 */
#ifdef	__MCU_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Mcu_PerformReset(
  void
);
#endif	/* __MCU_PERFORMRESET_API__ */

#endif	/* __EE_MCU_COMMON_MCU_H__ */
