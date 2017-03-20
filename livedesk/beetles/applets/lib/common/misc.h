/*
************************************************************************************************************************
*                                                        CUCKOO
*                                                   the Audio Player
*
*                             Copyright(C), 2006-2009, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : misc.h
*
* Author      : Gary.Wang
*
* Version     : 1.1.0
*
* Date        : 2008.11.08
*
* Description :
*
* Others      : None at present.
*
*
* History     :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang      2008.11.08       1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef  __misc_h
#define  __misc_h

#include "beetles_app.h"
#include "APP_Mem.h"
#include "touch_adapter.h"
#include "APP_Debug.h" //���Ҫ�������

typedef enum
{
    SHORT_KEY_WAVE_FILE_FM=0,
    SHORT_KEY_WAVE_FILE_PHOTO,
    SHORT_KEY_WAVE_FILE_MOVIE,
    SHORT_KEY_WAVE_FILE_MUSIC,
    SHORT_KEY_WAVE_FILE_SETTING,
    SHORT_KEY_WAVE_FILE_EBOOK,
    SHORT_KEY_WAVE_FILE_RECORD,
    SHORT_KEY_WAVE_FILE_PREV,
    SHORT_KEY_WAVE_FILE_NEXT,
    SHORT_KEY_WAVE_FILE_MUTE_OFF,
    SHORT_KEY_WAVE_FILE_MUTE_ON,
    SHORT_KEY_WAVE_FILE_NEXT_SONG,
    SHORT_KEY_WAVE_FILE_PREV_SONG,
    SHORT_KEY_WAVE_FILE_PLAY,
    SHORT_KEY_WAVE_FILE_PAUSE,
    SHORT_KEY_WAVE_FILE_RECORD_START,
    SHORT_KEY_WAVE_FILE_RECORD_PAUSE,
    SHORT_KEY_WAVE_FILE_RECORD_STOP,
    SHORT_KEY_WAVE_FILE_NUM,
}short_key_sound_t;

//////////////////////////////////////////////////////////////////////////
//��ʶ��ת�ַ���
#define _Symbol2Str(str)                    #str
#define Symbol2Str(str)                     _Symbol2Str(str)

//�����
#define VK_NULL                             0x0fffffff
#define VK_ESCAPE                           GUI_MSG_KEY_ESCAPE
#define VK_MENU                             GUI_MSG_KEY_MENU
#define VK_ENTER                            GUI_MSG_KEY_ENTER
#define VK_LEFT                             GUI_MSG_KEY_LEFT
#define VK_RIGHT                            GUI_MSG_KEY_RIGHT
#define VK_POP_VOLUME                       GUI_MSG_KEY_LONGVADD
#define VK_SUBMENU                          GUI_MSG_KEY_LONGMENU
#define VK_SELECT                           GUI_MSG_KEY_VADD
#define VK_PAUSE					GUI_MSG_KEY_ENTER

#define VK_SELECT_REPEAT                    GUI_MSG_KEY_LONGVADD
#define VK_LEFT_REPEAT                      GUI_MSG_KEY_LONGLEFT
#define VK_RIGHT_REPEAT                     GUI_MSG_KEY_LONGRIGHT
#define VK_ENTER_REPEAT                     GUI_MSG_KEY_LONGENTER
#define VK_ESCAPE_REPEAT                    GUI_MSG_KEY_LONGESCAPE

///////macro const
#define MAX_FILE_NAME_LEN                   (256 * 3)

//mem
void * My_Palloc(__u32 npage, __u32 mode);

void My_Pfree(void *mblk, __u32 npage);

void* My_Balloc(__u32 size);

void  My_Bfree(void *mblk, __u32 size);

void* My_Balloc(__u32 size);
//#define My_Balloc(a) (eLIBs_printf("@L%d(%s)\n", __LINE__, __FILE__),Misc_My_Balloc(a))

void  My_Mfree(__mems_heap_t *heap, void *f);


//macro function
#define app_sprintf                         eLIBs_sprintf
#define ZeroMemory(_p, _size)               eLIBs_memset((_p), 0, (_size))
#define ZeroTypeMem(_p, _type)              ZeroMemory(_p, sizeof(_type))

#define BallocType(_p, _type)               APP_NewMem_T_B(_p, _type)
#define BFreeType(_p, _type)                APP_DelMem_T_B(_p, _type)

#ifndef ValueInRange //touch_adapter.h
#define ValueInRange(_min, _v, _max)        ((_min) <= (_v) && (_v) < (_max))
#endif

#ifndef ValueBetweenRange //touch_adapter.h
#define ValueBetweenRange(_min, _v, _max)   ((_min) <= (_v) && (_v) <= (_max))
#endif

#define DATA_OF_ADRR(t, p, x, y, w) (*( (t*)(p)+(y)*(w)+(x) ))


//////////////////////////////////////////
//��Ϣ
#ifndef NOTIFY_MSG
#define NOTIFY_MSG(mid, src, des, d1, d2, reserved) \
	do{\
	__gui_msg_t  exmsg={0}; \
	exmsg.id = mid; \
	exmsg.h_srcwin = src; \
	exmsg.h_deswin = des; \
	exmsg.dwAddData1= (__u32)d1; \
	exmsg.dwAddData2 = (__u32)d2; \
	exmsg.dwReserved = (__u32)reserved;\
	GUI_SendNotifyMessage(&exmsg); \
	}while(0)
#endif

#ifndef SEND_MSG
#define SEND_MSG(mid, src, des, d1, d2) \
	do{\
	__gui_msg_t  exmsg={0}; \
	exmsg.id = mid; \
	exmsg.h_srcwin = src; \
	exmsg.h_deswin = des; \
	exmsg.dwAddData1= (__u32)d1; \
	exmsg.dwAddData2 = (__u32)d2; \
	exmsg.dwReserved = (__u32)0;\
	GUI_SendMessage(&exmsg); \
	}while(0)
#endif

#ifndef SEND_MSG_EX
#define SEND_MSG_EX(mid, src, des, d1, d2, reserved) \
	do{\
	__gui_msg_t  exmsg={0}; \
	exmsg.id = mid; \
	exmsg.h_srcwin = src; \
	exmsg.h_deswin = des; \
	exmsg.dwAddData1= (__u32)d1; \
	exmsg.dwAddData2 = (__u32)d2; \
	exmsg.dwReserved = (__u32)reserved;\
	GUI_SendMessage(&exmsg); \
	}while(0)
#endif


#ifndef POST_MSG
#define POST_MSG(mid, src, des, d1, d2) \
	do{\
	__gui_msg_t  exmsg; \
	exmsg.id = mid; \
	exmsg.h_srcwin = src; \
	exmsg.h_deswin = des; \
	exmsg.dwAddData1= (__u32)d1; \
	exmsg.dwAddData2 = (__u32)d2; \
	GUI_PostMessage(&exmsg); \
	}while(0)
#endif

// �ڴ��豸���⴦��
#define GUI_MEMDEV_CopyToLCD_Ex(_h)        {GUI_SetDrawMode(LCD_DRAWMODE_NORMAL); GUI_MEMDEV_CopyToLCD(_h);}
//////////////////////////////////////////////////////////////////////////
//ͼ��
#define _app_show_layer(_layer)             do {GUI_LyrWinSetSta(_layer, GUI_LYRWIN_STA_ON); GUI_LyrWinSetTop(_layer);} while (0);

#define _app_hide_layer(_layer)             GUI_LyrWinSetSta(_layer, GUI_LYRWIN_STA_SUSPEND)

#define app_show_layer                      _app_show_layer
#define app_hide_layer                      _app_hide_layer

#define _app_show_focus_wnd(_wnd, _layer)  {app_show_layer(_layer); GUI_WinSetFocusChild(_wnd);}
#define app_show_focus_wnd(_wnd)            _app_show_focus_wnd(_wnd, GUI_WinGetLyrWin(_wnd))
#define app_hide_wnd(_wnd)                  app_hide_layer(GUI_WinGetLyrWin(_wnd))

//ע���
#define _get_reg_pointer(_p, _t, _e)        (_p = (_t *)dsk_reg_get_para_by_app(_e))
#define get_reg_pointer(_p, _t, _e)         _get_reg_pointer(_p, _t, _e) //{_get_reg_pointer(_p, _t, _e); __msg("!!! reg(%d) = %x\n", _e, _p);}
#define update_to_reg()                     dsk_reg_flush()

//res string
#define _get_lang_res(_id, _pbuf)           dsk_langres_get_menu_text(_id, _pbuf, get_menu_len(_id) + 1)
#define get_lang_res(_id, _pbuf)            {_get_lang_res(_id, _pbuf);/* __msg("load str ( %s )\n", _pbuf);*/}

//gui bmp res ���
#define GUI_BMPRES_Draw(_hTheme, _x, _y)    GUI_BMP_Draw(dsk_theme_hdl2buf(_hTheme), _x, _y)

//load bmp res
typedef struct tag_load_bmpres_type
{
	void    *data;
	HTHEME handle;
}load_bmp_res_t;
#define ResHandle2Data(_res)           ((_res).data)
#define ResHandle2Handle(_res)         ((_res).handle)

#define _create_bmp_res(_id, _res)     \
do \
{ \
	APP_dsk_theme_open((_res).handle, _id); \
	(_res).data = dsk_theme_hdl2buf((_res).handle); \
	DEBUG_BMP_DATA((_res).data); \
} while(0)

#define _destroy_bmp_res(_res)         do {APP_dsk_theme_close((_res).handle);} while(0)

#define DEBUG_CallFuncHere(_FuncName)  __msg("   call %s()\n", Symbol2Str(_FuncName))

#define GUI_BMP_RES_Draw(_res, _x, _y) GUI_BMP_Draw(ResHandle2Data(_res), _x, _y)
#define GUI_BMP_RES_GetYSize(_res)     GUI_BMP_GetYSize(ResHandle2Data(_res))
#define GUI_BMP_RES_GetXSize(_res)     GUI_BMP_GetXSize(ResHandle2Data(_res))

#define IsValidBMPRes(_res)            (ResHandle2Handle(_res) != NULL)


#define create_bmp_res(_id, _res)      \
do\
{\
	_create_bmp_res(_id, _res);\
} while(0)

#define destroy_bmp_res(_res)          _destroy_bmp_res(_res)

#define BMP_RES_NULL                   -1
//////////////////////////////////////////////////////////////////////////

typedef struct _cuckoo_time_t
{
	__u32    hour;
	__u32    minute;
	__u32    second;
}cuckoo_time_t;


//add by libaiao, 2011.5.8; 
//TIME_S,��ʾֻ��ʾ��; TIME_MS, ��ʾ��ʾ����; TIME_HMS,��ʾ��ʾʱ����,��
//AUTO_HMS,����ʱ���Զ�ת��������M,S
typedef enum time_format_s
{
	TIME_S,
	TIME_MS,
	TIME_HMS,
	TIME_AUTO_HMS,
}time_format_e;

extern void   uint2str( __u32 input, char * str );//ע��ú���ֻ��ת��Ϊ��λ���.

extern void  time2time( __u32 second_time, cuckoo_time_t *format_time );////declaraed by libaiao,2011.5.8
//extern void  uint2str( __u32 input, char * str);	//declaraed by libaiao,2011.5.8

extern void  time2str( __u32 ms, char *str );


extern void filesize2str(__u32 size, char *str);									//add by libaiao, 2011.5.8
extern void time2str_by_format(__u32 ms, char *str, time_format_e format);		//add by libaiao, 2011.5.8

extern __s32 GetClippedString(GUI_RECT *region, char *src_str, char *dst_str, const char *append);//add by libaiao
extern __u32 get_memory_info(char* title, void* pExtPara);			//add by libaiao,2011.6.26
extern __s32 app_misc_audio_mute(__u8 mute);
extern __s32 app_misc_audio_release();
extern __s32 audio_mode_switch(__u8 mode);
extern __u8 app_misc_get_mute_status(void);


#define	app_print_memory_usage(title, pExtPara)		__here__;\
	__msg("\n");\
	__msg("=================title: %s=====================\n", title);\
	get_memory_info(title, pExtPara)


#define	__memory_usage(title, pExtPara)		app_print_memory_usage(title, pExtPara)


//����24ʱ��Сʱֵ������12ʱ��Сʱֵ������0Ϊam����0Ϊpm
extern __s32 hour24_to_hour12(__s32* hour);

//��ѯapp_name���Ƶ�Ӧ���Ƿ����
//��ѯ���������Ƿ���ڿ�����is_app_exist(APP_MUSIC)
extern __s32 is_app_exist(char* app_name);

__s32 APP_BMP_DrawEx(H_LYR hlyr, const void * pFileData, int x, int y, int x0, int y0, int x1, int y1);


//���ñ���
__s32 app_set_title(__s32 res_id);

//����Ĭ�ϱ�����ɫ
__s32 com_set_backcolor(__u32 color);

//libaiao, 2011.5.15
//����ָ�����ȴ�src�н�ȡ�ַ���
//����ֵΪ��ȡ�ַ����ĳ���(byteΪ��λ)
__s32 GUI_GetStringByWidth(char *dst, const char GUI_UNI_PTR *src, __u32 width);

void pull_down_gpio(__u32 port, __u32 port_num);

void pull_up_gpio(__u32 port, __u32 port_num);

//����gpioΪ����
//pull: 0:���� 1:����  2:����
void set_input_gpio(__u32 port, __u32 port_num, __s32 pull);

__s32 get_gpio_status(__u32 port, __u32 port_num);

#define PT_LYR_INFO eLIBs_printf("MSG:L%d(%s):", __LINE__, __FILE__);print_layer_info()
int print_layer_info(void);

__s32 app_root_play_app_sounds(short_key_sound_t type);

#endif     //  ifndef __misc_h

/* end of misc.h */
