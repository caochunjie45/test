
#ifndef TSC_CONFIG_H
#define TSC_CONFIG_H

//#include "maple_common.h"
#include "mod_cedar_i.h"
//#include "maple_config.h"

#include "tsc_interface.h"

#define TSC_DEBUG                       (1)
#define TSC_SHOW_REGISTER_SETTING       (0)
#define USE_MMU                         (1)
                                        
#define TSC_CHAN_NUM                    32  //* channel number for one port, there are two ports in F20 tsc module.

#define TSF_INTR_THRESHOLD              1   //* 0: 1/2 buffer size; 1: 1/4 buffer size
                                            //* 2: 1/8 buffer size; 3: 1/16 buffer size
                                            
#define TSC_CLOCK_SOURCE_SELECT         1   //* 0: core pll, 1: dram pll
#define TSC_CLOCK_DIVID_RATIO           2   //* only support 4/8/16/32 divide ratio

#if TSC_CLOCK_SOURCE_SELECT
//#define TSC_CLOCK_SOURCE                CSP_CCM_SYS_CLK_SDRAM_PLL		//aw1620 
#define TSC_CLOCK_SOURCE                CCMU_SYS_CLK_SDRAM_PLL_P  //aw 1625
#else
//#define TSC_CLOCK_SOURCE                CSP_CCM_SYS_CLK_CORE_PLL
#define TSC_CLOCK_SOURCE                CCMU_SYS_CLK_CORE_PLL

#endif


//#define VIDEO_CHAN_BASE             (0)
//#define MAX_VIDEO_CHAN              (1)
//
//#define AUDIO_CHAN_BASE             (VIDEO_CHAN_BASE + MAX_VIDEO_CHAN)
//#define MAX_AUDIO_CHAN              (1)
//
//#define SUBTITLE_CHAN_BASE          (AUDIO_CHAN_BASE + MAX_AUDIO_CHAN)
//#define MAX_SUBTITLE_CHAN           (1)
//
//#define SECTION_CHAN_BASE           (SUBTITLE_CHAN_BASE + MAX_SUBTITLE_CHAN)
//#define MAX_SECTION_CHAN            (12)
//
//#define TS_DATA_CHAN_BASE           (SECTION_CHAN_BASE + MAX_SECTION_CHAN)
//#define MAX_TS_DATA_CHAN            (15)
//
//#define VIDEO_NOTIFY_PKT_NUM        (1024)
//#define AUDIO_NOTIFY_PKT_NUM        (64)
//#define SUBTITLE_NOTIFY_PKT_NUM     (64)
//#define SECTION_NOTIFY_PKT_NUM      (64)
//#define TS_DATA_NOTIFY_PKT_NUM      (64)


#define NEW_SWITCH_METHOD           (1)

#endif
