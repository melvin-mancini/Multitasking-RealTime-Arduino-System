# ###*B*###
# ERIKA Enterprise - a tiny RTOS for small microcontrollers
# 
# Copyright (C) 2002-2014  Evidence Srl
# 
# This file is part of ERIKA Enterprise.
# 
# ERIKA Enterprise is free software; you can redistribute it
# and/or modify it under the terms of the GNU General Public License
# version 2 as published by the Free Software Foundation, 
# (with a special exception described below).
# 
# Linking this code statically or dynamically with other modules is
# making a combined work based on this code.  Thus, the terms and
# conditions of the GNU General Public License cover the whole
# combination.
# 
# As a special exception, the copyright holders of this library give you
# permission to link this code with independent modules to produce an
# executable, regardless of the license terms of these independent
# modules, and to copy and distribute the resulting executable under
# terms of your choice, provided that you also meet, for each linked
# independent module, the terms and conditions of the license of that
# module.  An independent module is a module which is not derived from
# or based on this library.  If you modify this code, you may extend
# this exception to your version of the code, but you are not
# obligated to do so.  If you do not wish to do so, delete this
# exception statement from your version.
# 
# ERIKA Enterprise is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License version 2 for more details.
# 
# You should have received a copy of the GNU General Public License
# version 2 along with ERIKA Enterprise; if not, write to the
# Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301 USA.
# ###*E*###

##
## Author:  2014,  Giuseppe Serano
##

ifneq ($(ENABLE_LIBS), TRUE)

ifeq ($(call iseeopt, __ARDUINO_SDK__), yes)

ifndef	ARDUINO_SDK_FILES
export	ARDUINO_SDK_FILES = C:/arduino-1.0.5-r2
endif

ARDUINO_SDK_ROOT := \
		$(shell cygpath $(call short_native_path, $(ARDUINO_SDK_FILES)))

EE_VPATH += $(ARDUINO_SDK_ROOT)

EEOPT += ARDUINO=105

##
## Add the inc path to the include pathlist
## New include mechanism

INCLUDE_PATH :=	$(ARDUINO_SDK_ROOT)/hardware/arduino/cores/arduino	\
		$(INCLUDE_PATH)

ifeq	($(call iseeopt, __ARDUINO_UNO_328__), yes)
INCLUDE_PATH :=	$(ARDUINO_SDK_ROOT)/hardware/arduino/variants/standard	\
		$(INCLUDE_PATH)
endif

ifeq	($(call iseeopt, __ARDUINO_NANO_V3__), yes)
INCLUDE_PATH :=	$(ARDUINO_SDK_ROOT)/hardware/arduino/variants/eightanaloginputs	\
		$(INCLUDE_PATH)
endif

ifeq	($(call iseeopt, __ARDUINO_SDK_SIMUL__), yes)
EEOPT += -DF_CPU=1000000UL
else
EEOPT += -DF_CPU=16000000UL
endif

ifeq ($(call iseeopt, __ATMEGA__), yes)

ifeq ($(call iseeopt, __AVR8__), yes)

EE_SRCS += hardware/arduino/cores/arduino/avr-libc/malloc.c
EE_SRCS += hardware/arduino/cores/arduino/avr-libc/realloc.c

EE_SRCS += hardware/arduino/cores/arduino/wiring.c
EE_SRCS += hardware/arduino/cores/arduino/wiring_digital.c
EE_SRCS += hardware/arduino/cores/arduino/wiring_analog.c
EE_SRCS += hardware/arduino/cores/arduino/wiring_pulse.c
EE_SRCS += hardware/arduino/cores/arduino/wiring_shift.c
EE_SRCS += hardware/arduino/cores/arduino/WInterrupts.c

EE_SRCS += hardware/arduino/cores/arduino/CDC.cpp
EE_SRCS += hardware/arduino/cores/arduino/HardwareSerial.cpp
EE_SRCS += hardware/arduino/cores/arduino/HID.c
EE_SRCS += hardware/arduino/cores/arduino/IPAddress.cpp
EE_SRCS += hardware/arduino/cores/arduino/new.c
EE_SRCS += hardware/arduino/cores/arduino/Print.cpp
EE_SRCS += hardware/arduino/cores/arduino/Stream.cpp
EE_SRCS += hardware/arduino/cores/arduino/Tone.cpp
EE_SRCS += hardware/arduino/cores/arduino/USBCore.cpp
EE_SRCS += hardware/arduino/cores/arduino/WMath.cpp
EE_SRCS += hardware/arduino/cores/arduino/WString.cpp


ifeq	($(call iseeopt, __ARDUINO_SDK_LIB_EEPROM__), yes)
INCLUDE_PATH :=	$(ARDUINO_SDK_ROOT)/libraries/EEPROM		$(INCLUDE_PATH)
EE_SRCS += libraries/EEPROM/EEPROM.cpp
endif

ifeq	($(call iseeopt, __ARDUINO_SDK_LIB_ESPLORA__), yes)
INCLUDE_PATH :=	$(ARDUINO_SDK_ROOT)/libraries/Esplora		$(INCLUDE_PATH)
EE_SRCS += libraries/Esplora/Esplora.cpp
endif

ifeq	($(call iseeopt, __ARDUINO_SDK_LIB_ETHERNET__), yes)
INCLUDE_PATH :=	$(ARDUINO_SDK_ROOT)/libraries/Ethernet		\
		$(ARDUINO_SDK_ROOT)/libraries/Ethernet/utility	\
		$(INCLUDE_PATH)
EE_SRCS += libraries/Ethernet/Dhcp.cpp
EE_SRCS += libraries/Ethernet/Dns.cpp
EE_SRCS += libraries/Ethernet/Ethernet.cpp
EE_SRCS += libraries/Ethernet/EthernetClient.cpp
EE_SRCS += libraries/Ethernet/EthernetServer.cpp
EE_SRCS += libraries/Ethernet/EthernetUdp.cpp
EE_SRCS += libraries/Ethernet/utility/socket.cpp
EE_SRCS += libraries/Ethernet/utility/w5100.cpp
endif

ifeq	($(call iseeopt, __ARDUINO_SDK_LIB_FIRMATA__), yes)
INCLUDE_PATH :=	$(ARDUINO_SDK_ROOT)/libraries/Firmata		$(INCLUDE_PATH)
EE_SRCS += libraries/Firmata/Firmata.cpp
endif

ifeq	($(call iseeopt, __ARDUINO_SDK_LIB_GSM__), yes)
INCLUDE_PATH :=	$(ARDUINO_SDK_ROOT)/libraries/GSM		$(INCLUDE_PATH)
EE_SRCS += libraries/GSM/GSM3CircularBuffer.cpp
EE_SRCS += libraries/GSM/GSM3MobileAccessProvider.cpp
EE_SRCS += libraries/GSM/GSM3MobileCellManagement.cpp
EE_SRCS += libraries/GSM/GSM3MobileClientProvider.cpp
EE_SRCS += libraries/GSM/GSM3MobileClientService.cpp
EE_SRCS += libraries/GSM/GSM3MobileDataNetworkProvider.cpp
EE_SRCS += libraries/GSM/GSM3MobileMockupProvider.cpp
EE_SRCS += libraries/GSM/GSM3MobileNetworkProvider.cpp
EE_SRCS += libraries/GSM/GSM3MobileNetworkRegistry.cpp
EE_SRCS += libraries/GSM/GSM3MobileServerProvider.cpp
EE_SRCS += libraries/GSM/GSM3MobileServerService.cpp
EE_SRCS += libraries/GSM/GSM3MobileSMSProvider.cpp
EE_SRCS += libraries/GSM/GSM3MobileVoiceProvider.cpp
EE_SRCS += libraries/GSM/GSM3ShieldV1.cpp
EE_SRCS += libraries/GSM/GSM3ShieldV1AccessProvider.cpp
EE_SRCS += libraries/GSM/GSM3ShieldV1BandManagement.cpp
EE_SRCS += libraries/GSM/GSM3ShieldV1BaseProvider.cpp
EE_SRCS += libraries/GSM/GSM3ShieldV1CellManagement.cpp
EE_SRCS += libraries/GSM/GSM3ShieldV1ClientProvider.cpp
EE_SRCS += libraries/GSM/GSM3ShieldV1DataNetworkProvider.cpp
EE_SRCS += libraries/GSM/GSM3ShieldV1DirectModemProvider.cpp
EE_SRCS += libraries/GSM/GSM3ShieldV1ModemCore.cpp
EE_SRCS += libraries/GSM/GSM3ShieldV1ModemVerification.cpp
EE_SRCS += libraries/GSM/GSM3ShieldV1MultiClientProvider.cpp
EE_SRCS += libraries/GSM/GSM3ShieldV1MultiServerProvider.cpp
EE_SRCS += libraries/GSM/GSM3ShieldV1PinManagement.cpp
EE_SRCS += libraries/GSM/GSM3ShieldV1ScanNetworks.cpp
EE_SRCS += libraries/GSM/GSM3ShieldV1ServerProvider.cpp
EE_SRCS += libraries/GSM/GSM3ShieldV1SMSProvider.cpp
EE_SRCS += libraries/GSM/GSM3ShieldV1VoiceProvider.cpp
EE_SRCS += libraries/GSM/GSM3SMSService.cpp
EE_SRCS += libraries/GSM/GSM3SoftSerial.cpp
EE_SRCS += libraries/GSM/GSM3VoiceCallService.cpp
endif

ifeq	($(call iseeopt, __ARDUINO_SDK_LIB_LIQUID_CRYSTAL__), yes)
INCLUDE_PATH :=	$(ARDUINO_SDK_ROOT)/libraries/LiquidCrystal	$(INCLUDE_PATH)
EE_SRCS += libraries/LiquidCrystal/LiquidCrystal.cpp
endif

ifeq	($(call iseeopt, __ARDUINO_SDK_LIB_ROBOT_CONTROL__), yes)
INCLUDE_PATH :=	$(ARDUINO_SDK_ROOT)/libraries/Robot_Control		\
		$(ARDUINO_SDK_ROOT)/libraries/Robot_Control/utility	\
		$(INCLUDE_PATH)
EE_SRCS += libraries/Robot_Control/Adafruit_GFX.cpp
EE_SRCS += libraries/Robot_Control/Arduino_LCD.cpp
EE_SRCS += libraries/Robot_Control/ArduinoRobot.cpp
EE_SRCS += libraries/Robot_Control/communication.cpp
EE_SRCS += libraries/Robot_Control/Compass.cpp
EE_SRCS += libraries/Robot_Control/EasyTransfer2.cpp
EE_SRCS += libraries/Robot_Control/EEPROM_I2C.cpp
EE_SRCS += libraries/Robot_Control/Fat16.cpp
EE_SRCS += libraries/Robot_Control/glcdfont.c
EE_SRCS += libraries/Robot_Control/helper.cpp
EE_SRCS += libraries/Robot_Control/information.cpp
EE_SRCS += libraries/Robot_Control/keyboard.cpp
EE_SRCS += libraries/Robot_Control/lcd.cpp
EE_SRCS += libraries/Robot_Control/Melody.cpp
EE_SRCS += libraries/Robot_Control/Motors.cpp
EE_SRCS += libraries/Robot_Control/Multiplexer.cpp
EE_SRCS += libraries/Robot_Control/RobotSdCard.cpp
EE_SRCS += libraries/Robot_Control/SdCard.cpp
EE_SRCS += libraries/Robot_Control/Sensors.cpp
EE_SRCS += libraries/Robot_Control/SPI.cpp
EE_SRCS += libraries/Robot_Control/Squawk.cpp
EE_SRCS += libraries/Robot_Control/SquawkSD.cpp
EE_SRCS += libraries/Robot_Control/Wire.cpp
EE_SRCS += libraries/Robot_Control/utility/RobotTextManager.cpp
EE_SRCS += libraries/Robot_Control/utility/twi.c
EE_SRCS += libraries/Robot_Control/utility/VirtualKeyboard.cpp
endif

ifeq	($(call iseeopt, __ARDUINO_SDK_LIB_ROBOT_MOTOR__), yes)
INCLUDE_PATH :=	$(ARDUINO_SDK_ROOT)/libraries/Robot_Control	$(INCLUDE_PATH)
EE_SRCS += libraries/Robot_Motor/ArduinoRobotMotorBoard.cpp
EE_SRCS += libraries/Robot_Motor/EasyTransfer2.cpp
EE_SRCS += libraries/Robot_Motor/lineFellow.cpp
EE_SRCS += libraries/Robot_Motor/Multiplexer.cpp
endif

ifeq	($(call iseeopt, __ARDUINO_SDK_LIB_SD__), yes)
INCLUDE_PATH :=	$(ARDUINO_SDK_ROOT)/libraries/SD		\
		$(ARDUINO_SDK_ROOT)/libraries/SD/utility	\
		$(INCLUDE_PATH)
EE_SRCS += libraries/SD/File.cpp
EE_SRCS += libraries/SD/SD.cpp
EE_SRCS += libraries/SD/utility/Sd2Card.cpp
EE_SRCS += libraries/SD/utility/SdFile.cpp
EE_SRCS += libraries/SD/utility/SdVolume.cpp
endif

ifeq	($(call iseeopt, __ARDUINO_SDK_LIB_SERVO__), yes)
INCLUDE_PATH :=	$(ARDUINO_SDK_ROOT)/libraries/Servo		$(INCLUDE_PATH)
EE_SRCS += libraries/Servo/Servo.cpp
endif

ifeq	($(call iseeopt, __ARDUINO_SDK_LIB_SOFTWARE_SERIAL__), yes)
INCLUDE_PATH :=	$(ARDUINO_SDK_ROOT)/libraries/SoftwareSerial	$(INCLUDE_PATH)
EE_SRCS += libraries/SoftwareSerial/SoftwareSerial.cpp
endif

ifeq	($(call iseeopt, __ARDUINO_SDK_LIB_SPI__), yes)
INCLUDE_PATH :=	$(ARDUINO_SDK_ROOT)/libraries/SPI $(INCLUDE_PATH)
EE_SRCS += libraries/SPI/SPI.cpp
endif

ifeq	($(call iseeopt, __ARDUINO_SDK_LIB_STEPPER__), yes)
INCLUDE_PATH :=	$(ARDUINO_SDK_ROOT)/libraries/Stepper		$(INCLUDE_PATH)
EE_SRCS += libraries/Stepper/Stepper.cpp
endif

ifeq	($(call iseeopt, __ARDUINO_SDK_LIB_TFT__), yes)
INCLUDE_PATH :=	$(ARDUINO_SDK_ROOT)/libraries/TFT		\
		$(ARDUINO_SDK_ROOT)/libraries/TFT/utility	\
		$(INCLUDE_PATH)
EE_SRCS += libraries/TFT/TFT.cpp
EE_SRCS += libraries/TFT/utility/Adafruit_GFX.cpp
EE_SRCS += libraries/TFT/utility/ST7735.cpp
EE_SRCS += libraries/TFT/utility/glcdfont.c
endif

ifeq	($(call iseeopt, __ARDUINO_SDK_LIB_TIMER1__), yes)
INCLUDE_PATH :=	$(ARDUINO_SDK_ROOT)/libraries/Timer1		$(INCLUDE_PATH)
EE_SRCS += libraries/Timer1/Timer1.cpp
endif

ifeq	($(call iseeopt, __ARDUINO_SDK_LIB_WIFI__), yes)
INCLUDE_PATH :=	$(ARDUINO_SDK_ROOT)/libraries/WiFi		\
		$(ARDUINO_SDK_ROOT)/libraries/WiFi/utility	\
		$(INCLUDE_PATH)
EE_SRCS += libraries/WiFi/WiFi.cpp
EE_SRCS += libraries/WiFi/WiFiClient.cpp
EE_SRCS += libraries/WiFi/WiFiServer.cpp
EE_SRCS += libraries/WiFi/WiFiUdp.cpp
EE_SRCS += libraries/WiFi/utility/server_drv.cpp
EE_SRCS += libraries/WiFi/utility/spi_drv.cpp
EE_SRCS += libraries/WiFi/utility/wifi_drv.cpp
endif

ifeq	($(call iseeopt, __ARDUINO_SDK_LIB_WIRE__), yes)
INCLUDE_PATH :=	$(ARDUINO_SDK_ROOT)/libraries/Wire		\
		$(ARDUINO_SDK_ROOT)/libraries/Wire/utility	\
		$(INCLUDE_PATH)
EE_SRCS += libraries/Wire/Wire.cpp
EE_SRCS += libraries/Wire/utility/twi.cpp
endif

ifeq	($(call iseeopt, __ARDUINO_SDK_LIB_TIMER1__), yes)
INCLUDE_PATH :=	$(ARDUINO_SDK_ROOT)/libraries/Timer1 $(INCLUDE_PATH)
EE_SRCS += libraries/Timer1/Timer1.cpp
endif

endif	# __AVR8__

endif	# __ATMEGA__

endif	# __ARDUINO_SDK__

endif	# ENABLE_LIBS

