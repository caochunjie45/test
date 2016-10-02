/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                                  DRV IR KEY
*
*                                    (c) Copyright 2011-2014, Jackie China
*                                             All Rights Reserved
*
* File    : ir_key_proc.c
* By      : Jackie
* Version : v1.0
* Date    : 2012-01-09
* Descript: ir key process source file.
* Update  : date                auther      ver     notes
*           2012-01-09 15:07    Jackie      1.0     Create this file.
*********************************************************************************************************
*/

#include "drv_ir_key_i.h"
#include "ir_protocol.h"

#define IR_NEC_SRC_CLK_SAMPLE_DIV   (8)                         // 24/(1+7)= 3MHz,ccmu decrease 1 

#define IR_NEC_IDLE_THRESHOLD		(4)   	                    // Idle Threshold = (4+1)*128*21.33 = ~13.651ms > 9ms
#define IR_NEC_NOISE_THRESHOLD		(16)	                    // Filter Threshold = 16*21.33us   ~= 341 < 560us 
#define IR_NEC_SAMPLE_CLK_DIV       (BSP_IR_CIR_SAMPCLK_DIV_64) // 3MHz/64 = 46.875khz 
#define IR_NEC_KEYUP_DELAY          (130)                       // repeat key interval times: 95ms 

#define IR_RAW_BUF_SIZE             128

#define IR_RX_ENABLE_INT_TYPE		(IR_RX_FIFO_AVAILABLE_INT|IR_RX_PACKET_END_INT|IR_RX_FIFO_OVERRUN_INT)

/* NEC remoter user code ,defined by user */
#define IR_NEC_USER_CODE1           (0x7f80)                    // 银色学习型的红外遥控 
#define IR_NEC_USER_CODE2           (0xfe01)                    // 黑色pvp方案的红外遥控 
#define IR_NEC_USER_CODE3           (0xff00) 		/* 银色学习型的红外遥控  */

#define IR_IGNORE_FIRSTKEY_ON   (1)
#define IR_IGNORE_FIRSTKEY_OFF  (0)

typedef struct IR_RAW_BUFFER    
{
    __u32 	len; /* Packet Count */
    __u8 	buf[IR_RAW_BUF_SIZE]; 
}__ir_rawbuf_t;


typedef struct IR_KEY
{
	__hdle 	hIR;
	
    __hdle 	hpin;            // used for request and release pin 
    __hdle 	hclk_apb;        // apb clock gate enable
    __hdle 	hclk_ir;         // ir ccmu module register 
    __hdle 	hTimer;          // timer handle,for detect key
    
    __u8  	pre_value; //previous key value
    __u8  	crt_value; //current key value
    __u32 	ignore_first; // 首次收到repeat code       
    
    __u32  	user_code;//user code or address, 0:NOT CHECK user code or address code.other value: check it! 
    __ir_rawbuf_t ir_rawbuf;
}__ir_key_t;


static __ir_key_t 	ir_key;

__u32 ir_key_map[]=
{
    /* 0                    1                       2                       3 */
    KEY_RESERVED,           KEY_RESERVED,         KPAD_DOWN,              KPAD_UP,
    /* 4                    5                       6                       7 */
    KPAD_RETURN,            KEY_RESERVED,           KEY_1,                  KEY_MEDIA_PREV_TRACK,
    /* 8                    9                       10                      11 */
    KEY_RESERVED,           KEY_MEDIA_PREV_TRACK,   KEY_RESERVED,           KEY_RESERVED,
    /* 12                   13                      14                      15 */
    KEY_RESERVED,           KEY_MEDIA_PLAY_PAUSE,   KPAD_LEFT,              KEY_RESERVED,
    /* 16                   17                      18                      19 */
    KEY_BROWSER_HOME,       KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 20                   21                      22                      23 */
    KEY_VOLUME_MUTE,        KEY_RESERVED,           KEY_4,                  KEY_RESERVED,
    /* 24                   25                      26                      27 */
    KEY_MEDIA_STOP,         KPAD_ENTER,             KPAD_RIGHT,             KEY_RESERVED,
    /* 28                   29                      30                      31 */
    KEY_SLEEP,              KEY_MEDIA_NEXT_TRACK,   KEY_RESERVED,           KEY_RESERVED,
    /* 32                   33                      34                      35 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 36                   37                      38                      39 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 40                   41                      42                      43 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 44                   45                      46                      47 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 48                   49                      50                      51 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 52                   53                      54                      55 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 56                   57                      58                      59 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 60                   61                      62                      63 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 64                   65                      66                      67 */
    KEY_5,                  KEY_3,                  KEY_9,                  KEY_RESERVED,
    /* 68                   69                      70                      71 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 72                    73                     74                      75 */
    KEY_8,                  KEY_HOME,               KEY_RESERVED,           KEY_RESERVED,
    /* 76                    77                     78                      79 */
    KEY_0,                  KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 80                    81                     82                      83 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 84                    85                     86                      87 */
    KEY_RESERVED,           KEY_6,                  KEY_RESERVED,           KEY_RESERVED,
    /* 88                    89                     90                      91 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 92                    93                     94                      95 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 96                   97                      98                      99 */
    KEY_MEDIA_NEXT_TRACK,   KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 100                  101                     102                     103 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 104                  105                     106                     107 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 108                  109                     100                     111 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 112                  113                     114                     115 */
    KEY_BROWSER_FAVORITES,  KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 116                  117                     118                     119 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 120                  121                     122                     123 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 124                  125                     126                     127 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 128                  129                     130                     131 */
    KEY_RESERVED,           KEY_7,                  KEY_RESERVED,           KEY_RESERVED,
    /* 132                  133                     134                     135 */
    KPAD_ZOOM,              KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 136                  137                     138                     139 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 140                  141                     142                     143 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 144                  145                     146                     147 */
    KEY_RESERVED,           KEY_2,                  KEY_RESERVED,           KEY_RESERVED,
    /* 148                  149                     150                     151 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 152                  153                     154                     155 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 156                  157                     158                     159 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 160                  161                     162                     163 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 164                  165                     166                     167 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 168                  169                     170                     171 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 172                  173                     174                     175 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 176                  177                     178                     179 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 180                  181                     182                     183 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 184                  185                     186                     187 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 188                  189                     190                     191 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 192                  193                     194                     195 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 196                  197                     198                     199 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 200                  201                     202                     203 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,        
    /* 204                  205                     206                     207 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 208                  209                     100                     211 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 212                  213                     214                     215 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 216                  217                     218                     219 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 220                  221                     222                     223 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 224                  225                     226                     227 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 228                  229                     230                     231 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 132                  233                     234                     235 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 236                  237                     238                     239 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 240                  241                     242                     243 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 244                  245                     246                     247 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 248                  249                     250                     251 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,
    /* 252                   253                    254                     255 */
    KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED,           KEY_RESERVED
};

extern __s32 DEV_IR_KEY_Ioctrl(__hdle hIR_KEY, __u32 cmd, __s32 aux, void *pbuffer);

static __s32 __SetGpio(__u32 onoff)
{
    __s32  ret;
    __user_pin_set_t  gpio_set[2];

    if(onoff == 0)
    {
        if(ir_key.hpin == NULL)
        {
            __wrn("ir gpio is NULL!\n");
            return EPDK_FAIL;//immediately go out
        }
        esPINS_PinGrpRel(ir_key.hpin, 1);
    }
    else if(onoff == 1)
    {
	    ret = esCFG_GetGPIOSecData("ir_para", (void *)gpio_set, 1);
	    if(!ret)
	    {
		    ir_key.hpin = esPINS_PinGrpReq(gpio_set, 1);
	    }
	    else
        {
            __wrn("ir request gpio error \n");
            return EPDK_FAIL;
        }
    }

    return EPDK_OK;
}

static __s32 __SetSysClk(__u32 ctl, __u32 div)
{
    __hdle hclk = NULL;
               
     if(ctl == 0)
     {
         if(ir_key.hclk_apb == NULL || ir_key.hclk_ir == NULL)
         {
             __wrn("IR hclk is is NULL!\n");
             return EPDK_FAIL;
         }
     }
     else if(ctl == 1)
     { 
        /* 1. ir apb clock */
         hclk = esCLK_OpenMclk(CCMU_MOD_CLK_APB_IR0);
         if(hclk == NULL)
         {
             __wrn("requset IR apb clock handle fail!\n");
             return EPDK_FAIL;
         }
         ir_key.hclk_apb = hclk;// for release
         esCLK_MclkOnOff(ir_key.hclk_apb, 1);
         esCLK_SetMclkSrc(hclk, CCMU_SYS_CLK_APB0); /* must set clock src */
         
        /* 2. ir module clock */
         hclk = esCLK_OpenMclk(CCMU_MOD_CLK_IR0);
         if(hclk == NULL)
         {
            esCLK_MclkOnOff(ir_key.hclk_apb, 0);/*close*/
            esCLK_CloseMclk(ir_key.hclk_apb);
            __wrn("requset IR module clock handle fail!\n");
            return EPDK_FAIL;
         }         
         ir_key.hclk_ir = hclk;// for release         
         esCLK_SetMclkSrc(ir_key.hclk_ir, CCMU_SYS_CLK_HOSC); /* must set clock src:24MHz */
         esCLK_SetMclkDiv(ir_key.hclk_ir, div);/* (1-64) */
         esCLK_MclkOnOff(ir_key.hclk_ir, 1);
     }
     else
     {
         __wrn("cmd = %d is undefined!\n", ctl);
         return EPDK_FAIL;
     }      
          
     if(ctl == 0)
     {
        esCLK_MclkOnOff(ir_key.hclk_apb, 0);/*close*/
        esCLK_CloseMclk(ir_key.hclk_apb);
        
        esCLK_MclkOnOff(ir_key.hclk_ir, 0);/*close*/
        esCLK_CloseMclk(ir_key.hclk_ir);        
     }    
     
     return EPDK_OK;
}

//#define IR_MODIFY_PUBLIC
//#define IR_MODYFY_LU
#define IR_MODIFY_CAI
void __InitKeyMap(void)
{
    ir_key_map[0x14]  = KPAD_POWER;
    ir_key_map[0x57]  = KPAD_DTVREC;
    ir_key_map[0x41]  = KPAD_TV;
    ir_key_map[0x46]  = KPAD_MUTE;
    ir_key_map[0x09]  = KPAD_NUM1;
    ir_key_map[0x1d]  = KPAD_NUM2;
    ir_key_map[0x1f]  = KPAD_NUM3;
    ir_key_map[0x0d]  = KPAD_SCAN;
    ir_key_map[0x19]  = KPAD_NUM4;
    ir_key_map[0x1b]  = KPAD_NUM5;
    ir_key_map[0x11]  = KPAD_NUM6;
    ir_key_map[0x15]  = KPAD_INFO;
    ir_key_map[0x17]  = KPAD_NUM7;
    ir_key_map[0x12]  = KPAD_NUM8;
    ir_key_map[0x59]  = KPAD_NUM9;
    ir_key_map[0x08]  = KPAD_NUM0;
    ir_key_map[0x50]  = KPAD_EPG;
    ir_key_map[0x55]  = KPAD_UP;
    ir_key_map[0x48]  = KPAD_MENU;
    ir_key_map[0x4a]  = KPAD_SUBTITLE;
    ir_key_map[0x5e]  = KPAD_LEFT;
    ir_key_map[0x06]  = KPAD_ENTER;
    ir_key_map[0x05]  = KPAD_RIGHT;
    ir_key_map[0x03]  = KPAD_AUDIO;
    ir_key_map[0x47]  = KPAD_TVOUT;
    ir_key_map[0x07]  = KPAD_DOWN;
    ir_key_map[0x40]  = KPAD_LIST;
    ir_key_map[0x02]  = KPAD_RETURN;
    ir_key_map[0x18]  = KPAD_CHANNEL_DEC;
    ir_key_map[0x44]  = KPAD_CHANNEL_ADD;
    ir_key_map[0x0f]  = KPAD_VOICEDOWN;
    ir_key_map[0x51]  = KPAD_VOICEUP;
    ir_key_map[0x0a]  = KPAD_RR;
    ir_key_map[0x1e]  = KPAD_ENTER;
    ir_key_map[0x0e]  = KPAD_FF;
}

void __KeyDownProc(__u32 key_value)
{
    __ir_key_msg_t msg;
    msg.key_value = ir_key_map[key_value];
    msg.flag = 1;

	eLIBs_printf(">>>>>>>> = 0x = %x\n",key_value);
    DEV_IR_KEY_Ioctrl(0, DRV_IRKEY_CMD_PUTKEY, 0, &msg);
}

void __KeyUpProc(__u32 key_value)
{
    __ir_key_msg_t msg;
    msg.key_value = ir_key_map[key_value];
    msg.flag = 0;
	
    DEV_IR_KEY_Ioctrl(0, DRV_IRKEY_CMD_PUTKEY, 0, &msg);
}

static void __ResetRawBuffer(__ir_key_t *pir_key)
{
    pir_key->ir_rawbuf.len = 0;
    return;
}

static void __WriteRawBuffer(__ir_key_t *pir_key, __u8 data)
{
    if(pir_key->ir_rawbuf.len < IR_RAW_BUF_SIZE)    
    {
        pir_key->ir_rawbuf.buf[pir_key->ir_rawbuf.len++] = data;
    }
    else
    {
        __wrn("IR Rx Buffer Full!!\n");
    }
    return;
}

static __s32 __RawBufferFull(__ir_key_t *pir_key)
{
    if(pir_key->ir_rawbuf.len >= IR_RAW_BUF_SIZE)
    {
        return EPDK_TRUE;
    }
    else
    {
        return EPDK_FALSE;
    } 
}

extern __sys_pwm_para_t PwmStandbyPar;
__s32 __IrqHandler(void *pArg)
{
    __u32 irq_no = 0;
    __u32 code   = 0; // user code + data 
    __s32 valid  = 0; 
    __u32 len    = 0; //data number in the rxFIFO 
    __u32   i    = 0;    

	__ir_key_t *pir_key = (__ir_key_t *)pArg;
	
    irq_no = BSP_IR_RxQueryInt(pir_key->hIR);
    if((irq_no&(IR_RX_FIFO_AVAILABLE_INT|IR_RX_PACKET_END_INT|IR_RX_FIFO_OVERRUN_INT)) == 0)
    {
        __wrn("invalid interrupt = %d \n", irq_no);
        return EPDK_FAIL;
    }
    /*clear int flag*/
    BSP_IR_RxClearIntPending(pir_key->hIR, irq_no);

    /*get data number in rxFIFO*/
    len =  BSP_IR_RxGetFifoCnt(pir_key->hIR);
	
    if(pir_key->ir_rawbuf.len == IR_RAW_BUF_SIZE)
    {
	    for(i = 0; i < len; i++)
	    {
	        BSP_IR_ReadData(pir_key->hIR);		
	    }
	    __ResetRawBuffer(pir_key);    		
	    return EPDK_FAIL;
    }
        
    /* Read FIFO */
    for(i = 0; i < len; i++)
    {
        if( EPDK_TRUE == __RawBufferFull(pir_key))
        {
            __wrn("Raw Buffer Full!!\n");
            break;
        }
        else
        {
        	__u8 data = BSP_IR_ReadData(pir_key->hIR);			
            __WriteRawBuffer(pir_key, data);
        }			
    }

    if(irq_no & IR_RX_PACKET_END_INT)         //数据结束标识
    {      
        code  = IR_PROTOCOL_NecDecode(pir_key->ir_rawbuf.buf, pir_key->ir_rawbuf.len); 		
        __ResetRawBuffer(pir_key);
        if(IR_ERROR_CODE == code)
        {
            __wrn("unknwon format ir remoter.\n");
            return EPDK_FAIL;
        }
        valid = IR_PROTOCOL_NecCodeValid(code, pir_key->user_code);//有效按键,repeat code为0        
        
        if((valid == EPDK_TRUE) && (code != 0))//第一次按下
        {
            pir_key->crt_value = (code >> 16)&0xff; 			
			
			//几个按键交叉按时，发送之前的keyup
		    if( (pir_key->pre_value != pir_key->crt_value)&&(pir_key->pre_value != 0xff) )
		    {	
		    	//__msg("previous key up, key = %x \n", pir_key->pre_value);
			    __KeyUpProc(pir_key->pre_value); // send keyup msg,previous key 	
			    pir_key->pre_value = 0xff;		
		    }
		    
            __KeyDownProc(pir_key->crt_value);//keydown                   
            pir_key->pre_value = pir_key->crt_value;//记录当前按键值  
              
            pir_key->ignore_first = IR_IGNORE_FIRSTKEY_ON;//按键首次按下
            
            //__msg("1. key value = %x \n", pir_key->crt_value);
        	/* 激活定时器检测按键抬起， */
            if(pir_key->hTimer)
            {
                esKRNL_TmrStart((__krnl_stmr_t*)pir_key->hTimer);
            }
        }/* 如果为重复按键 */
        else if( (code == IR_REPEAT_CODE)&&(pir_key->pre_value == pir_key->crt_value)
                    &&(pir_key->pre_value != 0xff) )
        {  
            if(pir_key->ignore_first == IR_IGNORE_FIRSTKEY_ON )
            {
                pir_key->ignore_first = IR_IGNORE_FIRSTKEY_OFF;
            	/* reset定时器delay按键抬起*/
	            if(pir_key->hTimer)
	            {
	                esKRNL_TmrStart((__krnl_stmr_t*)pir_key->hTimer);
	            } 
	            __msg(" delay key up \n");
                return EPDK_OK;
            }           
            
        	__KeyDownProc(pir_key->crt_value);//keydown 
        	pir_key->pre_value = pir_key->crt_value;//记录当前按键值
        	__msg("2. key value = %x \n", pir_key->crt_value);
            /* reset定时器delay按键抬起*/
            if(pir_key->hTimer)
            {
                esKRNL_TmrStart((__krnl_stmr_t*)pir_key->hTimer);
            }                           
        }
    }

    if(irq_no & IR_RX_FIFO_OVERRUN_INT) // 出错
    {
    	__wrn("rx fifo overflow\n");
        len =  BSP_IR_RxGetFifoCnt(pir_key->hIR);
	    for(i = 0; i < len; i++)
	    {
	        BSP_IR_ReadData(pir_key->hIR);		
	    }
	    __ResetRawBuffer(pir_key);

        return EPDK_FAIL;
    }

    return EPDK_OK;
}


void __TimerHandler(void *pArg)
{
	__ir_key_t *pir_key = (__ir_key_t *)pArg;
	
	//__msg("key up, key = %x \n", pir_key->crt_value); 
	__KeyUpProc(pir_key->crt_value);	
	pir_key->crt_value = 0xff;
    pir_key->pre_value = 0xff;
}

__s32 IR_KEY_Init(void)
{	
	eLIBs_memset(&ir_key, 0, sizeof(__ir_key_t));
    ir_key.crt_value = 0xff;
    ir_key.pre_value = 0xff;
    ir_key.user_code = 0xff00;	//0xfe01;	//0x9f00;	//IR_NEC_USER_CODE3;
	
    if(__SetGpio(1) == EPDK_FAIL)
    {
        __wrn("ir key set gpio fail\n");
        return EPDK_FAIL;
    }
    if(__SetSysClk(1, IR_NEC_SRC_CLK_SAMPLE_DIV) == EPDK_FAIL)
    {
        __wrn("ir key open system clock fail\n");
        return EPDK_FAIL;
    }
    
    esINT_InsISR(INTC_IRQNO_IR0, (__pISR_t)__IrqHandler, &ir_key);
    esINT_EnableINT(INTC_IRQNO_IR0);
    
    ir_key.hTimer = (__hdle)esKRNL_TmrCreate(IR_NEC_KEYUP_DELAY, OS_TMR_OPT_PRIO_HIGH|OS_TMR_OPT_ONE_SHOT, __TimerHandler, &ir_key);
    if(!ir_key.hTimer)
    {
        __wrn("request timer error. it can run but no keyup msg!\n");
    }
    
	//initialize hardware
    ir_key.hIR = BSP_IR_Init(BSP_IR_SELECT_IR0);	
	BSP_IR_SetMode(ir_key.hIR, IR_MODE_CIR);	
    BSP_IR_CirConfig(ir_key.hIR, IR_NEC_IDLE_THRESHOLD, IR_NEC_NOISE_THRESHOLD, IR_NEC_SAMPLE_CLK_DIV);
	BSP_IR_EnableRx(ir_key.hIR, 1);
	BSP_IR_RxInvertPulsePolarity(ir_key.hIR, 1);	
    BSP_IR_RxClearIntPending(ir_key.hIR, IR_RX_ALL_INT);
    BSP_IR_RxEnableInt(ir_key.hIR, IR_RX_ENABLE_INT_TYPE, 1);
    BSP_IR_RxSetFifoTrigLevel(ir_key.hIR, 8);	
	
    __InitKeyMap();
    
    return EPDK_OK;
}

__s32 IR_KEY_Exit(void)
{
	esINT_UniISR(INTC_IRQNO_IR0, (__pISR_t)__IrqHandler);
	esINT_DisableINT(INTC_IRQNO_IR0);
	esKRNL_TmrDel((__krnl_stmr_t*)ir_key.hTimer);
	
    __SetSysClk(0, 0);    
    __SetGpio(0);    
    BSP_IR_Exit(ir_key.hIR);
    
    return EPDK_OK;
}

__s32  IR_key_get_code(void )
{
	 return ir_key.user_code;
}

