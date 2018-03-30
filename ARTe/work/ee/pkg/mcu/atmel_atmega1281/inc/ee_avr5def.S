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


.equ PINA   , 0X00
.equ PINA7  , 7
.equ PINA6  , 6
.equ PINA5  , 5
.equ PINA4  , 4
.equ PINA3  , 3
.equ PINA2  , 2
.equ PINA1  , 1
.equ PINA0  , 

.equ DDRA   , 0X01
.equ DDA7   , 7
.equ DDA6   , 6
.equ DDA5   , 5
.equ DDA4   , 4
.equ DDA3   , 3
.equ DDA2   , 2
.equ DDA1   , 1
.equ DDA0   , 0

.equ PORTA  , 0X02
.equ PA7    , 7
.equ PA6    , 6
.equ PA5    , 5
.equ PA4    , 4
.equ PA3    , 3
.equ PA2    , 2
.equ PA1    , 1
.equ PA0    , 0

.equ PINB   , 0X03
.equ PINB7  , 7
.equ PINB6  , 6
.equ PINB5  , 5
.equ PINB4  , 4
.equ PINB3  , 3
.equ PINB2  , 2
.equ PINB1  , 1
.equ PINB0  , 0

.equ DDRB   , 0x04 
.equ DDB7   , 7
.equ DDB6   , 6
.equ DDB5   , 5
.equ DDB4   , 4
.equ DDB3   , 3
.equ DDB2   , 2
.equ DDB1   , 1
.equ DDB0   , 0

.equ PORTB  , 0x05
.equ PB7    , 7
.equ PB6    , 6
.equ PB5    , 5
.equ PB4    , 4
.equ PB3    , 3
.equ PB2    , 2
.equ PB1    , 1
.equ PB0    , 0

.equ PINC   , 0x06
.equ PINC7  , 7
.equ PINC6  , 6
.equ PINC5  , 5
.equ PINC4  , 4
.equ PINC3  , 3
.equ PINC2  , 2
.equ PINC1  , 1
.equ PINC0  , 0

.equ DDRC   , 0x07
.equ DDC7   , 7
.equ DDC6   , 6
.equ DDC5   , 5
.equ DDC4   , 4
.equ DDC3   , 3
.equ DDC2   , 2
.equ DDC1   , 1
.equ DDC0   , 0

.equ PORTC   , 0x08
.equ PC7     , 7
.equ PC6     , 6
.equ PC5     , 5
.equ PC4     , 4
.equ PC3     , 3
.equ PC2     , 2
.equ PC1     , 1
.equ PC0     , 0

.equ PIND   , 0x09
.equ PIND7  , 7
.equ PIND6  , 6
.equ PIND5  , 5
.equ PIND4  , 4
.equ PIND3  , 3
.equ PIND2  , 2
.equ PIND1  , 1
.equ PIND0  , 0

.equ DDRD   , 0x0A
.equ DDD7   , 7
.equ DDD6   , 6
.equ DDD5   , 5
.equ DDD4   , 4
.equ DDD3   , 3
.equ DDD2   , 2
.equ DDD1   , 1
.equ DDD0   , 0

.equ PORTD  ,  0x0B
.equ PD7    , 7
.equ PD6    , 6
.equ PD5    , 5
.equ PD4    , 4
.equ PD3    , 3
.equ PD2    , 2
.equ PD1    , 1
.equ PD0    , 0

 .equ PINE   ,  0x0C
 .equ PINE7  , 7
 .equ PINE6  , 6
 .equ PINE5  , 5
 .equ PINE4  , 4
 .equ PINE3  , 3
 .equ PINE2  , 2
 .equ PINE1  , 1
 .equ PINE0  , 0

 .equ DDRE   ,  0x0D
 .equ DDE7   , 7
 .equ DDE6   , 6
 .equ DDE5   , 5
 .equ DDE4   , 4
 .equ DDE3   , 3
 .equ DDE2   , 2
 .equ DDE1   , 1
 .equ DDE0   , 0

 .equ PORTE   , 0x0E
 .equ PE7     , 7
 .equ PE6     , 6
 .equ PE5    , 5
 .equ PE4    , 4
 .equ PE3    , 3
 .equ PE2    , 2
 .equ PE1    , 1
 .equ PE0    , 0

 .equ PINF   , 0x0F
 .equ PINF7  , 7
 .equ PINF6  , 6
 .equ PINF5  , 5
 .equ PINF4  , 4
 .equ PINF3  , 3
 .equ PINF2  , 2
 .equ PINF1  , 1
 .equ PINF0  , 0

 .equ DDRF   ,  0x10
 .equ DDF7   , 7
 .equ DDF6   , 6
 .equ DDF5   , 5
 .equ DDF4   , 4
 .equ DDF3   , 3
 .equ DDF2   , 2
 .equ DDF1   , 1
 .equ DDF0   , 0

 .equ PORTF  , 0x11
 .equ PF7    , 7
 .equ PF6    , 6
 .equ PF5    , 5
 .equ PF4    , 4
 .equ PF3    , 3
 .equ PF2    , 2
 .equ PF1    , 1
 .equ PF0    , 0

 .equ PING   , 0x12
 .equ PING5  , 5
 .equ PING4  , 4
 .equ PING3  , 3
 .equ PING2  , 2
 .equ PING1  , 1
 .equ PING0  , 0

 .equ DDRG   , 0x13
 .equ DDG5   , 5
 .equ DDG4   , 4
 .equ DDG3   , 3
 .equ DDG2   , 2
 .equ DDG1   , 1
 .equ DDG0   , 0

 .equ PORTG  ,  0x14
 .equ PG5    , 5
 .equ PG4    , 4
 .equ PG3    , 3
 .equ PG2    , 2
 .equ PG1    , 1
 .equ PG0    , 0

 .equ TIFR0  ,  0x15
 .equ OCF0B  , 2
 .equ OCF0A  , 1
 .equ TOV0   , 0

 .equ TIFR1  , 0x16
 .equ ICF1   , 5
 .equ OCF1C  , 3
 .equ OCF1B  , 2
 .equ OCF1A  , 1
 .equ TOV1   , 0

 .equ TIFR2  ,  0x17
 .equ OCF2B  , 2
 .equ OCF2A  , 1
 .equ TOV2   , 0

 .equ TIFR3  ,  0x18
 .equ ICF3   , 5
 .equ OCF3C  , 3
 .equ OCF3B  , 2
 .equ OCF3A  , 1
 .equ TOV3   , 0

 .equ TIFR4   , 0x19
 .equ ICF4   , 5
 .equ OCF4C  , 3
 .equ OCF4B  , 2
 .equ OCF4A  , 1
 .equ TOV4   , 0

 .equ TIFR5  ,  0x1A
 .equ ICF5   , 5
 .equ OCF5C  , 3
 .equ OCF5B  , 2
 .equ OCF5A  , 1
 .equ TOV5   , 0


 .equ PCIFR  ,  0x1B
 .equ PCIF1  , 1
 .equ PCIF0  , 0

 .equ EIFR   , 0x1C
 .equ INTF7  , 7
 .equ INTF6  , 6
 .equ INTF5  , 5
 .equ INTF4  , 4
 .equ INTF3  , 3
 .equ INTF2  , 2
 .equ INTF1  , 1
 .equ INTF0  , 0

 .equ EIMSK   , 0x1D
 .equ INT7   , 7
 .equ INT6   , 6
 .equ INT5   , 5
 .equ INT4   , 4
 .equ INT3   , 3
 .equ INT2   , 2
 .equ INT1   , 1 
 .equ INT0   , 0

 .equ GPIOR0  , 0x1E

 .equ EECR   , 0x1F
 .equ EEPM1  , 5
 .equ EEPM0  , 4
 .equ EERIE  , 3
 .equ EEMPE  , 2
 .equ EEPE   , 1
 .equ EERE   , 0

 .equ EEDR   , 0X20


 .equ EEAR   , 0x21

 .equ EEARL  ,  0x21
 .equ EEARH  ,  0X22

 .equ __EEPROM_REG_LOCATIONS__ , 1F2021

 .equ GTCCR   , 0x23
 .equ TSM     , 7
 .equ PSRASY  , 1
 .equ PSRSYNC , 0

 .equ TCCR0A  , 0x24
 .equ COM0A1  , 7
 .equ COM0A0  , 6
 .equ COM0B1  , 5
 .equ COM0B0  , 4
 .equ WGM01   , 1
 .equ WGM00   , 0

 .equ TCCR0B  , 0x25
 .equ FOC0A   , 7
 .equ FOC0B   , 6
 .equ WGM02   , 3
 .equ CS02    , 2
 .equ CS01    , 1
 .equ CS00    , 0

 .equ TCNT0   , 0X26

 .equ OCR0A   , 0X27

 .equ OCR0B   , 0X28

 .equ GPIOR1  , 0x2A

 .equ GPIOR2  , 0x2B

 .equ SPCR    , 0x2C
 .equ SPIE   , 7
 .equ SPE    , 6
 .equ DORD   , 5
 .equ MSTR   , 4
 .equ CPOL   , 3
 .equ CPHA   , 2
 .equ SPR1   , 1
 .equ SPR0   , 0

 .equ SPSR    , 0x2D
 .equ SPIF   , 7
 .equ WCOL   , 6
 .equ SPI2X  , 0

 .equ SPDR    , 0X2E

 .equ ACSR    , 0x30
 .equ ACD    , 7
 .equ ACBG   , 6
 .equ ACO    , 5
 .equ ACI    , 4
 .equ ACIE   , 3
 .equ ACIC   , 2
 .equ ACIS1  , 1
 .equ ACIS0  , 0

 .equ MONDR   , 0x31
 .equ OCDR    , 0x31
 .equ IDRD   , 7
 .equ OCDR7  , 7
 .equ OCDR6  , 6
 .equ OCDR5  , 5
 .equ OCDR4  , 4
 .equ OCDR3  , 3
 .equ OCDR2  , 2
 .equ OCDR1  , 1
 .equ OCDR0  , 0

 .equ SMCR    , 0x33
 .equ SM2    , 3
 .equ SM1    , 2
 .equ SM0    , 1
 .equ SE     , 0

 .equ MCUSR   , 0x34
 .equ JTRF   , 4
 .equ WDRF   , 3
 .equ BORF   , 2
 .equ EXTRF  , 1
 .equ PORF   , 0

 .equ MCUCR   , 0X35
 .equ JTD    , 7
 .equ PUD    , 4
 .equ IVSEL  , 1
 .equ IVCE   , 0

 .equ SPMCSR  , 0x37
 .equ SPMIE  , 7
 .equ RWWSB  , 6
 .equ SIGRD  , 5
 .equ RWWSRE , 4
 .equ BLBSET , 3
 .equ PGWRT  , 2
 .equ PGERS  , 1
 .equ SPMEN  , 0

 .equ RAMPZ   , 0X3B
 .equ RAMPZ0  , 0

 
 .equ WDTCSR  , 0x60
 .equ WDIF   , 7
 .equ WDIE   , 6
 .equ WDP3   , 5
 .equ WDCE   , 4
 .equ WDE    , 3
 .equ WDP2   , 2
 .equ WDP1   , 1
 .equ WDP0   , 0

 .equ CLKPR  , 0x61
 .equ CLKPCE , 7
 .equ CLKPS3 , 3
 .equ CLKPS2 , 2
 .equ CLKPS1 , 1
 .equ CLKPS0 , 0

 .equ PRR0    , 0x64
 .equ PRTWI      , 7
 .equ PRTIM2     , 6
 .equ PRTIM0     , 5
 .equ PRTIM1     , 3
 .equ PRSPI      , 2
 .equ PRUSART0   , 1
 .equ PRADC      , 0

 .equ PRR1    , 0x65
 .equ PRTIM5     , 5
 .equ PRTIM4     , 4
 .equ PRTIM3     , 3
 .equ PRUSART3   , 2
 .equ PRUSART2   , 1
 .equ PRUSART1   , 0

 .equ OSCCAL  , 0x66

 .equ PCICR  , 0x68
 .equ PCIE1  , 1
 .equ PCIE0  , 0



 .equ EICRA   , 0x69
 .equ ISC31  , 7
 .equ ISC30  , 6
 .equ ISC21  , 5
 .equ ISC20  , 4
 .equ ISC11  , 3
 .equ ISC10  , 2
 .equ ISC01  , 1
 .equ ISC00  , 0

 .equ EICRB   , 0x6A
 .equ ISC71   , 7
 .equ ISC70   , 6
 .equ ISC61   , 5
 .equ ISC60  , 4
 .equ ISC51  , 3
 .equ ISC50  , 2
 .equ ISC41  , 1
 .equ ISC40  , 0

 .equ PCMSK0  , 0x6B
 .equ PCINT7  , 7
 .equ PCINT6 , 6
 .equ PCINT5 , 5
 .equ PCINT4 , 4
 .equ PCINT3 , 3
 .equ PCINT2 , 2
 .equ PCINT1 , 1
 .equ PCINT0 , 0

 .equ PCMSK1  , 0x6C
 .equ PCINT15 , 7
 .equ PCINT14 , 6
 .equ PCINT13 , 5
 .equ PCINT12 , 4
 .equ PCINT11 , 3
 .equ PCINT10 , 2
 .equ PCINT9  , 1
 .equ PCINT8  , 0

 
 .equ TIMSK0  , 0x6E
 .equ OCIE0B  , 2
 .equ OCIE0A  , 1
 .equ TOIE0   , 0

 .equ TIMSK1  , 0x6F
 .equ ICIE1   , 5
 .equ OCIE1C  , 3
 .equ OCIE1B  , 2
 .equ OCIE1A  , 1
 .equ TOIE1   , 0

 .equ TIMSK2  , 0x70
 .equ OCIE2B  , 2
 .equ OCIE2A  , 1
 .equ TOIE2   , 0

 .equ TIMSK3  , 0x71
 .equ ICIE3  , 5
 .equ OCIE3C , 3
 .equ OCIE3B , 2
 .equ OCIE3A , 1
 .equ TOIE3  , 0

 .equ TIMSK4  , 0x72
 .equ ICIE4  , 5
 .equ OCIE4C , 3
 .equ OCIE4B , 2
 .equ OCIE4A , 1
 .equ TOIE4  , 0

 .equ TIMSK5  , 0x73
 .equ ICIE5  , 5
 .equ OCIE5C , 3
 .equ OCIE5B , 2
 .equ OCIE5A , 1
 .equ TOIE5  , 0

 .equ XMCRA   , 0x74
 .equ SRE    , 7
 .equ SRL2   , 6
 .equ SRL1   , 5
 .equ SRL0   , 4
 .equ SRW11  , 3
 .equ SRW10  , 2
 .equ SRW01  , 1
 .equ SRW00  , 0

 .equ XMCRB   , 0x75
 .equ XMBK   , 7
 .equ XMM2   , 2
 .equ XMM1   , 1
 .equ XMM0   , 0

 
 .equ ADCW   , 0x78
 .equ ADCL   , 0x78
 .equ ADCH   , 0x79

 .equ ADCSRA , 0x7A
 .equ ADEN   , 7
 .equ ADSC   , 6
 .equ ADATE  , 5
 .equ ADIF   , 4
 .equ ADIE   , 3
 .equ ADPS2  , 2
 .equ ADPS1  , 1
 .equ ADPS0  , 0

 .equ ADCSRB  , 0x7B
 .equ ACME    , 6
 
 .equ ADTS2  , 2
 .equ ADTS1  , 1
 .equ ADTS0  , 0

 .equ ADMUX   , 0x7C
 .equ REFS1  , 7
 .equ REFS0  , 6
 .equ ADLAR  , 5
 .equ MUX4   , 4
 .equ MUX3   , 3
 .equ MUX2   , 2
 .equ MUX1   , 1
 .equ MUX0   , 0

 .equ DIDR2   , 0x7D
 .equ ADC15D , 6
 .equ ADC14D , 7
 .equ ADC13D , 5
 .equ ADC12D , 4
 .equ ADC11D , 3
 .equ ADC10D , 2
 .equ ADC9D  , 1
 .equ ADC8D  , 0

 .equ DIDR0   , 0x7E
 .equ ADC7D   , 7
 .equ ADC6D  , 6
 .equ ADC5D  , 5
 .equ ADC4D  , 4
 .equ ADC3D  , 3
 .equ ADC2D  , 2
 .equ ADC1D  , 1
 .equ ADC0D  , 0

 .equ DIDR1   , 0x7F
 .equ AIN1D  , 1
 .equ AIN0D  , 0

 .equ TCCR1A  , 0x80
 .equ COM1A1 , 7
 .equ COM1A0 , 6
 .equ COM1B1 , 5
 .equ COM1B0 , 4
 .equ COM1C1 , 3
 .equ COM1C0 , 2
 .equ WGM11  , 1
 .equ WGM10  , 0

 .equ TCCR1B  , 0x81
 .equ ICNC1  , 7
 .equ ICES1  , 6
 .equ WGM13  , 4
 .equ WGM12  , 3
 .equ CS12   , 2
 .equ CS11   , 1
 .equ CS10   , 0

 .equ TCCR1C  , 0x82
 .equ FOC1A  , 7
 .equ FOC1B  , 6
 .equ FOC1C  , 5



 .equ TCNT1   , 0x84

 .equ TCNT1L  , 0x84
 .equ TCNT1H  , 0x85


 .equ ICR1    , 0x86

 .equ ICR1L   , 0x86
 .equ ICR1H   , 0x87


 .equ OCR1A   , 0x88

 .equ OCR1AL  , 0x88
 .equ OCR1AH  , 0x89


 .equ OCR1B   , 0x8A

 .equ OCR1BL  , 0x8A
 .equ OCR1BH  , 0x8B

 .equ OCR1C   , 0x8C

 .equ OCR1CL  , 0x8C
 .equ OCR1CH  , 0x8D


 .equ TCCR3A  , 0x90
 .equ COM3A1 , 7
 .equ COM3A0 , 6
 .equ COM3B1 , 5
 .equ COM3B0 , 4
 .equ COM3C1 , 3
 .equ COM3C0 , 2
 .equ WGM31  , 1
 .equ WGM30  , 0

 .equ TCCR3B  , 0x91
 .equ ICNC3  , 7
 .equ ICES3  , 6
 .equ WGM33  , 4
 .equ WGM32  , 3
 .equ CS32   , 2
 .equ CS31   , 1
 .equ CS30   , 0

 .equ TCCR3C  , 0x92
 .equ FOC3A  , 7
 .equ FOC3B  , 6
 .equ FOC3C  , 5


 .equ TCNT3   , 0x94

 .equ TCNT3L  , 0x94
 .equ TCNT3H  , 0x95

 .equ ICR3    , 0x96

 .equ ICR3L   , 0x96
 .equ ICR3H   , 0x97

 .equ OCR3A   , 0x98

 .equ OCR3AL  , 0x98
 .equ OCR3AH  , 0x99

 .equ OCR3B   , 0x9A

 .equ OCR3BL  , 0x9A
 .equ OCR3BH  , 0x9B

 .equ OCR3C   , 0x9C

 .equ OCR3CL  , 0x9C
 .equ OCR3CH  , 0x9D


 .equ TCCR4A  , 0xA0
 .equ COM4A1 , 7
 .equ COM4A0 , 6
 .equ COM4B1 , 5
 .equ COM4B0 , 4
 .equ COM4C1 , 3
 .equ COM4C0 , 2
 .equ WGM41  , 1
 .equ WGM40  , 0

 .equ TCCR4B  , 0xA1
 .equ ICNC4  , 7
 .equ ICES4  , 6
 .equ WGM43  , 4
 .equ WGM42  , 3
 .equ CS42   , 2
 .equ CS41   , 1
 .equ CS40   , 0

 .equ TCCR4C  , 0xA2
 .equ FOC4A  , 7
 .equ FOC4B  , 6
 .equ FOC4C  , 5


 .equ TCNT4   , 0xA4

 .equ TCNT4L  , 0xA4
 .equ TCNT4H  , 0xA5

 .equ ICR4    , 0xA6
 .equ ICR4L   , 0xA6
 .equ ICR4H   , 0xA7

 .equ OCR4A   , 0xA8

 .equ OCR4AL  , 0xA8
 .equ OCR4AH  , 0xA9

 .equ OCR4B   , 0xAA

 .equ OCR4BL  , 0xAA
 .equ OCR4BH  , 0xAB

 .equ OCR4C   , 0xAC

 .equ OCR4CL  , 0xAC
 .equ OCR4CH  , 0xAD


 .equ TCCR2A  , 0xB0
 .equ COM2A1 , 7
 .equ COM2A0 , 6
 .equ COM2B1 , 5
 .equ COM2B0 , 4
 .equ WGM21  , 1
 .equ WGM20  , 0

 .equ TCCR2B  , 0xB1
 .equ FOC2A  , 7
 .equ FOC2B  , 6
 .equ WGM22  , 3
 .equ CS22   , 2
 .equ CS21   , 1
 .equ CS20   , 0

 .equ TCNT2   , 0xB2

 .equ OCR2A   , 0xB3

 .equ OCR2B   , 0xB4


 .equ ASSR    , 0xB6
 .equ EXCLK  , 6
 .equ AS2    , 5
 .equ TCN2UB  , 4
 .equ OCR2AUB  , 3
 .equ OCR2BUB  , 2
 .equ TCR2AUB  , 1
 .equ TCR2BUB  , 0


 .equ TWBR    , 0xB8

 .equ TWSR    , 0xB9
 .equ TWS7   , 7
 .equ TWS6   , 6
 .equ TWS5   , 5
 .equ TWS4   , 4
 .equ TWS3   , 3
 .equ TWPS1  , 1
 .equ TWPS0  , 0

 .equ TWAR    , 0xBA
 .equ TWA6   , 7
 .equ TWA5   , 6
 .equ TWA4   , 5
 .equ TWA3   , 4
 .equ TWA2   , 3
 .equ TWA1   , 2
 .equ TWA0   , 1
 .equ TWGCE  , 0

 .equ TWDR    , 0xBB

 .equ TWCR    , 0xBC
 .equ TWINT  , 7
 .equ TWEA   , 6
 .equ TWSTA  , 5
 .equ TWSTO  , 4
 .equ TWWC   , 3
 .equ TWEN   , 2
 .equ TWIE   , 0

 .equ TWAMR   , 0xBD
 .equ TWAM6  , 7
 .equ TWAM5  , 6
 .equ TWAM4  , 5
 .equ TWAM3  , 4
 .equ TWAM2  , 3
 .equ TWAM1  , 2
 .equ TWAM0  , 1


 .equ UCSR0A  , 0xC0
 .equ RXC0   , 7
 .equ TXC0   , 6
 .equ UDRE0  , 5
 .equ FE0    , 4
 .equ DOR0   , 3
 .equ UPE0   , 2
 .equ U2X0   , 1
 .equ MPCM0  , 0

 .equ UCSR0B  , 0XC1
 .equ RXCIE0 , 7
 .equ TXCIE0 , 6
 .equ UDRIE0 , 5
 .equ RXEN0  , 4
 .equ TXEN0  , 3
 .equ UCSZ02 , 2
 .equ RXB80  , 1
 .equ TXB80  , 0

 .equ UCSR0C  , 0xC2
 .equ UMSEL01  , 7
 .equ UMSEL00  , 6
 .equ UPM01   , 5
 .equ UPM00  , 4
 .equ USBS0  , 3
 .equ UCSZ01 , 2
 .equ UCSZ00 , 1
 .equ UCPOL0 , 0


 .equ UBRR0   , 0xC4
 .equ UBRR0L  , 0xC4
 .equ UBRR0H  , 0xC5
 .equ UDR0    , 0XC6


 .equ UCSR1A  , 0xC8
 .equ RXC1   , 7
 .equ TXC1   , 6
 .equ UDRE1  , 5
 .equ FE1    , 4
 .equ DOR1   , 3
 .equ UPE1   , 2
 .equ U2X1   , 1
 .equ MPCM1  , 0

 .equ UCSR1B  , 0XC9
 .equ RXCIE1  , 7
 .equ TXCIE1 , 6
 .equ UDRIE1 , 5
 .equ RXEN1  , 4
 .equ TXEN1  , 3
 .equ UCSZ12 , 2
 .equ RXB81  , 1
 .equ TXB81  , 0

 .equ UCSR1C  , 0xCA
 .equ UMSEL11  , 7
 .equ UMSEL10  , 6
 .equ UPM11  , 5
 .equ UPM10  , 4
 .equ USBS1  , 3
 .equ UCSZ11 , 2
 .equ UCSZ10 , 1
 .equ UCPOL1 , 0


 .equ UBRR1   , 0xCC

 .equ UBRR1L  , 0xCC
 .equ UBRR1H  , 0xCD
 .equ UDR1    , 0XCE


 .equ TCCR5A  , 0x120
 .equ COM5A1 , 7
 .equ COM5A0 , 6
 .equ COM5B1 , 5
 .equ COM5B0 , 4
 .equ COM5C1 , 3
 .equ COM5C0 , 2
 .equ WGM51  , 1
 .equ WGM50  , 0

 .equ TCCR5B  , 0x121
 .equ ICNC5  , 7
 .equ ICES5  , 6
 .equ WGM53  , 4
 .equ WGM52  , 3
 .equ CS52   , 2
 .equ CS51   , 1
 .equ CS50   , 0

 .equ TCCR5C  , 0x122
 .equ FOC5A  , 7
 .equ FOC5B  , 6
 .equ FOC5C  , 5


 .equ TCNT5   , 0x124

 .equ TCNT5L  , 0x124
 .equ TCNT5H  , 0x125

 .equ ICR5    , 0x126

 .equ ICR5L   , 0x126
 .equ ICR5H   , 0x127

 .equ OCR5A   , 0x128

 .equ OCR5AL  , 0x128
 .equ OCR5AH  , 0x129

 .equ OCR5B   , 0x12A

 .equ OCR5BL  , 0x12A
 .equ OCR5BH  , 0x12B

 .equ OCR5C   , 0x12C

 .equ OCR5CL  , 0x12C
 .equ OCR5CH  , 0x12D

 



