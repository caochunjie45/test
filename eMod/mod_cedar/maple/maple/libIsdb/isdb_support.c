
#include "maple_i.h"
#include "isdb_context.h"

extern maple_t* g_maple;
//*****************************************************************************//
//****************** Functions outside provide to SI library ******************//
//*****************************************************************************//

//* function isdb si lib use to create a task.
__u8  isdb_create_task(__pTHD_t task, void* param, __u16 level)
{
    __u8 task_id;
    task_id = esKRNL_TCreate(task, param, 0x8000, level);
    return task_id;
}


//* function isdb si lib use to delete a task.
__u8  isdb_delete_task(__u8 task_id)
{
    if (task_id != 0xff)
    {
        esKRNL_TimeDlyResume(task_id);
        while(esKRNL_TDelReq(task_id) != OS_TASK_NOT_EXIST)
        {
            esKRNL_TimeDlyResume(task_id);
            esKRNL_TimeDly(1);
        }
    }
    else
    {
        esKRNL_TDel(task_id);
    }

    return 0;
}


//* function isdb si lib use to check task quit request.
__s32 isdb_check_task_delete_request(__u8 taskId)
{
    return esKRNL_TDelReq(taskId);
}


//* isdb si lib need this function to open demux section filter
/*******************************************************************************
Function name: isdb_open_section_filter
Description: 
    1.一般用于搜台的汇总函数
    2.也用于EIT和TDT/TOT的监控.
Parameters: 
    
Return: 
    
Time: 2011/2/29
*******************************************************************************/
__s32 isdb_open_section_filter(__u32 pid,__u32 servId, __s32 *pnVirtualChanId)
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
                requestDataCB = esKRNL_GetCallBack(isdb_pat_request_buffer);
                updateDataCB  = esKRNL_GetCallBack(isdb_pat_update_data);
                break;
             }
       case  CAT_PID:     
             {  
     //           requestDataCB = esKRNL_GetCallBack(isdb_cat_request_buffer);         
     //           updateDataCB  = esKRNL_GetCallBack(isdb_cat_update_data);
                break;
             }
       case  TSDT_PID:
             {
       //         requestDataCB = esKRNL_GetCallBack(isdb_tsdt_request_buffer);         
       //         updateDataCB  = esKRNL_GetCallBack(isdb_tsdt_update_data);
                break;
             }
       case  NIT_PID:
             {
                requestDataCB = esKRNL_GetCallBack(isdb_nit_request_buffer);         
                updateDataCB  = esKRNL_GetCallBack(isdb_nit_update_data);
                break;
             }
       case  SDT_BAT_PID:
             {
                requestDataCB = esKRNL_GetCallBack(isdb_sdt_request_buffer);         
                updateDataCB  = esKRNL_GetCallBack(isdb_sdt_update_data);
				
                break;
             }
       case  HEIT_PID:
             {
                requestDataCB = esKRNL_GetCallBack(isdb_heit_request_buffer);         
                updateDataCB  = esKRNL_GetCallBack(isdb_heit_update_data);
                break;
             }
       case  MEIT_PID:
             {
                requestDataCB = esKRNL_GetCallBack(isdb_meit_request_buffer);         
                updateDataCB  = esKRNL_GetCallBack(isdb_meit_update_data);
                break;
             }
       case  LEIT_PID:
             {
                requestDataCB = esKRNL_GetCallBack(isdb_leit_request_buffer);         
                updateDataCB  = esKRNL_GetCallBack(isdb_leit_update_data);
                break;
             }
       case  RST_PID:
             {
      //          requestDataCB = esKRNL_GetCallBack(isdb_rst_request_buffer);         
      //          updateDataCB  = esKRNL_GetCallBack(isdb_rst_update_data);
                break;
             }
       case  TDT_TOT_PID:
             {
                requestDataCB = esKRNL_GetCallBack(isdb_tdt_request_buffer);         
                updateDataCB  = esKRNL_GetCallBack(isdb_tdt_update_data);
                break;
             }
       case  DIT_PID:
             {
      //          requestDataCB = esKRNL_GetCallBack(isdb_dit_request_buffer);         
      //          updateDataCB  = esKRNL_GetCallBack(isdb_dit_update_data);
                break;
             }
       case  SIT_PID:
             {
      //          requestDataCB = esKRNL_GetCallBack(isdb_sit_request_buffer);         
     //           updateDataCB  = esKRNL_GetCallBack(isdb_sit_update_data);
                break;
             }
       case  PMT_PID_FOR_ONESEG:
             {
                requestDataCB = esKRNL_GetCallBack(isdb_pmt_request_buffer);         
                updateDataCB  = esKRNL_GetCallBack(isdb_pmt_update_data);
                break;
             }
       default:
           {
				eDbug("search pmt servId is 0x%x \n",servId);
               requestDataCB = esKRNL_GetCallBack(isdb_pmt_request_buffer);         
               updateDataCB  = esKRNL_GetCallBack(isdb_pmt_update_data);
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


//* isdb si lib need this function to close demux section filter  
__s32 isdb_close_section_filter(__u32 pid, __s32 nVirtualChanId)
{
    __s32 result;

    result = esMODS_MIoctrl(g_maple->demux, DVB_DMX_CMD_CLOSE_FILTER, pid, (void*)nVirtualChanId);
    
    if(result < 0)
        return -1;
    else
        return 0;
}


//* isdb si lib need this function to read section data
#if 0
__s32 isdb_read_section_data(__u32 pid, __u8* buf, __u8 checkCrcFlag)
{
    __u32 rdSize;

    // rdSize = esMODS_MRead(buf, 0, pid, g_maple->demux);
    if(g_isdbCtx->bufFullFlag == 0)
        return -1;
    else
        rdSize = g_isdbCtx->readDataLen;
    
	if((rdSize > 0) && (checkCrcFlag == 1))
	{
		if (Crc32Compute(0xffffffff, buf, rdSize))
		{
		    WARNING("crc check error.\n");
		    return -1;
		}
	}
    return rdSize;
}
#endif

//* event callback functions isdb si lib will call when event happen.
//* callback function to notify one service searched.
void  isdb_search_one_service_cb(maple_serv_item_t* newItem)
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
void  isdb_search_service_finish_cb(void)
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
void  isdb_search_sepg_finish_cb(maple_sepg_t* sepg)
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
void  isdb_search_one_schedule_cb(maple_schdl_event_t* newEvent)
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
void  isdb_search_schedule_finish_cb(void)
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
__s32 isdb_tune(maple_demod_info_t* demod_pm)
{
    __s32 result;
    tune_param_t tp;
    tp.frequency = demod_pm->frequency;
    tp.bandwidth = demod_pm->bandwidth;
    result = esMODS_MIoctrl(g_maple->demod, DEMOD_TUNE, 0, &tp);

	eDbug("isdb tune result is %d \n",result);
    if(result != 0)
        return -1;
    else
        return 0;
}

//* function use to get current tuner parameter.
__s32 isdb_get_tuner_param(maple_demod_info_t* demod_pm)
{
    tune_param_t tp;
    esMODS_MIoctrl(g_maple->demod, DEMOD_GET_CUR_TUNE_PARAM, 0, (void*)&tp);
    demod_pm->frequency = tp.frequency;
    demod_pm->bandwidth = tp.bandwidth;
    return 0;
}

