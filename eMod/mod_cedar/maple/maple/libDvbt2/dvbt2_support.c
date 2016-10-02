
#include "maple_i.h"
#include "dvbt2_context.h"

extern maple_t* g_maple;
//*****************************************************************************//
//**************** Functions outside provide to DVB SI library ****************//
//*****************************************************************************//

//* function dvbt2 si lib use to create a task.
__u8 dvbt2_create_task(__pTHD_t task, void* param, __u16 level)
{
    __u8 task_id;
    task_id = esKRNL_TCreate(task, param, 0x8000, level);
    return task_id;
}


//* function dvbt2 si lib use to delete a task.
__u8 dvbt2_delete_task(__u8 task_id)
{
    if (task_id != 0xff)
    {
        esKRNL_TimeDlyResume(task_id);
        while(esKRNL_TDelReq(task_id) != OS_TASK_NOT_EXIST)
        {
            esKRNL_TimeDlyResume(task_id);
            esKRNL_TimeDly(10);
        }
    }
    else
    {
        esKRNL_TDel(task_id);
    }

    return 0;
}


//* function dvbt2 si lib use to check task quit request.
__s32 dvbt2_check_task_delete_request(__u8 taskId)
{
    return esKRNL_TDelReq(taskId);
}


//* dvbt2 si lib need this function to open demux section filter
#if 0
__s32 dvbt2_open_section_filter(__u32 pid)
{
    __s32                   result;
    dvbt2_dmx_chan_param_t    dmxChanParam;

    dmxChanParam.dmx_type          = DMX_TYPE_SECTION;
    dmxChanParam.section_cb        = NULL;
    dmxChanParam.section_cb_param  = NULL;
    
    result = esMODS_MIoctrl(g_maple->demux, DVB_DMX_CMD_OPEN_FILTER, pid, (void*)&dmxChanParam);
    if(result < 0)
        return -1;
    else
        return 0;
}
#endif

/*****************************************************
注意:在打开PMT的PID通道时要连带SERVID一起
				传递下去过滤，因为有的节目的PMT表
				的PID是相同的，但是SERVID不同。具体
				参照13838-1中的pmt表的数据结构。

****************************************************/
__s32 dvbt2_open_section_filter(__u32 pid,__u32 servId,__s32 *pnVirtualChanId)
{
    __s32                   result;
    dvb_dmx_chan_param_t    dmxChanParam;
    __pCBK_t                requestDataCB;
    __pCBK_t                updateDataCB;

	*pnVirtualChanId = -1;
    dmxChanParam.dmx_type = DMX_TYPE_SECTION;

    switch(pid)
    {
       case  PAT_PID:
             {
                requestDataCB = esKRNL_GetCallBack(dvbt2_pat_request_buffer);
                updateDataCB  = esKRNL_GetCallBack(dvbt2_pat_update_data);
                break;
             }
       case  CAT_PID:     
             {  
     //           requestDataCB = esKRNL_GetCallBack(dvbt2_cat_request_buffer);         
     //           updateDataCB  = esKRNL_GetCallBack(dvbt2_cat_update_data);
                break;
             }
       case  TSDT_PID:
             {
       //         requestDataCB = esKRNL_GetCallBack(dvbt2_tsdt_request_buffer);         
       //         updateDataCB  = esKRNL_GetCallBack(dvbt2_tsdt_update_data);
                break;
             }
       case  NIT_PID:
             {
               requestDataCB = esKRNL_GetCallBack(dvbt2_nit_request_buffer);         
               updateDataCB  = esKRNL_GetCallBack(dvbt2_nit_update_data);
                break;
             }
       case  SDT_BAT_PID:
             {
                requestDataCB = esKRNL_GetCallBack(dvbt2_sdt_request_buffer);         
                updateDataCB  = esKRNL_GetCallBack(dvbt2_sdt_update_data);
                break;
             }
       case  EIT_PID:
             {
                requestDataCB = esKRNL_GetCallBack(dvbt2_eit_request_buffer);         
                updateDataCB  = esKRNL_GetCallBack(dvbt2_eit_update_data);
                break;
             }
       case  RST_PID:
             {
      //          requestDataCB = esKRNL_GetCallBack(dvbt2_rst_request_buffer);         
      //          updateDataCB  = esKRNL_GetCallBack(dvbt2_rst_update_data);
                break;
             }
       case  TDT_TOT_PID:
             {
                requestDataCB = esKRNL_GetCallBack(dvbt2_tdt_request_buffer);         
                updateDataCB  = esKRNL_GetCallBack(dvbt2_tdt_update_data);
                break;
             }
       case  DIT_PID:
             {
      //          requestDataCB = esKRNL_GetCallBack(dvbt2_dit_request_buffer);         
      //          updateDataCB  = esKRNL_GetCallBack(dvbt2_dit_update_data);
                break;
             }
       case  SIT_PID:
             {
      //          requestDataCB = esKRNL_GetCallBack(dvbt2_sit_request_buffer);         
     //           updateDataCB  = esKRNL_GetCallBack(dvbt2_sit_update_data);
                break;
             }
       default:
           {
               requestDataCB = esKRNL_GetCallBack(dvbt2_pmt_request_buffer);         
               updateDataCB  = esKRNL_GetCallBack(dvbt2_pmt_update_data);
               break;
           }
    }
    dmxChanParam.request_buffer_cb  = requestDataCB;
    dmxChanParam.update_data_cb     = updateDataCB;
    dmxChanParam.servId = servId;
    result = esMODS_MIoctrl(g_maple->demux, 
                            DVB_DMX_CMD_OPEN_FILTER, 
                            pid, 
                            (void*)&dmxChanParam);
    if(result < 0)
    {
    	*pnVirtualChanId = -1;
        return -1;
    }
	else
    {
    	*pnVirtualChanId = dmxChanParam.nVirtualChanId;
    	return 0;
	}
}

//* dvbt2 si lib need this function to close demux section filter  
__s32 dvbt2_close_section_filter(__u32 pid,__s32 nVirtualChanId)
{
    __s32 result;

    result = esMODS_MIoctrl(g_maple->demux, DVB_DMX_CMD_CLOSE_FILTER, pid, (void*)nVirtualChanId);
    
    if(result < 0)
        return -1;
    else
        return 0;
}


//* dvbt2 si lib need this function to read section data
#if 0
__s32 dvbt2_read_section_data(__u32 pid, __u8* buf, __u8 checkCrcFlag)
{
    __u32 rdSize;

    rdSize = esMODS_MRead(buf, 0, pid, g_maple->demux);
    if(rdSize > 0 && checkCrcFlag == 1)
    {
        if(Crc32Compute(0xffffffff, buf, rdSize) != 0)
        {
            WARNING("crc check error.\n");
            return -1;
        }
		else
            return rdSize;
    }
    else
        return rdSize;
}
#endif


//* event callback functions dvb si lib will call when event happen.
//* callback function to notify one service searched.
void dvbt2_search_one_service_cb(maple_serv_item_t* newItem)
{
    DISABLE_OS_TASK_SWITCH();
    if(g_maple->sl_event_cb)
    {
        if(g_maple->sl_event_cb_param)
            memcpy(&g_maple->sl_event_cb_param->serv_item, newItem, sizeof(maple_serv_item_t));
        esKRNL_CallBack(g_maple->sl_event_cb, (void*)g_maple->sl_event_cb_param);
    }
    ENABLE_OS_TASK_SWITCH();
    return;
}

//* callback function to notify service search finish.
void dvbt2_search_service_finish_cb(void)
{
    DISABLE_OS_TASK_SWITCH();
    if(g_maple->sl_finish_cb)
    {
        esKRNL_CallBack(g_maple->sl_finish_cb, (void*)g_maple->sl_finish_cb_param);
    }
    ENABLE_OS_TASK_SWITCH();
    return;
}

//* callback function to notify simple EPG searchi finish.
void dvbt2_search_sepg_finish_cb(maple_sepg_t* sepg)
{
    DISABLE_OS_TASK_SWITCH();
    if(g_maple->sepg_finish_cb)
    {
        if(g_maple->sepg_cb_param)
            memcpy(&g_maple->sepg_cb_param->sepg, sepg, sizeof(maple_sepg_t));
        esKRNL_CallBack(g_maple->sepg_finish_cb, (void*)g_maple->sepg_cb_param);
    }
    ENABLE_OS_TASK_SWITCH();
    return;
}

//* callback function to notify one schefule event searched.
void dvbt2_search_one_schedule_cb(maple_schdl_event_t* newEvent)
{
    DISABLE_OS_TASK_SWITCH();
    if(g_maple->schdl_event_cb)
    {
        if(g_maple->schdl_event_cb_param)
            memcpy(&g_maple->schdl_event_cb_param->event, newEvent, sizeof(maple_schdl_event_t));
        esKRNL_CallBack(g_maple->schdl_event_cb, (void*)g_maple->schdl_event_cb_param);
    }
    ENABLE_OS_TASK_SWITCH();
    return;
}

//* callback function to notify schedule search1 finish.
void dvbt2_search_schedule_finish_cb(void)
{
    DISABLE_OS_TASK_SWITCH();
    if(g_maple->schdl_finish_cb)
    {
        esKRNL_CallBack(g_maple->schdl_finish_cb, (void*)g_maple->schdl_finish_cb_param);
    }
    ENABLE_OS_TASK_SWITCH();
    return;
}
//* function use to control the tuner
//* function use to tune.
__s32 dvbt2_tune(maple_demod_info_t* demod_pm)
{
    __s32 result;
    tune_param_t tp;
    tp.frequency = demod_pm->frequency;
    tp.bandwidth = demod_pm->bandwidth;
    result = esMODS_MIoctrl(g_maple->demod, DEMOD_TUNE, 0, &tp);
		WARNING("result = %d\n", result);
    if(result != 0)
        return -1;
    else
        return 0;
}

//* function use to get current tuner parameter.
__s32 dvbt2_get_tuner_param(maple_demod_info_t* demod_pm)
{
    tune_param_t tp;
    esMODS_MIoctrl(g_maple->demod, DEMOD_GET_CUR_TUNE_PARAM, 0, (void*)&tp);
    demod_pm->frequency = tp.frequency;
    demod_pm->bandwidth = tp.bandwidth;
    demod_pm->plpnumber = tp.plpnumber;
    demod_pm->plpindex  = tp.plpindex;
	demod_pm->t_t2_flag = tp.t_t2_flag;
	WARNING("devt2_get_tuner_param: demod_pm->t_t2_flag = %d\n", demod_pm->t_t2_flag);
    return 0;
}

__s32 dvbt2_set_tuner_plpindex(maple_demod_info_t* demod_pm)
{
    __s32 result;
    tune_param_t tp;
    tp.frequency = demod_pm->frequency;
    tp.bandwidth = demod_pm->bandwidth;
	tp.plpnumber = demod_pm->plpnumber;
	tp.plpindex  = demod_pm->plpindex;
	WARNING("DEMOD_TUNE_T2_SEARCH: freq: %d, plpnumber: %d, plpindex: %d--\n", tp.frequency, tp.plpnumber, tp.plpindex);
    result = esMODS_MIoctrl(g_maple->demod, DEMOD_TUNE_T2_SEARCH, 0, &tp);
    
    if(result != 0)
        return -1;
    else
        return 0;	
}
__s32 dvbt2_get_plppid(maple_demod_info_t* demod_pm)
{
    tune_param_t tp;
    tp.frequency = demod_pm->frequency;
    tp.bandwidth = demod_pm->bandwidth;
		tp.plpnumber = demod_pm->plpnumber;
		tp.plpindex  = demod_pm->plpindex;	
    esMODS_MIoctrl(g_maple->demod, DEMOD_GET_TUNE_PLPPID, 0, (void*)&tp);
    demod_pm->plppid = tp.plppid;
    return 0;
}	
