/* ----------------------------------------------------------------------------
 File Name: HdicType.h

 Description:

 Version 1.0 : Created   2012.03.21  Sharp Wang
 				 2.0 ：Modified  2013.03.05  Sharp Wang	 

 ---------------------------------------------------------------------------- */ 
#ifndef __HDICTYPE_H__
#define __HDICTYPE_H__

// it define in x86DataType
#if 0
typedef int						BOOL;
typedef signed char		INT8;	/**< 8 bits, bit 7 is the signed bit */
typedef signed short	INT16;	/**< 16 bits, bit 15 is the signed bit */
typedef signed int		INT32;	/**< 32 bits, bit 31 is the signed bit */
typedef unsigned char	UINT8;	/**< 8 bits */
typedef unsigned short	UINT16;	/**< 16 bits */
typedef unsigned int	UINT32;	/**< 32 bits */
#endif


#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

enum
{
	HDIC_NO_ERROR = 0,				/* no error */
	HDIC_OTHER_ERROR					/* other error*/
};

enum
{
    HDIC2311_BANDWIDTH_6M,	   /* system  bandwidth  6 MHz */   
    HDIC2311_BANDWIDTH_7M,      /* system  bandwidth  7 MHz */   
    HDIC2311_BANDWIDTH_8M       /* system  bandwidth  8 MHz */   
};

enum
{
	HDIC2311_CABLE_16QAM,			/*Cable Mode, 16QAM*/
	HDIC2311_CABLE_32QAM,			/*Cable Mode, 32QAM*/
	HDIC2311_CABLE_64QAM,			/*Cable Mode, 64QAM*/
	HDIC2311_CABLE_128QAM,		    /*Cable Mode, 128QAM*/
	HDIC2311_CABLE_256QAM			/*Cable Mode, 256QAM*/
};

enum
{
	HDIC2311_DTMB_SELECT,			/*WorkMode, DTMB*/
	HDIC2311_CABLE_SELECT 		    /*WorkMode, Cable*/
};

enum
{
	HDIC2311_CRYSTAL_27M,			/*CrystalSelect, 27MHz*/
	HDIC2311_CRYSTAL_304M,		    /*CrystalSelect, 30.4MHz*/
	HDIC2311_CRYSTAL_24M,			/*CrystalSelect, 24MHz*/
	HDIC2311_CRYSTAL_16M			/*CrystalSelect, 16MHz*/
};

enum
{
	HDIC_TUNER_NORMAL,				/*TunerType, Normal IF*/
	HDIC_TUNER_ZERO_IF				/*TunerType, Zero IF*/
};
enum
{
	HDIC_DTMB_4QAM_NR,				    /*DTMB Mode, 4QAM NR*/
	HDIC_DTMB_4QAM,						/*DTMB Mode, 4QAM*/
	HDIC_DTMB_16QAM,					/*DTMB Mode, 16QAM*/	
	HDIC_DTMB_32QAM,					/*DTMB Mode, 32QAM*/
	HDIC_DTMB_64QAM						/*DTMB Mode, 64QAM*/
};

enum
{
	HDIC_PN_VARIABLE,				    /*变相位*/
	HDIC_PN_FIXED						/*定相位*/
};

enum
{
	HDIC_RATE_04,					/*0.4*/
	HDIC_RATE_06,					/*0.6*/
	HDIC_RATE_08					/*0.8*/
};

enum
{
	HDIC_CARRIER_SINGLE,			/*单载波*/
	HDIC_CARRIER_MULTI				/*多载波*/
};

enum{
	HDIC_PN_945,					/*PN945*/
	HDIC_PN_595,					/*PN595*/
	HDIC_PN_420						/*PN420*/
};

enum
{
	HDIC_INTERLEAVER_720,			/*交织长度720*/
	HDIC_INTERLEAVER_240			/*交织长度240*/
};

enum
{
	HDIC_TS_OUTPUT_PARALLEL,			    /* TS parallel output */
	HDIC_TS_OUTPUT_SERIAL					/* TS serial output */
};

enum
{
	HDIC_TS_DATA_SAMPLE_RISING,			/* TS Data should be sampled at the rising edge of TS_CLK */
	HDIC_TS_DATA_SAMPLE_FALLING			/* TS Data should be sampled at the falling edge of TS_CLK */
};

enum
{
	HDIC_TS_CLK_ALWAYS,						/* TS TS_CLK is always on */
	HDIC_TS_CLK_WITH_TSVLD				    /* TS TS_CLK is set to 0 when TS_VLD is low */
};

enum
{
	HDIC_TS_NULLPACKET_ENABLED,			/* TS Null packets are enabled.*/
	HDIC_TS_NULLPACKET_DELETED			/* TS Null packets are deleted.*/
};

enum
{
	HDIC_TS_SERIAL_OUTPUT_D7,			/* When TS is serial,TS_D7 is valid.*/
	HDIC_TS_SERIAL_OUTPUT_D0			/* When TS is serial,TS_D0 is valid*/
};

#endif
