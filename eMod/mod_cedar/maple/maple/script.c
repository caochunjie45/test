/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    :
*
* By      : Jerry
*
* Version : V2.00
*
* Date	  :
*
* Descript:

* modify by eric_wang
    V2.10
**********************************************************************************************************************
*/
#include "maple_i.h"
#include "script.h"

//#define   MAX_PATH                   256
#define   VALUE_STRING_SIZE          128    //ָ�洢subkey��=�������ߵ��ַ������ַ����鳤��
#define   ITEM_MAIN_NAME_MAX          32    //�洢�������ַ������ַ��������󳤶�,����\0,����subkey��=��ߵı��� 

//#define   DATA_COUNT_MAX         (4)
//#define   DATA_TYPE_SINGLE_WORD  (1)
//#define   DATA_TYPE_STRING       (2)
//#define   DATA_TYPE_MULTI_WORD   (3)
//#define   DATA_TYPE_GPIO         (4)
typedef enum tag_SubKeyDataType
{
    CEDARDATA_COUNT_MAX         = 4,
    CEDARDATA_TYPE_SINGLE_WORD  = 1,
    CEDARDATA_TYPE_STRING       = 2,
    CEDARDATA_TYPE_MULTI_WORD   = 3,
    CEDARDATA_TYPE_GPIO         = 4,
}SubKeyDataType;

typedef enum tag_LineStatus
{
    CEDARTHIS_LINE_NULL      = 0,
    CEDARTHIS_LINE_MAINKEY   = 1,
    CEDARTHIS_LINE_SUBKEY    = 2,
    CEDARTHIS_LINE_ERROR     = -1,
}LineStatus;

typedef enum tag_SubKeyType
{
    CEDARSUBKEY_UNKNOWN          = 0,
    CEDARSUBKEY_VIDEO_PERMISSION = 1,
    CEDARSUBKEY_AUDIO_PERMISSION = 2,
    CEDARSUBKEY_MISCELLANEOUS    = 3,
    CEDARSUBKEY_FECONFIG         = 4,
    CEDARSUBKEY_ERROR         = -1,
}SubKeyType;

/*******************************************************************************
Function name: VideoPermissionTable
Description: 
    ��Ƶ����������Ȩ��,��cedar.ini��ȡ,�û�����.
    Ĭ��ȫ����Ȩ.
Parameters: 
    
Return: 
    
Time: 2011/8/1
*******************************************************************************/
CedarVideoPermissionIndex VideoPermissionTable[] =
{
    {CEDAR_VBS_TYPE_MJPEG,           1},
    {CEDAR_VBS_TYPE_MPEG1_ES,        1},
    {CEDAR_VBS_TYPE_MPEG2_ES,        1},
    {CEDAR_VBS_TYPE_XVID,            1},
    {CEDAR_VBS_TYPE_DIVX3,           1},
    {CEDAR_VBS_TYPE_DIVX4,           1},
    {CEDAR_VBS_TYPE_DIVX5,           1},
    {CEDAR_VBS_TYPE_SORENSSON_H263,  1},
    {CEDAR_VBS_TYPE_RMG2,            1},
    {CEDAR_VBS_TYPE_RMVB8,           1},
    {CEDAR_VBS_TYPE_RMVB9,           1},
    {CEDAR_VBS_TYPE_H264,            1},
    {CEDAR_VBS_TYPE_VC1,             1},
    {CEDAR_VBS_TYPE_AVS,             1},
    {CEDAR_VBS_TYPE_H263,            1},
    {CEDAR_VBS_TYPE_VP6,             1},
    {CEDAR_VBS_TYPE_WMV1,            1},
    {CEDAR_VBS_TYPE_WMV2,            1},
        
    {CEDAR_VBS_TYPE_NONE,            1},

};

/*******************************************************************************
Function name: AudioPermissionTable
Description: 
    ��Ƶ����������Ȩ��,��cedar.ini��ȡ,�û�����.
    Ĭ��ȫ����Ȩ.
Parameters: 
    
Return: 
    
Time: 2011/8/1
*******************************************************************************/
CedarAudioPermissionIndex AudioPermissionTable[] =
{
    //normal audio file format
    {CEDAR_ABS_TYPE_MPEG12_LAYER1, 1},
    {CEDAR_ABS_TYPE_MPEG12_LAYER2, 1},
    {CEDAR_ABS_TYPE_MPEG12_LAYER3, 1},
    {CEDAR_ABS_TYPE_MPEG_AAC_LC,   1},
    {CEDAR_ABS_TYPE_AC3,           1},
    {CEDAR_ABS_TYPE_DTS,           1},      
    {CEDAR_ABS_TYPE_LPCM_V,        1},
    {CEDAR_ABS_TYPE_LPCM_A,        1},
    {CEDAR_ABS_TYPE_ADPCM,         1},
    {CEDAR_ABS_TYPE_PCM,           1},      
    {CEDAR_ABS_TYPE_WMA_STANDARD,  1},      
    {CEDAR_ABS_TYPE_FLAC,          1},
    {CEDAR_ABS_TYPE_APE,           1},
    {CEDAR_ABS_TYPE_OGG,           1},
    {CEDAR_ABS_TYPE_RAAC,          1},
    {CEDAR_ABS_TYPE_COOK,          1},
    {CEDAR_ABS_TYPE_SIPR,          1},
    {CEDAR_ABS_TYPE_ATRC,          1},
    {CEDAR_ABS_TYPE_AMR,           1},
    {CEDAR_ABS_TYPE_RA,            1},
    {CEDAR_ABS_TYPE_MID,           1},
        
    {CEDAR_ABS_TYPE_NONE,          1},

};

typedef struct tag_CedarINIVideoFormatIndex
{
    char                *pStrFormat;   /* extend name of the media file            */
    __cedar_video_fmt_t nType;          /* media file type                          */
}CedarINIVideoFormatIndex;
//extern CedarINIVideoFormatIndex   CedarINIVideoFmtTable[];
CedarINIVideoFormatIndex CedarINIVideoFmtTable[] =
{
	{"MJPEG"          , CEDAR_VBS_TYPE_MJPEG         },
	{"MPEG1_ES"       , CEDAR_VBS_TYPE_MPEG1_ES      },
	{"MPEG2_ES"       , CEDAR_VBS_TYPE_MPEG2_ES      },
	{"XVID"           , CEDAR_VBS_TYPE_XVID          },
	{"DIVX3"          , CEDAR_VBS_TYPE_DIVX3         },
	{"DIVX4"          , CEDAR_VBS_TYPE_DIVX4         },
	{"DIVX5"          , CEDAR_VBS_TYPE_DIVX5         },
	{"SORENSSON_H263" , CEDAR_VBS_TYPE_SORENSSON_H263},
	{"RMG2"           , CEDAR_VBS_TYPE_RMG2          },
	{"RMVB8"          , CEDAR_VBS_TYPE_RMVB8         },
	{"RMVB9"          , CEDAR_VBS_TYPE_RMVB9         },
	{"H264"           , CEDAR_VBS_TYPE_H264          },
	{"VC1"            , CEDAR_VBS_TYPE_VC1           },
	{"AVS"            , CEDAR_VBS_TYPE_AVS           },
	{"H263"           , CEDAR_VBS_TYPE_H263          },
	{"VP6"            , CEDAR_VBS_TYPE_VP6           },
	{"WMV1"           , CEDAR_VBS_TYPE_WMV1          },
	{"WMV2"           , CEDAR_VBS_TYPE_WMV2          },

    {"NONE"           , CEDAR_VBS_TYPE_NONE          },
};
__s32 ParseINIVideoCodecType(char *pStrFormat)
{
    __s32   i;
    for(i=0; ; i++)
    {
        if(CedarINIVideoFmtTable[i].nType == CEDAR_VBS_TYPE_NONE)
        {
            WARNING("Check error! [%s] is not match\n", pStrFormat);
            //reach the tail of the index table
            break;
        }
        if(strcmp(CedarINIVideoFmtTable[i].pStrFormat, pStrFormat) == 0)
        {
            //find the media  file type
            break;
        }
    }

    return CedarINIVideoFmtTable[i].nType;
}

typedef struct tag_CedarINIAudioFormatIndex
{
    char                *pStrFormat;   /* extend name of the media file            */
    __cedar_audio_fmt_t nType;          /* media file type                          */
}CedarINIAudioFormatIndex;
//extern CedarINIAudioFormatIndex   CedarINIAudioFmtTable[];
CedarINIAudioFormatIndex CedarINIAudioFmtTable[] =
{
    {"MP1"         , CEDAR_ABS_TYPE_MPEG12_LAYER1}, 
    {"MP2"         , CEDAR_ABS_TYPE_MPEG12_LAYER2},
    {"MP3"         , CEDAR_ABS_TYPE_MPEG12_LAYER3},
    {"MPEG_AAC_LC" , CEDAR_ABS_TYPE_MPEG_AAC_LC  },
    {"AC3"         , CEDAR_ABS_TYPE_AC3          },
    {"DTS"         , CEDAR_ABS_TYPE_DTS          },
    {"LPCM_V"      , CEDAR_ABS_TYPE_LPCM_V       },
    {"LPCM_A"      , CEDAR_ABS_TYPE_LPCM_A       },
    {"ADPCM"       , CEDAR_ABS_TYPE_ADPCM        },
    {"PCM"         , CEDAR_ABS_TYPE_PCM          },
    {"WMA_STANDARD", CEDAR_ABS_TYPE_WMA_STANDARD },
    {"FLAC"        , CEDAR_ABS_TYPE_FLAC         },
    {"APE"         , CEDAR_ABS_TYPE_APE          },
    {"OGG"         , CEDAR_ABS_TYPE_OGG          },
    {"RAAC"        , CEDAR_ABS_TYPE_RAAC         },
    {"COOK"        , CEDAR_ABS_TYPE_COOK         },
    {"SIPR"        , CEDAR_ABS_TYPE_SIPR         },
    {"ATRC"        , CEDAR_ABS_TYPE_ATRC         },
    {"AMR"         , CEDAR_ABS_TYPE_AMR          },
    {"RA"          , CEDAR_ABS_TYPE_RA           },
    {"MID"         , CEDAR_ABS_TYPE_MID          },

    {"NONE"        , CEDAR_ABS_TYPE_NONE         },
};

//typedef struct tag_CedarINIMiscellaneousIndex
//{
//    char                *pStr;   /* extend name of the media file            */
//}CedarINIMiscellaneousIndex;
//
//CedarINIMiscellaneousIndex CedarINIMiscellaneousTable[] =
//{
//    {"DelayBeforePlay"}, 
//    {"NONE"},
//};

__s32 ParseINIAudioCodecType(char *pStrFormat)
{
    __s32   i;
    for(i=0; ; i++)
    {
        if(CedarINIAudioFmtTable[i].nType == CEDAR_ABS_TYPE_NONE)
        {
            WARNING("[%s] is not match\n", pStrFormat);
            //reach the tail of the index table
            break;
        }
        if(strcmp(CedarINIAudioFmtTable[i].pStrFormat, pStrFormat) == 0)
        {
            //find the media  file type
            break;
        }
    }

    return CedarINIAudioFmtTable[i].nType;
}

/*******************************************************************************
Function name: CedarINI_Init
Description: 
    ��cedar�����ò�������Ĭ��ֵ.
Parameters: 
    
Return: 
    
Time: 2011/8/1
*******************************************************************************/
__s32 CedarINI_Init(CedarINI *pCedarINI)
{
    __s32   tmp1, tmp2;
    memset(pCedarINI, 0, sizeof(CedarINI));
    tmp1 = sizeof(pCedarINI->VideoPermissionTable);
    tmp2 = sizeof(VideoPermissionTable);
    if(tmp1 >= tmp2)
    {
        memcpy(pCedarINI->VideoPermissionTable, VideoPermissionTable, sizeof(VideoPermissionTable));
    }
    else
    {
        WARNING("video array is not enough![%d][%d]\n", tmp1, tmp2);
        return EPDK_FAIL;
    }

    tmp1 = sizeof(pCedarINI->AudioPermissionTable);
    tmp2 = sizeof(AudioPermissionTable);
    if(tmp1 >= tmp2)
    {
        memcpy(pCedarINI->AudioPermissionTable, AudioPermissionTable, sizeof(AudioPermissionTable));
    }
    else
    {
        WARNING("audio array is not enough![%d][%d]\n", tmp1, tmp2);
        return EPDK_FAIL;
    }
    pCedarINI->nAUDIO_RUN_ONLY_BUF_SIZE = 128*1024;
    pCedarINI->nAUDIO_WITH_VIDEO_BUF_SIZE = 512*1024;
    pCedarINI->nAACSBREnable = 1;
    pCedarINI->max_support_height = 0;
    return EPDK_OK;
}

__s32 FEConfig_Init(CFEConfig *pFEConfig)
{
    memset(pFEConfig, 0, sizeof(CFEConfig));
    pFEConfig->nTS_INTERFACE_TYPE = 0;
    pFEConfig->TS_INTERFACE_PACKET_SYNC_METHOD = 1;
    pFEConfig->TS_INTERFACE_SSI_DATA_ORDER = 0;
    pFEConfig->TS_INTERFACE_CLOCK_SIGNAL_POLARITY = 0;
    pFEConfig->TS_INTERFACE_ERROR_SIGNAL_POLARITY = 0;
    pFEConfig->TS_INTERFACE_DATA_VALID_SIGNAL_POLARITY = 0;
    pFEConfig->TS_INTERFACE_PSYNC_SIGNAL_POLARITY = 0;
    pFEConfig->TS_INTERFACE_PACKET_SIZE = 0;
    return EPDK_OK;
}

__s32 MapleINI_Init(CMapleINI *pMapleINI)
{
    __s32   tmpret;
    //��ʼ��FEConfig��
    FEConfig_Init(&pMapleINI->FEConfig);
    //��ʼ��pCedarINI
    tmpret = CedarINI_Init(&pMapleINI->CedarINIInstance);
    if(tmpret != EPDK_OK)
    {
        WARNING("init cedarINI fail\n");
        return EPDK_FAIL;
    }
    return EPDK_OK;
}

//------------------------------------------------------------------------------------------------------------
//
// ����˵��
//
//
// ����˵��
//
//
// ����ֵ
//
//
// ����
//    ��
//
//------------------------------------------------------------------------------------------------------------

//#define  THIS_LINE_NULL        (0)
//#define  THIS_LINE_MAINKEY     (1)
//#define  THIS_LINE_SUBKEY      (2)
//#define  THIS_LINE_ERROR       (-1)
//�˺������ص�ǰ�еĳ��ȣ���ָ�뷵�ص�ǰ������
/********************************************
* flag = 0      //��ǰ��ע���У������
*      = 1      //��ǰ���ֶ���
*      = 2      //��ǰ�������У������ֶε���һ��
*      = -1     //��ǰ�в����Ϲ淶������
*********************************************/
/*******************************************************************************
Function name: _get_line_status
Description: 
    
Parameters: 
    *flag: ����line_status; type = LineStatus,
    last_len: daddrv��ʣ�µ�δ���ĳ���.
Return: 
    ret = ������ֽ���.
Time: 2011/8/1
*******************************************************************************/
static  int  _get_line_status(char  *daddr,  int  *flag, int last_len)
{
    char  *src; //ָ��databuf
    int    len; //��¼������ֽ���
	char   ch;

    src = daddr;
	ch  = *src++;
	last_len --;
	switch(ch)
	{
		case ';':     //ע����
		case 0x0D:    //�س���
		{
			*flag = CEDARTHIS_LINE_NULL;
		}
		break;

		case '[':    //������
		{
			*flag = CEDARTHIS_LINE_MAINKEY;
			break;
		}

		default:     //�Ӽ���
		{
			*flag = CEDARTHIS_LINE_SUBKEY;
			break;
		}
	}

    len = 1;
	ch = *src++;    //�տ�ʼ�Ͷ���һ���ֽ�.�����ֶ���һ���ֽ�. 
    while((ch != 0x0A) && (last_len >  len))     //ֻҪ���ǻ��з��ţ���������,�������������,��Ҫ�˳�
    {
		ch = *src++;
		len ++;
        if(len >= 512)
        {
            WARNING("fatal error! line is too long!\n");
            *flag = CEDARTHIS_LINE_ERROR;

            return 0;
        }
    }

    return len + 1; //������ֽ���,����0x0d,0x0a. ��1����Ϊ�տ�ʼ����һ���ֽ�.
}

//���ҳ��������ַ�������
/*******************************************************************************
Function name: _fill_line_mainkey
Description: 
    
Parameters: 
    pbuf: һ�������,ָ��һ�еĸտ�ʼ�ĵط�.
    item_name:ֻװ�������ַ���.����:[video_permission], item_name = video_permission;
Return: 
    
Time: 2011/8/1
*******************************************************************************/
static  int _fill_line_mainkey(char *pbuf, char *item_name)
{
	char *src;
	char  ch, i;

	i = 0;
	src = pbuf + 1;        //���� ��
	ch  = *src++;

	while(']' != ch)
	{
		item_name[i] = ch;
		i++;
		ch = *src++;
		if(i >= ITEM_MAIN_NAME_MAX)
		{
            WARNING("fatal error! string array is not enough!\n");
			item_name[i-1] = 0;
			break;
		}
	}

	return 0;

}

/*******************************************************************************
Function name: _get_item_value
Description: 
    1.һ���Ӽ����øú�������.
    2. �ַ������Ȳ�Ҫ����31, ����һ����\0.
Parameters: 
    1.pbuf:��һ�е���ʼ��ַ.
    2.name:��Ⱥ���ߵ��ַ���, 
    3.value:��Ⱥ��ұߵ��ַ���.
Return: 
    
Time: 2011/8/1
*******************************************************************************/
static  int _get_item_value(char *pbuf, char *name, char *value)
{
    char  *src, *dstname, *dstvalue;
    int   len;

    src = pbuf;
    dstname = name;
    dstvalue = value;

    len = 0;
    while(*src != '=')
    {
        if((*src != 0x20) && (*src != 0x09))    //�ո����tab��
        {
            *dstname++ = *src;
            len ++;
        }
        src ++;
        if(len >= VALUE_STRING_SIZE-1)
        {
            WARNING("fatal error! name >= %d!\n", VALUE_STRING_SIZE-1);
			*dstname = '\0';
			break;
        }
    }
    *dstname = '\0';
    //src ++;      //���� '='
	while(*src != '=')
	{
		src ++;
	}

	src++;
    len = 0;
    while(*src != 0x0D)
    {
        if((*src != 0x20) && (*src != 0x09))
        {
            *dstvalue++ = *src;
            len ++;
        }
        src ++;
        if(len >= 127)
        {
            WARNING("fatal error! [=] right >= 127!\n");
            *dstvalue = '\0';
			return -1;
        }
    }
    *dstvalue = '\0';

    return 0;
}
//�˺���ת���ַ����ݳ�Ϊ�������ݣ�����10���ƺ�16����
//ת�����������value�У�����ֵ��־ת���ɹ�����ʧ��
/*******************************************************************************
Function name: _get_str2int
Description: 
    1.�Ӽ�= ���ұ�, ������Ϊ��3����ʽ:
    (1)�ַ���, value[0]�洢��4�ֽ�����ĸ���.
    (2)10������, value[0]����ֵ, value[1]��ʾ������ value[2] = 1����, -1��ʾ���滹��-��.
    (3)16������,ͬ��,

    �����Ǹ���.
Parameters: 
    
Return: 
    DATA_TYPE_SINGLE_WORD��
Time: 2011/8/1
*******************************************************************************/
static  int  _get_str2int(char *saddr, int value[] )
{
    char  *src;
    char   off;
    unsigned int  tmp_value = 0;
	int    data_count;
	int    sign = 1;    //��ʾ������

	data_count = 2;
    src = saddr;
    off = 0;         //0�ַ���;1����10����; 2����16����

    if(src[0] == '-')
    {
        sign = -1;
        if((src[1] == '0') && ((src[2] == 'x') || (src[2] == 'X')))      //ʮ������
        {
            src += 3;
            off  = 2;
        }
        else if((src[1] >= '0') && (src[1] <= '9'))         //����10����
        {
            src += 1;
		    off  = 1;
        }
    }
    else
    {
        if((src[0] == '0') && ((src[1] == 'x') || (src[1] == 'X')))      //ʮ������
        {
            src += 2;
            off  = 2;
        }
    	else if((src[0] >= '0') && (src[0] <= '9'))                     //ʮ����
    	{
    		off = 1;
    	}
    }

	//��ʾ���ַ���
	if(off == 0)
	{
		data_count = 0;
		while(src[data_count] != '\0')
		{
			data_count ++;
			if(data_count > 127)
			{
                WARNING("fatal errlr! [=] right > 127, too long!\n");
				break;
			}
		}
		if(data_count & 0x03)       //Ҫ�����ֽڶ���
		{
			data_count = (data_count & (~0x03)) + 4;
		}
		value[0] = data_count>>2;


		return CEDARDATA_TYPE_STRING;
	}
    else if(off == 1)   //10������
    {
        while(*src != '\0')
        {
            if((*src >= '0') && (*src <= '9'))
            {
                tmp_value = tmp_value * 10 + (*src - '0');
                src ++;
            }
            else if((*src == '+') || (*src == '-'))
            {
                WARNING("meet + or -\n");
                break;
            }
            else
            {
                return -1;
            }
        }
        value[0] = tmp_value;
		value[1] = sign;
		value[2] = 1;
        if(*src == '-')
        {
            value[2] = -1;
        }

		return CEDARDATA_TYPE_SINGLE_WORD;
    }
    else if(off == 2)
    {
        while(*src != '\0')
        {
			if((*src >= '0') && (*src <= '9'))
            {
                tmp_value = tmp_value * 16 + (*src - '0');
                src ++;
            }
            else if((*src >= 'A') && (*src <= 'F'))
            {
                tmp_value = tmp_value * 16 + (*src - 'A' + 10);
                src ++;
            }
            else if((*src >= 'a') && (*src <= 'f'))
            {
                tmp_value = tmp_value * 16 + (*src - 'a' + 10);
                src ++;
            }
            else if((*src == '+') || (*src == '-'))
            {
                WARNING("meet + or -\n");
                break;
            }
            else
            {
                return -1;
            }
        }
		value[0] = tmp_value;
		value[1] = sign;
		value[2] = 1;
		if(*src == '-')
        {
            value[2] = -1;
        }

		return CEDARDATA_TYPE_SINGLE_WORD;
    }
	else
	{
        WARNING("fatal error! unknown value type!\n");
		return -1;
	}
}

/*******************************************************************************
Function name: FindEntryInVideoPermissionTable
Description: 
    1.pTableһ�����VideoPermissionTable,���һ��һ����CEDAR_VBS_TYPE_NONE
Parameters: 
    1.pTable:������ָ��
    2.nCnt:��������
    3.nVideoFormat:��Ƶ�����ʽ.
Return: 
    
Time: 2011/8/2
*******************************************************************************/
CedarVideoPermissionIndex* FindEntryInVideoPermissionTable(CedarVideoPermissionIndex *pTable, __s32 nCnt, __s32 nVideoFormat)   //nVideoFormat = __cedar_video_fmt_t
{
    __s32   i;
    if(CEDAR_VBS_TYPE_NONE == nVideoFormat)
    {
        return NULL;
    }
    for(i=0; i<nCnt; i++)
    {
        if(pTable[i].nType == CEDAR_VBS_TYPE_NONE)
        {
            WARNING("video permission table search over, not find vformat[%x], nValidCnt[%d]\n", nVideoFormat, i+1);
            return NULL;
        }
        if(pTable[i].nType == nVideoFormat)
        {
            return pTable+i;
        }
    }
    WARNING("impossible? not find video permission entry!\n");
    return NULL;    
}

__s32 SetVideoPermissionEntry(CedarINI *pCedarINI, char *sub_name, int value)
{
    __s32 nVideoFormat;   //__cedar_video_fmt_t
    CedarVideoPermissionIndex   *pEntry;
    nVideoFormat = ParseINIVideoCodecType(sub_name);
    pEntry = FindEntryInVideoPermissionTable(pCedarINI->VideoPermissionTable, PERMISSION_TABLE_SIZE, nVideoFormat);
    if(pEntry)
    {
        pEntry->nPermissionFlag = value;
    }
    return EPDK_OK;
}

CedarAudioPermissionIndex* FindEntryInAudioPermissionTable(CedarAudioPermissionIndex *pTable, __s32 nCnt, __s32 nAudioFormat)   //nVideoFormat = __cedar_video_fmt_t
{
    __s32   i;
    if(CEDAR_ABS_TYPE_NONE == nAudioFormat)
    {
        return NULL;
    }
    for(i=0; i<nCnt; i++)
    {
        if(pTable[i].nType == CEDAR_ABS_TYPE_NONE)
        {
            WARNING("audio permission table search over, not find aformat[%x], nValidCnt[%d]\n", nAudioFormat, i+1);
            return NULL;
        }
        if(pTable[i].nType == nAudioFormat)
        {
            return pTable+i;
        }
    }
    WARNING("impossible? not find audio permission entry!\n");
    return NULL;    
}

__s32 SetAudioPermissionEntry(CedarINI *pCedarINI, char *sub_name, int value)
{
    __s32 nAudioFormat;   //__cedar_audio_fmt_t
    CedarAudioPermissionIndex   *pEntry;
    nAudioFormat = ParseINIAudioCodecType(sub_name);
    pEntry = FindEntryInAudioPermissionTable(pCedarINI->AudioPermissionTable, PERMISSION_TABLE_SIZE, nAudioFormat);
    if(pEntry)
    {
        pEntry->nPermissionFlag = value;
    }
    return EPDK_OK;
}

__s32 SetMiscellaneousEntry(CedarINI *pCedarINI, char *sub_name, int value)
{
    if(strcmp(sub_name, "DelayBeforePlay") == 0)
    {
        pCedarINI->nDelayBeforePlay = value;
        if(pCedarINI->nDelayBeforePlay < 0)
        {
            WARNING("fatal error! [%d] < 0\n", value);
            pCedarINI->nDelayBeforePlay = 0;
        }
    }
    else if(strcmp(sub_name, "AUDIO_RUN_ONLY_BUF_SIZE") == 0)
    {
        if(value > 0)
        {
            pCedarINI->nAUDIO_RUN_ONLY_BUF_SIZE = value*1024;
        }
        else
        {
            WARNING("fatal error! AUDIO_RUN_ONLY_BUF_SIZE[%d] < 0\n", value);
        }
    }
    else if(strcmp(sub_name, "AUDIO_WITH_VIDEO_BUF_SIZE") == 0)
    {
        if(value > 0)
        {
            pCedarINI->nAUDIO_WITH_VIDEO_BUF_SIZE = value*1024;
        }
        else
        {
            WARNING("fatal error! AUDIO_WITH_VIDEO_BUF_SIZE[%d] < 0\n", value);
        }
    }
    else if(strcmp(sub_name, "AACSBR") == 0)
    {
        if(value !=0)
        {
            pCedarINI->nAACSBREnable = 1;
        }
        else
        {
            pCedarINI->nAACSBREnable = 0;
        }
    }
    
    return EPDK_OK;
}

__s32 SetFEConfigEntry(CFEConfig *pFEConfig, char *sub_name, int value)
{
    if(strcmp(sub_name, "TS_INTERFACE_TYPE") == 0)
    {
        if(value >= 0)
        {
            pFEConfig->nTS_INTERFACE_TYPE = value;
        }
        else
        {
            WARNING("fatal error! nTS_INTERFACE_TYPE[%d] < 0\n", value);
        }
    }
    else if(strcmp(sub_name, "TS_INTERFACE_PACKET_SYNC_METHOD") == 0)
    {
        if(value >= 0)
        {
            pFEConfig->TS_INTERFACE_PACKET_SYNC_METHOD = value;
        }
        else
        {
            WARNING("fatal error! TS_INTERFACE_PACKET_SYNC_METHOD[%d] < 0\n", value);
        }
    }
    else if(strcmp(sub_name, "TS_INTERFACE_SSI_DATA_ORDER") == 0)
    {
        if(value >= 0)
        {
            pFEConfig->TS_INTERFACE_SSI_DATA_ORDER = value;
        }
        else
        {
            WARNING("fatal error! TS_INTERFACE_SSI_DATA_ORDER[%d] < 0\n", value);
        }
    }
    else if(strcmp(sub_name, "TS_INTERFACE_CLOCK_SIGNAL_POLARITY") == 0)
    {
        if(value >= 0)
        {
            pFEConfig->TS_INTERFACE_CLOCK_SIGNAL_POLARITY = value;
        }
        else
        {
            WARNING("fatal error! TS_INTERFACE_CLOCK_SIGNAL_POLARITY[%d] < 0\n", value);
        }
    }
    else if(strcmp(sub_name, "TS_INTERFACE_ERROR_SIGNAL_POLARITY") == 0)
    {
        if(value >= 0)
        {
            pFEConfig->TS_INTERFACE_ERROR_SIGNAL_POLARITY = value;
        }
        else
        {
            WARNING("fatal error! TS_INTERFACE_ERROR_SIGNAL_POLARITY[%d] < 0\n", value);
        }
    }
    else if(strcmp(sub_name, "TS_INTERFACE_DATA_VALID_SIGNAL_POLARITY") == 0)
    {
        if(value >= 0)
        {
            pFEConfig->TS_INTERFACE_DATA_VALID_SIGNAL_POLARITY = value;
        }
        else
        {
            WARNING("fatal error! TS_INTERFACE_DATA_VALID_SIGNAL_POLARITY[%d] < 0\n", value);
        }
    }
    else if(strcmp(sub_name, "TS_INTERFACE_PSYNC_SIGNAL_POLARITY") == 0)
    {
        if(value >= 0)
        {
            pFEConfig->TS_INTERFACE_PSYNC_SIGNAL_POLARITY = value;
        }
        else
        {
            WARNING("fatal error! TS_INTERFACE_PSYNC_SIGNAL_POLARITY[%d] < 0\n", value);
        }
    }
    else if(strcmp(sub_name, "TS_INTERFACE_PACKET_SIZE") == 0)
    {
        if(value >= 0)
        {
            pFEConfig->TS_INTERFACE_PACKET_SIZE = value;
        }
        else
        {
            WARNING("fatal error! TS_INTERFACE_PACKET_SIZE[%d] < 0\n", value);
        }
    }
    else
    {
        WARNING("fatal error! unkown entry[%s]\n", sub_name);
    }
    
    return EPDK_OK;
}



/*******************************************************************************
Function name: processSubKeyEntry
Description: 
    
Parameters: 
    int value[]: ��ʾ��һ������.������ int   value[8];

    �ο�_get_str2int();
    (2)10������, value[0]����ֵ, value[1]��ʾ������ value[2] = 1����, -1��ʾ���滹��-��.
    ����cedar.iniĿǰ����,ֻ�õ���������ʮ������.
Return: 
    
Time: 2011/8/1
*******************************************************************************/
__s32 processSubKeyEntry(CMapleINI *pMapleINI, __s32 next_subkey_flag, char *sub_name, int value[])
{
    __s32   ret;
    __s32   num;
    switch(next_subkey_flag)
    {
        case CEDARSUBKEY_VIDEO_PERMISSION:
        {
            SetVideoPermissionEntry(&pMapleINI->CedarINIInstance, sub_name, value[0]);    //Ŀǰ����cedar.ini��[video_permission]��˵, value[0]�Ϳ�����.
            ret = EPDK_OK;
		    break;
        }
        case CEDARSUBKEY_AUDIO_PERMISSION:
        {
            SetAudioPermissionEntry(&pMapleINI->CedarINIInstance, sub_name, value[0]);
            ret = EPDK_OK;
		    break;
        }
        case CEDARSUBKEY_MISCELLANEOUS:
        {
            num = value[0]*value[1];
            SetMiscellaneousEntry(&pMapleINI->CedarINIInstance, sub_name, num);
            ret = EPDK_OK;
		    break;
        }
        case CEDARSUBKEY_FECONFIG:
        {
            num = value[0]*value[1];
            SetFEConfigEntry(&pMapleINI->FEConfig, sub_name, num);
            ret = EPDK_OK;
		    break;
        }
        default:
        {
            WARNING("any other subkey??\n");
            ret = EPDK_FAIL;
            break;
        }
    }
    return ret;
}

//�㷨��һ��һ�еĲ���
//      ���ҵ�ð�ţ�ֱ����Ϊ��ע���У�Ȼ���ҵ��س����з��ţ��ҵ���һ��ȥ    (�﷨����)
//      ���ҵ�'['(��������)����Ϊ��һ������(��ΪA����)���ҳ��������м���ַ������Լ���������
//             �ҵ�������ַ��Լ����ݣ����δ����һ���ڴ���
//      ֱ���ҵ����������(B����)��Ȼ����ΪA�����������
//      ��A�����µ��������ݣ����մӼ�������ֵ��˳�����У��������һ���ڴ����
//      ��¼A�����ĳ���(�Ӽ��ĸ���)����¼A�����µ����ݴ�����ڴ��е�ƫ����

//#define TIEM_MAIN_MAX       16

/*******************************************************************************
Function name: parser_script_cedar_ini
Description: 
    
Parameters: 
    data_pbuf: �洢���������ݵ�buffer
    script_len: buffer�ĳ���.
    pCedarINI: �洢������Ϣ�����ݽṹ,�ڴ��ѷ����.
Return: 
    
Time: 2011/8/1
*******************************************************************************/
int parser_script_cedar_ini(void *data_pbuf, int script_len, CMapleINI *pMapleINI)
{
	int   ret = -1;
	int   next_subkey_flag = CEDARSUBKEY_ERROR; //SubKeyType
	int   line_len, line_status;    //line_status:LineStatus,
	char  sub_name[VALUE_STRING_SIZE], sub_value[VALUE_STRING_SIZE];    //sub_name,�洢'='��ߵı���,cedar.ini�涨=��߶�����������ַ���. =�ұ�����ֵ���ַ�����ʽ. ����ITEM_MAIN_NAME_MAX
	int   value[8];
	char  *src; //ָ��databuf

	src = (char *)data_pbuf;

	while(script_len)
	{
		line_len = _get_line_status(src, &line_status, script_len);//�жϵ�ǰ�е�״̬
		script_len -= line_len;

		switch(line_status)
		{
			case CEDARTHIS_LINE_NULL:                                //ע����
			{
				break;
			}

			case CEDARTHIS_LINE_MAINKEY:                             //������
			{
			    memset(sub_name, 0, VALUE_STRING_SIZE);
				if(_fill_line_mainkey(src, sub_name))  //��ȡһ�����������̱��������������Ϣ
				{
					goto _err;
				}
                if(!strcmp(sub_name, "video_permission"))
				{
				    next_subkey_flag = CEDARSUBKEY_VIDEO_PERMISSION;
				}
				else if(!strcmp(sub_name, "audio_permission"))
				{
				    next_subkey_flag = CEDARSUBKEY_AUDIO_PERMISSION;
				}
                else if(!strcmp(sub_name, "miscellaneous"))
				{
				    next_subkey_flag = CEDARSUBKEY_MISCELLANEOUS;
				}
                else if(!strcmp(sub_name, "FEConfig"))
				{
				    next_subkey_flag = CEDARSUBKEY_FECONFIG;
				}
				else
				{
                    next_subkey_flag = CEDARSUBKEY_UNKNOWN;
				}

				break;
			}

			case CEDARTHIS_LINE_SUBKEY:                              //�Ӽ��У���ǰ���������Ӧ
			{
				if(_get_item_value(src, sub_name, sub_value))   //ת�����ѵȺ��󷽵Ĵ�ŵ�sub_name���Ⱥ��ҷ��Ĵ�ŵ�sub_value�������ַ�����ʽ
				{
					goto _err;
				}
                //��ȡ����
                ret = _get_str2int(sub_value, value);
				if(ret == -1)           //���ݴ���
				{
                    WARNING("fatal error! get str2int fail\n");
					goto _err;
				}
                processSubKeyEntry(pMapleINI, next_subkey_flag, sub_name, value);
                break;
			}
			default:
			{
				goto _err;
			}
		}
		src += line_len;
	}

_err:

	ret = ((ret >= 0) ? 0: -1);

	return ret;
}

/*******************************************************************************
Function name: ScriptCedarINI
Description: 
    1.ר�Ž���cedar.ini�ű��ĺ���.
Parameters: 
    
Return: 
    
Time: 2011/8/1
*******************************************************************************/
__s32 ScriptMapleINI(char *script_name, CMapleINI *pMapleINI)
{
	FILE* script_file = NULL;
	int    ret = -1;
	char   *script_addr = NULL;
	int    script_len;
    __s32   tmpret;
    
    //��ʼ��MapleINI
    tmpret = MapleINI_Init(pMapleINI);
    if(tmpret != EPDK_OK)
    {
        WARNING("init MapleINI fail\n");
        return EPDK_FAIL;
    }
	//�򿪽ű��ļ����򲻿���ʧ��,ʹ��Ĭ�ϵ�������.
	if(!script_name)
	{
	    __wrn("the input script is empty\n");
	    return -1;
	}
	script_file = fopen(script_name, "rb");
	if(!script_file)
	{
		__wrn("unable to open script file %s\n", script_name);
		return -1;
	}
	//���Ȼ�ȡ�ű��ĳ���
	fseek(script_file, 0, SEEK_END);
    script_len = ftell(script_file);
    fseek(script_file, 0, SEEK_SET);
	//�����ű����е�����
	script_addr = (char *)malloc(script_len);
	if(!script_addr)
	{
	    __wrn("unable to malloc memory for script, [%d]\n", script_len);
	    goto _err_out;
	}
	fread(script_addr, 1, script_len, script_file);
	fclose(script_file);
	script_file = NULL;
	//�����ű�
    ret = parser_script_cedar_ini(script_addr, script_len, pMapleINI);
	if(ret)
	{
		__wrn("script is invalid\n");
	}
    free(script_addr);
    return ret;

_err_out:
	//�˳�ʱ��Ĵ���
	if(script_addr)
	{
		free(script_addr);
	}
	if(script_file)
	{
		fclose(script_file);
		script_file = NULL;
	}

	return ret;
}

