#ifndef _RDA5880H_USER_DEFINE_
#define _RDA5880H_USER_DEFINE_
RDA5880_ERR_MSG HAL_TUNER_Write( UINT8 dev_addr, UINT8 *reg, UINT8 reg_num, UINT8 *data, UINT8 num );
RDA5880_ERR_MSG HAL_TUNER_ReadbyAddr( UINT8 dev_addr, UINT8 reg, UINT8 *data, UINT8 num ) ;
void SAL_PLT_DelayMS( UINT32 deley_t ) ;

#endif

