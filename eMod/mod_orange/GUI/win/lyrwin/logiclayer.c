
#include "lyrwin_i.h"

#define DIR_CLOCKWISE_0 0
#define DIR_CLOCKWISE_90 1
#define DIR_CLOCKWISE_180 2
#define DIR_CLOCKWISE_270 3

//==================================================================
//function name��   GUI_LogicToPhysicalConvert
//author��   	danling
//date��            2010-03-30
//description��   �߼����ڵ������ڵ�ת��
//parameters��   dir: ����
//				rect_logic: �߼�����ָ��
//				rect_phy: ������ָ��
//				width: (LCD/TV)��Ļ��framebuffer�Ŀ��
//				height: (LCD/TV)��Ļ��framebuffer�ĸ߶�
//return��          success returns EPDK_OK
//                  	fail  returns the number of failed
//modify history��
//==================================================================
static __s32 GUI_LogicToPhysicalConvert(__u8 dir,__rect_t * rect_logic,__rect_t * rect_phy,__u32 width,__u32 height)
{
	switch (dir)
	{
		case DIR_CLOCKWISE_0:
			rect_phy->x = rect_logic->x;
			rect_phy->y = rect_logic->y;
			rect_phy->width = rect_logic->width;
			rect_phy->height = rect_logic->height;
			break;
		case DIR_CLOCKWISE_90:
			rect_phy->x = rect_logic->y;
			rect_phy->y = height - rect_logic->x - rect_logic->width;
			rect_phy->width = rect_logic->height;
			rect_phy->height = rect_logic->width;
			break;
		case DIR_CLOCKWISE_180:
			rect_phy->x = width - rect_logic->x - rect_logic->width;
			rect_phy->y = height - rect_logic->y - rect_logic->height;
			rect_phy->width = rect_logic->width;
			rect_phy->height = rect_logic->height;
			break;
		case DIR_CLOCKWISE_270:
			rect_phy->x = width - rect_logic->y - rect_logic->height;
			rect_phy->y = rect_logic->x;
			rect_phy->width = rect_logic->height;
			rect_phy->height = rect_logic->width;
			break;
		default:
			break;
	}
	return EPDK_OK;
}


//==================================================================
//function name��   GUI_PhysicalToLogicConvert
//author��   	danling
//date��            2010-03-30
//description��   �����ڵ��߼����ڵ�ת��
//parameters��   dir: ����
//				rect_logic: �߼�����ָ��
//				rect_phy: ������ָ��
//				width: (LCD/TV)��Ļ��framebuffer�Ŀ��
//				height: (LCD/TV)��Ļ��framebuffer�ĸ߶�
//return��          success returns EPDK_OK
//                  	fail  returns the number of failed
//modify history��
//==================================================================
static __s32 GUI_PhysicalToLogicConvert(__u8 dir,__rect_t * rect_logic,__rect_t * rect_phy,__u32 width,__u32 height)
{
	switch(dir)
	{
	case DIR_CLOCKWISE_0:
		rect_logic->x = rect_phy->x;
		rect_logic->y = rect_phy->y;
		rect_logic->width = rect_phy->width;
		rect_logic->height = rect_phy->height;
		break;
	case DIR_CLOCKWISE_90:
		rect_logic->x = height - rect_phy->height - rect_phy->y;
		rect_logic->y = rect_phy->x;
		rect_logic->width = rect_phy->height;
		rect_logic->height = rect_phy->width;
		break;
	case DIR_CLOCKWISE_180:
		rect_logic->x = width - rect_phy->width - rect_phy->x;
		rect_logic->y = height - rect_phy->height - rect_phy->y;
		rect_logic->width = rect_phy->width;
		rect_logic->height = rect_phy->height;
		break;
	case DIR_CLOCKWISE_270:
		rect_logic->x = rect_phy->y;
		rect_logic->y = width - rect_phy->width - rect_phy->x;
		rect_logic->width = rect_phy->height;
		rect_logic->height = rect_phy->width;
		break;
	default:
		break;
	}
	return EPDK_OK;
}


//==================================================================
//function name��   GUI_LogicLayerSetScnWin
//author��   	danling
//date��            2010-03-30
//description��   ���߼��ϵ�������Ļ����ת������Ӧ���������
//parameters��   dir: ����
//				disp_handle: ��ʾ�����ļ�ָ��
//				hlayer: ͼ����
//				rect: �߼���Ļ����
//return��          success returns EPDK_OK
//                  	fail  returns the number of failed
//modify history��
//==================================================================
__s32 GUI_LogicLayerSetScnWin(__u8 dir,ES_FILE *disp_handle,__hdle hlayer,__rect_t *rect)
{
	__u32 ret = 0;
	__u32 scn_width=0,scn_height=0;
	__rect_t scn_rect_phy;
	__disp_layer_para_t cur_player;
	FB fb;

	//modified by Derek,2010-12-9 17:30:49
	__disp_layer_info_t disp_lyr={0}, disp_src_lyr={0};
	__u32 pdispbuffer[3];

	scn_width = eLIBs_fioctrl(disp_handle,DISP_CMD_SCN_GET_WIDTH, SEL_SCREEN,0);
	scn_height = eLIBs_fioctrl(disp_handle,DISP_CMD_SCN_GET_HEIGHT, SEL_SCREEN,0);

	cur_player.fb = &fb;
	
	pdispbuffer[0] = hlayer;
	pdispbuffer[1] = (__u32)&disp_src_lyr;
	pdispbuffer[2] = 0;
	eLIBs_fioctrl(disp_handle, DISP_CMD_LAYER_GET_PARA, SEL_SCREEN, (void *)pdispbuffer);

	GUI_DispLyr2LogicLyr(&cur_player, &disp_src_lyr); //end modified
	
	if(cur_player.mode != DISP_LAYER_WORK_MODE_SCALER)
	{
		__u32 fb_width=0,fb_height=0;
		__rect_t src_rect_logic,src_rect_phy;
		
		src_rect_logic.width = rect->width;
		src_rect_logic.height = rect->height;
		fb_width = cur_player.fb->size.width;
		fb_height = cur_player.fb->size.height;
		switch(dir)
		{
			case DIR_CLOCKWISE_0:
				src_rect_logic.x = cur_player.src_win.x;
				src_rect_logic.y = cur_player.src_win.y;
				break;
			case DIR_CLOCKWISE_90:
			src_rect_logic.x = fb_height- cur_player.scn_win.height - cur_player.src_win.y;
			src_rect_logic.y = cur_player.src_win.x;
			break;
			case DIR_CLOCKWISE_180:
			src_rect_logic.x = fb_width- cur_player.scn_win.width - cur_player.src_win.x;
			src_rect_logic.y = fb_height- cur_player.scn_win.height - cur_player.src_win.y;
				break;
			case DIR_CLOCKWISE_270:
			src_rect_logic.x = cur_player.src_win.y;
			src_rect_logic.y = fb_width- cur_player.scn_win.width - cur_player.src_win.x;
				break;
			default:
				break;
		}

		GUI_LogicToPhysicalConvert(dir,&src_rect_logic,&src_rect_phy,fb_width,fb_height);
		cur_player.src_win.x = src_rect_phy.x;
		cur_player.src_win.y = src_rect_phy.y;
		cur_player.src_win.width = src_rect_phy.width;
		cur_player.src_win.height = src_rect_phy.height;

	}
	
	GUI_LogicToPhysicalConvert(dir,rect,&scn_rect_phy,scn_width,scn_height);
	cur_player.scn_win.x = scn_rect_phy.x;
	cur_player.scn_win.y = scn_rect_phy.y;
	cur_player.scn_win.width = scn_rect_phy.width;
	cur_player.scn_win.height = scn_rect_phy.height;

	GUI_LogicLyr2DispLyr(&cur_player, &disp_lyr); //modified by Derek,2010-12-9 17:07:38
	pdispbuffer[0] = hlayer;
	pdispbuffer[1] = (__u32)&disp_lyr;
	pdispbuffer[2] = 0;
	
	ret |= eLIBs_fioctrl(disp_handle, DISP_CMD_LAYER_SET_PARA, SEL_SCREEN, (void *)pdispbuffer); //end modified
	
    if(ret != EPDK_OK)
    {
        return EPDK_FAIL;
    }
    return EPDK_OK;
}


//==================================================================
//function name��   GUI_LogicLayerGetScnWin
//author��   	danling
//date��            2010-03-30
//description��  ��ȡ�߼��ϵ���Ļ����
//parameters��   dir: ����
//				disp_handle: ��ʾ�����ļ�ָ��
//				hlayer: ͼ����
//				rect: �߼���Ļ����
//return��          success returns EPDK_OK
//                  	fail  returns the number of failed
//modify history��
//==================================================================

__s32 GUI_LogicLayerGetScnWin(__u8 dir,ES_FILE *disp_handle,__hdle hlayer,__rect_t *rect)
{
	__u32 scn_width=0,scn_height=0;
	__rect_t scn_rect_phy;

	__u32 pdispbuffer[3];
	

	scn_width = eLIBs_fioctrl(disp_handle,DISP_CMD_SCN_GET_WIDTH, SEL_SCREEN,0); //modified by Derek,2010.12.07.15:08
	scn_height = eLIBs_fioctrl(disp_handle,DISP_CMD_SCN_GET_HEIGHT, SEL_SCREEN,0); //modified by Derek,2010.12.07.15:08
	
	pdispbuffer[0] = hlayer;
	pdispbuffer[1] = (__u32)&scn_rect_phy;
	pdispbuffer[2] = 0;
	eLIBs_fioctrl(disp_handle, DISP_CMD_LAYER_GET_SCN_WINDOW, SEL_SCREEN, (void *)pdispbuffer); //end modified
	
	GUI_PhysicalToLogicConvert(dir,rect,&scn_rect_phy,scn_width,scn_height);

	return EPDK_OK;
}


//==================================================================
//function name��   GUI_LogicLayerSetSrcWin
//author��   	danling
//date��            2010-03-30
//description��   ���߼��ϵ�����Դ����ת������Ӧ���������
//parameters��   dir: ����
//				disp_handle: ��ʾ�����ļ�ָ��
//				hlayer: ͼ����
//				rect: �߼�Դ����
//return��          success returns EPDK_OK
//                  	fail  returns the number of failed
//modify history��
//==================================================================

__s32 GUI_LogicLayerSetSrcWin(__u8 dir,ES_FILE *disp_handle,__hdle hlayer,__rect_t *rect)
{
	__u32       ret = 0;
	__u32       fb_width=0,fb_height=0;
	__rect_t    src_rect_phy;
	FB	        cur_fb;
    __rect_t    scn_rect;

	/*modified by Derek,2010-12-9 15:40:35*/
	__u32		pdispbuffer[3];
	__disp_fb_t disp_fb_para={0};

	pdispbuffer[0] = hlayer;
	pdispbuffer[1] = (__u32)&disp_fb_para;
	pdispbuffer[2] = 0;

	eLIBs_fioctrl(disp_handle, DISP_CMD_LAYER_GET_FB, SEL_SCREEN, (void *)pdispbuffer);
	GUI_Dispfb2Fb(&disp_fb_para, &cur_fb);

	pdispbuffer[0] = hlayer;
	pdispbuffer[1] = (__u32)&scn_rect;
	pdispbuffer[2] = 0;
    eLIBs_fioctrl(disp_handle, DISP_CMD_LAYER_GET_SCN_WINDOW, SEL_SCREEN, (void *)pdispbuffer);

	
	fb_width = cur_fb.size.width;
	fb_height = cur_fb.size.height;

	if(dir==DIR_CLOCKWISE_0 || dir==DIR_CLOCKWISE_180)
	{
		rect->width     = scn_rect.width;
		rect->height    = scn_rect.height;
	}
	else
	{
		rect->width     = scn_rect.height;
		rect->height    = scn_rect.width;
	}
	GUI_LogicToPhysicalConvert(dir,rect,&src_rect_phy,fb_width,fb_height);

	pdispbuffer[0] = hlayer;
	pdispbuffer[1] = (__u32)&src_rect_phy;
	pdispbuffer[2] = 0;
	ret |= eLIBs_fioctrl(disp_handle, DISP_CMD_LAYER_SET_SRC_WINDOW, SEL_SCREEN, (void *)pdispbuffer);//end modified 2010-12-9 15:40:35
    if(ret != EPDK_OK)
    {
        return EPDK_FAIL;
    }
    return EPDK_OK;
}

//==================================================================
//function name��   GUI_LogicLayerGetSrcWin
//author��   	danling
//date��            2010-03-30
//description��  ��ȡ�߼��ϵ�Դ����
//parameters��   dir: ����
//				disp_handle: ��ʾ�����ļ�ָ��
//				hlayer: ͼ����
//				rect: �߼�Դ����
//return��          success returns EPDK_OK
//                  	fail  returns the number of failed
//modify history��
//==================================================================

__s32 GUI_LogicLayerGetSrcWin(__u8 dir,ES_FILE *disp_handle,__hdle hlayer,__rect_t *rect)
{
	__u32 fb_width=0,fb_height=0;
	__rect_t src_rect_phy;
	__disp_layer_para_t cur_player;
	FB	        fb;
	__rect_t    scn_rect;

	/*modified by Derek,2010-12-9 18:33:55*/
	__disp_layer_info_t disp_src_lyr={0};
	__u32 pdispbuffer[3];
	
	cur_player.fb = &fb;
	
	pdispbuffer[0] = hlayer;
	pdispbuffer[1] = (__u32)&disp_src_lyr;
	pdispbuffer[2] = 0;
	eLIBs_fioctrl(disp_handle, DISP_CMD_LAYER_GET_PARA, SEL_SCREEN, (void *)pdispbuffer);

	GUI_DispLyr2LogicLyr(&cur_player, &disp_src_lyr); //end modified
	
	fb_width = fb.size.width;
	fb_height = fb.size.height;

	src_rect_phy.x = cur_player.src_win.x;
	src_rect_phy.y = cur_player.src_win.y;
	src_rect_phy.width = cur_player.src_win.width;
	src_rect_phy.width = cur_player.src_win.width;

	if(cur_player.mode != DISP_LAYER_WORK_MODE_SCALER)
	{
		src_rect_phy.width = cur_player.scn_win.width;
		src_rect_phy.height = cur_player.scn_win.height;
	}
	
	GUI_PhysicalToLogicConvert(dir,rect,&src_rect_phy,fb_width,fb_height);

	return EPDK_OK;
}

//==================================================================
//function name��   GUI_LogicLayerSetPara
//author��   	danling
//date��            2010-03-30
//description��   ���߼��ϵ�����ͼ�����ת������Ӧ���������
//parameters��   dir: ����
//				disp_handle: ��ʾ�����ļ�ָ��
//				hlayer: ͼ����
//				player: �߼�ͼ�����
//return��          success returns EPDK_OK
//                  	fail  returns the number of failed
//modify history��
//==================================================================
__s32 GUI_LogicLayerSetPara(__u8 dir,ES_FILE *disp_handle,__hdle hlayer,__disp_layer_para_t * player)
{
	__u32               ret = 0;
	__u32               fb_width=0,fb_height=0;
	__u32               scn_width=0,scn_height=0;
	__disp_layer_para_t player_phy={0};
	FB	                fb={0};

	__disp_layer_info_t disp_lyr={0}; //added by Derek,2010-12-9 17:02:36
	__u32 pdispbuffer[3];

	eLIBs_memcpy(&player_phy,player,sizeof(__disp_layer_para_t));
	eLIBs_memcpy(&fb,player->fb,sizeof(FB));
	player_phy.fb = &fb;

	if(dir==DIR_CLOCKWISE_0 || dir==DIR_CLOCKWISE_180)
	{
		fb_width = player->fb->size.width;
		fb_height = player->fb->size.height;
	}
	else
	{
		fb_width = player->fb->size.height;
		fb_height = player->fb->size.width;
	}
	scn_width = eLIBs_fioctrl(disp_handle,DISP_CMD_SCN_GET_WIDTH, SEL_SCREEN,0); //modified by Derek,2010.12.07.15:08
	scn_height = eLIBs_fioctrl(disp_handle,DISP_CMD_SCN_GET_HEIGHT, SEL_SCREEN,0); //modified by Derek,2010.12.07.15:08

	player->src_win.width     = player->scn_win.width;
	player->src_win.height    = player->scn_win.height;
	GUI_LogicToPhysicalConvert(dir,&(player->src_win),&(player_phy.src_win),fb_width,fb_height);
	GUI_LogicToPhysicalConvert(dir,&(player->scn_win),&(player_phy.scn_win),scn_width,scn_height);
	player_phy.fb->size.width = fb_width;
	player_phy.fb->size.height = fb_height;

	GUI_LogicLyr2DispLyr(&player_phy, &disp_lyr); //modified by Derek,2010-12-9 17:07:38
	pdispbuffer[0] = hlayer;
	pdispbuffer[1] = (__u32)&disp_lyr;
	pdispbuffer[2] = 0;
	ret = eLIBs_fioctrl(disp_handle, DISP_CMD_LAYER_SET_PARA, SEL_SCREEN, (void *)pdispbuffer); //end modified
	if(ret != EPDK_OK)
	{
		return EPDK_FAIL;
	}
	return EPDK_OK;
}

//==================================================================
//function name��   GUI_LogicLayerGetPara
//author��   	danling
//date��            2010-03-30
//description��   ��ȡ�߼��ϵ�ͼ�����
//parameters��   dir: ����
//				disp_handle: ��ʾ�����ļ�ָ��
//				hlayer: ͼ����
//				player: �߼�ͼ�����,���
//return��          success returns EPDK_OK
//                  	fail  returns the number of failed
//modify history��
//==================================================================
__s32 GUI_LogicLayerGetPara(__u8 dir,ES_FILE *disp_handle,__hdle hlayer,__disp_layer_para_t * player)
{
	__disp_layer_para_t cur_player;
	FB fb;
	__u32 scn_width=0,scn_height=0;

	/*modified by Derek,2010-12-9 18:43:49*/
	__u32 dispbuffer[3];
	__disp_layer_info_t disp_src_lyr={0};

	cur_player.fb = &fb;

	dispbuffer[0] = hlayer;
	dispbuffer[1] = (__s32)&disp_src_lyr;
	dispbuffer[2] = 0;
	eLIBs_fioctrl(disp_handle, DISP_CMD_LAYER_GET_PARA, SEL_SCREEN, (void *)dispbuffer);
	GUI_DispLyr2LogicLyr(&cur_player, &disp_src_lyr); //end modified
	
	eLIBs_memcpy(player,&cur_player,sizeof(__disp_layer_para_t));
	eLIBs_memcpy(player->fb,&fb,sizeof(FB));

	scn_width = eLIBs_fioctrl(disp_handle,DISP_CMD_SCN_GET_WIDTH, SEL_SCREEN,0); //modified by Derek,2010.12.07.15:08
	scn_height = eLIBs_fioctrl(disp_handle,DISP_CMD_SCN_GET_HEIGHT, SEL_SCREEN,0); //modified by Derek,2010.12.07.15:08

	if(cur_player.mode != DISP_LAYER_WORK_MODE_SCALER)
	{
		cur_player.src_win.width = cur_player.scn_win.width;
		cur_player.src_win.height = cur_player.scn_win.height;
	}
	GUI_PhysicalToLogicConvert(dir,&(player->src_win),&(cur_player.src_win),fb.size.width,fb.size.height);
	GUI_PhysicalToLogicConvert(dir,&(player->scn_win),&(cur_player.scn_win),scn_width,scn_height);

	if(dir==DIR_CLOCKWISE_0 || dir==DIR_CLOCKWISE_180)
	{
		player->fb->size.width = fb.size.width;
		player->fb->size.height = fb.size.height;
	}
	else
	{
		player->fb->size.width = fb.size.height;
		player->fb->size.height = fb.size.width;
	}

	return EPDK_OK;
}

//==================================================================
//function name��   GUI_LogicLayerRequest
//author��   	danling
//date��            2010-03-30
//description��   ���߼��ϵ�����ͼ��ת������Ӧ���������
//parameters��   dir: ����
//				disp_handle: ��ʾ�����ļ�ָ��
//				hlayer: ͼ����
//				player: ����ͼ����߼�����
//return��          success returns EPDK_OK
//                  	fail  returns the number of failed
//modify history��
//==================================================================
__s32 GUI_LogicLayerRequest(__u8 dir,ES_FILE *disp_handle,__hdle hlayer,__disp_layer_para_t * player)
{
	__u32 ret = 0;
	__u32 pbuffer[3]; //modified by Derek,2010.12.07.15:08
	__hdle layer_hdl;

	if(player != NULL)
	{
		__u32 fb_width=0,fb_height=0;
		__u32 scn_width=0,scn_height=0;
		__disp_layer_para_t player_phy={0};
		FB	fb;

		__disp_layer_info_t disp_layer={0};
		

		eLIBs_memcpy(&player_phy,player,sizeof(__disp_layer_para_t));
		eLIBs_memcpy(&fb,player->fb,sizeof(FB));
		player_phy.fb = &fb;

		if(dir==DIR_CLOCKWISE_0 || dir==DIR_CLOCKWISE_180)
		{
			fb_width = player->fb->size.width;
			fb_height = player->fb->size.height;
		}
		else
		{
			fb_width = player->fb->size.height;
			fb_height = player->fb->size.width;
		}
		scn_width = eLIBs_fioctrl(disp_handle,DISP_CMD_SCN_GET_WIDTH, SEL_SCREEN,0); //modified by Derek,2010.12.07.15:08
		scn_height = eLIBs_fioctrl(disp_handle,DISP_CMD_SCN_GET_HEIGHT, SEL_SCREEN,0); //modified by Derek,2010.12.07.15:08

		player->src_win.width     = player->scn_win.width;
		player->src_win.height    = player->scn_win.height;
		GUI_LogicToPhysicalConvert(dir,&(player->src_win),&(player_phy.src_win),fb_width,fb_height);
		GUI_LogicToPhysicalConvert(dir,&(player->scn_win),&(player_phy.scn_win),scn_width,scn_height);
		player_phy.fb->size.width = fb_width;
		player_phy.fb->size.height = fb_height;

		pbuffer[0] = player_phy.mode; //modified by Derek,2010.12.07.15:08
		pbuffer[1] = 0;
		pbuffer[2] = 0;
		layer_hdl = eLIBs_fioctrl(disp_handle, DISP_CMD_LAYER_REQUEST, SEL_SCREEN, (void *)pbuffer); //modified by Derek,2010.12.07.15:08

		GUI_LogicLyr2DispLyr(&player_phy, &disp_layer);
		pbuffer[0] = layer_hdl; //modified by Derek,2010.12.07.15:08
		pbuffer[1] = (__u32)&disp_layer;
		pbuffer[2] = 0;
		ret = eLIBs_fioctrl(disp_handle, DISP_CMD_LAYER_SET_PARA, SEL_SCREEN, (void *)pbuffer); //modified by Derek,2010.12.07.15:08
		if(ret != 0)
		{
			__wrn("set layer para fail\n");
			return NULL;
		}
	}	
	else
	{
		pbuffer[0] = DISP_LAYER_WORK_MODE_NORMAL; //modified by Derek,2010.12.07.15:08
		pbuffer[1] = 0;
		pbuffer[2] = 0;
		layer_hdl = eLIBs_fioctrl(disp_handle, DISP_CMD_LAYER_REQUEST, SEL_SCREEN, (void *)pbuffer); //modified by Derek,2010.12.07.15:08
	}

	return layer_hdl;
}



//==================================================================
//function name��   GUI_LogicLayerSetFb
//author��   	danling
//date��            2010-03-30
//description��   ���߼��ϵ�����framebuffer ת������Ӧ���������
//parameters��   dir: ����
//				disp_handle: ��ʾ�����ļ�ָ��
//				hlayer: ͼ����
//				pfb: �߼��ϵ�framebuffer����
//return��          success returns EPDK_OK
//                  	fail  returns the number of failed
//modify history��
//==================================================================
__s32 GUI_LogicLayerSetFb(__u8 dir,ES_FILE *disp_handle,__hdle hlayer, FB * pfb)
{
	__u32 ret = 0;
	FB pfb_phy;
	__u32 pdispbuffer[3];
	__disp_fb_t disp_fb_para={0};

	eLIBs_memcpy(&pfb_phy,pfb,sizeof(FB));

	switch (dir)
	{
	case DIR_CLOCKWISE_90:
	case DIR_CLOCKWISE_270:
		pfb_phy.size.width = pfb->size.height;
		pfb_phy.size.height = pfb->size.width;		
		break;
	default:
		break;
	}

	GUI_Fb2Dispfb(&pfb_phy, &disp_fb_para);

	pdispbuffer[0] = hlayer;
	pdispbuffer[1] = (__u32)&disp_fb_para;
	pdispbuffer[2] = 0;

	ret = eLIBs_fioctrl(disp_handle, DISP_CMD_LAYER_SET_FB, SEL_SCREEN, (void *)pdispbuffer);
	if(ret != EPDK_OK)
	{
		return EPDK_FAIL;
	}
	return EPDK_OK;
}

//==================================================================
//function name��   GUI_LogicLayerGetFb
//author��   	danling
//date��            2010-04-01
//description��   ��ȡ�߼��ϵ�framebuffer��Ϣ
//parameters��   dir: ����
//				disp_handle: ��ʾ�����ļ�ָ��
//				hlayer: ͼ����
//				pfb: �߼��ϵ�framebuffer����,���
//return��          success returns EPDK_OK
//                  	fail  returns the number of failed
//modify history��
//==================================================================
__s32 GUI_LogicLayerGetFb(__u8 dir,ES_FILE *disp_handle,__hdle hlayer, FB * pfb)
{
	FB cur_fb;

	/*modified by Derek,2010-12-9 15:51:43*/
	__u32 pdispbuffer[3];
	__disp_fb_t disp_fb_para={0};

	pdispbuffer[0] = hlayer;
	pdispbuffer[1] = (__u32)&disp_fb_para;
	pdispbuffer[2] = 0;
	
	eLIBs_fioctrl(disp_handle, DISP_CMD_LAYER_GET_FB, SEL_SCREEN, (void *)pdispbuffer);
	GUI_Dispfb2Fb(&disp_fb_para, &cur_fb); //end modified
	
	eLIBs_memcpy(pfb,&cur_fb,sizeof(FB));
	
	switch (dir)
	{
	case DIR_CLOCKWISE_90:
	case DIR_CLOCKWISE_270:
		pfb->size.width = cur_fb.size.height;
		pfb->size.height = cur_fb.size.width;		
		break;
	default:
		break;
	}
	return EPDK_OK;
}

