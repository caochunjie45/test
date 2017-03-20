/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
* File    	: 	Explorer.c,
*			Explorer.h
* By      	: 	Libaiao
* Func	: 
* Version	: 	v1.0
* ============================================================================================================
* 2010-4-14 15:24:52  Libaiao  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "beetles_app.h"

#include "Explorer.h"
#include "Explorer_List.h"
#include "Explorer_UI.h"





typedef enum
{
	
	EXP_MEDIA_TYPE_ALL,				//����ý������
	EXP_MEDIA_TYPE_PIC,				//����ͼƬ�ļ�
	EXP_MEDIA_TYPE_AUDIO,			//������Ƶ�ļ�
	EXP_MEDIA_TYPE_VIDEO,			//������Ƶ�ļ�
	EXP_MEDIA_TYPE_EBOOK,			//���е������ļ�
	EXP_MEDIA_TYPE_GAME,			//������Ϸ�ļ�
	EXP_MEDIA_TYPE_FIRMWARE,		//���й̼��ļ�
	EXP_MEDIA_TYPE_UNKNOWN,			//����ϵͳ��֧�ֵ��ļ�
}explorer_media_type_t;


typedef struct tag_explorer_ctrl
{
	H_WIN h_explr;
	//H_LYR BG_layer;			//background layer 8bpp(ע�ⱳ��ͼ��Ҫ����Ϊ8bpp)  //cai modify for ����ȫ��
	H_LYR list_layer;		//layer for listbar, 32bpp(32bpp,��Ϊlistbar����ͼΪ32bpp)
    H_WIN h_list_framewin;
	
	__s32 root_type;					//USB Device, or  SDCard
	__u32 last_start_id;				//�ϴ������ʼID
	__u32 last_focused_id;				//�ϴ�ѡ���ļ�id
	char  *last_filename;				//�ϴ�����ļ� ��
	
	explorer_view_mode_e view_mode;		//��ʾģʽ
	//explorer_media_type_t media_type;
	rat_media_type_t media_type;
	GUI_FONT *explr_font;
	char root[4];                          //��¼�������̷�
	root_para_t  *root_para ; 
}explorer_ctrl_t; 

//----------------------------------------------------------------------------------------------------
//define the global variables  
//static explorer_param_t *explorer_param = NULL;


//private function
static __s32 _explorer_manager_win_cb(__gui_msg_t *msg);//process the  message of the manager window from system 

static __s32 explorer_scene_create(__gui_msg_t *msg);

static H_LYR explorer_8bpp_layer_create(void);
static H_LYR explorer_32bpp_layer_create(void);
//static H_LYR explorer_layer_create(void);
static __s32 explorer_layer_delete(H_LYR layer);

static __s32 app_explorer_on_close(__gui_msg_t *msg);
static __s32 app_explorer_on_destroy(H_WIN h_exp_win);

//
//----------------------------------------------------------------------------------------------------

/*
************************************************************************************************************************
*Function	:           __s32 Explorer_create(root_para_t *para)          				
*
*Description	: 		Explorer Application Entry interface 
*					create explorer manager window
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
void __app_explorer_change__bg(void)
{
    reg_init_para_t* para;
    para = dsk_reg_get_para_by_app(REG_APP_INIT);
    if(para)
    {
        __s32 ret;

        ret = gscene_bgd_get_flag();
        if(ret != para->bgd_flag_in_main)
        {
            gscene_bgd_set_flag(para->bgd_flag_in_main);
            gscene_bgd_refresh();                                                      
        }   
        else
        {
            __msg("ret == para->bgd_flag_in_main, not need change bg...\n");
        }
    }
    else
    {
        __msg("dsk_reg_get_para_by_app fail...\n");
    }

    gscene_bgd_set_state(BGD_STATUS_SHOW);
    
}

__s32 app_explorer_create(root_para_t  *para)
{
	__gui_manwincreate_para_t create_info;
	explorer_ctrl_t	*explr_ctrl;
	//reg_root_para_t *root_para = NULL;
	
		
	__inf("****************************************************************************************\n");
	__inf("********  enter explorer app  **************\n");
	__inf("****************************************************************************************\n");

	//__wait__;	
	
	explr_ctrl = (explorer_ctrl_t *)esMEMS_Balloc(sizeof(explorer_ctrl_t));		//Ϊ˽�����������ڴ�
	eLIBs_memset(explr_ctrl, 0, sizeof(explorer_ctrl_t));

	//root_para = dsk_reg_get_para_by_app(REG_APP_ROOT);
	explr_ctrl->root_para = para ;
	explr_ctrl->explr_font = para->font;
	__app_explorer_change__bg();
	//gscene_bgd_set_state(BGD_STATUS_SHOW);
	__msg("para->root_type = %d\n", para->root_type);
	switch(para->data)
	{
	case ID_EXPLORER_ALL:
		explr_ctrl->view_mode = EXPLR_LIST_MODE;
		explr_ctrl->media_type = RAT_MEDIA_TYPE_ALL;
		explr_ctrl->root_type = para->root_type;
		//explr_ctrl->last_start_id = 0;
		//explr_ctrl->last_focused_id = 0;
		//explr_ctrl->last_filename = NULL;
		break;
	case ID_EXPLORER_MOVIE:
		explr_ctrl->view_mode = EXPLR_LIST_MODE;
		explr_ctrl->media_type = RAT_MEDIA_TYPE_VIDEO;
		explr_ctrl->root_type = para->root_type;
		/*
		if(para->root_type == RAT_TF)
		{
			explr_ctrl->last_start_id = root_para->last_movie_start_index_sd;
			explr_ctrl->last_focused_id = root_para->last_movie_index_sd;
			explr_ctrl->last_filename = root_para->last_movie_path_sd;			
		}
		else if(para->root_type == RAT_USB)
		{
			explr_ctrl->last_start_id = root_para->last_movie_start_index_ud;
			explr_ctrl->last_focused_id = root_para->last_movie_index_ud;
			explr_ctrl->last_filename = root_para->last_movie_path_ud;
		}
		*/
		break;
	case ID_EXPLORER_PHOTO:
		explr_ctrl->view_mode = EXPLR_LIST_MODE;		//EXPLR_SQUARE_MODE; 
		explr_ctrl->media_type = RAT_MEDIA_TYPE_PIC;
		explr_ctrl->root_type = para->root_type;
		/*
		if(para->root_type == RAT_TF)
		{
			explr_ctrl->last_start_id = root_para->last_photo_start_index_sd;
			explr_ctrl->last_focused_id = root_para->last_photo_index_sd;
			explr_ctrl->last_filename = root_para->last_photo_path_sd;			
		}
		else if(para->root_type == RAT_USB)
		{
			explr_ctrl->last_start_id = root_para->last_photo_start_index_ud;
			explr_ctrl->last_focused_id = root_para->last_photo_index_ud;
			explr_ctrl->last_filename = root_para->last_photo_path_ud;
		}
		*/
		break;
	case ID_EXPLORER_EBOOK:
		explr_ctrl->view_mode = EXPLR_LIST_MODE;
		explr_ctrl->media_type = RAT_MEDIA_TYPE_EBOOK;
		explr_ctrl->root_type = para->root_type;
		/*
		if(para->root_type == RAT_TF)
		{
			explr_ctrl->last_start_id = root_para->last_ebook_start_index_sd;
			explr_ctrl->last_focused_id = root_para->last_ebook_index_sd;
			explr_ctrl->last_filename = root_para->last_ebook_path_sd;			
		}
		else if(para->root_type == RAT_USB)
		{
			explr_ctrl->last_start_id = root_para->last_ebook_start_index_ud;
			explr_ctrl->last_focused_id = root_para->last_ebook_index_ud;
			explr_ctrl->last_filename = root_para->last_ebook_path_ud;
		}
		*/
		break;
	case ID_EXPLORER_MUSIC:
		__here__;
		explr_ctrl->view_mode = EXPLR_LIST_MODE;
		explr_ctrl->media_type = RAT_MEDIA_TYPE_AUDIO;
		explr_ctrl->root_type = para->root_type;		
		/*
		if(para->root_type == RAT_TF)
		{
			explr_ctrl->last_start_id = root_para->last_music_start_index_sd;
			explr_ctrl->last_focused_id = root_para->last_music_index_sd;
			explr_ctrl->last_filename = root_para->last_music_path_sd;			
		}
		else if(para->root_type == RAT_USB)
		{
			explr_ctrl->last_start_id = root_para->last_music_start_index_ud;
			explr_ctrl->last_focused_id = root_para->last_music_index_ud;
			explr_ctrl->last_filename = root_para->last_music_path_ud;
		}
		*/
		break;
#if EXTERN_KARAOKE_EXIST
	case ID_EXPLORER_KARAOKE:
		__here__;
		explr_ctrl->view_mode = EXPLR_LIST_MODE;
		explr_ctrl->media_type = RAT_MEDIA_TYPE_KARAOKE;
		explr_ctrl->root_type = para->root_type;
		//..explr_ctrl->karaoke_lab = 1;
		break;

#endif
	case ID_EXPLORER_RESTORE:
		explr_ctrl->media_type = RAT_MEDIA_TYPE_UNKNOWN;
		break;
	default:
		break;
	}
	
	eLIBs_memset(&create_info, 0, sizeof(__gui_manwincreate_para_t));
	
	create_info.name            = APP_EXPLORER;
	create_info.hParent         = para->h_parent;
	create_info.ManWindowProc   = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)_explorer_manager_win_cb); 
	create_info.attr            = (void*)explr_ctrl;			//����Managerwin ˽������
	create_info.id				= APP_EXPLORER_ID;
	create_info.hHosting        = NULL;

	return(GUI_ManWinCreate(&create_info));
}

/*
************************************************************************************************************************
*Function	:           __s32 app_explorer_on_close(H_WIN h_exp_win)
*
*Description	: 		����EXPLORER֮���AP, ��������,�ͷŴ���
*					(�ͷŴ��ں���֮����Ӧdestroy��Ϣ)
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
__s32 app_explorer_on_close(__gui_msg_t *msg)
{
	explorer_ctrl_t	*explr_ctrl;
			
	explr_ctrl	 = (explorer_ctrl_t *)GUI_WinGetAttr(msg->h_deswin);
	//save_setting(explr_ctrl);
	GUI_ManWinDelete(msg->h_deswin);				//it will send detroy message after call this function.
	//cat_item = (__cat_item_t *)msg->dwAddData1;
			
	//if (cat_item == 0)
	{
		//explorer_cmd2parent(msg->h_deswin, SWITCH_TO_OTHER_APP, EXPLR_SW_TO_MAIN, 0);
	}
	//else
	{				
		switch(LOWORD(msg->dwAddData2))
		{
			case EXP_MEDIA_TYPE_VIDEO:
				//explorer_cmd2parent(msg->h_deswin, SWITCH_TO_OTHER_APP, EXPLR_SW_TO_MOVIE, HIWORD(msg->dwAddData2));
				break;
			case EXP_MEDIA_TYPE_PIC:
				//explorer_cmd2parent(msg->h_deswin, SWITCH_TO_OTHER_APP, EXPLR_SW_TO_PHOTO, HIWORD(msg->dwAddData2));
				break;
			case EXP_MEDIA_TYPE_EBOOK:
				//explorer_cmd2parent(msg->h_deswin, SWITCH_TO_OTHER_APP, EXPLR_SW_TO_EBOOK, HIWORD(msg->dwAddData2));
				break;
			case EXP_MEDIA_TYPE_AUDIO:
				//explorer_cmd2parent(msg->h_deswin, SWITCH_TO_OTHER_APP, EXPLR_SW_TO_MUSIC, HIWORD(msg->dwAddData2));
				break;
			default:
				break;		
		}
	}	

	return EPDK_OK;
	
}
/*
************************************************************************************************************************
*Function	:           __s32 app_explorer_on_destroy(H_WIN h_exp_win)
*
*Description	: 		�ͷ�ͼ�㣬�ͷ��ڴ�
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
static __s32 app_explorer_on_destroy(H_WIN h_exp_win)
{
	explorer_ctrl_t	*explr_ctrl;
	explr_ctrl	 = (explorer_ctrl_t *)GUI_WinGetAttr(h_exp_win);
		
	explorer_layer_delete(explr_ctrl->list_layer);
#if  0 //cai modify 
    if(explr_ctrl->BG_layer)
    {
        GUI_LyrWinSetTop(explr_ctrl->BG_layer);
    }
#endif	
   // gscene_bgd_set_state(BGD_STATUS_SHOW);
	//explorer_layer_delete(explr_ctrl->BG_layer);
	
	esMEMS_Bfree(explr_ctrl, sizeof(explorer_ctrl_t));
	
	return EPDK_OK;
}

/*
************************************************************************************************************************
*Function	:           static __s32 app_explorer_on_command(__gui_msg_t *msg)
*
*Description	: 		�û��Զ����������
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
static __s32 app_explorer_on_command(__gui_msg_t *msg)
{
	__gui_msg_t my_msg;
	explorer_ctrl_t	*explr_ctrl;
	explr_ctrl	 = (explorer_ctrl_t *)GUI_WinGetAttr(msg->h_deswin);
	
	if (LOWORD(msg->dwAddData1) == DELETE_FILE_DIALOG_ID
        || LOWORD(msg->dwAddData1) == FW_UPDATE_DIALOG_ID)
	{
		if(explr_ctrl->h_list_framewin == NULL)
		{
			return EPDK_OK;
		}		
		my_msg.h_deswin = explr_ctrl->h_list_framewin;
		
		my_msg.h_srcwin = msg->h_deswin;	
        
		my_msg.id = GUI_MSG_COMMAND;
		my_msg.dwAddData1 = msg->dwAddData1;//
		my_msg.dwAddData2 = msg->dwAddData2;
		my_msg.dwReserved = msg->dwReserved;

        __here__;
		//GUI_SendNotifyMessage(msg);
		GUI_SendMessage(&my_msg);
        __here__;
	}		
	return EPDK_OK;
}

/*
************************************************************************************************************************
*Function	:          void  explorer_cmd2parent(H_WIN hwin, __s32 id, __s32 data1, __s32 data2)
*
*Description	: 		�򸸴��ڴ���������Ϣ
*					
*Arguments  	: 		hwin, �����ھ��
*					id,     �������
*Return     	: 	
*
************************************************************************************************************************
*/

void  app_explorer_cmd2parent(H_WIN hwin, __s32 id, __s32 data1, __s32 data2)
{
	__gui_msg_t msg;
	char buf[128];
	//H_WIN hExpWin;
	//hExpWin = GUI_WinGetParent(hwin);
	msg.h_deswin = GUI_WinGetParent(hwin);
	//msg.h_srcwin = NULL;
	msg.h_srcwin = hwin;	

    __here__;
	msg.id = GUI_MSG_COMMAND;
	msg.dwAddData1 = MAKELONG(GUI_WinGetItemId(hwin), id);
	msg.dwAddData2 = data1;
	msg.dwReserved = data2;	
	GUI_WinGetName(msg.h_deswin, buf);	
	__msg("%s\n", buf);	
	GUI_SendNotifyMessage(&msg);	
}

static __s32  ExplorerGetSearchPath(__s32 root_type, char* path)
{
	__s32 ret = 0;
	char str_root[50];
	char disk_name[RAT_MAX_PARTITION][4];
	
	eLIBs_memset(str_root, 0, sizeof(str_root));
	eLIBs_memset(disk_name, 0, sizeof(disk_name));
	
	switch(root_type)
	{
		case RAT_LOCAL:
			eLIBs_strcpy(str_root,RAT_LOCAL_DISK);				
			break;
		case RAT_TF:
			eLIBs_strcpy(str_root,RAT_SD_CARD);				
			break;
		case RAT_USB:
			eLIBs_strcpy(str_root,RAT_USB_DISK);				
			break;
		default:
			break;
	}

    __msg("root_type=%s\n", root_type);
	ret = rat_get_partition_name(str_root, disk_name);
    
	eLIBs_strcpy(path,disk_name[0]);	
    __msg("disk_name[0]=%s\n", disk_name[0]);
	
	if(path[0])
	{
	    return EPDK_OK;
	}
    else
    {
        return EPDK_FAIL;
    }
}

/*
************************************************************************************************************************
*Function	:           static __s32 _explorer_manager_win_cb(__gui_msg_t *msg)           				
*
*Description	: 		manager window contrl block
*					gets message from system, & dispatch command to child
*Arguments  	: 		get a message from system
*
*Return     	: 	
*
************************************************************************************************************************
*/
static __s32 _explorer_manager_win_cb(__gui_msg_t *msg)
{
	switch(msg->id)
	{
	case GUI_MSG_CREATE:
        __msg("----explorer manager window GUI_MSG_CREATE begin----\n"); 
		if(dsk_get_fsm_statu() != 2)
			dsk_set_fsm_statu(2);

		explorer_scene_create(msg);
        __msg("----explorer manager window GUI_MSG_CREATE end----\n"); 
		return EPDK_OK;
	case GUI_MSG_CLOSE:
		__msg("----explorer manager window GUI_MSG_CLOSE begin----\n"); 
		app_explorer_on_close(msg);
		return EPDK_OK;
	case GUI_MSG_DESTROY:			
		__msg("----explorer manager window GUI_MSG_DESTROY begin----\n"); 		
		app_explorer_on_destroy(msg->h_deswin);			//release  layer,memory 
		__msg("----explorer manager window GUI_MSG_DESTROY end----\n"); 		
		return EPDK_OK;
	case GUI_MSG_KEY:
		break;
	case GUI_MSG_TOUCH:
		break;
	case GUI_MSG_COMMAND:

        __here__;
		//__msg("----explorer manager window GUI_MSG_CLOSE begin----\n");
		app_explorer_on_command(msg);
        __here__;
		{
			/*
			if (LOWORD(msg->dwAddData1) == EXPLR_LIST_ID)
			{
				//explorer_list_command_proc(msg);
			}
			else if (LOWORD(msg->dwAddData1) == EXPLR_SQUARE_ID)
			{
				
			}
			*/
		}
		return EPDK_OK;    
	default:
		break;
	}
	
	return GUI_ManWinDefaultProc(msg);		
}


/*
************************************************************************************************************************
*Function	:          static H_LYR explorer_8bpp_layer_create(void)
*
*Description	: 		����ͼ�㣬��Ҫ������ʾ8bpp ����ͼƬ
*					ע����ǰ��ͼӦ���ڲ�ͬ��pipe
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
static H_LYR explorer_8bpp_layer_create(void)
{
	H_LYR layer = NULL;
	RECT LayerRect;
	//int ret;
	//__layerwincreate_para_t lyrcreate_info;
	FB  fb =
	{
	    {0, 0},                                   		/* size      */
	    {0, 0, 0},                                      /* buffer    */
	    //{FB_TYPE_RGB, {PIXEL_MONO_8BPP, 0, (__rgb_seq_t)0}},    /* fmt       */
	    //{FB_TYPE_RGB, {PIXEL_COLOR_ARGB8888, 0, (__rgb_seq_t)0}},    /* fmt       */
	    {FB_TYPE_RGB, {PIXEL_MONO_8BPP, 0, (__rgb_seq_t)0}},    /* fmt       */
	};

	__disp_layer_para_t lstlyr =
	{
	    DISP_LAYER_WORK_MODE_PALETTE,                    /* mode      */
	    //DISP_LAYER_WORK_MODE_NORMAL,
	    0,                                              /* ck_mode   */
	    0,                                              /* alpha_en  */
	    0,                                              /* alpha_val */
	    0,                                              /* pipe      */
	    0xff,                                           /* prio      */
	    {0, 0, 0, 0},                           		/* screen    */
	    {0, 0, 0, 0},                               	/* source    */
	    DISP_LAYER_OUTPUT_CHN_DE_CH1,                   /* channel   */
	   NULL                                            /* fb        */
    };	                                      
    __layerwincreate_para_t lyrcreate_info =  
    {                                         
        "APP_EXPLORER",                       
      	NULL,                                 
      	GUI_LYRWIN_STA_SUSPEND,               
      	GUI_LYRWIN_NORMAL                     
    };	
	
	explorer_get_screen_rect(&LayerRect);
	
    fb.size.width		= LayerRect.width;            
    fb.size.height		= LayerRect.height;	        
    fb.fmt.fmt.rgb.pixelfmt = PIXEL_MONO_8BPP; //PIXEL_COLOR_ARGB8888;
	
	lstlyr.src_win.x  		= LayerRect.x;
	lstlyr.src_win.y  		= LayerRect.y;
	lstlyr.src_win.width 	= LayerRect.width;
	lstlyr.src_win.height 	= LayerRect.height;
	
	lstlyr.scn_win.x		= LayerRect.x;
	lstlyr.scn_win.y		= LayerRect.y;
	lstlyr.scn_win.width  	= LayerRect.width;
	lstlyr.scn_win.height 	= LayerRect.height;
	
	lstlyr.pipe = 0;
	lstlyr.fb = &fb;
	lyrcreate_info.lyrpara = &lstlyr;
	
	layer = GUI_LyrWinCreate(&lyrcreate_info);
	
	if( !layer )
	{
		__err("app bar layer create error !\n");
	} 		
	return layer;	
}

/*
************************************************************************************************************************
*Function	:          static H_LYR explorer_32bpp_layer_create(void)
*
*Description	: 		ǰ��ͼ�㣬��Ҫ������ʾǰ��ͼƬ��ListBar���item, ����ͼ
*					ע���뱳��ͼӦ���ڲ�ͬ��pipe
*					
*Arguments  	: 		
*
*Return     	: 	     ͼ����
*
************************************************************************************************************************
*/
static H_LYR explorer_32bpp_layer_create(void)
{
	H_LYR layer = NULL;
	RECT LayerRect;
//	int ret;
	//__layerwincreate_para_t lyrcreate_info;
	FB  fb =
	{
	    {0, 0},                                   		/* size      */
	    {0, 0, 0},                                      /* buffer    */
	    //{FB_TYPE_RGB, {PIXEL_MONO_8BPP, 0, (__rgb_seq_t)0}},    /* fmt       */
	    {FB_TYPE_RGB, {PIXEL_COLOR_ARGB8888, 0, (__rgb_seq_t)0}},    /* fmt       */
	    
	};

	__disp_layer_para_t lstlyr =
	{
	    //DISP_LAYER_WORK_MODE_PALETTE,                    /* mode      */
	    DISP_LAYER_WORK_MODE_NORMAL,
	    0,                                              /* ck_mode   */
	    0,                                              /* alpha_en  */
	    0,                                              /* alpha_val */
	    1,                                              /* pipe      */
	    0xff,                                           /* prio      */
	    {0, 0, 0, 0},                           		/* screen    */
	    {0, 0, 0, 0},                               	/* source    */
	    DISP_LAYER_OUTPUT_CHN_DE_CH1,                   /* channel   */
	   NULL                                            /* fb        */
    };	                                      
    __layerwincreate_para_t lyrcreate_info =  
    {                                         
        "APP_EXPLORER",                       
      	NULL,                                 
      	GUI_LYRWIN_STA_SUSPEND,               
      	GUI_LYRWIN_NORMAL                     
    };	
	
	explorer_get_screen_rect(&LayerRect);
	
    fb.size.width		= LayerRect.width-80;//cai modify for 480X272_middle            
    fb.size.height		= LayerRect.height-32;	//cai modify for 480X272_middle 	        
    fb.fmt.fmt.rgb.pixelfmt = PIXEL_COLOR_ARGB8888; //PIXEL_MONO_8BPP; //PIXEL_COLOR_ARGB8888;
	
	lstlyr.src_win.x  		= LayerRect.x;
	lstlyr.src_win.y  		= LayerRect.y;
	lstlyr.src_win.width 	= LayerRect.width-80;//cai modify for 480X272_middle 
	lstlyr.src_win.height 	= LayerRect.height-32;//cai modify for 480X272_middle 
	
	lstlyr.scn_win.x		= LayerRect.x+40;//cai modify for 480X272_middle 
	lstlyr.scn_win.y		= LayerRect.y+16;//cai modify for 480X272_middle 
	lstlyr.scn_win.width  	= LayerRect.width-80;//cai modify for 480X272_middle 
	lstlyr.scn_win.height 	= LayerRect.height-32;//cai modify for 480X272_middle 
	
	lstlyr.pipe = 1;
	lstlyr.fb = &fb;
	lyrcreate_info.lyrpara = &lstlyr;
	
	layer = GUI_LyrWinCreate(&lyrcreate_info);
	
	if( !layer )
	{
		__err("app bar layer create error !\n");
	}
	return layer;	
}
/*
************************************************************************************************************************
*Function	:           static __s32 explorer_layer_delete(H_LYR layer)
*
*Description	: 		ɾ��ͼ��
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
static __s32 explorer_layer_delete(H_LYR layer)
{
	if(layer != NULL)
	{
		GUI_LyrWinDelete(layer);
	}	
	return EPDK_OK;
}
#if  0 //cai modify
static __s32 explorer_draw_bg(explorer_ctrl_t	*explr_ctrl)
{
    if(!explr_ctrl || !explr_ctrl->BG_layer)
    {
        __msg("invalid para...\n");
        return EPDK_FAIL;
    }
    
    {
        __s32 index;
        __s32 bg_id;
        reg_system_para_t* para;
        HTHEME hdl;

        para = dsk_reg_get_para_by_app(REG_APP_SYSTEM);
        if(para)
        {
            index = para->style;
        }
        else
        {
            index = 0;
        }

        switch(index)
        {
            case 0:
            {
                bg_id = ID_EXPLORER_BG0_BMP;
                break;
            }
            case 1:
            {
                bg_id = ID_EXPLORER_BG1_BMP;
                break;
            }
            case 2:
            {
                bg_id = ID_EXPLORER_BG2_BMP;
                break;
            }
            default:
            {
                bg_id = ID_EXPLORER_BG0_BMP;
                break;
            }
        }

        GUI_LyrWinSetSta(explr_ctrl->BG_layer, GUI_LYRWIN_STA_ON);
        GUI_LyrWinSetBottom(explr_ctrl->BG_layer);
        
        GUI_LyrWinSel(explr_ctrl->BG_layer);
        com_set_palette_by_id(ID_EXPLORER_BG_PAL_BMP);
        hdl = dsk_theme_open(bg_id);
        if(hdl)
        {
            RECT rect;

            GUI_LyrWinGetScnWindow(explr_ctrl->BG_layer, &rect);
            GUI_BMP_Draw(dsk_theme_hdl2buf(hdl), rect.x, rect.y);
            dsk_theme_close(hdl);
            hdl = NULL;
            gscene_bgd_set_state(BGD_STATUS_HIDE);
        }        
    }

    return EPDK_OK;
}
#endif

/*
************************************************************************************************************************
*Function	:           static __s32 explorer_scene_create()
*
*Description	: 		���������������ListBar
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/

static __s32 explorer_scene_create(__gui_msg_t *msg)
{
	explorer_ctrl_t	*explr_ctrl;
	//RECT LayerRect;	
	explr_list_para_t ListPara;
	
	eLIBs_memset(&ListPara, 0, sizeof(explr_list_para_t));

	//g_disable_standby();
	
	//__cat_item_t  *cat_cur_item;
	
	explr_ctrl = (explorer_ctrl_t *)GUI_WinGetAttr(msg->h_deswin);   
	
	ListPara.root_type = explr_ctrl->root_type;	
	ListPara.last_start_id = explr_ctrl->last_start_id;
	ListPara.last_focused_id = explr_ctrl->last_focused_id;
	ListPara.last_filename = explr_ctrl->last_filename;		//�����һ�������Ϣ
	
	ListPara.media_type =explr_ctrl->media_type;
	ListPara.view_mode = explr_ctrl->view_mode;		//view mode, 
	ListPara.root_para = explr_ctrl->root_para ;
    if(RAT_MEDIA_TYPE_ALL == explr_ctrl->media_type)
    {
        eLIBs_strcpy(explr_ctrl->root, "");
    }
    else
    {
        ExplorerGetSearchPath(explr_ctrl->root_type, explr_ctrl->root);
        __msg("explr_ctrl->root=%s\n", explr_ctrl->root);
    }    
        __msg("\n\n~~~ explr_ctrl->root=%s ~~~\n\n", explr_ctrl->root);	
//	explr_ctrl->BG_layer= explorer_8bpp_layer_create();  //cai modify for ����ȫ��
	explr_ctrl->list_layer = explorer_32bpp_layer_create();

//    explorer_draw_bg(explr_ctrl);   //cai modify for ����ȫ��

//	ListPara.BG_layer = explr_ctrl->BG_layer;		//����ͼ��  //cai modify
	ListPara.list_layer = explr_ctrl->list_layer;	//listbar layer
		
	explr_ctrl->h_list_framewin = explorer_list_win_create(msg->h_deswin, &ListPara);	
	GUI_WinSetFocusChild(explr_ctrl->h_list_framewin);		//frame win get focus.					//set picture layer as top level	
    //GUI_LyrWinCacheOn();		//cache on
    //GUI_LyrWinSel(explr_ctrl->lyr_explr);
    
    //GUI_LyrWinSetSta(explr_ctrl->BG_layer, GUI_LYRWIN_STA_ON);  //cai modify
      // gscene_bgd_set_state(BGD_STATUS_SHOW);   //cai++ for ����ͼ��
	GUI_LyrWinSetSta(explr_ctrl->list_layer, GUI_LYRWIN_STA_ON);
	//GUI_LyrWinSetTop(ListPara.layer);						//set picture layer as top level
	
    //GUI_LyrWinCacheOff();		//cache off             
    
    return EPDK_OK;		  
}

