/** @file	ee_mcu_common_rtc.h
 *
 *  @brief	Erika Enterprise - RTC Driver - MCU Common Header File.
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
 *  The RTC driver provides Real Time Counter(s) services.
 *
 *  The RTC driver is part of the microcontroller abstraction layer (MCAL). It
 *  initializes and controls the internal Real Time Counter(s) (RTC) of the
 *  microcontroller.
 *
 *  The RTC driver provides services and configuration parameters for
 *  - Starting and stopping hardware counters
 *  - Getting counter values
 *  - Controlling time triggered interrupt notifications
 *
 *  The tick duration of a RTC channel depends on channel specific settings
 *  (part of RTC driver) as well as on system clock and settings of the clock
 *  tree controlled by the MCU module. The tick duration is not limited by this
 *  specification.
 *
 *  Not all hardware counters must be controlled by the RTC module.
 *  Some counters  may be controlled by Operating System (OS) or Other Drivers
 *  directly.
 *
 *  The number of RTC channels controlled by the RTC driver depends on hardware,
 *  implementation and system configuration.
 * 
 *  @author	Riccardo Schiavi
 *  @author	Giuseppe Serano
 *  @version	0.1
 */

#ifndef	__EE_MCU_COMMON_RTC_H__
#define	__EE_MCU_COMMON_RTC_H__

#ifdef	__RTC_GETSTATUS_API__
/** @brief	RTC Status Type
 *
 *  The type <tt>EE_TYPERTCCHSTAT</tt> is the type of the return value of the
 *  function <tt>EE_Rtc_GetStatus()</tt>.
 *
 *  The type of <tt>EE_TYPERTCCHSTAT</tt> is an enumeration describing the
 *  operation states for a RTC channel.
 */
typedef enum {
  EE_RTC_CH_UNINIT,		/**< "Un-Initialized" State.		      */
  EE_RTC_CH_OPERATIONAL,	/**< "Initialized" State. Normal operation.   */
  EE_RTC_CH_STOPPED,		/**< "Stopped" State.			      */
  EE_RTC_CH_RUNNING,		/**< "Running" State.			      */

} EE_TYPERTCCHSTAT;
#endif	/* __RTC_GETSTATUS_API__ */

#ifdef	__RTC_EXTENDED_STATUS__
/** @brief	Invalid Channel.		*/
#define	E_RTC_PARAM_CHANNEL	0xFE

/** @brief	Invalid Configuration Flags.	*/
#define	E_RTC_PARAM_FLAGS	0xFD

/** @brief	Invalid Counter.		*/
#define	E_RTC_PARAM_COUNTER	0xFC

/** @brief	Notification Call-Back Function Pointer is NULL. */
#define	E_RTC_PARAM_CALLBACK	0xFB

/** @brief	Busy Channel.			*/
#define	E_RTC_CHANNEL_BUSY	0xFA
#endif	/* __RTC_EXTENDED_STATUS__ */

/** @brief	Numeric ID of a RTC channel.
 *
 *  Numeric ID of a RTC channel.
 *  Implementation specific.
 *  But not all values may be valid within this type.
 *  This type shall be chosen in order to have the most efficient implementation
 *  on a specific micro controller platform.
 *
 *  Parameters of type <tt>EE_TYPERTCCHANNEL</tt> contain the numeric ID of a
 *  RTC channel.
 *
 *  The mapping of the ID is implementation specific but not configurable and
 *  not all values may be valid within this type.
 *
 *  For parameter values of type <tt>EE_TYPERTCCHANNEL</tt>, the Rtc's user
 *  shall use the symbolic names provided by the driver implementation.
 *
 *  The type shall be EE_UREG depending on uC platform.
 */
#ifndef	EE_TYPERTCCHANNEL
#define	EE_TYPERTCCHANNEL	EE_UREG
#endif

/** @brief	RTC Counter Type
 *
 *  Type for reading and setting the timer values (in number of ticks).
 *  The range of this type is uC dependent (width of the timer register) and has
 *  to be described by the supplier.
 */
#ifndef	EE_TYPERTCCOUNTER
#define	EE_TYPERTCCOUNTER	EE_UREG
#endif

/** @brief	RTC Configuration Flags.
 *
 *  Type for setting-up a RTC Channel Configuration.
 */
#ifndef	EE_TYPERTCFLAGS
#define	EE_TYPERTCFLAGS		EE_UREG
#endif

/** @brief	RTC Default Flag.
 *
 *  @note	RTC Default Flag implies:
 *  		- RTC Continuous Flag.
 */
#define	EE_RTC_FLAG_DEFAULT	0x00U

#ifdef	__RTC_GETSTATUS_API__
/** @brief	RTC Channel Status Retrieval.
 *  @param[in]	Channel	Numeric identifier of the RTC channel.
 *  @return	Always:
 *  	- EE_RTC_CH_UNINIT:		"Un-Initialized" State.
 *  	- EE_RTC_CH_OPERATIONAL:	Normal operation. "Initialized" State.
 *  	- EE_RTC_CH_STOPPED:		"Stopped" State.
 *  	- EE_RTC_CH_RUNNING:		"Running" State.
 *
 *  The service gets the status of the specified RTC Channel.
 *
 *  The function <tt>EE_Rtc_GetStatus()</tt> shall return the current
 *  operational status of the RTC Channel.
 */
EE_TYPERTCCHSTAT
EE_Rtc_GetStatus(
  EE_TYPERTCCHANNEL	Channel
);
#endif	/* __RTC_GETSTATUS_API__ */

/** @brief	RTC Channel Initialization.
 *  @param[in]	Channel	RTC Channel to be Initialized.
 *  @param[in]	Flags	RTC Channel Configuration Flags.
 *  @param[in]	NotifCB	RTC Channel Notification Call-Back Function Pointer.
 *  @return	Extended Status Only:
 *  	- E_OK:			RTC Channel initializion success.
 *  	- E_NOT_OK:		RTC Channel already initialized.
 *  	- E_RTC_PARAM_CHANNEL:	RTC Channel Invalid.
 *  	- E_RTC_PARAM_FLAG:	RTC Channel Configuration Flags Invalid.
 *
 *  This service initializes a RTC Channel.
 *
 *  The function <tt>EE_Rtc_Init()</tt> shall initialize the hardware timer
 *  channel, referenced by <tt>Channel</tt>, according to configuration flags
 *  referenced by <tt>Flags</tt>.
 *
 *  The function <tt>EE_Rtc_Init()</tt> shall disable all interrupt
 *  notifications, controlled by the RTC Channel.
 *
 *  The function <tt>EE_Rtc_Init()</tt> shall only initialize the referenced
 *  Channel and shall not touch other channels.
 *
 *  The following rules regarding initialization of controller registers shall
 *  apply to this driver implementation:
 *  - If the hardware allows for only one usage of the register, the driver
 *    module implementing that functionality is responsible for initializing the
 *    register.
 *  - If the register can affect several hardware modules and if it is an I/O
 *    register it shall be initialized by the GPIO driver.
 *  - If the register can affect several hardware modules and if it is not an
 *    I/O register it shall be initialized by the MCU driver.
 *  - One-time writable registers that require initialization directly after
 *    reset shall be initialized by the startup code.
 *  - All other registers shall be initialized by the startup code.
 *
 *  If Extended Status for the RTC module is enabled: If the RTC Channel is not
 *  in operation mode "uninitialized", the function <tt>EE_Rtc_Init()</tt> shall
 *  return the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the RTC module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Rtc_Init()</tt> shall return the
 *  development error <tt>E_RTC_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the RTC module is enabled: If the parameter
 *  <tt>Flags</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Rtc_Init()</tt> shall return the
 *  development error <tt>E_RTC_PARAM_FLAGS</tt>.
 *
 *  A re-initialization of the RTC channel by executing the
 *  <tt>EE_Rtc_Init()</tt> function requires a de-initialization before by
 *  executing a <tt>EE_Rtc_DeInit</tt>.
 */
#ifdef	__RTC_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Rtc_Init(
  EE_TYPERTCCHANNEL	Channel,
  EE_TYPERTCFLAGS	Flags,
  EE_ISR_callback	NotifCB
);

#ifdef	__RTC_DEINIT_API__
/** @brief	RTC Channel De-Initialization.
 *  @param[in]	Channel	RTC Channel to be De-Initialized.
 *  @param[in]	Flags	RTC Channel De-initialization Configuration Flags.
 *  @return	Extended Status Only:
 *  	- E_OK:			RTC Channel initializion success.
 *  	- E_NOT_OK:		RTC Channel NOT initialized.
 *  	- E_RTC_PARAM_CHANNEL:	RTC Channel Invalid.
 *  	- E_RTC_PARAM_FLAG:	RTC Channel De-Initialization Configuration
 *  				Flags Invalid.
 *  	- E_RTC_CHANNEL_BUSY:	RTC Channel Busy.
 *
 *  Deinitializes a RTC Channel.
 *
 *  The function <tt>EE_Rtc_DeInit()</tt> shall deinitialize the hardware used
 *  by the RTC Channel, referenced by <tt>Channel</tt>,according to a
 *  configuration flags referenced by <tt>Flags</tt>. Values of registers which
 *  are not writeable are excluded. It's the responsibility of the hardware
 *  design that the state does not lead to undefined activities in the uC.
 *
 *  The function <tt>EE_Rtc_DeInit()</tt> shall disable all interrupt
 *  notifications, controlled by the RTC Channel.
 *
 *  The function <tt>EE_Rtc_DeInit()</tt> shall influence only the peripherals,
 *  which are associated to the referenced Channel.
 *
 *  The function <tt>EE_Rtc_DeInit()</tt> shall set the operation mode of the
 *  RTC Channel to "uninitialized".
 *
 *  If Extended Status for the RTC module is enabled: If the RTC Channel is in
 *  operation mode "uninitialized", the function <tt>EE_Rtc_DeInit()</tt> shall
 *  return the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the RTC module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Rtc_DeInit()</tt> shall return the
 *  development error <tt>E_RTC_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the RTC module is enabled: If the parameter
 *  <tt>Flags</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Rtc_DeInit()</tt> shall return the
 *  development error <tt>E_RTC_PARAM_FLAGS</tt>.
 *
 *  If Extended Status for the RTC module is enabled: If timer channel is in
 *  state "running", the function <tt>EE_Rtc_DeInit()</tt> shall return the
 *  development error <tt>E_RTC_CHANNEL_BUSY</tt>.
 */
#ifdef	__RTC_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Rtc_DeInit(
  EE_TYPERTCCHANNEL	Channel,
  EE_TYPERTCFLAGS	Flags
);
#endif	/* __RTC_DEINIT_API__ */

#ifdef	__RTC_ENABLENOTIF_API__
/** @brief	Channel Notification Enable
 *  @param[in]	Channel	Numeric identifier of the RTC channel.
 *  @return	Extended Status Only:
 *  	- E_OK:			Notification Enable Operation Success.
 *  	- E_NOT_OK:		RTC Channel NOT initialized.
 *  	- E_RTC_PARAM_CHANNEL:	RTC Channel Invalid.
 *  	- E_RTC_PARAM_CALLBACK:	RTC Channel Notification Call-Back Function
 *  				Pointer is <tt>NULL</tt>.
 *
 *  Enables the interrupt notification for a channel.
 *
 *  The function <tt>EE_Rtc_EnableNotification()</tt> shall enable the interrupt
 *  notification of the referenced channel configured for notification.
 *
 *  The function <tt>EE_Rtc_EnableNotification()</tt> shall be reentrant, if the
 *  timer channels used in concurrent calls are different.
 *
 *  If Extended Status for the RTC module is enabled: If the RTC Channel is in
 *  operation mode "uninitialized", the function shall return the development
 *  error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the RTC module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Rtc_EnableNotification()</tt> shall
 *  return the development error <tt>E_RTC_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the RTC module is enabled: If no valid notification
 *  function is configured, the function <tt>EE_Rtc_EnableNotification()</tt>
 *  shall raise the error <tt>E_RTC_PARAM_CALLBACK</tt>.
 */
#ifdef	__RTC_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Rtc_EnableNotification(
  EE_TYPERTCCHANNEL	Channel
);

#ifdef	__RTC_DISABLENOTIF_API__
/** @brief	Channel Notification Disable
 *  @param[in]	Channel	Numeric identifier of the RTC channel.
 *  @return	Extended Status Only:
 *  	- E_OK:			Notification Disable Operation Success.
 *  	- E_NOT_OK:		RTC Channel NOT initialized.
 *  	- E_RTC_PARAM_CHANNEL:	RTC Channel Invalid.
 *  	- E_RTC_PARAM_CALLBACK:	RTC Channel Notification Call-Back Function
 *  				Pointer is <tt>NULL</tt>.
 *
 *  Disables the interrupt notification for a channel.
 *
 *  The function <tt>EE_Rtc_DisableNotification()</tt> shall disable the
 *  interrupt notification of the referenced channel configured for
 *  notification.
 *
 *  The function <tt>EE_Rtc_DisableNotification()</tt> shall be reentrant, if
 *  the timer channels used in concurrent calls are different.
 *
 *  If Extended Status for the RTC module is enabled: If the RTC Channel is in
 *  operation mode "uninitialized", the function shall return the development
 *  error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the RTC module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Rtc_DisableNotification()</tt> shall
 *  return the development error <tt>E_RTC_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the RTC module is enabled: If no valid notification
 *  function is configured, the function <tt>EE_Rtc_DisableNotification()</tt>
 *  shall raise the error <tt>E_RTC_PARAM_CALLBACK</tt>.
 */
#ifdef	__RTC_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Rtc_DisableNotification(
  EE_TYPERTCCHANNEL	Channel
);
#endif	/* __RTC_DISABLENOTIF_API__ */
#endif	/* __RTC_ENABLENOTIF_API__ */

/** @brief	RTC Start.
 *  @param[in]	Channel	Numeric identifier of the RTC channel
 *  @param[in]	Counter	Target time period in number of ticks.
 *  @return	Extended Status Only:
 *  	- E_OK:			Start Operation Success.
 *  	- E_NOT_OK:		RTC Channel NOT initialized.
 *  	- E_RTC_PARAM_CHANNEL:	RTC Channel Invalid.
 *  	- E_RTC_PARAM_COUNTER:	Target Time Period Invalid.
 *  	- E_RTC_CHANNEL_BUSY:	RTC Channel Busy.
 *
 *  Starts a RTC channel.
 *
 *  The function <tt>EE_Rtc_Start()</tt> shall start the selected RTC channel
 *  with a defined target time period.
 *
 *  If configured and enabled, an interrupt notification occurs, when the target
 *  time is reached.
 *
 *  The function <tt>EE_Rtc_Start()</tt> shall be reentrant, if the RTC channels
 *  used in concurrent calls are different.
 *
 *  The state of the selected RTC channel shall be changed to "running" if
 *  <tt>EE_Rtc_Start()</tt> is called.
 *
 *  If Extended Status for the RTC module is enabled: If the RTC Channel is in
 *  operation mode "uninitialized", the function <tt>EE_Rtc_Start()</tt> shall
 *  return the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the RTC module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Rtc_Start()</tt> shall return the
 *  development error <tt>E_RTC_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the RTC module is enabled: The function
 *  <tt>EE_Rtc_Start()</tt> shall return the development error
 *  <tt>E_RTC_PARAM_COUNTER</tt> if the parameter <tt>Counter</tt> is "0" or not
 *  within the allowed range (exceeding the maximum timer resolution).
 *
 *  If Extended Status for the RTC module is enabled: If RTC channel is in
 *  state "running", the function <tt>EE_Rtc_Start()</tt> shall return the
 *  development error <tt>E_RTC_CHANNEL_BUSY</tt>.
 */
#ifdef	__RTC_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Rtc_Start(
  EE_TYPERTCCHANNEL	Channel,
  EE_TYPERTCCOUNTER	Counter
);

#ifdef	__RTC_STOP_API__
/** @brief	RTC Stop.
 *  @param[in]	Channel	Numeric identifier of the RTC channel
 *  @return	Extended Status Only:
 *  	- E_OK:			Stop Operation Success.
 *  	- E_NOT_OK:		RTC Channel NOT initialized.
 *  	- E_RTC_PARAM_CHANNEL:	RTC Channel Invalid.
 *
 *  Stops a timer channel.
 *
 *  The function <tt>EE_Rtc_Stop()</tt> shall stop the selected RTC channel.
 *
 *  The state of the selected RTC channel shall be changed to "stopped" if
 *  <tt>EE_Rtc_Stop()</tt> is called.
 *
 *  If Extended Status for the RTC module is enabled: If the function
 *  <tt>EE_Rtc_Stop()</tt> is called on a channel in state "initialized" or
 *  "stopped", the function shall not return a development error.
 *
 *  If the function <tt>EE_Rtc_Stop()</tt> is called on a channel in state
 *  "initialized" or "stopped", the function shall leave without any
 *  action (no change of the channel state).
 *
 *  The function <tt>EE_Rtc_Stop()</tt> shall be reentrant, if the RTC channel
 *  used in concurrent calls are different.
 *
 *  If Extended Status for the RTC module is enabled: If the RTC Channel is in
 *  operation mode "uninitialized", the function <tt>EE_Rtc_Stop()</tt>
 *  shall return the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the RTC module is enabled: If the parameter Channel
 *  is invalid (not within the range specified by configuration), the function
 *  <tt>EE_Rtc_Stop()</tt> shall raise the error
 *  <tt>E_RTC_PARAM_CHANNEL</tt>.
 */
#ifdef	__RTC_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Rtc_Stop(
  EE_TYPERTCCHANNEL	Channel
);
#endif	/* __RTC_STOPTIMER_API__ */

#ifdef	__RTC_GETCOUNTER_API__
/** @brief	Time Counter Retrieval.
 *  @param[in]	Channel	Numeric Identifier of the RTC channel.
 *  @param[out]	Counter	Time Counter Value (in number of ticks) to Retrieve.
 *  @return	Extended Status Only:
 *  	- E_OK:			Retrieval Operation Success.
 *  	- E_NOT_OK:		RTC Channel NOT initialized.
 *  	- E_RTC_PARAM_CHANNEL:	RTC Channel Invalid.
 *  	- E_RTC_PARAM_COUNTER:	Counter Pointer is <tt>NULL</tt>.
 *
 *  The service returns the time already elapsed.
 *
 *  The function <tt>EE_Rtc_GetCounter()</tt> shall retrieve the time already
 *  elapsed.
 *
 *  The retrieved value of <tt>EE_Rtc_GetCounter()</tt> is the value relative to
 *  the last recurrence (period time reached) or to the start of the channel
 *  before the first recurrence occurs.
 *
 *  If the function <tt>EE_Rtc_GetCounter()</tt> is called on a RTC channel in
 *  state "initialized" (channel started never before), the function shall
 *  retrieve the value "0".
 *
 *  If the function <tt>EE_Rtc_GetCounter()</tt> is called on a RTC channel in
 *  state "stopped", the function shall return the time value at the moment of
 *  stopping.
 *
 *  The function <tt>EE_Rtc_GetCounter()</tt> shall be reentrant, if the RTC
 *  channels used in concurrent calls are different.
 *
 *  If Extended Status for the RTC module is enabled: If the Channel is not
 *  initialized, the function <tt>EE_Rtc_GetCounter()</tt> shall return the
 *  development error <tt>E_NOT_OK</tt> and shall retrieve the value "0".
 *
 *  If Extended Status for the RTC module is enabled: If the parameter Channel
 *  is invalid (not within the range specified by configuration), the function
 *  shall return the development error <tt>E_RTC_PARAM_CHANNEL</tt> and shall
 *  retrieve the value "0".
 *
 *  If Extended Status for the RTC module is enabled: If the parameter Counter
 *  is <tt>EE_NULL_PTR</tt>, the function shall return the development error
 *  <tt>E_RTC_PARAM_COUNTER</tt>.
 */
#ifdef	__RTC_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Rtc_GetCounter(
  EE_TYPERTCCHANNEL	Channel,
  EE_TYPERTCCOUNTER	*Counter
);
#endif	/* __RTC_GETTIMEELAPSED_API__ */

#ifdef	__RTC_SETCOUNTER_API__
/** @brief	Time Counter Setup.
 *  @param[in]	Channel	Numeric identifier of the RTC channel.
 *  @param[out]	Counter	Time Counter Value (in number of ticks) to Setup.
 *  @return	Extended Status Only:
 *  	- E_OK:			Setup Operation Success.
 *  	- E_NOT_OK:		RTC Channel NOT initialized.
 *  	- E_RTC_PARAM_CHANNEL:	RTC Channel Invalid.
 *  	- E_RTC_PARAM_COUNTER:	Time Counter Invalid.
 *
 *  The service sets the time counter of the RTC Channel.
 *
 *  The function <tt>EE_Rtc_SetCounter()</tt> shall set the time counter of the
 *  RTC channel.
 *
 *  The function <tt>EE_Rtc_SetCounter()</tt> shall be reentrant, if the RTC
 *  channels used in concurrent calls are different.
 *
 *  If Extended Status for the RTC module is enabled: If the Channel is not
 *  initialized, the function <tt>EE_Rtc_SetCounter()</tt> shall return
 *  the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the RTC module is enabled: If the parameter Channel
 *  is invalid (not within the range specified by configuration), the function
 *  shall return the development error <tt>E_RTC_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the RTC module is enabled: If the parameter Counter
 *  is Invalid, the function shall return the development error
 *  <tt>E_RTC_PARAM_COUNTER</tt>.
 */
#ifdef	__RTC_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Rtc_SetCounter(
  EE_TYPERTCCHANNEL	Channel,
  EE_TYPERTCCOUNTER	Counter
);
#endif	/* __RTC_SETCOUNTER_API__ */

#endif	/* __EE_MCU_COMMON_RTC_H__ */
