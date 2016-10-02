/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           ir key driver
*
*						        (c) Copyright 2006-2007, Steven.ZGJ China
*											All	Rights Reserved
*
* File    : bsp_rk.c
* By      : 
* Version : V1.00
*********************************************************************************************************
*/

#include "epdk.h"
#include "bsp_rk.h"



#define  RKC_REGS_BASE                0xf1C25000

#define  RK_REG_o_CTRL0               0x00
#define  RK_REG_o_CTRL1               0x04
#define  RK_REG_o_CTRL2  	      0X08
#define  RK_REG_o_CTRL3		      0x0c

#define  RK_REG_o_INT_FIFOC          0x10  
#define  RK_REG_o_INT_FIFOS          0x14

#define  RK_REG_o_TP_TPR              0x18
#define  RK_REG_o_TEMP_DATA           0X20
#define  RK_REG_o_DATA                0x24

#define  RKC_AD_REGS_BASE             0xf1C20800
#define  RKC_AD_REGS_90               0x90



static   __u32      save_ctrl0;
static   __u32      save_ctrl1;
static   __u32      save_int_fifo;

__s32   RK_Init(void)
{
    __u32       reg_value;


        // REG_CTRL0 for clock
	reg_value = RK_READ_REG(RK_REG_o_CTRL0);
	save_ctrl0 = reg_value;	
	
	reg_value = reg_value | (0x09 << 16);   // set ADC Sample Frequency Divider
	reg_value = reg_value | (0x01 << 20);   // set ADC Clock Divider
	reg_value = reg_value | (0x00 << 22);   // set ADC Clock Source Select HOSC

	RK_WRITE_REG(RK_REG_o_CTRL0, reg_value);

	// REG_CTRL1
	reg_value = RK_READ_REG(RK_REG_o_CTRL1);
	save_ctrl1 = reg_value;

//	reg_value = reg_value | (0x04 << 0);  //set 4 channel select robin round
	reg_value = reg_value | (0x01 << 3);  //set ADC	
//	reg_value = reg_value | (0x00 << 4);  //set TP Mode Function disable
	reg_value = reg_value | (0x01 << 4);  //set TP Mode Function enable
	reg_value = reg_value & ~(0x01 << 5); //set TP double point disable

	RK_WRITE_REG(RK_REG_o_CTRL1, reg_value);
		

	// 初始化REG_INIT_FIFOC
	reg_value = RK_READ_REG(RK_REG_o_INT_FIFOC);
	save_int_fifo = reg_value;

	reg_value = reg_value | (0x00 << 0);        // FIRST Touch IRQ DISABLE
	reg_value = reg_value | (0x00 << 1);        // LAST Touch IRQ DISABLE
	reg_value = reg_value | (0x01 << 16);       // FIFO DATA IRQ ENABLE
	reg_value = reg_value | (0x01 << 17);       // FIFO OVER RUN IRQ ENABLE
	RK_WRITE_REG(RK_REG_o_INT_FIFOC, reg_value);	





/*
	// 初始化REG_CTRL
	reg_value = RK_READ_REG(RK_REG_o_CTRL);
	save_ctrl = reg_value;
	reg_value = reg_value & ~(0x01 << 0);       // set tp disable
	reg_value = reg_value | (0x01 << 1);        // set ADC enable
	reg_value = reg_value | (0x01 << 2);        // set ADC select
	reg_value = reg_value | ~(0x00 << 4);        // set 4 channel select robin round
	reg_value = reg_value | (0x0f << 13);       // set CLK_IN/32
//	reg_value = reg_value | (0x01 << 17);       // HOSC(24MHz)
	reg_value = reg_value & ~(0x03 << 20);      // ADC CLK_IN (CLK/2)

	RK_WRITE_REG(RK_REG_o_CTRL, reg_value);



	// 初始化REG_INT
	reg_value = RK_READ_REG(RK_REG_o_INT);
	save_int  = reg_value;
	reg_value = reg_value & ~(RK_DATA_IRQ | RK_LTOUCH_IRQ | RK_FTOUCH_IRQ | RK_FIFO_OV_IRQ);  // disable int
	RK_WRITE_REG(RK_REG_o_INT, reg_value);

	// 初始化REG_FIFOC
	reg_value = RK_READ_REG(RK_REG_o_FIFOC);
	save_fifo = reg_value;
	reg_value = reg_value | (0x01 << 0);        // FIRST Touch IRQ DISABLE
	reg_value = reg_value | (0x01 << 1);        // LAST Touch IRQ DISABLE
	reg_value = reg_value | (0x01 << 2);        // TOUCH PANEL IDLE
	reg_value = reg_value | (0x01 << 4);        // FIFO DATA IRQ ENABLE
	reg_value = reg_value | (0x01 << 5);        // FIFO OVER RUN IRQ ENABLE
	RK_WRITE_REG(RK_REG_o_FIFOC, reg_value);

	
*/

	return EPDK_OK;
}


void print_init()
{
	__u32       reg_value;
	
	reg_value = RK_READ_REG(0x00);
	eLIBs_printf("0x00 = %x \n",reg_value);
	reg_value = RK_READ_REG(0x04);
	eLIBs_printf("0x04 = %x \n",reg_value);
	reg_value = RK_READ_REG(0x08);
	eLIBs_printf("0x08 = %x \n",reg_value);
	reg_value = RK_READ_REG(0x0c);
	eLIBs_printf("0x0c = %x \n",reg_value);

	reg_value = RK_READ_REG(0x10);
	eLIBs_printf("0x10 = %x \n",reg_value);
	reg_value = RK_READ_REG(0x14);
	eLIBs_printf("0x14 = %x \n",reg_value);
	reg_value = RK_READ_REG(0x18);
	eLIBs_printf("0x18 = %x \n",reg_value);
	reg_value = RK_READ_REG(0x1c);
	eLIBs_printf("0x1c = %x \n",reg_value);

	reg_value = RK_READ_REG(0x20);
	eLIBs_printf("0x20 = %x \n",reg_value);	
}



/*void print_init()
{
	__u32       reg_value;
	reg_value = RK_READ_REG(0x00);
	eLIBs_printf("&&&&&&&&&&&&&& INIT 0xF1C24800 + 0x00 = %x &&&&&&&&&&&&&\n",reg_value);
	reg_value = RK_READ_REG(0x04);
	eLIBs_printf("&&&&&&&&&&&&&& INIT 0xF1C24800 + 0x04 = %x &&&&&&&&&&&&&\n",reg_value);
	reg_value = RK_READ_REG(0x10);
	eLIBs_printf("&&&&&&&&&&&&&& INIT 0xF1C24800 + 0x10 = %x &&&&&&&&&&&&&\n",reg_value);
	reg_value = RK_READ_REG(0x14);
	eLIBs_printf("&&&&&&&&&&&&&& INIT 0xF1C24800 + 0x14 = %x &&&&&&&&&&&&&\n",reg_value);
	reg_value = RK_READ_REG(0x18);
	eLIBs_printf("&&&&&&&&&&&&&& INIT 0xF1C24800 + 0x18 = %x &&&&&&&&&&&&&\n",reg_value);
	eLIBs_printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	reg_value = RK_AD_READ_REG(0x90);
	eLIBs_printf("&&&&&&&&&&&&&& INIT 0xF1C20800 + 0x90 = %x &&&&&&&&&&&&&\n",reg_value);


}

void print_init_later()
{	
		__u32       reg_value;
		reg_value = RK_READ_REG(0x00);
		eLIBs_printf("&&&&&&&&&&&&&& INIT LATER 0xF1C24800 + 0x00 = %x &&&&&&&&&&&&&\n",reg_value);
		reg_value = RK_READ_REG(0x04);
		eLIBs_printf("&&&&&&&&&&&&&& INIT LATER 0xF1C24800 + 0x04 = %x &&&&&&&&&&&&&\n",reg_value);
		reg_value = RK_READ_REG(0x10);
		eLIBs_printf("&&&&&&&&&&&&&& INIT LATER 0xF1C24800 + 0x10 = %x &&&&&&&&&&&&&\n",reg_value);
		reg_value = RK_READ_REG(0x14);
		eLIBs_printf("&&&&&&&&&&&&&& INIT LATER 0xF1C24800 + 0x14 = %x &&&&&&&&&&&&&\n",reg_value);
		reg_value = RK_READ_REG(0x18);
		eLIBs_printf("&&&&&&&&&&&&&& INIT LATER 0xF1C24800 + 0x18 = %x &&&&&&&&&&&&&\n",reg_value);
		eLIBs_printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
		reg_value = RK_AD_READ_REG(0x90);
		eLIBs_printf("&&&&&&&&&&&&&& INIT LATER 0xF1C20800 + 0x90 = %x &&&&&&&&&&&&&\n",reg_value);

}*/


__s32   RK_Exit(void)
{
	RK_WRITE_REG(RK_REG_o_CTRL0 , save_ctrl0);
	RK_WRITE_REG(RK_REG_o_CTRL1 , save_ctrl1);
	RK_WRITE_REG(RK_REG_o_INT_FIFOC, save_int_fifo);
    
    return EPDK_OK;
}



void    RK_ClearFifo(void)
{
    __s32       reg_value;
    
    reg_value = RK_READ_REG(RK_REG_o_INT_FIFOC);
    reg_value = reg_value | (0x1 << 4);        // clear fifo
    RK_WRITE_REG(RK_REG_o_INT_FIFOC, reg_value);
}

__s32   RK_EnableInt(__u32 int_no)
{
    __s32       reg_value;
    
    reg_value = RK_READ_REG(RK_REG_o_INT_FIFOC);
    reg_value = reg_value | int_no;
    RK_WRITE_REG(RK_REG_o_INT_FIFOC, reg_value);
    
    return EPDK_OK;
}


__s32   RK_DisableInt(__u32 int_no)
{
    __s32       reg_value;
    
    reg_value = RK_READ_REG(RK_REG_o_INT_FIFOC);
    reg_value = reg_value & (~int_no);
    RK_WRITE_REG(RK_REG_o_INT_FIFOC, reg_value);

    return EPDK_OK;
}

__s32   RK_GetData(__s32 *data, __u32 data_cnt)
{
    __s32       i;

    for(i = 0; i < data_cnt; i++)
    {
        data[i] = RK_READ_REG(RK_REG_o_DATA) & 0xfff;   // 读取数据
    }
    
    return EPDK_OK;
}



__s32   RK_QueryIntPending(void)
{
    __s32       ret;
    
    ret = RK_READ_REG(RK_REG_o_INT_FIFOS);
	
	return ret;
}

void    RK_ClearIntPending(__s32 pending)
{
    RK_WRITE_REG(RK_REG_o_INT_FIFOS, pending);
}

__s32   RK_SetSampleRate(__rk_sample_rate_e rate)
{
    __s32       reg_value;
    
    switch(rate)
    {
        case RK_SAMPLE_RATE_01K:
        {
            reg_value = RK_READ_REG(RK_REG_o_CTRL1);
            reg_value = reg_value & ~(0x0f << 16);
            reg_value = reg_value | (0x0d << 16);
            RK_WRITE_REG(RK_REG_o_CTRL1, reg_value);
            
            break;
        }
        case RK_SAMPLE_RATE_02K:
        {
            reg_value = RK_READ_REG(RK_REG_o_CTRL1);
            reg_value = reg_value & ~(0x0f << 16);
            reg_value = reg_value | (0x0c << 16);
            RK_WRITE_REG(RK_REG_o_CTRL1, reg_value);
            
            break;
        }
        case RK_SAMPLE_RATE_04K:
        {
            reg_value = RK_READ_REG(RK_REG_o_CTRL1);
            reg_value = reg_value & ~(0x0f << 16);
            reg_value = reg_value | (0x0b << 16);
            RK_WRITE_REG(RK_REG_o_CTRL1, reg_value);
            
            break;
        }
        case RK_SAMPLE_RATE_08K:
        {
            reg_value = RK_READ_REG(RK_REG_o_CTRL1);
            reg_value = reg_value & ~(0x0f << 16);
            reg_value = reg_value | (0x0a << 16);
            RK_WRITE_REG(RK_REG_o_CTRL1, reg_value);
            
            break;
        }
        case RK_SAMPLE_RATE_16K:
        {
            reg_value = RK_READ_REG(RK_REG_o_CTRL1);
            reg_value = reg_value & ~(0x0f << 16);
            reg_value = reg_value | (0x09 << 16);
            RK_WRITE_REG(RK_REG_o_CTRL1, reg_value);
            
            break;
        }
        default:
        {
            reg_value = RK_READ_REG(RK_REG_o_CTRL1);
            reg_value = reg_value & ~(0x0f << 16);
            reg_value = reg_value | (0x07 << 16);
            RK_WRITE_REG(RK_REG_o_CTRL1, reg_value);
            
            break;
        }
    }
    
    return EPDK_OK;
}

__s32   RK_SetFifoDepth(__rk_fifo_depth_e depth)
{
    __s32       reg_value;
    
    switch(depth)
    {
	case RK_FIFO_DEPTH_04:
	{
		reg_value = RK_READ_REG(RK_REG_o_INT_FIFOC);
		reg_value = reg_value &~(0x1f<<8);
		reg_value = reg_value |((0x4 - 1)<<8);
		RK_WRITE_REG(RK_REG_o_INT_FIFOC, reg_value);
		break;
	}
	case RK_FIFO_DEPTH_08:
	{
		__s8 temp1,temp2;
		reg_value = RK_READ_REG(RK_REG_o_INT_FIFOC);
		reg_value = reg_value &~(0x1f<<8);
		temp1 = 0x1f<<16;
		reg_value = reg_value |((0x8 - 1)<<8);
		temp2 = ((0x8 -1)<<8);
		RK_WRITE_REG(RK_REG_o_INT_FIFOC, reg_value);
		break;
	}	
	case RK_FIFO_DEPTH_16:
	{
		reg_value = RK_READ_REG(RK_REG_o_INT_FIFOC);
		reg_value = reg_value &~(0x1f<<8);
		reg_value = reg_value |((0x10 - 1)<<8);
		RK_WRITE_REG(RK_REG_o_INT_FIFOC, reg_value);
		break;
	}
	case RK_FIFO_DEPTH_32:
	{
		reg_value = RK_READ_REG(RK_REG_o_INT_FIFOC);
		reg_value = reg_value &~(0x1f<<8);
		reg_value = reg_value |((0x20 - 1)<<8);
		RK_WRITE_REG(RK_REG_o_INT_FIFOC, reg_value);
		break;
	}
	default:
	{
		reg_value = RK_READ_REG(RK_REG_o_INT_FIFOC);
		reg_value = reg_value &~(0x1f<<8);
		reg_value = reg_value |((0x8 - 1)<<8);
		RK_WRITE_REG(RK_REG_o_INT_FIFOC, reg_value);
		break;
	}	
	}
    
    return EPDK_OK;
}

__s32   RK_SelectChannel(__rk_channel_e channel)
{
    __u32       reg_value;

    switch(channel)
    {
        case RK_CHANNEL_X1:
        {
        	reg_value = RK_READ_REG(RK_REG_o_CTRL1);
        	reg_value = reg_value & ~(0x07 << 0);
        	reg_value = reg_value | (0x00 << 0);        
        	RK_WRITE_REG(RK_REG_o_CTRL1, reg_value);
		break;
        }
        case RK_CHANNEL_X2:
        {
        	reg_value = RK_READ_REG(RK_REG_o_CTRL1);
        	reg_value = reg_value & ~(0x07 << 0);
        	reg_value = reg_value | (0x01 << 0);        
        	RK_WRITE_REG(RK_REG_o_CTRL1, reg_value);
		break;
        }
        case RK_CHANNEL_Y1:
        {
        	reg_value = RK_READ_REG(RK_REG_o_CTRL1);
        	reg_value = reg_value & ~(0x07 << 0);
        	reg_value = reg_value | (0x02 << 0);        
        	RK_WRITE_REG(RK_REG_o_CTRL1, reg_value);
		break;
        }
        case RK_CHANNEL_Y2:
        {
        	reg_value = RK_READ_REG(RK_REG_o_CTRL1);
        	reg_value = reg_value & ~(0x07 << 0);
        	reg_value = reg_value | (0x03 << 0);        
        	RK_WRITE_REG(RK_REG_o_CTRL1, reg_value);
		break;
        }
        default:
        {
            break;
        }
    }
    
    return EPDK_OK;
}

