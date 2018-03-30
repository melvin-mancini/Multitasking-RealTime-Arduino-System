/** @file	ee_mcu_common_gpio.h
 *
 *  @brief	Erika Enterprise - GPIO Driver - MCU Common Header File.
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
 *  The GPIO driver provides General Porpose Input(s)/Output(s) services.
 *
 *  The GPIO driver is part of the microcontroller abstraction layer (MCAL). It
 *  initializes and controls the internal General Purpose Input(s)/Output(s)
 *  (GPIO) of the microcontroller.
 *
 *  The GPIO Driver abstracts the access to the microcontroller's hardware pins.
 *
 *  The GPIO Driver shall define synchronous read/write services.
 *
 *  The GPIO module's read and write services shall ensure for all services,
 *  that the data is consistent (Interruptible read-modify-write sequences are
 *  not allowed).
 *
 *  Values used by the GPIO Driver for the software level of Channels are either
 *  <tt>EE_HIGH</tt> or <tt>EE_LOW</tt>.
 *
 *  A general-purpose digital I/O pin represents a GPIO channel.
 *
 *  The GPIO module shall provide APIs for overall configuration and
 *  initialization of ALL general-purpose digital I/O pins.
 *
 *  The GPIO module shall adapt its configuration and usage to the
 *  microcontroller and ECU.
 *
 *  The GPIO driver shall provide the reconfiguration of the general-purpose
 *  digital I/O pins direction during runtime.
 *
 *  The GPIO module's user shall only use the GPIO functions after the GPIO
 *  Driver has been initialized. Otherwise the GPIO module will exhibit
 *  undefined behavior.
 *
 *  The GPIO module shall configure a GPIO channel as input or output.
 *
 *  The GPIO module shall not buffer data when providing read and write
 *  services.
 * 
 *  @author	Riccardo Schiavi
 *  @author	Giuseppe Serano
 *  @version	0.1
 */

#ifndef	__EE_MCU_COMMON_GPIO_H__
#define	__EE_MCU_COMMON_GPIO_H__

/*
 * Return Values: Extended Status Only.
 */
#ifdef	__GPIO_EXTENDED_STATUS__

/** @brief	Invalid Channel. */
#define	E_GPIO_PARAM_CHANNEL	0xFE

/** @brief	Invalid Configuration Flags. */
#define	E_GPIO_PARAM_FLAGS	0xFD

/** @brief	Notification Call-Back Function Pointer is NULL. */
#define	E_GPIO_PARAM_CALLBACK	0xFC

/** @brief	Invalid Value. */
#define	E_GPIO_PARAM_VALUE	0xFB

/** @brief	Pointer is NULL. */
#define	E_GPIO_PARAM_NULL	0xFA

#endif	/* __GPIO_EXTENDED_STATUS__ */

/** @brief	Numeric ID of a GPIO channel.
 *
 *  Numeric ID of a GPIO channel.
 *
 *  Implementation specific. But not all values may be valid within this type.
 *
 *  This type shall be chosen in order to have the most efficient implementation
 *  on a specific micro controller platform.
 *
 *  Parameters of type <tt>EE_TYPEGPIOCHANNEL</tt> contain the numeric ID of a
 *  GPIO channel.
 *
 *  The mapping of the ID is implementation specific but not configurable and
 *  not all values may be valid within this type.
 *
 *  For parameter values of type <tt>EE_TYPEGPIOCHANNEL</tt>, the Gpio's user
 *  shall use the symbolic names provided by the driver implementation.
 *
 *  The type shall be EE_UREG depending on uC platform.
 */
#ifndef	EE_TYPEGPIOCHANNEL
#define	EE_TYPEGPIOCHANNEL	EE_UREG
#endif

/** @brief	GPIO Value.
 *
 *  Type for GPIO Value.
 *
 *  Values used by the GPIO Driver for the software level of Channels are either
 *  <tt>EE_HIGH</tt> or <tt>EE_LOW</tt>.
 */
#ifndef	EE_TYPEGPIOVALUE
#define	EE_TYPEGPIOVALUE	EE_UREG
#endif

/** @brief	GPIO Configuration Flags.
 *
 *  Type for setting-up a GPIO Channel Configuration.
 */
#ifndef	EE_TYPEGPIOFLAGS
#define	EE_TYPEGPIOFLAGS	EE_UREG
#endif

/*
 * Configuration Flags.
 */

/** @brief	GPIO Default Flag.
 *
 *  @note	GPIO Default Flag implies:
 *  		- GPIO Direction Input Flag.
 *  		- GPIO Push-Pull Flag.
 *  		- GPIO Input No Edges Detection Flag.
 */
#define	EE_GPIO_FLAG_DEFAULT	0x00U

/** @brief	GPIO Direction Input Flag. */
#define	EE_GPIO_FLAG_DIR_IN	EE_GPIO_FLAG_DEFAULT

/** @brief	GPIO Direction Output Flag. */
#define	EE_GPIO_FLAG_DIR_OUT	0x01U

/** @brief	GPIO Initial Level Low Flag.
 *
 *  Only meaning if GPIO Direction Output Flag is set!!!
 */
#define	EE_GPIO_FLAG_LVL_LOW	EE_GPIO_FLAG_DEFAULT

/** @brief	GPIO Initial Level High Flag.
 *
 *  Only meaning if Direction Output Flags is set!!!
 */
#define	EE_GPIO_FLAG_LVL_HIGH	0x02U

/** @brief	GPIO Push-Pull Flag (if supported by hardware). */
#define	EE_GPIO_FLAG_PUSH_PULL	EE_GPIO_FLAG_DEFAULT

/** @brief	GPIO Bus Keeper Flag (if supported by hardware). */
#define	EE_GPIO_FLAG_BUS_KEEPER	0x04U

/** @brief	GPIO Pull Down Flag (if supported by hardware). */
#define	EE_GPIO_FLAG_PULL_DOWN	0x08U

/** @brief	GPIO Pull UP Flag (if supported by hardware). */
#define	EE_GPIO_FLAG_PULL_UP	0x0CU

#ifdef	__GPIO_ENABLENOTIF_API__
/** @brief	GPIO Input No Edges Detection Flag */
#define	EE_GPIO_FLAG_NO_EDGE	EE_GPIO_FLAG_DEFAULT

/** @brief	GPIO Input Rising Edge Detection Flag */
#define	EE_GPIO_FLAG_RISE_EDGE	0x10U

/** @brief	GPIO Input Falling Edge Detection Flag */
#define	EE_GPIO_FLAG_FALL_EDGE	0x20U

/** @brief	GPIO Input Both Edges Detection Flag */
#define	EE_GPIO_FLAG_BOTH_EDGE	( \
	EE_GPIO_FLAG_RISE_EDGE | EE_GPIO_FLAG_FALL_EDGE \
)
#endif	/* __GPIO_ENABLENOTIF_API__ */

#ifdef	__GPIO_EXTENDED_STATUS__
/** @brief	GPIO Peripheral Clock Output - PORT C,D,E PIN 7 ONLY */
#define	EE_GPIO_FLAG_CLK	0x40U
/** @brief	GPIO RTC Clock Output - PORT C PIN 6 ONLY */
#define	EE_GPIO_FLAG_RTC	0x80U
#endif	/* __GPIO_EXTENDED_STATUS__ */

/** @brief	GPIO Channel Initialization.
 *  @param[in]	Channel	GPIO Channel to be Initialized.
 *  @param[in]	Flags	GPIO Channel Configuration Flags.
 *  @param[in]	NotifCB	GPIO Channel Notification Call-Back Function Pointer.
 *  @return	Extended Status Only:
 *  	- E_OK:			GPIO Channel Initializion Operation Success.
 *  	- E_NOT_OK:		GPIO Channel already initialized.
 *  	- E_GPIO_PARAM_CHANNEL:	GPIO Channel Invalid.
 *  	- E_GPIO_PARAM_FLAG:	GPIO Channel Configuration Flags Invalid.
 *
 *  This service initializes a GPIO Channel.
 *
 *  The function <tt>EE_Gpio_Init()</tt> shall initialize the hardware pin,
 *  referenced by <tt>Channel</tt>, according to configuration flags
 *  referenced by <tt>Flags</tt>.
 *
 *  The function <tt>EE_Gpio_Init()</tt> shall disable all interrupt
 *  notifications, controlled by the GPIO Channel.
 *
 *  The function <tt>EE_Gpio_Init()</tt> shall only initialize the referenced
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
 *  If Extended Status for the GPIO module is enabled: If the GPIO Channel is
 *  not in operation mode "uninitialized", the function <tt>EE_Gpio_Init()</tt>
 *  shall return the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the GPIO module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Gpio_Init()</tt> shall return the
 *  development error <tt>E_GPIO_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the GPIO module is enabled: If the parameter
 *  <tt>Flags</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Gpio_Init()</tt> shall return the
 *  development error <tt>E_GPIO_PARAM_FLAGS</tt>.
 *
 *  A re-initialization of the GPIO channel by executing the
 *  <tt>EE_Gpio_Init()</tt> function requires a de-initialization before by
 *  executing a <tt>EE_Gpio_DeInit</tt>.
 */
#ifdef	__GPIO_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpio_Init(
  EE_TYPEGPIOCHANNEL	Channel,
  EE_TYPEGPIOFLAGS	Flags,
  EE_ISR_callback	NotifCB
);

#ifdef	__GPIO_DEINIT_API__
/** @brief	GPIO Channel De-Initialization.
 *  @param[in]	Channel	GPIO Channel to be De-Initialized.
 *  @param[in]	Flags	GPIO Channel De-initialization Configuration Flags.
 *  @return	Extended Status Only:
 *  	- E_OK:			GPIO Channel De-Initializion Operation Success.
 *  	- E_NOT_OK:		GPIO Channel NOT initialized.
 *  	- E_GPIO_PARAM_CHANNEL:	GPIO Channel Invalid.
 *  	- E_GPIO_PARAM_FLAG:	GPIO Channel De-Initialization Configuration
 *  				Flags Invalid.
 *
 *  Deinitializes a GPIO Channel.
 *
 *  The function <tt>EE_Gpio_DeInit()</tt> shall deinitialize the hardware used
 *  by the GPIO Channel, referenced by <tt>Channel</tt>,according to a
 *  configuration flags referenced by <tt>Flags</tt>. Values of registers which
 *  are not writeable are excluded. It's the responsibility of the hardware
 *  design that the state does not lead to undefined activities in the uC.
 *
 *  The function <tt>EE_Gpio_DeInit()</tt> shall disable all interrupt
 *  notifications, controlled by the GPIO Channel.
 *
 *  The function <tt>EE_Gpio_DeInit()</tt> shall influence only the peripherals,
 *  which are associated to the referenced Channel.
 *
 *  The function <tt>EE_Gpio_DeInit()</tt> shall set the operation mode of the
 *  GPIO Channel to "uninitialized".
 *
 *  If Extended Status for the GPIO module is enabled: If the GPIO Channel is in
 *  operation mode "uninitialized", the function <tt>EE_Gpio_DeInit()</tt> shall
 *  return the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the GPIO module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Gpio_DeInit()</tt> shall return the
 *  development error <tt>E_GPIO_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the GPIO module is enabled: If the parameter
 *  <tt>Flags</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Gpio_DeInit()</tt> shall return the
 *  development error <tt>E_GPIO_PARAM_FLAGS</tt>.
 */
#ifdef	__GPIO_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpio_DeInit(
  EE_TYPEGPIOCHANNEL	Channel,
  EE_TYPEGPIOFLAGS	Flags
);
#endif	/* __GPIO_DEINIT_API__ */

#ifdef	__GPIO_READCHANNEL_API__
/** @brief	GPIO Channel Read.
 *  @param[in]	Channel	Numeric identifier of the GPIO Channel.
 *  @param[out]	Value	GPIO Value to Read.
 *  	- <tt>EE_LOW</tt>:	The physical level is 0V.
 *  	- <tt>EE_HIGH</tt>:	The physical level is 3.3V or 5.0V
 *  @return	Extended Status Only:
 *  	- E_OK:			GPIO Channel Read Operation Success.
 *  	- E_NOT_OK:		GPIO Channel NOT initialized.
 *  	- E_GPIO_PARAM_CHANNEL:	GPIO Channel Invalid.
 *  	- E_GPIO_PARAM_NULL:	GPIO Value Pointer is <tt>NULL</tt>.
 *
 *  Returns the value of the specified GPIO channel.
 *
 *  The <tt>EE_Gpio_ReadChannel()</tt> function shall retrieve the value of
 *  the specified DIO channel.
 *
 *  If Extended Status for the GPIO module is enabled: If the GPIO Channel is in
 *  operation mode "uninitialized", the function <tt>EE_Gpio_ReadChannel()</tt>
 *  shall return the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the GPIO module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Gpio_ReadChannel()</tt> shall return
 *  the development error <tt>E_GPIO_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the GPIO module is enabled: If the parameter
 *  <tt>Value</tt> is a <tt>NULL</tt> pointer, the function
 *  <tt>EE_Gpio_ReadChannel()</tt> shall return the development error
 *  <tt>E_GPIO_PARAM_NULL</tt>.
 */
#ifdef	__GPIO_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpio_ReadChannel(
  EE_TYPEGPIOCHANNEL	Channel,
  EE_TYPEGPIOVALUE	*Value
);
#endif	/* __GPIO_READCHANNEL_API__ */

#ifdef	__GPIO_WRITECHANNEL_API__
/** @brief	GPIO Channel Write
 *  @param[in]	Channel	Numeric identifier of the GPIO Channel.
 *  @param[in]	Value	GPIO Value to Write.
 *  	- <tt>EE_LOW</tt>:	The physical level is 0V.
 *  	- <tt>EE_HIGH</tt>:	The physical level is 3.3V or 5.0V
 *  @return	Extended Status Only:
 *  	- E_OK:			GPIO Channel Write Operation Success.
 *  	- E_NOT_OK:		GPIO Channel NOT initialized.
 *  	- E_GPIO_PARAM_CHANNEL:	GPIO Channel Invalid.
 *  	- E_GPIO_PARAM_VALUE:	GPIO Value Invalid.
 *
 *  Service to set a value of a channel.
 *
 *  If the specified channel is configured as an output channel, the
 *  <tt>EE_Gpio_WriteChannel()</tt> function shall set the specified value for
 *  the specified channel.
 *
 *  If the specified channel is configured as an input channel, the
 *  <tt>EE_Gpio_WriteChannel()</tt> function shall have no influence on the
 *  physical output.
 *
 *  If the specified channel is configured as an input channel, the
 *  <tt>EE_Gpio_WriteChannel()</tt> function shall have no influence on the
 *  result of the next Read-Service.
 *
 *  If Extended Status for the GPIO module is enabled: If the GPIO Channel is in
 *  operation mode "uninitialized", the function <tt>EE_Gpio_WriteChannel()</tt>
 *  shall return the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the GPIO module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Gpio_WriteChannel()</tt> shall return
 *  the development error <tt>E_GPIO_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the GPIO module is enabled: If the parameter
 *  <tt>Value</tt> is invalid (not equal to <tt>EE_LOW</tt> or to
 *  <tt>EE_HIGH</tt>), the function <tt>EE_Gpio_WriteChannel()</tt> shall return
 *  the development error <tt>E_GPIO_PARAM_VALUE</tt>.
 */
#ifdef	__GPIO_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpio_WriteChannel(
  EE_TYPEGPIOCHANNEL	Channel,
  EE_TYPEGPIOVALUE	Value
);
#endif	/* __GPIO_WRITECHANNEL_API__ */

#ifdef	__GPIO_FLIPCHANNEL_API__
/** @brief	GPIO Channel Flip
 *  @param[in]	Channel	Numeric identifier of the GPIO Channel.
 *  @param[out]	Value	GPIO Value to Retrieve.
 *  @return	Extended Status Only:
 *  	- E_OK:			GPIO Channel Flip Operation Success.
 *  	- E_NOT_OK:		GPIO Channel NOT initialized.
 *  	- E_GPIO_PARAM_CHANNEL:	GPIO Channel Invalid.
 *  	- E_GPIO_PARAM_NULL:	GPIO Value Pointer is <tt>NULL</tt>.
 *
 *  Service to flip (change from <tt>EE_HIGH</tt> to <tt>EE_LOW</tt> or from
 *  <tt>EE_LOW</tt> to <tt>EE_HIGH</tt>) the level of a channel and retrieve the
 *  level of the channel after flip.
 *
 *  If the specified channel is configured as an output channel, the
 *  <tt>EE_Gpio_FlipChannel()</tt> function shall read level of the channel and
 *  invert it, then write the inverted level to the channel. The return value
 *  shall be the inverted level of the specified channel.
 *
 *  If the specified channel is configured as an input channel, the
 *  <b>EE_Gpio_FlipChannel()</b> function shall have no influence on the
 *  physical output. The retrieved value shall be the level of the specified
 *  channel.
 *
 *  If the specified channel is configured as an input channel, the
 *  <tt>EE_Gpio_FlipChannel()</tt> function shall have no influence on the
 *  result of the next Read-Service.
 *
 *  If Extended Status for the GPIO module is enabled: If the GPIO Channel is in
 *  operation mode "uninitialized", the function <tt>EE_Gpio_FlipChannel()</tt>
 *  shall return the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the GPIO module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Gpio_FlipChannel()</tt> shall return
 *  the development error <tt>E_GPIO_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the GPIO module is enabled: If the parameter
 *  <tt>Value</tt> is a <tt>NULL</tt> pointer, the function
 *  <tt>EE_Gpio_FlipChannel()</tt> shall return the development error
 *  <tt>E_GPIO_PARAM_NULL</tt>.
 */
#ifdef	__GPIO_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpio_FlipChannel(
  EE_TYPEGPIOCHANNEL	Channel,
  EE_TYPEGPIOVALUE	*Value
);
#endif	/* __GPIO_FLIPCHANNEL_API__ */

#ifdef	__GPIO_ENABLENOTIF_API__
/** @brief	Channel Notification Enable
 *  @param[in]	Channel	Numeric identifier of the GPIO channel.
 *  @return	Extended Status Only:
 *  	- E_OK:				Notification Enable Operation Success.
 *  	- E_NOT_OK:			GPIO Channel NOT initialized.
 *  	- E_GPIO_PARAM_CHANNEL:		GPIO Channel Invalid.
 *  	- E_GPIO_PARAM_CALLBACK:	GPIO Channel Notification Call-Back
 *  					Function Pointer is <tt>NULL</tt>.
 *
 *  Enables the interrupt notification for a channel.
 *
 *  The function <tt>EE_Gpio_EnableNotification()</tt> shall enable the
 *  interrupt notification of the referenced channel configured for
 *  notification.
 *
 *  The function <tt>EE_Gpio_EnableNotification()</tt> shall be reentrant, if
 *  the channels used in concurrent calls are different.
 *
 *  If Extended Status for the GPIO module is enabled: If the GPIO Channel is in
 *  operation mode "uninitialized", the function shall return the development
 *  error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the GPIO module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Gpio_EnableNotification()</tt> shall
 *  return the development error <tt>E_GPIO_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the GPIO module is enabled: If no valid notification
 *  function is configured, the function <tt>EE_Gpio_EnableNotification()</tt>
 *  shall raise the error <tt>E_GPIO_PARAM_CALLBACK</tt>.
 */
#ifdef	__GPIO_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpio_EnableNotification(
  EE_TYPEGPIOCHANNEL	Channel
);

#ifdef	__GPIO_DISABLENOTIF_API__
/** @brief	Channel Notification Disable
 *  @param[in]	Channel	Numeric identifier of the GPIO channel.
 *  @return	Extended Status Only:
 *  	- E_OK:				Notification Disable Operation Success.
 *  	- E_NOT_OK:			GPIO Channel NOT initialized.
 *  	- E_GPIO_PARAM_CHANNEL:		GPIO Channel Invalid.
 *  	- E_GPIO_PARAM_CALLBACK:	GPIO Channel Notification Call-Back
 *  					Function Pointer is <tt>NULL</tt>.
 *
 *  Disables the interrupt notification for a channel.
 *
 *  The function <tt>EE_Gpio_DisableNotification()</tt> shall disable the
 *  interrupt notification of the referenced channel configured for
 *  notification.
 *
 *  The function <tt>EE_Gpio_DisableNotification()</tt> shall be reentrant, if
 *  the timer channels used in concurrent calls are different.
 *
 *  If Extended Status for the GPIO module is enabled: If the GPIO Channel is in
 *  operation mode "uninitialized", the function shall return the development
 *  error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the GPIO module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Gpio_DisableNotification()</tt> shall
 *  return the development error <tt>E_GPIO_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the GPIO module is enabled: If no valid notification
 *  function is configured, the function <tt>EE_Gpio_DisableNotification()</tt>
 *  shall raise the error <tt>E_GPIO_PARAM_CALLBACK</tt>.
 */
#ifdef	__GPIO_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Gpio_DisableNotification(
  EE_TYPEGPIOCHANNEL	Channel
);
#endif	/* __GPIO_DISABLENOTIF_API__ */
#endif	/* __GPIO_ENABLENOTIF_API__ */

#endif	/* __EE_MCU_COMMON_GPIO_H__ */
