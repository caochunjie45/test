#include "record_list_view_doc.h"
#include "record_comm.h"

static __s32  record_list_GetSearchPath( precord_list_doc_t precord_list_doc )
{
	__s32 ret = 0;
	char root_type[50];
	char disk_name[RAT_MAX_PARTITION][4];

	eLIBs_memset(root_type, 0, sizeof(root_type));
	eLIBs_memset(disk_name, 0, sizeof(disk_name));
	eLIBs_memset(precord_list_doc->curSearchPath, 0, sizeof(precord_list_doc->curSearchPath));

	switch(precord_list_doc->curRootType)
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
	eLIBs_strcpy(precord_list_doc->curSearchPath,disk_name[0]);

    eLIBs_strcat(precord_list_doc->curSearchPath, RECORD_ROOT_PATH);

    __msg("disk_name[0]=%s\n", precord_list_doc->curSearchPath);
	return ret;
}


//Rat ��ʼ��
__s32 record_list_MusicInit( precord_list_doc_t precord_list_doc )
{
	if( record_list_GetSearchPath(precord_list_doc) == EPDK_FAIL )
		return EPDK_FAIL;

	precord_list_doc->total = record_file_num(precord_list_doc->curSearchPath, RECORD_LIST_FILTER);
	if( precord_list_doc->total == 0 )
	{
		__wrn("\n                     not audio file!                 \n");
		return EPDK_FAIL;
	}
	precord_list_doc->curIndex  = precord_list_doc->total-1;
	precord_list_doc->playIndex = precord_list_doc->total-1;
	precord_list_doc->topIndex  = precord_list_doc->total-RECORD_MUSIC_LIST_NUM;
	if( precord_list_doc->topIndex < 0 )
		precord_list_doc->topIndex = 0;

	eLIBs_memset(precord_list_doc->spctrmLevel, 0, sizeof(precord_list_doc->spctrmLevel));
	eLIBs_memset(precord_list_doc->oldSpctrmLevel, 0, sizeof(precord_list_doc->spctrmLevel));
	robin_open(ROBIN_MODE_AUDIO_MIN, NULL);
	robin_set_play_mode(RAT_PLAY_MODE_ONLY_ONCE);
	robin_enable_spectrum();
	robin_set_ff_rr_speed(CEDAR_FASTEST_FRSPEED);

	precord_list_doc->musicMaxTimes = 0;
	precord_list_doc->musicCurTimes = 0;


	return EPDK_OK;
}


//rat release
__s32 record_list_MusciUninit( precord_list_doc_t precord_list_doc )
{
	robin_disable_spectrum();

    robin_set_cmd_stop();
	robin_close();

	return EPDK_OK;
}

__s32 record_list_getFileName( precord_list_doc_t precord_list_doc, __s32 index, char* name)
{
	char fileName[50] = {0};

	if( index >= precord_list_doc->total || name == NULL )
		return EPDK_FAIL;

	record_get_fileName(precord_list_doc->curSearchPath, index, RECORD_LIST_FILTER,fileName);
	eLIBs_strcpy(name, (const char*)fileName );

	return EPDK_OK;
}
__s32 record_list_getFilePath(precord_list_doc_t precord_list_doc, __s32 index, char* name )
{
	char fileName[50] = {0};

	if( index >= precord_list_doc->total || name == NULL )
		return EPDK_FAIL;

	record_get_fileName(precord_list_doc->curSearchPath, index, RECORD_LIST_FILTER, fileName);
	eLIBs_strcpy(name, (const char*)precord_list_doc->curSearchPath);
	eLIBs_strcat(name, (const char*)fileName );

	return EPDK_OK;
}

__s32 record_list_play( precord_list_doc_t precord_list_doc )
{
	char 	file[50];

	precord_list_doc->playIndex = precord_list_doc->curIndex;

	record_list_getFilePath(precord_list_doc, precord_list_doc->curIndex, file);
	robin_set_cmd_stop();
	robin_set_cmd_play_file(file, NULL);
	robin_set_cmd_play();
	precord_list_doc->playSt = CEDAR_STAT_PLAY;

	return EPDK_OK;
}
__s32 record_list_playNext( precord_list_doc_t precord_list_doc )
{
	char 	file[50];
	__s32	tempIndex = precord_list_doc->playIndex;

	if( precord_list_doc->playIndex == precord_list_doc->total-1 )
	{
		precord_list_doc->playIndex = 0;
	}
	else
	{
		precord_list_doc->playIndex++;
	}
	if( tempIndex != precord_list_doc->curIndex )
	{
		precord_list_doc->curIndex = precord_list_doc->playIndex;
		if( precord_list_doc->curIndex + RECORD_MUSIC_LIST_NUM >= precord_list_doc->total )
		{
			precord_list_doc->topIndex = precord_list_doc->total-RECORD_MUSIC_LIST_NUM;
		}
		else
		{
			precord_list_doc->topIndex = precord_list_doc->playIndex;
		}
	}
	else
	{
		record_list_next(precord_list_doc);
	}
	precord_list_doc->curIndex = precord_list_doc->playIndex;

	record_list_getFilePath(precord_list_doc, precord_list_doc->playIndex, file);
	robin_set_cmd_stop();
	robin_set_cmd_play_file(file, NULL);
	robin_set_cmd_play();
	precord_list_doc->playSt = CEDAR_STAT_PLAY;

	return EPDK_OK;
}

__s32 record_list_continue( precord_list_doc_t precord_list_doc )
{
	char 	file[50];
	__cedar_status_t state;

	state = robin_get_fsm_status();
	if( CEDAR_STAT_STOP == state )
	{
		if( precord_list_doc->playSt == CEDAR_STAT_FF )
			record_list_playNext(precord_list_doc);
		else
			record_list_play(precord_list_doc);
	}
	else
	{
		robin_set_cmd_play();
	}

	return EPDK_OK;
}

__s32 record_list_pause( precord_list_doc_t precord_list_doc )
{

	if( precord_list_doc->musicMaxTimes != 0 )
	{
		robin_set_cmd_pause();
		precord_list_doc->playSt = CEDAR_STAT_PAUSE;
	}

	return EPDK_OK;
}

__s32 record_list_next( precord_list_doc_t precord_list_doc )
{
	if( precord_list_doc->curIndex == precord_list_doc->total-1 )
	{
		precord_list_doc->curIndex = 0;
		precord_list_doc->topIndex = 0;
	}
	else
	{
		precord_list_doc->curIndex++;
		if( precord_list_doc->curIndex >= precord_list_doc->total )
			precord_list_doc->curIndex = precord_list_doc->total-1;

		if( precord_list_doc->curIndex-precord_list_doc->topIndex >= RECORD_MUSIC_LIST_NUM )
		{
			precord_list_doc->topIndex++;
			if( precord_list_doc->topIndex >= precord_list_doc->total )
				precord_list_doc->topIndex = precord_list_doc->total-1;
		}
	}

	return EPDK_OK;
}

__s32 record_list_pre( precord_list_doc_t precord_list_doc )
{
	if( precord_list_doc->curIndex == 0 )
	{
		precord_list_doc->curIndex = precord_list_doc->total-1;
		precord_list_doc->topIndex = precord_list_doc->total-RECORD_MUSIC_LIST_NUM;
		if( precord_list_doc->topIndex < 0 )
			precord_list_doc->topIndex = 0;
	}
	else
	{
		precord_list_doc->curIndex--;
		if( precord_list_doc->curIndex < 0 )
			precord_list_doc->curIndex = 0;

		if( precord_list_doc->curIndex-precord_list_doc->topIndex < 0 )
		{
			precord_list_doc->topIndex--;
			if( precord_list_doc->topIndex < 0 )
				precord_list_doc->topIndex = 0;
		}
	}

	return EPDK_OK;
}

__s32 record_list_backward( precord_list_doc_t precord_list_doc )
{
	__s32 ret = 0;
	__cedar_status_t play_status;

	play_status = robin_get_fsm_status();
	ret = robin_set_cmd_rr();
	if((ret == -1)||(play_status == CEDAR_STAT_STOP))
	{
		record_list_play(precord_list_doc); //����ʧ��,���������
		return EPDK_OK;
	}
	else
	{
		precord_list_doc->playSt = CEDAR_STAT_RR;
	}
	return EPDK_OK;
}

__s32 record_list_forward( precord_list_doc_t precord_list_doc )
{
	__s32 ret = 0;
	__cedar_status_t play_status;

	play_status = robin_get_fsm_status();
	ret = robin_set_cmd_ff();
	if((ret == -1)||(play_status == CEDAR_STAT_STOP))
	{
		record_list_playNext(precord_list_doc); //ǰ��ʧ��,���������next
		return EPDK_OK;
	}
	else
	{
		precord_list_doc->playSt = CEDAR_STAT_FF;
	}
	return EPDK_OK;
}

__s32 record_list_delete( precord_list_doc_t precord_list_doc )
{
	char 	file[50];

	record_list_getFilePath(precord_list_doc, precord_list_doc->curIndex, file);

	if( precord_list_doc->playIndex == precord_list_doc->curIndex )
		robin_set_cmd_stop();

    eLIBs_remove(file);

    //����ͷ����
    if( precord_list_doc->total - precord_list_doc->topIndex == RECORD_MUSIC_LIST_NUM )
    {
    	precord_list_doc->topIndex--;
    	if( precord_list_doc->topIndex < 0 )
    		precord_list_doc->topIndex = 0;
    }
    //����ǰ������������
    if( precord_list_doc->curIndex == precord_list_doc->total-1 )
    	precord_list_doc->curIndex = precord_list_doc->curIndex-1;
    precord_list_doc->total--;

    if( precord_list_doc->total == 0 )
    	return EPDK_FAIL;
    else
    	return EPDK_OK;
}

__s32 record_list_timer( precord_list_doc_t precord_list_doc )
{
	__s32  retSpect, i, ret;
	__u16  levelTemp[SPECT_GROUP_WIDTH];

	//music play
	precord_list_doc->musicMaxTimes = robin_get_total_time();
	precord_list_doc->musicCurTimes = robin_get_cur_time();
	if( precord_list_doc->musicMaxTimes < precord_list_doc->musicCurTimes )
		precord_list_doc->musicCurTimes = precord_list_doc->musicMaxTimes;

	eLIBs_memcpy(precord_list_doc->oldSpctrmLevel, precord_list_doc->spctrmLevel, sizeof(precord_list_doc->spctrmLevel));
	if( precord_list_doc->musicMaxTimes == 0 )
	{
		eLIBs_memset(precord_list_doc->spctrmLevel, 0, sizeof(precord_list_doc->spctrmLevel));
		if( precord_list_doc->playSt == CEDAR_STAT_FF )
		{
			record_list_playNext(precord_list_doc);
			ret = EPDK_OK;
		}
		else if( precord_list_doc->playSt == CEDAR_STAT_RR )
		{
			record_list_play(precord_list_doc);
			ret = EPDK_OK;
		}
		else
		{
			ret = EPDK_FAIL;
		}
	}
	else
	{
		retSpect = robin_get_spectrum_info(levelTemp);
		if( retSpect )
		{
			eLIBs_memset(precord_list_doc->spctrmLevel, 0, sizeof(precord_list_doc->spctrmLevel));
		}
		else
		{
			eLIBs_memcpy(precord_list_doc->spctrmLevel, levelTemp, sizeof(precord_list_doc->spctrmLevel));
		}
		ret = EPDK_OK;
	}

	for( i = 0; i < RECORD_LIST_SPCTRM_LEVEL; i++ )
	{
		if( precord_list_doc->oldSpctrmLevel[i] < precord_list_doc->spctrmLevel[i] )
		{
			precord_list_doc->oldSpctrmLevel[i] = precord_list_doc->spctrmLevel[i];
		}
	}

	return ret;
}












