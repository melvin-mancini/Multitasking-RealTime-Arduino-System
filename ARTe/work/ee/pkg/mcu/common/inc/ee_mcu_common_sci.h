/** @file	ee_mcu_common_sci.h
 *
 *  @brief	Erika Enterprise - SCI Driver - MCU Common Header File.
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
 *  The SCI driver provides serial communications services.
 *
 *  The SCI driver is part of the microcontroller abstraction layer (MCAL),
 *  performs the hardware access and offers a hardware independent API to the
 *  upper layer.
 *
 *  A SCI driver can support more than one channel. This means that the SCI
 *  driver can handle one or more SCI channels as long as they are belonging to
 *  the same SCI hardware unit.
 *
 *  The hardware of the internal SCI hardware unit depends on the system clock,
 *  prescaler(s) and PLL. Hence, the length of the SCI bit timing depends on the
 *  clock settings made in module SCI.
 *
 *  The SCI driver module will not take care of setting the registers that
 *  configure the clock, prescaler(s) and PLL (e.g. switching on/off the PLL) in
 *  its init functions. The SCI module must do this.
 *
 *  The Port driver configures the port pins used for the SCI driver as input or
 *  output. Hence, the Port driver has to be initialized prior to the use of SCI
 *  functions. Otherwise, SCI driver functions will exhibit undefined behavior.
 *
 *  The SCI driver uses interrupts and therefore there is a dependency on the
 *  OS, which configures the interrupt sources.
 *
 *  @author	Riccardo Schiavi
 *  @author	Giuseppe Serano
 *  @version	0.1
 */

#ifndef	__EE_MCU_COMMON_SCI_H__
#define	__EE_MCU_COMMON_SCI_H__

/** @brief	SCI Channel Status Type
 *
 *  The type <tt>EE_TYPESCICHSTAT</tt> is the type of the return value of the
 *  function <tt>EE_Sci_GetStatus()</tt>.
 *
 *  The type of <tt>EE_TYPESCICHSTAT</tt> is an enumeration describing the
 *  operation states for a SCI channel or transmission.
 */
typedef enum {
  EE_SCI_CH_UNINIT,		/**< "Un-Initialized" State.		      */
  EE_SCI_CH_OPERATIONAL,	/**< Normal operation; the related SCI channel
  				 *   is ready to transmit or receive. No data
  				 *   from previous reception available (e.g.
  				 *   after initialization)		      */
  EE_SCI_CH_TX_OK,		/**< Successful transmission.		      */
  EE_SCI_CH_TX_BUSY,		/**< Ongoing transmission.		      */
  EE_SCI_CH_TX_ERROR,		/**< Erroneous transmission such as:
  				  *  - Transmission disabled.
  				  *  - Physical bus error.		      */
  EE_SCI_CH_RX_OK,		/**< Successful reception.		      */
  EE_SCI_CH_RX_BUSY,		/**< Ongoing reception.			      */
  EE_SCI_CH_RX_ERROR,		/**< Erroneous reception such as:
  				  *  - Reception disabled.
  				  *  - Parity error.
  				  *  - Framing error.
  				  *  - Overrun error.			      */
} EE_TYPESCICHSTAT;

/*
 * Return Values: Extended Status Only.
 */
#ifdef	__SCI_EXTENDED_STATUS__

/** @brief	Invalid Channel. */
#define	E_SCI_PARAM_CHANNEL	0xFE

/** @brief	Invalid Configuration Flags. */
#define	E_SCI_PARAM_FLAGS	0xFD

/** @brief	Invalid BaudRate. */
#define	E_SCI_PARAM_BAUDRATE	0xFC

/** @brief	Notification Call-Back Function Pointer is NULL. */
#define	E_SCI_PARAM_CALLBACK	0xFB

/** @brief	Busy Channel. */
#define	E_SCI_PARAM_BUSY	0xFA

/** @brief	Pointer is NULL. */
#define	E_SCI_PARAM_NULL	0xF9

#endif	/* __SCI_EXTENDED_STATUS__ */

/** @brief	Numeric ID of a SCI channel.
 *
 *  Numeric ID of a SCI channel.
 *
 *  Implementation specific. But not all values may be valid within this type.
 *
 *  This type shall be chosen in order to have the most efficient implementation
 *  on a specific micro controller platform.
 *
 *  Parameters of type <tt>EE_TYPESCICHANNEL</tt> contain the numeric ID of a
 *  SCI channel.
 *
 *  The mapping of the ID is implementation specific but not configurable and
 *  not all values may be valid within this type.
 *
 *  For parameter values of type <tt>EE_TYPESCICHANNEL</tt>, the Sci's user
 *  shall use the symbolic names provided by the driver implementation.
 *
 *  The type shall be EE_UREG depending on uC platform.
 */
#ifndef	EE_TYPESCICHANNEL
#define	EE_TYPESCICHANNEL	EE_UREG
#endif

/** @brief	SCI Configuration Flags.
 *
 *  Type for setting-up a SCI Channel Configuration.
 */
#ifndef	EE_TYPESCIFLAGS
#define	EE_TYPESCIFLAGS		EE_UREG
#endif

/** @brief	SCI Default Flag. */
#define	EE_SCI_FLAG_DEFAULT	0x00U

/** @brief	SCI Clock.
 *
 *  Type for SCI clock.
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
#ifndef	EE_TYPESCICLOCK
#define	EE_TYPESCICLOCK		EE_UINT32
#endif

/** @brief	SCI Baudrate.
 *
 *  Type for SCI baudrate.
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
#ifndef	EE_TYPESCIBAUDRATE
#define	EE_TYPESCIBAUDRATE	EE_UINT32
#endif

/** @brief	SCI Data.
 *
 *  Type for SCI data elements.
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
#ifndef	EE_TYPESCIDATA
#define	EE_TYPESCIDATA		EE_UINT8
#endif

#ifdef	__SCI_GETSTATUS_API__
/** @brief	Channel Status Retrieval.
 *  @param[in]	Channel	Numeric identifier of the SCI channel.
 *  @return	
 *  	- <tt>EE_SCI_CH_UNINIT</tt>:		"Un-Initialized" State.
 *  	- <tt>EE_SCI_CH_OPERATIONAL</tt>:	Normal operation; the related
 *  						SCI channel is ready to transmit
 *  						or receive. (e.g. after
 *  						initialization)
 *  	- <tt>EE_SCI_CH_TX_OK</tt>:		Successful transmission
 *  	- <tt>EE_SCI_CH_TX_BUSY</tt>:		Ongoing transmission
 *  	- <tt>EE_SCI_CH_TX_ERROR</tt>:		Erroneous response transmission
 *  						such as:
 *  						- Transmission disabled.
 *  						- Physical bus error.
 *  	- <tt>EE_SCI_CH_RX_OK</tt>:		Reception of correct response
 *  	- <tt>EE_SCI_CH_RX_BUSY</tt>:		Ongoing reception
 *  	- <tt>EE_SCI_CH_RX_ERROR</tt>:		Erroneous response reception
 *  						such as:
 *  						- Reception disabled.
 *  						- Parity error.
 *  						- Framing error.
 *  						- Overrun error.
 *
 *  The service gets the status of the specified SCI channel.
 *
 *  The function <tt>EE_Sci_GetStatus()</tt> shall return the current
 *  transmission, reception or operational status of the SCI driver.
 *
 *  The return states <tt>EE_SCI_CH_TX_OK</tt>, <tt>EE_SCI_CH_TX_BUSY</tt>,
 *  <tt>EE_SCI_CH_TX_ERROR</tt>, <tt>EE_SCI_CH_RX_OK</tt>,
 *  <tt>EE_SCI_CH_RX_BUSY</tt> and <tt>EE_SCI_CH_RX_ERROR</tt> are substates of
 *  the channel state <tt>EE_SCI_CH_OPERATIONAL</tt>.
 */
EE_TYPESCICHSTAT
EE_Sci_GetStatus(
  EE_TYPESCICHANNEL	Channel
);
#endif	__SCI_GETSTATUS_API__

/** @brief	SCI Channel Initialization
 *  @param[in]	Channel		Numeric identifier of the SCI channel.
 *  @param[in]	Flags		SCI Channel Configuration Flags.
 *  @param[in]	Clock		SCI Channel Clock Frequency (Hz).
 *  @param[in]	BaudRate	SCI Channel BaudRate.
 *  @param[in]	RxNotifCB	SCI Channel Notification Call-Back Function Pointer.
 *  @param[in]	TxNotifCB	SCI Channel Notification Call-Back Function Pointer.
 *  @return	Extended Status Only:
 *  	- E_OK:			SCI Channel Initializion Operation Success.
 *  	- E_NOT_OK:		SCI Channel already initialized.
 *  	- E_SCI_PARAM_CHANNEL:	SCI Channel Invalid.
 *  	- E_SCI_PARAM_FLAGS:	SCI Channel Configuration Flags Invalid.
 *  	- E_SCI_PARAM_BAUDRATE:	SCI Channel Baudrate Invalid.
 *
 *  This service initializes a SCI channel.
 *
 *  The function <tt>Sci_Init()</tt> shall initialize the SCI Channel (i.e.
 *  static variables, including flags and SCI Hardware Unit global hardware
 *  settings), as well as the SCI channel, according to configuration flags
 *  referenced by <tt>Flags</tt>.
 *
 *  The function <tt>EE_Sci_Init()</tt> shall disable all interrupt
 *  notifications, controlled by the SCI Channel.
 *
 *  The function <tt>Sci_Init()</tt> shall invoke initialization for relevant
 *  hardware register settings common to <tt>Channel</tt> available on the SCI
 *  Hardware Unit.
 *
 *  The function <tt>Sci_Init()</tt> shall also invoke initializations for SCI
 *  channel specific settings.
 *
 *  The function <tt>EE_Sci_Init()</tt> shall only initialize the referenced
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
 *  If Extended Status for the SCI module is enabled: If the SCI Channel is not
 *  in operation mode "uninitialized", the function <tt>EE_Sci_Init()</tt> shall
 *  return the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the SCI module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Sci_Init()</tt> shall return the
 *  development error <tt>E_SCI_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the SCI module is enabled: If the parameter
 *  <tt>Flags</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Sci_Init()</tt> shall return the
 *  development error <tt>E_SCI_PARAM_FLAGS</tt>.
 *
 *  A re-initialization of the SCI channel by executing the
 *  <tt>EE_Sci_Init()</tt> function requires a de-initialization before by
 *  executing a <tt>EE_Sci_DeInit()</tt>.
 *
 *  The SCI module's environment shall not call any function of the SCI module
 *  before having called <tt>Sci_Init()</tt>.
 */
#ifdef	__SCI_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Sci_Init(
  EE_TYPESCICHANNEL	Channel,
  EE_TYPESCIFLAGS	Flags,
  EE_TYPESCICLOCK	Clock,
  EE_TYPESCIBAUDRATE	BaudRate,
  EE_ISR_callback	RxNotifCB,
  EE_ISR_callback	TxNotifCB
);

#ifdef	__SCI_DEINIT_API__
/** @brief	SCI Channel De-Initialization.
 *  @param[in]	Channel	Numeric identifier of the SCI channel.
 *  @param[in]	Flags	SCI Channel De-initialization Configuration Flags.
 *  @return	Extended Status Only:
 *  	- E_OK:			SCI Channel De-Initializion Operation Success.
 *  	- E_NOT_OK:		SCI Channel NOT initialized.
 *  	- E_SCI_PARAM_CHANNEL:	SCI Channel Invalid.
 *  	- E_SCI_PARAM_FLAGS:	SCI Channel Configuration Flags Invalid.
 *
 *  Deinitializes a SCI channel.
 *
 *  The function <tt>Sci_DeInit()</tt> shall deinitialize the hardware used by
 *  the SCI Channel to the power on reset state.
 *  Values of registers which are not writeable are excluded. It's the
 *  responsibility of the hardware design that the state does not lead to
 *  undefined activities in the uC.
 *
 *  The function <tt>Sci_DeInit()</tt> shall disable all interrupt notifications
 *  controlled by the SCI Channel.
 *
 *  The function <tt>Sci_DeInit()</tt> shall influence only the peripherals,
 *  which are allocated to the referenced Channel.
 *
 *  The function <tt>Sci_DeInit()</tt> shall set the operation mode of the SCI
 *  Channel to "uninitialized".
 *
 *  If Extended Status for the SCI module is enabled: If the SCI Channel is in
 *  operation mode "uninitialized", the function <tt>EE_Sci_DeInit()</tt> shall
 *  return the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the SCI module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Sci_DeInit()</tt> shall return the
 *  development error <tt>E_SCI_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the SCI module is enabled: If the parameter
 *  <tt>Flags</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Sci_DeInit()</tt> shall return the
 *  development error <tt>E_SCI_PARAM_FLAGS</tt>.
 */
#ifdef	__SCI_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Sci_DeInit(
  EE_TYPESCICHANNEL	Channel,
  EE_TYPESCIFLAGS	Flags
);
#endif	/* __SCI_DEINIT_API__ */

#ifdef	__SCI_WRITETXDATA_API__
/** @brief	Tx Write.
 *  @param[in]	Channel	Numeric identifier of the SCI channel.
 *  @param[in]	Data	Data to be write.
 *  @return	Extended Status Only:
 *  	- E_OK:			SCI Write Operation Success.
 *  	- E_NOT_OK:		SCI Channel NOT initialized.
 *  	- E_SCI_PARAM_BUSY:	SCI Channel Busy.
 *  	- E_SCI_PARAM_CHANNEL:	SCI Channel Invalid.
 *
 *  The service writes a Data into the Tx data register of the SCI HW Unit
 *  for the addressed Channel.
 *
 *  If Extended Status for the SCI module is enabled: If the SCI Channel is in
 *  operation mode "uninitialized", the function <tt>EE_Sci_WriteTxData()</tt>
 *  shall return the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the SCI module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Sci_WriteTxData()</tt> shall return
 *  the development error <tt>E_SCI_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the SCI module is enabled: If the SCI HW Unit is
 *  "busy" (i.e. not ready to transmit), the function
 *  <tt>EE_Sci_WriteTxData()</tt> shall return the development error
 *  <tt>E_SCI_PARAM_BUSY</tt>.
 */
#ifdef	__SCI_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Sci_WriteTxData(
  EE_TYPESCICHANNEL	Channel,
  EE_TYPESCIDATA	Data
);
#endif	/* __SCI_WRITETXDATA_API__ */

#ifdef	__SCI_DISABLETX_API__
/** @brief	Channel Transmission Disable
 *  @param[in]	Channel	Numeric identifier of the SCI channel.
 *  @return	Extended Status Only:
 *  	- E_OK:			SCI Disable Operation Success.
 *  	- E_NOT_OK:		SCI Channel NOT initialized.
 *  	- E_SCI_PARAM_BUSY:	SCI Channel Busy.
 *  	- E_SCI_PARAM_CHANNEL:	SCI Channel Invalid.
 *
 *  The service instructs the driver to disable the transmission for the
 *  addressed SCI channel.
 *
 *  If Extended Status for the SCI module is enabled: If the SCI Channel is in
 *  operation mode "uninitialized", the function <tt>EE_Sci_DisableTx()</tt>
 *  shall return the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the SCI module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Sci_DisableTx()</tt> shall return
 *  the development error <tt>E_SCI_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the SCI module is enabled: If the SCI HW Unit is
 *  "busy" (i.e. not ready to transmit), the function
 *  <tt>EE_Sci_DisableTx()</tt> shall return the development error
 *  <tt>E_SCI_PARAM_BUSY</tt>.
 */
#ifdef	__SCI_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Sci_DisableTx(
  EE_TYPESCICHANNEL	Channel
);

#ifdef	__SCI_ENABLETX_API__
/** @brief	Channel Transmission Enable
 *  @param[in]	Channel	Numeric identifier of the SCI channel.
 *  @return	Extended Status Only:
 *  	- E_OK:			SCI Enable Operation Success.
 *  	- E_NOT_OK:		SCI Channel NOT initialized.
 *  	- E_SCI_PARAM_CHANNEL:	SCI Channel Invalid.
 *
 *  The service instructs the driver to enable the transmission for the
 *  addressed SCI channel.
 *
 *  If Extended Status for the SCI module is enabled: If the SCI Channel is in
 *  operation mode "uninitialized", the function <tt>EE_Sci_EnableTx()</tt>
 *  shall return the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the SCI module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Sci_EnableTx()</tt> shall return
 *  the development error <tt>E_SCI_PARAM_CHANNEL</tt>.
 */
#ifdef	__SCI_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Sci_EnableTx(
  EE_TYPESCICHANNEL	Channel
);
#endif	/* __SCI_ENABLETX_API__ */
#endif	/* __SCI_DISABLETX_API__ */

#ifdef	__SCI_READRXDATA_API__
/** @brief	Rx Read.
 *  @param[in]	Channel	Numeric identifier of the SCI channel.
 *  @param[out]	DataPtr	Data pointer for Data to be read.
 *  @return	Extended Status Only:
 *  	- E_OK:			SCI Read Operation Success.
 *  	- E_NOT_OK:		SCI Channel NOT initialized.
 *  	- E_SCI_PARAM_CHANNEL:	SCI Channel Invalid.
 *  	- E_SCI_PARAM_NULL:	SCI Data Pointer is <tt>NULL</tt>.
 *
 *  The service reads a Data from the Rx data register of the SCI HW Unit
 *  for the addressed Channel.
 *
 *  If Extended Status for the SCI module is enabled: If the SCI Channel is in
 *  operation mode "uninitialized", the function <tt>EE_Sci_ReadRxData()</tt>
 *  shall return the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the SCI module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Sci_ReadRxData()</tt> shall return
 *  the development error <tt>E_SCI_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the SCI module is enabled: If the parameter
 *  <tt>DataPtr</tt> is a <tt>NULL</tt> pointer, the function
 *  <tt>EE_Sci_ReadTxData()</tt> shall return the development error
 *  <tt>E_SCI_PARAM_NULL</tt>.
 */
#ifdef	__SCI_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Sci_ReadRxData(
  EE_TYPESCICHANNEL	Channel,
  EE_TYPESCIDATA *	DataPtr
);
#endif	/* __SCI_READRXDATA_API__ */

#ifdef	__SCI_DISABLERX_API__
/** @brief	Channel Reception Disable
 *  @param[in]	Channel	Numeric identifier of the SCI channel.
 *  @return	Extended Status Only:
 *  	- E_OK:			SCI Disable Operation Success.
 *  	- E_NOT_OK:		SCI Channel NOT initialized.
 *  	- E_SCI_PARAM_CHANNEL:	SCI Channel Invalid.
 *
 *  The service instructs the driver to disable the reception for the addressed
 *  SCI channel.
 *
 *  If Extended Status for the SCI module is enabled: If the SCI Channel is in
 *  operation mode "uninitialized", the function <tt>EE_Sci_DisableRx()</tt>
 *  shall return the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the SCI module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Sci_DisableRx()</tt> shall return
 *  the development error <tt>E_SCI_PARAM_CHANNEL</tt>.
 */
#ifdef	__SCI_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Sci_DisableRx(
  EE_TYPESCICHANNEL	Channel
);

#ifdef	__SCI_ENABLERX_API__
/** @brief	Channel Reception Enable
 *  @param[in]	Channel	Numeric identifier of the SCI channel.
 *  @return	Extended Status Only:
 *  	- E_OK:			SCI Enable Operation Success.
 *  	- E_NOT_OK:		SCI Channel NOT initialized.
 *  	- E_SCI_PARAM_CHANNEL:	SCI Channel Invalid.
 *
 *  The service instructs the driver to enable the reception for the
 *  addressed SCI channel.
 *
 *  If Extended Status for the SCI module is enabled: If the SCI Channel is in
 *  operation mode "uninitialized", the function <tt>EE_Sci_EnableRx()</tt>
 *  shall return the development error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the SCI module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Sci_EnableRx()</tt> shall return
 *  the development error <tt>E_SCI_PARAM_CHANNEL</tt>.
 */
#ifdef	__SCI_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Sci_EnableRx(
  EE_TYPESCICHANNEL	Channel
);
#endif	/* __SCI_ENABLERX_API__ */
#endif	/* __SCI_DISABLERX_API__ */

#ifdef	__SCI_ENABLENOTIF_API__
/** @brief	Channel Notifications Enable
 *  @param[in]	Channel	Numeric identifier of the SCI channel.
 *  @return	Extended Status Only:
 *  	- E_OK:			Notification Enable Operation Success.
 *  	- E_NOT_OK:		SCI Channel NOT initialized.
 *  	- E_SCI_PARAM_CHANNEL:	SCI Channel Invalid.
 *  	- E_SCI_PARAM_CALLBACK:	SCI Channel Notification Call-Back Function
 *  				Pointers are <tt>NULL</tt>.
 *
 *  Enables the interrupt notifications for a channel.
 *
 *  The function <tt>EE_Sci_EnableNotifications()</tt> shall enable the
 *  interrupt notifications of the referenced channel configured for
 *  notifications.
 *
 *  The function shall save an attribute like "notifications enabled" of the
 *  channel.
 *
 *  @note	This attribute affects the interrupt notifications always when
 *  		the channel is in <tt>EE_SCI_CH_OPERATIONAL</tt> state or
 *  		substates.
 *
 *  The function <tt>EE_Sci_EnableNotifications()</tt> shall be reentrant, if
 *  the SCI channels used in concurrent calls are different.
 *
 *  If Extended Status for the SCI module is enabled: If the SCI Channel is in
 *  operation mode "uninitialized", the function shall return the development
 *  error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the SCI module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Sci_EnableNotification()</tt> shall
 *  return the development error <tt>E_SCI_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the SCI module is enabled: If no valid notification
 *  function is configured, the function <tt>EE_Sci_EnableNotification()</tt>
 *  shall raise the error <tt>E_SCI_PARAM_CALLBACK</tt>.
 */
#ifdef	__SCI_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Sci_EnableNotifications(
  EE_TYPESCICHANNEL	Channel
);

#ifdef	__SCI_DISABLENOTIF_API__
/** @brief	Channel Notifications Disable
 *  @param[in]	Channel	Numeric identifier of the SCI channel.
 *  @return	Extended Status Only:
 *  	- E_OK:			Notification Disable Operation Success.
 *  	- E_NOT_OK:		SCI Channel NOT initialized.
 *  	- E_SCI_PARAM_CHANNEL:	SCI Channel Invalid.
 *  	- E_SCI_PARAM_CALLBACK:	SCI Channel Notification Call-Back Function
 *  				Pointers are <tt>NULL</tt>.
 *
 *  Disables the interrupt notifications for a channel.
 *
 *  The function <tt>EE_Sci_DisableNotifications()</tt> shall disable the
 *  interrupt notifications of the referenced channel configured for
 *  notifications.
 *
 *  The function shall save an attribute like "notifications disabled" of the
 *  channel.
 *
 *  @note	This attribute affects the interrupt notifications always when
 *  		the channel is in <tt>EE_SCI_CH_OPERATIONAL</tt> state or
 *  		substates.
 *
 *  The function <tt>EE_Sci_DisableNotifications()</tt> shall be reentrant, if
 *  the SCI channels used in concurrent calls are different.
 *
 *  If Extended Status for the SCI module is enabled: If the SCI Channel is in
 *  operation mode "uninitialized", the function shall return the development
 *  error <tt>E_NOT_OK</tt>.
 *
 *  If Extended Status for the SCI module is enabled: If the parameter
 *  <tt>Channel</tt> is invalid (not within the range specified by
 *  implementation), the function <tt>EE_Sci_DisableNotification()</tt> shall
 *  return the development error <tt>E_SCI_PARAM_CHANNEL</tt>.
 *
 *  If Extended Status for the SCI module is enabled: If no valid notification
 *  function is configured, the function <tt>EE_Sci_DisableNotification()</tt>
 *  shall return the error <tt>E_SCI_PARAM_CALLBACK</tt>.
 */
#ifdef	__SCI_EXTENDED_STATUS__
EE_TYPERET
#else
void
#endif
EE_Sci_DisableNotifications(
  EE_TYPESCICHANNEL	Channel
);
#endif	/* __SCI_DISABLENOTIF_API__ */
#endif	/* __SCI_ENABLENOTIF_API__ */

#endif	/* __EE_MCU_COMMON_SCI_H__ */
