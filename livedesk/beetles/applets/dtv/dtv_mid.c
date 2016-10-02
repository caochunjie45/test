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

#include "dtv_i.h"

extern dtv_ctr_t  dtv_ctr;
extern H_WIN g_dtv_mainwin;
extern __bool save_flag;

//在此定义的全局变量最好放到结构休中，
//仅声明该结构休的一个全局指针，指向在堆中申请的内存
//减小栈的使用
__u32 maple_mid;
__mp* maple = NULL;
extern __s32 cur_playing_index;



char *str = NULL;
char *strbw = NULL;

char *epgdate = NULL;
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)  
char *ca_sn = NULL;
#else
maple_cas_param_t *ca_sn=NULL;
#endif


#if EN_DTV_RECORD
char			curSearchDisk[16];
#endif

static __u32               video_layer_hdl = NULL;
static ES_FILE*            dis_file = NULL;

//服务列表(频道列表)
maple_demod_info_t	tps;
maple_sl_cb_param_t* sl_event_cbp = NULL;
__u32               sl_task_id;
maple_serv_list_t*	service_list = NULL;
maple_serv_list_t* search_service_list = NULL;//仅用于搜索时用

//当前频道的节目预告
maple_schdl_cb_param_t*  schdl_event_cbp = NULL;
maple_schdl_list_t*      schdl_list = NULL;
maple_schdl_list_day*   schdl_list_day = NULL;
__u32               schedule_task_id=0;
dtvmid_time_t *tv_time = NULL;


//当前频道的当前正在播放和下一个将要播放的节目预告
maple_sepg_t*            sepg = NULL;
maple_sepg_cb_param_t*   sepg_finish_cbp = NULL;
__u32               sepg_task_id=0;
char * sepg_string_c = NULL;
char * sepg_string_n = NULL;
__s32 g_strength = 0;
__s32 g_quality = 0;
__s32 g_weak_singal = 1;
__s32 g_record_flag = 0;

maple_ve_status_e *g_maple_ve_status = NULL;
reg_cmmb_para_t cmmb_para= {0};


//C300中未使用
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)  
maple_cmmb_datasvc_cb_param_t* cmmb_datasvc_updatebegin_para = NULL;
maple_cmmb_datasvc_cb_param_t* cmmb_datasvc_updateend_para = NULL;
#endif


__s32 maple_cur_search_freq = 0;//当前搜索的频点

static __epdk_charset_enm_e g_type = EPDK_CHARSET_ENM_ISO_8859_15; //默认巴西,需要根据出往哪个国家进行设置

static __u32 G_DTV_current_area = 0;//1;   //0:只用于深圳频率，1:全频段搜索



static __u32 g_programtype = SSET_NC_PROGRAMTYPE_TV;

#define QSIZE 8
__krnl_event_t  *maple_CmdQ = NULL;
__krnl_event_t  *maple_pFedBakQ = NULL; 



__bool b_full_screen = EPDK_TRUE;  //非缩略图播放


const Region_TPSects_t TPRegion_England[]=
{

#if(MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
	{ 52500 , 8000},
	{ 60500 , 8000},
	{ 68500 , 8000},
	{ 80000 , 8000},
	{ 88000 , 8000},
	{171000 , 8000},
	{179000 , 8000},
	{187000 , 8000},
	{195000 , 8000},
	{203000 , 8000},
	{211000 , 8000},
	{219000 , 8000},

	{474000,8000},
	{482000,8000},
	{490000,8000},
	{498000,8000},
	{506000,8000},
	{514000,8000},
	{522000,8000},
	{530000,8000},
	{538000,8000},
	{546000,8000},
	{554000,8000},
	{562000,8000},
	{610000,8000},
	{618000,8000},
	{626000,8000},
	{634000,8000},
	{642000,8000},
	{650000,8000},
	{658000,8000},
	{666000,8000},
	{674000,8000},
	{682000,8000},
	{690000,8000},
	{698000,8000},
	{706000,8000},
	{714000,8000},
	{722000,8000},
	{730000,8000},
	{738000,8000},
	{746000,8000},
	{754000,8000},
	{762000,8000},
	{770000,8000},
	{778000,8000},
	{786000,8000},
	{794000,8000},
	{802000,8000},
	{810000,8000},
	{818000,8000},
	{826000,8000},
	{834000,8000},
	{842000,8000},
	{850000,8000},
	{858000,8000}, 
    
#elif(MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
	{722000 , 8000},
    {730000 , 8000},
    {473000 , 6000},
    {479000 , 6000},
    {485000 , 6000},
    {491000 , 6000},
    {497000 , 6000},
    {503000 , 6000},
    {509000 , 6000},
    {515000 , 6000},
    {521000 , 6000},
    {527000 , 6000},
    {533000 , 6000},
    {539000 , 6000},
    {545000 , 6000},
    {551000 , 6000},
    {557000 , 6000},
    {563000 , 6000},     
    {569000 , 6000},    
	{575000 , 6000},
    {581000 , 6000},
    {587000 , 6000},
    {593000 , 6000},
    {599000 , 6000},
    {605000 , 6000},
    {611000 , 6000},
    {617000 , 6000},
    {623000 , 6000},
    {629000 , 6000},
    {635000 , 6000},
    {641000 , 6000},
    {647000 , 6000},
    {653000 , 6000},
    {659000 , 6000},
 	{665000 , 6000},
    {671000 , 6000},
    {677000 , 6000},
    {683000 , 6000},
    {689000 , 6000},
    {695000 , 6000},
    {701000 , 6000},
    {707000 , 6000},
    {713000 , 6000},
    {719000 , 6000},
    {725000 , 6000},
    {731000 , 6000},
    {737000 , 6000},
    {743000 , 6000},
    {749000 , 6000},
    {755000 , 6000},
    {761000 , 6000},
    {767000 , 6000},
    {773000 , 6000},
    {779000 , 6000},
    {785000 , 6000},
    {791000 , 6000},
    {797000 , 6000},
    {803000 , 6000},
#elif(MAPLE_TV_STANDAR == MAPLE_TV_ISDB_T)
    {473143 , 6000},
    {479143 , 6000},
    {485143 , 6000},
    {491143 , 6000},
    {497143 , 6000},
    {503143 , 6000},
    {509143 , 6000},
    {515143 , 6000},
    {521143 , 6000},
    {527143 , 6000},
    {533143 , 6000},
    {539143 , 6000},
    {545143 , 6000},
    {551143 , 6000},
    {557143 , 6000},
    {563143 , 6000},     
    {569143 , 6000},    
	{575143 , 6000},
    {581143 , 6000},
    {587143 , 6000},
    {593143 , 6000},
    {599143 , 6000},
    {605143 , 6000},
    {611143 , 6000},
    {617143 , 6000},
    {623143 , 6000},
    {629143 , 6000},
    {635143 , 6000},
    {641143 , 6000},
    {647143 , 6000},
    {653143 , 6000},
    {659143 , 6000},
    {665143 , 6000},
    {671143 , 6000},
    {677143 , 6000},
    {683143 , 6000},
    {689143 , 6000},
    {695143 , 6000},
    {701143 , 6000},
    {707143 , 6000},
    {713143 , 6000},
    {719143 , 6000},
    {725143 , 6000},
    {731143 , 6000},
    {737143 , 6000},
    {743143 , 6000},
    {749143 , 6000},
    {755143 , 6000},
    {761143 , 6000},
    {767143 , 6000},
    {773143 , 6000},
    {779143 , 6000},
    {785143 , 6000},
    {791143 , 6000},
    {797143 , 6000},
    {803143 , 6000},
#endif
};

const Region_TPSects_t TPRegion_China[]=
{
#if(MAPLE_TV_STANDAR == MAPLE_TV_DTMB)
	{474000,8000},
	{482000,8000},
	{490000,8000},
	{498000,8000},
	{506000,8000},
	{514000,8000},
	{522000,8000},
	{530000,8000},
	{538000,8000},
	{546000,8000},
	{554000,8000},
	{562000,8000},
	{610000,8000},
	{618000,8000},
	{626000,8000},
	{634000,8000},
	{642000,8000},
	{650000,8000},
	{658000,8000},
	{666000,8000},
	{674000,8000},
	{682000,8000},
	{690000,8000},
	{698000,8000},
	{706000,8000},
	{714000,8000},
	{722000,8000},
	{730000,8000},
	{738000,8000},
	{746000,8000},
	{754000,8000},
	{762000,8000},
	{770000,8000},
	{778000,8000},
	{786000,8000},
	{794000,8000},
	{802000,8000},
	{810000,8000},
	{818000,8000},
	{826000,8000},
	{834000,8000},
	{842000,8000},
	{850000,8000},
	{858000,8000}, 

#elif(MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
	{474000,8000},
	{482000,8000},
    {490000,8000},
    {498000,8000},
    {506000,8000},
    {514000,8000},
    {522000,8000},
    {530000,8000},
    {538000,8000},
    {546000,8000},
    {554000,8000},
    {562000,8000},
    {570000,8000},
    {578000,8000},
    {586000,8000},
    {594000,8000},
    {602000,8000},
    {610000,8000},
    {618000,8000},
    {626000,8000},
    {634000,8000},
    {642000,8000},
    {650000,8000},
    {658000,8000},
    {666000,8000},
    {674000,8000},
    {682000,8000},
    {690000,8000},
    {698000,8000},
    {706000,8000},
    {714000,8000},
    {722000,8000},
    {730000,8000},
    {738000,8000},
    {746000,8000},
    {754000,8000},
    {762000,8000},
    {770000,8000},
    {778000,8000},
    {786000,8000},
    {794000,8000},
    {802000,8000},
    {810000,8000},
    {818000,8000},
    {826000,8000},
    {834000,8000},
    {842000,8000},
    {850000,8000},
    {858000,8000}, 
    {866000,8000}, 
#elif(MAPLE_TV_STANDAR == MAPLE_TV_ISDB_T)
    {473143 , 6000},
    {479143 , 6000},
    {485143 , 6000},
    {491143 , 6000},
    {497143 , 6000},
    {503143 , 6000},
    {509143 , 6000},
    {515143 , 6000},
    {521143 , 6000},
    {527143 , 6000},
    {533143 , 6000},
    {539143 , 6000},
    {545143 , 6000},
    {551143 , 6000},
    {557143 , 6000},
    {563143 , 6000},     
    {569143 , 6000},    
	{575143 , 6000},
    {581143 , 6000},
    {587143 , 6000},
    {593143 , 6000},
    {599143 , 6000},
    {605143 , 6000},
    {611143 , 6000},
    {617143 , 6000},
    {623143 , 6000},
    {629143 , 6000},
    {635143 , 6000},
    {641143 , 6000},
    {647143 , 6000},
    {653143 , 6000},
    {659143 , 6000},
    {665143 , 6000},
    {671143 , 6000},
    {677143 , 6000},
    {683143 , 6000},
    {689143 , 6000},
    {695143 , 6000},
    {701143 , 6000},
    {707143 , 6000},
    {713143 , 6000},
    {719143 , 6000},
    {725143 , 6000},
    {731143 , 6000},
    {737143 , 6000},
    {743143 , 6000},
    {749143 , 6000},
    {755143 , 6000},
    {761143 , 6000},
    {767143 , 6000},
    {773143 , 6000},
    {779143 , 6000},
    {785143 , 6000},
    {791143 , 6000},
    {797143 , 6000},
    {803143 , 6000},
#endif
};

const  char *const TPStr_England[]=
{
    "1", "2","3","4", "5","6","7", "8","9","10",
    "1", "2","3","4", "5","6","7", "8","9","10",
    "1", "2","3","4", "5","6","7", "8","9","10",
    "1", "2","3","4", "5","6","7", "8","9","10",
    "1", "2","3","4", "5","6","7", "8","9"
};

const  char *const TPStr_China[]=
{
    "1", "2","3","4", "5","6","7", "8","9","10",
    "1", "2","3","4", "5","6","7", "8","9","10",
    "1", "2","3","4", "5","6","7", "8","9","10",
    "1", "2","3","4", "5","6","7", "8","9","10",
    "1", "2","3","4", "5","6","7", "8","9"
};

static void _clearprogramname(void);

DVBT_AreaTable_t dvb_t_areatp_table[SSET_NC_AREASET_AREAMAX]={
	
	{SSET_NC_AREASET_SHENZHEN,TPRegion_China,TPStr_China,sizeof(TPRegion_China)/sizeof(Region_TPSects_t),"CHN"}, 
	{SSET_NC_AREASET_ALLFREQ,TPRegion_England,TPStr_England,sizeof(TPRegion_England)/sizeof(Region_TPSects_t),"GBR"},    
    
};



/*
**********************************************************************************************************************
*                                            maple_open
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
__s32 maple_open(void)
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
	maple_set_tv_mode(1);

    sl_event_cbp = (maple_sl_cb_param_t*)esMEMS_Malloc(0, sizeof(maple_sl_cb_param_t));
    eLIBs_memset(sl_event_cbp,0,sizeof(sl_event_cbp));
    
    service_list=(maple_serv_list_t*)esMEMS_Malloc(0, sizeof(maple_serv_list_t));
    eLIBs_memset(service_list,0,sizeof(service_list));

    service_list->pServItems = (maple_serv_item_t*)esMEMS_Malloc(0, sizeof(maple_serv_item_t)*MAPLE_MAX_SERV_NUM);
    eLIBs_memset(service_list->pServItems,0,sizeof(service_list->pServItems));
    
	search_service_list = (maple_serv_list_t*)esMEMS_Malloc(0, sizeof(maple_serv_list_t));
    eLIBs_memset(search_service_list,0,sizeof(search_service_list));
	
	search_service_list->pServItems = (maple_serv_item_t*)esMEMS_Malloc(0, sizeof(maple_serv_item_t)*MAPLE_MAX_SERV_NUM);
    eLIBs_memset(search_service_list->pServItems,0,sizeof(search_service_list->pServItems));

    schdl_event_cbp = (maple_schdl_cb_param_t*)esMEMS_Malloc(0,sizeof(maple_schdl_cb_param_t));
    eLIBs_memset(schdl_event_cbp,0,sizeof(schdl_event_cbp));

    schdl_list = (maple_schdl_list_t*)esMEMS_Malloc(0,sizeof(maple_schdl_list_t));
    eLIBs_memset(schdl_list,0,sizeof(schdl_list));

    schdl_list->pSchdlEvents = (maple_schdl_event_t*)esMEMS_Malloc(0, sizeof(maple_schdl_event_t)*MAPLE_MAX_SCHDL_NUM);
    eLIBs_memset(schdl_list->pSchdlEvents,0,sizeof(schdl_list->pSchdlEvents));
    
    sepg_finish_cbp = (maple_sepg_cb_param_t*)esMEMS_Malloc(0,sizeof(maple_sepg_cb_param_t));
    eLIBs_memset(sepg_finish_cbp,0,sizeof(sepg_finish_cbp));

    sepg = (maple_sepg_t*)esMEMS_Malloc(0,sizeof(maple_sepg_t)); 
    eLIBs_memset(sepg,0,sizeof(sepg));
    
    sepg_string_c = (char*)esMEMS_Malloc(0,MAX_SEPG_STRING);
    eLIBs_memset(sepg_string_c,0,sizeof(sepg_string_c));
    
    sepg_string_n = (char*)esMEMS_Malloc(0,MAX_SEPG_STRING);
    eLIBs_memset(sepg_string_n,0,sizeof(sepg_string_n));
    
    str = (char*)esMEMS_Malloc(0,MAX_TMP_STRING);
    eLIBs_memset(str,0,sizeof(str));
    
    strbw = (char*)esMEMS_Malloc(0,MAX_TMP_STRING);
    eLIBs_memset(strbw,0,sizeof(strbw));
    
    epgdate = (char*)esMEMS_Malloc(0,MAX_TMP_STRING);
    eLIBs_memset(epgdate,0,sizeof(epgdate));
    
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)  
	ca_sn = (char *)esMEMS_Malloc(0,MAX_TMP_STRING);
#else
    ca_sn = (maple_cas_param_t *)esMEMS_Malloc(0,MAX_TMP_STRING);
    eLIBs_memset(ca_sn,0,sizeof(ca_sn));
  
	ca_sn->casNum = (__u8*)esMEMS_Malloc(0,32);
    eLIBs_memset(ca_sn->casNum,0,sizeof(ca_sn->casNum));
	
#endif
__here__;


#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)  
    cmmb_datasvc_updatebegin_para = (maple_cmmb_datasvc_cb_param_t*)esMEMS_Malloc(0,sizeof(maple_cmmb_datasvc_cb_param_t));
    eLIBs_memset(cmmb_datasvc_updatebegin_para,0,sizeof(cmmb_datasvc_updatebegin_para));

    cmmb_datasvc_updateend_para = (maple_cmmb_datasvc_cb_param_t*)esMEMS_Malloc(0,sizeof(maple_cmmb_datasvc_cb_param_t));
    eLIBs_memset(cmmb_datasvc_updateend_para,0,sizeof(cmmb_datasvc_updateend_para));

#endif
        
    schdl_list_day = (maple_schdl_list_day*)esMEMS_Malloc(0,MAX_EPG_DAY*sizeof(maple_schdl_list_day));
    eLIBs_memset(schdl_list_day,0,MAX_EPG_DAY*sizeof(maple_schdl_list_day));
    
	tv_time = (dtvmid_time_t *)esMEMS_Malloc(0,sizeof(dtvmid_time_t));
    eLIBs_memset(tv_time,0,sizeof(tv_time));

    maple_CmdQ = esKRNL_QCreate(QSIZE);
    maple_pFedBakQ = esKRNL_QCreate(QSIZE);

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
*                                            maple_close
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
__s32 maple_close(void)
{	
    __u8 err;
	__s32 ret;


    //* close demod driver.
    maple_set_tv_mode(0);
    maple_time_monitor_stop();
	maple_epg_search_stop();
    maple_search_stop();

    maple_stop_program(CLOSE_VIDEO_LAYER);	
	
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
    
    esMEMS_Mfree(0, sepg_string_c);
	
    esMEMS_Mfree(0, sepg_string_n);
	    
    esMEMS_Mfree(0,str);
    esMEMS_Mfree(0, strbw);
    esMEMS_Mfree(0, epgdate);

    esMEMS_Mfree(0, schdl_list_day);
    esMEMS_Mfree(0,tv_time);
#if (MAPLE_TV_STANDAR != MAPLE_TV_CMMB)  	
	esMEMS_Mfree(0, ca_sn->casNum);
	ca_sn->casNum = 0;
#endif	
    esMEMS_Mfree(0, ca_sn);
    
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)  
    esMEMS_Mfree(0,cmmb_datasvc_updatebegin_para);   
    esMEMS_Mfree(0,cmmb_datasvc_updateend_para);
#endif
    
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

	return EPDK_OK;

}

//根据当前搜索第几个频点，获取该频点
__s32 maple_get_cur_search_freq(__s32 index)
{
	DVBT_AreaTable_t * area_p;

	area_p = &dvb_t_areatp_table[G_DTV_current_area];

	return area_p->pTPRegion_Area[index].StartFreq;
}

//当前需要搜索的总频点数
__s32 maple_get_cur_search_freq_tatol(void)
{
	DVBT_AreaTable_t * area_p;

	area_p = &dvb_t_areatp_table[G_DTV_current_area];

	return (area_p->TPIndex);
}

//设置当前正在搜索的频点，用于更新搜索界面的频率显示
static void maple_set_cur_search_freq(__s32 freq,__u16 index)
{
	maple_cur_search_freq = freq;	

	if(dtv_ctr.h_scan)
	{
		__gui_msg_t 	setmsg;
		setmsg.id			=  GUI_MSG_COMMAND;	   
		setmsg.h_srcwin 	=  NULL;
		setmsg.h_deswin 	=  dtv_ctr.h_scan;	 
		setmsg.dwAddData1	= (index<<16)|SLIDE_UPDATE_FREQ;
		setmsg.dwAddData2	= freq;
		setmsg.p_arg		=  NULL;
		GUI_SendNotifyMessage(&setmsg);
	}	
}

#if (!IS_CA_EXIST)

#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)

//只保留免费台,add by hot.lee
static __s32 maple_get_free_channel(void)
{
	__s32 i=0;
	__s32 count = 0;
	__s32  channel[128]={0};

	for(i=0; i<search_service_list->servNum; i++)
	{
	    __s32 servid = search_service_list->pServItems[i].servId;
		//if(1 == search_service_list->pServItems[i].isCAFree)//CMMB协议未定义

		//排除4个固定的收费频率
		if(		CCTV1 != servid
			&&  QING_CHAI_DIAN_YIN != servid
			&&  CCTV5 != servid
			&&  QING_CHAI_DIAN_TIANXIA != servid)
		{
		    __msg("count = %d\n",count);
		    eLIBs_memcpy(&service_list->pServItems[count],&search_service_list->pServItems[i],sizeof(maple_serv_item_t));
			count++;
		}
	}
    __msg("count = %d\n",count);
		
	service_list->servNum = count;

    //更新中间件,因为中间在退出时保存时包括收费频道
    //不然退出再进入时频道号是错误的
	{
		esMODS_MIoctrl(maple, MAPLE_CMD_SL_DELETE, 0, NULL);
		
		//if(cmmb_para->flag == EPDK_TRUE)
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
#endif

#endif


#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)

//默认播放新闻台
//add by hot.lee
static __s32 __get_new_channel(void)
{
    __s32 i;
	for(i=0; i<service_list->servNum; i++)
	{
		if(service_list->pServItems[i].servId == CCTV_NEWS)//新闻台
		{
			maple_set_cur_channel(i);
			break;
		}
	}
}
#endif

#if (MAPLE_TV_STANDAR == MAPLE_TV_ISDB_T)
static __s32 __get_valid_channel(void)
{
	__s32 i=0,j=0;

	__msg("search_service_list->servNum = %d\n",search_service_list->servNum);
	for(i=0; i<search_service_list->servNum; i++)
	{
		__msg("search_service_list->pServItems->pmtId = %d\n",search_service_list->pServItems[i].pmtId);
		if(/*search_service_list->pServItems[i].pmtId >= 0x1fc8 
			&&*/ search_service_list->pServItems[i].pmtId <= 0x1fcf)
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
		
		//if(cmmb_para->flag == EPDK_TRUE)
		{
			//经过MAPLE_CMD_SL_ADD后，channelId会发生变化，所以需要重新调用MAPLE_CMD_SL_GET
			esMODS_MIoctrl(maple, MAPLE_CMD_SL_ADD, 0, (void*)service_list);
					
			service_list->servNum	 = 0;
			service_list->curServIdx = 0;
			 eLIBs_memset((void*)service_list->pServItems,0,sizeof(maple_serv_item_t)*MAPLE_MAX_SERV_NUM); 	
			esMODS_MIoctrl(maple, MAPLE_CMD_SL_GET, 0, (void*)service_list);			
		}	

	}	
}

#endif

#if (MAPLE_TV_STANDAR == MAPLE_TV_DTMB || MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)
static __s32 __get_valid_channel(void)
{
	__s32 i=0,j=0;

	__msg("search_service_list->servNum = %d\n",search_service_list->servNum);
	for(i=0; i<search_service_list->servNum; i++)
	{
		__msg("search_service_list->pServItems->pmtId = %d\n",search_service_list->pServItems[i].pmtId);
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
		
		//if(cmmb_para->flag == EPDK_TRUE)
		{
			//经过MAPLE_CMD_SL_ADD后，channelId会发生变化，所以需要重新调用MAPLE_CMD_SL_GET
			esMODS_MIoctrl(maple, MAPLE_CMD_SL_ADD, 0, (void*)service_list);
					
			service_list->servNum	 = 0;
			service_list->curServIdx = 0;
			 eLIBs_memset((void*)service_list->pServItems,0,sizeof(maple_serv_item_t)*MAPLE_MAX_SERV_NUM); 	
			esMODS_MIoctrl(maple, MAPLE_CMD_SL_GET, 0, (void*)service_list);			
		}	

	}	
}

#endif
//modify by hot.lee
__s32 maple_service_list_get(maple_serv_group_type_e type)
{
#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)    	
	
#if (IS_CA_EXIST == 0) 
		esMODS_MIoctrl(maple, MAPLE_CMD_SL_GET, type, (void*)search_service_list);//MAPLE_SERV_GROUP_TYPE_ALL
		maple_get_free_channel();

		__get_new_channel();//默认播放新闻台

		//service_list->curServIdx = service_list->pServItems[0].channelId; //搜索结束默认播放第一个有效台
		
#else //#if (IS_CA_EXIST == 0)
		esMODS_MIoctrl(maple, MAPLE_CMD_SL_GET, type, (void*)service_list);//MAPLE_SERV_GROUP_TYPE_ALL
		//service_list->curServIdx = service_list->pServItems[0].channelId; //搜索结束默认播放第一个有效台
		__get_new_channel();//默认播放新闻台

#endif//endif #if (IS_CA_EXIST == 0)
		__wrn("service_list->num = %d service_list->curServIdx = %d\n",service_list->servNum,service_list->curServIdx);
#elif((MAPLE_TV_STANDAR == MAPLE_TV_ISDB_T)||(MAPLE_TV_STANDAR == MAPLE_TV_DTMB)\
		|| (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)|| (MAPLE_TV_STANDAR == MAPLE_TV_ATSC))
		esMODS_MIoctrl(maple, MAPLE_CMD_SL_GET, type, (void*)search_service_list);//MAPLE_SERV_GROUP_TYPE_ALL
		__get_valid_channel();
#endif
		
#if (EPDK_DEBUG_LEVEL_GATE == 4)
		{
		    __s32 i;
			for(i=0; i<service_list->servNum; i++)
			{
				__msg("channelId = %d: %s\n",service_list->pServItems[i].channelId,service_list->pServItems[i].servName);
			}

		}
#endif										
	
	

	return service_list->servNum;

}

static __bool search_task_break = EPDK_FALSE; //人为中断搜台线程,add by hot.lee
__s32 g_is_searching = 0;

void maple_break_search_task(void)
{
	esKRNL_SemPost(sl_event_cbp->pOpaque);	
	search_task_break = EPDK_TRUE;	
}



void sl_area_search_task(void* arg)
{

	__u8				err;
	__s32               ret = EPDK_FAIL;
	int i;
	DVBT_AreaTable_t * area_p;

    maple_demod_ss_t    ss;
    
    char    bandw[24];
    char    band[12];
    char    frequ[30];
    char    fre[18];


	search_task_break = EPDK_FALSE;
	
	__msg("pend sem \n");
	
	esKRNL_SemPend(dtv_ctr.sem,0,NULL);
	
	__msg("pend sem end\n");
	
	area_p = &dvb_t_areatp_table[G_DTV_current_area];

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
	if(service_list->pServItems == NULL)
	{
		__wrn("malloc fail.\n");
		goto _sl_search_exit;
	}


	esMODS_MIoctrl(maple, MAPLE_CMD_SL_DELETE, 0, NULL);
    

	for(i=0;i<area_p->TPIndex;i++)
    {
         esKRNL_TimeDly(1);

         if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
         {
			 __msg("sl_area_search_task()... abort...\n");
	         goto _sl_search_exit;
         }

        tps.bandwidth=area_p->pTPRegion_Area[i].BandWidth;
        tps.frequency=area_p->pTPRegion_Area[i].StartFreq;
	
        //如果上次等待超时，则maple任务会被杀掉，maple多发一个finish，callback，信号量增加1，导致下一个频点搜索失败
        esKRNL_SemAccept(sl_event_cbp->pOpaque);
		
        maple_set_cur_search_freq(tps.frequency,i);
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

	    esMODS_MIoctrl(maple, 
				   MAPLE_CMD_SRCH_SERV, 
				  (tps.bandwidth/10)<<22 | tps.frequency, 
				   (void*)search_service_list);

		
	    if(EPDK_TRUE ==  search_task_break)
	    {
			goto _sl_search_exit;
		}

		//* wait a long time for search progress.
		__msg("wait sem.....\n");
	    esKRNL_SemPend(sl_event_cbp->pOpaque, 5000, &err);
		__msg("wait sem ok.....\n");
        

        ret = esMODS_MIoctrl(maple, MAPLE_CMD_SRCH_SERV_STOP, 0, NULL);

	    if(EPDK_TRUE ==  search_task_break)
	    {
			goto  _sl_search_exit;
		}
        
    }

    maple_service_list_get(MAPLE_SERV_GROUP_TYPE_TV);


	if(service_list->servNum > 0)//搜台结束之后做一次保存
	{
        esMODS_MIoctrl(maple, MAPLE_CMD_SORT_SERV_LIST,0, NULL);
        
        maple_service_list_get(MAPLE_SERV_GROUP_TYPE_TV);
        
		maple_cmmb_reg_flush();
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
	
	esKRNL_SemPost(dtv_ctr.sem);

	if(sl_event_cbp->pOpaque)
	{
		esKRNL_SemDel(sl_event_cbp->pOpaque, OS_DEL_ALWAYS, &err);
		sl_event_cbp->pOpaque = NULL;
	}
	
	eDbug("service area search task exit.\n");

	g_is_searching = 0;
	
	sl_task_id = NULL;
	
	{
		__gui_msg_t dmsg;

		dmsg.id         = GUI_MSG_COMMAND;
		dmsg.h_deswin   = g_dtv_mainwin;
		dmsg.dwAddData1 = CMD_TO_DELETE_SEARCH_THREAD;
		dmsg.dwAddData2 = 0;
		dmsg.dwReserved = 0;

		GUI_SendNotifyMessage(&dmsg);
	}


    esKRNL_TDel(EXEC_prioself);
	
} 

__s32 maple_register_search_callback(__pCBK_t cb_search_list_event_callback,__pCBK_t cb_search_list_finish_callback)
{
	__pCBK_t			callback;

	
	callback = esKRNL_GetCallBack(cb_search_list_event_callback);
	esMODS_MIoctrl(maple, MAPLE_CMD_CB_SET_PARAM, MAPLE_CB_SRCH_SL_EVENT, (void*)sl_event_cbp);
	esMODS_MIoctrl(maple, MAPLE_CMD_CB_SET, MAPLE_CB_SRCH_SL_EVENT, (void*)callback);

	callback = esKRNL_GetCallBack(cb_search_list_finish_callback);
	esMODS_MIoctrl(maple, MAPLE_CMD_CB_SET_PARAM, MAPLE_CB_SRCH_SL_FINISH, (void*)sl_event_cbp);
	esMODS_MIoctrl(maple, MAPLE_CMD_CB_SET, MAPLE_CB_SRCH_SL_FINISH, (void*)callback);
	__wrn("maple_register_search_callback exit.\n");
	return EPDK_OK;

}

__s32 maple_thread_delete(__u32 thread_id)
{
	if( thread_id != 0 )
	{
		__s8 ret;
		while( (ret=esKRNL_TDelReq(thread_id)) != OS_TASK_NOT_EXIST )
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

__s32 maple_area_search(void)
{    
    if(sl_task_id != 0)
    {
        g_is_searching = 0;
		return EPDK_OK;
	}
	
	sl_task_id = esKRNL_TCreate(sl_area_search_task, 
								(void*)NULL, 
								0x1000, 
								KRNL_priolevel3);

	if (sl_task_id == 0)
	{
        g_is_searching = 0;
		__wrn("create sl_areasearch_task fail.\n");
		return EPDK_FAIL;
	}
	else
	{
		
		__wrn("create sl_areasearch_task[id = %d] success.\n",sl_task_id);
		return EPDK_OK;
	}

}

__s32 maple_search_stop(void)
{
    
    if(sl_task_id != 0)
    {
        esKRNL_TDelReq(sl_task_id);
        esKRNL_TimeDlyResume(sl_task_id);
        while(sl_task_id && esKRNL_TDelReq(sl_task_id) != OS_TASK_NOT_EXIST)
        {
            esKRNL_TimeDlyResume(sl_task_id);
            esKRNL_TimeDly(10);
        }

        sl_task_id = 0;
    }
    return EPDK_OK;
}



/****************************************************************************************************************************
*save / load program ,查询是否有备份的播放列表,modify by hot.lee
******************************************************************************************************************************/
__s32 maple_program_load(char *database_file_name)
{
	int i,j=0;
	__s32 ret;
	ES_FILE *pf=NULL;
    char str_filename[128]={0};
	char baseinf_filename[64]={0};

    eLIBs_strcpy(str_filename, database_file_name);

	eLIBs_memset(&cmmb_para,0,sizeof(reg_cmmb_para_t));

	pf = eLIBs_fopen(str_filename,"rb");
	if(NULL == pf)
	{	
		//存台文件不存在，默认搜台
		eLIBs_printf("file [%s] not exist , search\n",str_filename);
		
        cmmb_para.lcn_state = 0;
        cmmb_para.index = 0;
        cmmb_para.mode = MAPLE_VID_WINDOW_FULLSCN;
        maple_reset_audio_num();
        
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
		eLIBs_fread(&cmmb_para,1,sizeof(reg_cmmb_para_t),pf);
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

    if(g_programtype==SSET_NC_PROGRAMTYPE_TV)
    {
        esMODS_MIoctrl(maple, MAPLE_CMD_SL_GET, MAPLE_SERV_GROUP_TYPE_TV, (void*)service_list);
    }
    else if(g_programtype==SSET_NC_PROGRAMTYPE_RADIO)
    {
        esMODS_MIoctrl(maple, MAPLE_CMD_SL_GET, MAPLE_SERV_GROUP_TYPE_RADIO, (void*)service_list);
    }   
    else if(g_programtype==SSET_NC_PROGRAMTYPE_ALL)
    {
        esMODS_MIoctrl(maple, MAPLE_CMD_SL_GET, MAPLE_SERV_GROUP_TYPE_ALL, (void*)service_list);
    }		
    if(cmmb_para.index >= service_list->servNum)
    {
        cmmb_para.index = service_list->servNum-1;
    }
    
    if(cmmb_para.index < 0)
    {
        cmmb_para.index = 0;
    }
    
	service_list->curServIdx = service_list->pServItems[cmmb_para.index].channelId;

    
	__msg("service_list->curServIdx = %d name = %s servId=%d...\n",service_list->curServIdx,service_list->pServItems[cmmb_para.index].servName,service_list->pServItems[cmmb_para.index].servId);
    __wrn("app2 :channelId[%x],servId[%x],subfrmId[%x],timeSlot[%x],timeSlotCnt[%x],demodArg[%x]\n",\
	service_list->pServItems[cmmb_para.index].channelId,service_list->pServItems[cmmb_para.index].servId,service_list->pServItems[cmmb_para.index].pmtId,\
	service_list->pServItems[cmmb_para.index].tsId,service_list->pServItems[cmmb_para.index].networkId,service_list->pServItems[cmmb_para.index].demodArg);
    __wrn("ldpcRate[%x],modulateMode[%x],byteInterMode[%x],rsRate[%x],scrambleMode[%x],muxfrmId[%x]\n",\
	service_list->pServItems[cmmb_para.index].ldpcRate,service_list->pServItems[cmmb_para.index].modulateMode,service_list->pServItems[cmmb_para.index].byteInterMode,\
	service_list->pServItems[cmmb_para.index].rsRate,service_list->pServItems[cmmb_para.index].scrambleMode,service_list->pServItems[cmmb_para.index].muxfrmId);
	return EPDK_OK;
}


__s32 maple_program_add_srvlist(void)
{
    __u32 service_num = 0;


    service_num = search_service_list->servNum;
    __wrn("==maple_program_add_srvlist---service_list->servNum=%d\n",search_service_list->servNum);
	
	if(search_service_list->servNum > 0)
	{
		esMODS_MIoctrl(maple, MAPLE_CMD_SL_ADD, 0, (void*)search_service_list);
		search_service_list->servNum=0;
		search_service_list->curServIdx = 0;   
        eLIBs_memset(search_service_list->pServItems,0, sizeof(maple_serv_item_t)*MAPLE_MAX_SERV_NUM);
	}

	return service_num;

}
__s32 maple_program_get_srvlist(__u32 programtype)
{
    
    if(programtype==SSET_NC_PROGRAMTYPE_TV)
    {
        esMODS_MIoctrl(maple, MAPLE_CMD_SL_GET, MAPLE_SERV_GROUP_TYPE_TV, (void*)service_list);
    }
    else if(programtype==SSET_NC_PROGRAMTYPE_RADIO)
    {
        esMODS_MIoctrl(maple, MAPLE_CMD_SL_GET, MAPLE_SERV_GROUP_TYPE_RADIO, (void*)service_list);
    }   
    else if(programtype==SSET_NC_PROGRAMTYPE_ALL)
    {
        esMODS_MIoctrl(maple, MAPLE_CMD_SL_GET, MAPLE_SERV_GROUP_TYPE_ALL, (void*)service_list);
    }
    
    return service_list->servNum;
}


__s32 maple_cmmb_reg_flush(void)
{
	__s32 result,result1,result2;
	ES_FILE* fp = NULL;
	ES_FILE* fp1 = NULL;
	__u32 len;
	char *buf=NULL;	
	char baseinf_filename[64]={0};
	
    spinor_wrsr(0<<2,0<<6);
    
     {
		eLIBs_strcpy(baseinf_filename,DTV_BASEINFO_NAME);
		fp = eLIBs_fopen(baseinf_filename, "wb");
		if(fp == NULL)//写不成功时才格式化Z盘
		{
			eLIBs_format("z:\\","fat",0);
			esKRNL_TimeDly(10);
			fp = eLIBs_fopen(baseinf_filename, "wb");
			if(fp == NULL)
			{
    			__wrn("2open file fail:%s\n", DTV_BASEINFO_NAME);
				if(fp)
				{
					eLIBs_fclose(fp);
				}				
    				return EPDK_FAIL;
			}				
		}			

		eLIBs_fwrite(&cmmb_para,1,sizeof(reg_cmmb_para_t),fp);
		
        
		eLIBs_fclose(fp);

		//其实可以不放在这里，我们可以在需要的时候才保存
		//这样我们只需要保存当前播放的信息到独立分区
		//而不需要和台的信息放在同一盘符
		maple_program_save(DTV_DATABASE_NAME);

        {
            ES_FILE* hFile;
            
            //here  actury  flush user disk data to NOR flash
            hFile = eLIBs_fopen("b:\\STORAGE\\CMMB", "rb");
            if (hFile == NULL)
            {
                __wrn("open NOR file failed\n");
                return EPDK_FAIL;
            }
            __msg("flush dtv file ....\n");
            eLIBs_fioctrl(hFile, DEV_IOC_USR_FLUSH_CACHE, 0, 0);
            
            __msg("flush dtv end ....\n");
            eLIBs_fclose(hFile);
        }
    } 
	spinor_wrsr(0<<2,1<<6);

	return EPDK_OK;
}



//文件名自定义，中间件会去创建
__s32 maple_program_save(char *database_file_name)
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
			ES_FILE *pf=NULL;

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
*Name        : maple_request_video_layer
*Prototype   : __s32  maple_request_video_layer( const RECT *rect_p, __s32 pipe, __s32 prio )
*Arguments   : rect_p    input. the coordinate and size of the video layer.
*              pipe      input. layer pipe.
*              prio      input. layer priority.
*Return      : the video layer handle. if return NULL, means failing.
*Description : request the video layer.
*Other       :
***************************************************************************************************/
__hdle  maple_request_video_layer( const RECT *rect_p, __s32 pipe, __s32 prio )
{
	__disp_layer_para_t     image_layer_para;
	FB                      image_fb_para;
	RECT                    image_win;
    __u32 arg[3];
	__s32 ret;
	
	if( rect_p == NULL )
		return NULL;
	
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


__s32 maple_get_play_status(void)
{

    if(!maple)
    {
		return -1;
	}
	return esMODS_MIoctrl(maple, MAPLE_CMD_GET_STATUS, 0, NULL);

}

__s32 maple_play_Audstream(__u32 audioIdx)
{
    __s32 ret = 0;
    ret = maple_get_cur_channel_austream_cnt();
    if(ret <= 0)
        return EPDK_FAIL;

    if(esMODS_MIoctrl(maple, MAPLE_CMD_SEL_AUDSTREAM,audioIdx,NULL) != EPDK_OK)
        return EPDK_FAIL;
 
    else
        return EPDK_OK;        
}

__s32 maple_play_program(void)
{
    
    RECT                video_window;
	__u32 arg[3];
	__s32 ret;
    __u8 audio_index;

    if(service_list->servNum<=0)
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
    video_layer_hdl = maple_request_video_layer(&video_window,0,0xff);

	maple_set_screensize(maple_get_video_window_ratio_mode());
    audio_index = maple_get_audio_num(service_list->curServIdx);
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

        cur_playing_index = service_list->curServIdx;
		
    }

	maple_epg_search_stop();
	esKRNL_TimeDly(1);
	maple_epg_search();
	maple_time_monitor_stop();
	esKRNL_TimeDly(1);
	maple_time_monitor_start();
	

    return EPDK_OK;
}

__s32 maple_stop_program(__u32 video_layer_state)
{
    __u32 arg[3];
	__s32 ret;
	__u8  i = 10;


    cur_playing_index = -1;
    
    if(service_list->servNum <= 0) 
    {
        __wrn("service_list->servNum = %d \n",service_list->servNum);
		return EPDK_FAIL;
	}
        __here__;

	if(maple_get_play_status() == MAPLE_STAT_STOP)
	{
			return EPDK_OK;
	}
	
        __here__;
	
	_clearprogramname();
            __here__;

   maple_epg_search_stop();
   maple_time_monitor_stop();
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
                  __here__;

    		ret = eLIBs_fioctrl( dis_file, DISP_CMD_LAYER_RELEASE, 0, (void*)arg );        
    	        __here__;
	
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
    
	while(maple_get_play_status() != MAPLE_STAT_STOP)
	{
		esKRNL_TimeDly(1);
	};	

   
   return EPDK_OK;
}


void maple_epg_reset_schdl_list_day(__u32 serviceid)
{
    int i;
    
     eLIBs_memset(schdl_list_day,0,MAX_EPG_DAY*sizeof(maple_schdl_list_day));

	for(i=0;i<1;i++)//只猎取当天的节目列表
    {
        schdl_list_day[i].eventNum = 0;
        //schdl_list_day[i].MJD=tv_time->MJD+i;//有问题
        schdl_list_day[i].MJD= i;
        schdl_list_day[i].serviceid=service_list->curServIdx;
		__msg("schdl_list_day[%d].MJD = %d \n",i,schdl_list_day[i].MJD);
    }
}

__s32 maple_epg_add_event_to_schdl(maple_schdl_event_t * event)
{
    int i;
    
	for(i=0;i<1;i++)//只获取当天的就OK		
     {
        {
            
            eLIBs_memcpy(schdl_list_day[i].pSchdlEvents+schdl_list_day[i].eventNum,event,sizeof(maple_schdl_event_t));
            schdl_list_day[i].eventNum += 1;
            
			schdl_list_day[i].MJD = event->startTime.MJD;
        }
            
     }; 

    return 0;
}
__s32 maple_register_epg_callback(__pCBK_t cb_search_schedule_event_callback,__pCBK_t cb_search_schedule_finish_callback)
{
	__pCBK_t			callback;

    callback = esKRNL_GetCallBack(cb_search_schedule_event_callback);
    esMODS_MIoctrl(maple, MAPLE_CMD_CB_SET_PARAM, MAPLE_CB_SRCH_SCHDL_EVENT, (void*)NULL);//schdl_event_cbp
    esMODS_MIoctrl(maple, MAPLE_CMD_CB_SET, MAPLE_CB_SRCH_SCHDL_EVENT, (void*)callback);
    
    callback = esKRNL_GetCallBack(cb_search_schedule_finish_callback);
    esMODS_MIoctrl(maple, MAPLE_CMD_CB_SET_PARAM, MAPLE_CB_SRCH_SCHDL_FINISH, (void*)NULL);//schdl_event_cbp
    esMODS_MIoctrl(maple, MAPLE_CMD_CB_SET, MAPLE_CB_SRCH_SCHDL_FINISH, (void*)callback);
    
	__wrn("maple_register_epg_callback exit.\n");
	return EPDK_OK;

}

//maple_epg_search()之后并不会马上得到epg数据
//所以马上进入schedule界面有可能数据还是空的
__s32 maple_epg_getdata(void)
{
    int i=0;
    maple_epg_reset_schdl_list_day(0);
    esMODS_MIoctrl(maple, MAPLE_CMD_GET_SCHEDULE, service_list->curServIdx, schdl_list);    
    for(i=0;i<schdl_list->eventNum;i++)
    {
        maple_time_deal_thai(&(schdl_list->pSchdlEvents[i].startTime));
        maple_time_deal_thai(&(schdl_list->pSchdlEvents[i].endTime));
        
        maple_epg_add_event_to_schdl(&(schdl_list->pSchdlEvents[i]));
    }

    return EPDK_OK;
}


//获取当前正在播放的节目是一天中的第几个节目，add by hot.lee
__s32 maple_get_cur_schedule_index(__s32 day_index)
{
    __s32 i;
	char tmp[128];

    for(i=0; i<schdl_list_day[day_index].eventNum; i++)
    {
        //endTime 都 是0，所以  //取起始时间相等就OK
		            eLIBs_sprintf(tmp,"  %d:%d:%d - %d:%d:%d : ",
					schdl_list_day[day_index].pSchdlEvents[i].startTime.hour,
					schdl_list_day[day_index].pSchdlEvents[i].startTime.min,
					schdl_list_day[day_index].pSchdlEvents[i].startTime.sec,
					schdl_list_day[day_index].pSchdlEvents[i].endTime.hour,
					schdl_list_day[day_index].pSchdlEvents[i].endTime.min,
					schdl_list_day[day_index].pSchdlEvents[i].endTime.sec);
		__msg("tmp = %s  sepg_string_c = %s\n",tmp,sepg_string_c);
		if(eLIBs_strncmp(tmp,sepg_string_c,10) == 0)//取起始时间相等就OK
		{
			__msg("i = %d\n",i);
			return i;
		}
			
	}

	return 0;
}


//获取某一天的event num
__s32 maple_epg_get_eventnum(__s32 day_index)
{
	__msg("schdl_list_day[%d].eventNum = %d\n",day_index,schdl_list_day[day_index].eventNum);
	return schdl_list_day[day_index].eventNum;
}

//获取某一天指定event index的schdl events
maple_schdl_event_t *maple_epg_get_schdl_event(__s32 day_index,__s32 event_index)
{
    if(maple_epg_get_eventnum(day_index) <= 0)
        return NULL;

    return &schdl_list_day[day_index].pSchdlEvents[event_index];
}


__s32 maple_epg_search(void)
{
    __s32 ret;
    
    __msg(".maple_epg_search\n");
    maple_epg_reset_schdl_list_day(0);
    ret = esMODS_MIoctrl(maple, MAPLE_CMD_EPG_MONITOR_START, 0, NULL);
	__msg("ret = %d \n",ret);
    return ret;
}

__s32 maple_epg_search_stop(void)
{
    esMODS_MIoctrl(maple, MAPLE_CMD_EPG_MONITOR_STOP, 0, NULL);
    return EPDK_OK;
}



void maple_time_monitor_start(void) 
{
    esMODS_MIoctrl(maple, MAPLE_CMD_TIME_MONITOR_START, 0, NULL);
}

void maple_time_monitor_stop(void)
{
    esMODS_MIoctrl(maple, MAPLE_CMD_TIME_MONITOR_STOP, 0, NULL);
}

__u32 maple_set_system_time(__date_t* date, __time_t* time)
{

    ES_FILE *pfile;
    __s32 result; 
    pfile = eLIBs_fopen("b:\\HWSC\\RTC", "r+");
    if(NULL == pfile)
    {
        eDbug("open RTC fail...\n");
        return EPDK_FAIL;
    }
    result = eLIBs_fioctrl(pfile,RTC_CMD_SET_DATE,0x00,date);
    if(result = EPDK_FAIL)
        eDbug("set system date failed \n");

    result = eLIBs_fioctrl(pfile,RTC_CMD_SET_TIME,0x00,time);
    if(result = EPDK_FAIL)
        eDbug("set system time failed \n");
    
    eLIBs_fclose(pfile);

}

__s32 maple_time_monitor_get(maple_time_t *tv_time)
{
    __time_t time;
    __date_t date;
    
    esMODS_MIoctrl(maple, MAPLE_CMD_TIME_GET, 0, tv_time);
    maple_time_deal_thai(tv_time);
    
    date.year = tv_time->year;
    date.month = tv_time->month;
    date.day = tv_time->day;

    time.hour = tv_time->hour;
    time.minute = tv_time->min;
    time.second = tv_time->sec;

    eDbug("date.year is %d \n",date.year);
    eDbug("date.month is %d \n",date.month);
    eDbug("date.day is %d \n",date.day);

    eDbug("time.hour is %d \n",time.hour);
    eDbug("time.minute is %d \n",time.minute);
    eDbug("time.second is %d \n",time.second);
    
    maple_set_system_time(&date,&time);
}

int judge(int year,int month)
{
	if(month==1||month==3||month==5||month==7||month==8||month==10||month==12)
		return 31;
	else if(month==2)
	{
		if(year%4!=0||year%100==0&&year%400!=0)
			return 28;
		else return 31;
	}
	else return 30;
}

static __bool IsLeapYear(__u16 Year) 
{ 
    if((Year&3)==0) 
    { 
        return ((Year%400==0) || (Year%100!=0)); 
    } 
     return 0; 
} 

__s32 maple_time_deal(maple_time_t *tv_time)
{
    eDbug("tv_time->hour is %d \n",tv_time->hour);
    eDbug("tv_time->min is %d \n",tv_time->min);

    if(tv_time->stTotLto.u8NumofRegion == 0)
    {
        eDbug("have no sttotlto \n");
        return EPDK_OK;
    }
    {
        __u8 hour_offset,min_offset;
        __u8 u16LTO1,u16LTO2, u16LTO3, u16LTO4;
        u16LTO1 = tv_time->stTotLto.aLTOInfo[0].u16LTO >>12;
        u16LTO2 = (tv_time->stTotLto.aLTOInfo[0].u16LTO & 0x0f00)>>8;
        u16LTO3 = (tv_time->stTotLto.aLTOInfo[0].u16LTO & 0x00f0)>>4;
        u16LTO4 = (tv_time->stTotLto.aLTOInfo[0].u16LTO & 0x000f);
        eDbug("u16LTO1 is %d \n",u16LTO1);
        eDbug("u16LTO2 is %d \n",u16LTO2);
        eDbug("u16LTO3 is %d \n",u16LTO3);
        eDbug("u16LTO4 is %d \n",u16LTO4);
        
        hour_offset = u16LTO1*10 + u16LTO2;
        min_offset  = u16LTO3*10 + u16LTO4;

        if(tv_time->stTotLto.aLTOInfo[0].u8LTOPolarity)
        {
            if(tv_time->min + min_offset < 60)
                tv_time->min = tv_time->min + min_offset;
            else
            {
                tv_time->min = tv_time->min +min_offset - 60;
                tv_time->hour ++;
            }
            
            if(tv_time->hour + hour_offset < 24)
                tv_time->hour = tv_time->hour + hour_offset;

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
                    if(tv_time->day == 29 || (tv_time->day == 28 && IsLeapYear(tv_time->year)==0))
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
        else
        {
            if(tv_time->min  > min_offset)
                tv_time->min = tv_time->min - min_offset;
            else
            {
                tv_time->min = tv_time->min + 60 - min_offset;
                tv_time->hour --;
            }

            if(tv_time->hour >= hour_offset) 
                tv_time->hour -= hour_offset;

            else
            {
                tv_time->hour = tv_time->hour + 24 - hour_offset;

                if(tv_time->month == 2 || tv_time->month == 4|| tv_time->month == 6
                || tv_time->month == 8|| tv_time->month == 9|| tv_time->month == 11)
                {
                    if(tv_time->day > 1) tv_time->day--;
                    else
                    {
                        tv_time->day = 31;
                        tv_time->day--;
                    }
                }
                else if(tv_time->month == 5 || tv_time->month == 7
                || tv_time->month == 10|| tv_time->month == 12)
                {
                    if(tv_time->day > 1) tv_time->day--;
                    else
                    {
                        tv_time->day = 30;
                        tv_time->day--;
                    }
                }
                else if(tv_time->month == 3)
                {
                    if(tv_time->day == 1 && IsLeapYear(tv_time->year)==0)
                    {
                        tv_time->day = 28;
                        tv_time->month--;
                    }
                    else
                        tv_time->month--;
                }
                else if(tv_time->month == 1)
                {
                    if(tv_time->day > 1) tv_time->day--;
                    else
                    {
                        tv_time->day = 31;
                        tv_time->month = 12;
                        tv_time->year --;
                    }
                }
            }
            
            
        }

    }
    
}

__s32 maple_time_deal_thai(maple_time_t *tv_time)
{
    __u8 hour_offset,min_offset;
    
    hour_offset = 7;
    min_offset  = 0;

    if(tv_time->hour + hour_offset < 24)
        tv_time->hour = tv_time->hour + hour_offset;

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
            if(tv_time->day == 29 || (tv_time->day == 28 && IsLeapYear(tv_time->year)==0))
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

//获取当前台数
__s32 maple_get_cur_service_list_num(void)
{
	return service_list->servNum;
}


//获取当前播放节目的逻辑频道号
__s32 maple_get_cur_program_index(void)
{
	__s32 i;

	for(i=0; i<service_list->servNum; i++)
	{
        if(service_list->curServIdx == service_list->pServItems[i].channelId)
		{
			return i;
		}

	}

	return 0;
}



//根据0 1 2 3 等设置curServIdx
//根据逻辑频道号去播放节目，更符合使用习惯,add by hot.lee
__s32  maple_set_cur_channel(__s32 index)
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
        save_flag = EPDK_TRUE;
	    service_list->curServIdx = service_list->pServItems[index].channelId;
        cmmb_para.index = index;        
    }
    return EPDK_OK;

}

__s32 maple_get_cur_channel_index_by_lcn(__s32 nRemoteControlKeyId)
{
    int i;
    for(i =0;i < service_list->servNum; i++)
    {
        if(nRemoteControlKeyId == service_list->pServItems[i].nRemoteControlKeyId)
            return i;
    }

    return -1;
}

__s32 maple_set_cur_channel_index_by_lcn(__s32 nRemoteControlKeyId)
{
    int ret;
    ret = maple_get_cur_channel_index_by_lcn(nRemoteControlKeyId);
    if(ret != -1)
    {
    	service_list->curServIdx = ret;
        return EPDK_OK;
    }

    else
    {
        return EPDK_FAIL;
    }
}

__s32  maple_get_cur_channel_lcn_id(__s32 index)
{
	if(index > service_list->servNum)
		return EPDK_FAIL;

	return service_list->pServItems[index].nRemoteControlKeyId;
    
}


__bool maple_get_lcn_state(void)
{
    return cmmb_para.lcn_state;
}

void maple_set_lcn_state(__bool state)
{
    if( cmmb_para.lcn_state != state)
    {
        save_flag = EPDK_TRUE;
        cmmb_para.lcn_state = state;
    }
}



__bool maple_get_antenna_state(void)
{
    return dtv_ctr.antenna_state;
}


void maple_set_antenna_state(__bool state)
{
    dtv_ctr.antenna_state = state;

    if(dtv_ctr.antenna_state)//state = 1:外部天线
    {
        pull_down_gpio(ANTENN_POWER_PORT ,ANTENN_POWER_PORTNUM);
        pull_down_gpio(RF_SW_PORT,RF_SW_PORTNUM);
	}
	
    else                      //state = 0:拉杆天线
    {
        pull_up_gpio(ANTENN_POWER_PORT,ANTENN_POWER_PORTNUM);
        pull_up_gpio(RF_SW_PORT,RF_SW_PORTNUM);
    }
}





void maple_reset_audio_num(void)
{
    __u8 i;
    for(i = 0; i < MAX_CHANNEL_NUM; i++)
    {
        cmmb_para.audio_num_select[i] = 0;
    }
}

__u8 maple_get_audio_num(__s32 index)
{
	if(index > service_list->servNum-1)
		return EPDK_FAIL;
    
    return cmmb_para.audio_num_select[index];
}

__u8 maple_set_audio_num(__u8 num)
{
    __s32 index =0;
    index = service_list->curServIdx;
	if(index > service_list->servNum-1)
		return EPDK_FAIL;

    if(cmmb_para.audio_num_select[index] != num)
    {
        save_flag = EPDK_TRUE;
        cmmb_para.audio_num_select[index] = num;
    }
}

__s32 maple_get_cur_channel_austream_cnt(void)
{
    __s32 index =0;
    index = service_list->curServIdx;
    return service_list->pServItems[index].adStreamNum;
}

__s32 maple_get_current_austream(void)
{
    int ret;
    ret = esMODS_MIoctrl(maple, MAPLE_CMD_GET_AUDSTREAM,0,NULL);
    return ret;
}

maple_ad_info_t *maple_get_cur_channel_austream_info(void)
{
    __s32 index =0;
    index = service_list->curServIdx;
    if(service_list->pServItems[index].adStreamNum > 0)
        return service_list->pServItems[index].adInfo;
}


// 获取频道名称
//根据逻辑频道号，获取频道名称,add by hot.lee
char  *maple_get_name_of_channel(__s32 index)
{
	if(index >= service_list->servNum  || index < 0)
	{
	    __wrn("index = %d service_list->servNum=%d\n",index,service_list->servNum);
		return NULL;
	}
    
	return service_list->pServItems[index].servName;
}

char *maple_get_channel_of_curprogramname(void)
{
	return sepg_string_c;
}

char *maple_get_channel_of_nextprogramname(void)
{
	return sepg_string_n;
}


static void _clearprogramname(void)
{
	eLIBs_memset(sepg_string_c,0,MAX_SEPG_STRING);
	eLIBs_memset(sepg_string_n,0,MAX_SEPG_STRING);
}



//* task for serching simple epg
static void sepg_search_task(void* arg)
{
    __u8 err;
    
    
    sepg_finish_cbp->pOpaque = esKRNL_SemCreate(0);
    if (!sepg_finish_cbp->pOpaque)
    {
        __wrn("sepg search semaphore create fail.\n");
        goto _sepg_search_exit;
    }
    
    if(service_list->servNum==0)
    {
        __wrn("no program.\n");
        goto _sepg_search_exit;
    }
    
    esMODS_MIoctrl(maple, MAPLE_CMD_SRCH_SEPG, service_list->curServIdx, (void*)sepg);
                   
    //* wait a long time for search progress.
    esKRNL_SemPend(sepg_finish_cbp->pOpaque, 60000, &err);
    
    if(err != 0)
    {
        __wrn("sepg search fail.\n");
    }
    else
    {
		__wrn("sepg search finish.\n");

	}
        
                   
_sepg_search_exit:
    
    esMODS_MIoctrl(maple, MAPLE_CMD_SRCH_SEPG_STOP, 0, NULL);
    
    if(sepg_finish_cbp->pOpaque)
    {
        esKRNL_SemDel(sepg_finish_cbp->pOpaque, OS_DEL_ALWAYS, &err);
        sepg_finish_cbp->pOpaque = NULL;
    }
    sepg_task_id = 0;
    __wrn("sepg search task exit.\n");
    esKRNL_TDel(EXEC_prioself);
}



__s32 maple_register_sepg_callback(__pCBK_t search_sepg_finish_callback)
{
	__pCBK_t			callback;

    callback = esKRNL_GetCallBack(search_sepg_finish_callback);
    esMODS_MIoctrl(maple, MAPLE_CMD_CB_SET_PARAM, MAPLE_CB_SRCH_SEPG_FINISH, (void*)sepg_finish_cbp);
    esMODS_MIoctrl(maple, MAPLE_CMD_CB_SET, MAPLE_CB_SRCH_SEPG_FINISH, (void*)callback);
    

	__wrn("maple_register_epg_callback exit.\n");
	return EPDK_OK;

}

__s32 maple_sepg_clean(void)
{
    sepg_string_c[0] = NULL;
	sepg_string_n[0] = NULL;
}

__s32 maple_sepg_search(void)
{
    int hassepg=0;
    int i=0;

    if(sepg->hasCurEventInfo)
    {
         sepg->curEventstartTime.hour = 0; 
         sepg->curEventstartTime.min= 0; 
         sepg->curEventstartTime.sec= 0;
         sepg->curEventEndTime.hour= 0; 
         sepg->curEventEndTime.min= 0; 
         sepg->curEventEndTime.sec= 0;

        eLIBs_memset(sepg->curEventName,0,64);
    
    }   
    else
    {
        __wrn(" ======== \n");
    }
       
    if(sepg->hasNextEventInfo)
    {          
         sepg->nextEventstartTime.hour = 0; 
         sepg->nextEventstartTime.min= 0; 
         sepg->nextEventstartTime.sec= 0; 
         sepg->nextEventEndTime.hour= 0; 
         sepg->nextEventEndTime.min= 0; 
         sepg->nextEventEndTime.sec= 0;
        eLIBs_memset(sepg->nextEventName,0,64);
        
    }
    else
    {
        __wrn(" ======== \n");
    }

    
     eLIBs_memset(sepg_string_c,0,MAX_SEPG_STRING);
     eLIBs_memset(sepg_string_n,0,MAX_SEPG_STRING);


    for(i=0;i<3;i++)
    {
        esKRNL_TimeDly(10);
        esMODS_MIoctrl(maple, MAPLE_CMD_GET_SEPG, service_list->curServIdx, (void*)sepg);
        if(sepg->hasCurEventInfo || sepg->hasNextEventInfo ) 
			break;
    }
    
    if(sepg->hasCurEventInfo)
    {
        maple_time_deal_thai(&sepg->curEventstartTime);
        maple_time_deal_thai(&sepg->curEventEndTime);
        
        eLIBs_sprintf(sepg_string_c,"  %d:%d:%d - %d:%d:%d : ",
                     sepg->curEventstartTime.hour, 
                     sepg->curEventstartTime.min, 
                     sepg->curEventstartTime.sec,
                     sepg->curEventEndTime.hour, 
                     sepg->curEventEndTime.min, 
                     sepg->curEventEndTime.sec);
    
        strcat(sepg_string_c,(char *)sepg->curEventName);   
        hassepg=1;
    
    }   
    else
    {
        __wrn(" ======== \n");
    }
	__msg("sepg_string_c = %s\n",sepg_string_c);
       
    if(sepg->hasNextEventInfo)
    {          
        maple_time_deal_thai(&sepg->nextEventstartTime);
        maple_time_deal_thai(&sepg->nextEventEndTime);
        
        eLIBs_sprintf(sepg_string_n,"  %d:%d:%d - %d:%d:%d :",
                     sepg->nextEventstartTime.hour, 
                     sepg->nextEventstartTime.min, 
                     sepg->nextEventstartTime.sec, 
                     sepg->nextEventEndTime.hour, 
                     sepg->nextEventEndTime.min, 
                     sepg->nextEventEndTime.sec);
        strcat(sepg_string_n,sepg->nextEventName);
        hassepg=1;
    }
    else
    {
        __wrn(" ======== \n");
    }
	__msg("sepg_string_c = %s\n",sepg_string_n);


    return hassepg;
}


__s32 maple_get_current_av_info(void)
{
    __s32 ret=0;
	if(maple)
	{
    	ret = esMODS_MIoctrl(maple, MAPLE_CMD_GET_SERVICE_INFO, 0, NULL);
	}
	__msg("ret = %d \n",ret);
    return ret;
}


//对于CMMB此函数只能在信号检测定时器里调用
//其它要获取信号状态的地方，请用全局变量获取
//因为MAPLE_CMD_GET_SS每次都会从通道里获取数据
//需要防止线程访问冲突造成的死锁
__s32 maple_get_ss(void)
{
	maple_demod_ss_t    ss;
	
	if(esMODS_MIoctrl(maple, MAPLE_CMD_GET_SS, 0, (void*)&ss)!= 0)
	{
			ss.strength = 0;
			ss.quality = 0;
	}
	else
	{
		__wrn("GET:S: %d, Q: %d\n",ss.strength,ss.quality);
	}

	g_strength = (__s8)ss.strength;
	g_quality = (__s8)ss.quality;
		
	return 0;
}

#if(EN_DTV_RECORD ==1)

#define DTV_RECORD_ROOT_PATH				"\\DTVRECORD\\"
#define DTVRECORD_FILTER					".ts"
char	curSearchPath[50];
static __u8 save_root_type = 0;

static __u32 root_check_disk(void)
{
	__s32 ret;
	__u32 root_type = RAT_UNKNOWN;
	char diskname[RAT_MAX_PARTITION][4] = {0x0};

	__msg("------root_check_disk-----\n");
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
	__msg("get NULL fs part\n");

	return root_type;   
}


__s32 maple_check_device(void)
{
	__u32 root_type = 0, temp_root=0;
	__u8 sd_root =0, usb_root = 0, device_cnt = 0;//device_cnt

	root_type = root_check_disk();	
	
	sd_root = (root_type >> 8)&0x00ff;
	usb_root = (root_type)&0x00ff; 

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

static void maple_record_GetSearchPath(__u32 curRootType, char *curSearchPath)
{
	__s32 ret = 0;
	char root_type[50];
	char disk_name[RAT_MAX_PARTITION][4];
	ES_DIR		*tmp_dir;
    
    __here__;
    
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

}


static __s32 maple_record_set_path(char  *recordFilePath)
{
	__date_t	date;
	char 		dataStr[16]={0}, cntStr[10]={0};
	__u32		fileNum = 0;
    __s32 root_type = 0;
    char channel_name[256]={0}; 
	char *tmp = channel_name;
    maple_time_t *tv_time;
    
    root_type = maple_check_device();
    if(root_type == 0)
        return -1;

    maple_record_GetSearchPath(root_type,recordFilePath);
    eDbug("recordFilePath is %s \n",recordFilePath);

    
    tmp = maple_get_name_of_channel(maple_get_cur_program_index());
    eLIBs_sprintf(channel_name,"%s",tmp);
    eDbug("channel_name is %s \n",channel_name);

	SLIB_strcat(recordFilePath, channel_name);
	SLIB_strcat(recordFilePath, "_");

	// eg. F:\RECORD\REC200101010000.mp3
	fileNum = record_file_num(curSearchDisk, DTVRECORD_FILTER);	

    eDbug("fileNum is %d \n",fileNum);
	cntStr[0] = ((fileNum/1000%10)+'0');
	cntStr[1] = ((fileNum/100%10)+'0');
	cntStr[2] = ((fileNum/10%10)+'0');
	cntStr[3] = ((fileNum/1%10)+'0');
	cntStr[4] = '\0';

    maple_time_monitor_get(tv_time);
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

	eDbug("\n\n\n\n    record file: %s \n\n\n\n", recordFilePath);

}


__s32 maple_set_record_disk_full_cb(__pCBK_t full_cb)
{
	__pCBK_t			callback;
	
    callback = esKRNL_GetCallBack(full_cb);

	return esMODS_MIoctrl( maple, MAPLE_CMD_SET_RECORD_CB, 0, (void*)callback );

}



__s32 maple_cmd_get_record_staue(void)
{
   return esMODS_MIoctrl(maple, MAPLE_CMD_GET_RECORD_STATUS, 0, NULL);
}

__bool maple_control_TS_record_start(void)
{
    __s32 nProgId;  //正在播放的service的progId号,和MAPLE_CMD_PLAY的channelid相同的意思
    __s32   tmpret;
    CedarHerbMediaFileInf   RecordFileInfo;
    char record_name[128];
    __s32 ret;
    if(maple == NULL)
    {
			return FALSE;
    }
	
		
    nProgId = maple_get_cur_program_index();

    __wrn("entry maple_control_TS_record_start****\n");
	
    eLIBs_memset(&RecordFileInfo, 0, sizeof(CedarHerbMediaFileInf));
    eLIBs_memset(record_name, 0, sizeof(record_name));
    ret = maple_record_set_path(record_name);
    if(ret == -1)
        return EPDK_FAIL;
    
    RecordFileInfo.uFileFmt = CEDAR_MEDIA_FILE_FMT_TS;
    eLIBs_strcpy(RecordFileInfo.file_path, record_name);
    
    //启动录制,启动录制前必须先play.否则会出错.
    esMODS_MIoctrl(maple, MAPLE_CMD_SET_RECORD_MEDIAFILE, 0, &RecordFileInfo);
    tmpret = esMODS_MIoctrl(maple, MAPLE_CMD_RECORD, nProgId, NULL);
    if(tmpret != EPDK_OK)
    {
        __wrn("why record progid[%d] fail!\n", nProgId);
    }

    return TRUE;
}

__bool maple_control_TS_record_stop(void)
{
    __s32   tmpret;

   __wrn("entry maple_control_TS_record_stop****\n");
   
    //停止录制
    tmpret = esMODS_MIoctrl(maple, MAPLE_CMD_STOP_RECORD, 0, NULL);
    if(tmpret != EPDK_OK)
    {
        __wrn("why stop record fail!\n");
    }
	
	return TRUE;
}

#endif

__u8  maple_control_get_lock_status( void )
{
    __u8 ret;

    ret = esMODS_MIoctrl(maple, MAPLE_CMD_GET_LOCK_STATUS, 0, 0 );
    __wrn(" MAPLE_CMD_GET_LOCK_STATUS ======= %d \n", ret );

    /*after reset to factory setting, clear service , no channel ,
    but tune freq and locked, so if channle num <= 0, then return no locked*/
    
    if(maple_get_cur_service_list_num() <= 0)
    {
        __wrn("*******service num <= 0***********\n");
        ret = FALSE;
    }

    return ret;
}

//检测解码状态，add by hot.lee
//只用在定时器检测线程中
maple_ve_status_e *maple_get_decode_status(void)
{
	__s32 result;	
	maple_ve_status_e *maple_ve_status;
	
	 result = esMODS_MIoctrl(maple,MAPLE_CMD_GET_VE_STATUS, 0, NULL);
	 maple_ve_status = (maple_ve_status_e*)result;	 

	 return maple_ve_status;
}

__s32 maple_set_tv_mode(__u8 flag)
{
	return esMODS_MIoctrl(maple,MAPLE_CMD_SET_TV_MODE, flag, NULL);	
}


void maple_set_video_window_ratio_mode(maple_vd_win_ratio_mode_e radio_mode)
{
    if( cmmb_para.mode != radio_mode)
    {
        save_flag = EPDK_TRUE;
        cmmb_para.mode = radio_mode;
    }
}

__s32  maple_get_video_window_ratio_mode(void)
{
	return cmmb_para.mode;
}

__s32 maple_set_screensize(maple_vd_win_ratio_mode_e usr_mode)
{
    return esMODS_MIoctrl(maple, MAPLE_CMD_SET_VID_SHOW_MODE, usr_mode, NULL);
}

__s32 maple_set_init_screensize(void)
{ 
    return esMODS_MIoctrl(maple, MAPLE_CMD_SET_VID_SHOW_MODE, maple_get_video_window_ratio_mode(), NULL);
}


__s32 maple_set_area(__u32 area)
{
    
    __hdle   h_node = NULL;
    char     key_value[ ELIBS_KEY_VALUE_LEN+1];
    
    G_DTV_current_area = area;
    h_node = eLIBs_RegOpenNode("\\dtv\\dtv",ELIBS_REG_READWRITE);
    if(h_node == NULL)
    {
        __wrn("reg node cannot be opened ! \n");
        return EPDK_FAIL;
    }
    key_value[0] = G_DTV_current_area/1000 + 0x30;
    key_value[1] = (G_DTV_current_area%1000)/100 + 0x30;
    key_value[2] = (G_DTV_current_area%100)/10 + 0x30;
    key_value[3] = (G_DTV_current_area%10) +0x30;
    key_value[4] = '\0';

    eLIBs_RegSetKeyValue(h_node,"config","area",key_value);

    eLIBs_RegCloseNode(h_node);
    return G_DTV_current_area;
    
}

__s32 maple_get_area(void)
{
    __hdle   h_node = NULL;
    char     key_value[ ELIBS_KEY_VALUE_LEN+1];

    h_node = eLIBs_RegOpenNode("\\dtv\\dtv",ELIBS_REG_READWRITE);
    if(h_node == NULL)
    {
        __wrn("reg node cannot be opened! \n");
        return EPDK_FAIL;
    }
    if(eLIBs_RegGetKeyValue(h_node,"config","area",key_value)== EPDK_FAIL)
    {
        eLIBs_RegCloseNode(h_node);
        return EPDK_FAIL;
    }
    G_DTV_current_area = (key_value[0]-0x30)*1000+(key_value[1]-0x30)*100+(key_value[2]-0x30)*10+(key_value[3]-0x30);
    eLIBs_RegCloseNode(h_node);
    
    return G_DTV_current_area;

}


__s32 maple_clear_database(char *database_file_name)
{
    esMODS_MIoctrl(maple, MAPLE_CMD_SL_DELETE, 0, NULL);
    maple_program_save(database_file_name);
    return EPDK_OK;
}


__u32 ymd2mjd(__u32 year, __u32 month, __u32 date)
{
    __u32 mjd;
    __u32 L;
    __u32 tmp;
    
    if(year < 1900)
        return 0;
    
    year -= 1900;
    
    if(month == 1 || month == 2)
        L = 1;
    else
        L = 0;
    
    mjd = 14956 + date;
    
    tmp = (year - L)*36525;
    tmp /= 100;
    
    mjd += tmp;
    
    tmp = (month + 1 + L*12) * 306001;
    tmp /= 10000;
    
    mjd += tmp;
    
    return mjd;
    
}

/**********************************************************************************
**********************************************************************************/

void mjd2ymd(__u32 MJD, __u32* pYear, __u32* pMonth, __u32* pDate)
{
    __u32 yearVal;
    __u32 monthVal;
    __u32 dayVal;
    __u32 k;
    
	yearVal  = (__u16) ((MJD*100-1507820)/36525);
	monthVal = (__u16) ((MJD*10000-149561000-(yearVal*3652500))/306001);
	dayVal   = (__u16) (MJD-14956-(yearVal*3652500)/10000-(monthVal*306001)/10000);

	k = ((monthVal==14)||(monthVal==15))?1:0;
	    
	(*pYear)   = yearVal+k+1900;
	(*pMonth)  = (__u32) (monthVal-1-k*12);
	(*pDate)   = (__u32) dayVal;

    
}

/********************************************************************************
********************************************************************************/
void getweekday(__u32 year, __u32 month, __u32 day, __u32* pWeekDay)
{
   __u32 century;
    __s32 w;
   
   century = 0;
   w       = 0;
   
   if(month < 3)
   {
        month += 12;
        year  -= 1;
   }

   century =  year/100;
   year    -= century*100;
   
   w = year + (year>>2) + (century>>2) - (century<<1) + (26*(month+1)/10) + day - 1;
   w %= 7;
    if(w < 0)
        w += 7;
   *pWeekDay = w;

}

/***********************************************************************************
*************************************************************************************/
#if((MAPLE_TV_STANDAR == MAPLE_TV_ISDB_T)||(MAPLE_TV_STANDAR == MAPLE_TV_DTMB)\
		|| (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)|| (MAPLE_TV_STANDAR == MAPLE_TV_ATSC))
maple_cas_param_t *maple_ca_get_sn(void)
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
#endif

#if (MAPLE_TV_STANDAR == MAPLE_TV_CMMB)  
__s32 maple_ca_get_sn(void)
{
    return esMODS_MIoctrl(maple, MAPLE_CMD_CA_GET_SN, MAX_TMP_STRING, ca_sn);
}

__s32 maple_register_cmmb_datasvcupdate_begin_callback(__pCBK_t cmmb_data_svc_update_begin_cb)
{
	__pCBK_t			callback;

	__msg("[maple_register_cmmb_datasvcupdate_begin_callback] called\n");
	
    callback = esKRNL_GetCallBack(cmmb_data_svc_update_begin_cb);
    esMODS_MIoctrl(maple, MAPLE_CMD_CB_SET_PARAM, MAPLE_CB_CMMB_DATASVC_BEGIN, (void*)cmmb_datasvc_updatebegin_para);
    esMODS_MIoctrl(maple, MAPLE_CMD_CB_SET, MAPLE_CB_CMMB_DATASVC_BEGIN, (void*)callback);
    
	return EPDK_OK;

}

__s32 maple_register_cmmb_datasvcupdate_end_callback(__pCBK_t cmmb_data_svc_update_end_cb)
{
	__pCBK_t			callback;

	__msg("[maple_register_cmmb_datasvcupdate_end_callback] called\n");
	
    callback = esKRNL_GetCallBack(cmmb_data_svc_update_end_cb);
    esMODS_MIoctrl(maple, MAPLE_CMD_CB_SET_PARAM, MAPLE_CB_CMMB_DATASVC_END, (void*)cmmb_datasvc_updateend_para);
    esMODS_MIoctrl(maple, MAPLE_CMD_CB_SET, MAPLE_CB_CMMB_DATASVC_END, (void*)callback);

	return EPDK_OK;

}
#endif

#if((MAPLE_TV_STANDAR == MAPLE_TV_ISDB_T)||(MAPLE_TV_STANDAR == MAPLE_TV_DTMB)\
		|| (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)|| (MAPLE_TV_STANDAR == MAPLE_TV_ATSC))
//只有isdbt有字幕，CMMB没有,add by hot.lee
__s32 maple_set_subtitle_cb(__pCBK_t isdbt_cb)
{
	__pCBK_t			callback;

	eDbug("isdbt_cb is %p \n",isdbt_cb);
    
    callback = esKRNL_GetCallBack(isdbt_cb);

	return esMODS_MIoctrl( maple, MAPLE_CMD_SET_SPLY_DRAW_CB, NULL, (void*)callback );

}


//字幕解码类型
__s32 maple_set_default_char_coding_type(void)
{	

	return esMODS_MIoctrl( maple, MAPLE_CMD_SET_DEFAULT_CHAR_CODING_TYPE, g_type, NULL );

}

#endif

