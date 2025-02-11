/*  -*-  mode: c; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4; coding: utf-8  -*-  */
/************************************************************************************
 **                                                                                 **
 **                                        UHSDR                                    **
 **               a powerful firmware for STM32 based SDR transceivers              **
 **                                                                                 **
 **---------------------------------------------------------------------------------**
 **                                                                                 **
 **  File name:                                                                     **
 **  Description:                                                                   **
 **  Last Modified:                                                                 **
 **  Licence:		GNU GPLv3                                                      **
 ************************************************************************************/

#define 	UHSDR_VER_MAJOR			"2"
#define 	UHSDR_VER_MINOR			"14"
#define 	UHSDR_VER_RELEASE		"1a"


#define		UHSDR_BOOT_VERS			"5.0.4m"


#ifdef SBLA									// Bootloader enables additional rf bands PCB
  #define	UHSDR_BOOT_VER			UHSDR_BOOT_VERS"a"
#elif SBLS									// Bootloader enables special debug capabilities
  #define	UHSDR_BOOT_VER			UHSDR_BOOT_VERS"s"
#else										// Standard Bootloader
  #define	UHSDR_BOOT_VER			UHSDR_BOOT_VERS
#endif

// trailing characters are needed for identifying version and building date+time in binary
#define	    UHSDR_VERSION           "fwv-"UHSDR_VER_MAJOR"."UHSDR_VER_MINOR"."UHSDR_VER_RELEASE
#define	    UHSDR_BUILD_DAT         "fwt-"__DATE__ " - " __TIME__
#define	    UHSDR_LICENCE           "GNU GPLv3"
#define     UHSDR_REPO              "github.com/strijar/UHSDR"
#define     UHSDR_COMMENT           "MOVVA ["__TIME__"]"
