/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                               DRAM CSP Module
*
*                                    (c) Copyright 2010-2014, Berg.Xing China
*                                             All Rights Reserved
*
* File    : csp_dram.h
* By      : Berg.Xing
* Version : v1.0
* Date    : 2010-12-2 13:24
* Descript:
* Update  : date                auther      ver     notes
*           2010-12-2 13:24     Berg.Xing   1.0     build the file;
*********************************************************************************************************
*/

#ifndef __CSP_DRAM_H__
#define __CSP_DRAM_H__


typedef enum __DRAM_TYPE
{
    DRAM_TYPE_DDR   =1,
    DRAM_TYPE_DDR2  =2,
    DRAM_TYPE_DDR3  =3
} __dram_type_e;

typedef enum __DRAM_BNKMOD
{
    DRAM_ACS_INTERLEAVE = 0,
    DRAM_ACS_SEQUENCE   = 1,
} __dram_bnkmod_e;


typedef enum __DRAM_PRIO_LEVEL
{
    DRAM_PRIO_LEVEL_0 = 0,
    DRAM_PRIO_LEVEL_1    ,
    DRAM_PRIO_LEVEL_2    ,
    DRAM_PRIO_LEVEL_3    ,
    DRAM_PRIO_LEVEL_

} __dram_prio_level_e;

typedef enum __DRAM_DEV
{
    DRAM_DEVICE_NDMA,
    DRAM_DEVICE_ATH,
    DRAM_DEVICE_SATA,
    DRAM_DEVICE_SD,
    DRAM_DEVICE_USB1,
    DRAM_DEVICE_USB2,
    DRAM_DEVICE_CPU,
    DRAM_DEVICE_GPU,
    DRAM_DEVICE_BE0,
    DRAM_DEVICE_FE0,
    DRAM_DEVICE_CSI0,
    DRAM_DEVICE_TSC,
    DRAM_DEVICE_VE,
    DRAM_DEVICE_BE1,
    DRAM_DEVICE_FE1,
    DRAM_DEVICE_MP,
    DRAM_DEVICE_TVIN,
    DRAM_DEVICE_CSI1,
	DRAM_DEVICE_ACE,
	DRAM_DEVICE_DDMA,
	DRAM_DEVICE_GPS
} __dram_dev_e;

typedef struct __DRAM_PARA
{
    __u32           dram_baseaddr;
    __u32           dram_clk;
    __u32           dram_type;
    __u32           dram_rank_num;
    __u32           dram_chip_density;
    __u32           dram_io_width;
    __u32		    dram_bus_width;
    __u32           dram_cas;
    __u32           dram_zq;
    __u32           dram_odt_en;
    __u32 			dram_size;
    __u32           dram_tpr0;
    __u32           dram_tpr1;
    __u32           dram_tpr2;
    __u32           dram_tpr3;
    __u32           dram_tpr4;
    __u32           dram_tpr5;
    __u32 			dram_emr1;
    __u32           dram_emr2;
    __u32           dram_emr3;
} __dram_para_t;

__s32 CSP_DRAMC_init(__dram_para_t *para);
__s32 CSP_DRAMC_exit(void);
__u32 CSP_DRAMC_set_emrs(__u32 emrs_id, __u32 emrs_val);
__s32 CSP_DRAMC_scan_readpipe(void);
void  CSP_DRAMC_gate_clock(__u32 on);
void  CSP_DRAMC_mode_exit(__dram_para_t *para);
void  CSP_DRAMC_enter_selfrefresh(void);
void  CSP_DRAMC_exit_selfrefresh(void);
void  CSP_DRAMC_enter_powerdown(__dram_para_t *para);
void  CSP_DRAMC_exit_powerdown(__dram_para_t *para);
void  CSP_DRAMC_precharge_all(__dram_para_t *para);
void  CSP_DRAMC_set_autorefresh_cycle(__u32 clk);
void  CSP_DRAMC_DLL_reset(void);
void  CSP_DRAMC_DLL_enable(void);
void  CSP_DRAMC_DLL_disable(void);
__s32 CSP_DRAMC_access_enable(void);
__s32 CSP_DRAMC_access_disable(void);

#endif  //__CSP_DRAM_H__

