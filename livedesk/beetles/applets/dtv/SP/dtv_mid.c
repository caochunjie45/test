/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           calendar app sample
*
*						        	(c) Copyright 2006-2009, ANDY, China
*											 All Rights Reserved
*
* File    : power.c
* By      :
* Version : V1.00
*********************************************************************************************************
*/

#include     "dtv_mid.h"



__u32                                maple_mid = 0;

__mp                                 *maple = NULL;

static ES_FILE                       *dis_file = NULL;

static __u32                         video_layer_hdl = NULL;

maple_sl_cb_param_t                  *sl_event_cbp = NULL;

maple_serv_list_t                    *service_list = NULL;

maple_serv_list_t                    *search_service_list = NULL;//仅用于搜索时用

//当前频道的节目预告
maple_schdl_cb_param_t               *schdl_event_cbp = NULL;

maple_schdl_list_t                   *schdl_list = NULL;

maple_schdl_list_day                 *schdl_list_day = NULL;

dtvmid_time_t                        *tv_time = NULL;



//当前频道的当前正在播放和下一个将要播放的节目预告
maple_sepg_t                         *sepg = NULL;

maple_sepg_cb_param_t                *sepg_finish_cbp = NULL;

char                                 *sepg_current = NULL;

char                                 *sepg_next = NULL;


maple_cas_param_t                    *ca_sn = NULL;


char			                     curSearchDisk[16];

static __u8                          save_root_type = 0;



__krnl_event_t                       *maple_CmdQ = NULL;

__krnl_event_t                       *maple_pFedBakQ = NULL; 

__bool                               b_full_screen = EPDK_TRUE;  //非缩略图播放

//服务列表(频道列表)
maple_demod_info_t	                 tps;


maple_save_para_t                    *save_para;

static __bool                        search_task_break = EPDK_FALSE; //人为中断搜台线程,add by hot.lee

H_WIN                                M_Logic_Win = NULL;




static __s32 Maple_App_CallBack(__s32 id,__s32 data1,__s32 data2)
{
	__gui_msg_t msg;
	
	msg.id 		    = GUI_MSG_COMMAND;
	msg.h_deswin	= M_Logic_Win;
	msg.dwAddData1  = MAKELONG(id, data1);
	msg.dwAddData2  = data2;
	
	GUI_SendNotifyMessage(&msg);

}


__s32 Maple_Set_Logic_Win(H_WIN win)
{
	M_Logic_Win = win;

	return EPDK_OK;

}


//当前需要搜索的总频点数
__s32 Maple_Get_Total_Frequency(void)
{
	area_frequencys_t * area_frequencys;

	area_frequencys = &area_frequencys_table[G_DTV_current_area];

	return (area_frequencys->frequency_total);
}


//根据当前搜索第几个频点，获取该频点
__s32 Maple_Get_Current_Search_Frequency(__s32 index)
{
	area_frequencys_t * area_frequencys;

	area_frequencys = &area_frequencys_table[G_DTV_current_area];

	return area_frequencys->frequencys[index].StartFreq;
}



//设置当前正在搜索的频点，用于更新搜索界面的频率显示
static __s32 Maple_Set_Current_Search_Frequency(__s32 freq,__u16 index)
{
	Maple_App_CallBack(UPDATE_FREQ,freq,index);
}





void Maple_Check_Disk_Format(void)
{
	ES_FILE *fp = NULL;
	
	//存贮盘符是否存在，不存在则需要格式盘符
	fp = eLIBs_fopen(DTV_BASEINFO_NAME, "rb");
	if(NULL == fp)
	{
		fp = eLIBs_fopen(DTV_BASEINFO_NAME, "wb");
		if(NULL == fp)//写不成功时才格式化Z盘
		{
			eLIBs_format("z:\\","fat",0);
			esKRNL_TimeDly(10);
		}
		else
		{
			//创建文件只为测试盘符是否已经格式化
			eLIBs_fclose(fp);
			fp = NULL;
			eLIBs_remove(DTV_BASEINFO_NAME);
		}
	}
	else
	{
		//文件存在，证明盘符已经格式化
		eLIBs_fclose(fp);
		fp = NULL;
	}	

}


maple_save_para_t *Maple_Get_Save_Data(void)
{
	return save_para;
}


__s32 Maple_Program_Load(char *database_file_name)
{
	int                        i,j = 0;
	
	__s32                      ret;
	
	ES_FILE                    *pf = NULL;
	
    char                       str_filename[32] = {0};
	
	char                       baseinf_filename[32] = {0};
	

    eLIBs_strcpy(str_filename, database_file_name);

	eLIBs_memset(save_para,0,sizeof(maple_save_para_t));

	pf = eLIBs_fopen(str_filename,"rb");
	if(NULL == pf)
	{	
		//存台文件不存在，默认搜台
		eLIBs_printf("file [%s] not exist , search\n",str_filename);
		
        save_para->LCN_status = 0;
        save_para->index      = 0;
        save_para->mode       = MAPLE_VID_WINDOW_FULLSCN;
		
        Maple_Reset_Audio_Num();
        
		//加载失败1
		return EPDK_FAIL;
	}
	else
	{
	
	}
	
	eLIBs_fclose(pf);
	pf = NULL;

	
	//文件存在时才提取基本信息，不存在时默认为0
	
	eLIBs_strcpy(baseinf_filename,DTV_BASEINFO_NAME);
	pf = eLIBs_fopen(baseinf_filename,"rb");
	if(pf)
	{
		eLIBs_fread(save_para,1,sizeof(maple_save_para_t),pf);
		eLIBs_fclose(pf);
		pf = NULL;

	}
	
	ret = esMODS_MIoctrl(maple, MAPLE_CMD_SL_OPEN, 0, (void*)str_filename);
	if(EPDK_FAIL == ret)
	{
		eLIBs_printf("reg ok , why open file [%s] fail?\n",str_filename);
		return EPDK_FAIL;
	}
    

    service_list->servNum    = 0;
	service_list->curServIdx = 0;
	eLIBs_memset((void*)service_list->pServItems,0,sizeof(maple_serv_item_t)*MAPLE_MAX_SERV_NUM);		

    if(SSET_NC_PROGRAMTYPE_TV == PROGRAMTYPE)
    {
        esMODS_MIoctrl(maple, MAPLE_CMD_SL_GET, MAPLE_SERV_GROUP_TYPE_TV, (void*)service_list);
    }
    else if(SSET_NC_PROGRAMTYPE_RADIO == PROGRAMTYPE)
    {
        esMODS_MIoctrl(maple, MAPLE_CMD_SL_GET, MAPLE_SERV_GROUP_TYPE_RADIO, (void*)service_list);
    }   
    else if(SSET_NC_PROGRAMTYPE_ALL == PROGRAMTYPE)
    {
        esMODS_MIoctrl(maple, MAPLE_CMD_SL_GET, MAPLE_SERV_GROUP_TYPE_ALL, (void*)service_list);
    }		
    if(save_para->index >= service_list->servNum)
    {
        save_para->index = service_list->servNum-1;
    }
    
    if(save_para->index < 0)
    {
        save_para->index = 0;
    }
    
	service_list->curServIdx = service_list->pServItems[save_para->index].channelId;


	return EPDK_OK;
}



__s32 Maple_Reg_Flush(void)
{
	
	ES_FILE                                  *fp = NULL;
		
	__u32                                    len;
	
	char                                     *buf = NULL;
	
	char                                     baseinf_filename[64]={0};

	
    spinor_wrsr(0<<2,0<<6);
    
     {
		eLIBs_strcpy(baseinf_filename,DTV_BASEINFO_NAME);
		
		fp = eLIBs_fopen(baseinf_filename, "wb");
		if(NULL == fp)//写不成功时才格式化Z盘
		{
			eLIBs_format("z:\\","fat",0);
			esKRNL_TimeDly(10);
			fp = eLIBs_fopen(baseinf_filename, "wb");
			if(NULL == fp)
			{
    			eDbug("2open file fail:%s\n", DTV_BASEINFO_NAME);
				if(fp)
				{
					eLIBs_fclose(fp);
				}				
    			return EPDK_FAIL;
			}				
		}			

		eLIBs_fwrite(save_para,1,sizeof(maple_save_para_t),fp);
		
        
		eLIBs_fclose(fp);

		//其实可以不放在这里，我们可以在需要的时候才保存
		//这样我们只需要保存当前播放的信息到独立分区
		//而不需要和台的信息放在同一盘符
		Maple_Program_Save(DTV_DATABASE_NAME);

        {
            ES_FILE *hFile = NULL;
            
            //here  actury  flush user disk data to NOR flash
            hFile = eLIBs_fopen("b:\\STORAGE\\SPINOR", "rb");
            if (NULL == hFile)
            {
                eDbug("open NOR file failed\n");
                return EPDK_FAIL;
            }
			
            eDbug("flush dtv file ....\n");
            eLIBs_fioctrl(hFile, DEV_IOC_USR_FLUSH_CACHE, 0, 0);          
            eDbug("flush dtv end ....\n");

			
            eLIBs_fclose(hFile);
        }
    } 
	spinor_wrsr(0<<2,1<<6);

	return EPDK_OK;
}


//文件名自定义，中间件会去创建
__s32 Maple_Program_Save(char *database_file_name)
{
	if(service_list->servNum > 0)
	{
		__s32 ret;

        char str[128];

        eLIBs_strcpy(str, DTV_DATABASE_NAME);
		ret = esMODS_MIoctrl(maple, MAPLE_CMD_SL_SAVE, 0, (void*)str);

		__msg("ret = %d\n",ret);
		if(ret != EPDK_OK)
		{
			__wrn("save program fail\n");
		}
		else
		{
			ES_FILE *pf = NULL;

			pf = eLIBs_fopen(database_file_name,"rb");
			if(pf)
			{
				eLIBs_fclose(pf);
				pf = NULL;
			}
			else
			{
				__wrn("why open file [%s] fail\n",database_file_name);
			}
		}
	}
	
	return EPDK_OK;

}



/***************************************************************************************************
*Name        : Maple_Request_Video_Layer
*Prototype   : __s32  Maple_Request_Video_Layer( const RECT *rect_p, __s32 pipe, __s32 prio )
*Arguments   : rect_p    input. the coordinate and size of the video layer.
*              pipe      input. layer pipe.
*              prio      input. layer priority.
*Return      : the video layer handle. if return NULL, means failing.
*Description : request the video layer.
*Other       :
***************************************************************************************************/
static __hdle  Maple_Request_Video_Layer( const RECT *rect_p, __s32 pipe, __s32 prio )
{
	__disp_layer_para_t     image_layer_para;
	FB                      image_fb_para;
	RECT                    image_win;
    __u32 arg[3];
	__s32 ret;
	
	if(NULL == rect_p)
	{
		return NULL;
	}
	
    if(NULL == video_layer_hdl)
    {
    	//* request a layer for playing video,
        arg[0] = DISP_LAYER_WORK_MODE_SCALER;
        arg[1] = 0;
        arg[2] = 0;
    	__msg("DISP_CMD_LAYER_REQUEST.............\n");
    	video_layer_hdl = eLIBs_fioctrl( dis_file, DISP_CMD_LAYER_REQUEST, 0, (void*)arg );
    	if( video_layer_hdl == NULL )
    	{
    		__wrn("Error in applying for the video layer.\n");
    		goto error;
    	}
    }

	
	arg[0] = video_layer_hdl;
	arg[1] = 0;
	arg[2] = 0;
    eLIBs_fioctrl(dis_file, DISP_CMD_LAYER_BOTTOM, 0, (void*)arg);

	image_win.x      = rect_p->x;
    image_win.y      = rect_p->y;
    image_win.width  = rect_p->width ;
    image_win.height = rect_p->height;	

    esMODS_MIoctrl(maple, MAPLE_CMD_SET_VID_LAYER, 0, (void*)video_layer_hdl);
    esMODS_MIoctrl(maple, MAPLE_CMD_SET_VID_WINDOW, 0, (void*)&image_win);

	return video_layer_hdl;

error:

	return NULL;
}




__s32 Maple_Register_Schedule_Callback(__pCBK_t cb_search_schedule_event_callback,__pCBK_t cb_search_schedule_finish_callback)
{
	__pCBK_t			callback;

    callback = esKRNL_GetCallBack(cb_search_schedule_event_callback);
    esMODS_MIoctrl(maple, MAPLE_CMD_CB_SET_PARAM, MAPLE_CB_SRCH_SCHDL_EVENT, (void*)NULL);//schdl_event_cbp
    esMODS_MIoctrl(maple, MAPLE_CMD_CB_SET, MAPLE_CB_SRCH_SCHDL_EVENT, (void*)callback);
    
    callback = esKRNL_GetCallBack(cb_search_schedule_finish_callback);
    esMODS_MIoctrl(maple, MAPLE_CMD_CB_SET_PARAM, MAPLE_CB_SRCH_SCHDL_FINISH, (void*)NULL);//schdl_event_cbp
    esMODS_MIoctrl(maple, MAPLE_CMD_CB_SET, MAPLE_CB_SRCH_SCHDL_FINISH, (void*)callback);
    
	return EPDK_OK;

}


__s32  Maple_Reset_Schedule_Event(__u32 serviceid)
{
    int            i;

	
    eLIBs_memset(schdl_list_day,0,MAX_EPG_DAY * sizeof(maple_schdl_list_day));

	for(i = 0;i < 1;i++)//只猎取当天的节目列表
    {
        schdl_list_day[i].eventNum = 0;
        //schdl_list_day[i].MJD=tv_time->MJD+i;//有问题
        schdl_list_day[i].MJD = i;
        schdl_list_day[i].serviceid = service_list->curServIdx;
		__msg("schdl_list_day[%d].MJD = %d \n",i,schdl_list_day[i].MJD);
    }

	return EPDK_OK;
}


__s32 Maple_Add_Schedule_Event(maple_schdl_event_t * event)
{
    int i;
    
	for(i = 0;i < 1;i++)//只获取当天的就OK		
	{
		eLIBs_memcpy(schdl_list_day[i].pSchdlEvents+schdl_list_day[i].eventNum,event,sizeof(maple_schdl_event_t));
		schdl_list_day[i].eventNum += 1;

		schdl_list_day[i].MJD = event->startTime.MJD;
	}; 

    return EPDK_OK;
}



//Maple_Epg_Search()之后并不会马上得到epg数据
//所以马上进入schedule界面有可能数据还是空的
__s32 Maple_Get_Schedule(void)
{
    int i = 0;
	
    Maple_Reset_Schedule_Event(0);
	
    esMODS_MIoctrl(maple, MAPLE_CMD_GET_SCHEDULE, service_list->curServIdx, schdl_list);    

	for(i = 0;i < schdl_list->eventNum;i++)
    {
        Maple_Time_Deal(&(schdl_list->pSchdlEvents[i].startTime));
        Maple_Time_Deal(&(schdl_list->pSchdlEvents[i].endTime));
        
        Maple_Add_Schedule_Event(&(schdl_list->pSchdlEvents[i]));
    }

    return EPDK_OK;
}


//获取当前正在播放的节目是一天中的第几个节目
__s32 Maple_Get_Current_ScheduleEvent_Index(__s32 day_index)
{
    __s32                 i;
	
	char                  tmp[128];

    for(i=0;i < schdl_list_day[day_index].eventNum; i++)
    {
        //endTime 都 是0，所以  //取起始时间相等就OK
        eLIBs_sprintf(tmp,"  %d:%d:%d - %d:%d:%d : ",
		schdl_list_day[day_index].pSchdlEvents[i].startTime.hour,
		schdl_list_day[day_index].pSchdlEvents[i].startTime.min,
		schdl_list_day[day_index].pSchdlEvents[i].startTime.sec,
		schdl_list_day[day_index].pSchdlEvents[i].endTime.hour,
		schdl_list_day[day_index].pSchdlEvents[i].endTime.min,
		schdl_list_day[day_index].pSchdlEvents[i].endTime.sec);
		
		if(eLIBs_strncmp(tmp,sepg_current,10) == 0)//取起始时间相等就OK
		{
			__msg("i = %d\n",i);
			return i;
		}
			
	}

	return EPDK_OK;
}


//获取某一天的event num
__s32 Maple_Get_ScheduleEvent_Num(__s32 day_index)
{
	return schdl_list_day[day_index].eventNum;
}


//获取某一天指定event index的schdl events
maple_schdl_event_t *Maple_Get_Schedule_Event(__s32 day_index,__s32 event_index)
{
    if(Maple_Get_ScheduleEvent_Num(day_index) <= 0)
    {
        return NULL;
    }

	
    return &schdl_list_day[day_index].pSchdlEvents[event_index];
}


__s32 Maple_Epg_Search(void)
{
    __s32 ret;
    
    Maple_Reset_Schedule_Event(0);
	
    ret = esMODS_MIoctrl(maple, MAPLE_CMD_EPG_MONITOR_START, 0, NULL);
	
	__msg("ret = %d \n",ret);
	
    return ret;
}

__s32 Maple_Epg_Search_Stop(void)
{
    esMODS_MIoctrl(maple, MAPLE_CMD_EPG_MONITOR_STOP, 0, NULL);
	
    return EPDK_OK;
}






static int Judge_Month(int year,int month)
{
	if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
	{
		return 31;
	}
	
	else if(month == 2)
	{
		if(year %4 !=0 || year%100 == 0 && year %400 != 0)
		{
			return 28;
		}
		else
		{
			return 31;
		}
	}
	
	else
	{
		return 30;
	}
	
}

static __bool Is_Leap_Year(__u16 Year) 
{ 
    if((Year & 3) == 0) 
    { 
        return ((Year %400 == 0) || (Year %100 != 0)); 
    } 
    return 0; 
} 

static __s32 Maple_Time_Deal(maple_time_t *tv_time)
{
    __u8 hour_offset,min_offset;
    
    hour_offset = 7;
    min_offset  = 0;

    if(tv_time->hour + hour_offset < 24)
    {
        tv_time->hour = tv_time->hour + hour_offset;
    }
    else
    {
        tv_time->hour = tv_time->hour + hour_offset - 24;
        if(tv_time->month == 1 || tv_time->month == 3|| tv_time->month == 5
        || tv_time->month == 7|| tv_time->month == 8|| tv_time->month == 10)
        {
            if(tv_time->day < 31) tv_time->day++;
            else
            {
               tv_time->day = 1;
               tv_time->month++;
            }
        }
        else if(tv_time->month == 4 || tv_time->month == 6
            || tv_time->month == 9|| tv_time->month == 11)
        {
            if(tv_time->day < 30) tv_time->day++;
            else
            {
               tv_time->day = 1;
               tv_time->month++;
            }

        }
        else if(tv_time->month == 2)
        {
            if(tv_time->day == 29 || (tv_time->day == 28 && Is_Leap_Year(tv_time->year)==0))
            {
                tv_time->day = 1;
                tv_time->month++;      
            }
            else
                tv_time->day ++;
                
        }

        else if(tv_time->month == 12)
        {
            if(tv_time->day < 31) tv_time->day ++;
            else
            {
                tv_time->day = 1;
                tv_time->month = 1;
                tv_time->year ++;
            }
        }
    }

}



__s32 Maple_Time_Monitor_Start(void) 
{
    esMODS_MIoctrl(maple, MAPLE_CMD_TIME_MONITOR_START, 0, NULL);

    return EPDK_OK;

}

__s32 Maple_Time_Monitor_Stop(void)
{
    esMODS_MIoctrl(maple, MAPLE_CMD_TIME_MONITOR_STOP, 0, NULL);

	return EPDK_OK;

}

__s32 Maple_Time_Monitor_Get(maple_time_t *tv_time)
{
    __time_t time;
    __date_t date;
    
    esMODS_MIoctrl(maple, MAPLE_CMD_TIME_GET, 0, tv_time);
    Maple_Time_Deal(tv_time);
    
    date.year = tv_time->year;
    date.month = tv_time->month;
    date.day = tv_time->day;

    time.hour = tv_time->hour;
    time.minute = tv_time->min;
    time.second = tv_time->sec;

	
	return EPDK_OK;
}





__s32 Maple_Set_Antenna_State(__bool state)
{
    if(state)//state = 1:外部天线
    {
        pull_down_gpio(ANTENN_POWER_PORT ,ANTENN_POWER_PORTNUM);
        pull_down_gpio(RF_SW_PORT,RF_SW_PORTNUM);
	}
	
    else     //state = 0:拉杆天线
    {
        pull_up_gpio(ANTENN_POWER_PORT,ANTENN_POWER_PORTNUM);
        pull_up_gpio(RF_SW_PORT,RF_SW_PORTNUM);
    }

	
	return EPDK_OK;
}



__s32 Maple_Reset_Audio_Num(void)
{
    __u8     i;

	
    for(i = 0; i < MAX_CHANNEL_NUM; i++)
    {
        save_para->audio_num_select[i] = 0;
    }

	return EPDK_OK;
	
}



__u8 Maple_Get_Audio_Num(__s32 index)
{
	if(index > service_list->servNum-1)
	{
		return EPDK_FAIL;
	}

	
    return save_para->audio_num_select[index];
}



__u8 Maple_Set_Audio_Num(__u8 num)
{
    __s32 index = 0;
	
    index = service_list->curServIdx;
	
	if(index > service_list->servNum-1)
	{
		return EPDK_FAIL;
	}
	
    if(save_para->audio_num_select[index] != num)
    {
        save_para->audio_num_select[index] = num;
    }
}




__s32 Maple_Get_Current_Channel_Austream_Cnt(void)
{
    __s32 index = 0;
	
    index = service_list->curServIdx;
	
    return service_list->pServItems[index].adStreamNum;
}


__s32 Maple_Get_Current_Austream(void)
{
    int     ret;
	
    ret = esMODS_MIoctrl(maple, MAPLE_CMD_GET_AUDSTREAM,0,NULL);
	
    return ret;
}

maple_ad_info_t *Maple_Get_Current_Channel_Austream_Info(void)
{
    __s32 index = 0;
	
    index = service_list->curServIdx;
	
    if(service_list->pServItems[index].adStreamNum > 0)
    {
        return service_list->pServItems[index].adInfo;
    }
}



char *Maple_Get_Current_Sepg(void)
{
	return sepg_current;
}

char *Maple_Get_Next_Sepg(void)
{
	return sepg_next;
}


__s32 Maple_Clear_Sepg(void)
{
	eLIBs_memset(sepg_current,0,MAX_SEPG_STRING);
	
	eLIBs_memset(sepg_next,0,MAX_SEPG_STRING);

	return EPDK_OK;
	
}




__s32 Maple_Sepg_Search(void)
{
    int hassepg   = 0;
	
    int i         = 0;

    if(sepg->hasCurEventInfo)
    {
         sepg->curEventstartTime.hour  = 0; 
         sepg->curEventstartTime.min   = 0; 
         sepg->curEventstartTime.sec   = 0;
         sepg->curEventEndTime.hour    = 0; 
         sepg->curEventEndTime.min     = 0; 
         sepg->curEventEndTime.sec     = 0;

        eLIBs_memset(sepg->curEventName,0,64);
    
    }   
       
    if(sepg->hasNextEventInfo)
    {          
         sepg->nextEventstartTime.hour  = 0; 
         sepg->nextEventstartTime.min   = 0; 
         sepg->nextEventstartTime.sec   = 0; 
         sepg->nextEventEndTime.hour    = 0; 
         sepg->nextEventEndTime.min     = 0; 
         sepg->nextEventEndTime.sec     = 0;
		 
        eLIBs_memset(sepg->nextEventName,0,64);
        
    }

    
     eLIBs_memset(sepg_current,0,MAX_SEPG_STRING);
     eLIBs_memset(sepg_next,0,MAX_SEPG_STRING);


    for(i = 0;i < 3;i++)
    {
        esKRNL_TimeDly(10);
		
        esMODS_MIoctrl(maple, MAPLE_CMD_GET_SEPG, service_list->curServIdx, (void*)sepg);
		
        if(sepg->hasCurEventInfo || sepg->hasNextEventInfo)
        {
			break;
        }
	}
    
    if(sepg->hasCurEventInfo)
    {
        Maple_Time_Deal(&sepg->curEventstartTime);
        Maple_Time_Deal(&sepg->curEventEndTime);
        
        eLIBs_sprintf(sepg_current,"  %d:%d:%d - %d:%d:%d : ",
                     sepg->curEventstartTime.hour, 
                     sepg->curEventstartTime.min, 
                     sepg->curEventstartTime.sec,
                     sepg->curEventEndTime.hour, 
                     sepg->curEventEndTime.min, 
                     sepg->curEventEndTime.sec);
    
        strcat(sepg_current,(char *)sepg->curEventName);  
		
        hassepg = 1;
    
    }   

	__msg("sepg_current = %s\n",sepg_current);
       
    if(sepg->hasNextEventInfo)
    {          
        Maple_Time_Deal(&sepg->nextEventstartTime);
        Maple_Time_Deal(&sepg->nextEventEndTime);
        
        eLIBs_sprintf(sepg_next,"  %d:%d:%d - %d:%d:%d :",
                     sepg->nextEventstartTime.hour, 
                     sepg->nextEventstartTime.min, 
                     sepg->nextEventstartTime.sec, 
                     sepg->nextEventEndTime.hour, 
                     sepg->nextEventEndTime.min, 
                     sepg->nextEventEndTime.sec);
		
        strcat(sepg_next,sepg->nextEventName);
		
        hassepg = 1;
    }

	__msg("sepg_current = %s\n",sepg_next);


    return hassepg;
}





static __u32 Maple_Check_Disk(void)
{
	__s32                  ret;
	
	__u32                  root_type = RAT_UNKNOWN;
	
	char                   diskname[RAT_MAX_PARTITION][4] = {0x0};


	ret = rat_get_partition_name(RAT_USB_DISK, diskname);
	if(EPDK_OK == ret)
	{
		__msg("get USB fs part\n");	
		
		if (diskname[0][0])
		{
			__msg("USB : diskname=%s\n", diskname[0]);
			root_type += RAT_USB;
		}
	}
	
	ret = rat_get_partition_name(RAT_SD_CARD, diskname);
	if(EPDK_OK == ret)
	{    
		__msg("get SD fs part\n");		

		if (diskname[0][0])
		{
			__msg("SD : diskname=%s\n", diskname[0]);
			root_type += RAT_TF<<8;
		}
	}


	return root_type;   
}


__s32 Maple_Check_Device(void)
{
	__u32                 root_type = 0;
	
	__u32	              temp_root = 0;
	
	__u8                  sd_root = 0;
	
	__u8	              usb_root = 0;
	
	__u8	              device_cnt = 0;


	root_type = Maple_Check_Disk();	
	
	sd_root = (root_type >> 8) & 0x00ff;
	usb_root = (root_type) & 0x00ff; 

	if(sd_root)
	{
		if(usb_root)	                    
		{
			if(save_root_type == sd_root)
			{			
				temp_root = RAT_USB;
			}
			else
			{
				temp_root = RAT_TF;
			}
			device_cnt = 2;
		}	
		else
		{
			temp_root = RAT_TF;
			device_cnt = 1;
		}				
	}			
	else if(usb_root)		
	{
		temp_root = RAT_USB;
		device_cnt = 1;
	}		
	else
	{
		return 0;
	}

    save_root_type = temp_root;
    
	return temp_root;
}                

static __s32 Maple_Get_RootPath(__u32 curRootType, char *curSearchPath)
{
	__s32              ret = 0;
	
	char               root_type[50];
	
	char               disk_name[RAT_MAX_PARTITION][4];
	
	ES_DIR		       *tmp_dir;
    
    
	eLIBs_memset(root_type, 0, sizeof(root_type));
	eLIBs_memset(disk_name, 0, sizeof(disk_name));
	eLIBs_memset(curSearchPath, 0, 50);
	switch(curRootType)
	{
		case RAT_LOCAL:
			eLIBs_strcpy(root_type,RAT_LOCAL_DISK);
			break;
		case RAT_TF:
			eLIBs_strcpy(root_type,RAT_SD_CARD);
			break;
		case RAT_USB:
			eLIBs_strcpy(root_type,RAT_USB_DISK);
			break;
		default:
			break;
	}
	ret = rat_get_partition_name(root_type, disk_name);
	eLIBs_strcpy(curSearchPath,disk_name[0]);
	eLIBs_strcpy(curSearchDisk,disk_name[0]);
    
	eDbug("curSearchPath = %s\n", curSearchPath);

	return EPDK_OK;
	
}


static __s32 Maple_Set_Record_Path(char  *recordFilePath)
{
	__date_t	      date;
	
	char 		      dataStr[16]={0};
	
	char              cntStr[10]={0};
	
	__u32		      fileNum = 0;
	
    __s32             root_type = 0;
	
    char              channel_name[256]={0}; 
	
	char              *tmp = channel_name;
	
    maple_time_t      *tv_time;

	
    root_type = Maple_Check_Device();
    if(root_type == 0)
    {
        return EPDK_FAIL;
    }

	
    Maple_Get_RootPath(root_type,recordFilePath);
	
    eDbug("recordFilePath is %s \n",recordFilePath);

	fileNum = record_file_num(recordFilePath, DTVRECORD_FILTER);	

    eDbug("fileNum is %d \n",fileNum);
	
	cntStr[0] = ((fileNum/1000%10)+'0');
	cntStr[1] = ((fileNum/100%10)+'0');
	cntStr[2] = ((fileNum/10%10)+'0');
	cntStr[3] = ((fileNum/1%10)+'0');
	cntStr[4] = '\0';
    
    tmp = Maple_Get_Name_Of_Channel(Maple_Get_Current_Channel_Index());
    eLIBs_sprintf(channel_name,"%s",tmp);
    eDbug("channel_name is %s \n",channel_name);

	SLIB_strcat(recordFilePath, channel_name);
	SLIB_strcat(recordFilePath, "_");


    Maple_Time_Monitor_Get(tv_time);
	
    if(tv_time)
    {
        char time[128];
        char tmp[128];
        eLIBs_memset(time, 0, sizeof(time));
        eLIBs_memset(tmp, 0, sizeof(tmp));

        eLIBs_sprintf(time,"%04d",tv_time->year);
        
        eLIBs_sprintf(tmp,"%02d",tv_time->month);
		SLIB_strcat(time, tmp);

        eLIBs_sprintf(tmp,"%02d",tv_time->day);
		SLIB_strcat(time, tmp);

        eLIBs_sprintf(tmp,"%02d",tv_time->hour);
		SLIB_strcat(time, tmp);

        eLIBs_sprintf(tmp,"%02d",tv_time->min);
		SLIB_strcat(time, tmp);

        eDbug("time is %s \n",time);
		SLIB_strcat(recordFilePath, time);

    }

    else
    {
		SLIB_strcat(recordFilePath,"20151106120101");
    }
    
    SLIB_strcat(recordFilePath, "_");
	SLIB_strcat(recordFilePath, cntStr);

	SLIB_strcat(recordFilePath, ".ts");

	eDbug("record file: %s \n", recordFilePath);


	return EPDK_OK;

}



__s32 Maple_Set_Record_Disk_Full_CB(__pCBK_t full_cb)
{
	__pCBK_t			callback;
	
    callback = esKRNL_GetCallBack(full_cb);

	return esMODS_MIoctrl( maple, MAPLE_CMD_SET_RECORD_CB, 0, (void*)callback );

}



__s32 Maple_Get_Record_Staue(void)
{
   return esMODS_MIoctrl(maple, MAPLE_CMD_GET_RECORD_STATUS, 0, NULL);
}



__bool Maple_Control_TS_Record_Start(void)
{
    __s32                        nProgId;  //正在播放的service的progId号,和MAPLE_CMD_PLAY的channelid相同的意思

	__s32                        tmpret;

	CedarHerbMediaFileInf        RecordFileInfo;

	char                         record_name[128];

	__s32                        ret;
	
    if(NULL == maple)
    {
		return EPDK_FALSE;
    }
	
		
    nProgId = Maple_Get_Current_Channel_Index();

	
    eLIBs_memset(&RecordFileInfo, 0, sizeof(CedarHerbMediaFileInf));
    eLIBs_memset(record_name, 0, sizeof(record_name));
	
    ret = Maple_Set_Record_Path(record_name);
    if(EPDK_FAIL == ret)
    {
        return EPDK_FALSE;
    }
	
    RecordFileInfo.uFileFmt = CEDAR_MEDIA_FILE_FMT_TS;
    eLIBs_strcpy(RecordFileInfo.file_path, record_name);
    
    //启动录制,启动录制前必须先play.否则会出错.
    esMODS_MIoctrl(maple, MAPLE_CMD_SET_RECORD_MEDIAFILE, 0, &RecordFileInfo);
    tmpret = esMODS_MIoctrl(maple, MAPLE_CMD_RECORD, nProgId, NULL);
    if(tmpret != EPDK_OK)
    {
        __wrn("why record progid[%d] fail!\n", nProgId);
    }

    return EPDK_TRUE;
}

__bool Maple_Control_TS_Record_Stop(void)
{
    __s32   tmpret;

   __wrn("entry Maple_Control_TS_Record_Stop****\n");
   
    //停止录制
    tmpret = esMODS_MIoctrl(maple, MAPLE_CMD_STOP_RECORD, 0, NULL);
    if(tmpret != EPDK_OK)
    {
        __wrn("why stop record fail!\n");
		return EPDK_FALSE;		
    }
	
	return EPDK_TRUE;
}



//检测解码状态，add by hot.lee
//只用在定时器检测线程中
maple_ve_status_e *Maple_Get_Decode_Status(void)
{
	__s32 result;	
	maple_ve_status_e *maple_ve_status;
	
	result = esMODS_MIoctrl(maple,MAPLE_CMD_GET_VE_STATUS, 0, NULL);
	maple_ve_status = (maple_ve_status_e *)result;	 

	return maple_ve_status;
}



__s32 Maple_Set_TV_Mode(__u8 flag)
{
	return esMODS_MIoctrl(maple,MAPLE_CMD_SET_TV_MODE, flag, NULL);	
}



__s32 Maple_Set_ScreenSize(maple_vd_win_ratio_mode_e usr_mode)
{
    return esMODS_MIoctrl(maple, MAPLE_CMD_SET_VID_SHOW_MODE, usr_mode, NULL);
}

__s32 Maple_Get_ScreenSize(maple_vd_win_ratio_mode_e usr_mode)
{
    return esMODS_MIoctrl(maple, MAPLE_CMD_GET_VID_SHOW_MODE, 0, NULL);
}





maple_cas_param_t *Maple_Get_Ca_Sn(void)
{
    __s32 ret;
	
    ret = esMODS_MIoctrl(maple, MAPLE_CMD_CA_GET_SN, MAX_TMP_STRING, ca_sn);
	if(EPDK_OK == ret)
	{
	    __msg("ca_sn  = %x\n", ca_sn);
		return ca_sn;
	}

	__here__;
	return NULL;
}


__s32 Maple_Set_Subtitle_CB(__pCBK_t isdbt_cb)
{
	__pCBK_t			callback;
    
    callback = esKRNL_GetCallBack(isdbt_cb);

	return esMODS_MIoctrl( maple, MAPLE_CMD_SET_SPLY_DRAW_CB, NULL, (void*)callback );

}


//字幕解码类型
__s32 Maple_Set_Default_Char_Coding_Type(void)
{	
	return esMODS_MIoctrl( maple, MAPLE_CMD_SET_DEFAULT_CHAR_CODING_TYPE,G_TYPE,NULL);
}


//对于CMMB此函数只能在信号检测定时器里调用
//其它要获取信号状态的地方，请用全局变量获取
//因为MAPLE_CMD_GET_SS每次都会从通道里获取数据
//需要防止线程访问冲突造成的死锁
__s32 Maple_Get_SS(maple_demod_ss_t *singal)
{
	maple_demod_ss_t    ss;
	
	if(esMODS_MIoctrl(maple, MAPLE_CMD_GET_SS, 0, (void*)&ss)!= 0)
	{
		ss.strength = 0;
		ss.quality  = 0;
	}
	else
	{
		__wrn("GET:S: %d, Q: %d\n",ss.strength,ss.quality);
	}

	singal->strength = (__s8)ss.strength;
	singal->quality  = (__s8)ss.quality;
		
	return EPDK_OK;
	
}


__s32 Maple_Play_Audstream(__u32 audioIdx)
{
    __s32 ret = 0;
	
    ret = Maple_Get_Current_Channel_Austream_Cnt();
    if(ret <= 0)
    {
        return EPDK_FAIL;
    }
	
    if(esMODS_MIoctrl(maple, MAPLE_CMD_SEL_AUDSTREAM,audioIdx,NULL) != EPDK_OK)
    {
        return EPDK_FAIL;
    }
    else
    {
        return EPDK_OK;        
    }
	
}




static __s32 Maple_Get_Valid_Channel(void)
{
	__s32 i = 0,j = 0;

	for(i = 0; i < search_service_list->servNum; i++)
	{
		if(search_service_list->pServItems[i].pmtId <= 0x1fcf)
		{
			eLIBs_memcpy(&service_list->pServItems[j],&search_service_list->pServItems[i],sizeof(maple_serv_item_t));
			j++;
		}
	}

	service_list->servNum = j;

   //更新中间件,因为中间在退出时保存时包括收费频道
    //不然退出再进入时频道号是错误的
	{
		esMODS_MIoctrl(maple, MAPLE_CMD_SL_DELETE, 0, NULL);
		
		{
			//经过MAPLE_CMD_SL_ADD后，channelId会发生变化，所以需要重新调用MAPLE_CMD_SL_GET
			esMODS_MIoctrl(maple, MAPLE_CMD_SL_ADD, 0, (void*)service_list);
					
			service_list->servNum	 = 0;
			service_list->curServIdx = 0;
			eLIBs_memset((void*)service_list->pServItems,0,sizeof(maple_serv_item_t)*MAPLE_MAX_SERV_NUM); 	
			esMODS_MIoctrl(maple, MAPLE_CMD_SL_GET, 0, (void*)service_list);			
		}	

	}	

	return EPDK_OK; 	   
   
}






__s32 Maple_Get_Current_Av_Info(void)
{
    __s32 ret = 0;
	
	if(maple)
	{
    	ret = esMODS_MIoctrl(maple, MAPLE_CMD_GET_SERVICE_INFO, 0, NULL);
	}
	__msg("ret = %d \n",ret);

	
    return ret;
}


__s32 Maple_Service_List_Get(maple_serv_group_type_e type)
{

	int i;
	
		
	esMODS_MIoctrl(maple, MAPLE_CMD_SL_GET, type, (void*)search_service_list);//MAPLE_SERV_GROUP_TYPE_ALL
	
	Maple_Get_Valid_Channel();
	
	for(i=0; i<service_list->servNum; i++)
	{
		__msg("channelId = %d: %s\n",service_list->pServItems[i].channelId,service_list->pServItems[i].servName);
	}

	return service_list->servNum;

}


__s32 Maple_Program_Add_ServiceList(void)
{
    __u32 service_num = 0;

    service_num = search_service_list->servNum;
	
	if(search_service_list->servNum > 0)
	{
		esMODS_MIoctrl(maple, MAPLE_CMD_SL_ADD, 0, (void*)search_service_list);
		search_service_list->servNum = 0;
		search_service_list->curServIdx = 0;   
        eLIBs_memset(search_service_list->pServItems,0, sizeof(maple_serv_item_t)*MAPLE_MAX_SERV_NUM);
	}

	return service_num;

}


__s32 Maple_Program_Get_ServiceList(__u32 programtype)
{
    
    if(programtype == SSET_NC_PROGRAMTYPE_TV)
    {
        esMODS_MIoctrl(maple, MAPLE_CMD_SL_GET, MAPLE_SERV_GROUP_TYPE_TV, (void*)service_list);
    }
    else if(programtype == SSET_NC_PROGRAMTYPE_RADIO)
    {
        esMODS_MIoctrl(maple, MAPLE_CMD_SL_GET, MAPLE_SERV_GROUP_TYPE_RADIO, (void*)service_list);
    }   
    else if(programtype == SSET_NC_PROGRAMTYPE_ALL)
    {
        esMODS_MIoctrl(maple, MAPLE_CMD_SL_GET, MAPLE_SERV_GROUP_TYPE_ALL, (void*)service_list);
    }
    
    return service_list->servNum;
}


__s32 Maple_Get_Play_Status(void)
{

    if(!maple)
    {
		return -1;
	}
	
	return esMODS_MIoctrl(maple, MAPLE_CMD_GET_STATUS, 0, NULL);

}


//获取当前台数
__s32 Maple_Get_Total_Services(void)
{
	return service_list->servNum;
}

// 获取频道名称
//根据逻辑频道号，获取频道名称
char  *Maple_Get_Name_Of_Channel(__s32 index)
{
	if(index >= service_list->servNum  || index < 0)
	{
		return NULL;
	}
    
	return service_list->pServItems[index].servName;
}


//获取当前播放节目的逻辑频道号
__s32 Maple_Get_Current_Channel_Index(void)
{
	__s32 i;

	for(i=0; i < service_list->servNum; i++)
	{
        if(service_list->curServIdx == service_list->pServItems[i].channelId)
		{
			return i;
		}

	}

	return 0;
}



__s32  Maple_Set_Current_Channel(__s32 index)
{
    if(index < 0)
    {
        index = 0;
    }
    
	if(index > service_list->servNum-1)
	{
		return EPDK_FAIL;
	}

    if(service_list->curServIdx != service_list->pServItems[index].channelId)
    {
	    service_list->curServIdx = service_list->pServItems[index].channelId;
        save_para->index = index;        
    }
    return EPDK_OK;

}


__s32 Maple_Get_Current_Channel_Index_By_LCN(__s32 nRemoteControlKeyId)
{
    int      i;
	
    for(i = 0;i < service_list->servNum; i++)
    {
        if(nRemoteControlKeyId == service_list->pServItems[i].nRemoteControlKeyId)
        {
            return i;
        }
    }

    return EPDK_FAIL;
}

__s32 Maple_Set_Current_Channel_Index_By_LCN(__s32 nRemoteControlKeyId)
{
    int         ret;

	
    ret = Maple_Get_Current_Channel_Index_By_LCN(nRemoteControlKeyId);
    if(EPDK_FAIL != ret)
    {
    	service_list->curServIdx = ret;
        return EPDK_OK;
    }

    else
    {
        return EPDK_FAIL;
    }
}


__s32  Maple_Get_Current_Channel_LCN_ID(__s32 index)
{
	if(index > service_list->servNum)
	{
		return EPDK_FAIL;
	}

	
	return service_list->pServItems[index].nRemoteControlKeyId;
    
}


__s32 Maple_Play_Program(void)
{
    
    RECT                     video_window;
	
	__u32                    arg[3];
	
	__s32                    ret;
	
    __u8                     audio_index;
	

    if(service_list->servNum <= 0)
    {
		__wrn("service_list->servNum<=0");
		return EPDK_FAIL;
	}
		
	b_full_screen = EPDK_TRUE;

	
    //* set video layer handle to dvbc
    video_window.x      = 0;
    video_window.y      = 0;
    video_window.width  = eLIBs_fioctrl(dis_file, DISP_CMD_SCN_GET_WIDTH, 0, 0);

	//预留字幕显示区域，避免影响观看效果
    video_window.height = eLIBs_fioctrl(dis_file, DISP_CMD_SCN_GET_HEIGHT, 0, 0);
    video_layer_hdl = Maple_Request_Video_Layer(&video_window,0,0xff);

	Maple_Set_ScreenSize(MAPLE_VID_WINDOW_ORIGINAL);
    audio_index = Maple_Get_Audio_Num(service_list->curServIdx);
    __here__;
	esMODS_MIoctrl(maple, MAPLE_CMD_ENABLE_DEINTERLACE, 2, NULL);
	__here__;
    if(esMODS_MIoctrl(maple, MAPLE_CMD_PLAY, service_list->curServIdx,(void *)audio_index) != EPDK_OK)
    {
        __wrn("play service %d fail.\n", service_list->curServIdx);
		dsk_speaker_onoff(TV_SPEAKER_OFF);//播放失败关闭功放
		return EPDK_FAIL;
    }
    else
    {
	    ES_FILE* display;
        if(video_layer_hdl)
        {
    		arg[0] = video_layer_hdl;
    		arg[1] = 0;
    		arg[2] = 0;
    	    eLIBs_fioctrl(dis_file, DISP_CMD_LAYER_OPEN, 0, (void *)arg);
        }
        
		dsk_speaker_onoff(TV_SPEAKER_ON);//播放成功再打开功放
		
    }

	Maple_Epg_Search_Stop();
	
	esKRNL_TimeDly(1);
	
	Maple_Epg_Search();
	
	Maple_Time_Monitor_Stop();
	
	esKRNL_TimeDly(1);
	
	Maple_Time_Monitor_Start();
	

    return EPDK_OK;
}





__s32 Maple_Stop_Program(__u32 video_layer_state)
{
    __u32 arg[3];
	__s32 ret;
	__u8  i = 10;

    
    if(service_list->servNum <= 0) 
    {
        __wrn("service_list->servNum = %d \n",service_list->servNum);
		return EPDK_FAIL;
	}
        __here__;

	if(Maple_Get_Play_Status() == MAPLE_STAT_STOP)
	{
		return EPDK_OK;
	}
	
    __here__;
	
	Maple_Clear_Sepg();

	Maple_Epg_Search_Stop();

	Maple_Time_Monitor_Stop();
        __here__;

    esMODS_MIoctrl(maple, MAPLE_CMD_STOP, 0, NULL);
        __here__;

    if(video_layer_state == CLOSE_VIDEO_LAYER)
    {
        if(video_layer_hdl)
        {    	
    		arg[0] = video_layer_hdl;
    		arg[1] = 0;
    		arg[2] = 0;	


    		ret = eLIBs_fioctrl( dis_file, DISP_CMD_LAYER_RELEASE, 0, (void*)arg );        
	
    	   if(EPDK_FAIL == ret)
    	   {
    		   __wrn("stop maple fail .......\n");	
    		   return EPDK_FAIL;
    	   }
		   
    	   video_layer_hdl = 0;
        }
    	else
    	{
    		__msg("video_layer_hdl = NULL\n");
    	}
    }
    
	while(Maple_Get_Play_Status() != MAPLE_STAT_STOP)
	{
		esKRNL_TimeDly(1);
	};	

   
   return EPDK_OK;
}


//添加服务列表，并更新搜台总数
__s32 Maple_Search_List_Finish_Callback(void* arg)
{
    maple_sl_cb_param_t    *p = NULL;
	
	__s32                  program_num = 0;
	
	__s32                  i,count = 0;
    
    p = (maple_sl_cb_param_t*)arg;        

    program_num = Maple_Program_Add_ServiceList();  

//ISDBT在搜索某个频率结束时才知道台数
	esMODS_MIoctrl(maple, MAPLE_CMD_SL_GET, 0, (void*)search_service_list);
	for(i=0; i<program_num; i++)
	{
		if(search_service_list->pServItems[i].pmtId <= 0x1fcf)			
		{
			count++;
		}
		
	}
	
	program_num = count;
	
	if(program_num > 0)
	{
		Maple_App_CallBack(UPDATE_CHANNEL,program_num,0);

		esKRNL_TimeDly(10);
	}

	
    if(p && p->pOpaque)
    {
        esKRNL_SemPost((__krnl_event_t*)p->pOpaque);
        __wrn("search service finish.\n");
    }
  
	return EPDK_OK;
	
}



//用于更新当前搜台数，每搜到一个台会触发一次
__s32 Maple_Search_List_Event_Callback(void* arg)
{
    return EPDK_OK;
}



__s32 Maple_Register_Search_Callback(__pCBK_t cb_search_list_event_callback,__pCBK_t cb_search_list_finish_callback)
{
	__pCBK_t			callback;

	
	callback = esKRNL_GetCallBack(cb_search_list_event_callback);
	esMODS_MIoctrl(maple, MAPLE_CMD_CB_SET_PARAM, MAPLE_CB_SRCH_SL_EVENT, (void*)sl_event_cbp);
	esMODS_MIoctrl(maple, MAPLE_CMD_CB_SET, MAPLE_CB_SRCH_SL_EVENT, (void*)callback);

	callback = esKRNL_GetCallBack(cb_search_list_finish_callback);
	esMODS_MIoctrl(maple, MAPLE_CMD_CB_SET_PARAM, MAPLE_CB_SRCH_SL_FINISH, (void*)sl_event_cbp);
	esMODS_MIoctrl(maple, MAPLE_CMD_CB_SET, MAPLE_CB_SRCH_SL_FINISH, (void*)callback);
	__wrn("Maple_Register_Search_Callback exit.\n");
	return EPDK_OK;

}



__s32 Maple_Break_Search(void)
{
	esKRNL_SemPost(sl_event_cbp->pOpaque);
	
	search_task_break = EPDK_TRUE;

	return EPDK_OK;
}


void Maple_Search_Task(void* arg)
{

	int                         i;
	
	__u8				        err;
	
	__s32                       ret = EPDK_FAIL;
	
	area_frequencys_t            *area_frequencys;

    maple_demod_ss_t            ss;
    
    char                        bandw[24];
	
    char                        band[12];
	
    char                        frequ[30];
	
    char                        fre[18];


	search_task_break = EPDK_FALSE;
	
	
	area_frequencys = &area_frequencys_table[G_DTV_current_area];

	service_list->servNum	= 0;
	service_list->curServIdx = 0;
    eLIBs_memset(service_list->pServItems,0, sizeof(maple_serv_item_t)*MAPLE_MAX_SERV_NUM);
    
	search_service_list->servNum	= 0;
	search_service_list->curServIdx = 0;
    eLIBs_memset(search_service_list->pServItems,0, sizeof(maple_serv_item_t)*MAPLE_MAX_SERV_NUM);
     
    
    sl_event_cbp->pOpaque	= NULL;
	sl_event_cbp->pOpaque = esKRNL_SemCreate(0);
	
	if (!sl_event_cbp->pOpaque)
	{
		__wrn("sl search semaphore create fail.\n");
		goto _sl_search_exit;
	}
	
	if(NULL == service_list->pServItems)
	{
		__wrn("malloc fail.\n");
		goto _sl_search_exit;
	}


	esMODS_MIoctrl(maple, MAPLE_CMD_SL_DELETE, 0, NULL);
    

	for(i = 0;i < area_frequencys->frequency_total;i++)
    {
         esKRNL_TimeDly(1);

		if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
		{
			 __msg("Maple_Search_Task()... abort...\n");
			 goto _sl_search_exit;
		}

        tps.bandwidth = area_frequencys->frequencys[i].BandWidth;
        tps.frequency = area_frequencys->frequencys[i].StartFreq;
	
        //如果上次等待超时，则maple任务会被杀掉，maple多发一个finish，callback，信号量增加1，导致下一个频点搜索失败
        esKRNL_SemAccept(sl_event_cbp->pOpaque);
		
        Maple_Set_Current_Search_Frequency(tps.frequency,i);
		__here__;
		
	    if(EPDK_TRUE ==  search_task_break)
	    {
			goto _sl_search_exit;
		}

        if(esMODS_MIoctrl(maple, MAPLE_CMD_TUNE, (tps.bandwidth/10)<<22 | tps.frequency, NULL) != 0)
        {
            __wrn(" tune maple fail.\n");
            continue;
        }
        
	    if(EPDK_TRUE ==  search_task_break)
	    {
			goto _sl_search_exit;
		}		

		__here__;

	    esMODS_MIoctrl(maple, MAPLE_CMD_SRCH_SERV, (tps.bandwidth/10)<<22 | tps.frequency, (void*)search_service_list);

		
	    if(EPDK_TRUE ==  search_task_break)
	    {
			goto _sl_search_exit;
		}

		//* wait a long time for search progress.
		

	    esKRNL_SemPend(sl_event_cbp->pOpaque, 5000, &err);


        ret = esMODS_MIoctrl(maple, MAPLE_CMD_SRCH_SERV_STOP, 0, NULL);

	    if(EPDK_TRUE ==  search_task_break)
	    {
			goto  _sl_search_exit;
		}
        
    }

    Maple_Service_List_Get(MAPLE_SERV_GROUP_TYPE_TV);


	if(service_list->servNum > 0)//搜台结束之后做一次保存
	{
        esMODS_MIoctrl(maple, MAPLE_CMD_SORT_SERV_LIST,0, NULL);
        
        Maple_Service_List_Get(MAPLE_SERV_GROUP_TYPE_TV);
        
		Maple_Reg_Flush();
	}
    
	
    
	if(err != 0)
	{
		__wrn("service arealist search fail.\n");
	}
	else
	{
		__wrn("service arealist search finish.\n");
	}
	
	
_sl_search_exit:

	search_task_break = EPDK_FALSE;
	

	if(sl_event_cbp->pOpaque)
	{
		esKRNL_SemDel(sl_event_cbp->pOpaque, OS_DEL_ALWAYS, &err);
		sl_event_cbp->pOpaque = NULL;
	}
	
	eDbug("service area search task exit.\n");


	Maple_App_CallBack(DELETE_SEARCH_THREAD,0,0);
	
	
    esKRNL_TDel(EXEC_prioself);
	
} 


__s32 Maple_Search(__u32 *task_ID)
{    
    if(*task_ID != 0)
    {
		return EPDK_OK;
	}

	
	*task_ID = esKRNL_TCreate(Maple_Search_Task, (void*)NULL, 0x1000, KRNL_priolevel3);

	if (*task_ID == 0)
	{
		eDbug("create search_task fail.\n");
		return EPDK_FAIL;
	}
	else
	{		
		eDbug("create search_task[id = %d] success.\n",*task_ID);
		return EPDK_OK;
	}

}



__s32 Maple_Search_Delete(__u32 thread_id)
{
	if( thread_id != 0 )
	{
		__s8 ret;
		while( (ret = esKRNL_TDelReq(thread_id)) != OS_TASK_NOT_EXIST )
        {  
			__msg("thread_id = %d : %d\n", thread_id, ret);
            esKRNL_TimeDlyResume(thread_id);
			esKRNL_TimeDly(5);
        }
		
		thread_id = 0;
		
		__msg("thread has been deleted.\n");
	}

    return EPDK_OK;

}

/*
**********************************************************************************************************************
*                                            Maple_Open
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
__s32 Maple_Open(void)
{

//mod:	
    __here__;

	
	maple_mid = esMODS_MInstall("d:\\mod\\maple.mod", 0);
	

    if(!maple_mid)
    {
        __wrn("install maple fail.\n");
		//goto mod;
		maple = NULL;
        return EPDK_FAIL;
    }
    __here__;

//mod1:
	
	maple = esMODS_MOpen(maple_mid, 0);	
    if(!maple)
    {
        __wrn("open maple fail.\n");
		//goto mod1;
        goto err0;
    }
    __here__;
    dis_file = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if(!dis_file)
	{
	    __wrn("Open display driver failed!\n");
	   goto err1;
	}

    __here__;
	Maple_Set_TV_Mode(1);

    sl_event_cbp = (maple_sl_cb_param_t *)esMEMS_Malloc(0, sizeof(maple_sl_cb_param_t));
    eLIBs_memset(sl_event_cbp,0,sizeof(sl_event_cbp));
    
    service_list=(maple_serv_list_t *)esMEMS_Malloc(0, sizeof(maple_serv_list_t));
    eLIBs_memset(service_list,0,sizeof(service_list));

    service_list->pServItems = (maple_serv_item_t *)esMEMS_Malloc(0, sizeof(maple_serv_item_t)*MAPLE_MAX_SERV_NUM);
    eLIBs_memset(service_list->pServItems,0,sizeof(service_list->pServItems));
    
	search_service_list = (maple_serv_list_t *)esMEMS_Malloc(0, sizeof(maple_serv_list_t));
    eLIBs_memset(search_service_list,0,sizeof(search_service_list));
	
	search_service_list->pServItems = (maple_serv_item_t *)esMEMS_Malloc(0, sizeof(maple_serv_item_t)*MAPLE_MAX_SERV_NUM);
    eLIBs_memset(search_service_list->pServItems,0,sizeof(search_service_list->pServItems));

    schdl_event_cbp = (maple_schdl_cb_param_t *)esMEMS_Malloc(0,sizeof(maple_schdl_cb_param_t));
    eLIBs_memset(schdl_event_cbp,0,sizeof(schdl_event_cbp));

    schdl_list = (maple_schdl_list_t *)esMEMS_Malloc(0,sizeof(maple_schdl_list_t));
    eLIBs_memset(schdl_list,0,sizeof(schdl_list));

    schdl_list->pSchdlEvents = (maple_schdl_event_t *)esMEMS_Malloc(0, sizeof(maple_schdl_event_t)*MAPLE_MAX_SCHDL_NUM);
    eLIBs_memset(schdl_list->pSchdlEvents,0,sizeof(schdl_list->pSchdlEvents));
    
    sepg_finish_cbp = (maple_sepg_cb_param_t *)esMEMS_Malloc(0,sizeof(maple_sepg_cb_param_t));
    eLIBs_memset(sepg_finish_cbp,0,sizeof(sepg_finish_cbp));

    sepg = (maple_sepg_t *)esMEMS_Malloc(0,sizeof(maple_sepg_t)); 
    eLIBs_memset(sepg,0,sizeof(sepg));
    
    sepg_current = (char *)esMEMS_Malloc(0,MAX_SEPG_STRING);
    eLIBs_memset(sepg_current,0,sizeof(sepg_current));
    
    sepg_next = (char *)esMEMS_Malloc(0,MAX_SEPG_STRING);
    eLIBs_memset(sepg_next,0,sizeof(sepg_next));

		
    ca_sn = (maple_cas_param_t *)esMEMS_Malloc(0,MAX_TMP_STRING);
    eLIBs_memset(ca_sn,0,sizeof(ca_sn));
  
	ca_sn->casNum = (__u8 *)esMEMS_Malloc(0,32);
    eLIBs_memset(ca_sn->casNum,0,sizeof(ca_sn->casNum));
	
__here__;
        
    schdl_list_day = (maple_schdl_list_day *)esMEMS_Malloc(0,MAX_EPG_DAY*sizeof(maple_schdl_list_day));
    eLIBs_memset(schdl_list_day,0,MAX_EPG_DAY*sizeof(maple_schdl_list_day));
    
	tv_time = (dtvmid_time_t *)esMEMS_Malloc(0,sizeof(dtvmid_time_t));
    eLIBs_memset(tv_time,0,sizeof(tv_time));

    maple_CmdQ = esKRNL_QCreate(QSIZE);
    maple_pFedBakQ = esKRNL_QCreate(QSIZE);

	save_para = (maple_save_para_t *)esMEMS_Malloc(0,sizeof(maple_save_para_t));
    eLIBs_memset(save_para,0,sizeof(save_para));


	b_full_screen = EPDK_TRUE;
    __here__;

    
	return EPDK_OK;
err1:
	esMODS_MClose(maple);
	maple = NULL;

err0:
    esMODS_MUninstall(maple_mid);
	maple_mid = NULL;	

	return EPDK_FAIL;
}




/*
**********************************************************************************************************************
*                                            Maple_Close
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
__s32 Maple_Close(void)
{	
    __u8 err;
	__s32 ret;


    //* close demod driver.
    Maple_Set_TV_Mode(0);
	
    Maple_Time_Monitor_Stop();
	
	Maple_Epg_Search_Stop();

    Maple_Stop_Program(CLOSE_VIDEO_LAYER);	
	
    if(maple)
    {
        ret = esMODS_MClose(maple);
        maple = NULL;
        __wrn(" close maple ret = %d.\n",ret);
    }
    
    //* uninstall demod driver.
    if(maple_mid)
    {
        ret = esMODS_MUninstall(maple_mid);
        maple_mid = 0;
        __wrn("uninstall maple. ret = %d \n",ret);
		
    }

    
    if(video_layer_hdl)
    {       
        __u32 arg[3];
        
        arg[0] = video_layer_hdl;
        arg[1] = 0;
        arg[2] = 0; 
        eLIBs_fioctrl( dis_file, DISP_CMD_LAYER_RELEASE, 0, (void*)arg );        
        
       
       video_layer_hdl = 0;
    }

	
    eLIBs_fclose(dis_file);

	if(service_list->pServItems)
	{
	    
		esMEMS_Mfree(0, service_list->pServItems);
		service_list->pServItems = NULL;
	}
	
	esMEMS_Mfree(0, service_list);
	
	esMEMS_Mfree(0, sl_event_cbp);

    if(schdl_list->pSchdlEvents)
    {
        
        esMEMS_Mfree(0, schdl_list->pSchdlEvents);
        schdl_list->pSchdlEvents = NULL;
    }
	
    esMEMS_Mfree(0, schdl_list);
	
    esMEMS_Mfree(0, schdl_event_cbp);

    if (search_service_list->pServItems)
    {
        
        esMEMS_Mfree(0, search_service_list->pServItems);
        search_service_list->pServItems = NULL;
    }
	
    esMEMS_Mfree(0, search_service_list);
    
    esMEMS_Mfree(0, sepg);
	
    esMEMS_Mfree(0, sepg_finish_cbp);
    
    esMEMS_Mfree(0, sepg_current);
	
    esMEMS_Mfree(0, sepg_next);
	    

    esMEMS_Mfree(0, schdl_list_day);
    esMEMS_Mfree(0,tv_time);

	
	esMEMS_Mfree(0, ca_sn->casNum);
	ca_sn->casNum = 0;

    esMEMS_Mfree(0, ca_sn);
    
    
    if(maple_CmdQ)
    {
        esKRNL_QDel(maple_CmdQ,0,&err);
        maple_CmdQ = NULL;
    }

    if(maple_pFedBakQ)
    {
        esKRNL_QDel(maple_pFedBakQ,0,&err);
        maple_pFedBakQ = NULL;
    }

    esMEMS_Mfree(0,save_para);

	

	return EPDK_OK;

}


