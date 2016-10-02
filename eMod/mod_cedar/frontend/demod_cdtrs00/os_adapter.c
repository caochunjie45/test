
#include "os_adapter.h"

static ES_FILE* ftwi = NULL;
#define I2C_100k_9090    (100000)

int iic_init(void)
{
	int ret;
	//unsigned int sclkI2ctemp = 100000;

	ftwi = eLIBs_fopen("b:\\BUS\\TWI2", "r");
	
    if(ftwi == NULL)
    {
	 	WARNING("open twi1 failed\n");
        return -1;
    }

	ret = eLIBs_fioctrl(ftwi, TWI_SET_SCL_CLOCK, 0, (void *)(100000));
    if(ret)
    {
		WARNING("set i2c clock[100K] failed\n");
    }
	WARNING("&&&&&&&&&&&&&&&&&&&&&&&&& TWI2 open sucess &&&&&&&&&&&&&&&&&\n");
	WARNING("&&&&&&&&&&&&&&&&&&&&&&&&& TWI2 open sucess &&&&&&&&&&&&&&&&&\n");
	WARNING("&&&&&&&&&&&&&&&&&&&&&&&&& TWI2 open sucess &&&&&&&&&&&&&&&&&\n");
	return 0;
}

int iic_exit(void)
{
    if(ftwi)
    {
        eLIBs_fclose(ftwi);
        ftwi = NULL;
    }
    
    return 0;
}

int iic_write(int devAddr, unsigned char* regAddr, int addrSize, unsigned char* txBuf, int txCnt)
{
	__twi_dev_para_ex_t    stwi;
	unsigned char iic_addr[4];
	int ret = 0;
	
	if(addrSize == 1)
	{
		iic_addr[0] = regAddr[0];
	}
	else if(addrSize == 2)
	{
		iic_addr[0] = regAddr[1];
	       iic_addr[1] = regAddr[0];
	}
	else if (addrSize == 4)
	{
		iic_addr[0] = regAddr[3];
	       iic_addr[1] = regAddr[2];
		iic_addr[2] = regAddr[1];
	       iic_addr[3] = regAddr[0];
	}
	else
	{
		WARNING(" addrsize is error,[%d]\n", addrSize);
	}
	
	if(ftwi)
	{
		stwi.slave_addr      = devAddr;
		stwi.slave_addr_flag  =  0;
		stwi.byte_addr = iic_addr;
		stwi.byte_addr_width = addrSize;
		stwi.byte_count = txCnt;
		stwi.data           = txBuf;

		ret = eLIBs_fioctrl(ftwi, TWI_WRITE_SPEC_RS, 0, (void *)&stwi);

		//demod_MB86A2x_printf("i2c write devAddr = %x ret = %d   reg[%x] \n", devAddr,ret,iic_addr[0]);
	}
	else
	{
		demod_MB86A2x_printf("i2c write failed ftwi is null\n");
	}

	return ret;
}

int iic_read(int devAddr, unsigned char* regAddr, int addrSize, unsigned char* rxBuf, int txCnt)
{
	__twi_dev_para_ex_t    stwi;
	unsigned char iic_addr[4];
	int ret = 0;

	if(addrSize == 1)
	{
		iic_addr[0] = regAddr[0];
	}
	else if(addrSize == 2)
	{
		iic_addr[0] = regAddr[1];
	       iic_addr[1] = regAddr[0];
	}
	else if (addrSize == 4)
	{
		iic_addr[0] = regAddr[3];
	       iic_addr[1] = regAddr[2];
		iic_addr[2] = regAddr[1];
	       iic_addr[3] = regAddr[0];
	}
	else
	{
		WARNING(" addrsize is error,[%d]\n", addrSize);
	}
	   
	
	if(ftwi)
	{
		stwi.slave_addr      = devAddr;
		stwi.slave_addr_flag  =  0;
		stwi.byte_addr = iic_addr;
		stwi.byte_addr_width = addrSize;
		stwi.byte_count = txCnt;
		stwi.data           = rxBuf;

		ret = eLIBs_fioctrl(ftwi, TWI_READ_SPEC_RS, 0, (void *)&stwi);
		//demod_MB86A2x_printf("i2c read devAddr = %x ret = %d   reg[%x] \n", devAddr,ret,iic_addr[0]);
	}
	else
	{
		demod_MB86A2x_printf("i2c read failed ftwi is null\n");
	}
	
	return ret;
}

void msleep(int ms)
{
    esKRNL_TimeDly((ms + 9)/10);
    return;
}

void usleep(int us)
{
    __u32 ms;
    
    ms = (us + 999)/1000;
    msleep(ms);
    return;
}

