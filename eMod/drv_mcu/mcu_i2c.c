

#include "epdk.h"
#include "drv_mcu_i.h"



//1, out;  0, input
//SDA, direction , output = SDADIROUT
#define  SDADIROUT()      esPINS_SetPinIO(h_pin_aux_i2c_SDA, 1, NULL)
#define  SDADIRIN()       esPINS_SetPinIO(h_pin_aux_i2c_SDA, 0, NULL)

#define  SCLDIROUT()      esPINS_SetPinIO(h_pin_aux_i2c_SCL, 1, NULL)
#define  SCLDIRIN()       esPINS_SetPinIO(h_pin_aux_i2c_SCL, 0, NULL)

#define  I2C_DATA_HIGH()  esPINS_WritePinData(h_pin_aux_i2c_SDA, 1, NULL)
#define  I2C_DATA_LOW()   esPINS_WritePinData(h_pin_aux_i2c_SDA, 0, NULL)
#define  I2C_CLK_HIGH()   esPINS_WritePinData(h_pin_aux_i2c_SCL, 1, NULL)
#define  I2C_CLK_LOW()    esPINS_WritePinData(h_pin_aux_i2c_SCL, 0, NULL)
#define  I2C_DATA_READ()  esPINS_ReadPinData(h_pin_aux_i2c_SDA, NULL)
ES_FILE   *hiic;







/***********************************************************************************/
void SoftDelay(__u8 n)
{
	//esKRNL_TimeDly(1);
    while (n--);
        //asm("nop");
}

__u16 mcu_ReadReg(__u8 addr,__u8 *data,__u16 n_byte)
{
  	__twi_dev_para_ex_t  piic;        
	__s32 ret = 0;
	__u8 add1=addr;
    eLIBs_memset(&piic, 0, sizeof(__twi_dev_para_ex_t));


	piic.slave_addr         = (DVR_MCU_ADD>>1); //设备地址，值为7bit地址，不含读写标志位
	piic.slave_addr_flag    = 0;
	piic.byte_addr          = &add1;    //如果访问设备的寄存器地址不是8位的话，需要传入最高byte的地址， 这里以8位为例，所以直接取地址
	piic.byte_addr_width    = 1;            //设备寄存器地址的长度，以byte为单位，这里以8位为例，所以取1, 如果>1，填所需长度，最大到3
	piic.data               = data;         //数据buffer
	piic.byte_count         = n_byte;          //读取数据长度


	//如果不是标准iic，即不发restart的读操作
	ret = eLIBs_fioctrl(hiic, TWI_READ_EX_NO_RS,  0, (void *)&piic);

	if (ret == EPDK_FAIL)
	{
		//FAIL 处理
	  eDbug("IIC READ ERR\n");
		return EPDK_FAIL;

	}
	return EPDK_OK;
}

__u16 mcu_ReadReg_hxn(__u8 addr,__u8 *data,__u16 n_byte)
{
  	__twi_dev_para_ex_t  piic;        
	__s32 ret = 0;
	__u8 add1=addr;
    eLIBs_memset(&piic, 0, sizeof(__twi_dev_para_ex_t));


	piic.slave_addr         = (DVR_MCU_HXN_ADD); //设备地址，值为7bit地址，不含读写标志位
	piic.slave_addr_flag    = 0;
	piic.byte_addr          = &add1;    //如果访问设备的寄存器地址不是8位的话，需要传入最高byte的地址， 这里以8位为例，所以直接取地址
	piic.byte_addr_width    = 1;            //设备寄存器地址的长度，以byte为单位，这里以8位为例，所以取1, 如果>1，填所需长度，最大到3
	piic.data               = data;         //数据buffer
	piic.byte_count         = n_byte;          //读取数据长度


	//如果不是标准iic，即不发restart的读操作
	ret = eLIBs_fioctrl(hiic, TWI_READ_SPEC_RS,  0, (void *)&piic);

	if (ret == EPDK_FAIL)
	{
		//FAIL 处理
	  eDbug("IIC READ ERR\n");
		return EPDK_FAIL;

	}
	return EPDK_OK;
}

__s32 mcu_WriteReg(__u8 addr,__u8 *data, __u16 n_byte)
{
	__u8 NumOfBytesToWrite;
  	__twi_dev_para_ex_t  iic_para;   
	__s32 ret = EPDK_FAIL;
	__u8 reg_addr = 0;
	eLIBs_memset(&iic_para, 0, sizeof(__twi_dev_para_ex_t));

	

	
	iic_para.slave_addr         = (DVR_MCU_ADD>>1); //设备地址，值为7bit地址，不含读写标志位
	iic_para.slave_addr_flag    = 0;
	iic_para.byte_addr          = &addr;    //如果访问设备的寄存器地址不是8位的话，需要传入最高byte的地址， 这里以8位为例，所以直接取地址
	iic_para.byte_addr_width    = 1;            //设备寄存器地址的长度，以byte为单位，这里以8位为例，所以取1, 如果>1，填所需长度，最大到3
	iic_para.data               = data;         //数据buffer
	iic_para.byte_count         = n_byte;          //写入数据长度
    
	ret = eLIBs_fioctrl(hiic, TWI_WRITE_SPEC_RS,  0, (void *)&iic_para);
	if (ret == EPDK_FAIL)
	{
		//FAIL 处理
		eDbug("---------IIC WRITE ERR\n");
		return EPDK_FAIL;		
	}
	return EPDK_OK;
       
}

__s32 mcu_check(ES_FILE   *fiic)
{	
      	__s32 ret = EPDK_FAIL;
	__u8  value=0;
	hiic = fiic;
	__wrn("   mcu_check  start\n");
       if(hiic == NULL)
	{
		eDbug("twi bus is not available!\n");
		return EPDK_FAIL;
	}
	  // TWI_SCLK_10KHZ  ???
	ret = eLIBs_fioctrl(hiic, TWI_SET_SCL_CLOCK, 0, (void *)TWI_SCLK_10KHZ);	
	if(ret == EPDK_OK)
	{
		eDbug("set iic clock as 100Khz successfully\n");
	}
	else
	{
	eDbug("set iic clock as 100Khz fail\n");

	}
	 ret=mcu_ReadReg(DRV_MCU_COMMAND1,&value,1) ;
	 eDbug("return -- 1value=%x\n",value);
	   if(value!=DRV_MCU_DATA1)
	   	return EPDK_FAIL;
	 ret=mcu_ReadReg(DRV_MCU_COMMAND2,&value,1) ;
	 
	 eDbug("return -- 2value=%x\n",value);
	   if(value!=DRV_MCU_DATA2)
	   	return EPDK_FAIL; 
	 ret=mcu_ReadReg(DRV_MCU_COMMAND3,&value,1) ;
	 
	 eDbug("return -- 3value=%x\n",value);
	   if(value!=DRV_MCU_DATA3)
	   	return EPDK_FAIL; 
	 ret=mcu_ReadReg(DRV_MCU_COMMAND4,&value,1) ;
	 
	 eDbug("return -- 4value=%x\n",value);
	   if(value!=DRV_MCU_DATA4)
	   	return EPDK_FAIL; 
	 ret=mcu_ReadReg(DRV_MCU_COMMAND5,&value,1) ;
	 
	 eDbug("return -- 5value=%x\n",value);
	   if(value!=DRV_MCU_DATA5)
	   	return EPDK_FAIL; 
	 ret=mcu_ReadReg(DRV_MCU_COMMAND6,&value,1) ;
	 
	 eDbug("return -- 6value=%x\n",value);
	   if(value!=DRV_MCU_DATA6)
	   	return EPDK_FAIL;    
	 ret=mcu_ReadReg(DRV_MCU_COMMAND7,&value,1) ;
	 
	 eDbug("return -- 7value=%x\n",value);
	   if(value!=DRV_MCU_DATA7)
	   	return EPDK_FAIL;       
	ret=mcu_ReadReg(DRV_MCU_COMMAND8,&value,1) ;
	
	eDbug("return -- 8value=%x\n",value);
	   if(value!=DRV_MCU_DATA8)
	   	return EPDK_FAIL;    
	
	
	
	return EPDK_OK;
}

__s32 mcu_check_hxn(ES_FILE   *fiic)
{	
      	__s32 ret = EPDK_FAIL;
	__u8  value=0;
	hiic = fiic;
	__wrn("   mcu_check  start\n");
       if(hiic == NULL)
	{
		eDbug("twi bus is not available!\n");
		return EPDK_FAIL;
	}
	  // TWI_SCLK_10KHZ
	ret = eLIBs_fioctrl(hiic, TWI_SET_SCL_CLOCK, 0, (void *)TWI_SCLK_HXN_100KHZ);	
	if(ret == EPDK_OK)
	{
		eDbug("set iic clock as 100Khz successfully\n");
	}
	else
	{
		eDbug("set iic clock as 100Khz fail\n");

	}
	 ret=mcu_ReadReg_hxn(DRV_MCU_HXN_COMMAND1,&value,1) ;
	 eDbug("return -- 1value=%x\n",value);
	   if(value!=DRV_MCU_DATA1)
	   	return EPDK_FAIL;
	 ret=mcu_ReadReg_hxn(DRV_MCU_HXN_COMMAND2,&value,1) ;
	 
	 eDbug("return -- 2value=%x\n",value);
	   if(value!=DRV_MCU_DATA2)
	   	return EPDK_FAIL; 
	 ret=mcu_ReadReg_hxn(DRV_MCU_HXN_COMMAND3,&value,1) ;
	 
	 eDbug("return -- 3value=%x\n",value);
	   if(value!=DRV_MCU_DATA3)
	   	return EPDK_FAIL; 
	 ret=mcu_ReadReg_hxn(DRV_MCU_HXN_COMMAND4,&value,1) ;
	 
	 eDbug("return -- 4value=%x\n",value);
	   if(value!=DRV_MCU_DATA4)
	   	return EPDK_FAIL; 
	 ret=mcu_ReadReg_hxn(DRV_MCU_HXN_COMMAND5,&value,1) ;
	 
	 eDbug("return -- 5value=%x\n",value);
	   if(value!=DRV_MCU_DATA5)
	   	return EPDK_FAIL; 
	 ret=mcu_ReadReg_hxn(DRV_MCU_HXN_COMMAND6,&value,1) ;
	 
	 eDbug("return -- 6value=%x\n",value);
	   if(value!=DRV_MCU_DATA6)
	   	return EPDK_FAIL;    
	
	return EPDK_OK;
}



