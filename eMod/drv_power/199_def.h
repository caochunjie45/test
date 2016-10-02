/*
**********************************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              eBIOS Sub-System
*
*                                   (c) Copyright 2007-2009, Steven.ZGJ.China
*                                             All Rights Reserved
*
* Moudle  : power bios module
* File    : power_i.h
* By      : Jerry
* Version : v1.00
* Date    : 2008-8-19 18:38:16
**********************************************************************************************************************
*/
#ifndef _P1_DEF_H_
#define _P1_DEF_H_

//define AXP19 REGISTER
#define POWER19_STATUS              (0x00)
#define POWER19_MODE_CHGSTATUS      (0x01)
#define POWER19_OTG_STATUS          (0x02)
#define POWER19_IC_TYPE          		(0x03)
#define POWER19_DATA_BUFFER1        (0x06)
#define POWER19_DATA_BUFFER2        (0x07)
#define POWER19_DATA_BUFFER3        (0x08)
#define POWER19_DATA_BUFFER4        (0x09)
#define POWER19_VERSION             (0x0C)
#define POWER19_LDO3_DC2_CTL        (0x10)
#define POWER19_LDO24_DC13_CTL      (0x12)
#define POWER19_DC2OUT_VOL          (0x23)
#define POWER19_LDO3_DC2_DVM        (0x25)
#define POWER19_DC1OUT_VOL          (0x26)
#define POWER19_DC3OUT_VOL          (0x27)
#define POWER19_LDO24OUT_VOL        (0x28)
#define POWER19_LDO3OUT_VOL         (0x29)
#define POWER19_IPS_SET             (0x30)
#define POWER19_VOFF_SET            (0x31)
#define POWER19_OFF_CTL             (0x32)
#define POWER19_CHARGE1             (0x33)
#define POWER19_CHARGE2             (0x34)
#define POWER19_BACKUP_CHG          (0x35)
#define POWER19_POK_SET             (0x36)
#define POWER19_DCDC_FREQSET        (0x37)
#define POWER19_VLTF_CHGSET         (0x38)
#define POWER19_VHTF_CHGSET         (0x39)
#define POWER19_APS_WARNING1        (0x3A)
#define POWER19_APS_WARNING2        (0x3B)
#define POWER19_VLTF_DISCHGSET      (0x3C)
#define POWER19_VHTF_DISCHGSET      (0x3D)
#define POWER19_DCDC_MODESET        (0x80)
#define POWER19_VOUT_MONITOR        (0x81)
#define POWER19_ADC_EN1             (0x82)
#define POWER19_ADC_EN2             (0x83)
#define POWER19_ADC_SPEED           (0x84)
#define POWER19_ADC_INPUTRANGE      (0x85)
#define POWER19_TIMER_CTL           (0x8A)
#define POWER19_VBUS_DET_SRP        (0x8B)
#define POWER19_HOTOVER_CTL         (0x8F)
#define POWER19_GPIO0_CTL           (0x90)
#define POWER19_GPIO0_VOL           (0x91)
#define POWER19_GPIO1_CTL           (0x92)
#define POWER19_GPIO2_CTL           (0x93)
#define POWER19_GPIO012_SIGNAL      (0x94)
#define POWER19_SENSE_CTL           (0x95)
#define POWER19_SENSE_SIGNAL        (0x96)
#define POWER19_GPIO20_PDCTL        (0x97)
#define POWER19_PWM1_FREQ           (0x98)
#define POWER19_PWM1_DUTYDE         (0x99)
#define POWER19_PWM1_DUTY           (0x9A)
#define POWER19_PWM2_FREQ           (0x9B)
#define POWER19_PWM2_DUTYDE         (0x9C)
#define POWER19_PWM2_DUTY           (0x9D)
#define POWER19_RSTO_CTL            (0x9E)
#define POWER19_GPIO67_CTL          (0x9F)
#define POWER19_INTEN1              (0x40)
#define POWER19_INTEN2              (0x41)
#define POWER19_INTEN3              (0x42)
#define POWER19_INTEN4              (0x43)
#define POWER19_INTSTS1             (0x44)
#define POWER19_INTSTS2             (0x45)
#define POWER19_INTSTS3             (0x46)
#define POWER19_INTSTS4             (0x47)
#define POWER19_COULOMB_CTL         (0xB8)

//adc data register
#define POWER19_BAT_AVERVOL_H8          (0x78)
#define POWER19_BAT_AVERVOL_L4          (0x79)
#define POWER19_BAT_AVERCHGCUR_H8       (0x7A)
#define POWER19_BAT_AVERCHGCUR_L5       (0x7B)
#define POWER19_ACIN_VOL_H8             (0x56)
#define POWER19_ACIN_VOL_L4             (0x57)
#define POWER19_ACIN_CUR_H8             (0x58)
#define POWER19_ACIN_CUR_L4             (0x59)
#define POWER19_VBUS_VOL_H8             (0x5A)
#define POWER19_VBUS_VOL_L4             (0x5B)
#define POWER19_VBUS_CUR_H8             (0x5C)
#define POWER19_VBUS_CUR_L4             (0x5D)
#define POWER19_BAT_AVERDISCHGCUR_H8    (0x7C)
#define POWER19_BAT_AVERDISCHGCUR_L5    (0x7D)
#define POWER19_APS_AVERVOL_H8          (0x7E)
#define POWER19_APS_AVERVOL_L4          (0x7F)
#define POWER19_BAT_CHGCOULOMB3         (0xB0)
#define POWER19_BAT_CHGCOULOMB2         (0xB1)
#define POWER19_BAT_CHGCOULOMB1         (0xB2)
#define POWER19_BAT_CHGCOULOMB0         (0xB3)
#define POWER19_BAT_DISCHGCOULOMB3      (0xB4)
#define POWER19_BAT_DISCHGCOULOMB2      (0xB5)
#define POWER19_BAT_DISCHGCOULOMB1      (0xB6)
#define POWER19_BAT_DISCHGCOULOMB0      (0xB7)
#define POWER19_BAT_POWERH8             (0x70)
#define POWER19_BAT_POWERM8             (0x71)
#define POWER19_BAT_POWERL8             (0x72)
#define POWER19_BAT_INT_CHG_CURR_H8          (0xA0)
#define POWER19_BAT_INT_CHG_CURR_L4          (0xA1)
#define POWER19_BAT_EXT_CHG_CURR_H8          (0xA2)
#define POWER19_BAT_EXT_CHG_CURR_L4          (0xA3)
#define POWER19_BAT_INT_DISCHG_CURR_H8          (0xA4)
#define POWER19_BAT_INT_DISCHG_CURR_L4          (0xA5)
#define POWER19_BAT_EXT_DISCHG_CURR_H8          (0xA6)
#define POWER19_BAT_EXT_DISCHG_CURR_L4          (0xA7)


#define BAT_CAP                     1645
#define RDC_COUNT                   5
#define RDC_DEFAULT                 300
#define END_VOLTAGE_APS             3350
#define END_VOLTAGE_BAT             0
#define FUELGUAGE_LOW_VOL           3400    //<3.4v,2%
#define FUELGUAGE_VOL1              3500    //<3.5v,3%
#define FUELGUAGE_VOL2              3600
#define FUELGUAGE_VOL3              3700
#define FUELGUAGE_VOL4              3800
#define FUELGUAGE_VOL5              3900
#define FUELGUAGE_VOL6              4000
#define FUELGUAGE_VOL7              4100
#define FUELGUAGE_TOP_VOL           4160    //>4.16v,100%

#define FUELGUAGE_LOW_LEVEL         2       //<3.4v,2%
#define FUELGUAGE_LEVEL1            3       //<3.5v,3%
#define FUELGUAGE_LEVEL2            5
#define FUELGUAGE_LEVEL3            16
#define FUELGUAGE_LEVEL4            46
#define FUELGUAGE_LEVEL5            66
#define FUELGUAGE_LEVEL6            83
#define FUELGUAGE_LEVEL7            93
#define FUELGUAGE_TOP_LEVEL         99     //>4.16v,100%

#define BAT_AVER_VOL                3820    //Aver Vol:3.82V
#define TEMP_COMPENSATION           0

#define LEVEL1                      1
#define LEVEL2                      2
#define LEVEL3                      3
#define LEVEL4                      4
#define LEVEL5                      5
#define LEVEL6                      6
#define LEVEL7                      7
#define LEVEL8                      8
#define LEVEL9                      9
#define LEVEL10                     10
#define LEVEL11                     11
#define LEVEL12                     12
#define LEVEL13                     13
#define LEVEL14                     14
#define LEVEL15                     15
#define LEVEL16                     16
#define LEVEL17                     17
#define LEVEL18                     18
#define LEVEL19                     19
#define LEVEL20                     20
#define LEVEL21                     21
#define LEVEL22                     22
#define LEVEL23                     23
#define LEVEL24                     24
#define LEVEL25                     25
#define LEVEL26                     26
#define LEVEL27                     27
#define LEVEL28                     28
#define LEVEL29                     29
#define LEVEL30                     30
#define LEVEL31                     31
#define LEVEL32                     32

//define AXP18 REGISTER
#define POWER18_STATUS              (0x00)
#define POWER18_IPS_SET             (0x01)
#define POWER18_ONOFF               (0x02)
#define POWER18_CHARGE1             (0x03)
#define POWER18_CHARGE2             (0x04)
#define POWER18_PEK                 (0x05)
#define POWER18_INTEN1              (0x06)
#define POWER18_INTEN2              (0x07)
#define POWER18_INTEN3              (0x08)
#define POWER18_INTSTS1             (0x09)
#define POWER18_INTSTS2             (0x0A)
#define POWER18_INTSTS3             (0x0B)
#define POWER18_VENDER_USED1        (0x0C)
#define POWER18_DCDCCTL             (0x0D)
#define POWER18_DC12OUT_VOL         (0x0E)
#define POWER18_LDOOUT_VOL          (0x0F)
#define POWER18_SW_CTL              (0x10)
#define POWER18_BATTERY_VOL         (0x11)
#define POWER18_BATTERY_CURRENT     (0x12)
#define POWER18_DCIN_VOL            (0x13)
#define POWER18_DCIN_CURRENT        (0x14)
#define POWER18_ADCSW_CTL           (0x15)
#define POWER18_VENDER_USED2        (0x16)
#define POWER18_EPT_SW              (0x17)
#define POWER18_DATA_BUFFER1        (0x18)
#define POWER18_DATA_BUFFER2        (0x19)
#define POWER18_VENDER_USED3        (0x1A)


//define AXP20 REGISTER
#define POWER20_STATUS              (0x00)
#define POWER20_MODE_CHGSTATUS      (0x01)
#define POWER20_OTG_STATUS          (0x02)
#define POWER20_VERSION          	(0x03)
#define POWER20_DATA_BUFFER0        (0x04)
#define POWER20_DATA_BUFFER1        (0x05)
#define POWER20_DATA_BUFFER2        (0x06)
#define POWER20_DATA_BUFFER3        (0x07)
#define POWER20_DATA_BUFFER4        (0x08)
#define POWER20_DATA_BUFFER5        (0x09)
#define POWER20_DATA_BUFFER6        (0x0a)
#define POWER20_DATA_BUFFER7        (0x0b)
#define POWER20_DATA_BUFFER8        (0x0c)
#define POWER20_DATA_BUFFER9        (0x0d)
#define POWER20_DATA_BUFFER10       (0x0e)
#define POWER20_DATA_BUFFER11       (0x0f)
#define POWER20_LDO3_DC2_CTL        (0x10)
#define POWER20_OUTPUT_CTL      		(0x12)
#define POWER20_DC2OUT_VOL          (0x23)
#define POWER20_LDO3_DC2_DVM        (0x25)
#define POWER20_DC3OUT_VOL          (0x27)
#define POWER20_LDO24OUT_VOL        (0x28)
#define POWER20_LDO3OUT_VOL         (0x29)
#define POWER20_IPS_SET             (0x30)
#define POWER20_VOFF_SET            (0x31)
#define POWER20_OFF_CTL             (0x32)
#define POWER20_CHARGE1             (0x33)
#define POWER20_CHARGE2             (0x34)
#define POWER20_BACKUP_CHG          (0x35)
#define POWER20_POK_SET             (0x36)
#define POWER20_DCDC_FREQSET        (0x37)
#define POWER20_VLTF_CHGSET         (0x38)
#define POWER20_VHTF_CHGSET         (0x39)
#define POWER20_APS_WARNING1        (0x3A)
#define POWER20_APS_WARNING2        (0x3B)
#define POWER20_VLTF_DISCHGSET      (0x3C)
#define POWER20_VHTF_DISCHGSET      (0x3D)
#define POWER20_DCDC_MODESET        (0x80)
#define POWER20_VOUT_MONITOR        (0x81)
#define POWER20_ADC_EN1             (0x82)
#define POWER20_ADC_EN2             (0x83)
#define POWER20_ADC_SPEED           (0x84)
#define POWER20_ADC_INPUTRANGE      (0x85)
#define POWER20_GPIO1ADC_VTH        (0x86)
#define POWER20_GPIO1ADC_VTL        (0x87)
#define POWER20_TIMER_CTL           (0x8A)
#define POWER20_VBUS_DET_SRP        (0x8B)
#define POWER20_HOTOVER_CTL         (0x8F)
#define POWER20_GPIO0_CTL           (0x90)
#define POWER20_GPIO0_VOL           (0x91)
#define POWER20_GPIO1_CTL           (0x92)
#define POWER20_GPIO2_CTL           (0x93)
#define POWER20_GPIO012_SIGNAL      (0x94)
#define POWER20_GPIO3_CTL           (0x95)
#define POWER20_GPIO012_PDCTL        (0x97)
#define POWER20_INTEN1              (0x40)
#define POWER20_INTEN2              (0x41)
#define POWER20_INTEN3              (0x42)
#define POWER20_INTEN4              (0x43)
#define POWER20_INTEN5              (0x44)
#define POWER20_INTSTS1             (0x48)
#define POWER20_INTSTS2             (0x49)
#define POWER20_INTSTS3             (0x4a)
#define POWER20_INTSTS4             (0x4b)
#define POWER20_INTSTS5             (0x4c)
#define POWER20_COULOMB_CTL         (0xB8)
#define POWER20_COULOMB_RESULT      (0xB9)
#define POWER20_RDC_H5      				(0xBA)
#define POWER20_RDC_L8      				(0xBB)

//adc data register
#define POWER20_BAT_AVERVOL_H8          (0x78)
#define POWER20_BAT_AVERVOL_L4          (0x79)
#define POWER20_BAT_AVERCHGCUR_H8       (0x7A)
#define POWER20_BAT_AVERCHGCUR_L4       (0x7B)
#define POWER20_ACIN_VOL_H8             (0x56)
#define POWER20_ACIN_VOL_L4             (0x57)
#define POWER20_ACIN_CUR_H8             (0x58)
#define POWER20_ACIN_CUR_L4             (0x59)
#define POWER20_VBUS_VOL_H8             (0x5A)
#define POWER20_VBUS_VOL_L4             (0x5B)
#define POWER20_VBUS_CUR_H8             (0x5C)
#define POWER20_VBUS_CUR_L4             (0x5D)
#define POWER20_BAT_AVERDISCHGCUR_H8    (0x7C)
#define POWER20_BAT_AVERDISCHGCUR_L5    (0x7D)
#define POWER20_APS_AVERVOL_H8          (0x7E)
#define POWER20_APS_AVERVOL_L4          (0x7F)
#define POWER20_BAT_CHGCOULOMB3         (0xB0)
#define POWER20_BAT_CHGCOULOMB2         (0xB1)
#define POWER20_BAT_CHGCOULOMB1         (0xB2)
#define POWER20_BAT_CHGCOULOMB0         (0xB3)
#define POWER20_BAT_DISCHGCOULOMB3      (0xB4)
#define POWER20_BAT_DISCHGCOULOMB2      (0xB5)
#define POWER20_BAT_DISCHGCOULOMB1      (0xB6)
#define POWER20_BAT_DISCHGCOULOMB0      (0xB7)
#define POWER20_BAT_POWERH8             (0x70)
#define POWER20_BAT_POWERM8             (0x71)
#define POWER20_BAT_POWERL8             (0x72)


#endif /* _P1_DEF_H_ */

