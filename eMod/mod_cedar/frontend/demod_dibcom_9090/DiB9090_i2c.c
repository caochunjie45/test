#include <platform/platform_dibcom.h>

//////////////////////////////////////
//include the .h file here

#ifdef __ZORAN_SYSTEM__
#include <stdio.h>
#include "os_defs.h"
#include "globals.h"
#include "demod.h"
#include "opentvx.h"
#include "i68dbg.h"
#include "gpio.h"
#include "i2c.h"
#include "Dmx_TsdDataRam.h"
#include "a_post.h"
#include "board_config.h"
#include "gpio.h"
#else

// please include the head files here

#endif
///////////////////////////////////////

#include "dib9090_i2c.h"


//#define  DEBUG_GPIO_I2C_QIAO

#if defined(DEBUG_GPIO_I2C_QIAO)  /*re-write the i2c interface by emulate gpio*/
static unsigned int  hdl_SCK_IO = NULL;
static unsigned int  hdl_SDA_IO = NULL;

static unsigned int i2cDelay = 10;  /*us*/


void i2csw_setDelayTime(unsigned int delay)
{
    i2cDelay = delay;
}

void Timer_wait(unsigned int us)
{
 	int i = 0;
	i = us*68;
	
 	while(i >0)
 	{
		i--;
 	}

	return;	
}

int i2csw_init(void)
{
    __user_pin_set_t  gpio_set[1];

	/*PB18*/
    gpio_set->port = 2;
    gpio_set->port_num = 18;
    gpio_set->mul_sel = 1;  /*output*/
    gpio_set->pull = 1;
    gpio_set->drv_level = 1;
    gpio_set->data = 0;
    hdl_SCK_IO= esPINS_PinGrpReq(gpio_set, 1);
    if (!hdl_SCK_IO)
    {
        printf("request hdl_VHF_IO pin failed\n");
        return 1;
    }

	/*pB19*/
    gpio_set->port = 2;
    gpio_set->port_num = 19;
    gpio_set->mul_sel = 1;
    gpio_set->pull = 1;
    gpio_set->drv_level = 1;
    gpio_set->data = 0;

    hdl_SDA_IO= esPINS_PinGrpReq(gpio_set, 1);
    if (!hdl_SDA_IO)
    {
        printf("request hdl_SDA_IO pin failed\n");
        return 1;
    }
	
	return 0;
}


static void sda_lo(void)
{
	esPINS_WritePinData(hdl_SDA_IO, 0x00, 0);
	Timer_wait(i2cDelay);
}

static void sda_hi(void)
{
	esPINS_WritePinData(hdl_SDA_IO, 0x01, 0);

	Timer_wait(i2cDelay);
}

static void scl_lo(void)
{
	esPINS_WritePinData(hdl_SCK_IO, 0x00, 0);
	Timer_wait(i2cDelay);
}

static void scl_hi(void)
{
	esPINS_WritePinData(hdl_SCK_IO, 0x01, 0);
	
	Timer_wait(i2cDelay);
}

static unsigned is_scl_hi(void)
{
	esPINS_SetPinIO(hdl_SCK_IO,0,0);

	Timer_wait(i2cDelay);
	
    if (esPINS_ReadPinData(hdl_SCK_IO, 0))
	{
		esPINS_SetPinIO(hdl_SCK_IO,1,0);
		return 1;
	}
	
	esPINS_SetPinIO(hdl_SCK_IO,1,0);
	return 0;

	
}

static unsigned is_sda_lo(void)
{
	int ret;
	esPINS_SetPinIO(hdl_SDA_IO,0,0);
	Timer_wait(i2cDelay);
	
    if (esPINS_ReadPinData(hdl_SDA_IO, 0))
	{
		ret = 0;
	}
	else
	{
		ret = 1;
	}
	
	esPINS_SetPinIO(hdl_SDA_IO,1,0);
	return ret;
}

static unsigned is_sda_hi()
{
	esPINS_SetPinIO(hdl_SDA_IO,0,0);
	Timer_wait(i2cDelay);
	
    if (esPINS_ReadPinData(hdl_SDA_IO, 0))
	{
		esPINS_SetPinIO(hdl_SDA_IO,1,0);
		return 1;
	}
	
	esPINS_SetPinIO(hdl_SDA_IO,1,0);
	return 0;
}

static void wait_sda_hi(void)
{
	unsigned int start;
	start = esKRNL_TimeGet();

    for (;;) 
	{
		if (is_sda_hi())
		{
			break;
		}
		if ((esKRNL_TimeGet() - start) > 10000)
		{
//			printf(("TIMEOUT: in wait_sda_hi\n"));
			break;
		}
    }
}

static void wait_scl_hi(void)
{
	unsigned int start;
	start = esKRNL_TimeGet();

    for (;;) 
	{
		if (is_scl_hi())
		{
			break;
		}
		if ((esKRNL_TimeGet() -start) > 10000)
		{
//			printf(("TIMEOUT: in wait_scl_hi\n"));
			break;
		}


    }
}


static void i2csw_start(void)
{
	if((hdl_SDA_IO == NULL) ||(hdl_SCK_IO == NULL))
	{
		i2csw_init();
	}

//	printf("i2c start***\n");
	
	scl_lo();

	sda_hi();
	scl_hi();
	wait_scl_hi();

	sda_lo();			/* this triggers the start */
	scl_lo();
}

static void i2csw_stop(void)
{
//	printf("i2c stop***\n");

	scl_lo();

	sda_lo();
	scl_hi();
	wait_scl_hi();

	sda_hi();			// this triggers the stop 
	wait_sda_hi();
}

unsigned i2csw_putc(unsigned char d)
{
    unsigned char mask;
    unsigned noack;


    for (mask = 0x80; mask; mask >>= 1) 
	{
		if (d & mask)
		{
			sda_hi();
		}
		else
		{
			sda_lo();
		}
		scl_hi();
		scl_lo();
    }

    sda_hi();			/* ack handshake after every byte */

    if (is_sda_lo())		/* sda low means an ack */
	{
//		printf("ACK OK\n");
		noack = 0;
	}
    else
	{
//		printf("ACK error\n");
		noack = 1;
	}

    scl_hi();
    scl_lo();

    return noack;
}

unsigned char i2csw_getc(int last)
{
    unsigned char d;
    unsigned char i;


    for (i = 0, d = 0; i < 8; i++) 
	{
		d <<= 1;

		scl_hi();		/* drive clock high to get data */

		wait_scl_hi();
		if (is_sda_hi())
		{
			d |= 1;		/* bit is a one */
		}

		scl_lo();		/* drive clock back down */
    }

	if (last)
	{
  		sda_hi();		/* send an ack */
	}
	else
	{
		sda_lo();		/* send an ack */
	}


	scl_hi();
	wait_scl_hi();
	scl_lo();

	sda_hi();		/* release if set */

    return d;
}

unsigned int i2csw_write_sequence(unsigned int addr,unsigned char* buff,unsigned int size,unsigned char stop )
{
	unsigned int				i;

	char			name[9];

	unsigned int			scheduled_count, stack_size, minimum_stack, time_slice;
	void            *stack_base;


   if( 0 != addr )
   {
	i2csw_start();

	if (i2csw_putc(addr))
	{
            //
            // Send stop after error
            //
//            printf("putc addr[0x%x] error\n", addr);
    	    i2csw_stop();			/* done */
            return 0;
	} 
   }

	for (i=0; i<size; i++)
	{
		if (i2csw_putc(buff[i]))
		{
//		        printf("putc data[0x%x] error\n", buff[i]);
			
                	i2csw_stop();			/* done */
                    //
                    // Send stop after error
                    //
			return i;
		} 
	}

	if(stop)
	{
//		printf("write finished stop\n");
        	i2csw_stop();			/* done */
	}

	return i;
}

unsigned int i2csw_read_sequence(unsigned int addr,unsigned char* buff,unsigned int size)
{
	unsigned int				i;
	char			name[9];
	unsigned int			scheduled_count, stack_size, minimum_stack, time_slice;
	void            *stack_base;

	addr |= 1; // read opration

	i2csw_start();		

	if (i2csw_putc(addr))
	{
            //
            // Send stop after error
            //
        	i2csw_stop();			/* done */
		return 0;
	}
	for (i=0; i<size; i++)
	{
		buff[i] = i2csw_getc((i == size-1));
	}

	i2csw_stop();			/* done */

	return i;
}

#endif

unsigned char DiBcom_i2c_write(int addr, unsigned char *i2c_tx_buf, int txcnt)
{
	int i;
    //replace the i2c write func
#ifdef __ZORAN_SYSTEM__
    if(i2csw_write_sequence(I2C1_SCL, I2C1_SDA, (unsigned char)addr, i2c_tx_buf, txcnt) != (unsigned)txcnt)
#else
    // write function
    	demod_9090_printf("***i2c write addr[0x%x], size[%d]\n", addr, txcnt);
       for(i = 0;i<txcnt; i++)
       {
	//   	demod_9090_printf("[%02x]",i2c_tx_buf[i]);
       }
	demod_9090_printf("\n");

	#if defined(DEBUG_GPIO_I2C_QIAO)
		if(i2csw_write_sequence((unsigned char)addr, i2c_tx_buf, txcnt, 1) != (unsigned)txcnt)
	#else
	      addr = addr>>1;
		if(iic_write(addr, i2c_tx_buf, 2, &i2c_tx_buf[2], txcnt -2 ) != 0)
	#endif

#endif
    {
        //if write error, return 1
        demod_9090_printf("error\n");
        return 1;
    }
    //write no error, return 0
    return 0;
}

unsigned char DiBcom_i2c_read(int addr, unsigned char *i2c_tx_buf, int txcnt, unsigned char *i2c_rx_buf, int rxcnt)
{
	int i;

	unsigned char tempReg[2];
#ifdef __ZORAN_SYSTEM__
    if(i2csw_write_without_stop(I2C1_SCL, I2C1_SDA, (unsigned char)addr, i2c_tx_buf, txcnt) != (unsigned)txcnt)
    {
        //if write without stop error, return 1
        return 1;
    }
    addr = addr | 1;
    //replace the i2c read func
    if(i2csw_read_sequence(I2C1_SCL, I2C1_SDA, (unsigned char)addr, i2c_rx_buf, rxcnt) != (unsigned)rxcnt)
#else

    	demod_9090_printf("***i2c read (write)addr[0x%x],size[%d]\n", addr, txcnt);
       for(i = 0;i<txcnt; i++)
       {
	   	demod_9090_printf("[%02x]",i2c_tx_buf[i]);
       }
	demod_9090_printf("\n");
	demod_9090_printf("read cnt[%d]\n", rxcnt);
	
	#if defined(DEBUG_GPIO_I2C_QIAO)
	   if(i2csw_write_sequence((unsigned char)addr, i2c_tx_buf, txcnt, 0) != (unsigned)txcnt)
	    {
	        //if write without stop error, return 1
	        return 1;
	    }
	    addr = addr | 1;
	    //replace the i2c read func
	    	printf("***i2c read (read)addr[0x%x], size[%d]\n", addr, rxcnt);
	    
	    if(i2csw_read_sequence((unsigned char)addr, i2c_rx_buf, rxcnt) != (unsigned)rxcnt)
	#else
		addr = addr>>1;
		
		if(iic_read(addr, i2c_tx_buf, txcnt, i2c_rx_buf, rxcnt) != 0)
	    // read fucntion
      #endif
#endif
    {
        //if read error, return 1
        demod_9090_printf("error\n");
        return 1;
    }

       for(i = 0;i<rxcnt; i++)
       {
	   	demod_9090_printf("[%02x]",i2c_rx_buf[i]);
       }
	demod_9090_printf("\n");
	
    return 0;
}

int test_dibcom(void)
{
    int reg = 896;
    unsigned char tx[2];
    unsigned char rx[2];
    unsigned char temp;

    printf("test dibcom\n");

    tx[0] = (reg >> 8) & 0xff;
    tx[1] = reg & 0xff;

    if(0 != DiBcom_i2c_read(0x12, tx, 2, rx, 2))
    {
        if(0 != DiBcom_i2c_read(0x22, tx, 2, rx, 2))
        {
            if(0 != DiBcom_i2c_read(0x20, tx, 2, rx, 2))
            {
                if(0 != DiBcom_i2c_read(0x10, tx, 2, rx, 2))
                {
                    return 1; // read fail.
                }
            }
        }
    }

    printf("tx[0] = %#x, tx[1] = %#x, rx[0] = %#x, rx[1] = %#x\n", tx[0], tx[1], rx[0], rx[1]);
    if(rx[0] == 0x01 && rx[1] == 0xb3)
        return 0;    // ok
    else
        return 1;    // fail
}

