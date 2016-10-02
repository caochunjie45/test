/****************************************************************************
*
*	MB86A22 Optimazed parameter for RDA5880HT
*
*  Copyright (c) 2013 Fujitsu Semiconductor 
*  All rights reserved. 
*
*  Created Aug.16.2013
*
****************************************************************************/

//#define 
#ifndef _MB86A22_RDA_DEFAULT_SETTING_H_
#define _MB86A22_RDA_DEFAULT_SETTING_H_

#define RF_Xtal_freq_default          26000
#define MB86A2x_CH_UNIT_DEFAULT       MB86A2x_CH_UNIT_BZ
#define MB86A2x_CH_DEFAULT            14
#define RF_freq_kHz_DEFAULT           473143

#define IQ_pol_DEFAULT                0	/* value after reset=0 */
#define POWER_DOWN_DEFAULT            0
#define RF_CONT_EN_DEFAULT            1

#define INPUT_CLK_FREQ_kHz_DEFAULT   26000
#define REFSEL_DEFAULT                MB86A2x_REFSEL_26MHz
#define PLLPASS_DEFAULT               0
#define BANDSEL_DEFAULT               MB86A2x_BANDSEL_ISDBT_6MHz
#define FREQSEL_DEFAULT               MB86A2x_FREQSEL_0

#define RECEP_LAYER_DEFAULT           MB86A2x_RECEP_HIGHEST

#define MAIF_DEFAULT                  0	/* value after reset=0 */
#define IFAGC_POL_DEFAULT             1	/* value after reset=1 */
#define RFAGC_ON_DEFAULT              0

#define VIFREF_DEFAULT                67	/* value after reset=50(d) */
#define AGAIN_DEFAULT                 140	/* value after reset=120(d) */
#define DTS_DEFAULT                   16	    /* value after reset=255(d) */
#define IFA_DEFAULT                   1656	/* value after reset=53(d) */
#define IFB_DEFAULT                   3800	/* value after reset=3800(d) */
#define IFAGCO_DEFAULT                0		/* value after reset=0(d) */
#define MAXIFAGC_DEFAULT              255	/* value after reset=0(d) */
#define IFSAMPLE_DEFAULT              100	/* value after reset=100(d) */
#define OUTSAMPLE_DEFAULT             100	/* value after reset=100(d) */
#define VMAIF_DEFAULT                 0		/* value after reset=0(d) */

#define VMARF_DEFAULT                 0		/* value after reset=0(d) */

#define S1CNT_DEFAULT                 100	/* value after reset=0(d) */
#define MULSEL_DEFAULT                2		/* value after reset=2(d) for A20S Only */

#define TS_OUT_SEL_DEFAULT            MB86A2x_TS_OUT_SERIAL
#define TS_LAYER_DEFAULT              MB86A2x_TS_LAYER_ALL
#define TS_CLK_DEFALT                 MB86A2x_TS_CLK_NEG
#define TS_ERR_MASK_DEFAULT           MB86A2x_TS_ERR_MASK_OFF
#define TS_PARITY_DEFAULT             MB86A2x_TS_RS_PARITY_OFF
#define TS_SYNC_DEFAULT               MB86A2x_TS_SYNC_ON

#define VBER_A_NUM_DEFAULT            0x000fff
#define VBER_B_NUM_DEFAULT            0x00ffff
#define VBER_C_NUM_DEFAULT            0x00ffff
#define SBER_A_NUM_DEFAULT            0x0003ff
#define SBER_B_NUM_DEFAULT            0x003fff
#define SBER_C_NUM_DEFAULT            0x003fff
#define PBER_A_NUM_DEFAULT            0x0003ff
#define PBER_B_NUM_DEFAULT            0x003fff
#define PBER_C_NUM_DEFAULT            0x003fff
#define CN_SYMBOL_DEFAULT             0x04
#define MER_SYMBOL_DEFAULT            0x04

#define START_CH_DEFAULT              7
#define STOP_CH_DEFAULT               69
#define WAIT_CH_SCH_STAT_DEFAULT      30
#define WAIT_CH_SCH_SEQ_DEFAULT       30
#define SCH_LOOP_MAX_DEFAULT          50

#endif

