#include "fc8300_api.h"

#include "demod/fc8300_regs.h"
#include "demod/bbm.h"
#include "demod/fci_types.h"
#include "demod/fci_tun.h"
#include "demod/fci_oal.h"



int broadcast_type;

static __s32 demod_power_onoff(__bool b_en)
{
	__s32 ret;
	ES_FILE *h_power = eLIBs_fopen("b:\\HID\\POWER", "w");

	if( b_en )
		ret = eLIBs_fioctrl(h_power, DRV_POWER_CMD_PWREPT_SET, POWER_EPT_3, (void *)((POWER_EPT_MOS_P) | (POWER_EPT_GATE_ON))); 
	else
    	ret = eLIBs_fioctrl(h_power, DRV_POWER_CMD_PWREPT_SET, POWER_EPT_3, (void *)((POWER_EPT_MOS_N) | (POWER_EPT_GATE_OFF)));
	
	if(ret)
	{
		eDbug("~~~~fail\n");
	}
	else
	{
		eDbug("~~~~ok\n");
		eLIBs_fclose(h_power);
   	}
	
	return 0;
}

void mtv_power_on(void)
{
	__hdle hdl_1 = NULL;
	__user_pin_set_t  gpio_set[1];

	eDbug( "FC8300  mtv_power_on\n");

	gpio_set->Port = 3;
	gpio_set->PortNum = 14;
	gpio_set->MultiSel = 1;
	gpio_set->Pull = 1;
	gpio_set->DrvLevel = 1;
	gpio_set->Data = 0;
	hdl_1 = esPINS_PinGrpReq(gpio_set, 1);

	//reset low
	esKRNL_TimeDly(1);
	esPINS_WritePinData(hdl_1, 0, NULL);
	esKRNL_TimeDly(1);
	
	//power on
	demod_power_onoff(1);
	msWait(5);
	
	//reset high
	esPINS_WritePinData(hdl_1, 1, NULL);	
	msWait(5);

	esPINS_PinGrpRel(hdl_1, 1);
	
}
void mtv_power_off(void)
{
	__hdle hdl_1 = NULL;
	__user_pin_set_t  gpio_set[1];

	eDbug( "FC8300  mtv_power_off\n");
	//reset low

	gpio_set->Port = 3;
	gpio_set->PortNum = 14;
	gpio_set->MultiSel = 1;
	gpio_set->Pull = 1;
	gpio_set->DrvLevel = 1;
	gpio_set->Data = 0;
	hdl_1 = esPINS_PinGrpReq(gpio_set, 1);

	esPINS_WritePinData(hdl_1, 0, NULL);	
	msWait(5);

	esPINS_PinGrpRel(hdl_1, 1);

}

int mtv_init(int mode)
{
	int	res = BBM_OK;

	eDbug( "FC8300  mtv_init mode : %d\n", mode);

	mtv_power_on();
	res = bbm_com_hostif_select(NULL, BBM_I2C);
	res |= bbm_com_i2c_init(NULL, FCI_HPI_TYPE);

	if (res) {
		eDbug( "FC8300 bbm_com_i2c_init Initialize Fail \n");
		return BBM_NOK;
	}

	res |= bbm_com_probe(NULL, DIV_BROADCAST);
	if (res) {
		eDbug( "FC8300  bbm_com_probe Fail \n");
		return BBM_NOK;
	}

	res |= bbm_com_init(NULL, DIV_BROADCAST);
	res |= bbm_com_tuner_select(NULL, DIV_BROADCAST, FC8300B_TUNER, mode);

	broadcast_type = mode;
	if(res)
		eDbug( "FC8300  initialize Fail \n");

	return res;
}

int mtv_deinit(void)
{
	int res = BBM_OK;

	eDbug( "FC8300  mtv_deinit \n");

	res = bbm_com_hostif_deselect(NULL);

	mtv_power_off();

	return res;
}

int mtv_set_channel(__s32 freq, __u8 subch_number)
{
	int res = BBM_OK;

	eDbug( "FC8300  mtv_set_channel freq : %d, subch : %d\n", freq, subch_number);

	if((broadcast_type < 1) || (broadcast_type > 3))
		subch_number = 0x16;

	res = bbm_com_tuner_set_freq(NULL, DIV_BROADCAST, freq, subch_number);

	if(res)
		eDbug( "FC8300 mtv_set_channel Fail!! \n");

	return res;
}

int mtv_lock_check(void)
{
	int res = BBM_OK;

	eDbug( "FC8300  mtv_lock_check\n");

	res = bbm_com_scan_status(NULL, DIV_BROADCAST);

	if(res)
		eDbug( "FC8300 mtv_lock_check Fail!! \n");

	return res;
}

void mtv_get_ews_flag(__u8 *ews)
{
	__u8 reg;

	bbm_com_byte_read(NULL, DIV_BROADCAST, 0x4113, &reg);

	*ews = (reg & 0x04) >> 2;

	return;
}

void mtv_monitor_mfd(void)
{
	__u8 mfd_status;
	__u8 mfd_value;
	static __u8 master = 0;
#if defined(BBM_2_DIVERSITY) || defined(BBM_4_DIVERSITY)
	static __u8 slave0 = 0;
#endif
#if defined(BBM_4_DIVERSITY)
	static __u8 slave1 = 0, slave2 = 0;
#endif

	bbm_com_read(NULL, DIV_MASTER, 0x4064, &mfd_status);

	if (mfd_status & 0x01) {
		bbm_com_read(NULL, DIV_MASTER, 0x4065, &mfd_value);

		if (mfd_value < 12) {
			if (master != 1) {
				bbm_com_byte_write(NULL, DIV_MASTER, 0x3022, 0x0b);
				bbm_com_byte_write(NULL, DIV_MASTER, 0x255c, 0x10);
				bbm_com_byte_write(NULL, DIV_MASTER, 0x2542, 0x04);
				master = 1;
			}
		} else {
			if (master != 2) {
				bbm_com_byte_write(NULL, DIV_MASTER, 0x3022, 0x0f);
				bbm_com_byte_write(NULL, DIV_MASTER, 0x255c, 0x3f);
				bbm_com_byte_write(NULL, DIV_MASTER, 0x2542, 0x10);
				master = 2;
			}
		}
	}

#if defined(BBM_2_DIVERSITY) || defined(BBM_4_DIVERSITY)
	bbm_com_read(NULL, DIV_SLAVE0, 0x4064, &mfd_status);

	if (mfd_status & 0x01) {
		bbm_com_read(NULL, DIV_SLAVE0, 0x4065, &mfd_value);

		if (mfd_value < 12) {
			if (slave0 != 1) {
				bbm_com_byte_write(NULL, DIV_SLAVE0, 0x3022, 0x0b);
				bbm_com_byte_write(NULL, DIV_SLAVE0, 0x255c, 0x10);
				bbm_com_byte_write(NULL, DIV_SLAVE0, 0x2542, 0x04);
				slave0 = 1;
			}
		} else {
			if (slave0 != 2) {
				bbm_com_byte_write(NULL, DIV_SLAVE0, 0x3022, 0x0f);
				bbm_com_byte_write(NULL, DIV_SLAVE0, 0x255c, 0x3f);
				bbm_com_byte_write(NULL, DIV_SLAVE0, 0x2542, 0x10);
				slave0 = 2;
			}
		}
	}
#endif

#if defined(BBM_4_DIVERSITY)
	bbm_com_read(NULL, DIV_SLAVE1, 0x4064, &mfd_status);

	if (mfd_status & 0x01) {
		bbm_com_read(NULL, DIV_SLAVE1, 0x4065, &mfd_value);

		if (mfd_value < 12) {
			if (slave1 != 1) {
				bbm_com_byte_write(NULL, DIV_SLAVE1, 0x3022, 0x0b);
				bbm_com_byte_write(NULL, DIV_SLAVE1, 0x255c, 0x10);
				bbm_com_byte_write(NULL, DIV_SLAVE1, 0x2542, 0x04);
				slave1 = 1;
			}
		} else {
			if (slave1 != 2) {
				bbm_com_byte_write(NULL, DIV_SLAVE1, 0x3022, 0x0f);
				bbm_com_byte_write(NULL, DIV_SLAVE1, 0x255c, 0x3f);
				bbm_com_byte_write(NULL, DIV_SLAVE1, 0x2542, 0x10);
				slave1 = 2;
			}
		}
	}

	bbm_com_read(NULL, DIV_SLAVE2, 0x4064, &mfd_status);

	if (mfd_status & 0x01) {
		bbm_com_read(NULL, DIV_SLAVE2, 0x4065, &mfd_value);

		if (mfd_value < 12) {
			if (slave2 != 1) {
				bbm_com_byte_write(NULL, DIV_SLAVE2, 0x3022, 0x0b);
				bbm_com_byte_write(NULL, DIV_SLAVE2, 0x255c, 0x10);
				bbm_com_byte_write(NULL, DIV_SLAVE2, 0x2542, 0x04);
				slave2 = 1;
			}
		} else {
			if (slave2 != 2) {
				bbm_com_byte_write(NULL, DIV_SLAVE2, 0x3022, 0x0f);
				bbm_com_byte_write(NULL, DIV_SLAVE2, 0x255c, 0x3f);
				bbm_com_byte_write(NULL, DIV_SLAVE2, 0x2542, 0x10);
				slave2 = 2;
			}
		}
	}
#endif
}

void mtv_signal_measure(__u8 *lock, double *CN
	, double *BER_A, double *PER_A
	, double *BER_B, double *PER_B
	, double *BER_C, double *PER_C, __s32 *i32RSSI)
{
	__u8 msnr_cn;
	__u8 data;
	__u16 ver;

	struct dm_st {
		__u8	start;
		__s8	rssi;
		__u8	sync_0;
		__u8	sync_1;

		__u8	fec_on;
		__u8	fec_layer;
		__u8	wscn;
		__u8	reserved;

		__u16 vit_a_ber_rxd_rsps;
		__u16 vit_a_ber_err_rsps;
		__u32 vit_a_ber_err_bits;

		__u16 vit_b_ber_rxd_rsps;
		__u16 vit_b_ber_err_rsps;
		__u32 vit_b_ber_err_bits;

		__u16 vit_c_ber_rxd_rsps;
		__u16 vit_c_ber_err_rsps;
		__u32 vit_c_ber_err_bits;

		__u16 reserved0;
		__u16 reserved1;
		__u32 reserved2;

		__u32 dmp_a_ber_rxd_bits;
		__u32 dmp_a_ber_err_bits;

		__u32 dmp_b_ber_rxd_bits;
		__u32 dmp_b_ber_err_bits;

		__u32 dmp_c_ber_rxd_bits;
		__u32 dmp_c_ber_err_bits;

		__u32 reserved3;
		__u32 reserved4;
	} dm;

	bbm_com_bulk_read(NULL, DIV_BROADCAST, BBM_DM_DATA, (__u8*) &dm, sizeof(dm));
	bbm_com_byte_read(NULL, DIV_BROADCAST, 0x403d, &msnr_cn);
	bbm_com_read(NULL, DIV_BROADCAST, 0x50c5, &data);
	bbm_com_tuner_get_rssi(NULL, DIV_BROADCAST, i32RSSI);
	bbm_com_word_read(NULL, DIV_BROADCAST, 0x0026, &ver);

	dm.vit_a_ber_err_bits &= 0x00ffffff;
	dm.vit_b_ber_err_bits &= 0x00ffffff;
	dm.vit_c_ber_err_bits &= 0x00ffffff;

	dm.dmp_a_ber_err_bits &= 0x00ffffff;
	dm.dmp_b_ber_err_bits &= 0x00ffffff;
	dm.dmp_c_ber_err_bits &= 0x00ffffff;

	if(data)
		*lock = 1;
	else
		*lock = 0;

	if (dm.vit_a_ber_rxd_rsps)
		*PER_A = ((double) dm.vit_a_ber_err_rsps / (double) dm.vit_a_ber_rxd_rsps);
	else
		*PER_A = 1;

	if (dm.vit_b_ber_rxd_rsps)
		*PER_B = ((double) dm.vit_b_ber_err_rsps / (double) dm.vit_b_ber_rxd_rsps);
	else
		*PER_B = 1;

	if (dm.vit_c_ber_rxd_rsps)
		*PER_C = ((double) dm.vit_c_ber_err_rsps / (double) dm.vit_c_ber_rxd_rsps);
	else
		*PER_C = 1;

	if (dm.dmp_a_ber_rxd_bits)
		*BER_A = ((double) dm.dmp_a_ber_err_bits / (double) dm.dmp_a_ber_rxd_bits);
	else
		*BER_A = 1;

	if (dm.dmp_b_ber_rxd_bits)
		*BER_B = ((double) dm.dmp_b_ber_err_bits / (double) dm.dmp_b_ber_rxd_bits);
	else
		*BER_B = 1;

	if (dm.dmp_c_ber_rxd_bits)
		*BER_C = ((double) dm.dmp_c_ber_err_bits / (double) dm.dmp_c_ber_rxd_bits);
	else
		*BER_C = 1;

	*CN = msnr_cn / 4;
	mtv_monitor_mfd();

	__msg( "FC8300 mtv_signal_measure berA : %d \n, perA : %d \n \
				, berB : %d \n, perB : %d \n, berC : %d \n, perC : %d \n \
				, cn : %d \n, rssi : %d \n, lock : %d \n, version : 0x%x\n"
				,(__u32) *BER_A,(__u32) *PER_A, (__u32)*BER_B,(__u32) *PER_B, (__u32)*BER_C,(__u32) *PER_C
					,(__u32) *CN, (__u32)*i32RSSI, *lock, ver);

	return;
}

