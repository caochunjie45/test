/*

Version 1.0   2013.09.13
 The  draft version

  Version 1.1 2013.09.26
 Modify the RDATVDownLoad in rdatv_bb.c for Solving the FEF issue.

  Version 1.2 2013.10.21
 Modify the RDATVDownLoad in rdatv_bb.c for Solving the FEF issue.
 
*/
//#include <string.h>
//#include "MsCommon.h"
//#include "Board.h"
//#include "apiDigiTuner.h"
//#include"drvTuner.h"
//#include "drvIIC.h"
//#include <windows.h>
#include	"rdatv_bb.h"
//#include    "..\Pfdriver.h"
#include "../Os_adapter.h"


#define mrom

//#define   RDA5880_TUNER_I2C_addr 0xc0  
//static s_tuner tuner_RDA5880H;//REX

static RDATVM_REG const mrom tvpwon[]=
{
	{0xff,0x0000,0}, 
	{0xe2,0x0002,0}, 
	{0xe2,0x003a,0}, 
	{0xe4,0xae6c,1}, 
	{0xe4,0xaeec,1}, 

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
	{0x06,0x000B,0}, //iq swap=1, Equal to 5880p  //orig 0x001b ylzhu
	{0x06,0x0007,0},  // orig 0x0017 ylzhu
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
};


static UINT32 const mrom freq_adc_tbl[]=
{
	151875,162000,171125,0,141750,162000,182250
};

static UINT16 const mrom freq_if_tbl[]=
{
	2900,3025,2675,2675,2000,2000,2900,2900,0
};

static UINT16 const mrom freq_ifout_tbl[]=
{
	6850,7250,6400,6400,6400,6400,6750,6150,4500
};
#ifdef	RDATV_TEST
#pragma	udata
#endif
static UINT16 TVReg240,TVReg242,TVReg62,TVReg64,TVReg250,TVReg24e,TVReg232,TVReg66;
static UINT8 TVReg46_2_X,TVreg1c_0;
static UINT8 lnamode, demod,tvmode,ifout_mode,freq_adc_index,mode_t;
static UINT32	freq_ifout;
static unsigned short  I2VFLOOR,I2VLTH,I2VHTH;
#ifdef	RDATV_TEST
#pragma
#endif
//rex add

// blow two function use to open IIC through mode.
extern UINT8 HDIC2311_OpenTunerI2C(UINT8 Address);
extern UINT8 HDIC2311_CloseTunerI2C(void);

static UINT8 RDATVM_WRITE(UINT8  reg, UINT16 tdata);
static UINT8 RDATVM_READ(UINT8 reg,UINT16* pdata);
static void RDADelayMs(UINT32 delay_ms);
static void RDATVRfBp(UINT8 on);
static void RDATVdelIM(UINT8 on);

// 实现IIC通讯接口 
static UINT8 RDATVM_WRITE(UINT8  reg, UINT16 tdata)
{
	UINT8 u8Data[2]={0, 0};
	UINT8 u8Ret = 0; // 0: no error  
	UINT8 u8RetryTimes = 10, u8tmp = 0;

	//HDIC2311_OpenTunerI2C(RDA5880_I2C_addr_WR_BIT);
	
	u8Data[0] = tdata>>8;
	u8Data[1] = tdata&0xFF;
	for(u8tmp = 0; u8tmp < u8RetryTimes; u8tmp++)
	{
		u8Ret=iic_write( RDA5880_TUNER_I2C_addr,&reg, 1, u8Data, 2);
		if(u8Ret == 0)
		{
			break;
		}
		else
		{
			printf(">RDA 5880H IIC_WRITE Error \n");
		}
		
	}
	//HDIC2311_CloseTunerI2C();
	return u8Ret;
}

static UINT8 RDATVM_READ(UINT8 reg,UINT16* pdata)
{
	UINT8 u8Data[2]={0, 0};
	UINT8 u8Ret = 0; // 0: no error  
	UINT8 u8RetryTimes = 10, u8tmp = 0;
	//HDIC2311_OpenTunerI2C(RDA5880_I2C_addr_WR_BIT);
	for(u8tmp = 0; u8tmp < u8RetryTimes; u8tmp++)
	{
		#if 0
		u8Ret =iic_write( RDA5880_TUNER_I2C_addr,&reg, 1, NULL, 0);
		u8Ret|=iic_read(RDA5880_TUNER_I2C_addr, NULL, 0, u8Data, 2);
		#else
		u8Ret|=iic_read(RDA5880_TUNER_I2C_addr, &reg, 1, u8Data, 2);
		#endif
		if(u8Ret == 0)
		{
			*pdata = (UINT16)(u8Data[0]<<8) + u8Data[1];
			break;
		}
		else
		{
			printf(">RDA 5880H IIC_WRITE Error \n");
		}
		
	}
	//HDIC2311_CloseTunerI2C();
	return u8Ret;
}

static void RDADelayMs(UINT32 delay_ms)
{
	msleep(delay_ms);
}

UINT16 RDATVGetChipID(void)
{
	UINT16 chipid;

	
	RDATVM_WRITE(0xff,0);
	RDATVM_READ(0xe2,&chipid);

	return chipid;
}

#ifndef CLOSE_TUNER_UNNECESSARY_FUNCTION
int RDATVDetected(void)
{
	UINT16 chipid;

	
	RDATVM_WRITE(0xff,0);
	RDATVM_READ(0xe2,&chipid);

	return ((chipid == 0x0018)?1:0);
}
#endif
	
void	RDATVPowerOn(void)
{
	UINT32 i,j;
	//UINT32 reg;
	
	j = sizeof(tvpwon)/sizeof(RDATVM_REG);
	do 
	{
		for(i=0;i<j;i++)
		{
			RDATVM_WRITE(tvpwon[i].add,tvpwon[i].data);
			RDADelayMs(tvpwon[i].delay);
		}
	}while(0);

	RDATVRfBp(0);
	RDATVdelIM(1);
}

void RDATVSet51Sta(UINT8 Status)
{
	if(Status==0)
	{
		RDATVM_WRITE(0xff,0x0000);  
		RDATVM_WRITE(0x30,0x011C);
		RDATVM_WRITE(0x30,0x811C);
		RDATVM_WRITE(0xff,0x0001);  
		RDATVM_WRITE(0x3a,0x0000);//  release lna
		RDATVM_WRITE(0x94,0x0802);//release 
		RDADelayMs(2);
	}
	else
	{
		RDATVM_WRITE(0xff,0x0000);  
		RDATVM_WRITE(0x30,0x011C);
		RDATVM_WRITE(0x30,0x011D);
		RDADelayMs(2);
	}	  
}
void RDATVGetI2VTH(void) 
{    
	UINT16 temp = 0;

	RDATVSetFreq(666000,8000);

	RDATVM_WRITE(0xff,0x0000);  
	RDATVM_WRITE(0x4A,0xbf2e); 
	   
	RDADelayMs(50);
	RDATVM_WRITE(0xff,0x0001);  
	RDATVM_READ(0x6A,&temp);
	I2VFLOOR=((temp>>4)&0x0fff);
	I2VLTH=I2VFLOOR+0X0180;
	I2VLTH=((I2VLTH<<4)&0xfff0);    
	I2VHTH=I2VLTH*4;
	RDATVM_WRITE(0x66,I2VLTH);
	RDATVM_WRITE(0x64,I2VHTH);


	RDATVM_WRITE(0xff,0x0000);  
	RDATVM_WRITE(0x4A,0xff2e); 

}

PATCH_ADC_BY_FRE Patch_ADC_Tbl[]=
{
 	{52500 ,2000,ADC_172},
	{80000 ,2000,ADC_172},
	
	{68500 ,2000,ADC_151},

	
};

RDATV_ADC_INDEX GetADCPatchByFre(UINT32 freq)  
{
	UINT8 i;

	 for(i=0;i<(sizeof(Patch_ADC_Tbl)/sizeof(PATCH_ADC_BY_FRE));i++)
	{	
		if((freq>=(Patch_ADC_Tbl[i].Fre-Patch_ADC_Tbl[i].Delta))&&(freq<=(Patch_ADC_Tbl[i].Fre+Patch_ADC_Tbl[i].Delta)))
			return Patch_ADC_Tbl[i].Adc;
	}
 	
 
	 return ADC_162;
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

static void RDATVSetFreCalPar(UINT32 freq)
{
	UINT32 freq_lo,freq_vco_core,freq_if,freq_adc;
	UINT8 freq_lo_index=0;
	UINT32 freqlo_base = 435000;					//870*1000/2
	UINT8 i;
	UINT32 pll_cal,rfpll_sdm,bbpll_sdm,pll_cal_cnttimer,temp;
	
       RDATVSetAdcIndex(GetADCPatchByFre(freq));	
	freq_if = freq_if_tbl[tvmode];
	freq_lo = freq+freq_if;
	freq_adc = (freq_adc_tbl[freq_adc_index]);
	
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

	TVReg46_2_X = 1<<(4-freq_lo_index);
	TVreg1c_0 = (freq_lo_index == 0)? 1:0;

	
	freq_vco_core = freq_lo<<(freq_lo_index+1);
	if(freq_vco_core < 1250000)
		TVReg46_2_X |= 0x20;
	
	pll_cal_cnttimer = RDATVMulDivCompensate(RDATV_XTAL,1000,5);//(UINT16)(RDATV_XTAL/1000)<<5;
	TVReg66 = pll_cal_cnttimer&0xffff;
	
	pll_cal = RDATVMulDivCompensate(freq_vco_core,1000,5);//(freq_vco_core/1000)<<5;
	TVReg64 = pll_cal>>16&0xffff;
	TVReg62 = pll_cal&0xffff;

	rfpll_sdm = RDATVMulDivCompensate(freq_vco_core,RDATV_XTAL,23);//(freq_vco_core/RDATV_XTAL);
	TVReg250 = rfpll_sdm>>16&0xffff;
	TVReg24e = rfpll_sdm&0xffff;

	bbpll_sdm = RDATVMulDivCompensate(freq_adc,RDATV_XTAL,24);//(freq_adc/1000/RDATV_XTAL)<<24;
	TVReg242 = bbpll_sdm>>16&0xffff;
	TVReg240 = bbpll_sdm&0xffff;

	if(freq_adc_index > 2)
		temp = (freq_ifout-freq_if)*3;
	else
		temp = (freq_ifout-freq_if)*2;

	if(ifout_mode == manual)
	{
		TVReg232 = RDATVMulDivCompensate(temp,freq_adc,16);
		TVReg232 |= 0x8000;
	}
	else
	{
		TVReg232 = 0;
	}

}


UINT32	RDATVSetFreq(UINT32 freq,UINT32 bw)
{
	UINT16 reg;
	UINT32 result=0;
	UINT8 tmp;

	//dsp reset	
	RDATVM_WRITE(0xff,0x0001);
	RDATVM_WRITE(0x02,0x0000);
	RDATVsetlna(freq);
	
	RDATVSetFreCalPar(freq);
	
	RDATVM_WRITE(0xff,0x0000);
	RDATVM_READ(0x46,&reg);
	reg &= ~0x00fc;
	TVReg46_2_X <<= 2;
	reg |= TVReg46_2_X;
	RDATVM_WRITE(0x46,reg);

	RDATVM_READ(0x1c,&reg);
	reg &= ~0x0001;
	reg |= TVreg1c_0;
	RDATVM_WRITE(0x1c,reg);

       

	//Set PLL_CAL_cnt_time

	RDATVM_WRITE(0xff,0x0000);
	RDATVM_WRITE(0x66,TVReg66);
	//Set PLL_CAL
	RDATVM_WRITE(0x62,TVReg62);
	RDATVM_WRITE(0x64,TVReg64);
	//pll_cal reset
	RDATVM_WRITE(0x6c,0x0000);
	RDATVM_WRITE(0x6c,0x0001);

	//set rfpll_sdm
	RDATVM_WRITE(0xff,0x0002);
	RDATVM_WRITE(0x4e,TVReg24e);
	RDATVM_WRITE(0x50,TVReg250);
	// rfpll_sdm reset
	RDATVM_WRITE(0xff,0x0001);
	RDATVM_READ(0x0a,&reg);
	reg &= ~0x0002;
	RDATVM_WRITE(0x0a,reg);
	reg |= 0x0002;
	RDATVM_WRITE(0x0a,reg);
	//set bbpll_sdm
	RDATVM_WRITE(0xff,0x0002);
	RDATVM_READ(0x5e,&reg);
	reg &= ~0x0010;
	RDATVM_WRITE(0x5e,reg);
	RDATVM_READ(0x48,&reg);
	reg |= 0x4000;
	RDATVM_WRITE(0x48,reg);
	RDATVM_WRITE(0x40,TVReg240);
	RDATVM_WRITE(0x42,TVReg242);
	// bbpll_sdm reset
	RDATVM_WRITE(0xff,0x0001);
	RDATVM_READ(0x0a,&reg);
	reg &= ~0x0001;
	RDATVM_WRITE(0x0a,reg);
	reg |= 0x0001;
	RDATVM_WRITE(0x0a,reg);
	//Set Tuner
	RDATVM_WRITE(0xff,0x0001);
	RDATVM_READ(0x0c,&reg);
	reg &= ~0x73f0;
	reg |= (UINT16)(mode_t&0x07)<<12;
	tmp = (tvmode>7)?7:tvmode;
	reg |= (UINT16)tmp<<7;
	reg |= (UINT16)(freq_adc_index+1)<<4;
	RDATVM_WRITE(0x0c,reg);
	//Set ifout
	RDATVM_WRITE(0xff,0x0002);
	RDATVM_WRITE(0x32,TVReg232);

	//set band
	RDATVM_WRITE(0xff,0x0000);
	if(bw==8000)
	{
		RDATVM_READ(0x1C,&reg);
		RDATVM_WRITE(0x1c,(reg&0x0fff)|0x9000); 
	}
	else if(bw==7000)
	{
		RDATVM_READ(0x1C,&reg);
		RDATVM_WRITE(0x1c,(reg&0x9fff)|0x9000); 
	}
	else if(bw==6000)
	{
		RDATVM_READ(0x1C,&reg);
		RDATVM_WRITE(0x1c,(reg|0xf000)); 
	}

	RDATVM_WRITE(0xff,0x0002);
	reg = (bw/1000 - 6)*4;
	reg <<= 4;
	reg += 0x015d;
	RDATVM_WRITE(0x28,reg);

	RDATVM_WRITE(0xff,0x0001);
	//set agc fast
	RDATVM_WRITE(0x2e,0xc18f);
	RDATVM_WRITE(0xa2,0x02ff);
	RDATVM_WRITE(0xb0,0x8c63);
		//dsp start	
	RDATVM_WRITE(0x02,0x0001);
	RDADelayMs(50);
	//set agc slow
	RDATVM_WRITE(0x2e,0x118f);
	RDATVM_WRITE(0xa2,0x0214);
	RDATVM_WRITE(0xb0,0xcc50);
	RDADelayMs(5);
	
	return result;
}

void RDATVSleepnoloop(void)
{     
       RDATVSet51Sta(0);
	RDATVM_WRITE(0xff,0x0000);
	RDATVM_WRITE(0xe2,0x001A);
	RDATVM_WRITE(0xe4,0xa444);
}


void RDATVSleepwithloop(void)
{     
       RDATVSet51Sta(0);
	RDATVM_WRITE(0xff,0x0000);
	RDATVM_WRITE(0xe2,0x003a);
	RDATVM_WRITE(0xe4,0xacfc);
	RDATVM_WRITE(0x4A,0x802e);
	RDATVM_WRITE(0x4C,0x0040);
}


static UINT32  RDATVGetIntensity(void)
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


	//lna gain   
	RDATVM_WRITE(0xff,0x0001);  
	RDATVM_READ(0x3a,&temp);
	Lna1= (temp>>6)&0x7; 
	lna_gain1=lna_gain_tab[Lna1];
	//RDATVM_READ(0x3a,&temp);
	Lna2= temp&0x3f; 
	lna_gain2=Lna2*13/63;
	lna_gain = lna_gain1+lna_gain2;

	//i2v gain
	RDATVM_READ(0x68,&temp);
	I2v= (temp>>3)&0x3;
	i2v_gain = I2v*6;

	//filter gain
	RDATVM_READ(0x98,&temp);
	Filter= (temp>>13)&0x3;
	filter_gain = Filter*12;

	//vga gain
	//RDATVM_READ(0x98,&temp);
	Vga= temp&0xf;
	vga_gain = Vga*15/10;

	//digital gain
	RDATVM_READ(0xb2,&temp);
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

	return intensity;


}

RDA5880_ERR_MSG RDATVSingalStrength( UINT8 *strength )
{
	UINT32 vgain = 0;
	UINT16 signal_strength = 0;

	RDA5880_ERR_MSG err_msg = RDA5880_OK;

	err_msg = RDATVGetIntensity();
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


void RDATVLnaMode(UINT8 mode)		
{
    lnamode = mode;
	
}


void RDATVsetdemod(UINT8 mode)		
{
	demod = mode;
	if((demod==MSB7819)||(demod==MSB7818)||(demod==NOV))
	{	
		RDATVM_WRITE(0xff,0x0001);  
		RDATVM_WRITE(0xac,0x0766);
		freq_ifout=4500;
	} //if power=-6dBm
	else if((demod==MSB71236D)||(demod==PAN88472))
	{	
		RDATVM_WRITE(0xff,0x0001);  
		RDATVM_WRITE(0xac,0x0790);
		freq_ifout=5000;
	}//if power=0dBm
	else if((demod==AVAILINK)||(demod==ALTOBEAM))
	{	
		RDATVM_WRITE(0xff,0x0001);  
		RDATVM_WRITE(0xac,0x0790);
		freq_ifout=4500;
	}//if power=0dBm

	else if(demod==X)    	
	{	
		RDATVM_WRITE(0xff,0x0001);  
		RDATVM_WRITE(0xac,0x0760);  // note ylzhu  orig 0x0790
		freq_ifout=RDATV_IF_FREQ;
	}//if power=0dBm

}

void RDATVSetMode(UINT8 mode)		
{
	tvmode =  mode;
}

void RDATVSetIfOutMode(UINT8 mode)
{
	ifout_mode = mode;

	if(mode == manual)
	{
		freq_ifout = RDATV_IF_FREQ;
	}
	else 
	{
		freq_ifout = freq_ifout_tbl[tvmode];
	}
}

void RDATVSetModeOutT(UINT8 mode)
{
	mode_t = mode;
}

void RDATVSetAdcIndex(UINT8 index)
{
	freq_adc_index = index;
}


void RDATVsetlna(UINT32 freq)
{
	//Set LNA
	if(freq<= BAND_VH_START)
	{
		RDATVM_WRITE(0xff,0x0000);
		RDATVM_WRITE(0x10,0x8194);
		RDATVM_WRITE(0x12,0x8214);

		if(lnamode==SINGLE)
		{
		RDATVM_WRITE(0x14,0x070b);//single lna
		}
		else if(lnamode==DIFF)
		{
		RDATVM_WRITE(0x14,0x270b);//diff lna
		}


		RDATVM_WRITE(0xff,0x0001);
		RDATVM_WRITE(0x50,0x6000);
		RDATVM_WRITE(0x52,0x6000);
		RDATVM_WRITE(0x54,0x6000);
		RDATVM_WRITE(0x56,0x6000);
		RDATVM_WRITE(0x58,0x6000);
		RDATVM_WRITE(0x5a,0x6000);
		RDATVM_WRITE(0x5c,0x6000);
		RDATVM_WRITE(0x5e,0x6000);
		RDATVM_WRITE(0x32,0x2b07);

	}
	else if((freq>= BAND_VH_START)&&(freq  < BAND_U_START))
	{
		RDATVM_WRITE(0xff,0x0000);
		RDATVM_WRITE(0x10,0x8294);
		RDATVM_WRITE(0x12,0x8224);
		if(lnamode==SINGLE)
		{
			RDATVM_WRITE(0x14,0x0b0b);//single lna
		}
		else if(lnamode==DIFF)
		{
			RDATVM_WRITE(0x14,0x4b0b);//RDATVM_WRITE(0x14,0x2b0b);//diff lna
		}

		RDATVM_WRITE(0xff,0x0001);
		RDATVM_WRITE(0x50,0x6000);
		RDATVM_WRITE(0x52,0x6000);
		RDATVM_WRITE(0x54,0x6000);
		RDATVM_WRITE(0x56,0x6000);
		RDATVM_WRITE(0x58,0x6000);
		RDATVM_WRITE(0x5a,0x6000);
		RDATVM_WRITE(0x5c,0x6000);
		RDATVM_WRITE(0x5e,0x6000);
		RDATVM_WRITE(0x32,0x2b07);

	}
	else
	{
		RDATVM_WRITE(0xff,0x0000);
		RDATVM_WRITE(0x10,0xc494);
		RDATVM_WRITE(0x12,0x8244);
		if(lnamode==SINGLE)
		{
		RDATVM_WRITE(0x14,0x130b);//single lna
		}
		else if(lnamode==DIFF)
		{
		RDATVM_WRITE(0x14,0x930b);//diff lna
		}

		RDATVM_WRITE(0xff,0x0001);
		RDATVM_WRITE(0x50,0x6000);
		RDATVM_WRITE(0x52,0x6000);
		RDATVM_WRITE(0x54,0x6000);
		RDATVM_WRITE(0x56,0x6000);
		RDATVM_WRITE(0x58,0x6000);
		RDATVM_WRITE(0x5a,0x6000);
		RDATVM_WRITE(0x5c,0x6000);
		RDATVM_WRITE(0x5e,0x6000);
		RDATVM_WRITE(0x32,0x2b17);

	}
}




static void RDATVRfBp(UINT8 on)
{
	if(on)
	{
		RDATVM_WRITE(0xff,0x0000);
		RDATVM_WRITE(0x1c,0x9E8F);
	}
	else
	{
		RDATVM_WRITE(0xff,0x0000);
		RDATVM_WRITE(0x1c,0x9E8D);
	}
}

static void RDATVdelIM(UINT8 on)
{
	if(on)
	{
		RDATVM_WRITE(0xff,0x0002);
		RDATVM_WRITE(0x1c,0x0040);
	}
	else
	{
		RDATVM_WRITE(0xff,0x0002);
		RDATVM_WRITE(0x1c,0x0140);
	}
}

void RDATVDownLoad(void)
{
	RDATVM_WRITE(0xff,0x0000);
	RDATVM_WRITE(0x30,0x011E);
	RDADelayMs(100);
	                    
	RDATVM_WRITE(0xFE,0x0204);
	RDATVM_WRITE(0xFE,0x4FE4);
	RDATVM_WRITE(0xFE,0xF50C);
	RDATVM_WRITE(0xFE,0xF50D);
	RDATVM_WRITE(0xFE,0xF50E);
	RDATVM_WRITE(0xFE,0xF50F);
	RDATVM_WRITE(0xFE,0xF510);
	RDATVM_WRITE(0xFE,0x7511);
	RDATVM_WRITE(0xFE,0x19F5);
	RDATVM_WRITE(0xFE,0x1275);
	RDATVM_WRITE(0xFE,0x1305);
	RDATVM_WRITE(0xFE,0xF514);
	RDATVM_WRITE(0xFE,0xF515);
	RDATVM_WRITE(0xFE,0x7516);
	RDATVM_WRITE(0xFE,0x0775);
	RDATVM_WRITE(0xFE,0x17D0);
	RDATVM_WRITE(0xFE,0x1204);
	RDATVM_WRITE(0xFE,0xDB8F);
	RDATVM_WRITE(0xFE,0x0985);
	RDATVM_WRITE(0xFE,0x201F);
	RDATVM_WRITE(0xFE,0x8521);
	RDATVM_WRITE(0xFE,0x2085);
	RDATVM_WRITE(0xFE,0x2221);
	RDATVM_WRITE(0xFE,0xE509);
	RDATVM_WRITE(0xFE,0x1313);
	RDATVM_WRITE(0xFE,0x543F);
	RDATVM_WRITE(0xFE,0xF522);
	RDATVM_WRITE(0xFE,0xE51F);
	RDATVM_WRITE(0xFE,0x2520);
	RDATVM_WRITE(0xFE,0xF509);
	RDATVM_WRITE(0xFE,0x2521);
	RDATVM_WRITE(0xFE,0xF509);
	RDATVM_WRITE(0xFE,0x2522);
	RDATVM_WRITE(0xFE,0xF509);
	RDATVM_WRITE(0xFE,0xFB7D);
	RDATVM_WRITE(0xFE,0xBF7F);
	RDATVM_WRITE(0xFE,0x01B1);
	RDATVM_WRITE(0xFE,0x93E5);
	RDATVM_WRITE(0xFE,0x09C3);
	RDATVM_WRITE(0xFE,0x9525);
	RDATVM_WRITE(0xFE,0x400D);
	RDATVM_WRITE(0xFE,0xE509);
	RDATVM_WRITE(0xFE,0xD395);
	RDATVM_WRITE(0xFE,0x2950);
	RDATVM_WRITE(0xFE,0x067E);
	RDATVM_WRITE(0xFE,0x007F);
	RDATVM_WRITE(0xFE,0x0180);
	RDATVM_WRITE(0xFE,0x047E);
	RDATVM_WRITE(0xFE,0x007F);
	RDATVM_WRITE(0xFE,0x008E);
	RDATVM_WRITE(0xFE,0x148F);
	RDATVM_WRITE(0xFE,0x15E5);
	RDATVM_WRITE(0xFE,0x1545);
	RDATVM_WRITE(0xFE,0x1460);
	RDATVM_WRITE(0xFE,0x087B);
	RDATVM_WRITE(0xFE,0x307D);
	RDATVM_WRITE(0xFE,0xA17F);
	RDATVM_WRITE(0xFE,0x01A1);
	RDATVM_WRITE(0xFE,0x93E5);
	RDATVM_WRITE(0xFE,0x09C3);
	RDATVM_WRITE(0xFE,0x9525);
	RDATVM_WRITE(0xFE,0x4002);
	RDATVM_WRITE(0xFE,0x2139);
	RDATVM_WRITE(0xFE,0xE509);
	RDATVM_WRITE(0xFE,0xC395);
	RDATVM_WRITE(0xFE,0x2840);
	RDATVM_WRITE(0xFE,0x0221);
	RDATVM_WRITE(0xFE,0xFF7B);
	RDATVM_WRITE(0xFE,0x317D);
	RDATVM_WRITE(0xFE,0xA17F);
	RDATVM_WRITE(0xFE,0x01B1);
	RDATVM_WRITE(0xFE,0x93E5);
	RDATVM_WRITE(0xFE,0x09D3);
	RDATVM_WRITE(0xFE,0x9525);
	RDATVM_WRITE(0xFE,0x4008);
	RDATVM_WRITE(0xFE,0x750E);
	RDATVM_WRITE(0xFE,0x0075);
	RDATVM_WRITE(0xFE,0x0F05);
	RDATVM_WRITE(0xFE,0x8006);
	RDATVM_WRITE(0xFE,0x750E);
	RDATVM_WRITE(0xFE,0x0075);
	RDATVM_WRITE(0xFE,0x0F19);
	RDATVM_WRITE(0xFE,0x7D3B);
	RDATVM_WRITE(0xFE,0x7F01);
	RDATVM_WRITE(0xFE,0xB1A5);
	RDATVM_WRITE(0xFE,0x8F0A);
	RDATVM_WRITE(0xFE,0x7D3A);
	RDATVM_WRITE(0xFE,0x7F01);
	RDATVM_WRITE(0xFE,0xB1A5);
	RDATVM_WRITE(0xFE,0x8F0B);
	RDATVM_WRITE(0xFE,0xE50A);
	RDATVM_WRITE(0xFE,0x5401);
	RDATVM_WRITE(0xFE,0xFE7C);
	RDATVM_WRITE(0xFE,0x00E4);
	RDATVM_WRITE(0xFE,0x250B);
	RDATVM_WRITE(0xFE,0xF50D);
	RDATVM_WRITE(0xFE,0xEC3E);
	RDATVM_WRITE(0xFE,0xF50C);
	RDATVM_WRITE(0xFE,0xE50D);
	RDATVM_WRITE(0xFE,0xB4FF);
	RDATVM_WRITE(0xFE,0x06E5);
	RDATVM_WRITE(0xFE,0x0CB4);
	RDATVM_WRITE(0xFE,0x0101);
	RDATVM_WRITE(0xFE,0x227B);
	RDATVM_WRITE(0xFE,0x327D);
	RDATVM_WRITE(0xFE,0xA17F);
	RDATVM_WRITE(0xFE,0x01B1);
	RDATVM_WRITE(0xFE,0x93E5);
	RDATVM_WRITE(0xFE,0x0F25);
	RDATVM_WRITE(0xFE,0x0DF5);
	RDATVM_WRITE(0xFE,0x0DE5);
	RDATVM_WRITE(0xFE,0x0E35);
	RDATVM_WRITE(0xFE,0x0CF5);
	RDATVM_WRITE(0xFE,0x0CC3);
	RDATVM_WRITE(0xFE,0xE50D);
	RDATVM_WRITE(0xFE,0x94FF);
	RDATVM_WRITE(0xFE,0xE50C);
	RDATVM_WRITE(0xFE,0x6480);
	RDATVM_WRITE(0xFE,0x9481);
	RDATVM_WRITE(0xFE,0x4014);
	RDATVM_WRITE(0xFE,0x430A);
	RDATVM_WRITE(0xFE,0x0375);
	RDATVM_WRITE(0xFE,0x0BFF);
	RDATVM_WRITE(0xFE,0xAB0A);
	RDATVM_WRITE(0xFE,0x7D3B);
	RDATVM_WRITE(0xFE,0x7F01);
	RDATVM_WRITE(0xFE,0xB193);
	RDATVM_WRITE(0xFE,0xAB0B);
	RDATVM_WRITE(0xFE,0x7D3A);
	RDATVM_WRITE(0xFE,0x8022);
	RDATVM_WRITE(0xFE,0x7B33);
	RDATVM_WRITE(0xFE,0x7DA1);
	RDATVM_WRITE(0xFE,0x7F01);
	RDATVM_WRITE(0xFE,0xB193);
	RDATVM_WRITE(0xFE,0xE50D);
	RDATVM_WRITE(0xFE,0xF50B);
	RDATVM_WRITE(0xFE,0xE50C);
	RDATVM_WRITE(0xFE,0xFFE5);
	RDATVM_WRITE(0xFE,0x0A54);
	RDATVM_WRITE(0xFE,0xFE2F);
	RDATVM_WRITE(0xFE,0xF50A);
	RDATVM_WRITE(0xFE,0xAB0B);
	RDATVM_WRITE(0xFE,0x7D3A);
	RDATVM_WRITE(0xFE,0x7F01);
	RDATVM_WRITE(0xFE,0xB193);
	RDATVM_WRITE(0xFE,0xAB0A);
	RDATVM_WRITE(0xFE,0x7D3B);
	RDATVM_WRITE(0xFE,0x7F01);
	RDATVM_WRITE(0xFE,0xB193);
	RDATVM_WRITE(0xFE,0x1204);
	RDATVM_WRITE(0xFE,0xDB8F);
	RDATVM_WRITE(0xFE,0x09AB);
	RDATVM_WRITE(0xFE,0x097D);
	RDATVM_WRITE(0xFE,0xBE7F);
	RDATVM_WRITE(0xFE,0x01B1);
	RDATVM_WRITE(0xFE,0x9301);
	RDATVM_WRITE(0xFE,0x7EE5);
	RDATVM_WRITE(0xFE,0x09D3);
	RDATVM_WRITE(0xFE,0x9529);
	RDATVM_WRITE(0xFE,0x5002);
	RDATVM_WRITE(0xFE,0x21FF);
	RDATVM_WRITE(0xFE,0xE509);
	RDATVM_WRITE(0xFE,0xD395);
	RDATVM_WRITE(0xFE,0x2850);
	RDATVM_WRITE(0xFE,0x0221);
	RDATVM_WRITE(0xFE,0xFF7B);
	RDATVM_WRITE(0xFE,0x347D);
	RDATVM_WRITE(0xFE,0xA17F);
	RDATVM_WRITE(0xFE,0x01B1);
	RDATVM_WRITE(0xFE,0x93E5);
	RDATVM_WRITE(0xFE,0x09C3);
	RDATVM_WRITE(0xFE,0x9529);
	RDATVM_WRITE(0xFE,0x5008);
	RDATVM_WRITE(0xFE,0x8512);
	RDATVM_WRITE(0xFE,0x0E85);
	RDATVM_WRITE(0xFE,0x130F);
	RDATVM_WRITE(0xFE,0x8006);
	RDATVM_WRITE(0xFE,0x8510);
	RDATVM_WRITE(0xFE,0x0E85);
	RDATVM_WRITE(0xFE,0x110F);
	RDATVM_WRITE(0xFE,0x7D3B);
	RDATVM_WRITE(0xFE,0x7F01);
	RDATVM_WRITE(0xFE,0xB1A5);
	RDATVM_WRITE(0xFE,0x8F0A);
	RDATVM_WRITE(0xFE,0x7D3A);
	RDATVM_WRITE(0xFE,0x7F01);
	RDATVM_WRITE(0xFE,0xB1A5);
	RDATVM_WRITE(0xFE,0x8F0B);
	RDATVM_WRITE(0xFE,0xE50A);
	RDATVM_WRITE(0xFE,0x5401);
	RDATVM_WRITE(0xFE,0xFE7C);
	RDATVM_WRITE(0xFE,0x00E4);
	RDATVM_WRITE(0xFE,0x250B);
	RDATVM_WRITE(0xFE,0xF50D);
	RDATVM_WRITE(0xFE,0xEC3E);
	RDATVM_WRITE(0xFE,0xF50C);
	RDATVM_WRITE(0xFE,0x450D);
	RDATVM_WRITE(0xFE,0x7001);
	RDATVM_WRITE(0xFE,0x227B);
	RDATVM_WRITE(0xFE,0x357D);
	RDATVM_WRITE(0xFE,0xA17F);
	RDATVM_WRITE(0xFE,0x01B1);
	RDATVM_WRITE(0xFE,0x93C3);
	RDATVM_WRITE(0xFE,0xE50D);
	RDATVM_WRITE(0xFE,0x950F);
	RDATVM_WRITE(0xFE,0xF50D);
	RDATVM_WRITE(0xFE,0xE50C);
	RDATVM_WRITE(0xFE,0x950E);
	RDATVM_WRITE(0xFE,0xF50C);
	RDATVM_WRITE(0xFE,0xD3E5);
	RDATVM_WRITE(0xFE,0x0D94);
	RDATVM_WRITE(0xFE,0x00E5);
	RDATVM_WRITE(0xFE,0x0C64);
	RDATVM_WRITE(0xFE,0x8094);
	RDATVM_WRITE(0xFE,0x8050);
	RDATVM_WRITE(0xFE,0x1C7B);
	RDATVM_WRITE(0xFE,0x367D);
	RDATVM_WRITE(0xFE,0xA17F);
	RDATVM_WRITE(0xFE,0x01B1);
	RDATVM_WRITE(0xFE,0x9353);
	RDATVM_WRITE(0xFE,0x0AFE);
	RDATVM_WRITE(0xFE,0xE4F5);
	RDATVM_WRITE(0xFE,0x0BAB);
	RDATVM_WRITE(0xFE,0x0A7D);
	RDATVM_WRITE(0xFE,0x3B7F);
	RDATVM_WRITE(0xFE,0x01B1);
	RDATVM_WRITE(0xFE,0x93AB);
	RDATVM_WRITE(0xFE,0x0B7D);
	RDATVM_WRITE(0xFE,0x3A80);
	RDATVM_WRITE(0xFE,0x227B);
	RDATVM_WRITE(0xFE,0x377D);
	RDATVM_WRITE(0xFE,0xA17F);
	RDATVM_WRITE(0xFE,0x01B1);
	RDATVM_WRITE(0xFE,0x93E5);
	RDATVM_WRITE(0xFE,0x0DF5);
	RDATVM_WRITE(0xFE,0x0BE5);
	RDATVM_WRITE(0xFE,0x0CFF);
	RDATVM_WRITE(0xFE,0xE50A);
	RDATVM_WRITE(0xFE,0x54FE);
	RDATVM_WRITE(0xFE,0x2FF5);
	RDATVM_WRITE(0xFE,0x0AAB);
	RDATVM_WRITE(0xFE,0x0B7D);
	RDATVM_WRITE(0xFE,0x3A7F);
	RDATVM_WRITE(0xFE,0x01B1);
	RDATVM_WRITE(0xFE,0x93AB);
	RDATVM_WRITE(0xFE,0x0A7D);
	RDATVM_WRITE(0xFE,0x3B7F);
	RDATVM_WRITE(0xFE,0x01B1);
	RDATVM_WRITE(0xFE,0x9391);
	RDATVM_WRITE(0xFE,0xDB8F);
	RDATVM_WRITE(0xFE,0x09AB);
	RDATVM_WRITE(0xFE,0x097D);
	RDATVM_WRITE(0xFE,0xBE7F);
	RDATVM_WRITE(0xFE,0x01B1);
	RDATVM_WRITE(0xFE,0x9321);
	RDATVM_WRITE(0xFE,0x4222);
	RDATVM_WRITE(0xFE,0xE4F5);
	RDATVM_WRITE(0xFE,0x0BF5);
	RDATVM_WRITE(0xFE,0x0DF5);
	RDATVM_WRITE(0xFE,0x0E7D);
	RDATVM_WRITE(0xFE,0x967F);
	RDATVM_WRITE(0xFE,0x01B1);
	RDATVM_WRITE(0xFE,0xA58F);
	RDATVM_WRITE(0xFE,0x0A7F);
	RDATVM_WRITE(0xFE,0x02B1);
	RDATVM_WRITE(0xFE,0x7C75);
	RDATVM_WRITE(0xFE,0x0D00);
	RDATVM_WRITE(0xFE,0x750E);
	RDATVM_WRITE(0xFE,0x017D);
	RDATVM_WRITE(0xFE,0x947F);
	RDATVM_WRITE(0xFE,0x01B1);
	RDATVM_WRITE(0xFE,0xA58F);
	RDATVM_WRITE(0xFE,0x0C43);
	RDATVM_WRITE(0xFE,0x0C08);
	RDATVM_WRITE(0xFE,0xAB0C);
	RDATVM_WRITE(0xFE,0x7D94);
	RDATVM_WRITE(0xFE,0x7F01);
	RDATVM_WRITE(0xFE,0xB193);
	RDATVM_WRITE(0xFE,0x7B40);
	RDATVM_WRITE(0xFE,0x7DA1);
	RDATVM_WRITE(0xFE,0x7F01);
	RDATVM_WRITE(0xFE,0xB193);
	RDATVM_WRITE(0xFE,0x7B20);
	RDATVM_WRITE(0xFE,0x7A4E);
	RDATVM_WRITE(0xFE,0x7D01);
	RDATVM_WRITE(0xFE,0x7F03);
	RDATVM_WRITE(0xFE,0x91E3);
	RDATVM_WRITE(0xFE,0x8F09);
	RDATVM_WRITE(0xFE,0xE509);
	RDATVM_WRITE(0xFE,0xC394);
	RDATVM_WRITE(0xFE,0x1040);
	RDATVM_WRITE(0xFE,0x0575);
	RDATVM_WRITE(0xFE,0x0B01);
	RDATVM_WRITE(0xFE,0x8003);
	RDATVM_WRITE(0xFE,0x750B);
	RDATVM_WRITE(0xFE,0x02E4);
	RDATVM_WRITE(0xFE,0xF50D);
	RDATVM_WRITE(0xFE,0xF50E);
	RDATVM_WRITE(0xFE,0xE50B);
	RDATVM_WRITE(0xFE,0x6401);
	RDATVM_WRITE(0xFE,0x7026);
	RDATVM_WRITE(0xFE,0xE509);
	RDATVM_WRITE(0xFE,0xD395);
	RDATVM_WRITE(0xFE,0x2650);
	RDATVM_WRITE(0xFE,0x0DE5);
	RDATVM_WRITE(0xFE,0x09C3);
	RDATVM_WRITE(0xFE,0x9523);
	RDATVM_WRITE(0xFE,0x4006);
	RDATVM_WRITE(0xFE,0x7E00);
	RDATVM_WRITE(0xFE,0x7F01);
	RDATVM_WRITE(0xFE,0x8004);
	RDATVM_WRITE(0xFE,0x7E00);
	RDATVM_WRITE(0xFE,0x7F00);
	RDATVM_WRITE(0xFE,0x8E0D);
	RDATVM_WRITE(0xFE,0x8F0E);
	RDATVM_WRITE(0xFE,0xE50E);
	RDATVM_WRITE(0xFE,0x450D);
	RDATVM_WRITE(0xFE,0x602A);
	RDATVM_WRITE(0xFE,0x7B41);
	RDATVM_WRITE(0xFE,0x805F);
	RDATVM_WRITE(0xFE,0xE509);
	RDATVM_WRITE(0xFE,0xD395);
	RDATVM_WRITE(0xFE,0x2750);
	RDATVM_WRITE(0xFE,0x0DE5);
	RDATVM_WRITE(0xFE,0x09C3);
	RDATVM_WRITE(0xFE,0x9524);
	RDATVM_WRITE(0xFE,0x4006);
	RDATVM_WRITE(0xFE,0x7E00);
	RDATVM_WRITE(0xFE,0x7F01);
	RDATVM_WRITE(0xFE,0x8004);
	RDATVM_WRITE(0xFE,0x7E00);
	RDATVM_WRITE(0xFE,0x7F00);
	RDATVM_WRITE(0xFE,0x8E0D);
	RDATVM_WRITE(0xFE,0x8F0E);
	RDATVM_WRITE(0xFE,0xE50E);
	RDATVM_WRITE(0xFE,0x450D);
	RDATVM_WRITE(0xFE,0x6004);
	RDATVM_WRITE(0xFE,0x7B42);
	RDATVM_WRITE(0xFE,0x8039);
	RDATVM_WRITE(0xFE,0x7B02);
	RDATVM_WRITE(0xFE,0x7D91);
	RDATVM_WRITE(0xFE,0x7F01);
	RDATVM_WRITE(0xFE,0xB193);
	RDATVM_WRITE(0xFE,0xE4FB);
	RDATVM_WRITE(0xFE,0x7D90);
	RDATVM_WRITE(0xFE,0x7F01);
	RDATVM_WRITE(0xFE,0xB193);
	RDATVM_WRITE(0xFE,0x7D94);
	RDATVM_WRITE(0xFE,0x7F01);
	RDATVM_WRITE(0xFE,0xB1A5);
	RDATVM_WRITE(0xFE,0x8F0C);
	RDATVM_WRITE(0xFE,0x530C);
	RDATVM_WRITE(0xFE,0xF7AB);
	RDATVM_WRITE(0xFE,0x0C7D);
	RDATVM_WRITE(0xFE,0x947F);
	RDATVM_WRITE(0xFE,0x01B1);
	RDATVM_WRITE(0xFE,0x937F);
	RDATVM_WRITE(0xFE,0x02B1);
	RDATVM_WRITE(0xFE,0x7C7B);
	RDATVM_WRITE(0xFE,0x0C7D);
	RDATVM_WRITE(0xFE,0x917F);
	RDATVM_WRITE(0xFE,0x01B1);
	RDATVM_WRITE(0xFE,0x937B);
	RDATVM_WRITE(0xFE,0x507D);
	RDATVM_WRITE(0xFE,0x907F);
	RDATVM_WRITE(0xFE,0x01B1);
	RDATVM_WRITE(0xFE,0x937B);   
	RDATVM_WRITE(0xFE,0x437D);   
	RDATVM_WRITE(0xFE,0xA17F);   
	RDATVM_WRITE(0xFE,0x01B1);   
	RDATVM_WRITE(0xFE,0x937F);   
	RDATVM_WRITE(0xFE,0x01A1);   
	RDATVM_WRITE(0xFE,0x7CE4);   
	RDATVM_WRITE(0xFE,0xFFF5);   
	RDATVM_WRITE(0xFE,0x09F5);   
	RDATVM_WRITE(0xFE,0x0AF5);   
	RDATVM_WRITE(0xFE,0x0B7D);   
	RDATVM_WRITE(0xFE,0x3B0F);   
	RDATVM_WRITE(0xFE,0xB1A5);   
	RDATVM_WRITE(0xFE,0xEF54);   
	RDATVM_WRITE(0xFE,0x01F5);   
	RDATVM_WRITE(0xFE,0x097D);   
	RDATVM_WRITE(0xFE,0x3A7F);   
	RDATVM_WRITE(0xFE,0x01B1);   
	RDATVM_WRITE(0xFE,0xA5BF);   
	RDATVM_WRITE(0xFE,0xFF04);   
	RDATVM_WRITE(0xFE,0x7E01);   
	RDATVM_WRITE(0xFE,0x8002);   
	RDATVM_WRITE(0xFE,0x7E00);   
	RDATVM_WRITE(0xFE,0xE509);   
	RDATVM_WRITE(0xFE,0xB401);   
	RDATVM_WRITE(0xFE,0x047D);   
	RDATVM_WRITE(0xFE,0x0180);   
	RDATVM_WRITE(0xFE,0x027D);   
	RDATVM_WRITE(0xFE,0x00ED);   
	RDATVM_WRITE(0xFE,0x5EF5);   
	RDATVM_WRITE(0xFE,0x0A7D);   
	RDATVM_WRITE(0xFE,0x687F);   
	RDATVM_WRITE(0xFE,0x01B1);   
	RDATVM_WRITE(0xFE,0xA5EF);   
	RDATVM_WRITE(0xFE,0x1313);   
	RDATVM_WRITE(0xFE,0x1354);   
	RDATVM_WRITE(0xFE,0x03FE);   
	RDATVM_WRITE(0xFE,0x25E0);   
	RDATVM_WRITE(0xFE,0x25E0);   
	RDATVM_WRITE(0xFE,0xF509);   
	RDATVM_WRITE(0xFE,0x2480);   
	RDATVM_WRITE(0xFE,0xF509);   
	RDATVM_WRITE(0xFE,0xBE03);   
	RDATVM_WRITE(0xFE,0x047E);   
	RDATVM_WRITE(0xFE,0x0180);   
	RDATVM_WRITE(0xFE,0x027E);   
	RDATVM_WRITE(0xFE,0x00EE);   
	RDATVM_WRITE(0xFE,0x520A);   
	RDATVM_WRITE(0xFE,0x7D99);   
	RDATVM_WRITE(0xFE,0x7F01);   
	RDATVM_WRITE(0xFE,0xB1A5);   
	RDATVM_WRITE(0xFE,0xEFC4);   
	RDATVM_WRITE(0xFE,0x1354);   
	RDATVM_WRITE(0xFE,0x07F5);   
	RDATVM_WRITE(0xFE,0x0B53);   
	RDATVM_WRITE(0xFE,0x0B03);   
	RDATVM_WRITE(0xFE,0xE50B);   
	RDATVM_WRITE(0xFE,0x3333);   
	RDATVM_WRITE(0xFE,0x3354);   
	RDATVM_WRITE(0xFE,0xF8F5);   
	RDATVM_WRITE(0xFE,0x0BC3);   
	RDATVM_WRITE(0xFE,0xE509);   
	RDATVM_WRITE(0xFE,0x950B);    
	RDATVM_WRITE(0xFE,0xF509);   
	RDATVM_WRITE(0xFE,0x7D98);   
	RDATVM_WRITE(0xFE,0x7F01);   
	RDATVM_WRITE(0xFE,0xB1A5);   
	RDATVM_WRITE(0xFE,0x8F0B);   
	RDATVM_WRITE(0xFE,0x530B);   
	RDATVM_WRITE(0xFE,0x0FC3);   
	RDATVM_WRITE(0xFE,0xE509);   
	RDATVM_WRITE(0xFE,0x950B);   
	RDATVM_WRITE(0xFE,0xF509);   
	RDATVM_WRITE(0xFE,0xE50A);   
	RDATVM_WRITE(0xFE,0x6017);   
	RDATVM_WRITE(0xFE,0x7D1C);   
	RDATVM_WRITE(0xFE,0xE4FF);   
	RDATVM_WRITE(0xFE,0xB1A5);   
	RDATVM_WRITE(0xFE,0x8F0B);   
	RDATVM_WRITE(0xFE,0x530B);   
	RDATVM_WRITE(0xFE,0xFDAB);   
	RDATVM_WRITE(0xFE,0x0B7D);   
	RDATVM_WRITE(0xFE,0x1CE4);   
	RDATVM_WRITE(0xFE,0xFFB1);   
	RDATVM_WRITE(0xFE,0x937F);   
	RDATVM_WRITE(0xFE,0x0AA1);   
	RDATVM_WRITE(0xFE,0x7CE5);   
	RDATVM_WRITE(0xFE,0x09C3);   
	RDATVM_WRITE(0xFE,0x9474);   
	RDATVM_WRITE(0xFE,0x5013);  
	RDATVM_WRITE(0xFE,0x7D1C);   
	RDATVM_WRITE(0xFE,0xE4FF);   
	RDATVM_WRITE(0xFE,0xB1A5);   
	RDATVM_WRITE(0xFE,0x8F0B);   
	RDATVM_WRITE(0xFE,0x430B);    
	RDATVM_WRITE(0xFE,0x02AB);   
	RDATVM_WRITE(0xFE,0x0B7D);   
	RDATVM_WRITE(0xFE,0x1CE4);   
	RDATVM_WRITE(0xFE,0xFFB1);   
	RDATVM_WRITE(0xFE,0x93E5);   
	RDATVM_WRITE(0xFE,0x09D3);   
	RDATVM_WRITE(0xFE,0x9480);   
	RDATVM_WRITE(0xFE,0x4013);   
	RDATVM_WRITE(0xFE,0x7D1C);   
	RDATVM_WRITE(0xFE,0xE4FF);   
	RDATVM_WRITE(0xFE,0xB1A5);   
	RDATVM_WRITE(0xFE,0x8F0B);   
	RDATVM_WRITE(0xFE,0x530B);   
	RDATVM_WRITE(0xFE,0xFDAB);   
	RDATVM_WRITE(0xFE,0x0B7D);   
	RDATVM_WRITE(0xFE,0x1CE4);   
	RDATVM_WRITE(0xFE,0xFFB1);   
	RDATVM_WRITE(0xFE,0x9322);   
	RDATVM_WRITE(0xFE,0xE4F5);   
	RDATVM_WRITE(0xFE,0x087B);   
	RDATVM_WRITE(0xFE,0x107D);   
	RDATVM_WRITE(0xFE,0xA17F);   
	RDATVM_WRITE(0xFE,0x01B1);   
	RDATVM_WRITE(0xFE,0x937F);   
	RDATVM_WRITE(0xFE,0x02B1);   
	RDATVM_WRITE(0xFE,0x7C7D);   
	RDATVM_WRITE(0xFE,0x3A7F);  
	RDATVM_WRITE(0xFE,0x01B1);   
	RDATVM_WRITE(0xFE,0xA58F);   
	RDATVM_WRITE(0xFE,0x087D);   
	RDATVM_WRITE(0xFE,0x3B7F);   
	RDATVM_WRITE(0xFE,0x01B1);    
	RDATVM_WRITE(0xFE,0xA5AB);   
	RDATVM_WRITE(0xFE,0x0743);   
	RDATVM_WRITE(0xFE,0x0302);   
	RDATVM_WRITE(0xFE,0x7D3B);   
	RDATVM_WRITE(0xFE,0x7F01);   
	RDATVM_WRITE(0xFE,0xB193);   
	RDATVM_WRITE(0xFE,0xAB08);   
	RDATVM_WRITE(0xFE,0x7D3A);   
	RDATVM_WRITE(0xFE,0x7F01);   
	RDATVM_WRITE(0xFE,0xB193);   
	RDATVM_WRITE(0xFE,0x7DBB);   
	RDATVM_WRITE(0xFE,0x7F01);   
	RDATVM_WRITE(0xFE,0xB1A5);   
	RDATVM_WRITE(0xFE,0x8F26);   
	RDATVM_WRITE(0xFE,0x7DBA);   
	RDATVM_WRITE(0xFE,0x7F01);   
	RDATVM_WRITE(0xFE,0xB1A5);   
	RDATVM_WRITE(0xFE,0x8F23);   
	RDATVM_WRITE(0xFE,0x7DBD);   
	RDATVM_WRITE(0xFE,0x7F01);   
	RDATVM_WRITE(0xFE,0xB1A5);   
	RDATVM_WRITE(0xFE,0x8F27);   
	RDATVM_WRITE(0xFE,0x7DBC);   
	RDATVM_WRITE(0xFE,0x7F01);   
	RDATVM_WRITE(0xFE,0xB1A5);   
	RDATVM_WRITE(0xFE,0x8F24);   
	RDATVM_WRITE(0xFE,0x7D71);  
	RDATVM_WRITE(0xFE,0x7F01);   
	RDATVM_WRITE(0xFE,0xB1A5);   
	RDATVM_WRITE(0xFE,0x8F29);   
	RDATVM_WRITE(0xFE,0x7D70);   
	RDATVM_WRITE(0xFE,0x7F01);    
	RDATVM_WRITE(0xFE,0xB1A5);   
	RDATVM_WRITE(0xFE,0x8F25);   
	RDATVM_WRITE(0xFE,0x7D72);   
	RDATVM_WRITE(0xFE,0x7F01);   
	RDATVM_WRITE(0xFE,0xB1A5);   
	RDATVM_WRITE(0xFE,0x8F28);   
	RDATVM_WRITE(0xFE,0x7BD0);   
	RDATVM_WRITE(0xFE,0x7A07);   
	RDATVM_WRITE(0xFE,0x91DF);   
	RDATVM_WRITE(0xFE,0xAB07);   
	RDATVM_WRITE(0xFE,0xEB13);   
	RDATVM_WRITE(0xFE,0x1354);   
	RDATVM_WRITE(0xFE,0x3FF5);   
	RDATVM_WRITE(0xFE,0x1FF5);   
	RDATVM_WRITE(0xFE,0x20F5);   
	RDATVM_WRITE(0xFE,0x21F5);   
	RDATVM_WRITE(0xFE,0x227B);   
	RDATVM_WRITE(0xFE,0x207D);   
	RDATVM_WRITE(0xFE,0xA17F);   
	RDATVM_WRITE(0xFE,0x01B1);   
	RDATVM_WRITE(0xFE,0x937F);   
	RDATVM_WRITE(0xFE,0x02B1);   
	RDATVM_WRITE(0xFE,0x7C51);   
	RDATVM_WRITE(0xFE,0xE911);   
	RDATVM_WRITE(0xFE,0x0351);   
	RDATVM_WRITE(0xFE,0x0080);   
	RDATVM_WRITE(0xFE,0xEC78);  
	RDATVM_WRITE(0xFE,0x7FE4);
	RDATVM_WRITE(0xFE,0xF6D8);
	RDATVM_WRITE(0xFE,0xFD75);
	RDATVM_WRITE(0xFE,0x8129);
	RDATVM_WRITE(0xFE,0x0204);
	RDATVM_WRITE(0xFE,0x9602);
	RDATVM_WRITE(0xFE,0x03BE);
	RDATVM_WRITE(0xFE,0xE493);
	RDATVM_WRITE(0xFE,0xA3F8);
	RDATVM_WRITE(0xFE,0xE493);
	RDATVM_WRITE(0xFE,0xA340);
	RDATVM_WRITE(0xFE,0x03F6);
	RDATVM_WRITE(0xFE,0x8001);
	RDATVM_WRITE(0xFE,0xF208);
	RDATVM_WRITE(0xFE,0xDFF4);
	RDATVM_WRITE(0xFE,0x8029);
	RDATVM_WRITE(0xFE,0xE493);
	RDATVM_WRITE(0xFE,0xA3F8);
	RDATVM_WRITE(0xFE,0x5407);
	RDATVM_WRITE(0xFE,0x240C);
	RDATVM_WRITE(0xFE,0xC8C3);
	RDATVM_WRITE(0xFE,0x33C4);
	RDATVM_WRITE(0xFE,0x540F);
	RDATVM_WRITE(0xFE,0x4420);
	RDATVM_WRITE(0xFE,0xC883);
	RDATVM_WRITE(0xFE,0x4004);
	RDATVM_WRITE(0xFE,0xF456);
	RDATVM_WRITE(0xFE,0x8001);
	RDATVM_WRITE(0xFE,0x46F6);
	RDATVM_WRITE(0xFE,0xDFE4);
	RDATVM_WRITE(0xFE,0x800B);
	RDATVM_WRITE(0xFE,0x0102);
	RDATVM_WRITE(0xFE,0x0408);
	RDATVM_WRITE(0xFE,0x1020);
	RDATVM_WRITE(0xFE,0x4080);
	RDATVM_WRITE(0xFE,0x9005);
	RDATVM_WRITE(0xFE,0x5AE4);
	RDATVM_WRITE(0xFE,0x7E01);
	RDATVM_WRITE(0xFE,0x9360);
	RDATVM_WRITE(0xFE,0xBCA3);
	RDATVM_WRITE(0xFE,0xFF54);
	RDATVM_WRITE(0xFE,0x3F30);
	RDATVM_WRITE(0xFE,0xE509);
	RDATVM_WRITE(0xFE,0x541F);
	RDATVM_WRITE(0xFE,0xFEE4);
	RDATVM_WRITE(0xFE,0x93A3);
	RDATVM_WRITE(0xFE,0x6001);
	RDATVM_WRITE(0xFE,0x0ECF);
	RDATVM_WRITE(0xFE,0x54C0);
	RDATVM_WRITE(0xFE,0x25E0);
	RDATVM_WRITE(0xFE,0x60A8);
	RDATVM_WRITE(0xFE,0x40B8);
	RDATVM_WRITE(0xFE,0xE493);
	RDATVM_WRITE(0xFE,0xA3FA);
	RDATVM_WRITE(0xFE,0xE493);
	RDATVM_WRITE(0xFE,0xA3F8);
	RDATVM_WRITE(0xFE,0xE493);
	RDATVM_WRITE(0xFE,0xA3C8);
	RDATVM_WRITE(0xFE,0xC582);
	RDATVM_WRITE(0xFE,0xC8CA);
	RDATVM_WRITE(0xFE,0xC583);
	RDATVM_WRITE(0xFE,0xCAF0);
	RDATVM_WRITE(0xFE,0xA3C8);
	RDATVM_WRITE(0xFE,0xC582);
	RDATVM_WRITE(0xFE,0xC8CA);
	RDATVM_WRITE(0xFE,0xC583);
	RDATVM_WRITE(0xFE,0xCADF);
	RDATVM_WRITE(0xFE,0xE9DE);
	RDATVM_WRITE(0xFE,0xE780);
	RDATVM_WRITE(0xFE,0xBEAB);
	RDATVM_WRITE(0xFE,0x17AA);
	RDATVM_WRITE(0xFE,0x167D);
	RDATVM_WRITE(0xFE,0x017F);
	RDATVM_WRITE(0xFE,0x018F);
	RDATVM_WRITE(0xFE,0x188D);
	RDATVM_WRITE(0xFE,0x198A);
	RDATVM_WRITE(0xFE,0x1A8B);
	RDATVM_WRITE(0xFE,0x1BE4);
	RDATVM_WRITE(0xFE,0xFFF5);
	RDATVM_WRITE(0xFE,0x1CE5);
	RDATVM_WRITE(0xFE,0x18B4);
	RDATVM_WRITE(0xFE,0x0130);
	RDATVM_WRITE(0xFE,0xE4F5);
	RDATVM_WRITE(0xFE,0x1DF5);
	RDATVM_WRITE(0xFE,0x1EC3);
	RDATVM_WRITE(0xFE,0xE51E);
	RDATVM_WRITE(0xFE,0x951B);
	RDATVM_WRITE(0xFE,0xE51A);
	RDATVM_WRITE(0xFE,0x6480);
	RDATVM_WRITE(0xFE,0xF8E5);
	RDATVM_WRITE(0xFE,0x1D64);
	RDATVM_WRITE(0xFE,0x8098);
	RDATVM_WRITE(0xFE,0x5018);
	RDATVM_WRITE(0xFE,0x7D2B);
	RDATVM_WRITE(0xFE,0x7F01);
	RDATVM_WRITE(0xFE,0xB1A5);
	RDATVM_WRITE(0xFE,0xEFD3);
	RDATVM_WRITE(0xFE,0x951C);
	RDATVM_WRITE(0xFE,0x4002);
	RDATVM_WRITE(0xFE,0x8F1C);
	RDATVM_WRITE(0xFE,0x051E);
	RDATVM_WRITE(0xFE,0xE51E);
	RDATVM_WRITE(0xFE,0x70DB);
	RDATVM_WRITE(0xFE,0x051D);
	RDATVM_WRITE(0xFE,0x80D7);
	RDATVM_WRITE(0xFE,0xAF1C);
	RDATVM_WRITE(0xFE,0xE518);
	RDATVM_WRITE(0xFE,0xB403);
	RDATVM_WRITE(0xFE,0x30E4);
	RDATVM_WRITE(0xFE,0xF51D);
	RDATVM_WRITE(0xFE,0xF51E);
	RDATVM_WRITE(0xFE,0xC3E5);
	RDATVM_WRITE(0xFE,0x1E95);
	RDATVM_WRITE(0xFE,0x1BE5);
	RDATVM_WRITE(0xFE,0x1A64);
	RDATVM_WRITE(0xFE,0x80F8);
	RDATVM_WRITE(0xFE,0xE51D);
	RDATVM_WRITE(0xFE,0x6480);
	RDATVM_WRITE(0xFE,0x9850);
	RDATVM_WRITE(0xFE,0x187D);
	RDATVM_WRITE(0xFE,0x8D7F);
	RDATVM_WRITE(0xFE,0x01B1);
	RDATVM_WRITE(0xFE,0xA5EF);
	RDATVM_WRITE(0xFE,0xD395);
	RDATVM_WRITE(0xFE,0x1C40);
	RDATVM_WRITE(0xFE,0x028F);
	RDATVM_WRITE(0xFE,0x1C05);
	RDATVM_WRITE(0xFE,0x1EE5);
	RDATVM_WRITE(0xFE,0x1E70);
	RDATVM_WRITE(0xFE,0xDB05);
	RDATVM_WRITE(0xFE,0x1D80);
	RDATVM_WRITE(0xFE,0xD7AF);
	RDATVM_WRITE(0xFE,0x1C22);
	RDATVM_WRITE(0xFE,0x0126);
	RDATVM_WRITE(0xFE,0x1D01);
	RDATVM_WRITE(0xFE,0x2300);
	RDATVM_WRITE(0xFE,0x0127);
	RDATVM_WRITE(0xFE,0x0001);
	RDATVM_WRITE(0xFE,0x2400);
	RDATVM_WRITE(0xFE,0x0129);
	RDATVM_WRITE(0xFE,0x8C01);
	RDATVM_WRITE(0xFE,0x2536);
	RDATVM_WRITE(0xFE,0x0128);
	RDATVM_WRITE(0xFE,0x6001);
	RDATVM_WRITE(0xFE,0x1F00);
	RDATVM_WRITE(0xFE,0x0120);
	RDATVM_WRITE(0xFE,0x0001);
	RDATVM_WRITE(0xFE,0x2100);
	RDATVM_WRITE(0xFE,0x0122);
	RDATVM_WRITE(0xFE,0x0000);
	RDATVM_WRITE(0xFE,0x7DFA);
	RDATVM_WRITE(0xFE,0x7C00);
	RDATVM_WRITE(0xFE,0xAE07);
	RDATVM_WRITE(0xFE,0x1FEE);
	RDATVM_WRITE(0xFE,0x600C);
	RDATVM_WRITE(0xFE,0xED1D);
	RDATVM_WRITE(0xFE,0xAA04);
	RDATVM_WRITE(0xFE,0x7001);
	RDATVM_WRITE(0xFE,0x1C4A);
	RDATVM_WRITE(0xFE,0x60F0);
	RDATVM_WRITE(0xFE,0x80F4);
	RDATVM_WRITE(0xFE,0x2275);
	RDATVM_WRITE(0xFE,0x90FF);
	RDATVM_WRITE(0xFE,0x8F80);
	RDATVM_WRITE(0xFE,0xC2B5);
	RDATVM_WRITE(0xFE,0xD2B5);
	RDATVM_WRITE(0xFE,0x8D90);
	RDATVM_WRITE(0xFE,0x8B80);
	RDATVM_WRITE(0xFE,0xC2B5);
	RDATVM_WRITE(0xFE,0xD2B5);
	RDATVM_WRITE(0xFE,0x2275);
	RDATVM_WRITE(0xFE,0x90FF);
	RDATVM_WRITE(0xFE,0x8F80);
	RDATVM_WRITE(0xFE,0xC2B5);
	RDATVM_WRITE(0xFE,0xD2B5);
	RDATVM_WRITE(0xFE,0x8D90);
	RDATVM_WRITE(0xFE,0xAF90);
	RDATVM_WRITE(0xFE,0x2200);
	RDATVM_WRITE(0xFE,0x0000);
	RDATVM_WRITE(0xFE,0x0000);
	RDATVM_WRITE(0xFE,0x0000);
	RDATVM_WRITE(0xFE,0x0000);
	                    
	RDADelayMs(200);    
	RDATVM_WRITE(0x30,0x011C);
	RDATVM_WRITE(0x30,0x811C);
}                   


UINT32  devTunerInit(void)
{
	RDATVPowerOn();
#if 1
	RDATVLnaMode(DIFF);				// 旧模块
#else
	RDATVLnaMode(SINGLE);			// 新模块
#endif
	RDATVSetMode(DTV);
	RDATVSetIfOutMode(manual);
	RDATVSetAdcIndex(ADC_162);
	RDATVSetModeOutT(DTV_IF);   
	RDATVsetdemod(X);
	RDATVGetI2VTH();
	RDATVDownLoad();
	return 1;    
}                   
UINT32 devDigitalTuner_SetFreq (UINT32 freq , UINT32 eBandWidth )
{	    
//	eLIBs_printf("_SetFreq(%d, %d)\n", freq, eBandWidth);

    RDATVSet51Sta(0);
	RDATVSetFreq(freq,eBandWidth);
 	RDATVSet51Sta(1); 
	return 1;
	
}

