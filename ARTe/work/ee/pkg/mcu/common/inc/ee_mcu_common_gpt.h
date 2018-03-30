/** @file	ee_mcu_common_gpt.h
 *
 *  @brief	Erika Enterprise - GPT Driver - MCU Common Header File.
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
 *  The GPT driver provides General Porpose Times(s) services.
 *
 *  The GPT driver is part of the microcontroller abstraction layer (MCAL). It
 *  initializes and controls the internal General Purpose Timer(s) (GPT) of the
 *  microcontroller.
 *
 *  The GPT driver provides services and configuration parameters for
 *  - Starting and stopping hardware timers
 *  - Getting timer values
 *  - Controlling time triggered interrupt notifications
 *
 *  The tick duration of a timer channel depends on channel specific settings
 *  (part of GPT driver) as well as on system clock and settings of the clock
 *  tree controlled by the MCU module. The tick duration is not limited by this
 *  specification.
 *
 *  Not all hardware timers must be controlled by the GPT module. Some timers
 *  may be controlled by Operating System (OS) or Other Drivers directly.
 *
 *  The number of timer channels controlled by the GPT driver depends on
 *  hardware, implementation and system configuration.
 *
 *  The GPT driver only generates time bases, and does not serve as an event
 *  counter. This functionality is provided by another driver module
 * (ICU driver).
 * 
 *  @author	Riccardo Schiavi
 *  @author	Giuseppe Serano
 *  @version	0.1
 */

#ifndef	__EE_MCU_COMMON_GPT_H__
#define	__EE_MCU_COMMON_GPT_H__

#ifdef	__GPT_GETSTATUS_API__
/** @brief	GPT Status Type
 *
 *  The type <tt>EE_TYPEGPTCHSTAT</tt> is the type of the return value of the
 *  function <tt>EE_Gpt_GetStatus()</tt>.
 *
 *  The type of <tt>EE_TYPEGPTCHSTAT</tt> is an enumeration describing the
 *  operation states for a GPT channel.
 */
typedef enum {
  EE_GPT_CH_UNINIT,		/**< "Un-Initialized" State.		      */
  EE_GPT_CH_OPERATIONAL,	/**< "Initialized" State. Normal operation.   */
#ifdef	__GPT_ONESHOT__
  EE_GPT_CH_EXPIRED,		/**< "Expired" State.			      */
#endif	/* __GPT_ONESHOT__ */
  EE_GPT_CH_STOPPED,		/**< "Stopped" State.			      */
  EE_GPT_CH_RUNNING,		/**< "Running" State.			      */

} EE_TYPEGPTCHSTAT;
#endif	/* __GPT_GETSTATUS_API__ */

#ifdef	__GPT_EXTENDED_STATUS__
/** @brief	Invalid Channel.		*/
#define	E_GPT_PARAM_CHANNEL	0xFE

/** @brief	Invalid Configuration Flags.	*/
#define	E_GPT_PARAM_FLAGS	0xFD

/** @brief	Invalid Counter.		*/
#define	E_GPT_PARAM_COUNTER	0xFC

/** @brief	Notification Call-Back Function Pointer is NULL. */
#define	E_GPT_PARAM_CALLBACK	0xFB

/** @brief	Busy Channel.			*/
#define	E_GPT_CHANNEL_BUSY	0xFA
#endif	/* __GPT_EXTENDED_STATUS__ */

/** @brief	Numeric ID of a GPT channel.
 *
 *  Numeric ID of a GPT channel.
 *  Implementation specific.
 *  But not all values may be valid within this type.
 *  This type shall be chosen in order to have the most efficient implementation
 *  on a specific micro controller platform.
 *
 *  Parameters of type <tt>EE_TYPEGPTCHANNEL</tt> contain the numeric ID of a
 *  GPT channel.
 *
 *  The mapping of the ID is implementation specific but not configurable and
 *  not all values may be valid within this type.
 *
 *  For parameter values of type <tt>EE_TYPEGPTCHANNEL</tt>, the Gpt's user
 *  shall use the symbolic names provided by the driver implementation.
 *
 *  The type shall be EE_UREG depending on uC platform.
 */
#ifndef	EE_TYPEGPTCHANNEL
#define	EE_TYPEGPTCHANNEL	EE_UREG
#endif

/** @brief	GPT Counter Type
 *
 *  Type for reading and setting the timer values (in number of ticks).
 *  The range of this type is uC dependent (width of the timer register) and has
 *  to be described by the supplier.
 */
#ifndef	EE_TYPEGPTCOUNTER
#define	EE_TYPEGPTCOUNTER	EE_UREG
#endif

/** @brief	GPT Configuration Flags.
 *
 *  Type for setting-up a GPT Channel Configuration.
 */
#ifndef	EE_TYPEGPTFLAGS
#define	EE_TYPEGPTFLAGS		EE_UREG
#endif

/** @brief	GPT Default Flag.
 *
 *  @note	GPT Default Flag implies:
 *  		- GPT Continuous Flag.
 */
#define	EE_GPT_FLAG_DEFAULT	0x00U

#ifdef	__GPT_ONESHOT__
/** @brief	GPT Channel Continuous Mode Flag. */
#define	EE_GPT_FLAG_CONTINUOUS	EE_GPT_FLAG_DEFAULT

/** @brief	GPT Channel One-Shot Mode Flag. */
#define	EE_GPT_FLAG_ONESHOT	0x01U
#endif	/* __GPT_ONESHOT__ */

#ifdef	__GPT_GETSTATUS_API__
/** @brief	GPT Channel Status Retrieval.
 *  @param[in]	Channel	Numeric identifier of the GPT channel.
 *  @return	Always:
 *  	- EE_GPT_CH_UNINIT:		"Un-Initialized" State.
 *  	- EE_GPT_CH_OPERATIONAL:	Normal operation. "Initialized" State.
 *  	- EE_GPT_CH_EXPIRED:		Expired" State.
 *  	- EE_GPT_CH_STOPPED:		"Stopped" State.
 *  	- EE_GPT_CH_RUNNING:		"Running" State.
 *
 *  The service gets the status of the specified GPT Channel.
 *
 *  The function <tt>EE_Gpt_GetStatus()</tt> shall return the current
 *  operational status of the GPT Channel.
 */
EE_TYPEGPTCHSTAT
EE_Gpt_GetStatus(
  EE_TYPEGPTCHANNEL	Channel
);
#endif	/* __GPT_GETSTATUS_API__ */

/** @brief	GPT Channel Initialization.
 *  @param[in]	Channel	GPT Channel to be Initialized.
 *  @param[in]	Flags	GPT Channel Configuration Flags.
 *  @param[in]	NotifCB	GPT Channel Notification Call-Back Function Pointer.
 *  @return	Extended Status Only:
 *  	- E_OK:			GPT Channel initializion success.
 *  	- E_NOT_OK:		GPT Channel already initialized.
 *  	- E_GPT_PARAM_CHANNEL:	GPT Channel Invalid.
 *  	- E_GPT_PARAM_FLAG:	GPT Channel Configuration Flags Invalid.
 *
 *  This service initializes a GPT Channel.
 *
 *  The function <tt>EE_Gpt_Init()</tt> shall initialize the hardware timer
 *  channel, referenced by <tt>Channel</tt>, according to configuration flags
 *  referenced by <tt>Flags</tt>.
 *
 *  The function <tt>EE_Gpt_Init()</tt> shall disable all interrupt
 *  notifications, controlled by the GPT Channel.
 *
 *  The function <tt>EE_Gpt_Init()</tt> shall only initialize the referenced
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
 *  If Extended Status for the GPT module is enabled: If the GPT Channel is not
 *  in operation mode "uninitialized", the function <tt>EE_Gpt_Init()</tt> shall
 *  return the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the GPT module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Gpt_Init()</tt> shall return the
 *  development error <tt>E_GPT_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the GPT module is enabled: If the parameter
 *  <tt>Flags</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Gpt_Init()</tt> shall return the
 *  development error <tt>E_GPT_PARAM_FLAGS</tt>.
 *
 *  A re-initialization of the GPT channel by executing the
 *  <tt>EE_Gpt_Init()</tt> function requires a de-initialization before by
 *  executing a <tt>EE_Gpt_DeInit()</tt>.
 */
#ifdef	__GPT_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpt_Init(
  EE_TYPEGPTCHANNEL	Channel,
  EE_TYPEGPTFLAGS	Flags,
  EE_ISR_callback	NotifCB
);

#ifdef	__GPT_DEINIT_API__
/** @brief	GPT Channel De-Initialization.
 *  @param[in]	Channel	GPT Channel to be De-Initialized.
 *  @param[in]	Flags	GPT Channel De-initialization Configuration Flags.
 *  @return	Extended Status Only:
 *  	- E_OK:			GPT Channel initializion success.
 *  	- E_NOT_OK:		GPT Channel NOT initialized.
 *  	- E_GPT_PARAM_CHANNEL:	GPT Channel Invalid.
 *  	- E_GPT_PARAM_FLAG:	GPT Channel De-Initialization Configuration
 *  				Flags Invalid.
 *  	- E_GPT_CHANNEL_BUSY:	GPT Channel Busy.
 *
 *  Deinitializes a GPT Channel.
 *
 *  The function <tt>EE_Gpt_DeInit()</tt> shall deinitialize the hardware used
 *  by the GPT Channel, referenced by <tt>Channel</tt>,according to a
 *  configuration flags referenced by <tt>Flags</tt>. Values of registers which
 *  are not writeable are excluded. It's the responsibility of the hardware
 *  design that the state does not lead to undefined activities in the uC.
 *
 *  The function <tt>EE_Gpt_DeInit()</tt> shall disable all interrupt
 *  notifications, controlled by the GPT Channel.
 *
 *  The function <tt>EE_Gpt_DeInit()</tt> shall influence only the peripherals,
 *  which are associated to the referenced Channel.
 *
 *  The function <tt>EE_Gpt_DeInit()</tt> shall set the operation mode of the
 *  GPT Channel to "uninitialized".
 *
 *  If Extended Status for the GPT module is enabled: If the GPT Channel is in
 *  operation mode "uninitialized", the function <tt>EE_Gpt_DeInit()</tt> shall
 *  return the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the GPT module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Gpt_DeInit()</tt> shall return the
 *  development error <tt>E_GPT_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the GPT module is enabled: If the parameter
 *  <tt>Flags</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Gpt_DeInit()</tt> shall return the
 *  development error <tt>E_GPT_PARAM_FLAGS</tt>.
 *
 *  If Extended Status for the GPT module is enabled: If timer channel is in
 *  state "running", the function <tt>EE_Gpt_DeInit()</tt> shall return the
 *  development error <tt>E_GPT_CHANNEL_BUSY</tt>.
 */
#ifdef	__GPT_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpt_DeInit(
  EE_TYPEGPTCHANNEL	Channel,
  EE_TYPEGPTFLAGS	Flags
);
#endif	/* __GPT_DEINIT_API__ */

#ifdef	__GPT_ENABLENOTIF_API__
/** @brief	Channel Notification Enable
 *  @param[in]	Channel	Numeric identifier of the GPT channel.
 *  @return	Extended Status Only:
 *  	- E_OK:			Notification Enable Operation Success.
 *  	- E_NOT_OK:		GPT Channel NOT initialized.
 *  	- E_GPT_PARAM_CHANNEL:	GPT Channel Invalid.
 *  	- E_GPT_PARAM_CALLBACK:	GPT Channel Notification Call-Back Function
 *  				Pointer is <tt>NULL</tt>.
 *
 *  Enables the interrupt notification for a channel.
 *
 *  The function <tt>EE_Gpt_EnableNotification()</tt> shall enable the interrupt
 *  notification of the referenced channel configured for notification.
 *
 *  The function <tt>EE_Gpt_EnableNotification()</tt> shall be reentrant, if the
 *  timer channels used in concurrent calls are different.
 *
 *  If Extended Status for the GPT module is enabled: If the GPT Channel is in
 *  operation mode "uninitialized", the function shall return the development
 *  error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the GPT module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Gpt_EnableNotification()</tt> shall
 *  return the development error <tt>E_GPT_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the GPT module is enabled: If no valid notification
 *  function is configured, the function <tt>EE_Gpt_EnableNotification()</tt>
 *  shall raise the error <tt>E_GPT_PARAM_CALLBACK</tt>.
 */
#ifdef	__GPT_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpt_EnableNotification(
  EE_TYPEGPTCHANNEL	Channel
);

#ifdef	__GPT_DISABLENOTIF_API__
/** @brief	Channel Notification Disable
 *  @param[in]	Channel	Numeric identifier of the GPT channel.
 *  @return	Extended Status Only:
 *  	- E_OK:			Notification Disable Operation Success.
 *  	- E_NOT_OK:		GPT Channel NOT initialized.
 *  	- E_GPT_PARAM_CHANNEL:	GPT Channel Invalid.
 *  	- E_GPT_PARAM_CALLBACK:	GPT Channel Notification Call-Back Function
 *  				Pointer is <tt>NULL</tt>.
 *
 *  Disables the interrupt notification for a channel.
 *
 *  The function <tt>EE_Gpt_DisableNotification()</tt> shall disable the
 *  interrupt notification of the referenced channel configured for
 *  notification.
 *
 *  The function <tt>EE_Gpt_DisableNotification()</tt> shall be reentrant, if
 *  the timer channels used in concurrent calls are different.
 *
 *  If Extended Status for the GPT module is enabled: If the GPT Channel is in
 *  operation mode "uninitialized", the function shall return the development
 *  error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the GPT module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Gpt_DisableNotification()</tt> shall
 *  return the development error <tt>E_GPT_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the GPT module is enabled: If no valid notification
 *  function is configured, the function <tt>EE_Gpt_DisableNotification()</tt>
 *  shall raise the error <tt>E_GPT_PARAM_CALLBACK</tt>.
 */
#ifdef	__GPT_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpt_DisableNotification(
  EE_TYPEGPTCHANNEL	Channel
);
#endif	/* __GPT_DISABLENOTIF_API__ */
#endif	/* __GPT_ENABLENOTIF_API__ */

/** @brief	Timer Start.
 *  @param[in]	Channel	Numeric identifier of the GPT channel
 *  @param[in]	Counter	Target time in number of ticks.
 *  @return	Extended Status Only:
 *  	- E_OK:			Timer Start Operation Success.
 *  	- E_NOT_OK:		GPT Channel NOT initialized.
 *  	- E_GPT_PARAM_CHANNEL:	GPT Channel Invalid.
 *  	- E_GPT_PARAM_COUNTER:	Target Time Invalid.
 *  	- E_GPT_CHANNEL_BUSY:	GPT Channel Busy.
 *
 *  Starts a timer channel.
 *
 *  The function <tt>EE_Gpt_StartTimer()</tt> shall start the selected timer
 *  channel with a defined target time.
 *
 *  If configured and enabled, an interrupt notification occurs, when the target
 *  time is reached.
 *
 *  The function <tt>EE_Gpt_StartTimer()</tt> shall be reentrant, if the timer
 *  channels used in concurrent calls are different.
 *
 *  The state of the selected timer channel shall be changed to "running" if
 *  <tt>EE_Gpt_StartTimer()</tt> is called.
 *
 *  If Extended Status for the GPT module is enabled: If the GPT Channel is in
 *  operation mode "uninitialized", the function <tt>EE_Gpt_StartTimer()</tt>
 *  shall return the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the GPT module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Gpt_StartTimer()</tt> shall return the
 *  development error <tt>E_GPT_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the GPT module is enabled: The function
 *  <tt>EE_Gpt_StartTimer()</tt> shall return the development error
 *  <tt>E_GPT_PARAM_COUNTER</tt> if the parameter <tt>Counter</tt> is "0" or not
 *  within the allowed range (exceeding the maximum timer resolution).
 *
 *  If Extended Status for the GPT module is enabled: If timer channel is in
 *  state "running", the function <tt>EE_Gpt_StartTimer()</tt> shall return the
 *  development error <tt>E_GPT_CHANNEL_BUSY</tt>.
 */
#ifdef	__GPT_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpt_StartTimer(
  EE_TYPEGPTCHANNEL	Channel,
  EE_TYPEGPTCOUNTER	Counter
);

#ifdef	__GPT_STOPTIMER_API__
/** @brief	Timer Stop.
 *  @param[in]	Channel	Numeric identifier of the GPT channel
 *  @return	Extended Status Only:
 *  	- E_OK:			Timer Stop Operation Success.
 *  	- E_NOT_OK:		GPT Channel NOT initialized.
 *  	- E_GPT_PARAM_CHANNEL:	GPT Channel Invalid.
 *
 *  Stops a timer channel.
 *
 *  The function <tt>EE_Gpt_StopTimer()</tt> shall stop the selected timer
 *  channel.
 *
 *  The state of the selected timer channel shall be changed to "stopped" if
 *  <tt>EE_Gpt_StopTimer()</tt> is called.
 *
 *  If Extended Status for the GPT module is enabled: If the function
 *  <tt>EE_Gpt_StopTimer()</tt> is called on a channel in state "initialized",
 *  "stopped" or "expired", the function shall not return a development error.
 *
 *  If the function <tt>EE_Gpt_StopTimer()</tt> is called on a channel in state
 *  "initialized", "stopped" or "expired", the function shall leave without any
 *  action (no change of the channel state).
 *
 *  The function <tt>EE_Gpt_StopTimer()</tt> shall be reentrant, if the timer
 *  channels used in concurrent calls are different.
 *
 *  If Extended Status for the GPT module is enabled: If the GPT Channel is in
 *  operation mode "uninitialized", the function <tt>EE_Gpt_StartTimer()</tt>
 *  shall return the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the GPT module is enabled: If the parameter Channel
 *  is invalid (not within the range specified by configuration), the function
 *  <tt>EE_Gpt_StopTimer()</tt> shall raise the error
 *  <tt>E_GPT_PARAM_CHANNEL</tt>.
 */
#ifdef	__GPT_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpt_StopTimer(
  EE_TYPEGPTCHANNEL	Channel
);
#endif	/* __GPT_STOPTIMER_API__ */

#ifdef	__GPT_GETTIMEELAPSED_API__
/** @brief	Elapsed Time Retrieval.
 *  @param[in]	Channel	Numeric Identifier of the GPT channel.
 *  @param[out]	Counter	Elapsed Time Value (in number of ticks) to Retrieve.
 *  @return	Extended Status Only:
 *  	- E_OK:			Elapsed Time Retrieval Operation Success.
 *  	- E_NOT_OK:		GPT Channel NOT initialized.
 *  	- E_GPT_PARAM_CHANNEL:	GPT Channel Invalid.
 *  	- E_GPT_PARAM_COUNTER:	Counter Pointer is <tt>NULL</tt>.
 *
 *  The service returns the time already elapsed.
 *
 *  The function <tt>EE_Gpt_GetTimeElapsed()</tt> shall retrieve the time
 *  already elapsed.
 *
 *  When the Channel is in mode "one-shot mode", this is the value relative to
 *  the point in time, the channel has been started.
 *
 *  When the channel is in mode "continuous mode", the retrieved value of
 *  <tt>EE_Gpt_GetTimeElapsed()</tt> is the value relative to the last
 *  recurrence (target time reached) or to the start of the channel before the
 *  first recurrence occurs.
 *
 *  If the function <tt>EE_Gpt_GetTimeElapsed()</tt> is called on a timer
 *  channel in state "initialized" (channel started never before), the function
 *  shall retrieve the value "0".
 *
 *  If the function <tt>EE_Gpt_GetTimeElapsed()</tt> is called on a timer
 *  channel in state "stopped", the function shall return the time value at the
 *  moment of stopping.
 *
 *  If the function <tt>EE_Gpt_GetTimeElapsed()</tt> is called on a channel
 *  configured for "one-shot mode" in state "expired" (timer has reached the
 *  target time), the function shall return the target time.
 *
 *  The function <tt>EE_Gpt_GetTimeElapsed()</tt> shall be reentrant, if the
 *  timer channels used in concurrent calls are different.
 *
 *  If Extended Status for the GPT module is enabled: If the Channel is not
 *  initialized, the function <tt>EE_Gpt_GetTimeElapsed()</tt> shall return the
 *  development error <tt>E_NOT_OK</tt> and shall retrieve the value "0".
 *
 *  If Extended Status for the GPT module is enabled: If the parameter Channel
 *  is invalid (not within the range specified by configuration), the function
 *  shall return the development error <tt>E_GPT_PARAM_CHANNEL</tt> and shall
 *  retrieve the value "0".
 *
 *  If Extended Status for the GPT module is enabled: If the parameter Counter
 *  is <tt>EE_NULL_PTR</tt>, the function shall return the development error
 *  <tt>E_GPT_PARAM_COUNTER</tt>.
 */
#ifdef	__GPT_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpt_GetTimeElapsed(
  EE_TYPEGPTCHANNEL	Channel,
  EE_TYPEGPTCOUNTER	*Counter
);
#endif	/* __GPT_GETTIMEELAPSED_API__ */

#ifdef	__GPT_GETTIMEREMAINING_API__
/** @brief	Time Remaining Retrieval.
 *  @param[in]	Channel	Numeric identifier of the GPT channel.
 *  @param[out]	Counter	Remaining Time Value (in number of ticks) to Retrieve.
 *  @return	Extended Status Only:
 *  	- E_OK:			Remaining Time Retrieval Operation Success.
 *  	- E_NOT_OK:		GPT Channel NOT initialized.
 *  	- E_GPT_PARAM_CHANNEL:	GPT Channel Invalid.
 *  	- E_GPT_PARAM_COUNTER:	Counter Pointer is <tt>NULL</tt>.
 *
 *  The service returns the time remaining until the target time is reached.
 *
 *  The function <tt>EE_Gpt_GetTimeRemaining()</tt> shall retrieve the timer
 *  value remaining until the target time will be reached next time.
 *
 *  The remaining time is the "target time" minus the time already elapsed.
 *
 *  If the function <tt>EE_Gpt_GetTimeRemaining()</tt> is called on a timer
 *  channel in state "initialized" (channel started never before), the function
 *  shall retrive the value "0".
 *
 *  If the function <tt>EE_Gpt_GetTimeRemaining()</tt> is called on a timer
 *  channel in state "stopped", the function shall retrieve the remaining time
 *  value at the moment of stopping.
 *
 *  If the function <tt>EE_Gpt_GetTimeRemaining()</tt> is called on a channel
 *  configured for "one-shot mode" in state "expired" (timer has reached the
 *  target time), the function shall retrieve the value "0".
 *
 *  The function <tt>EE_Gpt_GetTimeRemaining()</tt> shall be reentrant, if the
 *  timer channels used in concurrent calls are different.
 *
 *  If Extended Status for the GPT module is enabled: If the Channel is not
 *  initialized, the function <tt>EE_Gpt_GetTimeRemaining()</tt> shall return
 *  the development error <tt>E_NOT_OK</tt> and shall retrieve the value "0".
 *
 *  If Extended Status for the GPT module is enabled: If the parameter Channel
 *  is invalid (not within the range specified by configuration), the function
 *  shall return the development error <tt>E_GPT_PARAM_CHANNEL</tt> and shall
 *  retrieve the value "0".
 *
 *  If Extended Status for the GPT module is enabled: If the parameter Counter
 *  is <tt>EE_NULL_PTR</tt>, the function shall return the development error
 *  <tt>E_GPT_PARAM_COUNTER</tt>.
 */
#ifdef	__GPT_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpt_GetTimeRemaining(
  EE_TYPEGPTCHANNEL	Channel,
  EE_TYPEGPTCOUNTER	*Counter
);
#endif	/* __GPT_GETTIMEREMAINING_API__ */

#endif	/* __EE_MCU_COMMON_GPT_H__ */
