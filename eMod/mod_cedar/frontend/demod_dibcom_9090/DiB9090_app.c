#include "interface/host.h"
#include <adapter/frontend_tune.h>
#include <adapter/sip.h>
#include <adapter/pmu.h>
#include <sip/dibx09x.h>
#include <demod/dib7000m.h>
#include <pmu/osiris.h>
#include "monitor/monitor.h"

#include "dib9090_app.h"

/*********************************************************************/
// 使用的demod的数量
//#define NUM_OF_DEMOD 2
#define NUM_OF_DEMOD 1

// 主dibcom和从dibcom的连接方式
// 如果从dibcom连接到主dibcom的GPIO34上面，请将该值置为 1
#define DIBCOM_SLAVER_TO_MASTER 1

// 信号强度和质量的最小值和最大值
// 如果是 0 ~ 100 之间请加上该定义，否则为 0 ~ 255
#define _SIGNAL_STRENGTH_QUALITY_VALUE_MIN_0_MAX_100_
// 是否需要寄存器的打印，如果需要请将该值设置为 1
unsigned char g_DebutI2cData = 0;

/********************************************************************/
struct dibFrontend frontend[2] ;
extern void dib9090_set_pmu(struct dibFrontend *fe, struct dibPMU *pmu);
static const struct dib9090_config nim9090md_config[2] = {
    {
        NULL, // update_lna

        0,     // dib0090_freq_offset_khz_uhf
        0,     // dib0090_freq_offset_khz_vhf
        30000, // clock_khz
        1,     // clockouttobamse

        (0 << 15) | (1 << 13) | (0 << 12) | (3 << 10) | (0 << 9) | (1 << 7) | (0 << 6) | (0 << 4) | (1 << 3) | (1 << 1) | (0), // drives

        1,    // output_mpeg2_in_188_bytes

        #if ( NUM_OF_DEMOD == 1)
        {
            { BOARD_GPIO_COMPONENT_DEMOD, BOARD_GPIO_FUNCTION_COMPONENT_ON,  0x0000, 0x0000, 0x0000 },
            { BOARD_GPIO_COMPONENT_DEMOD, BOARD_GPIO_FUNCTION_COMPONENT_OFF, 0x0000, 0x0000, 0x0000 }
        },

        { /* subband */
            2,
            {
                { 381, { BOARD_GPIO_COMPONENT_DEMOD, BOARD_GPIO_FUNCTION_SUBBAND_GPIO, 0x0008, 0x0000, 0x0008 } },
                { 890, { BOARD_GPIO_COMPONENT_DEMOD, BOARD_GPIO_FUNCTION_SUBBAND_GPIO, 0x0008, 0x0000, 0x0000 } },
                { 0 },
            },
        },
        #endif
    }, {
        NULL, // update_lna

        0,     // dib0090_freq_offset_khz_uhf
        0,     // dib0090_freq_offset_khz_vhf
        30000, // clock_khz
        0,     // clockouttobamse

        (0 << 15) | (1 << 13) | (0 << 12) | (3 << 10) | (0 << 9) | (1 << 7) | (0 << 6) | (0 << 4) | (1 << 3) | (1 << 1) | (0), // drives

        1,    // output_mpeg2_in_188_bytes

        {
            { BOARD_GPIO_COMPONENT_DEMOD, BOARD_GPIO_FUNCTION_COMPONENT_ON,  0x0010, 0x0000, 0x0010 }, // LED is on GPIO 4
            { BOARD_GPIO_COMPONENT_DEMOD, BOARD_GPIO_FUNCTION_COMPONENT_OFF, 0x0010, 0x0000, 0x0000 },
        },

        { /* subband */
            2,
            {
                { 381, { BOARD_GPIO_COMPONENT_DEMOD, BOARD_GPIO_FUNCTION_SUBBAND_GPIO, 0x0006, 0x0000, 0x0006 } }, /* GPIO 1 and 2 to 1 for VHF */
                { 890, { BOARD_GPIO_COMPONENT_DEMOD, BOARD_GPIO_FUNCTION_SUBBAND_GPIO, 0x0006, 0x0000, 0x0000 } }, /* GPIO 1 and 2 to 0 for UHF */
                { 0 },
            },
        },
    },
};

static const struct osiris_config nim9090md_osiris_config = {
    1000,     /* DCDC1                             */
    2000,     /* DCDC2                             */
    1800,     /* LDO_SRAM                          */
    1800,     /* LDO_TUNER                         */
    1800,     /* LDO_ADC1                          */
    1020,     /* LDO_CORE_MAC                      */
    3000,     /* LDO_ADC2                          */
    (1 << 1), /* Enable LDO_ADC2, Disable LDO_ADC1 */
    0,        /* Disable DCDC2 current sense       */
    0,        /* Divide DCDC2 clock by 2           */
};

unsigned char LITTLE_ENDIAN_SYSTEM = 0;
void little_endian()
{
    unsigned short tmp = 0x00ff;
    unsigned char b[2];
    memcpy(b,&tmp,sizeof(tmp));
    if(b[0]==0)
    {
        LITTLE_ENDIAN_SYSTEM = 0;
    }
    else
    {
        LITTLE_ENDIAN_SYSTEM = 1;
    }

#ifdef DIBCOM_DEBUG_INFO
    printf("b[0]=0x%x b[1]=0x%x\n",b[0],b[1]);
    printf("LITTLE_ENDIAN_SYSTEM = %d\n",LITTLE_ENDIAN_SYSTEM);
#endif
}

#define  _USE_DIBCOM_
char dib_init()
{
#ifdef _USE_DIBCOM_
    struct dibDataBusHost *i2c;
#ifdef _USE_DIBCOM0190_PMU_
    struct dibDataBusHost *b;
    struct dibPMU *pmu;
#endif

#ifdef DIBCOM_DEBUG_INFO
    printf("dibcom star\n");
#endif
    little_endian();
    i2c = host_i2c_interface_attach(NULL);

    if (i2c == NULL)
        return 1;

    frontend_init(&frontend[0]); /* initializing the frontend-structure */

    #if (NUM_OF_DEMOD == 2)
        frontend_init(&frontend[1]); /* initializing the frontend-structure */
    #endif

    frontend_set_id(&frontend[0], 0); /* assign an absolute ID to the frontend */

    #if (NUM_OF_DEMOD == 2)
        frontend_set_id(&frontend[1], 1); /* assign an absolute ID to the frontend */
    #endif


    if (dib9090_firmware_sip_register(&frontend[0], 0x80, i2c, &nim9090md_config[0]) == NULL) { /* using a frontend on I2C address 0x80 */
        dbgp(" DiB9090: attaching demod and tuners failed.\n");
        return DIB_RETURN_ERROR;
        }

    #if (NUM_OF_DEMOD == 2)
        #if (DIBCOM_SLAVER_TO_MASTER == 1)
        /* the two dibcom 9080 connect to same place */
        ///////////////////////////////////////////////
        /*  ---------------     ---------------      */
        /*  | 90X0 master |     | 90X0 slaver |      */
        /*  |-------------|     ---------------      */
        /*  |             |     |             |      */
        /*  |        GPIO3|-----|SCL          |      */
        /*  |        GPIO4|-----|SDA          |      */
        /*  |-------------|     |-------------|      */
        /*             | |                           */
        /*             | |                           */
        /*          ----------------                 */
        /*          |  main chip  |                  */
        /*          ----------------                 */
        ///////////////////////////////////////////////
        i2c = dib7000m_get_i2c_master(&frontend[0], DIBX000_I2C_INTERFACE_GPIO_3_4, 0);
        #endif
  	    if (dib9090_firmware_sip_register(&frontend[1], 0x82, i2c, &nim9090md_config[1]) == NULL) { /* using a frontend on I2C address 0x80 */
            dbgp(" DiB9090: attaching demod and tuners failed.\n");
            return DIB_RETURN_ERROR;
        }
    #endif

    /* do the i2c-enumeration for 2 demod and use 0x80 as the I2C address for first device */
    i2c = data_bus_client_get_data_bus(demod_get_data_bus_client(&frontend[0]));

#if 1
            if(dib7000m_i2c_enumeration(i2c, 1, 0x12, 0x80) != 0)
            {
                printf("Master I2c addr = 0x12 failed\n");
            }
#else
    if(dib7000m_i2c_enumeration(i2c, 1, 0x22, 0x80) != 0)
    {
        printf("Master I2c addr = 0x22 failed\n");
        if(dib7000m_i2c_enumeration(i2c, 1, 0x20, 0x80) != 0)
        {
            printf("Master I2c addr = 0x20 failed\n");
            if(dib7000m_i2c_enumeration(i2c, 1, 0x12, 0x80) != 0)
            {
                printf("Master I2c addr = 0x12 failed\n");
                if(dib7000m_i2c_enumeration(i2c, 1, 0x10, 0x80) != 0)
                {
                    printf("Master I2c addr = 0x10 failed\n");
                    return DIB_RETURN_ERROR;
                }
            }
        }
    }
#endif

    #if (NUM_OF_DEMOD == 2)
        i2c = data_bus_client_get_data_bus(demod_get_data_bus_client(&frontend[1]));
        if(dib7000m_i2c_enumeration(i2c, 1, DEFAULT_DIB9000_I2C_ADDRESS, 0x82) != 0)
        {
            return DIB_RETURN_ERROR;
        }
    #endif

    #ifdef _USE_DIBCOM0190_PMU_
        #ifdef DIBCOM_DEBUG_INFO
            printf("USE DIBCOM0190 PMU\n");
        #endif
        b = dib7000m_get_i2c_master(&frontend[0], DIBX000_I2C_INTERFACE_GPIO_1_2, 0);
        if ((pmu = osiris_create(b, &nim9090md_osiris_config)) == NULL)
            return DIB_RETURN_ERROR;
        dib9090_set_pmu(&frontend[0], pmu);
    #endif

    if(DIB_RETURN_SUCCESS != frontend_reset(&frontend[0]))
        return DIB_RETURN_ERROR;

    #if (NUM_OF_DEMOD == 2)
        if(DIB_RETURN_SUCCESS != frontend_reset(&frontend[1]))
            return DIB_RETURN_ERROR;
    #endif
#endif
    return DIB_RETURN_SUCCESS;
}

unsigned char dib_tune(unsigned int frequency, char bandwidth)
{
#ifdef _USE_DIBCOM_
    struct dibChannel ch;
    struct dibDemodMonitor mon[2];
    printf("frequency = %d, bandwidth= %d\n", frequency, bandwidth);
    DibZeroMemory(&mon, sizeof(mon));

    //tune ....
    INIT_CHANNEL(&ch, STANDARD_DVBT);
    ch.RF_kHz = frequency;
    switch(bandwidth)
    {
        case 2:
            ch.bandwidth_kHz     = 2000;
            break;
        case 3:
            ch.bandwidth_kHz     = 2500;
            break;
        case 4:
            ch.bandwidth_kHz     = 4000;
            break;
        case 6:
            ch.bandwidth_kHz     = 6000;
            break;
        case 7:
            ch.bandwidth_kHz     = 7000;
            break;
        case 8:
        default:
            ch.bandwidth_kHz     = 8000;
            break;
    }
#ifdef DIBCOM_DEBUG_INFO
        printf("Dibcom connect: freq=%dkHz,Bandwidth=%dMHz\n", ch.RF_kHz, ch.bandwidth_kHz);
#endif

    printf("-I- dib_tune begin\n");
    tune_diversity(frontend, NUM_OF_DEMOD, &ch);
    printf("-I- dib_tune end\n");
    DibMSleep(200);         // at least wait 100ms

#ifdef DIBCOM_DEBUG_INFO
    do{
        DibMSleep(300);
        demod_get_monitoring(&frontend[0], &mon[0]);
        #if (NUM_OF_DEMOD == 2)
        demod_get_monitoring(&frontend[1], &mon[1]);
        #endif
        dib7000_print_monitor(mon, NULL, 0 ,NUM_OF_DEMOD);
    }while(0);
#endif

	if(1 == dib_lockstatus())
	{
		return 0;
	}
	else
	{
		return 1;
	}
		
#endif

	return 0;
}

char dib_lockstatus()
{
#ifdef _USE_DIBCOM_
    struct dibDVBSignalStatus status;

    demod_get_signal_status(&frontend[0], &status);
    printf("status.mpeg_data_lock = %d\n", status.mpeg_data_lock);

    return status.mpeg_data_lock;
#else
    return 1;
#endif

}

void dib_signal_strenth_quality(unsigned char *signal_quality, unsigned char *signal_strength)
{
#ifdef _USE_DIBCOM_
    struct dibDVBSignalStatus status;

    demod_get_signal_status(&frontend[0], &status);

#ifdef _SIGNAL_STRENGTH_QUALITY_VALUE_MIN_0_MAX_100_

    *signal_quality = (unsigned char)(((float)(2097151 - status.bit_error_rate)/2097151.0)*100) ;
    *signal_strength = (unsigned char)status.signal_strength;

    if(*signal_quality < 1)
        *signal_quality = 1;
    else if (*signal_quality >100)
        *signal_quality =100;

    if(*signal_strength < 1)
        *signal_strength = 1;
    else if (*signal_strength >100)
        *signal_strength =100;

#else

// Version: Min:0 ~ Max:255
    if(status.bit_error_rate > 20000)
    {
        *signal_quality = 1;
    }
    else
    {
        *signal_quality = (unsigned char)(((20000 - status.bit_error_rate)*255)/20000);
    }
    *signal_strength = (unsigned char)((status.signal_strength*255)/100);

    if(*signal_quality < 1)
        *signal_quality = 1;
    else if (*signal_quality >255)
        *signal_quality =255;

    if(*signal_strength < 1)
        *signal_strength = 1;
    else if (*signal_strength >255)
        *signal_strength =255;

#endif

#else

    *signal_quality = 50;
    *signal_strength = 50;

#endif
    //printf("signal_quality = %d signal_strength = %d\n", *signal_quality, *signal_strength);
}

