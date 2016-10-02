#include "write_bh.h"
#define SPINOR_RDID_CMD      0x9f
#define SPINOR_WORK_MODE (3)
#define SPINOR_CHIP_SELECT (0x01)
static ES_FILE         *hfile;
static __spinor_info_t *spinor_info;
static __bool           spinor_info_flag = 0;//spinor_info指向的结构体是否动态分配的内存
#define SPINOR_WORK_CLK_RATE 10000000 /* 10MHz */
__spinor_cmd_set_t spinor_cmd_set [] =
{
    {0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x02, 0xD8, 0xC7}
};
__spinor_info_t    spinor_info_set[] =
{
    /* AMIC系列 */
	{"AMIC",    "A25L032",    0x30, 0x00163037, 0x1537, SPINOR_UNIFORM, 256, 32 * 1024,  50000000, 100000000,  0},
    /* SPATION系列 */
    {"SPATION", "S25FL032A",  0x30, 0x00150201, 0xffff, SPINOR_UNIFORM, 256, 32 * 1024,  33000000,  50000000,  0},
    {"SPATION", "S25FL064A",  0x30, 0x00160201, 0xffff, SPINOR_UNIFORM, 256, 64 * 1024,  25000000,  50000000,  0},
    /* ST系列 */
	{"ST",      "M25P32",     0x30, 0x00162020, 0xffff, SPINOR_UNIFORM, 256, 32 * 1024,  33000000,  75000000,  0},
	{"ST",      "M25PX32",    0x30, 0x00167120, 0xffff, SPINOR_UNIFORM, 256, 32 * 1024,  33000000,  75000000,  0},
	{"ST",      "M25P64",     0x30, 0x00172020, 0xffff, SPINOR_UNIFORM, 256, 64 * 1024,  20000000,  50000000,  0},
	{"ST",      "M25PX64",    0x30, 0x00177120, 0xffff, SPINOR_UNIFORM, 256, 64 * 1024,  33000000,  75000000,  0},
	/*PM系列*/
    {"PM",   "PM25LQ032C",    0x30, 0x00469d7f, 0xffff, SPINOR_UNIFORM,  256, 32 * 1024,  10000000,  50000000,  0},
    //zhcai added 
	/* MX(旺宏)系列 */
	{"MX",      "MX25L3206E",     0x30, 0x001620c2, 0xffff, SPINOR_UNIFORM, 256, 32 * 1024,  33000000,  86000000,  0},	
   	{"MX",      "MX25L3206E",     0x30, 0x001720c2, 0xffff, SPINOR_UNIFORM, 256, 64 * 1024,  33000000,  86000000,  0},	

    {"GD",      "GD25Q32",     0x30, 0x001640c8, 0xffff, SPINOR_UNIFORM, 256, 32 * 1024,  50000000,  80000000,  0},	

    {"W25",      "W25Q64BV",     0x30, 0x001740ef, 0xffff, SPINOR_UNIFORM, 256, 64 * 1024,  /*50000000*/10000000,   /*80000000*/10000000,  0},	
    {"EN",      "EN25Q32B",     0x30, 0x0016301c, 0xffff, SPINOR_UNIFORM, 256, 32 * 1024,  50000000,  80000000,  0},	


	/* EON系列 */
	{"EON",     "EN25F32",    0x30, 0x0016311C, 0x151C, SPINOR_UNIFORM, 256, 32 * 1024,  50000000, 100000000,  0},
    {"EON",     "EN25P32",    0x30, 0x0016201C, 0x151C, SPINOR_UNIFORM, 256, 32 * 1024,  66000000, 100000000,  0},
	{"EON",     "EN25P64",    0x30, 0x0017201C, 0x161C, SPINOR_UNIFORM, 256, 64 * 1024,  66000000, 100000000,  0},
	/* WINBOND系列 */
	{"WINBOND", "M25X32",     0x30, 0x001630EF, 0x15EF, SPINOR_UNIFORM, 256, 32 * 1024,  33000000,  50000000,  0},
	{"WINBOND", "M25Q32",     0x30, 0x001640EF, 0x15EF, SPINOR_UNIFORM, 256, 32 * 1024, 104000000,  104000000,  0},
	{"WINBOND", "M25Q64",     0x30, 0x001740ef, 0xffff, SPINOR_UNIFORM, 256, 64 * 1024, 104000000,  104000000,  0},
	{"WINBOND", "M25X64",     0x30, 0x001730EF, 0xEFFF, SPINOR_UNIFORM, 256, 64 * 1024,  33000000,  50000000,  0},
    /* ATMEL系列 */
	{"ATMEL",   "AT26DF321",  0x30, 0x0000471F, 0xffff, SPINOR_UNIFORM, 256, 32 * 1024,  33000000,  66000000,  0},
	{"ATMEL",   "AT25DF321",  0x30, 0x0000471F, 0xffff, SPINOR_UNIFORM, 256, 32 * 1024,  33000000,  70000000,  0},
    /* INTEL系列 */
    {"INTEL",   "S33-32",     0x30, 0x00128989, 0xffff, SPINOR_BOTTOM,  256, 32 * 1024,  33000000,  68000000,  0},
    {"INTEL",   "S33-64",     0x30, 0x00138989, 0xffff, SPINOR_BOTTOM,  256, 64 * 1024,  33000000,  68000000,  0},
    /* MXIC系列 */
    {"MXIC",    "MX25L3206E", 0x30, 0x001620c2, 0x15c2, SPINOR_BOTTOM,  256, 32 * 1024,  30000000,  80000000,  0},

	{"YUNS",    "MXYUNSHENG", 0x30, 0x001740c8, 0xffff, SPINOR_BOTTOM,  256, 64 * 1024,  50000000,  50000000,  0},
    
    //???
    {"UNKOWN",  "UNKOWN",     0x30, 0x001720c2, 0xffff, SPINOR_BOTTOM,  256, 32 * 1024,  10000000,  40000000,  0},

};

/********************************************************************************************************
* Function   : spinor_wren
* Description: write enable
* Arguments  : 
* Return     : 
*********************************************************************************************************/
static __s32 spinor_wren(void)
{
	__u8 sdata[4] = {0};
	__spi_dev_set_xfer_t spi_xfer;

	sdata[0] = 0x06;    //spinor_cmd_set[spinor_info->cmd_index].WREN; //写使能命令
	spi_xfer.tx_addr   = sdata;
	spi_xfer.tx_count  = 1;
	spi_xfer.rx_addr   = NULL;
	spi_xfer.rx_count  = 0;
	spi_xfer.work_mode = SPINOR_WORK_MODE;
	spi_xfer.work_clk  = 33000000;//spinor_info->freq;
	spi_xfer.work_slot = SPINOR_CHIP_SELECT;
	spi_xfer.reserved  = NULL;

	if (eLIBs_fioctrl(hfile, SPI_DEV_CMD_MASTER_RW, 0, &spi_xfer))
	{
		return EPDK_FAIL;
	}

	return EPDK_OK;
}
#if 0
static __s32 spinor_detect_basic_info(__spinor_info_t **spinor_info)
{
    __u32 rdid;
    __u32 rems;
    __u32 i, cnt = sizeof(spinor_info_set)/sizeof(__spinor_info_t);
    
    if (spinor_read_rdid(&rdid))
    {
        __wrn("get spinor rdid failed\n");
        return EPDK_FAIL;
    }
    
    if (spinor_read_rems(&rems))
    {
        __wrn("get spinor rems failed\n");
        return EPDK_FAIL;
    }

    eLIBs_printf("spinor rdid is 0x%08x, rems is %x\n", rdid, rems);
    
    do{//read nor info from config script first
        __s32 ret;
        __u32 spinor_num;
        __s32 i;
        __u32 cfg_rdid;
        __u32 cfg_freq_read;
        __u32 cfg_freq;
        __u32 cfg_capaticy;
        char name[32];
        __spinor_info_t* pnor_info;

        spinor_info_flag = 0;
        spinor_num = 0;
    	ret = esCFG_GetKeyValue("spinor_para", "spinor_patten_num", (int *)&spinor_num, 1);
    	if(ret != 0){
    		__wrn("get spinor_patten_num failed\n");
            break;
    	}

        for(i = 0 ; i < spinor_num ; i++)
        {
            eLIBs_sprintf(name, "spinor%d", i);
            ret = esCFG_GetKeyValue(name, "rdid", (int *)&cfg_rdid, 1);
        	if(ret != 0){
        		__wrn("get rdid failed\n");
                continue;
        	}
            
            if(cfg_rdid != rdid)
            {
                continue;
            }
            
            ret = esCFG_GetKeyValue(name, "freq_read", (int *)&cfg_freq_read, 1);
        	if(ret != 0){
        		__wrn("get freq_read failed\n");
                continue;
        	}
            
            ret = esCFG_GetKeyValue(name, "freq", (int *)&cfg_freq, 1);
        	if(ret != 0){
        		__wrn("get freq failed\n");
                continue;
        	}
            
            ret = esCFG_GetKeyValue(name, "capaticy", (int *)&cfg_capaticy, 1);
        	if(ret != 0){
        		__wrn("get freq failed\n");
                continue;
        	}            

            pnor_info = esMEMS_Balloc(sizeof(__spinor_info_t));
            if(!pnor_info)
            {
                __wrn("esMEMS_Balloc fail...\n");
                break;
            }
            eLIBs_memset(pnor_info, 0, sizeof(__spinor_info_t));
            pnor_info->RDID_ID = cfg_rdid;
            pnor_info->freq_read= cfg_freq_read;
            pnor_info->freq = cfg_freq;
            pnor_info->capaticy = cfg_capaticy*1024;
            spinor_info_flag = 1;
            
            *spinor_info = pnor_info;

            return EPDK_OK;
            
        }
        
    }while(0);
    for (i = 0; i < cnt; i++)
    {
        if (spinor_info_set[i].RDID_ID == rdid)
		{
			__inf("successfully find one spinor\n");
			__inf("manufactory is %s, pattern is %s\n", spinor_info_set[i].factory, spinor_info_set[i].pattern);
			__inf("spinor store struct is %d\n", spinor_info_set[i].store_struct);
			__inf("spinor page size is %d, capacity is %dMB\n", spinor_info_set[i].page_size, spinor_info_set[i].capaticy >> 13);
			__inf("spinor frequency is %uMHz, read frequency is %uMHz\n",
				spinor_info_set[i].freq / 1000000, spinor_info_set[i].freq_read / 1000000);

			*spinor_info = &spinor_info_set[i];

			return EPDK_OK;
		}
    }

	eLIBs_printf("this spinor is not support, rdid is %x, rems is %x\n", rdid, rems);
	debug("cannot find spinor information\n");
	return EPDK_FAIL;
}
#endif
/********************************************************************************************************
* Function   : spinor_rdsr
* Description: read status register
* Arguments  : 
* Return     : 
*********************************************************************************************************/
static __s32 spinor_rdsr(__u32 *status)
{
	__u8 sdata[4] = {0};
	__spi_dev_set_xfer_t spi_xfer;

    *status = 0;
	sdata[0] = spinor_cmd_set[spinor_info->cmd_index].RDSR;
	spi_xfer.tx_addr   = sdata;
	spi_xfer.tx_count  = 1;
	spi_xfer.rx_addr   = status;
	spi_xfer.rx_count  = 1;
	spi_xfer.work_mode = SPINOR_WORK_MODE;
	spi_xfer.work_clk  = spinor_info->freq;
	spi_xfer.work_slot = SPINOR_CHIP_SELECT;
	spi_xfer.reserved  = NULL;

	if (eLIBs_fioctrl(hfile, SPI_DEV_CMD_MASTER_RW, 0, &spi_xfer))
	{
		return EPDK_FAIL;
	}

	return EPDK_OK;
}
/********************************************************************************************************
* Function   : spinor_wrsr
* Description: write status register  写保护
* Arguments  : 
* Return     : 
*********************************************************************************************************/

__s32 spinor_wrsr(__u8 value1,__u8 value2)
{
	__u8 sdata[4] = {0};
	__spi_dev_set_xfer_t spi_xfer;
	
	
	if(NULL==hfile)
{
	hfile = eLIBs_fopen("b:\\BUS\\SPI0", "r+");
	if(NULL == hfile)
	{
		__wrn("why fail, check it \n");
		return EPDK_FAIL;
	}
	
}
	spinor_wren();
	
	sdata[0] = 0x01;//spinor_cmd_set[spinor_info->cmd_index].WRSR; 写状态命令
	sdata[1] = value1;//状态寄存器低8位PB4到PB0的值 4MB GD为例 = 0b00010=2 为对0-63BLOCK写保护,即3968K,最后128K为用户区数据，不能写保护
	sdata[2] = value2;//状态寄存器CMP的值 ,以4MB GD为例 CMP=1, PB4-PB0 = 0b00010=2才能对0-61block写保护
	
	spi_xfer.tx_addr   = sdata;
	spi_xfer.tx_count  = 3;
	spi_xfer.rx_addr   = NULL;
	spi_xfer.rx_count  = 0;
	spi_xfer.work_mode = SPINOR_WORK_MODE;
	spi_xfer.work_clk  = 33000000;//spinor_info->freq;
	spi_xfer.work_slot = SPINOR_CHIP_SELECT;
	spi_xfer.reserved  = NULL;

	if (eLIBs_fioctrl(hfile, SPI_DEV_CMD_MASTER_RW, 0, &spi_xfer))
	{
		return EPDK_FAIL;
	}
	if(NULL != hfile)
	{
		eLIBs_fclose(hfile);
	}
	

	return EPDK_OK;
}
#if 1
__s32 spinor_bulk_eraser(void)
{
	__u8  sdata[4] = {0};
	__u32 status   = 0;
	__u32 i        = 0;

	__spi_dev_set_xfer_t spi_xfer;
	if(NULL==hfile)
{
	hfile = eLIBs_fopen("b:\\BUS\\SPI0", "r+");
	if(NULL == hfile)
	{
		__wrn("why fail, check it \n");
		return EPDK_FAIL;
	}
	
}
    if (spinor_wren())
    {
        return EPDK_FAIL;
    }
	sdata[0] = spinor_cmd_set[spinor_info->cmd_index].BE;
	spi_xfer.tx_addr   = sdata;
	spi_xfer.tx_count  = 1;
	spi_xfer.rx_addr   = NULL;
	spi_xfer.rx_count  = 0;
	spi_xfer.work_mode = SPINOR_WORK_MODE;
	spi_xfer.work_clk  = spinor_info->freq;
	spi_xfer.work_slot = SPINOR_CHIP_SELECT;
	spi_xfer.reserved  = NULL;

	if (eLIBs_fioctrl(hfile, SPI_DEV_CMD_MASTER_RW, 0, &spi_xfer))
	{
		return EPDK_FAIL;
	}

	do 
	{
		if (spinor_rdsr(&status))
			return EPDK_FAIL;
		for (; i < 100; i++)
			;
	} while (status & 0x01);
if(NULL != hfile)
	{
		eLIBs_fclose(hfile);
	}
	return EPDK_OK;
}
#endif



/********************************************************************************************************
* Function   : spinor_read_rdid
* Description: read spinor rdid
* Arguments  : 
* Return     : 
*********************************************************************************************************/
__s32 spinor_read_rdid(__u32 *rdid)
{
	__u8 sdata[4] = {0};
	__spi_dev_set_xfer_t spi_xfer;

	*rdid = 0;
	sdata[0] = SPINOR_RDID_CMD;
	spi_xfer.tx_addr   = sdata;
	spi_xfer.tx_count  = 1;
	spi_xfer.rx_addr   = rdid;
	spi_xfer.rx_count  = 3;
	spi_xfer.work_mode = SPINOR_WORK_MODE;
	spi_xfer.work_clk  = SPINOR_WORK_CLK_RATE; /* use default frequency */
	spi_xfer.work_slot = SPINOR_CHIP_SELECT;
	spi_xfer.reserved  = NULL;

	if (eLIBs_fioctrl(hfile, SPI_DEV_CMD_MASTER_RW, 0, &spi_xfer))
	{
		return EPDK_FAIL;
	}

	return EPDK_OK;
}
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
#if 0
__s32 spinor_init(void)
{
	hfile = eLIBs_fopen("b:\\BUS\\SPI0", "r+");
	if (!hfile)
	{
		__wrn("open spi dev failed\n");
		return EPDK_FAIL;
	}

	if (spinor_detect_basic_info(&spinor_info))
	{
		return EPDK_FAIL;
	}

	return spinor_info->capaticy >> 2;
}
#endif
/********************************************************************************************************
* Function   : spinor_exit
* Description: exit spinor
* Arguments  : 
* Return     : 
*********************************************************************************************************/
#if 0
__s32 spinor_exit(void)
{
	if (hfile)
	{
		eLIBs_fclose(hfile);
	}

    if(spinor_info_flag && spinor_info)
    {        
        esMEMS_Bfree(spinor_info, sizeof(__spinor_info_t));
        spinor_info = NULL;
        spinor_info_flag = 0;
    }
	return EPDK_OK;
}
#endif
__s32 DISK_Clear(void)
{
	__s32 ret;
	ES_FILE *hFile;

	hFile = eLIBs_fopen("b:\\STORAGE\\SPINOR", "rb");
    if (hFile == NULL)
    {
        __wrn("open NOR file failed\n");
        return EPDK_FAIL;
    }
    
    ret = eLIBs_fioctrl(hFile, DEV_IOC_USR_PHY_ERASE, 0, 0);
    
    eLIBs_fclose(hFile);

	return ret;
}
