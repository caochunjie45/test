
#include "dtv_singal_uipara.h"
	
static dtv_singal_uipara_t dtv_singal_uipara_480_272 =
{
	
     //顶部的信号坐标
    {0,0,480,272},   //图层大小
	{0,2,120,20},    //节目名称  
	{150,2, 50, 20}, //信号质量文字
	{200,2, 50, 20}, //信号强度文字
	{280,2, 40, 20}, //信号强度
	{330,2, 25, 20}, //mute display
	{355,2, 30, 20}, //volume txt
	{420,2, 40, 20}, // dtv_charge
	
	{
		{0,272-36,480,18}, //cur_next_prog[0]
		{0,272-18,480,18},//cur_next_prog[1]
	},
	
    {150,100,157,88},
    {150,120,140,70},
    ID_DTV_VOLUME_STATUS_BMP,
    ID_DTV_VOLUME_MUTE_BMP,

	//信号强度
    {
    	ID_DTV_ANTENNA_0_BMP,
		ID_DTV_ANTENNA_1_BMP,
		ID_DTV_ANTENNA_2_BMP,
		ID_DTV_ANTENNA_3_BMP,
		ID_DTV_ANTENNA_4_BMP,
		ID_DTV_ANTENNA_5_BMP,
    },
    //电量
    {
		ID_DTV_CHARGE0_BMP,
		ID_DTV_CHARGE1_BMP,
		ID_DTV_CHARGE2_BMP,
		ID_DTV_CHARGE3_BMP,
		ID_DTV_CHARGE4_BMP,
		ID_DTV_CHARGE5_BMP,
	},
	{
		ID_DTV_BATTERY0_BMP,
		ID_DTV_BATTERY1_BMP,
		ID_DTV_BATTERY2_BMP,
		ID_DTV_BATTERY3_BMP,
		ID_DTV_BATTERY4_BMP,
		ID_DTV_BATTERY5_BMP,
	},

	ID_DTV_LIST_TOP_480_BMP, 
};

static dtv_singal_uipara_t dtv_singal_uipara_800_480 =
{	
     //顶部的信号坐标
    {0,0,800,480},           //图层大小
	{0,2,298,32},            //节目名称  
	{330,2, 58, 32},         //信号质量文字
	{390,2, 60, 32},         //信号强度文字
	{450,6, 50, 32},         //信号强度
	{600,7, 40, 32},         //mute display
	{640,4, 40, 32},         //volume txt
	{720, 5, 67, 31},        // dtv_charge
	
	{
		{0,392+44,800,22}, //cur_next_prog[0]
		{0,414+44,800,22},//cur_next_prog[1]
	},
    {300,200,157,88},
    {320,220,140,70},
    ID_DTV_VOLUME_STATUS_BMP,
    ID_DTV_VOLUME_MUTE_BMP,

	//信号强度
    {
    	ID_DTV_ANTENNA_0_BMP,
		ID_DTV_ANTENNA_1_BMP,
		ID_DTV_ANTENNA_2_BMP,
		ID_DTV_ANTENNA_3_BMP,
		ID_DTV_ANTENNA_4_BMP,
		ID_DTV_ANTENNA_5_BMP,
    },
    //电量
    {
		ID_DTV_CHARGE0_BMP,
		ID_DTV_CHARGE1_BMP,
		ID_DTV_CHARGE2_BMP,
		ID_DTV_CHARGE3_BMP,
		ID_DTV_CHARGE4_BMP,
		ID_DTV_CHARGE5_BMP,
	},
	{
		ID_DTV_BATTERY0_BMP,
		ID_DTV_BATTERY1_BMP,
		ID_DTV_BATTERY2_BMP,
		ID_DTV_BATTERY3_BMP,
		ID_DTV_BATTERY4_BMP,
		ID_DTV_BATTERY5_BMP,
	},

	ID_DTV_LIST_TOP_800_BMP, 
};


static dtv_singal_uipara_t dtv_singal_uipara_720_576 =
{
	
     //顶部的信号坐标
    {0,0,720,576},              //图层大小
	{0,2,150,32},               //节目名称
	{150,2, 50, 32},            //信号质量文字
	{200,2, 50, 32},            //信号强度文字
	{250+20,0, 50, 32},         //信号强度
	{350+20,7, 40, 32},         //mute display
	{350+20+40,4, 40, 32},      //volume txt  
	{(720-20)-67, 5, 67, 31},   // dtv_charge	

	{
		{0,576-22-22,720,22}, //cur_next_prog[0]
		{0,576-22,720,22},//cur_next_prog[1]
	},
    {300,200,157,88},
    {320,220,140,70},
    ID_DTV_VOLUME_STATUS_BMP,
    ID_DTV_VOLUME_MUTE_BMP,

     //信号强度
    {
    	ID_DTV_ANTENNA_0_BMP,
		ID_DTV_ANTENNA_1_BMP,
		ID_DTV_ANTENNA_2_BMP,
		ID_DTV_ANTENNA_3_BMP,
		ID_DTV_ANTENNA_4_BMP,
		ID_DTV_ANTENNA_5_BMP,
    },
    //电量
    {
		ID_DTV_CHARGE0_BMP,
		ID_DTV_CHARGE1_BMP,
		ID_DTV_CHARGE2_BMP,
		ID_DTV_CHARGE3_BMP,
		ID_DTV_CHARGE4_BMP,
		ID_DTV_CHARGE5_BMP,
	},
	{
		ID_DTV_BATTERY0_BMP,
		ID_DTV_BATTERY1_BMP,
		ID_DTV_BATTERY2_BMP,
		ID_DTV_BATTERY3_BMP,
		ID_DTV_BATTERY4_BMP,
		ID_DTV_BATTERY5_BMP,
	},

	ID_DTV_LIST_TOP_720_BMP, 
};


static dtv_singal_uipara_t dtv_singal_uipara_720_480 =
{
     //顶部的信号坐标
    {0,0,720,480},              //图层大小
	{0,2,150,32},               //节目名称    
	{150,2, 50, 32},            //信号质量文字
	{200,2, 50, 32},            //信号强度文字
	{270,0, 50, 32},            //信号强度
	{370,7, 40, 32},            //mute display
	{410,4, 40, 32},            //volume txt          
	{630,5, 67, 31},            //dtv_charge	

	{
		{0,480-22-22,720,22},  //cur_next_prog[0]
		{0,480-22,720,22},     //cur_next_prog[1]
	},
    {300,200,157,88},
    {320,220,140,70},
	
    ID_DTV_VOLUME_STATUS_BMP,
    ID_DTV_VOLUME_MUTE_BMP,

     //信号强度
    {
    	ID_DTV_ANTENNA_0_BMP,
		ID_DTV_ANTENNA_1_BMP,
		ID_DTV_ANTENNA_2_BMP,
		ID_DTV_ANTENNA_3_BMP,
		ID_DTV_ANTENNA_4_BMP,
		ID_DTV_ANTENNA_5_BMP,
    },
    //电量
    {
		ID_DTV_CHARGE0_BMP,
		ID_DTV_CHARGE1_BMP,
		ID_DTV_CHARGE2_BMP,
		ID_DTV_CHARGE3_BMP,
		ID_DTV_CHARGE4_BMP,
		ID_DTV_CHARGE5_BMP,
	},
	{
		ID_DTV_BATTERY0_BMP,
		ID_DTV_BATTERY1_BMP,
		ID_DTV_BATTERY2_BMP,
		ID_DTV_BATTERY3_BMP,
		ID_DTV_BATTERY4_BMP,
		ID_DTV_BATTERY5_BMP,
	},

	ID_DTV_LIST_TOP_720_BMP, 
};



static dtv_singal_uipara_t dtv_singal_uipara_1024_600 =
{
     //顶部的信号坐标
    {0,0,1024,600},
	{0,2,300,32},    
	{330,2, 60, 32},
	{400,2, 60, 32},
	{480,6, 50, 32},
	{700,7, 40, 32},
	{740,4, 40, 32},        
	{900, 5, 67, 31},		 // dtv_charge	

	{
		{0,600-22-22,1024,22}, //cur_next_prog[0]
		{0,600-22,1024,22},//cur_next_prog[1]
	},
	
    {450,250,157,88},
    {470,270,140,70},
	
    ID_DTV_VOLUME_STATUS_BMP,
    ID_DTV_VOLUME_MUTE_BMP,

     //信号强度
    {
    	ID_DTV_ANTENNA_0_BMP,
		ID_DTV_ANTENNA_1_BMP,
		ID_DTV_ANTENNA_2_BMP,
		ID_DTV_ANTENNA_3_BMP,
		ID_DTV_ANTENNA_4_BMP,
		ID_DTV_ANTENNA_5_BMP,
    },
    //电量
    {
		ID_DTV_CHARGE0_BMP,
		ID_DTV_CHARGE1_BMP,
		ID_DTV_CHARGE2_BMP,
		ID_DTV_CHARGE3_BMP,
		ID_DTV_CHARGE4_BMP,
		ID_DTV_CHARGE5_BMP,
	},
	{
		ID_DTV_BATTERY0_BMP,
		ID_DTV_BATTERY1_BMP,
		ID_DTV_BATTERY2_BMP,
		ID_DTV_BATTERY3_BMP,
		ID_DTV_BATTERY4_BMP,
		ID_DTV_BATTERY5_BMP,
	},


	ID_DTV_LIST_TOP_1024_BMP, 
};

dtv_singal_uipara_t* dtv_singal_get_uipara()
{
    RECT rect;

	dsk_display_get_size(&rect.width,&rect.height);

	if(rect.width == 480 && rect.height == 272)
	{
		return &dtv_singal_uipara_480_272;
	}
	else if(rect.width == 800 && rect.height == 480)
	{
		return &dtv_singal_uipara_800_480;
	}
	else if(rect.width == 720 && rect.height == 576)
	{
		return &dtv_singal_uipara_720_576;
	}
	else if(rect.width == 720 && rect.height == 480)
	{
		return &dtv_singal_uipara_720_480;
	}
    
	else if(rect.width == 1024 && rect.height == 600)
	{
		return &dtv_singal_uipara_1024_600;
	}
    
	else
	{
		return &dtv_singal_uipara_480_272;
	}
	
}


