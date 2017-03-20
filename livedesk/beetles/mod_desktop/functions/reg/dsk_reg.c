
#include "..\\functions_i.h"

#define REG_SETTING_PARA_FILE_PATH 			"e:\\setting.bin"

#define REG_DEFAULT_BGD_PATH   BEETLES_APP_ROOT"apps\\"

#define REG_VERSION_INFO "REG1"

#define REG_VERSION_INFO_LEN (eLIBs_strlen(REG_VERSION_INFO))

typedef struct
{	
	reg_system_para_t	system_para;
	reg_init_para_t	    init_para;			
	reg_root_para_t	    root_para;
	reg_movie_para_t	movie_para;
	reg_music_para_t	music_para;
	reg_photo_para_t	photo_para;
	reg_fm_para_t		fm_para;
	reg_ebook_para_t	ebook_para;
	reg_record_para_t   record_para;
	//reg_calendar_para_t calendar_para;    
	//reg_dv_para_t		dv_para ;	

}reg_app_para_t;

typedef struct  
{
	__u32   restore_flag[REG_APP_MAX];
	reg_app_para_t para_current;//��ǰֵ
	reg_app_para_t para_default;//����Ĭ��ֵ
	__u32   reserverd[4]; //��������
}reg_all_para_t;

static reg_all_para_t*	all_app_para = NULL;

#if 1
//��ȡ�����ļ�Ĭ������
static __s32 __reg_paramter_config_init(reg_app_para_t* para)
{    

    if (!para)
	{
		__wrn("invalid para...\n");
		return EPDK_FAIL;
	}
    __here__;
    {//init key val by app_config.fex
        char* path ="d:\\app_config.bin";
        H_PARSER parser;
        __s32 ret;
        __s32 i;
        char keyname[64];
        int val;
        char str_val[32];
        
        do{
            parser = NULL;

            parser = esCFG_Init_Ex(path);
            if(NULL == parser)
            {
                __wrn("esCFG_Init_Ex fail...\n");
                break;
            }
			__here__;

            __wrn("before esCFG_GetKeyValue_Ex\n");

            eLIBs_strcpy(keyname, "system");
            eLIBs_strcat(keyname, "/");
            eLIBs_strcat(keyname, "volume");                        
            ret = esCFG_GetKeyValue_Ex(parser, keyname, (int*)&val, 1);
            __wrn("after esCFG_GetKeyValue_Ex, ret=%d\n", ret);
            if(EPDK_OK == ret)
            {
                if(val < AUDIO_DEVICE_VOLUME_MIN || val > AUDIO_DEVICE_VOLUME_MAX)
                {
                    val = 26;
                }
                
                para->system_para.volume = val;
                __wrn("default volume=%d\n", para->system_para.volume);
            }                                          

            __here__;
            
            eLIBs_strcpy(keyname, "system");
            eLIBs_strcat(keyname, "/");
            eLIBs_strcat(keyname, "lang");                        
            ret = esCFG_GetKeyValue_Ex(parser, keyname, (int*)&val, 1);
            if(EPDK_OK == ret)
            {
                if(EPDK_LANGUAGE_ENM_ENGLISH != val
                    &&EPDK_LANGUAGE_ENM_CHINESES!= val
                    &&EPDK_LANGUAGE_ENM_THAI!= val)
                {
                    val = EPDK_LANGUAGE_ENM_ENGLISH;
                }
                
                para->system_para.language= val;
                __wrn("default lang=0x%x\n", para->system_para.language);
            }     
            
            eLIBs_strcpy(keyname, "system");
            eLIBs_strcat(keyname, "/");
            eLIBs_strcat(keyname, "style");                        
            ret = esCFG_GetKeyValue_Ex(parser, keyname, (int*)&val, 1);
            if(EPDK_OK == ret)
            {
                if(val < 0)
                {
                    val = 0;
                }
                
                para->init_para.default_bgd_index = val;
                __wrn("default style=0x%d\n", para->system_para.style);
            }     

            eLIBs_strcpy(keyname, "system");
            eLIBs_strcat(keyname, "/");
            eLIBs_strcat(keyname, "light");                        
            ret = esCFG_GetKeyValue_Ex(parser, keyname, (int*)&val, 1);
            if(EPDK_OK == ret)
            {
                if(val < LION_BRIGHT_LEVEL1 || val > LION_BRIGHT_LEVEL16)
                {
                    val = LION_BRIGHT_LEVEL12;
                }
                
                para->system_para.backlight = val;
                __wrn("default light=%d\n", para->system_para.backlight);
            }     

        	////bright	
        	eLIBs_strcpy(keyname, "system");
        	eLIBs_strcat(keyname, "/");
        	eLIBs_strcat(keyname, "bright");
        	ret = esCFG_GetKeyValue_Ex(parser, keyname, (int*)&val, 1);
                if(EPDK_OK == ret)
        		para->system_para.brightness = val;

        	////contrast	
        	eLIBs_strcpy(keyname, "system");
        	eLIBs_strcat(keyname, "/");
        	eLIBs_strcat(keyname, "contrast");
        	ret = esCFG_GetKeyValue_Ex(parser, keyname, (int*)&val, 1);
                if(EPDK_OK == ret)
        		para->system_para.constract= val;

        	////saturation	
        	eLIBs_strcpy(keyname, "system");
        	eLIBs_strcat(keyname, "/");
        	eLIBs_strcat(keyname, "saturation");
        	ret = esCFG_GetKeyValue_Ex(parser, keyname, (int*)&val, 1);
                if(EPDK_OK == ret)
        		para->system_para.saturation = val;

        	////hue
        	eLIBs_strcpy(keyname, "system");
        	eLIBs_strcat(keyname, "/");
        	eLIBs_strcat(keyname, "hue");
        	ret = esCFG_GetKeyValue_Ex(parser, keyname, (int*)&val, 1);
                if(EPDK_OK == ret)
        		para->system_para.hue = val;

            eLIBs_strcpy(keyname, "system");
            eLIBs_strcat(keyname, "/");
            eLIBs_strcat(keyname, "power_off");                        
            ret = esCFG_GetKeyValue_Ex(parser, keyname, (int*)&val, 1);
            if(EPDK_OK == ret)
            {
                if(0 != val
                    &&3*60 != val
                    &&5*60 != val
                    &&10*60 != val
                    &&15*60 != val)
                {
                    val = 3*60;
                }
                
                para->system_para.poweroff = val;
                __wrn("default poweroff=%d\n", para->system_para.poweroff);
            } 

            eLIBs_strcpy(keyname, "system");
            eLIBs_strcat(keyname, "/");
            eLIBs_strcat(keyname, "keytone");                        
            ret = esCFG_GetKeyValue_Ex(parser, keyname, (int*)&val, 1);
            if(EPDK_OK == ret)
            {
                if(val < 0 || val > 1)
                {
                    val = 0;
                }
                
                para->system_para.keytone = val;
                __wrn("default keytone=%d\n", para->system_para.keytone);
            }    

            eLIBs_strcpy(keyname, "system");
            eLIBs_strcat(keyname, "/");
            eLIBs_strcat(keyname, "light_off");                        
            ret = esCFG_GetKeyValue_Ex(parser, keyname, (int*)&val, 1);
            if(EPDK_OK == ret)
            {
                if(0 != val
                    &&10 != val
                    &&30 != val
                    &&60 != val
                    &&90 != val)
                {
                    val = 60;
                }
                
                para->system_para.backlightoff= val;
                __wrn("default backlightoff=%d\n", para->system_para.backlightoff);
            } 

            eLIBs_strcpy(keyname, "system");
            eLIBs_strcat(keyname, "/");
            eLIBs_strcat(keyname, "cvbs");                        
            ret = esCFG_GetKeyValue_Ex(parser, keyname, (int*)&val, 1);
            if(EPDK_OK == ret)
            {
                if(val < 0 || val > 1)
                {
                    val = 0;
                }
                
                para->system_para.pal_ntsc = val;
                __wrn("default cvbs=%d\n", para->system_para.pal_ntsc);
            }            
        }while(0);

        if(parser)
        {
            esCFG_Exit_Ex(parser);
            parser = NULL;
        }
    }
    
    return EPDK_OK;
}
#endif

static __s32 __reg_paramter_init(reg_app_para_t* para)
{
	if (!para)
	{
		__wrn("invalid para...\n");
		return EPDK_FAIL;
	}	

	para->system_para.audio_output = 0;//normal
	para->system_para.channel=0;//������
	para->system_para.charset=5;//gbk
	para->system_para.detect_gate=1;//on
	para->system_para.volume=30;
	para->system_para.language= EPDK_LANGUAGE_ENM_ENGLISH;//EPDK_LANGUAGE_ENM_CHINESES;//��������
	para->system_para.style=0;
	para->system_para.backlight=1;
	para->system_para.backlightoff=0;//���Զ�����
	para->system_para.poweroff=0;//���Զ��ػ�
	para->system_para.keytone=1;//�������Ƿ��
	para->system_para.output=LION_DISP_LCD;
    para->system_para.pal_ntsc=1;//0����ntsc, 1����pal
    para->system_para.time_set = 0;
	para->system_para.date.year= 2014;
	para->system_para.date.month= 1;
	para->system_para.date.day = 1;
	para->system_para.time.hour= 12;
	para->system_para.time.minute= 0;
	para->system_para.time.second= 0;
	para->system_para.tp_adjust_finish_flag = 0;

	para->init_para.default_bgd_index=0;
	eLIBs_strcpy(para->init_para.default_bgd_path,REG_DEFAULT_BGD_PATH);
	eLIBs_strcpy(para->init_para.setting_bgd_path, "");
	para->init_para.bgd_flag=1;//Ĭ�ϱ���
	para->init_para.bgd_flag_in_main = 1;//Ĭ�ϱ���

	//   para->root_para.last_app_name;
	para->root_para.last_movie_index_sd=0;
	//  para->root_para.last_movie_path_sd;
	para->root_para.last_movie_index_ud=0;
	//  para->root_para.last_movie_path_ud;
	para->root_para.last_music_index_sd=0;
	//  para->root_para.last_music_path_sd;
	para->root_para.last_music_index_ud=0;
	//  para->root_para.last_music_path_ud;
	para->root_para.last_photo_index_sd=0;
	//  para->root_para.last_photo_path_sd;
	para->root_para.last_photo_index_ud=0;
	//  para->root_para.last_photo_path_ud;
	para->root_para.last_ebook_index_sd=0;
	//  para->root_para.last_ebook_path_sd;
	para->root_para.last_ebook_index_ud=0;
	//  para->root_para.last_ebook_path_ud =0;

	para->movie_para.rotate_mode=2;//ROTATE_ALL
	para->movie_para.zoom_mode=1;//VIDEO_RATIO
	para->movie_para.sub_state=1;//on
	para->movie_para.sub_color=GUI_WHITE;
    para->movie_para.sub_stream=0;
	para->movie_para.sub_pos=0;//����

	para->music_para.start_item_id=0;
	para->music_para.focus_item_id=0;
	para->music_para.play_mode=2;//ROTATE_ALL
	para->music_para.EQ_mode=0;//NORMAL

	para->photo_para.bgmusic=0;//�ر�
	para->photo_para.speedflag=0;
	para->photo_para.interval_time=6;
	para->photo_para.ratio=0;
	para->photo_para.effect=0;//Ĭ�����л�Ч��

	para->ebook_para.text_colour=GUI_WHITE;
	para->ebook_para.switch_time=10;
	para->ebook_para.switch_time_flag=0;
	para->ebook_para.bg_music_switch=0;
	para->ebook_para.tts_switch = 0;			//Ĭ��TTS �Ǵ�״̬

	para->fm_para.mode=0;
	para->fm_para.channel_id=0;          

    //para->ebook_para
    //para->calendar_para.time_format=0;

    {
        __s32 ret;

        ret = __reg_paramter_config_init(para);
        if(EPDK_FAIL == ret)
        {
            return EPDK_FAIL;
        }
    }
	
	return EPDK_OK;
}

__s32 dsk_reg_init_para(void)
{
	//�����ڴ�
	{		
		if (all_app_para)
		{
			__msg("para already init...\n");
			return EPDK_OK;
		}

		all_app_para = esMEMS_Balloc(sizeof(reg_all_para_t));
		if (!all_app_para)
		{
			__msg("mem not enough...\n");
			return EPDK_FAIL;
		}
	}

	//���Լ����ļ�
	{
		ES_FILE* fp;

		fp = eLIBs_fopen(REG_SETTING_PARA_FILE_PATH, "rb");
		if(fp == NULL)
    	{
    		eLIBs_format("e:\\","fat",0);
			fp = eLIBs_fopen(REG_SETTING_PARA_FILE_PATH, "rb");			
    	}

		if (fp)
		{
			__s32 file_len;
			__s32 read_num;

            __msg("file exist:%s\n", REG_SETTING_PARA_FILE_PATH);
			eLIBs_fseek(fp, 0, SEEK_END);
			file_len = eLIBs_ftell(fp);
			if (file_len == sizeof(reg_all_para_t)+REG_VERSION_INFO_LEN)
			{
				char version[5]={0};
				eLIBs_fseek(fp, 0, SEEK_SET);	
                    	            __here__;

				read_num = eLIBs_fread(version, 1, REG_VERSION_INFO_LEN, fp);
				if (REG_VERSION_INFO_LEN == read_num)
				{
                        	            __here__;

					if (0 == eLIBs_strncmp(version, REG_VERSION_INFO, REG_VERSION_INFO_LEN))
					{
                            	            __here__;

						read_num = eLIBs_fread(all_app_para, 1, sizeof(reg_all_para_t), fp);
						if(sizeof(reg_all_para_t) == read_num)
						{	
                            
							eLIBs_fclose(fp);
							__msg("load reg para success...\n");				
							return EPDK_OK;
						}	
					}	
					else
					{
						__msg("reg version info incorrect...\n");
					}
				}
				else
				{					
					__msg("read file fail:%s\n", REG_SETTING_PARA_FILE_PATH);									
				}				
			}
    	            __here__;

			eLIBs_fclose(fp);
		}
        else
        {
            __msg("file not exist:%s\n", REG_SETTING_PARA_FILE_PATH);
        }
	}

    __here__;

	//�ļ������ڻ��С���ԣ�������д��
	//��ʼ������
	{
		__s32 sta;
		__s32 ret;		

		eLIBs_memset(all_app_para, 0, sizeof(reg_all_para_t));
		
		ret = EPDK_OK;
		sta =__reg_paramter_init(&all_app_para->para_current);
		if (EPDK_FAIL == sta)
		{
			__msg("__reg_paramter_init fail...\n");
			ret = EPDK_FAIL;
		}
		
		sta =__reg_paramter_init(&all_app_para->para_default);
		if (EPDK_FAIL == sta)
		{
			__msg("__reg_paramter_init fail...\n");
			ret = EPDK_FAIL;
		}
		
		if (EPDK_FAIL == ret)
		{
			return EPDK_FAIL;
		}		
	}   
#if 1
	//д���ļ�
	{//
		ES_FILE* fp;
		__s32 write_num;


		fp = eLIBs_fopen(REG_SETTING_PARA_FILE_PATH, "wb");
		if (!fp)
		{
			__msg("open file fail:%s\n", REG_SETTING_PARA_FILE_PATH);
			return EPDK_FAIL;
		}

		write_num = eLIBs_fwrite(REG_VERSION_INFO, 1, REG_VERSION_INFO_LEN, fp);
		if (write_num != REG_VERSION_INFO_LEN)
		{
			eLIBs_fclose(fp);
			__wrn("eLIBs_fwrite fail:%s\n", REG_SETTING_PARA_FILE_PATH);
			return EPDK_FAIL;
		}
		
		write_num = eLIBs_fwrite(all_app_para, 1, sizeof(reg_all_para_t), fp);
		if (write_num != sizeof(reg_all_para_t))
		{
			eLIBs_fclose(fp);
			__wrn("eLIBs_fwrite fail:%s\n", REG_SETTING_PARA_FILE_PATH);
			return EPDK_FAIL;
		}
		else
		{
			__msg("write para success...\n");
			eLIBs_fclose(fp);
			return EPDK_OK;
		}
	}		    
#endif
	return EPDK_OK;
}

__s32 dsk_reg_deinit_para(void)
{
	if (all_app_para)
	{
		esMEMS_Bfree(all_app_para, sizeof(reg_all_para_t));
		all_app_para = NULL;
	}

	return EPDK_OK;
}

__s32 dsk_reg_flush(void)
{
	__s32 result;
	ES_FILE* fp = NULL;
    
    if(1)//
    {
    	if (!all_app_para)
    	{
    		__wrn("reg para not init...\n");
    		return EPDK_FALSE;
    	}
    	
    	fp = eLIBs_fopen(REG_SETTING_PARA_FILE_PATH, "wb");
    	if(fp == NULL)
    	{
    		eLIBs_format("e:\\","fat",0);
			fp = eLIBs_fopen(REG_SETTING_PARA_FILE_PATH, "wb");
			if(fp == NULL)
			{
    			__msg("2open file fail:%s\n", REG_SETTING_PARA_FILE_PATH);
    			return EPDK_FAIL;
			}
    	}

    	result = eLIBs_fwrite(REG_VERSION_INFO, 1, REG_VERSION_INFO_LEN, fp);
    	if (result != REG_VERSION_INFO_LEN)
    	{
    		eLIBs_fclose(fp);
    		__wrn("eLIBs_fwrite fail:%s\n", REG_SETTING_PARA_FILE_PATH);
    		return EPDK_FAIL;
    	}
    	
    	result = eLIBs_fwrite(all_app_para, 1, sizeof(reg_all_para_t), fp);
    	if(result < sizeof(reg_all_para_t))
    	{
            eLIBs_fclose(fp);
    		__msg("write file fail:%s\n", REG_SETTING_PARA_FILE_PATH);
    		return EPDK_FAIL;
    	}
    	
    	eLIBs_fclose(fp);
		

        {//
            ES_FILE* hFile;
            
            //flush user disk data to NOR flash
            hFile = eLIBs_fopen("b:\\STORAGE\\SPINOR", "rb");
            if (hFile == NULL)
            {
                __wrn("open NOR file failed\n");
                return EPDK_FAIL;
            }
            __msg("flush file ....\n");
            eLIBs_fioctrl(hFile, DEV_IOC_USR_FLUSH_CACHE, 0, 0);
            
            __msg("flush end ....\n");
            eLIBs_fclose(hFile);
        }
    }  

	return EPDK_OK;
}

void* dsk_reg_get_para_by_app(reg_app_e eApp)
{
	if (!all_app_para)
	{
		__wrn("reg para not init...\n");
		return EPDK_FALSE;
	}

	switch(eApp)
	{
		case REG_APP_SYSTEM:
			return &all_app_para->para_current.system_para;		
		case REG_APP_INIT:
			return &all_app_para->para_current.init_para;		
		case REG_APP_ROOT:
			return &all_app_para->para_current.root_para;		
		case REG_APP_MOVIE:
			return &all_app_para->para_current.movie_para;		
		case REG_APP_MUSIC:
			return &all_app_para->para_current.music_para;		
		case REG_APP_PHOTO:
			return &all_app_para->para_current.photo_para;		
		case REG_APP_EBOOK:
			return &all_app_para->para_current.ebook_para;		
		case REG_APP_FM:
			return &all_app_para->para_current.fm_para;		
		case REG_APP_RECORD:
			return &all_app_para->para_current.record_para;		
		//case REG_APP_CALENDAR:
		//	return &all_app_para->para_current.calendar_para;
		//case REG_APP_DV:
		//	return &all_app_para->para_current.dv_para;
			
#if 0//EXTERN_KARAOKE_EXIST //add by hot.lee
		case REG_APP_KARAOKE:
			return &all_app_para->para_current.karaoke_para;			
#endif			

	}

	__wrn("invalid para...\n");
	return NULL;
}

void* dsk_reg_get_default_para_by_app(reg_app_e eApp)
{
	if (!all_app_para)
	{
		__wrn("reg para not init...\n");
		return EPDK_FALSE;
	}

	switch(eApp)
	{
	case REG_APP_SYSTEM:
		return &all_app_para->para_default.system_para;		
	case REG_APP_INIT:
		return &all_app_para->para_default.init_para;		
	case REG_APP_ROOT:
		return &all_app_para->para_default.root_para;		
	case REG_APP_MOVIE:
		return &all_app_para->para_default.movie_para;		
	case REG_APP_MUSIC:
		return &all_app_para->para_default.music_para;		
	case REG_APP_PHOTO:
		return &all_app_para->para_default.photo_para;		
	case REG_APP_EBOOK:
		return &all_app_para->para_default.ebook_para;		
	case REG_APP_FM:
		return &all_app_para->para_default.fm_para;		
	case REG_APP_RECORD:
		return &all_app_para->para_default.record_para;		
	//case REG_APP_CALENDAR:
	//	return &all_app_para->para_default.calendar_para;
	//case REG_APP_DV:
	//	return &all_app_para->para_default.dv_para;
		
#if 0//EXTERN_KARAOKE_EXIST //add by hot.lee
	case REG_APP_KARAOKE:
		return &all_app_para->para_default.karaoke_para;			
#endif		
	}
	
	__wrn("invalid para...\n");
	return NULL;
}

__bool dsk_reg_get_app_restore_flag(reg_app_e eApp)
{
	if (!all_app_para)
	{
		__wrn("reg para not init...\n");
		return EPDK_FALSE;
	}

	if (eApp >= REG_APP_MAX)
	{
		__wrn("invalid para...\n");
		return NULL;
	}
	
	return (__bool)(all_app_para->restore_flag[eApp]);
}

__s32 dsk_reg_set_app_restore_flag(reg_app_e eApp, __bool flag)
{
	if (!all_app_para)
	{
		__wrn("reg para not init...\n");
		return EPDK_FALSE;
	}
	
	if (eApp >= REG_APP_MAX)
	{
		__wrn("invalid para...\n");
		return NULL;
	}
	
	all_app_para->restore_flag[eApp] = flag;

	return EPDK_OK;
}
__s32 dsk_reg_save_cur_play_info_t(reg_app_e eApp, __s32 index, char* filename, rat_root_t root_type,__u32 time)
{
	if (!all_app_para)
	{
		__err("reg para not init...\n");
		return EPDK_FAIL;
	}
	switch(eApp)
	{		
		case REG_APP_MOVIE:
		{
			if(RAT_TF == root_type)
			{
				all_app_para->para_current.root_para.last_movie_index_sd = index;
				eLIBs_strcpy(all_app_para->para_current.root_para.last_movie_path_sd, filename);
			}
			else if(RAT_USB == root_type)
			{
				all_app_para->para_current.root_para.last_movie_index_ud = index;
				eLIBs_strcpy(all_app_para->para_current.root_para.last_movie_path_ud, filename);
			}
			break;
		}
		case REG_APP_MUSIC:
		{
		__msg("--------save music para----------\n");
			if(RAT_TF == root_type)
			{
				all_app_para->para_current.root_para.last_music_index_sd = index;
				all_app_para->para_current.root_para.last_music_time_sd= time;
				eLIBs_strcpy(all_app_para->para_current.root_para.last_music_path_sd, filename);
			}
			else if(RAT_USB == root_type)
			{
				all_app_para->para_current.root_para.last_music_index_ud = index;
				all_app_para->para_current.root_para.last_music_time_ud= time;
				eLIBs_strcpy(all_app_para->para_current.root_para.last_music_path_ud, filename);
			}
			break;
		}	
		case REG_APP_PHOTO:
		{
			if(RAT_TF == root_type)
			{
				all_app_para->para_current.root_para.last_photo_index_sd = index;
				eLIBs_strcpy(all_app_para->para_current.root_para.last_photo_path_sd, filename);
			}
			else if(RAT_USB == root_type)
			{
				all_app_para->para_current.root_para.last_photo_index_ud = index;
				eLIBs_strcpy(all_app_para->para_current.root_para.last_photo_path_ud, filename);
			}
			break;
		}			
		case REG_APP_EBOOK:
		{
			if(RAT_TF == root_type)
			{
				all_app_para->para_current.root_para.last_ebook_index_sd = index;
				eLIBs_strcpy(all_app_para->para_current.root_para.last_ebook_path_sd, filename);
			}
			else if(RAT_USB == root_type)
			{
				all_app_para->para_current.root_para.last_ebook_index_ud = index;
				eLIBs_strcpy(all_app_para->para_current.root_para.last_ebook_path_ud, filename);
			}
			break;
		}
		default:
			__err("invalid app type...\n"); 		   
			return EPDK_FAIL;
	}
	return EPDK_OK;
}
__s32 dsk_reg_save_cur_play_info(reg_app_e eApp, __s32 index, char* filename, rat_root_t root_type)
{
    if (!all_app_para)
	{
		__wrn("reg para not init...\n");
		return EPDK_FAIL;
	}

	switch(eApp)
	{		
    	case REG_APP_MOVIE:
        {
            if(RAT_TF == root_type)
            {
                all_app_para->para_current.root_para.last_movie_index_sd = index;
                eLIBs_strcpy(all_app_para->para_current.root_para.last_movie_path_sd, filename);
            }
            else if(RAT_USB == root_type)
            {
                all_app_para->para_current.root_para.last_movie_index_ud = index;
                eLIBs_strcpy(all_app_para->para_current.root_para.last_movie_path_ud, filename);
            }
    		
            break;
    	}
    	case REG_APP_MUSIC:
    	{
            if(RAT_TF == root_type)
            {
                all_app_para->para_current.root_para.last_music_index_sd = index;
                eLIBs_strcpy(all_app_para->para_current.root_para.last_music_path_sd, filename);
            }
            else if(RAT_USB == root_type)
            {
                all_app_para->para_current.root_para.last_music_index_ud = index;
                eLIBs_strcpy(all_app_para->para_current.root_para.last_music_path_ud, filename);
            }
    		
            break;
    	}	
    	case REG_APP_PHOTO:
    	{
            if(RAT_TF == root_type)
            {
                all_app_para->para_current.root_para.last_photo_index_sd = index;
                eLIBs_strcpy(all_app_para->para_current.root_para.last_photo_path_sd, filename);
            }
            else if(RAT_USB == root_type)
            {
                all_app_para->para_current.root_para.last_photo_index_ud = index;
                eLIBs_strcpy(all_app_para->para_current.root_para.last_photo_path_ud, filename);
            }
    		
            break;
    	}			
    	case REG_APP_EBOOK:
    	{
            if(RAT_TF == root_type)
            {
                all_app_para->para_current.root_para.last_ebook_index_sd = index;
                eLIBs_strcpy(all_app_para->para_current.root_para.last_ebook_path_sd, filename);
            }
            else if(RAT_USB == root_type)
            {
                all_app_para->para_current.root_para.last_ebook_index_ud = index;
                eLIBs_strcpy(all_app_para->para_current.root_para.last_ebook_path_ud, filename);
            }
    		
            break;
    	}
#if 0//EXTERN_KARAOKE_EXIST  // add by hot.lee
		case REG_APP_KARAOKE:
		{
			__msg("--------save music para----------\n");
            if(RAT_TF == root_type)
            {
                all_app_para->para_current.root_para.last_krk_index_sd = index;
                eLIBs_strcpy(all_app_para->para_current.root_para.last_krk_path_sd, filename);
            }
            else if(RAT_USB == root_type)
            {
                all_app_para->para_current.root_para.last_krk_index_ud = index;
                eLIBs_strcpy(all_app_para->para_current.root_para.last_krk_path_ud, filename);
            }
			
			break;
		}

#endif		
        default:
            __wrn("invalid app type...\n");            
            return EPDK_FAIL;
	}
	    
    return EPDK_OK;
}


__s32 dsk_reg_set_app_restore_all(void)
{
    if (!all_app_para)
	{
		__wrn("reg para not init...\n");
		return EPDK_FAIL;
	}
	
	eLIBs_memcpy(&all_app_para->para_current,(const void*)(&all_app_para->para_default),sizeof(reg_app_para_t));
    return EPDK_OK;
}

