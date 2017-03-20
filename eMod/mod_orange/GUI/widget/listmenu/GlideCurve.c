#include "ListMenu_i.h"
#include "GlideCurve.h"

static __lstmgcuv_para_t 	gcuv_global;
static int 		start_pos;


int lstmglidecuv_init(__lstmgcuv_para_t *para)
{
	gcuv_global.fb_len		= para->fb_len;			// buffer���
	gcuv_global.frame		= para->frame;			// ��Ԫ����
	gcuv_global.screen		= para->screen;			// ��Ļ���			
	gcuv_global.fb_start	= para->fb_start;		// fb��ʼλ�õ�����
	return 1;
}

int lstmglidecuv_start( int src )
{
	start_pos	= src;
	return 1;
}

int lstmglidecuv_move( int dest , int src , int *array, int max_array_num)
{
	int delta_y;
	int temp_pos;

	if(gcuv_global.fb_len <= gcuv_global.screen)
		return 0;
		
	delta_y		= 	dest - src;
	if(delta_y == 0)
		return 0;

	if(delta_y > 0)					//�����ƶ�
	{	
		//��˵��߽�����
		if(gcuv_global.fb_start + 	src - start_pos == 2 * gcuv_global.frame)
			return 0;
		temp_pos	= 	gcuv_global.fb_start + 	src - start_pos + delta_y;
		if(temp_pos > 2 * gcuv_global.frame)
		{
			delta_y = 2 * gcuv_global.frame - (gcuv_global.fb_start + 	src - start_pos);		
		}	
		array[0]	=	gcuv_global.fb_start + 	src - start_pos + delta_y / 2;
		array[1]	=	array[0] + delta_y / 4;
		array[2]	=	array[1] + delta_y / 8;
		array[3]	=	gcuv_global.fb_start + 	src - start_pos + delta_y;
		return 4;	
	}
	if(delta_y < 0)					//�����ƶ�
	{
		//�Ҷ˵��߽�����
		if(gcuv_global.fb_start + gcuv_global.fb_len + 	src - start_pos ==gcuv_global.screen - 2 * gcuv_global.frame)
			return 0;
		temp_pos = gcuv_global.fb_start + gcuv_global.fb_len + src - start_pos + delta_y;
		if(temp_pos < gcuv_global.screen - 2 * gcuv_global.frame)	
		{
			delta_y = (gcuv_global.screen - 2 * gcuv_global.frame) - (gcuv_global.fb_start + gcuv_global.fb_len + src - start_pos);		
		}
		array[0]	=	gcuv_global.fb_start + 	src - start_pos + delta_y / 2;
		array[1]	=	array[0] + delta_y / 4;
		array[2]	=	array[1] + delta_y / 8;
		array[3]	=	gcuv_global.fb_start + 	src - start_pos + delta_y;
		return 4;		
	}
	return 0;
}

int lstmglidecuv_up(int dest, int src, int speed,int speed_direct, int *array, int max_array_num )
{
	int temp_pos;
	int delta;
	int i;
	int j;
	int step_a;

	int step_count;
	
	speed = speed * 2;
	if(gcuv_global.fb_len <= gcuv_global.screen)
		return 0;
	
	if(speed == 0)			//ִ�ж������
	{
		temp_pos	= gcuv_global.fb_start + src - start_pos ;		
		if(temp_pos > 0 )					//�������С��֮�ڵĻص�
		{
			i 		= temp_pos / 10;
			delta	= 0 - temp_pos;
			for(j = 0; j < i ; j++)
			{
				array[j]	=	gcuv_global.fb_start + 	src - start_pos - (j + 1) * 10;			
			}
			array[j] = 0;
			return (j+1);	
		}
		
		if(temp_pos + gcuv_global.fb_len <  gcuv_global.screen)					//�Ҷ�����С��֮�ڵĻص�
		{
			delta	= gcuv_global.screen - (temp_pos + gcuv_global.fb_len);
			i 		= delta / 10;			
			
			for(j = 0; j < i ; j++)
			{
				array[j]	=	gcuv_global.fb_start + 	src - start_pos + (j + 1) * 10;			
			}
			array[j] = gcuv_global.fb_start + 	src - start_pos + delta;
			return (j+1);	
		}
		
		temp_pos 	= temp_pos + gcuv_global.fb_len;							//fbβ����λ��
		i 			= temp_pos % gcuv_global.frame ;
		if(2 * i >  gcuv_global.frame)
		{
			delta = gcuv_global.frame - i;
			i 	  = delta / 5;
			for(j = 0; j < i ; j++)
			{
				array[j]	=	gcuv_global.fb_start + 	src - start_pos + (j + 1) * 5;			
			}
			array[j] = gcuv_global.fb_start + 	src - start_pos + delta;
			return (j+1);				
		}
		else
		{
			delta = i;
			i 	  = delta / 5;
			for(j = 0; j < i ; j++)
			{
				array[j]	=	gcuv_global.fb_start + 	src - start_pos - (j + 1) * 5;			
			}
			array[j] = gcuv_global.fb_start + 	src - start_pos - delta;
			return (j+1);			
		}				
	}	

	if(speed_direct == EV_TP_ACTION_DOWN )					//�����ƶ�
	{		
		if((gcuv_global.fb_start + src - start_pos) == 2*gcuv_global.frame)
		{
			i 		= 2 * gcuv_global.frame / 10;
			for(j = 0; j < i; j++)
			{
				array[j]	= 2*gcuv_global.frame - (j + 1) * 10;			
			}
			array[j] = 0;
			step_count = j + 1;
											
			return (step_count);
		}
		else if(gcuv_global.fb_start + src - start_pos > 0)
		{
			delta 	= 2 * gcuv_global.frame - (gcuv_global.fb_start + src - start_pos);
			i 		= delta / 10;
			for(j = 0; j < i; j++)
			{
				array[j]	= (gcuv_global.fb_start + src - start_pos) + (j + 1) * 10;			
			}
			array[j] = 2 * gcuv_global.frame;
			step_count = j + 1;
			
			i 		= 2 * gcuv_global.frame / 10;
			for(j = 0; j < i; j++)
			{
				array[step_count+j]	= 2 * gcuv_global.frame - (j + 1) * 10;			
			}
			array[step_count+j] = 0;
			step_count = step_count + j + 1;
			return (step_count);
		}
		
		if(gcuv_global.fb_start + src - start_pos + speed * gcuv_global.frame > 0)
		{

			if(speed * gcuv_global.frame + (gcuv_global.fb_start + src - start_pos) > gcuv_global.frame)
			{
				delta = gcuv_global.frame - (gcuv_global.fb_start + src - start_pos); 
			}
			else
			{
				delta = speed * gcuv_global.frame;
			}		
			
			step_a  = delta / 2; 	
			i 		= step_a / 24;
			
			for(j = 0; j < i; j++)
			{
				array[j]	=	gcuv_global.fb_start + 	src - start_pos + (j + 1) * 24;			
			}
			step_count = j;			
			
			step_a 	= delta / 4;
			i 		= step_a / 20;
			for(j = 0; j < i; j++)
			{
				array[step_count+j]	= array[step_count-1] + (j + 1) * 20;			
			}
			step_count = step_count + j;	
			
			
			step_a = delta - (array[step_count-1]-(gcuv_global.fb_start + src - start_pos));
			i 		= step_a / 10;
			for(j = 0; j < i; j++)
			{
				array[step_count+j]	= array[step_count-1] + (j + 1) * 10;			
			}
			array[step_count+j] = gcuv_global.fb_start + src - start_pos + delta;	
			step_count = step_count + j + 1;		
			
			
			//�ص��������
			i 		= array[step_count-1] / 5;
			for(j = 0; j < i; j++)
			{
				array[step_count+j]	= array[step_count-1] - (j + 1) * 5;			
			}
			array[step_count+j] = 0;
			step_count = step_count + j + 1;		
									
			return (step_count);
					
		}	
		else
		{
			
			if((gcuv_global.fb_start + src - start_pos + gcuv_global.fb_len) % gcuv_global.frame > gcuv_global.frame/2)
				delta = speed * gcuv_global.frame + gcuv_global.frame -(gcuv_global.fb_start + src - start_pos + gcuv_global.fb_len) % gcuv_global.frame; 
			else
				delta = speed * gcuv_global.frame - (gcuv_global.fb_start + src - start_pos + gcuv_global.fb_len) % gcuv_global.frame;
									  
			step_a  = delta / 2; 	
			i 		= step_a / 24;
			
			for(j = 0; j < i; j++)
			{
				array[j]	=	gcuv_global.fb_start + 	src - start_pos + (j + 1) * 24;			
			}
			step_count = j;
			step_a 	= delta / 4;
			i 		= step_a / 20;
			for(j = 0; j < i; j++)
			{
				array[step_count+j]	= array[step_count-1] + (j + 1) * 20;			
			}
			step_count = step_count + j;
			step_a = gcuv_global.fb_start + src - start_pos + delta - array[step_count - 1];
			i 		= step_a / 8;
			for(j = 0; j < i; j++)
			{
				array[step_count+j]	= array[step_count-1] + (j + 1) * 8;			
			}
			array[step_count+j] = gcuv_global.fb_start + src - start_pos + delta;
			step_count = step_count + j + 1;
			return (step_count);
		}		
	}
	else if(speed_direct == EV_TP_ACTION_UP )								//�����ƶ�
	{
		if(gcuv_global.fb_start +gcuv_global.fb_len + src - start_pos == gcuv_global.screen - 2*gcuv_global.frame)
		{
			i 		= 2 * gcuv_global.frame / 10;
			for(j = 0; j < i; j++)
			{
				array[j]	= gcuv_global.fb_start + src - start_pos + (j + 1) * 10;			
			}
			array[j] = gcuv_global.screen - gcuv_global.fb_len;
			step_count = j + 1;
											
			return (step_count);
		}
		else if(gcuv_global.fb_start +gcuv_global.fb_len + src - start_pos < gcuv_global.screen )
		{
			delta 	= 2 * gcuv_global.frame-(gcuv_global.screen - (gcuv_global.fb_start +gcuv_global.fb_len + src - start_pos));
			i 		= delta / 10;
			for(j = 0; j < i; j++)
			{
				array[j]	= (gcuv_global.fb_start + src - start_pos) - (j + 1) * 10;			
			}
			array[j] = gcuv_global.screen - 2 * gcuv_global.frame - gcuv_global.fb_len;
			step_count = j + 1;
			
			i 		= 2 * gcuv_global.frame / 10;
			for(j = 0; j < i; j++)
			{
				array[step_count+j]	= array[step_count-1] + (j + 1) * 10;			
			}
			array[step_count+j] = gcuv_global.screen - gcuv_global.fb_len;
			step_count = step_count + j + 1;
			return (step_count);
		}
		if(gcuv_global.fb_start +gcuv_global.fb_len + src - start_pos - speed * gcuv_global.frame < gcuv_global.screen)
		{
			if(gcuv_global.fb_start +gcuv_global.fb_len + src - start_pos - speed * gcuv_global.frame < gcuv_global.screen - gcuv_global.frame)
			{
				delta = gcuv_global.fb_start +gcuv_global.fb_len + src - start_pos - (gcuv_global.screen - gcuv_global.frame);
			}
			else
			{
				delta = speed * gcuv_global.frame;
			}
			step_a  = delta / 2; 	
			i 		= step_a / 24;			
			for(j = 0; j < i; j++)
			{
				array[j]	=	gcuv_global.fb_start + 	src - start_pos - (j + 1) * 24;			
			}
			step_count = j;
			step_a 	= delta / 4;
			i 		= step_a / 20;
			for(j = 0; j < i; j++)
			{
				array[step_count+j]	= array[step_count-1] - (j + 1) * 20;			
			}
			step_count = step_count + j;
			
			step_a = delta - (-array[step_count-1] + (gcuv_global.fb_start + src - start_pos));
			i 		= step_a / 10;
			for(j = 0; j < i; j++)
			{
				array[step_count+j]	= array[step_count-1] - (j + 1) * 10;			
			}
			array[step_count+j] = gcuv_global.fb_start + src - start_pos - delta;
			step_count = step_count + j + 1;
			
			//�ص������Ҷ�
			i 		= (gcuv_global.screen-(array[step_count-1]+gcuv_global.fb_len)) / 5;
			for(j = 0; j < i; j++)
			{
				array[step_count+j]	= array[step_count-1] + (j + 1) * 5;			
			}
			array[step_count+j] = gcuv_global.screen - gcuv_global.fb_len;
			step_count = step_count + j + 1;											
			return (step_count);
					
		}	
		else
		{
			
			if((gcuv_global.fb_start + src - start_pos + gcuv_global.fb_len) % gcuv_global.frame > gcuv_global.frame/2)
				delta = speed * gcuv_global.frame - (gcuv_global.frame -(gcuv_global.fb_start + src - start_pos + gcuv_global.fb_len) % gcuv_global.frame); 
			else
				delta = speed * gcuv_global.frame + (gcuv_global.fb_start + src - start_pos + gcuv_global.fb_len) % gcuv_global.frame;
									  
			step_a  = delta / 2; 	
			i 		= step_a / 24;
			
			for(j = 0; j < i; j++)
			{
				array[j]	=	gcuv_global.fb_start + 	src - start_pos - (j + 1) * 24;			
			}
			step_count = j;
			step_a 	= delta / 4;
			i 		= step_a / 20;
			for(j = 0; j < i; j++)
			{
				array[step_count+j]	= array[step_count-1] - (j + 1) * 20;			
			}
			step_count = step_count + j;
			step_a = array[step_count - 1] - (gcuv_global.fb_start + src - start_pos - delta) ;
			i 		= step_a / 8;
			for(j = 0; j < i; j++)
			{
				array[step_count+j]	= array[step_count-1] - (j + 1) * 8;			
			}
			array[step_count+j] = gcuv_global.fb_start + src - start_pos - delta;
			step_count = step_count + j + 1;
			return (step_count);
		}
	}	
    else
    {
        temp_pos	= gcuv_global.fb_start + src - start_pos ;		
		if(temp_pos > 0 )					//�������С��֮�ڵĻص�
		{
			i 		= temp_pos / 10;
			delta	= 0 - temp_pos;
			for(j = 0; j < i ; j++)
			{
				array[j]	=	gcuv_global.fb_start + 	src - start_pos - (j + 1) * 10;			
			}
			array[j] = 0;
			return (j+1);	
		}
		
		if(temp_pos + gcuv_global.fb_len <  gcuv_global.screen)					//�Ҷ�����С��֮�ڵĻص�
		{
			delta	= gcuv_global.screen - (temp_pos + gcuv_global.fb_len);
			i 		= delta / 10;			
			
			for(j = 0; j < i ; j++)
			{
				array[j]	=	gcuv_global.fb_start + 	src - start_pos + (j + 1) * 10;			
			}
			array[j] = gcuv_global.fb_start + 	src - start_pos + delta;
			return (j+1);	
		}
		
		temp_pos 	= temp_pos + gcuv_global.fb_len;							//fbβ����λ��
		i 			= temp_pos % gcuv_global.frame ;
		if(2 * i >  gcuv_global.frame)
		{
			delta = gcuv_global.frame - i;
			i 	  = delta / 5;
			for(j = 0; j < i ; j++)
			{
				array[j]	=	gcuv_global.fb_start + 	src - start_pos + (j + 1) * 5;			
			}
			array[j] = gcuv_global.fb_start + 	src - start_pos + delta;
			return (j+1);				
		}
		else
		{
			delta = i;
			i 	  = delta / 5;
			for(j = 0; j < i ; j++)
			{
				array[j]	=	gcuv_global.fb_start + 	src - start_pos - (j + 1) * 5;			
			}
			array[j] = gcuv_global.fb_start + 	src - start_pos - delta;
			return (j+1);			
		}
    }
	return 0;
}

int lstmglidecuv_stop(int dest, int src, int *array, int max_array_num )
{
	int temp_pos;
	int delta;
	int i;
	int j;

	temp_pos	= gcuv_global.fb_start + src - start_pos ;		
	if(temp_pos > 0 )					//�������С��֮�ڵĻص�
	{
		i 		= temp_pos / 10;
		delta	= 0 - temp_pos;
		for(j = 0; j < i ; j++)
		{
			array[j]	=	gcuv_global.fb_start + 	src - start_pos - (j + 1) * 10;			
		}
		array[j] = 0;
		return (j+1);	
	}
	
	if(temp_pos + gcuv_global.fb_len <  gcuv_global.screen)					//�Ҷ�����С��֮�ڵĻص�
	{
		delta	= gcuv_global.screen - (temp_pos + gcuv_global.fb_len);
		i 		= delta / 10;			
		
		for(j = 0; j < i ; j++)
		{
			array[j]	=	gcuv_global.fb_start + 	src - start_pos + (j + 1) * 10;			
		}
		array[j] = gcuv_global.fb_start + 	src - start_pos + delta;
		return (j+1);	
	}
	
	temp_pos 	= temp_pos + gcuv_global.fb_len;							//fbβ����λ��
	i 			= temp_pos % gcuv_global.frame ;
	if(2 * i >  gcuv_global.frame)
	{
		delta = gcuv_global.frame - i;
		i 	  = delta / 5;
		for(j = 0; j < i ; j++)
		{
			array[j]	=	gcuv_global.fb_start + 	src - start_pos + (j + 1) * 5;			
		}
		array[j] = gcuv_global.fb_start + 	src - start_pos + delta;
		return (j+1);				
	}
	else
	{
		delta = i;
		i 	  = delta / 5;
		for(j = 0; j < i ; j++)
		{
			array[j]	=	gcuv_global.fb_start + 	src - start_pos - (j + 1) * 5;			
		}
		array[j] = gcuv_global.fb_start + 	src - start_pos - delta;
		return (j+1);			
	}					
}






