/** @file	ee_mcu_common_pwm.h
 *
 *  @brief	Erika Enterprise - PWM Driver - MCU Common Header File.
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
 *  The PWM driver provides Pulse-Width Modulation(s) services.
 *
 *  The PWM driver is part of the microcontroller abstraction layer (MCAL). It
 *  initializes and controls the internal Pulse-Width Modulator(s) (PWM) of the
 *  microcontroller.
 *
 *  Each PWM channel is linked to a hardware PWM which belongs to the
 *  microcontroller.
 *
 *  The type of the PWM signal (for example center Align, left Align, Etc...) is
 *  not defined within this specification and is left up to the implementation. 
 *
 *  The driver provides functions for initialization and control of the
 *  microcontroller internal PWM stage (pulse width modulation).
 *
 *  The PWM module generates pulses with variable pulse width. It allows the
 *  selection ofthe duty cycle and the signal period time.
 *
 *  The PWM depends on the system clock. Thus, changes of the system clock (e.g.
 *  PLL on -> PLL off) also affect the clock settings of the PWM hardware.
 *
 *  All functions from the PWM module except <tt>EE_Pwm_Init()</tt> and
 *  <tt>Pwm_DeInit</tt> shall be re-entrant for different PWM channel numbers.
 *
 *  The PWM module's user shall ensure the integrity if several function calls
 *  are made during run time in different TASKs or ISRs for the same PWM
 *  channel.
 *
 *  To get times out of register values it is necessary to know the oscillator
 *  frequency, prescalers and so on. Since these settings are made in MCU and/or
 *  in other modules it is not possible to calculate such times. Hence the
 *  conversions between time and ticks shall be part of an upper layer.
 *
 *  All time units used within the API services of the PWM module shall be of
 *  the unit ticks.
 * 
 *  @author	Riccardo Schiavi
 *  @author	Giuseppe Serano
 *  @version	0.1
 */

#ifndef	__EE_MCU_COMMON_PWM_H__
#define	__EE_MCU_COMMON_PWM_H__

#ifdef	__PWM_ENABLENOTIF_API__
/** @brief	PWM Edge Notification Type
 *
 *  Definition of the type of edge notification of a PWM channel.
 */
typedef enum {

  /** @brief	Rising Edge.
   *
   *  Notification will be called when a rising edge occurs on the PWM output
   *  signal.
   */
  EE_PWM_RISING_EDGE,

  /** @brief	Falling Edge.
   *
   *  Notification will be called when a falling edge occurs on the PWM output
   *  signal.
   */
  EE_PWM_FALLING_EDGE,

  /** @brief	Both Edges.
   *
   *  Notification will be called when either a rising edge or falling edge
   *  occurs on the PWM output signal.
   */
  EE_PWM_BOTH_EDGES

} EE_TYPEPWMEDGENOTIF;
#endif	/* __PWM_ENABLENOTIF_API__ */

#ifdef	__PWM_GETSTATUS_API__
/** @brief	PWM Status Type
 *
 *  The type <tt>EE_TYPEPWMCHSTAT</tt> is the type of the return value of the
 *  function <tt>EE_Pwm_GetStatus()</tt>.
 *
 *  The type of <tt>EE_TYPEPWMCHSTAT</tt> is an enumeration describing the
 *  operation states for a PWM channel.
 */
typedef enum {
  EE_PWM_CH_UNINIT,		/**< "Un-Initialized" State.		      */
  EE_PWM_CH_OPERATIONAL,	/**< "Initialized" State. Normal operation.   */
  EE_PWM_CH_IDLE,		/**< "Idle" State.			      */
  EE_PWM_CH_RUNNING,		/**< "Running" State.			      */

} EE_TYPEPWMCHSTAT;
#endif	/* __PWM_GETSTATUS_API__ */

#ifdef	__PWM_EXTENDED_STATUS__
/** @brief	Invalid Channel.					*/
#define	E_PWM_PARAM_CHANNEL	0xFE

/** @brief	Invalid Configuration Flags.				*/
#define	E_PWM_PARAM_FLAGS	0xFD

/** @brief	Invalid Edge Notification Type.				*/
#define	E_PWM_PARAM_EDGE	0xFC

/** @brief	Invalid Period.						*/
#define	E_PWM_PARAM_PERIOD	0xFB

/** @brief	Invalid Duty-Cycle.					*/
#define	E_PWM_PARAM_DUTY	0xFA

/** @brief	Notification Call-Back Function Pointer is NULL.	*/
#define	E_PWM_PARAM_CALLBACK	0xF9

/** @brief	PWM Signal Output State Pointer is NULL.		*/
#define	E_PWM_PARAM_STATE	0xF8

/** @brief	Busy Channel.						*/
#define	E_PWM_CHANNEL_BUSY	0xF7

/** @brief	Fixed Period.						*/
#define	E_PWM_FIXED_PERIOD	0xF6
#endif	/* __PWM_EXTENDED_STATUS__ */

/** @brief	Numeric ID of a PWM channel.
 *
 *  Numeric ID of a PWM channel.
 *  Implementation specific.
 *  But not all values may be valid within this type.
 *  This type shall be chosen in order to have the most efficient implementation
 *  on a specific micro controller platform.
 *
 *  Parameters of type <tt>EE_TYPEPWMCHANNEL</tt> contain the numeric ID of a
 *  PWM channel.
 *
 *  The mapping of the ID is implementation specific but not configurable and
 *  not all values may be valid within this type.
 *
 *  For parameter values of type <tt>EE_TYPEPWMCHANNEL</tt>, the PWM's user
 *  shall use the symbolic names provided by the driver implementation.
 *
 *  The type shall be EE_UREG depending on uC platform.
 */
#ifndef	EE_TYPEPWMCHANNEL
#define	EE_TYPEPWMCHANNEL	EE_UREG
#endif

/** @brief	PWMConfiguration Flags.
 *
 *  Type for setting-up a PWM Channel Configuration.
 */
#ifndef	EE_TYPEPWMFLAGS
#define	EE_TYPEPWMFLAGS		EE_UREG
#endif

/** @brief	PWM Channel Default Flag.
 *
 *  @note	PWM Default Flag implies:
 *  		- PWM Polarity Low Flag.
 *  		- PWM Idle State Low Flag.
 *  		- PWM Variable Period Flag.
 *  		- PWM Immediately Updated Period Flag.
 *  		- PWM Immediately Updated Duty-Cycle Flag.
 */
#define	EE_PWM_FLAG_DEFAULT	0x00U

/** @brief	PWM Polarity Low Flag.
 *
 *  The PWM channel output is low at the beginning of the cycle and then goes
 *  high when the duty count is reached.
 */
#define	EE_PWM_FLAG_POL_LOW	EE_PWM_FLAG_DEFAULT

/** @brief	PWM Polarity High Flag.
 *
 *  The PWM channel output is high at the beginning of the cycle and then goes
 *  low when the duty count is reached.
 */
#define	EE_PWM_FLAG_POL_HIGH	0x01U

/** @brief	PWM Idle State Low Flag.
 *
 *  The PWM channel output will be set to low ( 0 V ) in idle state.
 */
#define	EE_PWM_FLAG_IDLE_LOW	EE_PWM_FLAG_DEFAULT

/** @brief	PWM Idle State High Flag.
 *
 *  The PWM channel output will be set to high ( 3.3 V or 5 V ) in idle state.
 */
#define	EE_PWM_FLAG_IDLE_HIGH	0x02U

/** @brief	PWM Variable Period Flag.
 *
 *  The PWM channel has a variable period. The duty cycle and the period can be
 *  changed while PWM channel is in "Running" state. 
 */
#define	EE_PWM_FLAG_PER_VAR	EE_PWM_FLAG_DEFAULT

/** @brief	PWM Fixed Period Flag.
 *
 *  The PWM channel has a fixed period. Only the duty cycle can be changed while
 *  PWM channel is in "Running" state.
 */
#define	EE_PWM_FLAG_PER_FIX	0x04U

/** @brief	PWM Immediately Updated Period Flag.
 *
 *  The PWM channel update of period is done immediately (just after service
 *  call, current waveform is cut).
 */
#define	EE_PWM_FLAG_PER_IMM_UP	EE_PWM_FLAG_DEFAULT

/** @brief	PWM End Period Updated Period Flag.
 *
 *  The PWM channel update of period is done at the end of period of currently
 *  generated waveform (current waveform is finished).
 */
#define	EE_PWM_FLAG_PER_END_UP	0x08U

/** @brief	PWM Immediately Updated Duty-Cycle Flag.
 *
 *  The PWM channel update of duty-cycle is done immediately (just after service
 *  call, current waveform is cut).
 */
#define	EE_PWM_FLAG_DUTY_IMM_UP	EE_PWM_FLAG_DEFAULT

/** @brief	PWM End Period Updated Duty-Cycle Flag.
 *
 *  The PWM channel update of duty-cycle is done at the end of period of
 *  currently generated waveform (current waveform is finished)
 */
#define	EE_PWM_FLAG_DUTY_END_UP	0x10U


/** @brief	PWM Period Type
 *
 *  Type for setting the period values (in number of ticks).
 *  The range of this type is uC dependent (width of the pwm register) and has
 *  to be described by the supplier.
 */
#ifndef	EE_TYPEPWMPERIOD
#define	EE_TYPEPWMPERIOD	EE_UREG
#endif

/** @brief	PWM Duty-Cycle Type
 *
 *  Type for setting the duty-cycle values (in number of ticks).
 *  The range of this type is uC dependent (width of the pwm register) and has
 *  to be described by the supplier.
 */
#ifndef	EE_TYPEPWMDUTYCYCLE
#define	EE_TYPEPWMDUTYCYCLE	EE_UREG
#endif

/** @brief	Minimum PWM Duty-Cycle Value: 0% */
#define	EE_PWM_DUTY_CYCLE_MIN	0x00U

/** @brief	Minimum PWM Duty-Cycle Value: 100% */
#define	EE_PWM_DUTY_CYCLE_MAX	0x64U	/* 0x64 = 100 */

#ifdef	__PWM_GETSTATUS_API__
/** @brief	PWM Channel Status Retrieval.
 *  @param[in]	Channel	Numeric identifier of the PWM channel.
 *  @return	Always:
 *  	- EE_PWM_CH_UNINIT:		"Un-Initialized" State.
 *  	- EE_PWM_CH_OPERATIONAL:	"Initialized" State. Normal operation.
 *  	- EE_PWM_CH_IDLE:		"Idle" State.
 *  	- EE_PWM_CH_RUNNING:		"Running" State.
 *
 *  The service gets the status of the specified PWM Channel.
 *
 *  The function <tt>EE_Pwm_GetStatus()</tt> shall return the current
 *  operational status of the PWM Channel.
 */
EE_TYPEPWMCHSTAT
EE_Pwm_GetStatus(
  EE_TYPEPWMCHANNEL	Channel
);
#endif	/* __PWM_GETSTATUS_API__ */

/** @brief	PWM Channel Initialization.
 *  @param[in]	Channel	PWM Channel to be Initialized.
 *  @param[in]	Flags	PWM Channel Configuration Flags.
 *  @param[in]	NotifCB	PWM Channel Notification Call-Back Function Pointer.
 *  @return	Extended Status Only:
 *  	- E_OK:			PWM Channel initializion successul.
 *  	- E_NOT_OK:		PWM Channel already initialized.
 *  	- E_PWM_PARAM_CHANNEL:	PWM Channel Invalid.
 *  	- E_PWM_PARAM_FLAG:	PWM Channel Configuration Flags Invalid.
 *
 *  This service initializes a PWM Channel.
 *
 *  The function <tt>EE_Pwm_init()</tt> shall initialize the hardware PWM
 *  channel, referenced by <tt>Channel</tt>, according to configuration flags
 *  referenced by <tt>Flags</tt>.
 *
 *  The function <tt>EE_Pwm_Init()</tt> shall only initialize the referenced
 *  Channel and shall not touch other channels.
 *
 *  The function <tt>EE_Pwm_Init()</tt> shall disable all interrupt
 *  notifications, controlled by PWM Channel.
 *
 *  @note	The reason is that the users of these notifications may not be
 *  		ready. They can call <tt>EE_Pwm_EnableNotification()</tt> to
 *  		start notifications.
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
 *  The users of the PWM module shall not call the function
 *  <tt>EE_Pwm_Init()</tt> during a "running" operation.
 *
 *  The PWM module's environment shall not call any function of the PWM module,
 *  except <tt>EE_Pwm_GetStatus()</tt>, before having called
 *  <tt>EE_Pwm_Init()</tt>.
 *
 *  If Extended Status for the PWM module is enabled: If the PWM Channel is not
 *  in operation mode "uninitialized", the function <tt>EE_Pwm_Init()</tt> shall
 *  return the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the PWM module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Pwm_Init()</tt> shall return the
 *  development error <tt>E_PWM_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the PWM module is enabled: If the parameter
 *  <tt>Flags</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Pwm_Init()</tt> shall return the
 *  development error <tt>E_PWM_PARAM_FLAGS</tt>.
 *
 *  A re-initialization of the PWM Channel by executing the
 *  <tt>EE_Pwm_Init()</tt> function requires a de-initialization before by
 *  executing a <tt>EE_Pwm_DeInit()</tt>.
 */
#ifdef	__PWM_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Pwm_Init(
  EE_TYPEPWMCHANNEL	Channel,
  EE_TYPEPWMFLAGS	Flags,
  EE_ISR_callback	NotifCB
);

#ifdef	__PWM_DEINIT_API__
/** @brief	PWM Channel De-Initialization.
 *  @param[in]	Channel	PWM Channel to be De-Initialized.
 *  @param[in]	Flags	PWM Channel De-initialization Configuration Flags.
 *  @return	Extended Status Only:
 *  	- E_OK:			PWM Channel de-initializion successfull.
 *  	- E_NOT_OK:		PWM Channel NOT initialized.
 *  	- E_PWM_PARAM_CHANNEL:	PWM Channel Invalid.
 *  	- E_PWM_PARAM_FLAG:	PWM Channel De-Initialization Configuration
 *  				Flags Invalid.
 *  	- E_PWM_CHANNEL_BUSY:	PWM Channel Busy.
 *
 *  Service for PWM Channel De-Initialization.
 *
 *  The function <tt>EE_Pwm_DeInit()</tt> shall De-Initialize the PWM Channel.
 *
 *  The function <tt>EE_Pwm_DeInit()</tt> shall set the state of the PWM output
 *  signal to the idle state.
 *
 *  The function <tt>EE_Pwm_DeInit()</tt> shall disable PWM interrupts and PWM
 *  signal notifications.
 *
 *  The function <tt>EE_Pwm_DeInit()</tt> shall influence only the peripherals,
 *  which are associated to the referenced Channel.
 *
 *  The function <tt>EE_Pwm_DeInit()</tt> shall set the operation mode of the
 *  PWM Channel to "uninitialized".
 *
 *  If Extended Status for the PWM module is enabled: If the PWM Channel is in
 *  operation mode "uninitialized", the function <tt>EE_Pwm_DeInit()</tt> shall
 *  return the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the PWM module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Pwm_DeInit()</tt> shall return the
 *  development error <tt>E_PWM_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the PWM module is enabled: If the parameter
 *  <tt>Flags</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Pwm_DeInit()</tt> shall return the
 *  development error <tt>E_PWM_PARAM_FLAGS</tt>.
 *
 *  If Extended Status for the PWM module is enabled: If PWM channel is in
 *  state "running", the function <tt>EE_Pwm_DeInit()</tt> shall return the
 *  development error <tt>E_PWM_CHANNEL_BUSY</tt>.
 */
#ifdef	__PWM_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
Pwm_DeInit(
  EE_TYPEPWMCHANNEL	Channel,
  EE_TYPEPWMFLAGS	Flags
);
#endif	/* __PWM_DEINIT_API__ */

#ifdef	__PWM_ENABLENOTIF_API__
/** @brief	Enable notification for a PWM channel
 *  @param[in]	Channel		Numeric identifier of the PWM Channel.
 *  @param[in]	Notification	Type of notification:
 *  				- <tt>EE_PWM_RISING_EDGE</tt>.
 *  				- <tt>EE_PWM_FALLING_EDGE</tt>.
 *  				- <tt>EE_PWM_BOTH_EDGES</tt>.
 *  @return	Extended Status Only:
 *  	- E_OK:			Notification Disable Operation Successfull.
 *  	- E_NOT_OK:		PWM Channel NOT initialized.
 *  	- E_PWM_PARAM_CHANNEL:	PWM Channel Invalid.
 *  	- E_PWM_PARAM_EDGE:	Invalid Type of Notification.
 *  	- E_PWM_PARAM_CALLBACK:	PWM Channel Notification Call-Back Function
 *  				Pointer is <tt>NULL</tt>.
 *
 *  Service to enable the PWM signal edge notification according to notification
 *  parameter.
 *
 *  The function <tt>EE_Pwm_EnableNotification()</tt> shall enable the PWM
 *  signal edge notification according to notification parameter.
 *
 *  The function <tt>EE_Pwm_EnableNotification</tt> shall cancel pending
 *  interrupts.
 *
 *  If Extended Status for the PWM module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Pwm_EnableNotification()</tt> shall
 *  return the development error <tt>E_PWM_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the PWM module is enabled: If the parameter
 *  <tt>Notification</tt> is invalid (not within the range specified by
 *  <tt>EE_TYPEPWMEDGENOTIF</tt>),
 *  the function <tt>EE_Pwm_EnableNotification()</tt> shall return the
 *  development error <tt>E_PWM_PARAM_EDGE</tt>.
 *
 *  If Extended Status for the PWM module is enabled: If no valid notification
 *  function is configured, the function <tt>EE_Pwm_EnableNotification()</tt>
 *  shall raise the error <tt>E_PWM_PARAM_CALLBACK</tt>.
 */
#ifdef	__PWM_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Pwm_EnableNotification(
  EE_TYPEPWMCHANNEL	Channel,
  EE_TYPEPWMEDGENOTIF	Notification
);

#ifdef	__PWM_DISABLENOTIF_API__
/** @brief	Disable notification for a PWM channel
 *  @param[in]	Channel		Numeric identifier of the PWM Channel.
 *  @return	Extended Status Only:
 *  	- E_OK:			Notification Disable Operation Successull.
 *  	- E_NOT_OK:		PWM Channel NOT initialized.
 *  	- E_PWM_PARAM_CHANNEL:	PWM Channel Invalid.
 *  	- E_PWM_PARAM_CALLBACK:	PWM Channel Notification Call-Back Function
 *  				Pointer is <tt>NULL</tt>.
 *
 *  Service to disable the PWM signal edge notification.
 *
 *  The function <tt>EE_Pwm_DisableNotification</tt> shall disable the PWM
 *  signal edge notification.
 *
 *  The function <tt>EE_Pwm_EnableNotification</tt> shall cancel pending
 *  interrupts.
 *
 *  If Extended Status for the PWM module is enabled: If the PWM Channel is in
 *  operation mode "uninitialized", the function <tt>EE_Pwm_DeInit()</tt> shall
 *  return the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the PWM module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Pwm_DisableNotification()</tt> shall
 *  return the development error <tt>E_PWM_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the PWM module is enabled: If no valid notification
 *  function is configured, the function <tt>EE_Pwm_DisableNotification()</tt>
 *  shall raise the error <tt>E_PWM_PARAM_CALLBACK</tt>.
 */
#ifdef	__PWM_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Pwm_DisableNotification(
  EE_TYPEPWMCHANNEL	Channel
);
#endif	/* __PWM_DISABLENOTIF_API__ */
#endif	/* __PWM_ENABLENOTIF_API__ */

/** @brief	Setting period and duty cycle of a PWM channel.
 *  @param[in]	Channel		Numeric identifier of the PWM Channel.
 *  @param[in]	Period		Period of the PWM signal.
 *  @param[in]	DutyCycle	Duty-Cycle of the PWM signal.
 *  @return	Extended Status Only:
 *  	- E_OK:				Operation Successfull.
 *  	- E_NOT_OK:			PWM Channel NOT initialized.
 *  	- E_PWM_PARAM_CHANNEL:		PWM Channel Invalid.
 *  	- E_PWM_PARAM_PERIOD:		PWM Period Invalid.
 *  	- E_PWM_PARAM_DUTYCYCLE:	PWM Duty-Cycle Invalid.
 *  	- E_PWM_FIXED_PERIOD:		PWM Channel Period NOT Variable.
 *
 *  Service to set the period and the duty cycle of a PWM channel.
 *
 *  The function <tt>EE_Pwm_SetPeriodAndDuty</tt> shall set the period and the
 *  duty cycle of a PWM channel.
 *
 *  The function <tt>EE_Pwm_SetPeriodAndDuty()e</tt> shall allow changing the
 *  period for the PWM channel configured as variable period type.
 *
 *  The function <tt>EE_Pwm_SetPeriodAndDuty</tt> shall update the period always
 *  at the end of the current period if supported by the implementation and
 *  configured with <tt>EE_PWM_FLAG_PER_END_UP</tt>.
 *
 *  When updating the PWM period and duty, the driver shall repress any spikes
 *  on the PWM output signal.
 *
 *  @note	The PWM duty cycle parameter is necessary to maintain the
 *  		consistency between frequency and duty cycle.
 *
 *  If Extended Status for the PWM module is enabled: If the PWM Channel is in
 *  operation mode "uninitialized", the function
 *  <tt>EE_Pwm_SetPeriodAndDuty()</tt> shall return the development error
 *  <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the PWM module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Pwm_SetPeriodAndDuty()</tt> shall
 *  return the development error <tt>E_PWM_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the PWM module is enabled: If the parameter
 *  <tt>Period</tt> is invalid, the function <tt>EE_Pwm_SetPeriodAndDuty()</tt>
 *  shall return the development error <tt>E_PWM_PARAM_PERIOD</tt>.
 *
 *  If Extended Status for the PWM module is enabled: If the parameter
 *  <tt>DutyCycle</tt> is invalid, the function
 *  <tt>EE_Pwm_SetPeriodAndDuty()</tt> shall return the development error
 *  <tt>E_PWM_PARAM_DUTYCYCLE</tt>.
 *
 *  If Extended Status for the PWM module is enabled: the API
 *  <tt>EE_Pwm_SetPeriodAndDuty()</tt> shall check if the given PWM channel was
 *  configured with initialization flag <tt>EE_PWM_FLAG_PER_FIX</tt>. If this is
 *  the case, the development error <tt>E_PWM_FIXED_PERIOD</tt> shall be
 *  returned.
 *
 */
#ifdef	__PWM_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Pwm_SetPeriodAndDuty(
  EE_TYPEPWMCHANNEL	Channel,
  EE_TYPEPWMPERIOD	Period,
  EE_TYPEPWMDUTYCYCLE	DutyCycle
);

#ifdef	__PWM_SETDUTYCYCLE_API__
/** @brief	Setting duty cycle of a PWM channel.
 *  @param[in]	Channel		Numeric identifier of the PWM Channel.
 *  @param[in]	DutyCycle	Duty-Cycle of the PWM signal.
 *  @return	Extended Status Only:
 *  	- E_OK:				Operation Successfull.
 *  	- E_NOT_OK:			PWM Channel NOT initialized.
 *  	- E_PWM_PARAM_CHANNEL:		PWM Channel Invalid.
 *  	- E_PWM_PARAM_DUTYCYCLE:	PWM Duty-Cycle Invalid.
 *
 *  Service sets the duty cycle of the PWM channel.
 *
 *  The function <tt>EE_Pwm_SetDutyCycle()</tt> shall set the duty cycle of the
 *  PWM channel.
 *
 *  When the requested duty cycle is either 0% or 100%, the function
 *  <tt>EE_Pwm_SetDutyCycle()</tt> shall set the PWM output state to either
 *  <tt>PWM_HIGH</tt> or <tt>PWM_LOW</tt>, with regard to both the configured
 *  polarity parameter and the requested duty cycle.
 *  Thus for 0% requested Duty Cycle the output will be the inverse of the
 *  configured polarity parameter, and for 100% Duty Cycle the output will be
 *  equal to the configured polarity parameter.
 *
 *  The function <tt>EE_Pwm_SetDutyCycle()</tt> shall modulate the PWM output
 *  signal according to parameters period, duty cycle and configured
 *  polarity, when the duty cycle > 0% and < 100%.
 *
 *  The function <tt>EE_Pwm_SetDutyCycle()</tt> shall update the duty cycle
 *  always at the end of the period if supported by the implementation and
 *  configured with <tt>EE_PWM_FLAG_DUTY_END_UP</tt>.
 *
 *  @note	The driver shall forbid the spike on the PWM output signal.
 *
 *  If Extended Status for the PWM module is enabled: If the PWM Channel is in
 *  operation mode "uninitialized", the function <tt>EE_Pwm_SetDutyCycle()</tt>
 *  shall return the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the PWM module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Pwm_SetDutyCycle()</tt> shall return
 *  the development error <tt>E_PWM_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the PWM module is enabled: If the parameter
 *  <tt>DutyCycle</tt> is invalid, the function <tt>EE_Pwm_SetDutyCycle()</tt>
 *  shall return the development error <tt>E_PWM_PARAM_DUTYCYCLE</tt>.
 */
#ifdef	__PWM_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Pwm_SetDutyCycle(
  EE_TYPEPWMCHANNEL	Channel,
  EE_TYPEPWMDUTYCYCLE	DutyCycle
);
#endif	/* __PWM_SETDUTYCYCLE_API__ */

#ifdef	__PWM_SETOUTPUTTOIDLE_API__
/** @brief	Set PWM output to Idle state.
 *  @param[in]	Channel		Numeric identifier of the PWM Channel.
 *  @return	Extended Status Only:
 *  	- E_OK:				Operation Successfull.
 *  	- E_NOT_OK:			PWM Channel NOT initialized.
 *  	- E_PWM_PARAM_CHANNEL:		PWM Channel Invalid.
 *
 *  Service to set the PWM output to the configured Idle state.
 *
 *  The function <tt>EE_Pwm_SetOutputToIdle</tt> shall stop immediately the PWM
 *  channel and set immediately the PWM output to the configured Idle state.
 *
 *  After the call of the function <tt>EE_Pwm_SetOutputToIdle</tt>, variable
 *  period type channels shall be reactivated using the API
 *  <tt>EE_Pwm_SetPeriodAndDuty()</tt> to activate the PWM channel with the new
 *  passed period.
 *
 *  After the call of the function <tt>EE_Pwm_SetOutputToIdle</tt>, channels
 *  shall be reactivated using the API <tt>EE_Pwm_SetDutyCycle()</tt> to
 *  activate the PWM channel with the old period.
 *
 *  After the call of the function <tt>EE_Pwm_SetPeriodAndDuty</tt>, fixed
 *  period type channels shall be reactivated using only the API
 *  <tt>EE_Pwm_SetDutyCycle()</tt> to activate the PWM channel with the old
 *  period.
 *
 *  If Extended Status for the PWM module is enabled: If the PWM Channel is in
 *  operation mode "uninitialized", the function
 *  <tt>EE_Pwm_SetOutputToIdle()</tt> shall return the development error
 *  <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the PWM module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Pwm_SetOutputToIdle()</tt> shall return
 *  the development error <tt>E_PWM_PARAM_CHANNEL</tt>.
 */
#ifdef	__PWM_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Pwm_SetOutputToIdle(
  EE_TYPEPWMCHANNEL	Channel
);
#endif	/* __PWM_SETOUTPUTTOIDLE_API__ */

#ifdef __PWM_GETOUTPUTSTATE_API__
/** @brief	Read internal state of a PWM output signal.
 *  @param[in]	Channel		Numeric identifier of the PWM Channel.
 *  @param[out]	State		PWM output signal state to Retrieve.
 *  @return	Extended Status Only:
 *  	- E_OK:				Operation Successfull.
 *  	- E_NOT_OK:			PWM Channel NOT initialized.
 *  	- E_PWM_PARAM_CHANNEL:		PWM Channel Invalid.
 *  	- E_GPT_PARAM_STATE:		State Pointer is <tt>NULL</tt>.
 *
 *   Service to read the internal state of the PWM output signal.
 *
 *  The function <tt>EE_Pwm_GetOutputState</tt> shall read the internal state of
 *  the PWM output signal and return it.
 *
 *  @note	Due to real time constraint and setting of the PWM channel
 *  		(project dependent), the output state can be modified just after
 *  		the call of the service <tt>EE_Pwm_GetOutputState</tt>.
 *
 *  If Extended Status for the PWM module is enabled: If the PWM Channel is in
 *  operation mode "uninitialized", the function
 *  <tt>EE_Pwm_GetOutputState()</tt> shall return the development error
 *  <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the PWM module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Pwm_GetOutputState()</tt> shall return
 *  the development error <tt>E_PWM_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the GPT module is enabled: If the parameter State
 *  is <tt>EE_NULL_PTR</tt>, the function shall return the development error
 *  <tt>E_PWM_PARAM_STATE</tt>.
 *
 *  If development error detection for the PWM module is enabled, and a
 *  development error occurs, then the corresponding PWM function shall return
 *  <tt>STD_LOW</tt> for the value of parameter <tt>State</tt>.
 */
#ifdef	__PWM_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Pwm_GetOutputState(
  EE_TYPEPWMCHANNEL	Channel,
  EE_UINT8		*State
);
#endif	/* __PWM_GETOUTPUTSTATE_API__ */

#endif	/* __EE_MCU_COMMON_PWM_H__ */
