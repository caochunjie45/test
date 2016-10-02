//------------------------------------
//David.H, rda5880h driver, 2013-05-07
//------------------------------------
// #include <stdio.h>
#include "rdatv_bb_defs.h"
#include "rdatv_bb_user_define.h"

#if 0
#define RDA_MSG(p)		printf p
#else
#define RDA_MSG(p) 		
#endif
#if 0
#define RDA_DBG(p)		printf p
#else
#define RDA_DBG(...) 		
#endif


RDA5880_ERR_MSG RDATVSetFreq(UINT32 freq, UINT32 bw, RDA5880_PARAM *rf_param );
void RDATVSetAdcIndex(UINT8 index, RDA5880_PARAM *rf_param);
RDA5880_ERR_MSG RDATVsetlna(UINT32 freq, RDA5880_PARAM *rf_param);
RDA5880_ERR_MSG RDATVRfBp(UINT8 on);
RDA5880_ERR_MSG RDATVdelIM(UINT8 on);



//#define RDA5880H_TUNER_I2C_ADDR 0xc0

#if (MTC_CUSTOMER_CONFIG==MTC_ENGEL_REQUEST)
#define MTC_STANDBY_MODE_RF_LOOP_DISABLE
#endif

//static UINT32 gu32FreqKHz= 474000;
//static UINT32 gu32BandwidthKHz=8000;
//static UINT8 gu8Count=0;
//static UINT8 gb8Display = 0;
//static bool bRDAStandbyMode = FALSE;

/*
static RDATVM_REG const tvpwon[]=
{
	#include "rda5880h_poweron_firmware.h"
};
*/
/*
static UINT32 const freq_adc_tbl[]=
{
	151875,162000,171125,0,141750,162000,182250
};
*/
/*
static UINT16 const freq_if_tbl[]=
{
	2900,3025,2675,2675,2000,2000,2900,2900,0
};
*/
/*
static UINT16 const freq_ifout_tbl[]=
{
	6850,7250,6400,6400,6400,6400,6750,6150,4500
};
*/
//#ifdef	RDATV_TEST
//#pragma	udata
//#endif
/*
static UINT16 TVReg240,TVReg242,TVReg62,TVReg64,TVReg250,TVReg24e,TVReg232,TVReg66;
static UINT8 TVReg46_2_X,TVreg1c_0,TVReg10_8_10,TVReg14_10_12;
static UINT8 tvmode,ifout_mode,freq_adc_index,mode_t;
static UINT32	freq_ifout;
*/
//#ifdef	RDATV_TEST
//#pragma
//#endif

//static void RDATVRfBp(UINT8 on);
//static void RDATVdelIM(UINT8 on);

RDA5880_ERR_MSG RDATVM_WRITE(UINT8 reg, UINT16 tdata)
{
	UINT8 u8Data[2]={0, 0};
	RDA5880_ERR_MSG ret = RDA5880_OK;

	u8Data[0] = tdata>>8;
	u8Data[1] = tdata&0xFF;
	ret=HAL_TUNER_Write( RDA5880H_TUNER_I2C_ADDR, &reg, 1, u8Data, 2);
	if(ret==RDA5880_ERR)
	{
		RDA_DBG(">RDA 5880H IIC_WRITE Error \n");
	}
	return ret;
}

RDA5880_ERR_MSG RDATVM_READ(UINT8 reg,UINT16* pdata)
{
	RDA5880_ERR_MSG ret = RDA5880_OK;
	UINT8 data[2];
	UINT16 u16buffer;
	
	ret = HAL_TUNER_ReadbyAddr(RDA5880H_TUNER_I2C_ADDR, reg, data, 2);
	
	if(ret==RDA5880_ERR)
	{
		RDA_DBG(">RDA5880p_Read error: reg:0x%2x\n",reg);
		*pdata=0;
		return RDA5880_ERR;
	}
	else
	{
		//RDA_DBG(("RDA5880p_Read : 0X%2x     0X%2x  0X%2x\n",reg,data[0],data[1])); 
		u16buffer = data[0];
		*pdata = (u16buffer<<8)|data[1];
		return RDA5880_OK;
	}
}

static void RDADelayMs(UINT32 delay_ms)
{	
	SAL_PLT_DelayMS(delay_ms);
}

#ifndef CLOSE_TUNER_UNNECESSARY_FUNCTION
RDA5880_ERR_MSG RDATVDetected( int *detect )
{
	UINT16 chipid;
	RDA5880_ERR_MSG err_msg  = RDA5880_OK;

	err_msg = RDATVM_WRITE(0xff,0);
	err_msg |= RDATVM_READ(0xe2,&chipid);

	if( chipid==0x18 ) *detect = 1 ;
	else               *detect = 0 ;

	return RDA5880_OK ;
}
#endif
	
RDA5880_ERR_MSG	RDATVPowerOn(void)
{
	UINT32 i,j;
	RDA5880_ERR_MSG err_msg  = RDA5880_OK;

	RDATVM_REG const tvpwon[]=
	{
		//power on
		{0xff,0x0000,0}, 
		{0xe2,0x0002,0}, 
		{0xe2,0x003a,0}, 
		{0xe4,0xae6c,1}, 
		{0xe4,0xaeec,1}, 
#if 1
		{0xe2,0x001a,0}, 
		{0xe2,0x003a,0}, 
         
		{0x30,0x811c,0}, 
		//{0x30,0x811d,0}, 
		{0xff,0x0000,0}, 
		{0x14,0x014b,0}, 
		{0x4a,0xff2e,0}, 
		{0x4c,0x007e,1}, 
		{0x4c,0x00fe,0}, 
             
		//{0xff,0x0001,0}, 
		//{0x00,0x0000,1}, 
		//{0x00,0x0001,1}, 

		//{0xff,0x0002,0}, 
		//{0x00,0x0000,1},
		//{0x00,0x0001,0}, 

		//ana init
		{0xff,0x0000,0}, 
		{0x12,0x8244,0}, 
		{0x14,0x134b,0}, 
		{0x1a,0x883f,0}, 
		{0x22,0x0326,0}, 
		{0x28,0xff98,0}, 
		{0x2c,0x0490,0}, 
		{0x2e,0x8888,0}, 
		{0x40,0x8188,0}, 
		{0x42,0x2802,0}, 
		{0x44,0x60b8,0}, 
		{0x60,0x0891,0}, 
		{0x66,0x0360,0}, 
		{0x24,0x4023,0}, //{0x24,0x8023,0}, 
		{0x24,0x6023,0}, //{0x24,0xA023,0}, 
		{0x3c,0x0300,0}, 
		{0x3e,0x0f0f,0}, 
		{0x50,0x0011,0}, 
		{0x52,0x0000,0}, 
		//dsp init     
		{0xff,0x0002,0}, 
		{0x28,0x01c0,0}, 
		{0x56,0x08fd,0},

		//im    
		{0xff,0x0002,0}, 
		{0x1c,0x0040,0}, 
		{0x1e,0x01ff,0}, 
		{0x20,0xc0cc,0}, 
		{0x22,0x0303,0}, 

		//RDA_PDX_AGC             
		{0xff,0x0000,0}, 
		{0x26,0x4a10,0}, 
             
		{0xff,0x0001,0}, 
		{0x20,0x0018,0}, 
		{0x60,0x0018,0}, 
		{0x8A,0x64c8,0}, 
		{0xac,0x07b6,0}, 
		{0x94,0x0282,0}, 
             
		{0x2c,0x249f,0}, 
		{0x26,0x20ff,0}, 
		{0x28,0x41ff,0}, 
		{0x2e,0x118f,0}, 
		{0x34,0x8fb0,0}, 
		{0x36,0x0602,0}, 
             
		{0x62,0x02ff,0}, 
		{0x64,0x8800,0}, //{0x64,0x8000,0}, 
		{0x66,0x2200,0}, //{0x66,0x1800,0}, 
             
		{0x84,0x1000,0}, //{0x84,0x2ad0,0}, 
		{0x86,0x0218,0}, 
		{0x88,0x60ff,0}, 
		{0x8a,0x67af,0}, 
		{0x8e,0x314e,0}, 
		{0x90,0x0c50,0}, //{0x90,0x1400,0}, 
		{0x92,0xfff0,0}, 
		{0x94,0x0802,0}, 
		{0x96,0x2705,0}, 
		{0xba,0x1d09,0}, 
		{0xbc,0x1c08,0},
		{0x70,0x8c36,0}, 
		{0x72,0x0060,0},
		//{0xac,0x0766,0},  //if power=-5dBm
		{0xac,0x0790,0},  //if power=0dBm
		{0xa2,0x0214,0}, 
		{0xa4,0x14ff,0}, 
		{0xa6,0x280a,0}, 
		{0xa8,0x0300,0}, 
		{0xae,0x45cf,0}, 
		{0xb0,0xcc50,0}, 
             
		{0xff,0x0000,0}, 
		{0x1C,0x8C8f,0}, 
		{0x1C,0x8E8f,0}, 
             
		{0xff,0x0001,0}, 
		{0x9C,0x2000,0}, 
		{0x9C,0x1000,0}, 
             
		{0xff,0x0002,0}, 
		{0x06,0x001B,0}, //iq swap=1, Equal to 5880p
		{0x06,0x0017,0}, 
		//{0x06,0x000B,0},  //iq swap=0
		//{0x06,0x0007,0}, 
		//RDA_PDX_DC_CAL             
		{0xff,0x0002,0}, 
		{0x0c,0x24cc,0}, 
		{0x0e,0x40c0,0}, 
		{0x10,0x0880,0}, 
		{0x36,0x0c80,0}, 
		{0x38,0x8c80,0}, 
		{0x3c,0x9004,0},
#endif
	};

	j = sizeof(tvpwon)/sizeof(RDATVM_REG);
	
	for(i=0;i<j;i++)
	{
		err_msg = RDATVM_WRITE(tvpwon[i].add,tvpwon[i].data);
		if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;
		RDADelayMs(tvpwon[i].delay);
	}
	
	err_msg = RDATVRfBp(0);
	err_msg |= RDATVdelIM(1);
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;

	return RDA5880_OK ;
}


RDA5880_ERR_MSG RDATVSet51Sta(UINT8 Status)
{
	RDA5880_ERR_MSG err_msg = RDA5880_OK;

	if(Status==0)
	{
		err_msg = RDATVM_WRITE(0xff,0x0000);  
		err_msg |= RDATVM_WRITE(0x30,0x011C);
		err_msg |= RDATVM_WRITE(0x30,0x811C);
		err_msg |= RDATVM_WRITE(0xff,0x0001);  
		err_msg |= RDATVM_WRITE(0x3a,0x0000);//  release lna
		err_msg |= RDATVM_WRITE(0x94,0x0802);//release 
		if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;
		RDADelayMs(2);
	}
	else
	{
		err_msg = RDATVM_WRITE(0xff,0x0000);  
		err_msg |= RDATVM_WRITE(0x30,0x011C);
		err_msg |= RDATVM_WRITE(0x30,0x011D);
		if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;
		RDADelayMs(2);
	}
	
	return RDA5880_OK ;
}


RDA5880_ERR_MSG RDATVGetI2VTH( RDA5880_PARAM *rf_param ) 
{    
	RDA5880_ERR_MSG err_msg = RDA5880_OK;
	UINT16 temp = 0;
    
	err_msg = RDATVSetFreq(666000,8000, rf_param);
	err_msg |= RDATVM_WRITE(0xff,0x0000);  
	err_msg |= RDATVM_WRITE(0x4A,0xbf2e); 
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;
           
	RDADelayMs(50);
	err_msg |= RDATVM_WRITE(0xff,0x0001);  
	err_msg |= RDATVM_READ(0x6A,&temp);

	rf_param->I2VFLOOR=((temp>>4)&0x0fff);
	rf_param->I2VLTH=rf_param->I2VFLOOR+0X0180;
	rf_param->I2VLTH=(((rf_param->I2VLTH)<<4)&0xfff0);    
	rf_param->I2VHTH=rf_param->I2VLTH*4;
	err_msg |= RDATVM_WRITE(0x66,rf_param->I2VLTH);
	err_msg |= RDATVM_WRITE(0x64,rf_param->I2VHTH);
	err_msg |= RDATVM_WRITE(0xff,0x0000);  
	err_msg |= RDATVM_WRITE(0x4A,0xff2e); 
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;
 
	return RDA5880_OK ;
}


RDATV_ADC_INDEX GetADCPatchByFre(UINT32 freq)  
{
	UINT8 i;

	PATCH_ADC_BY_FRE Patch_ADC_Tbl[]=
	{
		{52500 ,2000,ADC_172},
		{80000 ,2000,ADC_172},
		{68500 ,2000,ADC_151},
	};

	for(i=0;i<(sizeof(Patch_ADC_Tbl)/sizeof(PATCH_ADC_BY_FRE));i++)
	{	
	    if((freq>=(Patch_ADC_Tbl[i].Fre-Patch_ADC_Tbl[i].Delta))&&(freq<=(Patch_ADC_Tbl[i].Fre+Patch_ADC_Tbl[i].Delta)))
				return Patch_ADC_Tbl[i].Adc;
	}
 	
  return ADC_162;
}


RDA5880_ERR_MSG RDATVDownLoad(void)
{
	RDA5880_ERR_MSG err_msg = RDA5880_OK;

	err_msg = RDATVM_WRITE(0xff,0x0000);
	err_msg |= RDATVM_WRITE(0x30,0x011E);
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;
	RDADelayMs(100);

	err_msg |= RDATVM_WRITE(0xFE,0x0204);
	err_msg |= RDATVM_WRITE(0xFE,0x50E4);
	err_msg |= RDATVM_WRITE(0xFE,0xF50C);
	err_msg |= RDATVM_WRITE(0xFE,0xF50D);
	err_msg |= RDATVM_WRITE(0xFE,0xF50E);
	err_msg |= RDATVM_WRITE(0xFE,0xF50F);
	err_msg |= RDATVM_WRITE(0xFE,0xF510);
	err_msg |= RDATVM_WRITE(0xFE,0x7511);
	err_msg |= RDATVM_WRITE(0xFE,0x19F5);
	err_msg |= RDATVM_WRITE(0xFE,0x1275);
	err_msg |= RDATVM_WRITE(0xFE,0x1305);
	err_msg |= RDATVM_WRITE(0xFE,0xF514);
	err_msg |= RDATVM_WRITE(0xFE,0xF515);
	err_msg |= RDATVM_WRITE(0xFE,0x7516);
	err_msg |= RDATVM_WRITE(0xFE,0x0775);
	err_msg |= RDATVM_WRITE(0xFE,0x17D0);
	err_msg |= RDATVM_WRITE(0xFE,0x1204);
	err_msg |= RDATVM_WRITE(0xFE,0xDC8F);
	err_msg |= RDATVM_WRITE(0xFE,0x0985);
	err_msg |= RDATVM_WRITE(0xFE,0x201F);
	err_msg |= RDATVM_WRITE(0xFE,0x8521);
	err_msg |= RDATVM_WRITE(0xFE,0x2085);
	err_msg |= RDATVM_WRITE(0xFE,0x2221);
	err_msg |= RDATVM_WRITE(0xFE,0xE509);
	err_msg |= RDATVM_WRITE(0xFE,0x1313);
	err_msg |= RDATVM_WRITE(0xFE,0x543F);
	err_msg |= RDATVM_WRITE(0xFE,0xF522);
	err_msg |= RDATVM_WRITE(0xFE,0xE51F);
	err_msg |= RDATVM_WRITE(0xFE,0x2520);
	err_msg |= RDATVM_WRITE(0xFE,0xF509);
	err_msg |= RDATVM_WRITE(0xFE,0x2521);
	err_msg |= RDATVM_WRITE(0xFE,0xF509);
	err_msg |= RDATVM_WRITE(0xFE,0x2522);
	err_msg |= RDATVM_WRITE(0xFE,0xF509);
	err_msg |= RDATVM_WRITE(0xFE,0xFB7D);
	err_msg |= RDATVM_WRITE(0xFE,0xBF7F);
	err_msg |= RDATVM_WRITE(0xFE,0x01B1);
	err_msg |= RDATVM_WRITE(0xFE,0x82E5);
	err_msg |= RDATVM_WRITE(0xFE,0x09C3);
	err_msg |= RDATVM_WRITE(0xFE,0x9525);
	err_msg |= RDATVM_WRITE(0xFE,0x400D);
	err_msg |= RDATVM_WRITE(0xFE,0xE509);
	err_msg |= RDATVM_WRITE(0xFE,0xD395);
	err_msg |= RDATVM_WRITE(0xFE,0x2950);
	err_msg |= RDATVM_WRITE(0xFE,0x067E);
	err_msg |= RDATVM_WRITE(0xFE,0x007F);
	err_msg |= RDATVM_WRITE(0xFE,0x0180);
	err_msg |= RDATVM_WRITE(0xFE,0x047E);
	err_msg |= RDATVM_WRITE(0xFE,0x007F);
	err_msg |= RDATVM_WRITE(0xFE,0x008E);
	err_msg |= RDATVM_WRITE(0xFE,0x148F);
	err_msg |= RDATVM_WRITE(0xFE,0x15E5);
	err_msg |= RDATVM_WRITE(0xFE,0x1545);
	err_msg |= RDATVM_WRITE(0xFE,0x1460);
	err_msg |= RDATVM_WRITE(0xFE,0x087B);
	err_msg |= RDATVM_WRITE(0xFE,0x307D);
	err_msg |= RDATVM_WRITE(0xFE,0xA17F);
	err_msg |= RDATVM_WRITE(0xFE,0x01A1);
	err_msg |= RDATVM_WRITE(0xFE,0x82E5);
	err_msg |= RDATVM_WRITE(0xFE,0x09C3);
	err_msg |= RDATVM_WRITE(0xFE,0x9525);
	err_msg |= RDATVM_WRITE(0xFE,0x4002);
	err_msg |= RDATVM_WRITE(0xFE,0x213B);
	err_msg |= RDATVM_WRITE(0xFE,0xE509);
	err_msg |= RDATVM_WRITE(0xFE,0xC395);
	err_msg |= RDATVM_WRITE(0xFE,0x2840);
	err_msg |= RDATVM_WRITE(0xFE,0x0241);
	err_msg |= RDATVM_WRITE(0xFE,0x007B);
	err_msg |= RDATVM_WRITE(0xFE,0x317D);
	err_msg |= RDATVM_WRITE(0xFE,0xA17F);
	err_msg |= RDATVM_WRITE(0xFE,0x01B1);
	err_msg |= RDATVM_WRITE(0xFE,0x82E5);
	err_msg |= RDATVM_WRITE(0xFE,0x09D3);
	err_msg |= RDATVM_WRITE(0xFE,0x9525);
	err_msg |= RDATVM_WRITE(0xFE,0x4008);
	err_msg |= RDATVM_WRITE(0xFE,0x750E);
	err_msg |= RDATVM_WRITE(0xFE,0x0075);
	err_msg |= RDATVM_WRITE(0xFE,0x0F05);
	err_msg |= RDATVM_WRITE(0xFE,0x8006);
	err_msg |= RDATVM_WRITE(0xFE,0x750E);
	err_msg |= RDATVM_WRITE(0xFE,0x0075);
	err_msg |= RDATVM_WRITE(0xFE,0x0F19);
	err_msg |= RDATVM_WRITE(0xFE,0x7D3B);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB194);
	err_msg |= RDATVM_WRITE(0xFE,0x8F0A);
	err_msg |= RDATVM_WRITE(0xFE,0x7D3A);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB194);
	err_msg |= RDATVM_WRITE(0xFE,0x8F0B);
	err_msg |= RDATVM_WRITE(0xFE,0xE50A);
	err_msg |= RDATVM_WRITE(0xFE,0x5401);
	err_msg |= RDATVM_WRITE(0xFE,0xFE7C);
	err_msg |= RDATVM_WRITE(0xFE,0x00E4);
	err_msg |= RDATVM_WRITE(0xFE,0x250B);
	err_msg |= RDATVM_WRITE(0xFE,0xF50D);
	err_msg |= RDATVM_WRITE(0xFE,0xEC3E);
	err_msg |= RDATVM_WRITE(0xFE,0xF50C);
	err_msg |= RDATVM_WRITE(0xFE,0xE50D);
	err_msg |= RDATVM_WRITE(0xFE,0xF470);
	err_msg |= RDATVM_WRITE(0xFE,0x04E5);
	err_msg |= RDATVM_WRITE(0xFE,0x0C64);
	err_msg |= RDATVM_WRITE(0xFE,0x0170);
	err_msg |= RDATVM_WRITE(0xFE,0x0241);
	err_msg |= RDATVM_WRITE(0xFE,0x007B);
	err_msg |= RDATVM_WRITE(0xFE,0x327D);
	err_msg |= RDATVM_WRITE(0xFE,0xA17F);
	err_msg |= RDATVM_WRITE(0xFE,0x01B1);
	err_msg |= RDATVM_WRITE(0xFE,0x82E5);
	err_msg |= RDATVM_WRITE(0xFE,0x0F25);
	err_msg |= RDATVM_WRITE(0xFE,0x0DF5);
	err_msg |= RDATVM_WRITE(0xFE,0x0DE5);
	err_msg |= RDATVM_WRITE(0xFE,0x0E35);
	err_msg |= RDATVM_WRITE(0xFE,0x0CF5);
	err_msg |= RDATVM_WRITE(0xFE,0x0CC3);
	err_msg |= RDATVM_WRITE(0xFE,0xE50D);
	err_msg |= RDATVM_WRITE(0xFE,0x94FF);
	err_msg |= RDATVM_WRITE(0xFE,0xE50C);
	err_msg |= RDATVM_WRITE(0xFE,0x6480);
	err_msg |= RDATVM_WRITE(0xFE,0x9481);
	err_msg |= RDATVM_WRITE(0xFE,0x4014);
	err_msg |= RDATVM_WRITE(0xFE,0x430A);
	err_msg |= RDATVM_WRITE(0xFE,0x0375);
	err_msg |= RDATVM_WRITE(0xFE,0x0BFF);
	err_msg |= RDATVM_WRITE(0xFE,0xAB0A);
	err_msg |= RDATVM_WRITE(0xFE,0x7D3B);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB182);
	err_msg |= RDATVM_WRITE(0xFE,0xAB0B);
	err_msg |= RDATVM_WRITE(0xFE,0x7D3A);
	err_msg |= RDATVM_WRITE(0xFE,0x8022);
	err_msg |= RDATVM_WRITE(0xFE,0x7B33);
	err_msg |= RDATVM_WRITE(0xFE,0x7DA1);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB182);
	err_msg |= RDATVM_WRITE(0xFE,0xE50D);
	err_msg |= RDATVM_WRITE(0xFE,0xF50B);
	err_msg |= RDATVM_WRITE(0xFE,0xE50C);
	err_msg |= RDATVM_WRITE(0xFE,0xFFE5);
	err_msg |= RDATVM_WRITE(0xFE,0x0A54);
	err_msg |= RDATVM_WRITE(0xFE,0xFE2F);
	err_msg |= RDATVM_WRITE(0xFE,0xF50A);
	err_msg |= RDATVM_WRITE(0xFE,0xAB0B);
	err_msg |= RDATVM_WRITE(0xFE,0x7D3A);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB182);
	err_msg |= RDATVM_WRITE(0xFE,0xAB0A);
	err_msg |= RDATVM_WRITE(0xFE,0x7D3B);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB182);
	err_msg |= RDATVM_WRITE(0xFE,0x1204);
	err_msg |= RDATVM_WRITE(0xFE,0xDC8F);
	err_msg |= RDATVM_WRITE(0xFE,0x09AB);
	err_msg |= RDATVM_WRITE(0xFE,0x097D);
	err_msg |= RDATVM_WRITE(0xFE,0xBE7F);
	err_msg |= RDATVM_WRITE(0xFE,0x01B1);
	err_msg |= RDATVM_WRITE(0xFE,0x8201);
	err_msg |= RDATVM_WRITE(0xFE,0x7EE5);
	err_msg |= RDATVM_WRITE(0xFE,0x09D3);
	err_msg |= RDATVM_WRITE(0xFE,0x9529);
	err_msg |= RDATVM_WRITE(0xFE,0x5002);
	err_msg |= RDATVM_WRITE(0xFE,0x4100);
	err_msg |= RDATVM_WRITE(0xFE,0xE509);
	err_msg |= RDATVM_WRITE(0xFE,0xD395);
	err_msg |= RDATVM_WRITE(0xFE,0x2850);
	err_msg |= RDATVM_WRITE(0xFE,0x0241);
	err_msg |= RDATVM_WRITE(0xFE,0x007B);
	err_msg |= RDATVM_WRITE(0xFE,0x347D);
	err_msg |= RDATVM_WRITE(0xFE,0xA17F);
	err_msg |= RDATVM_WRITE(0xFE,0x01B1);
	err_msg |= RDATVM_WRITE(0xFE,0x82E5);
	err_msg |= RDATVM_WRITE(0xFE,0x09C3);
	err_msg |= RDATVM_WRITE(0xFE,0x9529);
	err_msg |= RDATVM_WRITE(0xFE,0x5008);
	err_msg |= RDATVM_WRITE(0xFE,0x8512);
	err_msg |= RDATVM_WRITE(0xFE,0x0E85);
	err_msg |= RDATVM_WRITE(0xFE,0x130F);
	err_msg |= RDATVM_WRITE(0xFE,0x8006);
	err_msg |= RDATVM_WRITE(0xFE,0x8510);
	err_msg |= RDATVM_WRITE(0xFE,0x0E85);
	err_msg |= RDATVM_WRITE(0xFE,0x110F);
	err_msg |= RDATVM_WRITE(0xFE,0x7D3B);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB194);
	err_msg |= RDATVM_WRITE(0xFE,0x8F0A);
	err_msg |= RDATVM_WRITE(0xFE,0x7D3A);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB194);
	err_msg |= RDATVM_WRITE(0xFE,0x8F0B);
	err_msg |= RDATVM_WRITE(0xFE,0xE50A);
	err_msg |= RDATVM_WRITE(0xFE,0x5401);
	err_msg |= RDATVM_WRITE(0xFE,0xFE7C);
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;
	err_msg |= RDATVM_WRITE(0xFE,0x00E4);
	err_msg |= RDATVM_WRITE(0xFE,0x250B);
	err_msg |= RDATVM_WRITE(0xFE,0xF50D);
	err_msg |= RDATVM_WRITE(0xFE,0xEC3E);
	err_msg |= RDATVM_WRITE(0xFE,0xF50C);
	err_msg |= RDATVM_WRITE(0xFE,0x450D);
	err_msg |= RDATVM_WRITE(0xFE,0x6072);
	err_msg |= RDATVM_WRITE(0xFE,0x7B35);
	err_msg |= RDATVM_WRITE(0xFE,0x7DA1);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB182);
	err_msg |= RDATVM_WRITE(0xFE,0xC3E5);
	err_msg |= RDATVM_WRITE(0xFE,0x0D95);
	err_msg |= RDATVM_WRITE(0xFE,0x0FF5);
	err_msg |= RDATVM_WRITE(0xFE,0x0DE5);
	err_msg |= RDATVM_WRITE(0xFE,0x0C95);
	err_msg |= RDATVM_WRITE(0xFE,0x0EF5);
	err_msg |= RDATVM_WRITE(0xFE,0x0CD3);
	err_msg |= RDATVM_WRITE(0xFE,0xE50D);
	err_msg |= RDATVM_WRITE(0xFE,0x9400);
	err_msg |= RDATVM_WRITE(0xFE,0xE50C);
	err_msg |= RDATVM_WRITE(0xFE,0x6480);
	err_msg |= RDATVM_WRITE(0xFE,0x9480);
	err_msg |= RDATVM_WRITE(0xFE,0x501C);
	err_msg |= RDATVM_WRITE(0xFE,0x7B36);
	err_msg |= RDATVM_WRITE(0xFE,0x7DA1);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB182);
	err_msg |= RDATVM_WRITE(0xFE,0x530A);
	err_msg |= RDATVM_WRITE(0xFE,0xFEE4);
	err_msg |= RDATVM_WRITE(0xFE,0xF50B);
	err_msg |= RDATVM_WRITE(0xFE,0xAB0A);
	err_msg |= RDATVM_WRITE(0xFE,0x7D3B);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB182);
	err_msg |= RDATVM_WRITE(0xFE,0xAB0B);
	err_msg |= RDATVM_WRITE(0xFE,0x7D3A);
	err_msg |= RDATVM_WRITE(0xFE,0x8022);
	err_msg |= RDATVM_WRITE(0xFE,0x7B37);
	err_msg |= RDATVM_WRITE(0xFE,0x7DA1);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB182);
	err_msg |= RDATVM_WRITE(0xFE,0xE50D);
	err_msg |= RDATVM_WRITE(0xFE,0xF50B);
	err_msg |= RDATVM_WRITE(0xFE,0xE50C);
	err_msg |= RDATVM_WRITE(0xFE,0xFFE5);
	err_msg |= RDATVM_WRITE(0xFE,0x0A54);
	err_msg |= RDATVM_WRITE(0xFE,0xFE2F);
	err_msg |= RDATVM_WRITE(0xFE,0xF50A);
	err_msg |= RDATVM_WRITE(0xFE,0xAB0B);
	err_msg |= RDATVM_WRITE(0xFE,0x7D3A);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB182);
	err_msg |= RDATVM_WRITE(0xFE,0xAB0A);
	err_msg |= RDATVM_WRITE(0xFE,0x7D3B);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB182);
	err_msg |= RDATVM_WRITE(0xFE,0x91DC);
	err_msg |= RDATVM_WRITE(0xFE,0x8F09);
	err_msg |= RDATVM_WRITE(0xFE,0xAB09);
	err_msg |= RDATVM_WRITE(0xFE,0x7DBE);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB182);
	err_msg |= RDATVM_WRITE(0xFE,0x2144);
	err_msg |= RDATVM_WRITE(0xFE,0x22E4);
	err_msg |= RDATVM_WRITE(0xFE,0xF50B);
	err_msg |= RDATVM_WRITE(0xFE,0xF50D);
	err_msg |= RDATVM_WRITE(0xFE,0xF50E);
	err_msg |= RDATVM_WRITE(0xFE,0x7D96);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB194);
	err_msg |= RDATVM_WRITE(0xFE,0x8F0A);
	err_msg |= RDATVM_WRITE(0xFE,0x7F02);
	err_msg |= RDATVM_WRITE(0xFE,0xB16B);
	err_msg |= RDATVM_WRITE(0xFE,0x750D);
	err_msg |= RDATVM_WRITE(0xFE,0x0075);
	err_msg |= RDATVM_WRITE(0xFE,0x0E01);
	err_msg |= RDATVM_WRITE(0xFE,0x7D94);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB194);
	err_msg |= RDATVM_WRITE(0xFE,0x8F0C);
	err_msg |= RDATVM_WRITE(0xFE,0x430C);
	err_msg |= RDATVM_WRITE(0xFE,0x08AB);
	err_msg |= RDATVM_WRITE(0xFE,0x0C7D);
	err_msg |= RDATVM_WRITE(0xFE,0x947F);
	err_msg |= RDATVM_WRITE(0xFE,0x01B1);
	err_msg |= RDATVM_WRITE(0xFE,0x827B);
	err_msg |= RDATVM_WRITE(0xFE,0x407D);
	err_msg |= RDATVM_WRITE(0xFE,0xA17F);
	err_msg |= RDATVM_WRITE(0xFE,0x01B1);
	err_msg |= RDATVM_WRITE(0xFE,0x82E5);
	err_msg |= RDATVM_WRITE(0xFE,0x09C3);
	err_msg |= RDATVM_WRITE(0xFE,0x9410);
	err_msg |= RDATVM_WRITE(0xFE,0x4005);
	err_msg |= RDATVM_WRITE(0xFE,0x750B);
	err_msg |= RDATVM_WRITE(0xFE,0x0180);
	err_msg |= RDATVM_WRITE(0xFE,0x0375);
	err_msg |= RDATVM_WRITE(0xFE,0x0B02);
	err_msg |= RDATVM_WRITE(0xFE,0x7B01);
	err_msg |= RDATVM_WRITE(0xFE,0x7A00);
	err_msg |= RDATVM_WRITE(0xFE,0x7D01);
	err_msg |= RDATVM_WRITE(0xFE,0x7F03);
	err_msg |= RDATVM_WRITE(0xFE,0x91E4);
	err_msg |= RDATVM_WRITE(0xFE,0x8F09);
	err_msg |= RDATVM_WRITE(0xFE,0xE4F5);
	err_msg |= RDATVM_WRITE(0xFE,0x0DF5);
	err_msg |= RDATVM_WRITE(0xFE,0x0EE5);
	err_msg |= RDATVM_WRITE(0xFE,0x0B64);
	err_msg |= RDATVM_WRITE(0xFE,0x0170);
	err_msg |= RDATVM_WRITE(0xFE,0x26E5);
	err_msg |= RDATVM_WRITE(0xFE,0x09D3);
	err_msg |= RDATVM_WRITE(0xFE,0x9526);
	err_msg |= RDATVM_WRITE(0xFE,0x500D);
	err_msg |= RDATVM_WRITE(0xFE,0xE509);
	err_msg |= RDATVM_WRITE(0xFE,0xC395);
	err_msg |= RDATVM_WRITE(0xFE,0x2340);
	err_msg |= RDATVM_WRITE(0xFE,0x067E);
	err_msg |= RDATVM_WRITE(0xFE,0x007F);
	err_msg |= RDATVM_WRITE(0xFE,0x0180);
	err_msg |= RDATVM_WRITE(0xFE,0x047E);
	err_msg |= RDATVM_WRITE(0xFE,0x007F);
	err_msg |= RDATVM_WRITE(0xFE,0x008E);
	err_msg |= RDATVM_WRITE(0xFE,0x0D8F);
	err_msg |= RDATVM_WRITE(0xFE,0x0EE5);
	err_msg |= RDATVM_WRITE(0xFE,0x0E45);
	err_msg |= RDATVM_WRITE(0xFE,0x0D60);
	err_msg |= RDATVM_WRITE(0xFE,0x2A7B);
	err_msg |= RDATVM_WRITE(0xFE,0x4180);
	err_msg |= RDATVM_WRITE(0xFE,0x5FE5);
	err_msg |= RDATVM_WRITE(0xFE,0x09D3);
	err_msg |= RDATVM_WRITE(0xFE,0x9527);
	err_msg |= RDATVM_WRITE(0xFE,0x500D);
	err_msg |= RDATVM_WRITE(0xFE,0xE509);
	err_msg |= RDATVM_WRITE(0xFE,0xC395);
	err_msg |= RDATVM_WRITE(0xFE,0x2440);
	err_msg |= RDATVM_WRITE(0xFE,0x067E);
	err_msg |= RDATVM_WRITE(0xFE,0x007F);
	err_msg |= RDATVM_WRITE(0xFE,0x0180);
	err_msg |= RDATVM_WRITE(0xFE,0x047E);
	err_msg |= RDATVM_WRITE(0xFE,0x007F);
	err_msg |= RDATVM_WRITE(0xFE,0x008E);
	err_msg |= RDATVM_WRITE(0xFE,0x0D8F);
	err_msg |= RDATVM_WRITE(0xFE,0x0EE5);
	err_msg |= RDATVM_WRITE(0xFE,0x0E45);
	err_msg |= RDATVM_WRITE(0xFE,0x0D60);
	err_msg |= RDATVM_WRITE(0xFE,0x047B);
	err_msg |= RDATVM_WRITE(0xFE,0x4280);
	err_msg |= RDATVM_WRITE(0xFE,0x397B);
	err_msg |= RDATVM_WRITE(0xFE,0x027D);
	err_msg |= RDATVM_WRITE(0xFE,0x917F);
	err_msg |= RDATVM_WRITE(0xFE,0x01B1);
	err_msg |= RDATVM_WRITE(0xFE,0x82E4);
	err_msg |= RDATVM_WRITE(0xFE,0xFB7D);
	err_msg |= RDATVM_WRITE(0xFE,0x907F);
	err_msg |= RDATVM_WRITE(0xFE,0x01B1);
	err_msg |= RDATVM_WRITE(0xFE,0x827D);
	err_msg |= RDATVM_WRITE(0xFE,0x947F);
	err_msg |= RDATVM_WRITE(0xFE,0x01B1);
	err_msg |= RDATVM_WRITE(0xFE,0x948F);
	err_msg |= RDATVM_WRITE(0xFE,0x0C53);
	err_msg |= RDATVM_WRITE(0xFE,0x0CF7);
	err_msg |= RDATVM_WRITE(0xFE,0xAB0C);
	err_msg |= RDATVM_WRITE(0xFE,0x7D94);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB182);
	err_msg |= RDATVM_WRITE(0xFE,0x7F02);
	err_msg |= RDATVM_WRITE(0xFE,0xB16B);
	err_msg |= RDATVM_WRITE(0xFE,0x7B0C);
	err_msg |= RDATVM_WRITE(0xFE,0x7D91);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB182);
	err_msg |= RDATVM_WRITE(0xFE,0x7B50);
	err_msg |= RDATVM_WRITE(0xFE,0x7D90);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB182);
	err_msg |= RDATVM_WRITE(0xFE,0x7B43);
	err_msg |= RDATVM_WRITE(0xFE,0x7DA1);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB182);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xA16B);
	err_msg |= RDATVM_WRITE(0xFE,0xE4FF);
	err_msg |= RDATVM_WRITE(0xFE,0xF509);
	err_msg |= RDATVM_WRITE(0xFE,0xF50A);
	err_msg |= RDATVM_WRITE(0xFE,0xF50B);
	err_msg |= RDATVM_WRITE(0xFE,0x7D3B);
	err_msg |= RDATVM_WRITE(0xFE,0x0FB1);
	err_msg |= RDATVM_WRITE(0xFE,0x94EF);
	err_msg |= RDATVM_WRITE(0xFE,0x5401);
	err_msg |= RDATVM_WRITE(0xFE,0xF509);
	err_msg |= RDATVM_WRITE(0xFE,0x7D3A);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB194);
	err_msg |= RDATVM_WRITE(0xFE,0xBFFF);
	err_msg |= RDATVM_WRITE(0xFE,0x047E);
	err_msg |= RDATVM_WRITE(0xFE,0x0180);
	err_msg |= RDATVM_WRITE(0xFE,0x027E);
	err_msg |= RDATVM_WRITE(0xFE,0x00E5);
	err_msg |= RDATVM_WRITE(0xFE,0x09B4);
	err_msg |= RDATVM_WRITE(0xFE,0x0104);
	err_msg |= RDATVM_WRITE(0xFE,0x7D01);
	err_msg |= RDATVM_WRITE(0xFE,0x8002);
	err_msg |= RDATVM_WRITE(0xFE,0x7D00);
	err_msg |= RDATVM_WRITE(0xFE,0xED5E);
	err_msg |= RDATVM_WRITE(0xFE,0xF50A);
	err_msg |= RDATVM_WRITE(0xFE,0x7D68);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB194);
	err_msg |= RDATVM_WRITE(0xFE,0xEF13);
	err_msg |= RDATVM_WRITE(0xFE,0x1313);
	err_msg |= RDATVM_WRITE(0xFE,0x5403);
	err_msg |= RDATVM_WRITE(0xFE,0xFE25);
	err_msg |= RDATVM_WRITE(0xFE,0xE025);
	err_msg |= RDATVM_WRITE(0xFE,0xE0F5);
	err_msg |= RDATVM_WRITE(0xFE,0x0924);
	err_msg |= RDATVM_WRITE(0xFE,0x80F5);
	err_msg |= RDATVM_WRITE(0xFE,0x09BE);
	err_msg |= RDATVM_WRITE(0xFE,0x0304);
	err_msg |= RDATVM_WRITE(0xFE,0x7E01);
	err_msg |= RDATVM_WRITE(0xFE,0x8002);
	err_msg |= RDATVM_WRITE(0xFE,0x7E00);
	err_msg |= RDATVM_WRITE(0xFE,0xEE52);
	err_msg |= RDATVM_WRITE(0xFE,0x0A7D);
	err_msg |= RDATVM_WRITE(0xFE,0x997F);
	err_msg |= RDATVM_WRITE(0xFE,0x01B1);
	err_msg |= RDATVM_WRITE(0xFE,0x94EF);
	err_msg |= RDATVM_WRITE(0xFE,0xC413);
	err_msg |= RDATVM_WRITE(0xFE,0x5407);
	err_msg |= RDATVM_WRITE(0xFE,0xF50B);
	err_msg |= RDATVM_WRITE(0xFE,0x530B);
	err_msg |= RDATVM_WRITE(0xFE,0x03E5);
	err_msg |= RDATVM_WRITE(0xFE,0x0B33);
	err_msg |= RDATVM_WRITE(0xFE,0x3333);
	err_msg |= RDATVM_WRITE(0xFE,0x54F8);
	err_msg |= RDATVM_WRITE(0xFE,0xF50B);
	err_msg |= RDATVM_WRITE(0xFE,0xC3E5);
	err_msg |= RDATVM_WRITE(0xFE,0x0995);
	err_msg |= RDATVM_WRITE(0xFE,0x0BF5);
	err_msg |= RDATVM_WRITE(0xFE,0x097D);
	err_msg |= RDATVM_WRITE(0xFE,0x987F);
	err_msg |= RDATVM_WRITE(0xFE,0x01B1);
	err_msg |= RDATVM_WRITE(0xFE,0x948F);
	err_msg |= RDATVM_WRITE(0xFE,0x0B53);
	err_msg |= RDATVM_WRITE(0xFE,0x0B0F);
	err_msg |= RDATVM_WRITE(0xFE,0xC3E5);
	err_msg |= RDATVM_WRITE(0xFE,0x0995);
	err_msg |= RDATVM_WRITE(0xFE,0x0BF5);
	err_msg |= RDATVM_WRITE(0xFE,0x09E5);
	err_msg |= RDATVM_WRITE(0xFE,0x0A60);
	err_msg |= RDATVM_WRITE(0xFE,0x177D);
	err_msg |= RDATVM_WRITE(0xFE,0x1CE4);
	err_msg |= RDATVM_WRITE(0xFE,0xFFB1);
	err_msg |= RDATVM_WRITE(0xFE,0x948F);
	err_msg |= RDATVM_WRITE(0xFE,0x0B53);
	err_msg |= RDATVM_WRITE(0xFE,0x0BFD);
	err_msg |= RDATVM_WRITE(0xFE,0xAB0B);
	err_msg |= RDATVM_WRITE(0xFE,0x7D1C);
	err_msg |= RDATVM_WRITE(0xFE,0xE4FF);
	err_msg |= RDATVM_WRITE(0xFE,0xB182);
	err_msg |= RDATVM_WRITE(0xFE,0x7F0A);
	err_msg |= RDATVM_WRITE(0xFE,0xA16B);
	err_msg |= RDATVM_WRITE(0xFE,0xE509);
	err_msg |= RDATVM_WRITE(0xFE,0xC394);
	err_msg |= RDATVM_WRITE(0xFE,0x7450);
	err_msg |= RDATVM_WRITE(0xFE,0x137D);
	err_msg |= RDATVM_WRITE(0xFE,0x1CE4);
	err_msg |= RDATVM_WRITE(0xFE,0xFFB1);
	err_msg |= RDATVM_WRITE(0xFE,0x948F);
	err_msg |= RDATVM_WRITE(0xFE,0x0B43);
	err_msg |= RDATVM_WRITE(0xFE,0x0B02);
	err_msg |= RDATVM_WRITE(0xFE,0xAB0B);
	err_msg |= RDATVM_WRITE(0xFE,0x7D1C);
	err_msg |= RDATVM_WRITE(0xFE,0xE4FF);
	err_msg |= RDATVM_WRITE(0xFE,0xB182);
	err_msg |= RDATVM_WRITE(0xFE,0xE509);
	err_msg |= RDATVM_WRITE(0xFE,0xD394);
	err_msg |= RDATVM_WRITE(0xFE,0x8040);
	err_msg |= RDATVM_WRITE(0xFE,0x137D);
	err_msg |= RDATVM_WRITE(0xFE,0x1CE4);
	err_msg |= RDATVM_WRITE(0xFE,0xFFB1);
	err_msg |= RDATVM_WRITE(0xFE,0x948F);
	err_msg |= RDATVM_WRITE(0xFE,0x0B53);
	err_msg |= RDATVM_WRITE(0xFE,0x0BFD);
	err_msg |= RDATVM_WRITE(0xFE,0xAB0B);
	err_msg |= RDATVM_WRITE(0xFE,0x7D1C);
	err_msg |= RDATVM_WRITE(0xFE,0xE4FF);
	err_msg |= RDATVM_WRITE(0xFE,0xB182);
	err_msg |= RDATVM_WRITE(0xFE,0x22E4);
	err_msg |= RDATVM_WRITE(0xFE,0xF508);
	err_msg |= RDATVM_WRITE(0xFE,0x7B10);
	err_msg |= RDATVM_WRITE(0xFE,0x7DA1);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB182);
	err_msg |= RDATVM_WRITE(0xFE,0x7F02);
	err_msg |= RDATVM_WRITE(0xFE,0xB16B);
	err_msg |= RDATVM_WRITE(0xFE,0x7D3A);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB194);
	err_msg |= RDATVM_WRITE(0xFE,0x8F08);
	err_msg |= RDATVM_WRITE(0xFE,0x7D3B);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB194);
	err_msg |= RDATVM_WRITE(0xFE,0xAB07);
	err_msg |= RDATVM_WRITE(0xFE,0x4303);
	err_msg |= RDATVM_WRITE(0xFE,0x027D);
	err_msg |= RDATVM_WRITE(0xFE,0x3B7F);
	err_msg |= RDATVM_WRITE(0xFE,0x01B1);
	err_msg |= RDATVM_WRITE(0xFE,0x82AB);
	err_msg |= RDATVM_WRITE(0xFE,0x087D);
	err_msg |= RDATVM_WRITE(0xFE,0x3A7F);
	err_msg |= RDATVM_WRITE(0xFE,0x01B1);
	err_msg |= RDATVM_WRITE(0xFE,0x827D);
	err_msg |= RDATVM_WRITE(0xFE,0xBB7F);
	err_msg |= RDATVM_WRITE(0xFE,0x01B1);
	err_msg |= RDATVM_WRITE(0xFE,0x948F);
	err_msg |= RDATVM_WRITE(0xFE,0x267D);
	err_msg |= RDATVM_WRITE(0xFE,0xBA7F);
	err_msg |= RDATVM_WRITE(0xFE,0x01B1);
	err_msg |= RDATVM_WRITE(0xFE,0x948F);
	err_msg |= RDATVM_WRITE(0xFE,0x237D);
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;
	err_msg |= RDATVM_WRITE(0xFE,0xBD7F);
	err_msg |= RDATVM_WRITE(0xFE,0x01B1);
	err_msg |= RDATVM_WRITE(0xFE,0x948F);
	err_msg |= RDATVM_WRITE(0xFE,0x277D);
	err_msg |= RDATVM_WRITE(0xFE,0xBC7F);
	err_msg |= RDATVM_WRITE(0xFE,0x01B1);
	err_msg |= RDATVM_WRITE(0xFE,0x948F);
	err_msg |= RDATVM_WRITE(0xFE,0x247D);
	err_msg |= RDATVM_WRITE(0xFE,0x717F);
	err_msg |= RDATVM_WRITE(0xFE,0x01B1);
	err_msg |= RDATVM_WRITE(0xFE,0x948F);
	err_msg |= RDATVM_WRITE(0xFE,0x297D);
	err_msg |= RDATVM_WRITE(0xFE,0x707F);
	err_msg |= RDATVM_WRITE(0xFE,0x01B1);
	err_msg |= RDATVM_WRITE(0xFE,0x948F);
	err_msg |= RDATVM_WRITE(0xFE,0x257D);
	err_msg |= RDATVM_WRITE(0xFE,0x727F);
	err_msg |= RDATVM_WRITE(0xFE,0x01B1);
	err_msg |= RDATVM_WRITE(0xFE,0x948F);
	err_msg |= RDATVM_WRITE(0xFE,0x287B);
	err_msg |= RDATVM_WRITE(0xFE,0xD07A);
	err_msg |= RDATVM_WRITE(0xFE,0x0791);
	err_msg |= RDATVM_WRITE(0xFE,0xE0AB);
	err_msg |= RDATVM_WRITE(0xFE,0x07EB);
	err_msg |= RDATVM_WRITE(0xFE,0x1313);
	err_msg |= RDATVM_WRITE(0xFE,0x543F);
	err_msg |= RDATVM_WRITE(0xFE,0xF51F);
	err_msg |= RDATVM_WRITE(0xFE,0xF520);
	err_msg |= RDATVM_WRITE(0xFE,0xF521);
	err_msg |= RDATVM_WRITE(0xFE,0xF522);
	err_msg |= RDATVM_WRITE(0xFE,0x7B20);
	err_msg |= RDATVM_WRITE(0xFE,0x7DA1);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0xB182);
	err_msg |= RDATVM_WRITE(0xFE,0x7F02);
	err_msg |= RDATVM_WRITE(0xFE,0xB16B);
	err_msg |= RDATVM_WRITE(0xFE,0x51EA);
	err_msg |= RDATVM_WRITE(0xFE,0x1103);
	err_msg |= RDATVM_WRITE(0xFE,0x5101);
	err_msg |= RDATVM_WRITE(0xFE,0x80EC);
	err_msg |= RDATVM_WRITE(0xFE,0x787F);
	err_msg |= RDATVM_WRITE(0xFE,0xE4F6);
	err_msg |= RDATVM_WRITE(0xFE,0xD8FD);
	err_msg |= RDATVM_WRITE(0xFE,0x7581);
	err_msg |= RDATVM_WRITE(0xFE,0x2902);
	err_msg |= RDATVM_WRITE(0xFE,0x0497);
	err_msg |= RDATVM_WRITE(0xFE,0x0203);
	err_msg |= RDATVM_WRITE(0xFE,0xBFE4);
	err_msg |= RDATVM_WRITE(0xFE,0x93A3);
	err_msg |= RDATVM_WRITE(0xFE,0xF8E4);
	err_msg |= RDATVM_WRITE(0xFE,0x93A3);
	err_msg |= RDATVM_WRITE(0xFE,0x4003);
	err_msg |= RDATVM_WRITE(0xFE,0xF680);
	err_msg |= RDATVM_WRITE(0xFE,0x01F2);
	err_msg |= RDATVM_WRITE(0xFE,0x08DF);
	err_msg |= RDATVM_WRITE(0xFE,0xF480);
	err_msg |= RDATVM_WRITE(0xFE,0x29E4);
	err_msg |= RDATVM_WRITE(0xFE,0x93A3);
	err_msg |= RDATVM_WRITE(0xFE,0xF854);
	err_msg |= RDATVM_WRITE(0xFE,0x0724);
	err_msg |= RDATVM_WRITE(0xFE,0x0CC8);
	err_msg |= RDATVM_WRITE(0xFE,0xC333);
	err_msg |= RDATVM_WRITE(0xFE,0xC454);
	err_msg |= RDATVM_WRITE(0xFE,0x0F44);
	err_msg |= RDATVM_WRITE(0xFE,0x20C8);
	err_msg |= RDATVM_WRITE(0xFE,0x8340);
	err_msg |= RDATVM_WRITE(0xFE,0x04F4);
	err_msg |= RDATVM_WRITE(0xFE,0x5680);
	err_msg |= RDATVM_WRITE(0xFE,0x0146);
	err_msg |= RDATVM_WRITE(0xFE,0xF6DF);
	err_msg |= RDATVM_WRITE(0xFE,0xE480);
	err_msg |= RDATVM_WRITE(0xFE,0x0B01);
	err_msg |= RDATVM_WRITE(0xFE,0x0204);
	err_msg |= RDATVM_WRITE(0xFE,0x0810);
	err_msg |= RDATVM_WRITE(0xFE,0x2040);
	err_msg |= RDATVM_WRITE(0xFE,0x8090);
	err_msg |= RDATVM_WRITE(0xFE,0x0549);
	err_msg |= RDATVM_WRITE(0xFE,0xE47E);
	err_msg |= RDATVM_WRITE(0xFE,0x0193);
	err_msg |= RDATVM_WRITE(0xFE,0x60BC);
	err_msg |= RDATVM_WRITE(0xFE,0xA3FF);
	err_msg |= RDATVM_WRITE(0xFE,0x543F);
	err_msg |= RDATVM_WRITE(0xFE,0x30E5);
	err_msg |= RDATVM_WRITE(0xFE,0x0954);
	err_msg |= RDATVM_WRITE(0xFE,0x1FFE);
	err_msg |= RDATVM_WRITE(0xFE,0xE493);
	err_msg |= RDATVM_WRITE(0xFE,0xA360);
	err_msg |= RDATVM_WRITE(0xFE,0x010E);
	err_msg |= RDATVM_WRITE(0xFE,0xCF54);
	err_msg |= RDATVM_WRITE(0xFE,0xC025);
	err_msg |= RDATVM_WRITE(0xFE,0xE060);
	err_msg |= RDATVM_WRITE(0xFE,0xA840);
	err_msg |= RDATVM_WRITE(0xFE,0xB8E4);
	err_msg |= RDATVM_WRITE(0xFE,0x93A3);
	err_msg |= RDATVM_WRITE(0xFE,0xFAE4);
	err_msg |= RDATVM_WRITE(0xFE,0x93A3);
	err_msg |= RDATVM_WRITE(0xFE,0xF8E4);
	err_msg |= RDATVM_WRITE(0xFE,0x93A3);
	err_msg |= RDATVM_WRITE(0xFE,0xC8C5);
	err_msg |= RDATVM_WRITE(0xFE,0x82C8);
	err_msg |= RDATVM_WRITE(0xFE,0xCAC5);
	err_msg |= RDATVM_WRITE(0xFE,0x83CA);
	err_msg |= RDATVM_WRITE(0xFE,0xF0A3);
	err_msg |= RDATVM_WRITE(0xFE,0xC8C5);
	err_msg |= RDATVM_WRITE(0xFE,0x82C8);
	err_msg |= RDATVM_WRITE(0xFE,0xCAC5);
	err_msg |= RDATVM_WRITE(0xFE,0x83CA);
	err_msg |= RDATVM_WRITE(0xFE,0xDFE9);
	err_msg |= RDATVM_WRITE(0xFE,0xDEE7);
	err_msg |= RDATVM_WRITE(0xFE,0x80BE);
	err_msg |= RDATVM_WRITE(0xFE,0xAB17);
	err_msg |= RDATVM_WRITE(0xFE,0xAA16);
	err_msg |= RDATVM_WRITE(0xFE,0x7D01);
	err_msg |= RDATVM_WRITE(0xFE,0x7F01);
	err_msg |= RDATVM_WRITE(0xFE,0x8F18);
	err_msg |= RDATVM_WRITE(0xFE,0x8D19);
	err_msg |= RDATVM_WRITE(0xFE,0x8A1A);
	err_msg |= RDATVM_WRITE(0xFE,0x8B1B);
	err_msg |= RDATVM_WRITE(0xFE,0xE4FF);
	err_msg |= RDATVM_WRITE(0xFE,0xF51C);
	err_msg |= RDATVM_WRITE(0xFE,0xE518);
	err_msg |= RDATVM_WRITE(0xFE,0xB401);
	err_msg |= RDATVM_WRITE(0xFE,0x30E4);
	err_msg |= RDATVM_WRITE(0xFE,0xF51D);
	err_msg |= RDATVM_WRITE(0xFE,0xF51E);
	err_msg |= RDATVM_WRITE(0xFE,0xC3E5);
	err_msg |= RDATVM_WRITE(0xFE,0x1E95);
	err_msg |= RDATVM_WRITE(0xFE,0x1BE5);
	err_msg |= RDATVM_WRITE(0xFE,0x1A64);
	err_msg |= RDATVM_WRITE(0xFE,0x80F8);
	err_msg |= RDATVM_WRITE(0xFE,0xE51D);
	err_msg |= RDATVM_WRITE(0xFE,0x6480);
	err_msg |= RDATVM_WRITE(0xFE,0x9850);
	err_msg |= RDATVM_WRITE(0xFE,0x187D);
	err_msg |= RDATVM_WRITE(0xFE,0x2B7F);
	err_msg |= RDATVM_WRITE(0xFE,0x01B1);
	err_msg |= RDATVM_WRITE(0xFE,0x94EF);
	err_msg |= RDATVM_WRITE(0xFE,0xD395);
	err_msg |= RDATVM_WRITE(0xFE,0x1C40);
	err_msg |= RDATVM_WRITE(0xFE,0x028F);
	err_msg |= RDATVM_WRITE(0xFE,0x1C05);
	err_msg |= RDATVM_WRITE(0xFE,0x1EE5);
	err_msg |= RDATVM_WRITE(0xFE,0x1E70);
	err_msg |= RDATVM_WRITE(0xFE,0xDB05);
	err_msg |= RDATVM_WRITE(0xFE,0x1D80);
	err_msg |= RDATVM_WRITE(0xFE,0xD7AF);
	err_msg |= RDATVM_WRITE(0xFE,0x1CE5);
	err_msg |= RDATVM_WRITE(0xFE,0x18B4);
	err_msg |= RDATVM_WRITE(0xFE,0x031E);
	err_msg |= RDATVM_WRITE(0xFE,0xE4F5);
	err_msg |= RDATVM_WRITE(0xFE,0x1DF5);
	err_msg |= RDATVM_WRITE(0xFE,0x1E7D);
	err_msg |= RDATVM_WRITE(0xFE,0x8D7F);
	err_msg |= RDATVM_WRITE(0xFE,0x01B1);
	err_msg |= RDATVM_WRITE(0xFE,0x9405);
	err_msg |= RDATVM_WRITE(0xFE,0x1EE5);
	err_msg |= RDATVM_WRITE(0xFE,0x1E70);
	err_msg |= RDATVM_WRITE(0xFE,0x0205);
	err_msg |= RDATVM_WRITE(0xFE,0x1DC3);
	err_msg |= RDATVM_WRITE(0xFE,0x9401);
	err_msg |= RDATVM_WRITE(0xFE,0xE51D);
	err_msg |= RDATVM_WRITE(0xFE,0x6480);
	err_msg |= RDATVM_WRITE(0xFE,0x9480);
	err_msg |= RDATVM_WRITE(0xFE,0x40E7);
	err_msg |= RDATVM_WRITE(0xFE,0x2201);
	err_msg |= RDATVM_WRITE(0xFE,0x261D);
	err_msg |= RDATVM_WRITE(0xFE,0x0123);
	err_msg |= RDATVM_WRITE(0xFE,0x0001);
	err_msg |= RDATVM_WRITE(0xFE,0x2700);
	err_msg |= RDATVM_WRITE(0xFE,0x0124);
	err_msg |= RDATVM_WRITE(0xFE,0x0001);
	err_msg |= RDATVM_WRITE(0xFE,0x298C);
	err_msg |= RDATVM_WRITE(0xFE,0x0125);
	err_msg |= RDATVM_WRITE(0xFE,0x3601);
	err_msg |= RDATVM_WRITE(0xFE,0x2860);
	err_msg |= RDATVM_WRITE(0xFE,0x011F);
	err_msg |= RDATVM_WRITE(0xFE,0x0001);
	err_msg |= RDATVM_WRITE(0xFE,0x2000);
	err_msg |= RDATVM_WRITE(0xFE,0x0121);
	err_msg |= RDATVM_WRITE(0xFE,0x0001);
	err_msg |= RDATVM_WRITE(0xFE,0x2200);
	err_msg |= RDATVM_WRITE(0xFE,0x007D);
	err_msg |= RDATVM_WRITE(0xFE,0xFA7C);
	err_msg |= RDATVM_WRITE(0xFE,0x00AE);
	err_msg |= RDATVM_WRITE(0xFE,0x071F);
	err_msg |= RDATVM_WRITE(0xFE,0xEE60);
	err_msg |= RDATVM_WRITE(0xFE,0x0CED);
	err_msg |= RDATVM_WRITE(0xFE,0x1DAA);
	err_msg |= RDATVM_WRITE(0xFE,0x0470);
	err_msg |= RDATVM_WRITE(0xFE,0x011C);
	err_msg |= RDATVM_WRITE(0xFE,0x4A60);
	err_msg |= RDATVM_WRITE(0xFE,0xF080);
	err_msg |= RDATVM_WRITE(0xFE,0xF422);
	err_msg |= RDATVM_WRITE(0xFE,0x7590);
	err_msg |= RDATVM_WRITE(0xFE,0xFF8F);
	err_msg |= RDATVM_WRITE(0xFE,0x80C2);
	err_msg |= RDATVM_WRITE(0xFE,0xB5D2);
	err_msg |= RDATVM_WRITE(0xFE,0xB58D);
	err_msg |= RDATVM_WRITE(0xFE,0x908B);
	err_msg |= RDATVM_WRITE(0xFE,0x80C2);
	err_msg |= RDATVM_WRITE(0xFE,0xB5D2);
	err_msg |= RDATVM_WRITE(0xFE,0xB522);
	err_msg |= RDATVM_WRITE(0xFE,0x7590);
	err_msg |= RDATVM_WRITE(0xFE,0xFF8F);
	err_msg |= RDATVM_WRITE(0xFE,0x80C2);
	err_msg |= RDATVM_WRITE(0xFE,0xB5D2);
	err_msg |= RDATVM_WRITE(0xFE,0xB58D);
	err_msg |= RDATVM_WRITE(0xFE,0x90AF);
	err_msg |= RDATVM_WRITE(0xFE,0x9022);
	err_msg |= RDATVM_WRITE(0xFE,0x2200);
	err_msg |= RDATVM_WRITE(0xFE,0x0000);
	err_msg |= RDATVM_WRITE(0xFE,0x0000);
	err_msg |= RDATVM_WRITE(0xFE,0x0000);
	err_msg |= RDATVM_WRITE(0xFE,0x0000);
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;
	RDADelayMs(200);
	err_msg |= RDATVM_WRITE(0xff,0x011C);
	err_msg |= RDATVM_WRITE(0x30,0x811C);
	
	

	return err_msg ;
}

static UINT32 RDATVMulDivCompensate(UINT32 par1,UINT32 par2,UINT32 par3)
{
	UINT32 result=0;
	UINT32 div_int,div_remain,div_remain1;
	UINT32 mremain,mb,fremain;
	
	div_int = par1/par2;
	div_remain = par1%par2;

	div_int <<= par3;
	result += div_int;

	mremain = ((UINT32)1<<(32-par3))-1;//why?
	mb = div_remain/mremain;
	fremain = div_remain%mremain;

	div_int = (fremain<<par3)/par2;
	result += div_int;

	div_remain1 = (fremain<<par3)%par2;

	div_int = (mremain<<par3)/par2;
	div_int *= mb;
	result += div_int;

	div_remain = (mremain<<par3)%par2;
	div_remain *= mb;
	div_remain += div_remain1;
	
	div_int = div_remain/par2;
	result += div_int;

	div_remain1 = div_remain%par2;
	div_remain1 *= 2;

	if(div_remain1 >= par2)
		result += 1;
	
	return result;
}

static void RDATVSetFreCalPar(UINT32 freq, RDA5880_PARAM *rf_param)
{
	UINT32 freq_lo,freq_vco_core,freq_if,freq_adc;
	UINT8 freq_lo_index=0;
	UINT32 freqlo_base = 435000;					//870*1000/2
	UINT8 i;
	UINT32 pll_cal,rfpll_sdm,bbpll_sdm,pll_cal_cnttimer,temp;

	UINT32  freq_adc_tbl[]=
	{
		151875,162000,171125,0,141750,162000,182250
	};

	UINT16  freq_if_tbl[]=
	{
		2900,3025,2675,2675,2000,2000,2900,2900,0
	};

    RDATVSetAdcIndex((UINT8)(GetADCPatchByFre(freq)), rf_param);	
	freq_if = freq_if_tbl[rf_param->tvmode];
	freq_lo = freq+freq_if;
	freq_adc = (freq_adc_tbl[rf_param->freq_adc_index]);
	
	for(i=0;i<5;i++)
	{
		if(freq_lo > freqlo_base)
		{
			freq_lo_index = i;
			break;
		}
		else
		{
			freqlo_base /= 2;
		}
	}

	rf_param->TVReg46_2_X = 1<<(4-freq_lo_index);
	rf_param->TVreg1c_0 = (freq_lo_index == 0)? 1:0;

	
	freq_vco_core = freq_lo<<(freq_lo_index+1);
	if(freq_vco_core < 1250000)
		rf_param->TVReg46_2_X |= 0x20;
	
	pll_cal_cnttimer = RDATVMulDivCompensate(rf_param->RDATV_XTAL,1000,5);//(U16)(RDATV_XTAL/1000)<<5;
	rf_param->TVReg66 = (UINT16)(pll_cal_cnttimer&0xffff);
	
	pll_cal = RDATVMulDivCompensate(freq_vco_core,1000,5);//(freq_vco_core/1000)<<5;
	rf_param->TVReg64 = (UINT16)(pll_cal>>16&0xffff);
	rf_param->TVReg62 = (UINT16)(pll_cal&0xffff);

	rfpll_sdm = RDATVMulDivCompensate(freq_vco_core,rf_param->RDATV_XTAL,23);//(freq_vco_core/RDATV_XTAL);
	rf_param->TVReg250 = (UINT16)(rfpll_sdm>>16&0xffff);
	rf_param->TVReg24e = (UINT16)(rfpll_sdm&0xffff);

	bbpll_sdm = RDATVMulDivCompensate(freq_adc,rf_param->RDATV_XTAL,24);//(freq_adc/1000/RDATV_XTAL)<<24;
	rf_param->TVReg242 = (UINT16)(bbpll_sdm>>16&0xffff);
	rf_param->TVReg240 = (UINT16)(bbpll_sdm&0xffff);

	if(rf_param->freq_adc_index > 2)
		temp = (rf_param->freq_ifout-freq_if)*3;
	else
		temp = (rf_param->freq_ifout-freq_if)*2;

	if(rf_param->ifout_mode == manual)
	{
		rf_param->TVReg232 = (UINT16)(RDATVMulDivCompensate(temp,freq_adc,16));
		rf_param->TVReg232 |= 0x8000;
	}
	else
	{
		rf_param->TVReg232 = 0;
	}
}


RDA5880_ERR_MSG RDATVSetFreq(UINT32 freq, UINT32 bw, RDA5880_PARAM *rf_param )
{
	UINT16 reg;
	// UINT32 result=0;
	UINT8 tmp;
	RDA5880_ERR_MSG err_msg = RDA5880_OK;

	//dsp reset	
	err_msg = RDATVM_WRITE(0xff,0x0001);
	err_msg |= RDATVM_WRITE(0x02,0x0000);
	err_msg |= RDATVsetlna(freq, rf_param);
	RDATVSetFreCalPar(freq, rf_param);
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;
	
	err_msg = RDATVM_WRITE(0xff,0x0000);
	err_msg |= RDATVM_READ(0x46,&reg);
	reg &= ~0x00fc;
	rf_param->TVReg46_2_X <<= 2;
	reg |= rf_param->TVReg46_2_X;
	err_msg |= RDATVM_WRITE(0x46,reg);
	err_msg |= RDATVM_READ(0x1c,&reg);
	reg &= ~0x0001;
	reg |= rf_param->TVreg1c_0;
	err_msg |= RDATVM_WRITE(0x1c,reg);
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;

	//Set PLL_CAL_cnt_time
	err_msg = RDATVM_WRITE(0xff,0x0000);
	err_msg |= RDATVM_WRITE(0x66,rf_param->TVReg66);
	
	//Set PLL_CAL
	err_msg |= RDATVM_WRITE(0x62,rf_param->TVReg62);
	err_msg |= RDATVM_WRITE(0x64,rf_param->TVReg64);
	
	//pll_cal reset
	err_msg |= RDATVM_WRITE(0x6c,0x0000);
	err_msg |= RDATVM_WRITE(0x6c,0x0001);

	//set rfpll_sdm
	err_msg |= RDATVM_WRITE(0xff,0x0002);
	err_msg |= RDATVM_WRITE(0x4e,rf_param->TVReg24e);
	err_msg |= RDATVM_WRITE(0x50,rf_param->TVReg250);
	
	// rfpll_sdm reset
	err_msg |= RDATVM_WRITE(0xff,0x0001);
	err_msg |= RDATVM_READ(0x0a,&reg);
	reg &= ~0x0002;
	err_msg |= RDATVM_WRITE(0x0a,reg);
	reg |= 0x0002;
	err_msg |= RDATVM_WRITE(0x0a,reg);
	
	//set bbpll_sdm
	err_msg |= RDATVM_WRITE(0xff,0x0002);
	err_msg |= RDATVM_READ(0x5e,&reg);
	
	reg &= ~0x0010;
	err_msg |= RDATVM_WRITE(0x5e,reg);
	err_msg |= RDATVM_READ(0x48,&reg);
	reg |= 0x4000;
	err_msg |= RDATVM_WRITE(0x48,reg);
	err_msg |= RDATVM_WRITE(0x40,rf_param->TVReg240);
	err_msg |= RDATVM_WRITE(0x42,rf_param->TVReg242);
	// bbpll_sdm reset
	err_msg |= RDATVM_WRITE(0xff,0x0001);
	err_msg |= RDATVM_READ(0x0a,&reg);
	reg &= ~0x0001;
	err_msg |= RDATVM_WRITE(0x0a,reg);
	reg |= 0x0001;
	err_msg |= RDATVM_WRITE(0x0a,reg);
	//Set Tuner
	err_msg |= RDATVM_WRITE(0xff,0x0001);
	err_msg |= RDATVM_READ(0x0c,&reg);
	reg &= ~0x73f0;
	reg |= (UINT16)((rf_param->mode_t)&0x07)<<12;
	tmp = ((rf_param->tvmode)>7)?7:(rf_param->tvmode);
	reg |= (UINT16)tmp<<7;
	reg |= (UINT16)((rf_param->freq_adc_index)+1)<<4;
	err_msg |= RDATVM_WRITE(0x0c,reg);
	//Set ifout
	err_msg |= RDATVM_WRITE(0xff,0x0002);
	err_msg |= RDATVM_WRITE(0x32,rf_param->TVReg232);
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;

	//set band
	err_msg |= RDATVM_WRITE(0xff,0x0000);
	if(bw==8000)
	{
		err_msg |= RDATVM_READ(0x1C,&reg);
		err_msg |= RDATVM_WRITE(0x1c,(UINT16)((reg&0x0fff)|0x9000)); 
	}
	else if(bw==7000)
	{
		err_msg |= RDATVM_READ(0x1C,&reg);
		err_msg |= RDATVM_WRITE(0x1c,(UINT16)((reg&0x9fff)|0x9000)); 
	}
	else if(bw==6000)
	{
		err_msg |= RDATVM_READ(0x1C,&reg);
		err_msg |= RDATVM_WRITE(0x1c,(UINT16)(reg|0xf000));
	}

	err_msg |= RDATVM_WRITE(0xff,0x0002);
	reg = (UINT16)((bw/1000 - 6)*4);
	reg <<= 4;
	reg += 0x015d;
	err_msg |= RDATVM_WRITE(0x28,reg);
	err_msg |= RDATVM_WRITE(0xff,0x0001);
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;
	
	//set agc fast
	err_msg |= RDATVM_WRITE(0x2e,0xc18f);
	err_msg |= RDATVM_WRITE(0xa2,0x02ff);
	err_msg |= RDATVM_WRITE(0xb0,0x8c63);
		//dsp start	
	err_msg |= RDATVM_WRITE(0x02,0x0001);
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;
	RDADelayMs(50);
	//set agc slow
	err_msg |= RDATVM_WRITE(0x2e,0x118f);
	err_msg |= RDATVM_WRITE(0xa2,0x0214);
	err_msg |= RDATVM_WRITE(0xb0,0xcc50);
	RDADelayMs(5);

	//Fujitsu added
	err_msg |= RDATVM_WRITE(0xff,0x0001);
	err_msg |= RDATVM_WRITE(0xac,0x0760);
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;

	return RDA5880_OK;
}


RDA5880_ERR_MSG RDATVGetIntensity(UINT32 *level )
{
	UINT32 Lna1=0;
	UINT32 Lna2=0;
	UINT32 I2v=0;
	UINT32 Filter=0;
	UINT32 Vga=0;
	UINT32 digital_polarity=0;
	UINT32 digital=0;
	UINT16 temp = 0;
	UINT32 lna_gain1 = 0;
	UINT32 lna_gain2= 0;
	UINT32 lna_gain= 0;
	UINT32 i2v_gain = 0;
	UINT32 filter_gain = 0;
	UINT32 vga_gain = 0;
	UINT32 digital_gain_p=0;
	UINT32 digital_gain_n=0;
	UINT32 intensity;

	unsigned char lna_gain_tab[8]={0,0,2,8,13,18,24,32};

	RDA5880_ERR_MSG err_msg = RDA5880_OK;

	//lna gain   
	err_msg = RDATVM_WRITE(0xff,0x0001);
	err_msg |= RDATVM_READ(0x3a,&temp);
	Lna1= (temp>>6)&0x7; 
	lna_gain1=lna_gain_tab[Lna1];
	//RDATVM_READ(0x3a,&temp);
	Lna2= temp&0x3f; 
	lna_gain2=Lna2*13/63;
	lna_gain = lna_gain1+lna_gain2;

	//i2v gain
	err_msg |= RDATVM_READ(0x68,&temp);
	I2v= (temp>>3)&0x3;
	i2v_gain = I2v*6;

	//filter gain
	err_msg |= RDATVM_READ(0x98,&temp);
	Filter= (temp>>13)&0x3;
	filter_gain = Filter*12;

	//vga gain
	//RDATVM_READ(0x98,&temp);
	Vga= temp&0xf;
	vga_gain = Vga*15/10;

	//digital gain
	err_msg = RDATVM_READ(0xb2,&temp);
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;
	digital= (temp>>2)&0x1ff;
	digital_polarity= (temp>>11)&0x1;	

	if(digital_polarity==0)
	{
		digital_gain_p=digital/8;
		digital_gain_n = 0;
	}

	else if(digital_polarity==1)  
	{
		digital_gain_n=(511-digital)/8;
		digital_gain_p = 0;
	}

	intensity = lna_gain+i2v_gain+filter_gain+vga_gain+digital_gain_p-digital_gain_n-10;

	*level = intensity ;

	return RDA5880_OK;

}

RDA5880_ERR_MSG RDATVSingalStrength( UINT8 *strength )
{
	UINT32 vgain = 0;
	UINT16 signal_strength = 0;

	RDA5880_ERR_MSG err_msg = RDA5880_OK;

	err_msg = RDATVGetIntensity( &vgain );
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;

	signal_strength = (UINT16)(100 - vgain);

	if(signal_strength > 90)
	{
		signal_strength = 90;
	}
	if(signal_strength < 20)
	{
		signal_strength = 20;
	}

	*strength = (UINT8)signal_strength ;

	return RDA5880_OK;
}


void RDATVLnaMode(UINT8 mode, RDA5880_PARAM *rf_param)		
{
	rf_param->lnamode = mode;

}


RDA5880_ERR_MSG RDATVsetdemod(UINT8 mode, RDA5880_PARAM *rf_param)		
{
	RDA5880_ERR_MSG err_msg = RDA5880_OK;

	rf_param->demod = mode;
	if((rf_param->demod==MSB7819)||(rf_param->demod==MSB7818)||(rf_param->demod==NOV))
	{	
		err_msg = RDATVM_WRITE(0xff,0x0001);  
		err_msg |= RDATVM_WRITE(0xac,0x0766);
		if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;
		rf_param->freq_ifout=4500;
	} //if power=-6dBm

	else if((rf_param->demod==MSB71236D)||(rf_param->demod==PAN88472))
    {	
		err_msg = RDATVM_WRITE(0xff,0x0001);  
		err_msg |= RDATVM_WRITE(0xac,0x0790);
		if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;
		rf_param->freq_ifout=5000;
	}//if power=0dBm
	
	else if((rf_param->demod==AVAILINK)||(rf_param->demod==ALTOBEAM))
    {	
		err_msg = RDATVM_WRITE(0xff,0x0001);  
		err_msg |= RDATVM_WRITE(0xac,0x0790);
		if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;
		rf_param->freq_ifout=4500;
	}//if power=0dBm
 
	else if(rf_param->demod==X)    	
	{	
		err_msg = RDATVM_WRITE(0xff,0x0001);  
		err_msg |= RDATVM_WRITE(0xac,0x0790);
		if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;
		rf_param->freq_ifout=RDATV_IF_FREQ;
	}//if power=0dBm

	return RDA5880_OK;
}


void RDATVSetMode(UINT8 mode, RDA5880_PARAM *rf_param)		
{
	rf_param->tvmode =  mode;
}

void RDATVSetIfOutMode(UINT8 mode, RDA5880_PARAM *rf_param)
{
	static UINT16 const freq_ifout_tbl[]=
	{
		6850,7250,6400,6400,6400,6400,6750,6150,4500
	};

	rf_param->ifout_mode = mode;

	if(mode == manual)
	{
		rf_param->freq_ifout = RDATV_IF_FREQ;
	}
	else 
	{
		rf_param->freq_ifout = freq_ifout_tbl[rf_param->tvmode];
	}
}

void RDATVSetModeOutT(UINT8 mode, RDA5880_PARAM *rf_param)
{
	rf_param->mode_t = mode;
}

void RDATVSetAdcIndex(UINT8 index, RDA5880_PARAM *rf_param)
{
	rf_param->freq_adc_index = index;
}


RDA5880_ERR_MSG RDATVsetlna(UINT32 freq, RDA5880_PARAM *rf_param)
{
	RDA5880_ERR_MSG err_msg = RDA5880_OK;

	//Set LNA
	if(freq<= BAND_VH_START)
	{
		err_msg = RDATVM_WRITE(0xff,0x0000);
		err_msg |= RDATVM_WRITE(0x10,0x0194);
		err_msg |= RDATVM_WRITE(0x12,0x8214);

		if(rf_param->lnamode==SINGLE)
		{
			err_msg |= RDATVM_WRITE(0x14,0x070b);//single lna
		}
		else if(rf_param->lnamode==DIFF)
		{
			err_msg |= RDATVM_WRITE(0x14,0x270b);//diff lna
		}

		err_msg |= RDATVM_WRITE(0xff,0x0001);
		err_msg |= RDATVM_WRITE(0x50,0x6000);
		err_msg |= RDATVM_WRITE(0x52,0x6000);
		err_msg |= RDATVM_WRITE(0x54,0x6000);
		err_msg |= RDATVM_WRITE(0x56,0x6000);
		err_msg |= RDATVM_WRITE(0x58,0x6000);
		err_msg |= RDATVM_WRITE(0x5a,0x6000);
		err_msg |= RDATVM_WRITE(0x5c,0x6000);
		err_msg |= RDATVM_WRITE(0x5e,0x6000);
		err_msg |= RDATVM_WRITE(0x32,0x2b07);
		if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;
	}
	else if((freq>= BAND_VH_START)&&(freq  < BAND_U_START))
	{
		err_msg = RDATVM_WRITE(0xff,0x0000);
		err_msg |= RDATVM_WRITE(0x10,0x0294);
		err_msg |= RDATVM_WRITE(0x12,0x8224);

		if(rf_param->lnamode==SINGLE)
		{
			err_msg |= RDATVM_WRITE(0x14,0x0b0b);//single lna
		}
		else if(rf_param->lnamode==DIFF)
		{
			err_msg |= RDATVM_WRITE(0x14,0x4b0b);//RDATVM_WRITE(0x14,0x2b0b);//diff lna
		}

		err_msg |= RDATVM_WRITE(0xff,0x0001);
		err_msg |= RDATVM_WRITE(0x50,0x6000);
		err_msg |= RDATVM_WRITE(0x52,0x6000);
		err_msg |= RDATVM_WRITE(0x54,0x6000);
		err_msg |= RDATVM_WRITE(0x56,0x6000);
		err_msg |= RDATVM_WRITE(0x58,0x6000);
		err_msg |= RDATVM_WRITE(0x5a,0x6000);
		err_msg |= RDATVM_WRITE(0x5c,0x6000);
		err_msg |= RDATVM_WRITE(0x5e,0x6000);
		err_msg |= RDATVM_WRITE(0x32,0x2b07);
		if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;
	}
	else
	{
		err_msg = RDATVM_WRITE(0xff,0x0000);
		err_msg |= RDATVM_WRITE(0x10,0xc494);
		err_msg |= RDATVM_WRITE(0x12,0x8244);

		if(rf_param->lnamode==SINGLE)
		{
			err_msg |= RDATVM_WRITE(0x14,0x130b);//single lna
		}
		else if(rf_param->lnamode==DIFF)
		{
			err_msg |= RDATVM_WRITE(0x14,0x930b);//diff lna
		}

		err_msg |= RDATVM_WRITE(0xff,0x0001);
		err_msg |= RDATVM_WRITE(0x50,0x6000);
		err_msg |= RDATVM_WRITE(0x52,0x6000);
		err_msg |= RDATVM_WRITE(0x54,0x6000);
		err_msg |= RDATVM_WRITE(0x56,0x6000);
		err_msg |= RDATVM_WRITE(0x58,0x6000);
		err_msg |= RDATVM_WRITE(0x5a,0x6000);
		err_msg |= RDATVM_WRITE(0x5c,0x6000);
		err_msg |= RDATVM_WRITE(0x5e,0x6000);
		err_msg |= RDATVM_WRITE(0x32,0x2b17);
		if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;
	}
	return RDA5880_OK;
}


RDA5880_ERR_MSG RDATVRfBp(UINT8 on)
{
	RDA5880_ERR_MSG err_msg = RDA5880_OK;

	if(on)
	{
		err_msg = RDATVM_WRITE(0xff,0x0000);
		err_msg |= RDATVM_WRITE(0x1c,0x9E8F);
	}
	else
	{
		err_msg = RDATVM_WRITE(0xff,0x0000);
		err_msg |= RDATVM_WRITE(0x1c,0x9E8D);
	}
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;

	return RDA5880_OK;
}

RDA5880_ERR_MSG RDATVdelIM(UINT8 on)
{
	RDA5880_ERR_MSG err_msg = RDA5880_OK;

	if(on)
	{
		err_msg = RDATVM_WRITE(0xff,0x0002);
		err_msg |= RDATVM_WRITE(0x1c,0x0040);
	}
	else
	{
		err_msg = RDATVM_WRITE(0xff,0x0002);
		err_msg |= RDATVM_WRITE(0x1c,0x0140);
	}
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;
	return RDA5880_OK;
}

RDA5880_ERR_MSG RDATVSleepnoloop(void)
{
	RDA5880_ERR_MSG err_msg = RDA5880_OK;

    err_msg = RDATVSet51Sta(0);
	err_msg |= RDATVM_WRITE(0xff,0x0000);
	err_msg |= RDATVM_WRITE(0xe2,0x001A);
	err_msg |= RDATVM_WRITE(0xe4,0xa444);
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;

	return RDA5880_OK;
}


RDA5880_ERR_MSG RDATVSleepwithloop(void)
{
	RDA5880_ERR_MSG err_msg = RDA5880_OK;

    err_msg = RDATVSet51Sta(0);
	err_msg |= RDATVM_WRITE(0xff,0x0000);
	err_msg |= RDATVM_WRITE(0xe2,0x003a);
	err_msg |= RDATVM_WRITE(0xe4,0xacfc);
	err_msg |= RDATVM_WRITE(0x4A,0x802e);
	err_msg |= RDATVM_WRITE(0x4C,0x0040);
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;

	return RDA5880_OK;
}


RDA5880_ERR_MSG devTunerInit( RDA5880_PARAM *rf_param, UINT32 Xtal_freq )
{
	RDA5880_ERR_MSG err_msg = RDA5880_OK;
	
	rf_param->RDATV_XTAL = Xtal_freq ;

	err_msg = RDATVPowerOn();
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;

	RDATVLnaMode(SINGLE, rf_param);	
	RDATVSetMode(DTV, rf_param);
	RDATVSetIfOutMode(manual, rf_param);
	RDATVSetAdcIndex(ADC_162, rf_param);
	RDATVSetModeOutT(DTV_IF, rf_param);   
	err_msg = RDATVsetdemod(X, rf_param);
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;

	err_msg = RDATVGetI2VTH( rf_param );
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;
	err_msg = RDATVDownLoad();
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;

	return RDA5880_OK;
}


RDA5880_ERR_MSG devDigitalTuner_SetFreq (UINT32 Freq , UINT8 eBandWidth, RDA5880_PARAM *rf_param )
{
	RDA5880_ERR_MSG err_msg = RDA5880_OK;
	
	err_msg = RDATVSet51Sta(0);
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;

	err_msg = RDATVSetFreq(Freq,6000, rf_param);
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;

	err_msg = RDATVSet51Sta(1); 
	if( err_msg==RDA5880_ERR ) return RDA5880_ERR ;

	return RDA5880_OK;
}

