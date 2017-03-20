#ifndef __TXTLAYER_H__
#define __TXTLAYER_H__

#include "Txtcfg.h"
#include "TxtBuffer.h"
#include "TxtList.h"
#include "Txtbox_i.h"

struct tag_TXTBOX_DATA;

#define   LINE_MODE    1
#define   OK_MODE      0

#define   NO_FOUCS     0
#define   REAL_FOUCS   1     /*ʵ�����ý���*/
#define   VAR_FOUCS    2     /*�������ý���(���ٻ���ʱ����)*/

#define   WAIT_LOAD    0x01
//------------------------------------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------------------------------------
typedef struct tag_TXTLAYER_MAN
{
    __u8                        FoucsTID;
    __u8                        FoucsPrio;
    __u8                        mode;
	__u8						lastmode;
    __u8                        scnnum;
    __pos_t                     start_pos;
    __bool                      backup;
	H_LYR	    	            layer;         /*����ͼ��ľ��*/   
    __rect_t                    scnrect;
	txtbuffer_man_t *           txtbuffer;
    void*                       foucs_FB;
    __disp_layer_para_t         sec_layer;
    H_LYR                       focus_lyr;
    __bool                      backalpha;
    void*                       chg_fb;     /*���ݵ�Ŀ�ĵ�ַ*/
    void*                       head_fb;     /*���ݵ�ͷframebuffer*/
    void*                       tail_fb;     /*���ݵ�βalpha framebuffer*/
    void*                       head_addr;   /*���ݵ�Դ��ַ*/
    void*                       tail_addr;   /*���ݵ�β��ַ*/
    __u32                       asize;       /*���ݴ�С*/
    void*                       res_addr;    /*���ݵ�Դ��ַ*/
    __u32                       res_x;
    __u32                       res_y;
    __u32                       back_size;   /*���ݵ�framebuffer��С*/
	__u32                       back_count;  /*���ݵ�item count*/
    __pos_t                     back_pos;
	txtlist_t*                  txtlist;   
    struct tag_TXTBOX_DATA*     ptxtbox;
    __u32                       status; 
    __u32                       font_size;
    GUI_FONT*                   draw_font;
    __epdk_charset_enm_e        draw_code;   /*�ַ�����*/
    GUI_COLOR                   bk_color;
}txtlayer_man_t;

__s32               TxtLayer_GetBPP  (txtlayer_man_t *ptxt_layer);
__s32               TxtLayer_SwitchFB(txtlayer_man_t *txtlayer,__u32 x,__u32 y);
txtlayer_man_t*     TxtLayer_Create  (H_LYR txtlayer,__hdle listmenu,__u32 item_w);
__u32               TxtLayer_Destroy (txtlayer_man_t *listlayer);
__u32               TxtLayer_OnDraw  (txtlayer_man_t *listlayer);

#endif
