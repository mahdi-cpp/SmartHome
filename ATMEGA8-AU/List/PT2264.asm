
;CodeVisionAVR C Compiler V2.05.3 Standard
;(C) Copyright 1998-2011 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com

;Chip type                : ATmega8
;Program type             : Application
;Clock frequency          : 16.000000 MHz
;Memory model             : Small
;Optimize for             : Size
;(s)printf features       : int, width
;(s)scanf features        : int, width
;External RAM size        : 0
;Data Stack size          : 256 byte(s)
;Heap size                : 0 byte(s)
;Promote 'char' to 'int'  : Yes
;'char' is unsigned       : Yes
;8 bit enums              : Yes
;Global 'const' stored in FLASH     : No
;Enhanced function parameter passing: Yes
;Enhanced core instructions         : On
;Smart register allocation          : On
;Automatic register allocation      : On

	#pragma AVRPART ADMIN PART_NAME ATmega8
	#pragma AVRPART MEMORY PROG_FLASH 8192
	#pragma AVRPART MEMORY EEPROM 512
	#pragma AVRPART MEMORY INT_SRAM SIZE 1119
	#pragma AVRPART MEMORY INT_SRAM START_ADDR 0x60

	.LISTMAC
	.EQU UDRE=0x5
	.EQU RXC=0x7
	.EQU USR=0xB
	.EQU UDR=0xC
	.EQU SPSR=0xE
	.EQU SPDR=0xF
	.EQU EERE=0x0
	.EQU EEWE=0x1
	.EQU EEMWE=0x2
	.EQU EECR=0x1C
	.EQU EEDR=0x1D
	.EQU EEARL=0x1E
	.EQU EEARH=0x1F
	.EQU WDTCR=0x21
	.EQU MCUCR=0x35
	.EQU GICR=0x3B
	.EQU SPL=0x3D
	.EQU SPH=0x3E
	.EQU SREG=0x3F

	.DEF R0X0=R0
	.DEF R0X1=R1
	.DEF R0X2=R2
	.DEF R0X3=R3
	.DEF R0X4=R4
	.DEF R0X5=R5
	.DEF R0X6=R6
	.DEF R0X7=R7
	.DEF R0X8=R8
	.DEF R0X9=R9
	.DEF R0XA=R10
	.DEF R0XB=R11
	.DEF R0XC=R12
	.DEF R0XD=R13
	.DEF R0XE=R14
	.DEF R0XF=R15
	.DEF R0X10=R16
	.DEF R0X11=R17
	.DEF R0X12=R18
	.DEF R0X13=R19
	.DEF R0X14=R20
	.DEF R0X15=R21
	.DEF R0X16=R22
	.DEF R0X17=R23
	.DEF R0X18=R24
	.DEF R0X19=R25
	.DEF R0X1A=R26
	.DEF R0X1B=R27
	.DEF R0X1C=R28
	.DEF R0X1D=R29
	.DEF R0X1E=R30
	.DEF R0X1F=R31

	.EQU __SRAM_START=0x0060
	.EQU __SRAM_END=0x045F
	.EQU __DSTACK_SIZE=0x0100
	.EQU __HEAP_SIZE=0x0000
	.EQU __CLEAR_SRAM_SIZE=__SRAM_END-__SRAM_START+1

	.MACRO __CPD1N
	CPI  R30,LOW(@0)
	LDI  R26,HIGH(@0)
	CPC  R31,R26
	LDI  R26,BYTE3(@0)
	CPC  R22,R26
	LDI  R26,BYTE4(@0)
	CPC  R23,R26
	.ENDM

	.MACRO __CPD2N
	CPI  R26,LOW(@0)
	LDI  R30,HIGH(@0)
	CPC  R27,R30
	LDI  R30,BYTE3(@0)
	CPC  R24,R30
	LDI  R30,BYTE4(@0)
	CPC  R25,R30
	.ENDM

	.MACRO __CPWRR
	CP   R@0,R@2
	CPC  R@1,R@3
	.ENDM

	.MACRO __CPWRN
	CPI  R@0,LOW(@2)
	LDI  R30,HIGH(@2)
	CPC  R@1,R30
	.ENDM

	.MACRO __ADDB1MN
	SUBI R30,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDB2MN
	SUBI R26,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDW1MN
	SUBI R30,LOW(-@0-(@1))
	SBCI R31,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW2MN
	SUBI R26,LOW(-@0-(@1))
	SBCI R27,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	SBCI R22,BYTE3(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1N
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	SBCI R22,BYTE3(-@0)
	SBCI R23,BYTE4(-@0)
	.ENDM

	.MACRO __ADDD2N
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	SBCI R24,BYTE3(-@0)
	SBCI R25,BYTE4(-@0)
	.ENDM

	.MACRO __SUBD1N
	SUBI R30,LOW(@0)
	SBCI R31,HIGH(@0)
	SBCI R22,BYTE3(@0)
	SBCI R23,BYTE4(@0)
	.ENDM

	.MACRO __SUBD2N
	SUBI R26,LOW(@0)
	SBCI R27,HIGH(@0)
	SBCI R24,BYTE3(@0)
	SBCI R25,BYTE4(@0)
	.ENDM

	.MACRO __ANDBMNN
	LDS  R30,@0+(@1)
	ANDI R30,LOW(@2)
	STS  @0+(@1),R30
	.ENDM

	.MACRO __ANDWMNN
	LDS  R30,@0+(@1)
	ANDI R30,LOW(@2)
	STS  @0+(@1),R30
	LDS  R30,@0+(@1)+1
	ANDI R30,HIGH(@2)
	STS  @0+(@1)+1,R30
	.ENDM

	.MACRO __ANDD1N
	ANDI R30,LOW(@0)
	ANDI R31,HIGH(@0)
	ANDI R22,BYTE3(@0)
	ANDI R23,BYTE4(@0)
	.ENDM

	.MACRO __ANDD2N
	ANDI R26,LOW(@0)
	ANDI R27,HIGH(@0)
	ANDI R24,BYTE3(@0)
	ANDI R25,BYTE4(@0)
	.ENDM

	.MACRO __ORBMNN
	LDS  R30,@0+(@1)
	ORI  R30,LOW(@2)
	STS  @0+(@1),R30
	.ENDM

	.MACRO __ORWMNN
	LDS  R30,@0+(@1)
	ORI  R30,LOW(@2)
	STS  @0+(@1),R30
	LDS  R30,@0+(@1)+1
	ORI  R30,HIGH(@2)
	STS  @0+(@1)+1,R30
	.ENDM

	.MACRO __ORD1N
	ORI  R30,LOW(@0)
	ORI  R31,HIGH(@0)
	ORI  R22,BYTE3(@0)
	ORI  R23,BYTE4(@0)
	.ENDM

	.MACRO __ORD2N
	ORI  R26,LOW(@0)
	ORI  R27,HIGH(@0)
	ORI  R24,BYTE3(@0)
	ORI  R25,BYTE4(@0)
	.ENDM

	.MACRO __DELAY_USB
	LDI  R24,LOW(@0)
__DELAY_USB_LOOP:
	DEC  R24
	BRNE __DELAY_USB_LOOP
	.ENDM

	.MACRO __DELAY_USW
	LDI  R24,LOW(@0)
	LDI  R25,HIGH(@0)
__DELAY_USW_LOOP:
	SBIW R24,1
	BRNE __DELAY_USW_LOOP
	.ENDM

	.MACRO __GETD1S
	LDD  R30,Y+@0
	LDD  R31,Y+@0+1
	LDD  R22,Y+@0+2
	LDD  R23,Y+@0+3
	.ENDM

	.MACRO __GETD2S
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	LDD  R24,Y+@0+2
	LDD  R25,Y+@0+3
	.ENDM

	.MACRO __PUTD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R31
	STD  Y+@0+2,R22
	STD  Y+@0+3,R23
	.ENDM

	.MACRO __PUTD2S
	STD  Y+@0,R26
	STD  Y+@0+1,R27
	STD  Y+@0+2,R24
	STD  Y+@0+3,R25
	.ENDM

	.MACRO __PUTDZ2
	STD  Z+@0,R26
	STD  Z+@0+1,R27
	STD  Z+@0+2,R24
	STD  Z+@0+3,R25
	.ENDM

	.MACRO __CLRD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R30
	STD  Y+@0+2,R30
	STD  Y+@0+3,R30
	.ENDM

	.MACRO __POINTB1MN
	LDI  R30,LOW(@0+(@1))
	.ENDM

	.MACRO __POINTW1MN
	LDI  R30,LOW(@0+(@1))
	LDI  R31,HIGH(@0+(@1))
	.ENDM

	.MACRO __POINTD1M
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __POINTW1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	.ENDM

	.MACRO __POINTD1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	LDI  R22,BYTE3(2*@0+(@1))
	LDI  R23,BYTE4(2*@0+(@1))
	.ENDM

	.MACRO __POINTB2MN
	LDI  R26,LOW(@0+(@1))
	.ENDM

	.MACRO __POINTW2MN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	.ENDM

	.MACRO __POINTW2FN
	LDI  R26,LOW(2*@0+(@1))
	LDI  R27,HIGH(2*@0+(@1))
	.ENDM

	.MACRO __POINTD2FN
	LDI  R26,LOW(2*@0+(@1))
	LDI  R27,HIGH(2*@0+(@1))
	LDI  R24,BYTE3(2*@0+(@1))
	LDI  R25,BYTE4(2*@0+(@1))
	.ENDM

	.MACRO __POINTBRM
	LDI  R@0,LOW(@1)
	.ENDM

	.MACRO __POINTWRM
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __POINTBRMN
	LDI  R@0,LOW(@1+(@2))
	.ENDM

	.MACRO __POINTWRMN
	LDI  R@0,LOW(@2+(@3))
	LDI  R@1,HIGH(@2+(@3))
	.ENDM

	.MACRO __POINTWRFN
	LDI  R@0,LOW(@2*2+(@3))
	LDI  R@1,HIGH(@2*2+(@3))
	.ENDM

	.MACRO __GETD1N
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __GETD2N
	LDI  R26,LOW(@0)
	LDI  R27,HIGH(@0)
	LDI  R24,BYTE3(@0)
	LDI  R25,BYTE4(@0)
	.ENDM

	.MACRO __GETB1MN
	LDS  R30,@0+(@1)
	.ENDM

	.MACRO __GETB1HMN
	LDS  R31,@0+(@1)
	.ENDM

	.MACRO __GETW1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	.ENDM

	.MACRO __GETD1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	LDS  R22,@0+(@1)+2
	LDS  R23,@0+(@1)+3
	.ENDM

	.MACRO __GETBRMN
	LDS  R@0,@1+(@2)
	.ENDM

	.MACRO __GETWRMN
	LDS  R@0,@2+(@3)
	LDS  R@1,@2+(@3)+1
	.ENDM

	.MACRO __GETWRZ
	LDD  R@0,Z+@2
	LDD  R@1,Z+@2+1
	.ENDM

	.MACRO __GETD2Z
	LDD  R26,Z+@0
	LDD  R27,Z+@0+1
	LDD  R24,Z+@0+2
	LDD  R25,Z+@0+3
	.ENDM

	.MACRO __GETB2MN
	LDS  R26,@0+(@1)
	.ENDM

	.MACRO __GETW2MN
	LDS  R26,@0+(@1)
	LDS  R27,@0+(@1)+1
	.ENDM

	.MACRO __GETD2MN
	LDS  R26,@0+(@1)
	LDS  R27,@0+(@1)+1
	LDS  R24,@0+(@1)+2
	LDS  R25,@0+(@1)+3
	.ENDM

	.MACRO __PUTB1MN
	STS  @0+(@1),R30
	.ENDM

	.MACRO __PUTW1MN
	STS  @0+(@1),R30
	STS  @0+(@1)+1,R31
	.ENDM

	.MACRO __PUTD1MN
	STS  @0+(@1),R30
	STS  @0+(@1)+1,R31
	STS  @0+(@1)+2,R22
	STS  @0+(@1)+3,R23
	.ENDM

	.MACRO __PUTB1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	RCALL __EEPROMWRB
	.ENDM

	.MACRO __PUTW1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	RCALL __EEPROMWRW
	.ENDM

	.MACRO __PUTD1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	RCALL __EEPROMWRD
	.ENDM

	.MACRO __PUTBR0MN
	STS  @0+(@1),R0
	.ENDM

	.MACRO __PUTBMRN
	STS  @0+(@1),R@2
	.ENDM

	.MACRO __PUTWMRN
	STS  @0+(@1),R@2
	STS  @0+(@1)+1,R@3
	.ENDM

	.MACRO __PUTBZR
	STD  Z+@1,R@0
	.ENDM

	.MACRO __PUTWZR
	STD  Z+@2,R@0
	STD  Z+@2+1,R@1
	.ENDM

	.MACRO __GETW1R
	MOV  R30,R@0
	MOV  R31,R@1
	.ENDM

	.MACRO __GETW2R
	MOV  R26,R@0
	MOV  R27,R@1
	.ENDM

	.MACRO __GETWRN
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __PUTW1R
	MOV  R@0,R30
	MOV  R@1,R31
	.ENDM

	.MACRO __PUTW2R
	MOV  R@0,R26
	MOV  R@1,R27
	.ENDM

	.MACRO __ADDWRN
	SUBI R@0,LOW(-@2)
	SBCI R@1,HIGH(-@2)
	.ENDM

	.MACRO __ADDWRR
	ADD  R@0,R@2
	ADC  R@1,R@3
	.ENDM

	.MACRO __SUBWRN
	SUBI R@0,LOW(@2)
	SBCI R@1,HIGH(@2)
	.ENDM

	.MACRO __SUBWRR
	SUB  R@0,R@2
	SBC  R@1,R@3
	.ENDM

	.MACRO __ANDWRN
	ANDI R@0,LOW(@2)
	ANDI R@1,HIGH(@2)
	.ENDM

	.MACRO __ANDWRR
	AND  R@0,R@2
	AND  R@1,R@3
	.ENDM

	.MACRO __ORWRN
	ORI  R@0,LOW(@2)
	ORI  R@1,HIGH(@2)
	.ENDM

	.MACRO __ORWRR
	OR   R@0,R@2
	OR   R@1,R@3
	.ENDM

	.MACRO __EORWRR
	EOR  R@0,R@2
	EOR  R@1,R@3
	.ENDM

	.MACRO __GETWRS
	LDD  R@0,Y+@2
	LDD  R@1,Y+@2+1
	.ENDM

	.MACRO __PUTBSR
	STD  Y+@1,R@0
	.ENDM

	.MACRO __PUTWSR
	STD  Y+@2,R@0
	STD  Y+@2+1,R@1
	.ENDM

	.MACRO __MOVEWRR
	MOV  R@0,R@2
	MOV  R@1,R@3
	.ENDM

	.MACRO __INWR
	IN   R@0,@2
	IN   R@1,@2+1
	.ENDM

	.MACRO __OUTWR
	OUT  @2+1,R@1
	OUT  @2,R@0
	.ENDM

	.MACRO __CALL1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	ICALL
	.ENDM

	.MACRO __CALL1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	RCALL __GETW1PF
	ICALL
	.ENDM

	.MACRO __CALL2EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	RCALL __EEPROMRDW
	ICALL
	.ENDM

	.MACRO __GETW1STACK
	IN   R26,SPL
	IN   R27,SPH
	ADIW R26,@0+1
	LD   R30,X+
	LD   R31,X
	.ENDM

	.MACRO __GETD1STACK
	IN   R26,SPL
	IN   R27,SPH
	ADIW R26,@0+1
	LD   R30,X+
	LD   R31,X+
	LD   R22,X
	.ENDM

	.MACRO __NBST
	BST  R@0,@1
	IN   R30,SREG
	LDI  R31,0x40
	EOR  R30,R31
	OUT  SREG,R30
	.ENDM


	.MACRO __PUTB1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RNS
	MOVW R26,R@0
	ADIW R26,@1
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	RCALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	RCALL __PUTDP1
	.ENDM


	.MACRO __GETB1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R30,Z
	.ENDM

	.MACRO __GETB1HSX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	.ENDM

	.MACRO __GETW1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R31,Z
	MOV  R30,R0
	.ENDM

	.MACRO __GETD1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R1,Z+
	LD   R22,Z+
	LD   R23,Z
	MOVW R30,R0
	.ENDM

	.MACRO __GETB2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R26,X
	.ENDM

	.MACRO __GETW2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	.ENDM

	.MACRO __GETD2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R1,X+
	LD   R24,X+
	LD   R25,X
	MOVW R26,R0
	.ENDM

	.MACRO __GETBRSX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	LD   R@0,Z
	.ENDM

	.MACRO __GETWRSX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	LD   R@0,Z+
	LD   R@1,Z
	.ENDM

	.MACRO __GETBRSX2
	MOVW R26,R28
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	LD   R@0,X
	.ENDM

	.MACRO __GETWRSX2
	MOVW R26,R28
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	LD   R@0,X+
	LD   R@1,X
	.ENDM

	.MACRO __LSLW8SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	CLR  R30
	.ENDM

	.MACRO __PUTB1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __CLRW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __CLRD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R30
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __PUTB2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z,R26
	.ENDM

	.MACRO __PUTW2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z,R27
	.ENDM

	.MACRO __PUTD2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z+,R27
	ST   Z+,R24
	ST   Z,R25
	.ENDM

	.MACRO __PUTBSRX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	ST   Z,R@0
	.ENDM

	.MACRO __PUTWSRX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	ST   Z+,R@0
	ST   Z,R@1
	.ENDM

	.MACRO __PUTB1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __MULBRR
	MULS R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRRU
	MUL  R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRR0
	MULS R@0,R@1
	.ENDM

	.MACRO __MULBRRU0
	MUL  R@0,R@1
	.ENDM

	.MACRO __MULBNWRU
	LDI  R26,@2
	MUL  R26,R@0
	MOVW R30,R0
	MUL  R26,R@1
	ADD  R31,R0
	.ENDM

;NAME DEFINITIONS FOR GLOBAL VARIABLES ALLOCATED TO REGISTERS
	.DEF _PowerStatus=R4
	.DEF _STM32Data=R6
	.DEF _POWER_LED_Cunter=R8
	.DEF _KEY_PRESS=R10
	.DEF _StartPower=R12

	.CSEG
	.ORG 0x00

;START OF CODE MARKER
__START_OF_CODE:

;INTERRUPT VECTORS
	RJMP __RESET
	RJMP _ext_int0_isr
	RJMP _ext_int1_isr
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00
	RJMP 0x00

_tbl10_G100:
	.DB  0x10,0x27,0xE8,0x3,0x64,0x0,0xA,0x0
	.DB  0x1,0x0
_tbl16_G100:
	.DB  0x0,0x10,0x0,0x1,0x10,0x0,0x1,0x0

;REGISTER BIT VARIABLES INITIALIZATION
__REG_BIT_VARS:
	.DW  0x0001

_0x3:
	.DB  0x1
_0x4:
	.DB  0x1
_0x148:
	.DB  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0
	.DB  0x0,0x0

__GLOBAL_INI_TBL:
	.DW  0x01
	.DW  0x02
	.DW  __REG_BIT_VARS*2

	.DW  0x01
	.DW  _PulseNumber
	.DW  _0x3*2

	.DW  0x01
	.DW  _NumberPacket
	.DW  _0x4*2

	.DW  0x0A
	.DW  0x04
	.DW  _0x148*2

_0xFFFFFFFF:
	.DW  0

__RESET:
	CLI
	CLR  R30
	OUT  EECR,R30

;INTERRUPT VECTORS ARE PLACED
;AT THE START OF FLASH
	LDI  R31,1
	OUT  GICR,R31
	OUT  GICR,R30
	OUT  MCUCR,R30

;DISABLE WATCHDOG
	LDI  R31,0x18
	OUT  WDTCR,R31
	OUT  WDTCR,R30

;CLEAR R2-R14
	LDI  R24,(14-2)+1
	LDI  R26,2
	CLR  R27
__CLEAR_REG:
	ST   X+,R30
	DEC  R24
	BRNE __CLEAR_REG

;CLEAR SRAM
	LDI  R24,LOW(__CLEAR_SRAM_SIZE)
	LDI  R25,HIGH(__CLEAR_SRAM_SIZE)
	LDI  R26,__SRAM_START
__CLEAR_SRAM:
	ST   X+,R30
	SBIW R24,1
	BRNE __CLEAR_SRAM

;GLOBAL VARIABLES INITIALIZATION
	LDI  R30,LOW(__GLOBAL_INI_TBL*2)
	LDI  R31,HIGH(__GLOBAL_INI_TBL*2)
__GLOBAL_INI_NEXT:
	LPM  R24,Z+
	LPM  R25,Z+
	SBIW R24,0
	BREQ __GLOBAL_INI_END
	LPM  R26,Z+
	LPM  R27,Z+
	LPM  R0,Z+
	LPM  R1,Z+
	MOVW R22,R30
	MOVW R30,R0
__GLOBAL_INI_LOOP:
	LPM  R0,Z+
	ST   X+,R0
	SBIW R24,1
	BRNE __GLOBAL_INI_LOOP
	MOVW R30,R22
	RJMP __GLOBAL_INI_NEXT
__GLOBAL_INI_END:

;HARDWARE STACK POINTER INITIALIZATION
	LDI  R30,LOW(__SRAM_END-__HEAP_SIZE)
	OUT  SPL,R30
	LDI  R30,HIGH(__SRAM_END-__HEAP_SIZE)
	OUT  SPH,R30

;DATA STACK POINTER INITIALIZATION
	LDI  R28,LOW(__SRAM_START+__DSTACK_SIZE)
	LDI  R29,HIGH(__SRAM_START+__DSTACK_SIZE)

	RJMP _main

	.ESEG
	.ORG 0

	.DSEG
	.ORG 0x160

	.CSEG
;
;/*
;    This version is compatible PCB Board of VERSION 3
;*/
;
;
;#include <mega8.h>
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x80
	.EQU __sm_mask=0x70
	.EQU __sm_powerdown=0x20
	.EQU __sm_powersave=0x30
	.EQU __sm_standby=0x60
	.EQU __sm_ext_standby=0x70
	.EQU __sm_adc_noise_red=0x10
	.SET power_ctrl_reg=mcucr
	#endif
;#include <stdio.h>
;#include <delay.h>
;
;#define      LED_ON            PORTD.7 = 1
;#define      LED_OFF           PORTD.7 = 0
;
;#define      BUZZER_ON         PORTC.0 = 1
;#define      BUZZER_OFF        PORTC.0 = 0
;
;#define      LEARN             PIND.0
;
;#define      YES               1
;#define      NO                0
;
;#define      OK                1
;
;#define      BIT0              0
;#define      BIT1              1
;#define      FLOATING          2
;
;
;
;
;
;#define     LED1            PORTD.7
;#define     LED2            PORTD.6
;#define     LED3            PORTD.5
;#define     LED4            PORTD.4
;
;#define     POWER_LED       PORTC.5
;
;
;
;
;
;#define     RELAY_OUT       PORTB.0
;#define     RELAY_SELECT    PORTB.1
;#define     RELAY_POWER     PORTB.2
;
;#define     M8_TO_STM32     PORTC.4
;
;
;#define     M8_Q0           PORTC.0
;#define     M8_Q1           PORTC.1
;#define     M8_Q2           PORTC.2
;#define     M8_Q3           PORTC.3
;
;#define     STM32_Q0        PINC.0
;#define     STM32_Q1        PINC.1
;#define     STM32_Q2        PINC.2
;#define     STM32_Q3        PINC.3
;
;#define     DELAY           120
;
;#define     SET_OUTPUT      DDRC = 0x3F
;#define     SET_INPUT
;
;
;
;short int PowerStatus = 0 , STM32Data, POWER_LED_Cunter, KEY_PRESS = 0;
;
;//char string[20];
;int StartPower = 0;
;
;float T1, T2, DutyCycle;
;
;bit RisingEdge = OK, StartT2 = NO, GetPulse = NO, Sync = NO, AddressReceive = NO;
;
;short int i = 0, j = 0, PulseNumber = 1, Pulse1, Pulse2;

	.DSEG
;
;short int  AddressData1[12];
;short int  AddressData2[12];
;short int  AddressData[8];
;eeprom short int  EepromAddressData[8];
;
;short int NumberPacket = 1, Error = NO;
;//short int receive = 0,
;short int  Data;
;
;void Init(){
; 0000 0057 void Init(){

	.CSEG
_Init:
; 0000 0058 
; 0000 0059 
; 0000 005A 
; 0000 005B 
; 0000 005C PORTB=0x00;
	LDI  R30,LOW(0)
	OUT  0x18,R30
; 0000 005D DDRB=0x07;
	LDI  R30,LOW(7)
	OUT  0x17,R30
; 0000 005E 
; 0000 005F 
; 0000 0060 PORTC=0x00;
	LDI  R30,LOW(0)
	OUT  0x15,R30
; 0000 0061 DDRC=0x30;
	LDI  R30,LOW(48)
	OUT  0x14,R30
; 0000 0062 
; 0000 0063 
; 0000 0064 PORTD=0x03;
	LDI  R30,LOW(3)
	OUT  0x12,R30
; 0000 0065 DDRD=0xF0;
	LDI  R30,LOW(240)
	OUT  0x11,R30
; 0000 0066 
; 0000 0067 
; 0000 0068 
; 0000 0069 // Timer/Counter 1 initialization
; 0000 006A // Clock source: System Clock
; 0000 006B // Clock value: 2000.000 kHz
; 0000 006C // Mode: Normal top=0xFFFF
; 0000 006D // OC1A output: Discon.
; 0000 006E // OC1B output: Discon.
; 0000 006F // Noise Canceler: Off
; 0000 0070 // Input Capture on Falling Edge
; 0000 0071 // Timer1 Overflow Interrupt: Off
; 0000 0072 // Input Capture Interrupt: Off
; 0000 0073 // Compare A Match Interrupt: Off
; 0000 0074 // Compare B Match Interrupt: Off
; 0000 0075 TCCR1A=0x00;
	LDI  R30,LOW(0)
	OUT  0x2F,R30
; 0000 0076 TCCR1B=0x02;
	LDI  R30,LOW(2)
	OUT  0x2E,R30
; 0000 0077 TCNT1H=0x00;
	LDI  R30,LOW(0)
	OUT  0x2D,R30
; 0000 0078 TCNT1L=0x00;
	OUT  0x2C,R30
; 0000 0079 ICR1H=0x00;
	OUT  0x27,R30
; 0000 007A ICR1L=0x00;
	OUT  0x26,R30
; 0000 007B OCR1AH=0x00;
	OUT  0x2B,R30
; 0000 007C OCR1AL=0x00;
	OUT  0x2A,R30
; 0000 007D OCR1BH=0x00;
	OUT  0x29,R30
; 0000 007E OCR1BL=0x00;
	OUT  0x28,R30
; 0000 007F 
; 0000 0080 
; 0000 0081 
; 0000 0082 
; 0000 0083 // External Interrupt(s) initialization
; 0000 0084 // INT0: On
; 0000 0085 // INT0 Mode: Rising Edge
; 0000 0086 // INT1: On
; 0000 0087 // INT1 Mode: Rising Edge
; 0000 0088 GICR|=0xC0;
	IN   R30,0x3B
	ORI  R30,LOW(0xC0)
	OUT  0x3B,R30
; 0000 0089 MCUCR=0x0F;
	LDI  R30,LOW(15)
	OUT  0x35,R30
; 0000 008A GIFR=0xC0;
	LDI  R30,LOW(192)
	OUT  0x3A,R30
; 0000 008B 
; 0000 008C 
; 0000 008D     // Global enable interrupts
; 0000 008E     #asm("sei")
	sei
; 0000 008F 
; 0000 0090 
; 0000 0091 
; 0000 0092     for(i=0; i<8; i++)
	RCALL SUBOPT_0x0
_0x6:
	RCALL SUBOPT_0x1
	SBIW R26,8
	BRGE _0x7
; 0000 0093        AddressData[i]= EepromAddressData[i];
	RCALL SUBOPT_0x2
	RCALL SUBOPT_0x3
	RCALL SUBOPT_0x4
	RCALL SUBOPT_0x5
	RCALL __EEPROMRDW
	MOVW R26,R0
	RCALL SUBOPT_0x6
	RCALL SUBOPT_0x7
	RJMP _0x6
_0x7:
; 0000 0094 }
	RET
;
;
;
;// External Interrupt 0 service routine
;interrupt [EXT_INT0] void ext_int0_isr(void)
; 0000 009A {
_ext_int0_isr:
	RCALL SUBOPT_0x8
; 0000 009B     if(PowerStatus == 1)
	RCALL SUBOPT_0x9
	RCALL SUBOPT_0xA
	BRNE _0x8
; 0000 009C     {
; 0000 009D         SET_INPUT;
; 0000 009E         delay_ms(2);
	RCALL SUBOPT_0xB
; 0000 009F         STM32Data =  (STM32_Q0 == 1) ? 1 : 0;
	SBIS 0x13,0
	RJMP _0x9
	RCALL SUBOPT_0x9
	RJMP _0xA
_0x9:
	RCALL SUBOPT_0xC
_0xA:
	MOVW R6,R30
; 0000 00A0 		STM32Data += (STM32_Q1 == 1) ? 2 : 0;
	SBIS 0x13,1
	RJMP _0xC
	RCALL SUBOPT_0xD
	RJMP _0xD
_0xC:
	RCALL SUBOPT_0xC
_0xD:
	__ADDWRR 6,7,30,31
; 0000 00A1 		STM32Data += (STM32_Q2 == 1) ? 4 : 0;
	SBIS 0x13,2
	RJMP _0xF
	RCALL SUBOPT_0xE
	RJMP _0x10
_0xF:
	RCALL SUBOPT_0xC
_0x10:
	__ADDWRR 6,7,30,31
; 0000 00A2 		STM32Data += (STM32_Q3 == 1) ? 8 : 0;
	SBIS 0x13,3
	RJMP _0x12
	LDI  R30,LOW(8)
	LDI  R31,HIGH(8)
	RJMP _0x13
_0x12:
	RCALL SUBOPT_0xC
_0x13:
	__ADDWRR 6,7,30,31
; 0000 00A3 
; 0000 00A4         if(STM32Data == 7)
	LDI  R30,LOW(7)
	LDI  R31,HIGH(7)
	CP   R30,R6
	CPC  R31,R7
	BRNE _0x15
; 0000 00A5         {
; 0000 00A6             LED1 = 1;
	RCALL SUBOPT_0xF
; 0000 00A7             LED2 = 1;
; 0000 00A8             LED3 = 1;
; 0000 00A9             LED4 = 1;
; 0000 00AA             delay_ms(1000);
	LDI  R26,LOW(1000)
	LDI  R27,HIGH(1000)
	RCALL SUBOPT_0x10
; 0000 00AB             LED1 = 0;
; 0000 00AC             LED2 = 0;
; 0000 00AD             LED3 = 0;
; 0000 00AE             LED4 = 0;
; 0000 00AF         }
; 0000 00B0     }
_0x15:
; 0000 00B1 
; 0000 00B2 }
_0x8:
	RJMP _0x147
;
;// External Interrupt 1 service routine
;interrupt [EXT_INT1] void ext_int1_isr(void)
; 0000 00B6 {
_ext_int1_isr:
	RCALL SUBOPT_0x8
; 0000 00B7 
; 0000 00B8     if(RisingEdge == YES)   //Rising Edge
	SBRS R2,0
	RJMP _0x26
; 0000 00B9     {
; 0000 00BA 
; 0000 00BB         if(StartT2 == YES)
	SBRS R2,1
	RJMP _0x27
; 0000 00BC         {
; 0000 00BD             T2 = TCNT1;
	IN   R30,0x2C
	IN   R31,0x2C+1
	LDI  R26,LOW(_T2)
	LDI  R27,HIGH(_T2)
	RCALL SUBOPT_0x11
; 0000 00BE             StartT2 = NO;
	CLT
	BLD  R2,1
; 0000 00BF             GetPulse = OK;
	SET
	BLD  R2,2
; 0000 00C0         }
; 0000 00C1         TCNT1 = 0;
_0x27:
	RCALL SUBOPT_0x12
; 0000 00C2         //MCUCR=0x08;     //INT1 Mode : Falling Edge
; 0000 00C3         RisingEdge = NO;
	CLT
	RCALL SUBOPT_0x13
; 0000 00C4 
; 0000 00C5         // External Interrupt(s) initialization
; 0000 00C6         // INT0: On
; 0000 00C7         // INT0 Mode: Rising Edge
; 0000 00C8         // INT1: On
; 0000 00C9         // INT1 Mode: Falling Edge
; 0000 00CA 
; 0000 00CB         MCUCR=0x0B;
; 0000 00CC 
; 0000 00CD     }
; 0000 00CE     else    // Falling Edge
	RJMP _0x28
_0x26:
; 0000 00CF     {
; 0000 00D0         T1 = TCNT1;
	IN   R30,0x2C
	IN   R31,0x2C+1
	LDI  R26,LOW(_T1)
	LDI  R27,HIGH(_T1)
	RCALL SUBOPT_0x11
; 0000 00D1         TCNT1 = 0;
	RCALL SUBOPT_0x12
; 0000 00D2 
; 0000 00D3         // External Interrupt(s) initialization
; 0000 00D4         // INT0: On
; 0000 00D5         // INT0 Mode: Rising Edge
; 0000 00D6         // INT1: On
; 0000 00D7         // INT1 Mode: Rising Edge
; 0000 00D8 
; 0000 00D9         MCUCR=0x0F;
	LDI  R30,LOW(15)
	OUT  0x35,R30
; 0000 00DA 
; 0000 00DB 
; 0000 00DC         RisingEdge = OK;
	SET
	BLD  R2,0
; 0000 00DD         StartT2 = OK;
	BLD  R2,1
; 0000 00DE     }
_0x28:
; 0000 00DF 
; 0000 00E0     if( GetPulse == OK) {
	SBRS R2,2
	RJMP _0x29
; 0000 00E1 
; 0000 00E2         if(Sync == NO)
	SBRC R2,3
	RJMP _0x2A
; 0000 00E3         {
; 0000 00E4             DutyCycle = (T1 / (T1 + T2));
	RCALL SUBOPT_0x14
; 0000 00E5             if(DutyCycle > 0.015 && DutyCycle < 0.035) //Normal 0.0333
	RCALL SUBOPT_0x15
	__GETD1N 0x3C75C28F
	RCALL __CMPF12
	BREQ PC+2
	BRCC PC+2
	RJMP _0x2C
	RCALL SUBOPT_0x15
	__GETD1N 0x3D0F5C29
	RCALL __CMPF12
	BRLO _0x2D
_0x2C:
	RJMP _0x2B
_0x2D:
; 0000 00E6             {
; 0000 00E7                 Sync = OK;
	SET
	BLD  R2,3
; 0000 00E8                 AddressReceive = YES;
	BLD  R2,4
; 0000 00E9                 Error = NO;
	RCALL SUBOPT_0x16
; 0000 00EA                 PulseNumber = 1;
	RCALL SUBOPT_0x17
; 0000 00EB                 i = 0;
	RCALL SUBOPT_0x0
; 0000 00EC                 j = 0;
	RCALL SUBOPT_0x18
; 0000 00ED             }
; 0000 00EE         }
_0x2B:
; 0000 00EF         else if(AddressReceive == YES)
	RJMP _0x2E
_0x2A:
	SBRS R2,4
	RJMP _0x2F
; 0000 00F0         {
; 0000 00F1             DutyCycle = (T1 / (T1 + T2));
	RCALL SUBOPT_0x14
; 0000 00F2 
; 0000 00F3             if(i != 16)
	RCALL SUBOPT_0x1
	SBIW R26,16
	BRNE PC+2
	RJMP _0x30
; 0000 00F4             {
; 0000 00F5                 if(DutyCycle > 0.15 && DutyCycle < 0.3) //4a  Normal 0.239
	RCALL SUBOPT_0x15
	__GETD1N 0x3E19999A
	RCALL __CMPF12
	BREQ PC+2
	BRCC PC+2
	RJMP _0x32
	RCALL SUBOPT_0x15
	__GETD1N 0x3E99999A
	RCALL __CMPF12
	BRLO _0x33
_0x32:
	RJMP _0x31
_0x33:
; 0000 00F6                 {
; 0000 00F7                     if(PulseNumber == 1)
	RCALL SUBOPT_0x19
	SBIW R26,1
	BRNE _0x34
; 0000 00F8                         Pulse1 = 4;
	RCALL SUBOPT_0xE
	RCALL SUBOPT_0x1A
; 0000 00F9                     else if(PulseNumber == 2)
	RJMP _0x35
_0x34:
	RCALL SUBOPT_0x1B
	BRNE _0x36
; 0000 00FA                         Pulse2 = 4;
	RCALL SUBOPT_0xE
	RCALL SUBOPT_0x1C
; 0000 00FB                 }
_0x36:
_0x35:
; 0000 00FC                 else if(DutyCycle > 0.5 && DutyCycle < 0.8) //12a  Normal 0.729
	RJMP _0x37
_0x31:
	RCALL SUBOPT_0x15
	__GETD1N 0x3F000000
	RCALL __CMPF12
	BREQ PC+2
	BRCC PC+2
	RJMP _0x39
	RCALL SUBOPT_0x15
	__GETD1N 0x3F4CCCCD
	RCALL __CMPF12
	BRLO _0x3A
_0x39:
	RJMP _0x38
_0x3A:
; 0000 00FD                 {
; 0000 00FE                     if(PulseNumber == 1)
	RCALL SUBOPT_0x19
	SBIW R26,1
	BRNE _0x3B
; 0000 00FF                         Pulse1 = 12;
	LDI  R30,LOW(12)
	LDI  R31,HIGH(12)
	RCALL SUBOPT_0x1A
; 0000 0100                     else if(PulseNumber == 2)
	RJMP _0x3C
_0x3B:
	RCALL SUBOPT_0x1B
	BRNE _0x3D
; 0000 0101                         Pulse2 = 12;
	LDI  R30,LOW(12)
	LDI  R31,HIGH(12)
	RCALL SUBOPT_0x1C
; 0000 0102                 }
_0x3D:
_0x3C:
; 0000 0103                 else
	RJMP _0x3E
_0x38:
; 0000 0104                 {
; 0000 0105                     for(i=0; i<12; i++)
	RCALL SUBOPT_0x0
_0x40:
	RCALL SUBOPT_0x1
	SBIW R26,12
	BRGE _0x41
; 0000 0106                     {
; 0000 0107                         AddressData1[i] = 0;
	RCALL SUBOPT_0x1D
	RCALL SUBOPT_0x1E
; 0000 0108                         AddressData2[i] = 0;
	RCALL SUBOPT_0x1F
	RCALL SUBOPT_0x1E
; 0000 0109                     }
	RCALL SUBOPT_0x7
	RJMP _0x40
_0x41:
; 0000 010A 
; 0000 010B                     NumberPacket = 1;
	RCALL SUBOPT_0x9
	STS  _NumberPacket,R30
	STS  _NumberPacket+1,R31
; 0000 010C                     Error = NO;
	RCALL SUBOPT_0x16
; 0000 010D                     AddressReceive = NO;
	CLT
	BLD  R2,4
; 0000 010E                     PulseNumber = 1;
	RCALL SUBOPT_0x17
; 0000 010F                     Sync = NO;
	CLT
	BLD  R2,3
; 0000 0110                     i = 0;
	RCALL SUBOPT_0x0
; 0000 0111                     j = 0;
	RCALL SUBOPT_0x18
; 0000 0112                     TCNT1 = 0;
	RCALL SUBOPT_0x12
; 0000 0113                     RisingEdge = OK;
	SET
	RCALL SUBOPT_0x13
; 0000 0114                     // External Interrupt(s) initialization
; 0000 0115                     // INT0: On
; 0000 0116                     // INT0 Mode: Rising Edge
; 0000 0117                     // INT1: On
; 0000 0118                     // INT1 Mode: Falling Edge
; 0000 0119 
; 0000 011A                     MCUCR=0x0B;
; 0000 011B 
; 0000 011C 
; 0000 011D                     StartT2 = NO;
	CLT
	BLD  R2,1
; 0000 011E                     GetPulse = NO;
	RJMP _0x13F
; 0000 011F                     goto EndInterrupt;
; 0000 0120                 }
_0x3E:
_0x37:
; 0000 0121 
; 0000 0122                 if(PulseNumber == 2)
	RCALL SUBOPT_0x1B
	BREQ PC+2
	RJMP _0x43
; 0000 0123                 {
; 0000 0124                     if( Pulse1 == 4 && Pulse2 == 4)
	RCALL SUBOPT_0x20
	SBIW R26,4
	BRNE _0x45
	RCALL SUBOPT_0x21
	BREQ _0x46
_0x45:
	RJMP _0x44
_0x46:
; 0000 0125                     {
; 0000 0126                         if(NumberPacket == 1)
	RCALL SUBOPT_0x22
	SBIW R26,1
	BRNE _0x47
; 0000 0127                             AddressData1[j] = BIT0 ;
	RCALL SUBOPT_0x23
	LDI  R26,LOW(_AddressData1)
	LDI  R27,HIGH(_AddressData1)
	RJMP _0x140
; 0000 0128                         else if(NumberPacket == 2)
_0x47:
	RCALL SUBOPT_0x24
	BRNE _0x49
; 0000 0129                             AddressData2[j] = BIT0 ;
	RCALL SUBOPT_0x25
_0x140:
	LSL  R30
	ROL  R31
	RCALL SUBOPT_0x5
	RCALL SUBOPT_0x1E
; 0000 012A                     }
_0x49:
; 0000 012B                     else if(Pulse1 == 12 && Pulse2 == 4)
	RJMP _0x4A
_0x44:
	RCALL SUBOPT_0x20
	SBIW R26,12
	BRNE _0x4C
	RCALL SUBOPT_0x21
	BREQ _0x4D
_0x4C:
	RJMP _0x4B
_0x4D:
; 0000 012C                     {
; 0000 012D                         if(NumberPacket == 1)
	RCALL SUBOPT_0x22
	SBIW R26,1
	BRNE _0x4E
; 0000 012E                             AddressData1[j] = BIT1 ;
	RCALL SUBOPT_0x23
	LDI  R26,LOW(_AddressData1)
	LDI  R27,HIGH(_AddressData1)
	RJMP _0x141
; 0000 012F                         else if(NumberPacket == 2)
_0x4E:
	RCALL SUBOPT_0x24
	BRNE _0x50
; 0000 0130                             AddressData2[j] = BIT1 ;
	RCALL SUBOPT_0x25
_0x141:
	LSL  R30
	ROL  R31
	RCALL SUBOPT_0x5
	RCALL SUBOPT_0x9
	RCALL SUBOPT_0x6
; 0000 0131                     }
_0x50:
; 0000 0132                     else if(Pulse1 == 4 && Pulse2 == 12)
	RJMP _0x51
_0x4B:
	RCALL SUBOPT_0x20
	SBIW R26,4
	BRNE _0x53
	LDS  R26,_Pulse2
	LDS  R27,_Pulse2+1
	SBIW R26,12
	BREQ _0x54
_0x53:
	RJMP _0x52
_0x54:
; 0000 0133                     {
; 0000 0134                         if(NumberPacket == 1)
	RCALL SUBOPT_0x22
	SBIW R26,1
	BRNE _0x55
; 0000 0135                             AddressData1[j] = FLOATING ;
	RCALL SUBOPT_0x23
	LDI  R26,LOW(_AddressData1)
	LDI  R27,HIGH(_AddressData1)
	RJMP _0x142
; 0000 0136                         else if(NumberPacket == 2)
_0x55:
	RCALL SUBOPT_0x24
	BRNE _0x57
; 0000 0137                             AddressData2[j] = FLOATING ;
	RCALL SUBOPT_0x25
_0x142:
	LSL  R30
	ROL  R31
	RCALL SUBOPT_0x5
	RCALL SUBOPT_0xD
	RCALL SUBOPT_0x6
; 0000 0138                     }
_0x57:
; 0000 0139                     j++;
_0x52:
_0x51:
_0x4A:
	LDI  R26,LOW(_j)
	LDI  R27,HIGH(_j)
	LD   R30,X+
	LD   R31,X+
	ADIW R30,1
	ST   -X,R31
	ST   -X,R30
; 0000 013A                 }
; 0000 013B 
; 0000 013C 
; 0000 013D                 if(PulseNumber == 1)
_0x43:
	RCALL SUBOPT_0x19
	SBIW R26,1
	BRNE _0x58
; 0000 013E                     PulseNumber = 2;
	RCALL SUBOPT_0xD
	RJMP _0x143
; 0000 013F                 else if(PulseNumber == 2)
_0x58:
	RCALL SUBOPT_0x1B
	BRNE _0x5A
; 0000 0140                     PulseNumber = 1;
	RCALL SUBOPT_0x9
_0x143:
	STS  _PulseNumber,R30
	STS  _PulseNumber+1,R31
; 0000 0141             }
_0x5A:
; 0000 0142 
; 0000 0143             i++;
_0x30:
	RCALL SUBOPT_0x7
; 0000 0144 
; 0000 0145             if(i > 23)
	RCALL SUBOPT_0x1
	SBIW R26,24
	BRGE PC+2
	RJMP _0x5B
; 0000 0146             {
; 0000 0147 
; 0000 0148                 if(NumberPacket == 2)
	RCALL SUBOPT_0x24
	BREQ PC+2
	RJMP _0x5C
; 0000 0149                 {
; 0000 014A                     for(i=0; i<12; i++)
	RCALL SUBOPT_0x0
_0x5E:
	RCALL SUBOPT_0x1
	SBIW R26,12
	BRGE _0x5F
; 0000 014B                         if(AddressData1[i] != AddressData2[i])
	RCALL SUBOPT_0x1D
	LD   R0,X+
	LD   R1,X
	RCALL SUBOPT_0x1F
	RCALL __GETW1P
	CP   R30,R0
	CPC  R31,R1
	BREQ _0x60
; 0000 014C                             Error = YES;
	RCALL SUBOPT_0x26
; 0000 014D 
; 0000 014E                     if(Error == NO)
_0x60:
	RCALL SUBOPT_0x7
	RJMP _0x5E
_0x5F:
	RCALL SUBOPT_0x27
	BREQ PC+2
	RJMP _0x61
; 0000 014F                     {
; 0000 0150                         if(!LEARN)
	SBIC 0x10,0
	RJMP _0x62
; 0000 0151                         {
; 0000 0152                             for(i=0; i<4; i++)
	RCALL SUBOPT_0x0
_0x64:
	RCALL SUBOPT_0x1
	SBIW R26,4
	BRGE _0x65
; 0000 0153                             {
; 0000 0154                                 LED1 = 1;
	RCALL SUBOPT_0xF
; 0000 0155                                 LED2 = 1;
; 0000 0156                                 LED3 = 1;
; 0000 0157                                 LED4 = 1;
; 0000 0158                                 delay_ms(100);
	RCALL SUBOPT_0x28
	RCALL SUBOPT_0x10
; 0000 0159                                 LED1 = 0;
; 0000 015A                                 LED2 = 0;
; 0000 015B                                 LED3 = 0;
; 0000 015C                                 LED4 = 0;
; 0000 015D                                 delay_ms(100);
	RCALL SUBOPT_0x28
	RCALL _delay_ms
; 0000 015E                                 //BUZZER_ON;
; 0000 015F                                 //delay_ms(50);
; 0000 0160                                 //BUZZER_OFF;
; 0000 0161                                 //delay_ms(50);
; 0000 0162                             }
	RCALL SUBOPT_0x7
	RJMP _0x64
_0x65:
; 0000 0163 
; 0000 0164                             for(i=0; i<8; i++)
	RCALL SUBOPT_0x0
_0x77:
	RCALL SUBOPT_0x1
	SBIW R26,8
	BRGE _0x78
; 0000 0165                             {
; 0000 0166                                 EepromAddressData[i] = AddressData1[i];
	RCALL SUBOPT_0x2
	RCALL SUBOPT_0x4
	ADD  R30,R26
	ADC  R31,R27
	MOVW R0,R30
	RCALL SUBOPT_0x1D
	RCALL __GETW1P
	MOVW R26,R0
	RCALL __EEPROMWRW
; 0000 0167                                 AddressData[i] = AddressData2[i];
	RCALL SUBOPT_0x2
	RCALL SUBOPT_0x3
	LDI  R26,LOW(_AddressData2)
	LDI  R27,HIGH(_AddressData2)
	LSL  R30
	ROL  R31
	RCALL SUBOPT_0x5
	RCALL __GETW1P
	MOVW R26,R0
	RCALL SUBOPT_0x6
; 0000 0168                                 //lcd_clear();
; 0000 0169                                 //sprintf(string, "%0d %0d %0d %0d %0d %0d %0d %0d", AddressData[0] , AddressData[1], AddressData[2], AddressData[3], AddressData[4], AddressData[5], AddressData[6], AddressData[7]);
; 0000 016A                                 //lcd_gotoxy(0, 0);
; 0000 016B                                 //lcd_puts(string);
; 0000 016C                             }
	RCALL SUBOPT_0x7
	RJMP _0x77
_0x78:
; 0000 016D                         }
; 0000 016E                         else
	RJMP _0x79
_0x62:
; 0000 016F                         {
; 0000 0170                             Error = NO;
	RCALL SUBOPT_0x16
; 0000 0171 
; 0000 0172                             for(i=0; i<8; i++)
	RCALL SUBOPT_0x0
_0x7B:
	RCALL SUBOPT_0x1
	SBIW R26,8
	BRGE _0x7C
; 0000 0173                             {
; 0000 0174                                 if(AddressData[i] != AddressData2[i])
	RCALL SUBOPT_0x2
	LDI  R26,LOW(_AddressData)
	LDI  R27,HIGH(_AddressData)
	LSL  R30
	ROL  R31
	RCALL SUBOPT_0x5
	LD   R0,X+
	LD   R1,X
	RCALL SUBOPT_0x1F
	RCALL __GETW1P
	CP   R30,R0
	CPC  R31,R1
	BREQ _0x7D
; 0000 0175                                 {
; 0000 0176                                     Error = YES;
	RCALL SUBOPT_0x26
; 0000 0177 
; 0000 0178                                     //lcd_clear();
; 0000 0179                                     //sprintf(string, "%0d %0d %0d %0d %0d %0d %0d %0d", AddressData2[0] , AddressData2[1], AddressData2[2], AddressData2[3], AddressData2[4], AddressData2[5], AddressData2[6], AddressData2[7]);
; 0000 017A                                     //lcd_gotoxy(0, 0);
; 0000 017B                                     //lcd_puts(string);
; 0000 017C                                     //sprintf(string, "NO MATCH ADDRESS");
; 0000 017D                                     //lcd_gotoxy(0, 1);
; 0000 017E                                     //lcd_puts(string);
; 0000 017F 
; 0000 0180                                     LED1 = 1;
	RCALL SUBOPT_0xF
; 0000 0181                                     LED2 = 1;
; 0000 0182                                     LED3 = 1;
; 0000 0183                                     LED4 = 1;
; 0000 0184                                     delay_ms(200);
	LDI  R26,LOW(200)
	RCALL SUBOPT_0x29
	RCALL SUBOPT_0x10
; 0000 0185                                     LED1 = 0;
; 0000 0186                                     LED2 = 0;
; 0000 0187                                     LED3 = 0;
; 0000 0188                                     LED4 = 0;
; 0000 0189 
; 0000 018A                                 }
; 0000 018B                             }
_0x7D:
	RCALL SUBOPT_0x7
	RJMP _0x7B
_0x7C:
; 0000 018C 
; 0000 018D                             if(Error == NO)
	RCALL SUBOPT_0x27
	BREQ PC+2
	RJMP _0x8E
; 0000 018E                             {
; 0000 018F 
; 0000 0190                                 //lcd_clear();
; 0000 0191                                 //sprintf(string, "%0d %0d %0d %0d %0d %0d %0d %0d", AddressData1[0] , AddressData1[1], AddressData1[2], AddressData1[3], AddressData1[4], AddressData1[5], AddressData1[6], AddressData1[7]);
; 0000 0192                                 //lcd_gotoxy(0, 0);
; 0000 0193                                 //lcd_puts(string);
; 0000 0194 
; 0000 0195                                 Data = ((AddressData1[8]*1000) + (AddressData1[9]*100) + (AddressData1[10]*10) + AddressData1[11]);
	__GETW1MN _AddressData1,16
	LDI  R26,LOW(1000)
	LDI  R27,HIGH(1000)
	RCALL __MULW12
	MOVW R22,R30
	__GETW1MN _AddressData1,18
	LDI  R26,LOW(100)
	LDI  R27,HIGH(100)
	RCALL __MULW12
	__ADDWRR 22,23,30,31
	__GETW1MN _AddressData1,20
	LDI  R26,LOW(10)
	LDI  R27,HIGH(10)
	RCALL __MULW12
	MOVW R26,R22
	RCALL SUBOPT_0x5
	__GETW1MN _AddressData1,22
	ADD  R30,R26
	ADC  R31,R27
	RCALL SUBOPT_0x2A
; 0000 0196                                 LED1 = 0;
	CBI  0x12,7
; 0000 0197                                 LED2 = 0;
	CBI  0x12,6
; 0000 0198                                 LED3 = 0;
	CBI  0x12,5
; 0000 0199                                 LED4 = 0;
	CBI  0x12,4
; 0000 019A                                 switch(Data){
	LDS  R30,_Data
	LDS  R31,_Data+1
; 0000 019B                                     case 20:      //0001
	CPI  R30,LOW(0x14)
	LDI  R26,HIGH(0x14)
	CPC  R31,R26
	BRNE _0x9A
; 0000 019C                                         Data = 1;
	RCALL SUBOPT_0x9
	RCALL SUBOPT_0x2A
; 0000 019D                                         LED4 = 1;
	SBI  0x12,4
; 0000 019E                                         break;
	RJMP _0x99
; 0000 019F                                     case 210:     //0010
_0x9A:
	CPI  R30,LOW(0xD2)
	LDI  R26,HIGH(0xD2)
	CPC  R31,R26
	BRNE _0x9D
; 0000 01A0                                         Data = 2;
	RCALL SUBOPT_0xD
	RCALL SUBOPT_0x2A
; 0000 01A1                                         LED3 = 1;
	SBI  0x12,5
; 0000 01A2                                         break;
	RJMP _0x99
; 0000 01A3                                     case 200:     //0011
_0x9D:
	CPI  R30,LOW(0xC8)
	LDI  R26,HIGH(0xC8)
	CPC  R31,R26
	BRNE _0xA0
; 0000 01A4                                         Data = 3;
	LDI  R30,LOW(3)
	LDI  R31,HIGH(3)
	RCALL SUBOPT_0x2A
; 0000 01A5                                         LED3 = 1;
	SBI  0x12,5
; 0000 01A6                                         LED4 = 1;
	SBI  0x12,4
; 0000 01A7                                         break;
	RJMP _0x99
; 0000 01A8                                     case 2100:    //0100
_0xA0:
	CPI  R30,LOW(0x834)
	LDI  R26,HIGH(0x834)
	CPC  R31,R26
	BRNE _0xA5
; 0000 01A9                                         Data = 4;
	RCALL SUBOPT_0xE
	RCALL SUBOPT_0x2A
; 0000 01AA                                         LED2 = 1;
	SBI  0x12,6
; 0000 01AB                                         break;
	RJMP _0x99
; 0000 01AC                                     case 2120:    //0101
_0xA5:
	CPI  R30,LOW(0x848)
	LDI  R26,HIGH(0x848)
	CPC  R31,R26
	BRNE _0xA8
; 0000 01AD                                         LED2 = 1;
	SBI  0x12,6
; 0000 01AE                                         LED4 = 1;
	SBI  0x12,4
; 0000 01AF                                         Data = 5;
	LDI  R30,LOW(5)
	LDI  R31,HIGH(5)
	RJMP _0x144
; 0000 01B0                                         break;
; 0000 01B1                                     case 2010:    //0110
_0xA8:
	CPI  R30,LOW(0x7DA)
	LDI  R26,HIGH(0x7DA)
	CPC  R31,R26
	BRNE _0xAD
; 0000 01B2                                         LED2 = 1;
	SBI  0x12,6
; 0000 01B3                                         LED3 = 1;
	SBI  0x12,5
; 0000 01B4                                         Data = 6;
	LDI  R30,LOW(6)
	LDI  R31,HIGH(6)
	RJMP _0x144
; 0000 01B5                                         break;
; 0000 01B6                                     case 2000:   //0111
_0xAD:
	CPI  R30,LOW(0x7D0)
	LDI  R26,HIGH(0x7D0)
	CPC  R31,R26
	BRNE _0xB2
; 0000 01B7                                         LED2 = 1;
	SBI  0x12,6
; 0000 01B8                                         LED3 = 1;
	SBI  0x12,5
; 0000 01B9                                         LED4 = 1;
	SBI  0x12,4
; 0000 01BA                                         Data = 7;
	LDI  R30,LOW(7)
	LDI  R31,HIGH(7)
	RJMP _0x144
; 0000 01BB                                         break;
; 0000 01BC                                     case 1000:   //1000
_0xB2:
	CPI  R30,LOW(0x3E8)
	LDI  R26,HIGH(0x3E8)
	CPC  R31,R26
	BRNE _0xB9
; 0000 01BD                                         LED1 = 1;
	SBI  0x12,7
; 0000 01BE                                         Data = 8;
	LDI  R30,LOW(8)
	LDI  R31,HIGH(8)
	RJMP _0x144
; 0000 01BF                                         break;
; 0000 01C0                                     case 1020:   //1001
_0xB9:
	CPI  R30,LOW(0x3FC)
	LDI  R26,HIGH(0x3FC)
	CPC  R31,R26
	BRNE _0xBC
; 0000 01C1                                         LED1 = 1;
	SBI  0x12,7
; 0000 01C2                                         LED4 = 1;
	SBI  0x12,4
; 0000 01C3                                         Data = 9;
	LDI  R30,LOW(9)
	LDI  R31,HIGH(9)
	RJMP _0x144
; 0000 01C4                                         break;
; 0000 01C5                                     case 1210:   //1010
_0xBC:
	CPI  R30,LOW(0x4BA)
	LDI  R26,HIGH(0x4BA)
	CPC  R31,R26
	BRNE _0xC1
; 0000 01C6                                         LED1 = 1;
	SBI  0x12,7
; 0000 01C7                                         LED3 = 1;
	SBI  0x12,5
; 0000 01C8                                         Data = 10;
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	RJMP _0x144
; 0000 01C9                                         break;
; 0000 01CA                                     case 1200: //1011
_0xC1:
	CPI  R30,LOW(0x4B0)
	LDI  R26,HIGH(0x4B0)
	CPC  R31,R26
	BRNE _0xC6
; 0000 01CB                                         LED1 = 1;
	SBI  0x12,7
; 0000 01CC                                         LED3 = 1;
	SBI  0x12,5
; 0000 01CD                                         LED4 = 1;
	SBI  0x12,4
; 0000 01CE                                         Data = 11;
	LDI  R30,LOW(11)
	LDI  R31,HIGH(11)
	RJMP _0x144
; 0000 01CF                                         break;
; 0000 01D0                                     case 100: // 1100
_0xC6:
	CPI  R30,LOW(0x64)
	LDI  R26,HIGH(0x64)
	CPC  R31,R26
	BRNE _0x99
; 0000 01D1                                         LED1 = 1;
	SBI  0x12,7
; 0000 01D2                                         LED2 = 1;
	SBI  0x12,6
; 0000 01D3                                         Data = 12;
	LDI  R30,LOW(12)
	LDI  R31,HIGH(12)
_0x144:
	STS  _Data,R30
	STS  _Data+1,R31
; 0000 01D4                                         break;
; 0000 01D5                                 }
_0x99:
; 0000 01D6 
; 0000 01D7                                 SET_OUTPUT;
	LDI  R30,LOW(63)
	OUT  0x14,R30
; 0000 01D8                                 M8_Q0 = LED4;
	SBIC 0x12,4
	RJMP _0xD2
	CBI  0x15,0
	RJMP _0xD3
_0xD2:
	SBI  0x15,0
_0xD3:
; 0000 01D9                                 M8_Q1 = LED3;
	SBIC 0x12,5
	RJMP _0xD4
	CBI  0x15,1
	RJMP _0xD5
_0xD4:
	SBI  0x15,1
_0xD5:
; 0000 01DA                                 M8_Q2 = LED2;
	SBIC 0x12,6
	RJMP _0xD6
	CBI  0x15,2
	RJMP _0xD7
_0xD6:
	SBI  0x15,2
_0xD7:
; 0000 01DB                                 M8_Q3 = LED1;
	SBIC 0x12,7
	RJMP _0xD8
	CBI  0x15,3
	RJMP _0xD9
_0xD8:
	SBI  0x15,3
_0xD9:
; 0000 01DC 
; 0000 01DD                                 delay_ms(2);
	RCALL SUBOPT_0xB
; 0000 01DE 
; 0000 01DF                                 M8_TO_STM32 = 1;
	SBI  0x15,4
; 0000 01E0                                 delay_ms(8);
	LDI  R26,LOW(8)
	RCALL SUBOPT_0x2B
; 0000 01E1                                 M8_TO_STM32 = 0;
	CBI  0x15,4
; 0000 01E2                                 delay_ms(8);
	LDI  R26,LOW(8)
	RCALL SUBOPT_0x2B
; 0000 01E3                                 M8_Q0 = 0;
	CBI  0x15,0
; 0000 01E4                                 M8_Q1 = 0;
	CBI  0x15,1
; 0000 01E5                                 M8_Q2 = 0;
	CBI  0x15,2
; 0000 01E6                                 M8_Q3 = 0;
	CBI  0x15,3
; 0000 01E7                                 SET_INPUT;
; 0000 01E8 
; 0000 01E9                                 //sprintf(string, "%0d", Data);
; 0000 01EA                                 //lcd_gotoxy(0, 1);
; 0000 01EB                                 //lcd_puts(string);
; 0000 01EC                                 //receive = 1;
; 0000 01ED                             }
; 0000 01EE                         }
_0x8E:
_0x79:
; 0000 01EF 
; 0000 01F0                     }
; 0000 01F1                     for(i=0; i<12; i++)
_0x61:
	RCALL SUBOPT_0x0
_0xE7:
	RCALL SUBOPT_0x1
	SBIW R26,12
	BRGE _0xE8
; 0000 01F2                     {
; 0000 01F3                         AddressData1[i] = 0;
	RCALL SUBOPT_0x1D
	RCALL SUBOPT_0x1E
; 0000 01F4                         AddressData2[i] = 0;
	RCALL SUBOPT_0x1F
	RCALL SUBOPT_0x1E
; 0000 01F5                     }
	RCALL SUBOPT_0x7
	RJMP _0xE7
_0xE8:
; 0000 01F6                     NumberPacket = 1;
	RCALL SUBOPT_0x9
	RJMP _0x145
; 0000 01F7                 }
; 0000 01F8                 else
_0x5C:
; 0000 01F9                 {
; 0000 01FA                     NumberPacket = 2;
	RCALL SUBOPT_0xD
_0x145:
	STS  _NumberPacket,R30
	STS  _NumberPacket+1,R31
; 0000 01FB                 }
; 0000 01FC 
; 0000 01FD                 Error = NO;
	RCALL SUBOPT_0x16
; 0000 01FE                 AddressReceive = NO;
	CLT
	BLD  R2,4
; 0000 01FF                 PulseNumber = 1;
	RCALL SUBOPT_0x17
; 0000 0200                 Sync = NO;
	CLT
	BLD  R2,3
; 0000 0201                 i = 0;
	RCALL SUBOPT_0x0
; 0000 0202                 j = 0;
	RCALL SUBOPT_0x18
; 0000 0203                 TCNT1 = 0;
	RCALL SUBOPT_0x12
; 0000 0204                 RisingEdge = OK;
	SET
	RCALL SUBOPT_0x13
; 0000 0205 
; 0000 0206                 // External Interrupt(s) initialization
; 0000 0207                 // INT0: On
; 0000 0208                 // INT0 Mode: Rising Edge
; 0000 0209                 // INT1: On
; 0000 020A                 // INT1 Mode: Falling Edge
; 0000 020B 
; 0000 020C                 MCUCR=0x0B;
; 0000 020D 
; 0000 020E 
; 0000 020F                 RisingEdge = OK;
	SET
	BLD  R2,0
; 0000 0210                 StartT2 = NO;
	CLT
	BLD  R2,1
; 0000 0211                 GetPulse = NO;
; 0000 0212                 goto EndInterrupt;
; 0000 0213             }
; 0000 0214         }
_0x5B:
; 0000 0215 
; 0000 0216         GetPulse = NO;
_0x2F:
_0x2E:
_0x13F:
	CLT
	BLD  R2,2
; 0000 0217     }
; 0000 0218     EndInterrupt:
_0x29:
; 0000 0219 
; 0000 021A     //Add to version 2
; 0000 021B     POWER_LED_Cunter++;
	MOVW R30,R8
	ADIW R30,1
	MOVW R8,R30
; 0000 021C     if(POWER_LED_Cunter > 5000 && PowerStatus == 1)
	LDI  R30,LOW(5000)
	LDI  R31,HIGH(5000)
	CP   R30,R8
	CPC  R31,R9
	BRGE _0xEB
	RCALL SUBOPT_0x9
	RCALL SUBOPT_0xA
	BREQ _0xEC
_0xEB:
	RJMP _0xEA
_0xEC:
; 0000 021D     {
; 0000 021E         POWER_LED = 1 ;
	SBI  0x15,5
; 0000 021F     }
; 0000 0220     if(POWER_LED_Cunter > 5500 && PowerStatus == 1)
_0xEA:
	LDI  R30,LOW(5500)
	LDI  R31,HIGH(5500)
	CP   R30,R8
	CPC  R31,R9
	BRGE _0xF0
	RCALL SUBOPT_0x9
	RCALL SUBOPT_0xA
	BREQ _0xF1
_0xF0:
	RJMP _0xEF
_0xF1:
; 0000 0221     {
; 0000 0222         POWER_LED = 0 ;
	CBI  0x15,5
; 0000 0223         POWER_LED_Cunter = 0;
	CLR  R8
	CLR  R9
; 0000 0224     }
; 0000 0225 }
_0xEF:
_0x147:
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	LD   R27,Y+
	LD   R26,Y+
	LD   R25,Y+
	LD   R24,Y+
	LD   R23,Y+
	LD   R22,Y+
	LD   R15,Y+
	LD   R1,Y+
	LD   R0,Y+
	RETI
;//-----------------------------------------------------------------------------------
;
;
;
;void main(void)
; 0000 022B {
_main:
; 0000 022C     POWER_LED = 0;
	CBI  0x15,5
; 0000 022D     Init();
	RCALL _Init
; 0000 022E 
; 0000 022F       while (1)
_0xF6:
; 0000 0230       {
; 0000 0231            /*
; 0000 0232             if(!LEARN){
; 0000 0233                 M8_TO_STM32 = 1;
; 0000 0234                 delay_ms(250);
; 0000 0235                 M8_TO_STM32 = 0;
; 0000 0236             }
; 0000 0237            */
; 0000 0238             if(StartPower == 0)
	MOV  R0,R12
	OR   R0,R13
	BRNE _0xF9
; 0000 0239             {
; 0000 023A                 POWER_LED = 0;
	CBI  0x15,5
; 0000 023B                 delay_ms(4000);
	LDI  R26,LOW(4000)
	LDI  R27,HIGH(4000)
	RCALL _delay_ms
; 0000 023C                 LED1 = 1;
	RCALL SUBOPT_0x2C
; 0000 023D                 delay_ms(DELAY);
; 0000 023E                 LED1 = 0;
	RCALL SUBOPT_0x2D
; 0000 023F                 LED2 = 1;
; 0000 0240                 delay_ms(DELAY);
; 0000 0241                 LED2 = 0;
	RCALL SUBOPT_0x2E
; 0000 0242                 LED3 = 1;
; 0000 0243                 delay_ms(DELAY);
; 0000 0244                 LED3 = 0;
	RCALL SUBOPT_0x2F
; 0000 0245                 LED4 = 1;
; 0000 0246                 delay_ms(DELAY);
; 0000 0247                 LED4 = 0;
	CBI  0x12,4
; 0000 0248 
; 0000 0249 
; 0000 024A                 //RELAY_OUT = 1;
; 0000 024B                 RELAY_POWER = 1;
	SBI  0x18,2
; 0000 024C 
; 0000 024D                 PowerStatus = 1;
	RCALL SUBOPT_0x9
	MOVW R4,R30
; 0000 024E                 delay_ms(20);
	LDI  R26,LOW(20)
	RCALL SUBOPT_0x2B
; 0000 024F 
; 0000 0250                 StartPower = 1;
	RCALL SUBOPT_0x9
	MOVW R12,R30
; 0000 0251             }
; 0000 0252 
; 0000 0253 
; 0000 0254             if(PIND.1 == 1)
_0xF9:
	SBIS 0x10,1
	RJMP _0x10E
; 0000 0255                 KEY_PRESS = 0;
	CLR  R10
	CLR  R11
; 0000 0256 
; 0000 0257             if(PIND.1 == 0 && PowerStatus == 0 && KEY_PRESS == 0)
_0x10E:
	RCALL SUBOPT_0x30
	BRNE _0x110
	CLR  R0
	CP   R0,R4
	CPC  R0,R5
	BRNE _0x110
	CLR  R0
	CP   R0,R10
	CPC  R0,R11
	BREQ _0x111
_0x110:
	RJMP _0x10F
_0x111:
; 0000 0258             {
; 0000 0259                 KEY_PRESS = 1;
	RCALL SUBOPT_0x9
	MOVW R10,R30
; 0000 025A 
; 0000 025B                 LED1 = 1;
	RCALL SUBOPT_0x2C
; 0000 025C                 delay_ms(DELAY);
; 0000 025D                 LED1 = 0;
	RCALL SUBOPT_0x2D
; 0000 025E                 LED2 = 1;
; 0000 025F                 delay_ms(DELAY);
; 0000 0260                 LED2 = 0;
	RCALL SUBOPT_0x2E
; 0000 0261                 LED3 = 1;
; 0000 0262                 delay_ms(DELAY);
; 0000 0263                 LED3 = 0;
	RCALL SUBOPT_0x2F
; 0000 0264                 LED4 = 1;
; 0000 0265                 delay_ms(DELAY);
; 0000 0266                 LED4 = 0;
	CBI  0x12,4
; 0000 0267                 POWER_LED = 0;
	CBI  0x15,5
; 0000 0268 
; 0000 0269                 //RELAY_OUT = 1;
; 0000 026A                 RELAY_POWER = 1;
	SBI  0x18,2
; 0000 026B 
; 0000 026C                 PowerStatus = 1;
	RCALL SUBOPT_0x9
	MOVW R4,R30
; 0000 026D                 delay_ms(20);
	RJMP _0x146
; 0000 026E             }
; 0000 026F             else if(PIND.1 == 0 && PowerStatus == 1 && KEY_PRESS == 0) //if Device On
_0x10F:
	RCALL SUBOPT_0x30
	BRNE _0x128
	RCALL SUBOPT_0x9
	RCALL SUBOPT_0xA
	BRNE _0x128
	CLR  R0
	CP   R0,R10
	CPC  R0,R11
	BREQ _0x129
_0x128:
	RJMP _0x127
_0x129:
; 0000 0270             {
; 0000 0271                 KEY_PRESS = 1;
	RCALL SUBOPT_0x9
	MOVW R10,R30
; 0000 0272 
; 0000 0273                 PowerStatus = 0;
	CLR  R4
	CLR  R5
; 0000 0274                 POWER_LED = 0;
	CBI  0x15,5
; 0000 0275                 LED4 = 1;
	SBI  0x12,4
; 0000 0276                 delay_ms(DELAY);
	LDI  R26,LOW(120)
	RCALL SUBOPT_0x2B
; 0000 0277                 LED4 = 0;
	CBI  0x12,4
; 0000 0278                 LED3 = 1;
	SBI  0x12,5
; 0000 0279                 delay_ms(DELAY);
	LDI  R26,LOW(120)
	RCALL SUBOPT_0x2B
; 0000 027A                 LED3 = 0;
	CBI  0x12,5
; 0000 027B                 LED2 = 1;
	SBI  0x12,6
; 0000 027C                 delay_ms(DELAY);
	LDI  R26,LOW(120)
	RCALL SUBOPT_0x2B
; 0000 027D                 LED2 = 0;
	CBI  0x12,6
; 0000 027E                 LED1 = 1;
	RCALL SUBOPT_0x2C
; 0000 027F                 delay_ms(DELAY);
; 0000 0280                 LED1 = 0;
	CBI  0x12,7
; 0000 0281 
; 0000 0282                 //RELAY_OUT = 0;
; 0000 0283                 RELAY_POWER = 0;
	CBI  0x18,2
; 0000 0284 
; 0000 0285                 delay_ms(20);
_0x146:
	LDI  R26,LOW(20)
	RCALL SUBOPT_0x2B
; 0000 0286             }
; 0000 0287 
; 0000 0288 
; 0000 0289  }
_0x127:
	RJMP _0xF6
; 0000 028A }
_0x13E:
	RJMP _0x13E
;
;
;
;
;
;
;
;
;
;
;
;
;
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x80
	.EQU __sm_mask=0x70
	.EQU __sm_powerdown=0x20
	.EQU __sm_powersave=0x30
	.EQU __sm_standby=0x60
	.EQU __sm_ext_standby=0x70
	.EQU __sm_adc_noise_red=0x10
	.SET power_ctrl_reg=mcucr
	#endif

	.CSEG

	.CSEG

	.CSEG

	.DSEG
_T1:
	.BYTE 0x4
_T2:
	.BYTE 0x4
_DutyCycle:
	.BYTE 0x4
_i:
	.BYTE 0x2
_j:
	.BYTE 0x2
_PulseNumber:
	.BYTE 0x2
_Pulse1:
	.BYTE 0x2
_Pulse2:
	.BYTE 0x2
_AddressData1:
	.BYTE 0x18
_AddressData2:
	.BYTE 0x18
_AddressData:
	.BYTE 0x10

	.ESEG
_EepromAddressData:
	.BYTE 0x10

	.DSEG
_NumberPacket:
	.BYTE 0x2
_Error:
	.BYTE 0x2
_Data:
	.BYTE 0x2

	.CSEG
;OPTIMIZER ADDED SUBROUTINE, CALLED 10 TIMES, CODE SIZE REDUCTION:34 WORDS
SUBOPT_0x0:
	LDI  R30,LOW(0)
	STS  _i,R30
	STS  _i+1,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:22 WORDS
SUBOPT_0x1:
	LDS  R26,_i
	LDS  R27,_i+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 14 TIMES, CODE SIZE REDUCTION:37 WORDS
SUBOPT_0x2:
	LDS  R30,_i
	LDS  R31,_i+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x3:
	LDI  R26,LOW(_AddressData)
	LDI  R27,HIGH(_AddressData)
	LSL  R30
	ROL  R31
	ADD  R30,R26
	ADC  R31,R27
	MOVW R0,R30
	RJMP SUBOPT_0x2

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x4:
	LDI  R26,LOW(_EepromAddressData)
	LDI  R27,HIGH(_EepromAddressData)
	LSL  R30
	ROL  R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 15 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0x5:
	ADD  R26,R30
	ADC  R27,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x6:
	ST   X+,R30
	ST   X,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:40 WORDS
SUBOPT_0x7:
	LDI  R26,LOW(_i)
	LDI  R27,HIGH(_i)
	LD   R30,X+
	LD   R31,X+
	ADIW R30,1
	ST   -X,R31
	ST   -X,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:10 WORDS
SUBOPT_0x8:
	ST   -Y,R0
	ST   -Y,R1
	ST   -Y,R15
	ST   -Y,R22
	ST   -Y,R23
	ST   -Y,R24
	ST   -Y,R25
	ST   -Y,R26
	ST   -Y,R27
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 20 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0x9:
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xA:
	CP   R30,R4
	CPC  R31,R5
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xB:
	LDI  R26,LOW(2)
	LDI  R27,0
	RJMP _delay_ms

;OPTIMIZER ADDED SUBROUTINE, CALLED 13 TIMES, CODE SIZE REDUCTION:10 WORDS
SUBOPT_0xC:
	LDI  R30,LOW(0)
	LDI  R31,HIGH(0)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xD:
	LDI  R30,LOW(2)
	LDI  R31,HIGH(2)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xE:
	LDI  R30,LOW(4)
	LDI  R31,HIGH(4)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0xF:
	SBI  0x12,7
	SBI  0x12,6
	SBI  0x12,5
	SBI  0x12,4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x10:
	RCALL _delay_ms
	CBI  0x12,7
	CBI  0x12,6
	CBI  0x12,5
	CBI  0x12,4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x11:
	CLR  R22
	CLR  R23
	RCALL __CDF1
	RCALL __PUTDP1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x12:
	RCALL SUBOPT_0xC
	OUT  0x2C+1,R31
	OUT  0x2C,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x13:
	BLD  R2,0
	LDI  R30,LOW(11)
	OUT  0x35,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:31 WORDS
SUBOPT_0x14:
	LDS  R30,_T2
	LDS  R31,_T2+1
	LDS  R22,_T2+2
	LDS  R23,_T2+3
	LDS  R26,_T1
	LDS  R27,_T1+1
	LDS  R24,_T1+2
	LDS  R25,_T1+3
	RCALL __ADDF12
	LDS  R26,_T1
	LDS  R27,_T1+1
	LDS  R24,_T1+2
	LDS  R25,_T1+3
	RCALL __DIVF21
	STS  _DutyCycle,R30
	STS  _DutyCycle+1,R31
	STS  _DutyCycle+2,R22
	STS  _DutyCycle+3,R23
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:33 WORDS
SUBOPT_0x15:
	LDS  R26,_DutyCycle
	LDS  R27,_DutyCycle+1
	LDS  R24,_DutyCycle+2
	LDS  R25,_DutyCycle+3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:10 WORDS
SUBOPT_0x16:
	LDI  R30,LOW(0)
	STS  _Error,R30
	STS  _Error+1,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x17:
	RCALL SUBOPT_0x9
	STS  _PulseNumber,R30
	STS  _PulseNumber+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x18:
	LDI  R30,LOW(0)
	STS  _j,R30
	STS  _j+1,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:16 WORDS
SUBOPT_0x19:
	LDS  R26,_PulseNumber
	LDS  R27,_PulseNumber+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x1A:
	STS  _Pulse1,R30
	STS  _Pulse1+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x1B:
	RCALL SUBOPT_0x19
	SBIW R26,2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x1C:
	STS  _Pulse2,R30
	STS  _Pulse2+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x1D:
	RCALL SUBOPT_0x2
	LDI  R26,LOW(_AddressData1)
	LDI  R27,HIGH(_AddressData1)
	LSL  R30
	ROL  R31
	RJMP SUBOPT_0x5

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x1E:
	RCALL SUBOPT_0xC
	RJMP SUBOPT_0x6

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x1F:
	RCALL SUBOPT_0x2
	LDI  R26,LOW(_AddressData2)
	LDI  R27,HIGH(_AddressData2)
	LSL  R30
	ROL  R31
	RJMP SUBOPT_0x5

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x20:
	LDS  R26,_Pulse1
	LDS  R27,_Pulse1+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x21:
	LDS  R26,_Pulse2
	LDS  R27,_Pulse2+1
	SBIW R26,4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:16 WORDS
SUBOPT_0x22:
	LDS  R26,_NumberPacket
	LDS  R27,_NumberPacket+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x23:
	LDS  R30,_j
	LDS  R31,_j+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x24:
	RCALL SUBOPT_0x22
	SBIW R26,2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x25:
	RCALL SUBOPT_0x23
	LDI  R26,LOW(_AddressData2)
	LDI  R27,HIGH(_AddressData2)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x26:
	RCALL SUBOPT_0x9
	STS  _Error,R30
	STS  _Error+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x27:
	LDS  R30,_Error
	LDS  R31,_Error+1
	SBIW R30,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x28:
	LDI  R26,LOW(100)
	LDI  R27,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 17 TIMES, CODE SIZE REDUCTION:30 WORDS
SUBOPT_0x29:
	LDI  R27,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:10 WORDS
SUBOPT_0x2A:
	STS  _Data,R30
	STS  _Data+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 16 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x2B:
	RCALL SUBOPT_0x29
	RJMP _delay_ms

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x2C:
	SBI  0x12,7
	LDI  R26,LOW(120)
	RJMP SUBOPT_0x2B

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x2D:
	CBI  0x12,7
	SBI  0x12,6
	LDI  R26,LOW(120)
	RJMP SUBOPT_0x2B

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x2E:
	CBI  0x12,6
	SBI  0x12,5
	LDI  R26,LOW(120)
	RJMP SUBOPT_0x2B

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x2F:
	CBI  0x12,5
	SBI  0x12,4
	LDI  R26,LOW(120)
	RJMP SUBOPT_0x2B

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x30:
	LDI  R26,0
	SBIC 0x10,1
	LDI  R26,1
	CPI  R26,LOW(0x0)
	RET


	.CSEG
_delay_ms:
	adiw r26,0
	breq __delay_ms1
__delay_ms0:
	__DELAY_USW 0xFA0
	wdr
	sbiw r26,1
	brne __delay_ms0
__delay_ms1:
	ret

__ROUND_REPACK:
	TST  R21
	BRPL __REPACK
	CPI  R21,0x80
	BRNE __ROUND_REPACK0
	SBRS R30,0
	RJMP __REPACK
__ROUND_REPACK0:
	ADIW R30,1
	ADC  R22,R25
	ADC  R23,R25
	BRVS __REPACK1

__REPACK:
	LDI  R21,0x80
	EOR  R21,R23
	BRNE __REPACK0
	PUSH R21
	RJMP __ZERORES
__REPACK0:
	CPI  R21,0xFF
	BREQ __REPACK1
	LSL  R22
	LSL  R0
	ROR  R21
	ROR  R22
	MOV  R23,R21
	RET
__REPACK1:
	PUSH R21
	TST  R0
	BRMI __REPACK2
	RJMP __MAXRES
__REPACK2:
	RJMP __MINRES

__UNPACK:
	LDI  R21,0x80
	MOV  R1,R25
	AND  R1,R21
	LSL  R24
	ROL  R25
	EOR  R25,R21
	LSL  R21
	ROR  R24

__UNPACK1:
	LDI  R21,0x80
	MOV  R0,R23
	AND  R0,R21
	LSL  R22
	ROL  R23
	EOR  R23,R21
	LSL  R21
	ROR  R22
	RET

__CDF1U:
	SET
	RJMP __CDF1U0
__CDF1:
	CLT
__CDF1U0:
	SBIW R30,0
	SBCI R22,0
	SBCI R23,0
	BREQ __CDF10
	CLR  R0
	BRTS __CDF11
	TST  R23
	BRPL __CDF11
	COM  R0
	RCALL __ANEGD1
__CDF11:
	MOV  R1,R23
	LDI  R23,30
	TST  R1
__CDF12:
	BRMI __CDF13
	DEC  R23
	LSL  R30
	ROL  R31
	ROL  R22
	ROL  R1
	RJMP __CDF12
__CDF13:
	MOV  R30,R31
	MOV  R31,R22
	MOV  R22,R1
	PUSH R21
	RCALL __REPACK
	POP  R21
__CDF10:
	RET

__SWAPACC:
	PUSH R20
	MOVW R20,R30
	MOVW R30,R26
	MOVW R26,R20
	MOVW R20,R22
	MOVW R22,R24
	MOVW R24,R20
	MOV  R20,R0
	MOV  R0,R1
	MOV  R1,R20
	POP  R20
	RET

__UADD12:
	ADD  R30,R26
	ADC  R31,R27
	ADC  R22,R24
	RET

__NEGMAN1:
	COM  R30
	COM  R31
	COM  R22
	SUBI R30,-1
	SBCI R31,-1
	SBCI R22,-1
	RET

__ADDF12:
	PUSH R21
	RCALL __UNPACK
	CPI  R25,0x80
	BREQ __ADDF129

__ADDF120:
	CPI  R23,0x80
	BREQ __ADDF128
__ADDF121:
	MOV  R21,R23
	SUB  R21,R25
	BRVS __ADDF1211
	BRPL __ADDF122
	RCALL __SWAPACC
	RJMP __ADDF121
__ADDF122:
	CPI  R21,24
	BRLO __ADDF123
	CLR  R26
	CLR  R27
	CLR  R24
__ADDF123:
	CPI  R21,8
	BRLO __ADDF124
	MOV  R26,R27
	MOV  R27,R24
	CLR  R24
	SUBI R21,8
	RJMP __ADDF123
__ADDF124:
	TST  R21
	BREQ __ADDF126
__ADDF125:
	LSR  R24
	ROR  R27
	ROR  R26
	DEC  R21
	BRNE __ADDF125
__ADDF126:
	MOV  R21,R0
	EOR  R21,R1
	BRMI __ADDF127
	RCALL __UADD12
	BRCC __ADDF129
	ROR  R22
	ROR  R31
	ROR  R30
	INC  R23
	BRVC __ADDF129
	RJMP __MAXRES
__ADDF128:
	RCALL __SWAPACC
__ADDF129:
	RCALL __REPACK
	POP  R21
	RET
__ADDF1211:
	BRCC __ADDF128
	RJMP __ADDF129
__ADDF127:
	SUB  R30,R26
	SBC  R31,R27
	SBC  R22,R24
	BREQ __ZERORES
	BRCC __ADDF1210
	COM  R0
	RCALL __NEGMAN1
__ADDF1210:
	TST  R22
	BRMI __ADDF129
	LSL  R30
	ROL  R31
	ROL  R22
	DEC  R23
	BRVC __ADDF1210

__ZERORES:
	CLR  R30
	CLR  R31
	CLR  R22
	CLR  R23
	POP  R21
	RET

__MINRES:
	SER  R30
	SER  R31
	LDI  R22,0x7F
	SER  R23
	POP  R21
	RET

__MAXRES:
	SER  R30
	SER  R31
	LDI  R22,0x7F
	LDI  R23,0x7F
	POP  R21
	RET

__DIVF21:
	PUSH R21
	RCALL __UNPACK
	CPI  R23,0x80
	BRNE __DIVF210
	TST  R1
__DIVF211:
	BRPL __DIVF219
	RJMP __MINRES
__DIVF219:
	RJMP __MAXRES
__DIVF210:
	CPI  R25,0x80
	BRNE __DIVF218
__DIVF217:
	RJMP __ZERORES
__DIVF218:
	EOR  R0,R1
	SEC
	SBC  R25,R23
	BRVC __DIVF216
	BRLT __DIVF217
	TST  R0
	RJMP __DIVF211
__DIVF216:
	MOV  R23,R25
	PUSH R17
	PUSH R18
	PUSH R19
	PUSH R20
	CLR  R1
	CLR  R17
	CLR  R18
	CLR  R19
	CLR  R20
	CLR  R21
	LDI  R25,32
__DIVF212:
	CP   R26,R30
	CPC  R27,R31
	CPC  R24,R22
	CPC  R20,R17
	BRLO __DIVF213
	SUB  R26,R30
	SBC  R27,R31
	SBC  R24,R22
	SBC  R20,R17
	SEC
	RJMP __DIVF214
__DIVF213:
	CLC
__DIVF214:
	ROL  R21
	ROL  R18
	ROL  R19
	ROL  R1
	ROL  R26
	ROL  R27
	ROL  R24
	ROL  R20
	DEC  R25
	BRNE __DIVF212
	MOVW R30,R18
	MOV  R22,R1
	POP  R20
	POP  R19
	POP  R18
	POP  R17
	TST  R22
	BRMI __DIVF215
	LSL  R21
	ROL  R30
	ROL  R31
	ROL  R22
	DEC  R23
	BRVS __DIVF217
__DIVF215:
	RCALL __ROUND_REPACK
	POP  R21
	RET

__CMPF12:
	TST  R25
	BRMI __CMPF120
	TST  R23
	BRMI __CMPF121
	CP   R25,R23
	BRLO __CMPF122
	BRNE __CMPF121
	CP   R26,R30
	CPC  R27,R31
	CPC  R24,R22
	BRLO __CMPF122
	BREQ __CMPF123
__CMPF121:
	CLZ
	CLC
	RET
__CMPF122:
	CLZ
	SEC
	RET
__CMPF123:
	SEZ
	CLC
	RET
__CMPF120:
	TST  R23
	BRPL __CMPF122
	CP   R25,R23
	BRLO __CMPF121
	BRNE __CMPF122
	CP   R30,R26
	CPC  R31,R27
	CPC  R22,R24
	BRLO __CMPF122
	BREQ __CMPF123
	RJMP __CMPF121

__ANEGW1:
	NEG  R31
	NEG  R30
	SBCI R31,0
	RET

__ANEGD1:
	COM  R31
	COM  R22
	COM  R23
	NEG  R30
	SBCI R31,-1
	SBCI R22,-1
	SBCI R23,-1
	RET

__CWD1:
	MOV  R22,R31
	ADD  R22,R22
	SBC  R22,R22
	MOV  R23,R22
	RET

__MULW12U:
	MUL  R31,R26
	MOV  R31,R0
	MUL  R30,R27
	ADD  R31,R0
	MUL  R30,R26
	MOV  R30,R0
	ADD  R31,R1
	RET

__MULW12:
	RCALL __CHKSIGNW
	RCALL __MULW12U
	BRTC __MULW121
	RCALL __ANEGW1
__MULW121:
	RET

__CHKSIGNW:
	CLT
	SBRS R31,7
	RJMP __CHKSW1
	RCALL __ANEGW1
	SET
__CHKSW1:
	SBRS R27,7
	RJMP __CHKSW2
	COM  R26
	COM  R27
	ADIW R26,1
	BLD  R0,0
	INC  R0
	BST  R0,0
__CHKSW2:
	RET

__GETW1P:
	LD   R30,X+
	LD   R31,X
	SBIW R26,1
	RET

__PUTDP1:
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	RET

__EEPROMRDW:
	ADIW R26,1
	RCALL __EEPROMRDB
	MOV  R31,R30
	SBIW R26,1

__EEPROMRDB:
	SBIC EECR,EEWE
	RJMP __EEPROMRDB
	PUSH R31
	IN   R31,SREG
	CLI
	OUT  EEARL,R26
	OUT  EEARH,R27
	SBI  EECR,EERE
	IN   R30,EEDR
	OUT  SREG,R31
	POP  R31
	RET

__EEPROMWRW:
	RCALL __EEPROMWRB
	ADIW R26,1
	PUSH R30
	MOV  R30,R31
	RCALL __EEPROMWRB
	POP  R30
	SBIW R26,1
	RET

__EEPROMWRB:
	SBIS EECR,EEWE
	RJMP __EEPROMWRB1
	WDR
	RJMP __EEPROMWRB
__EEPROMWRB1:
	IN   R25,SREG
	CLI
	OUT  EEARL,R26
	OUT  EEARH,R27
	SBI  EECR,EERE
	IN   R24,EEDR
	CP   R30,R24
	BREQ __EEPROMWRB0
	OUT  EEDR,R30
	SBI  EECR,EEMWE
	SBI  EECR,EEWE
__EEPROMWRB0:
	OUT  SREG,R25
	RET

;END OF CODE MARKER
__END_OF_CODE:
