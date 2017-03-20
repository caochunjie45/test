#ifndef __GLIDE_CURVE_H__
#define __GLIDE_CURVE_H__

typedef struct 
{
	int screen; 	// ��Ļ���
	int fb_len;		// �ǿհ����򳤶�
	int fb_start;	// �ǿհ�������ʼλ��
	int frame;		// ��Ԫ����
}__lstmgcuv_para_t;


int lstmglidecuv_init(__lstmgcuv_para_t *para);
int lstmglidecuv_start( int src );
int lstmglidecuv_move( int dest , int src , int *array, int max_array_num);
int lstmglidecuv_up(int dest, int src, int speed, int speed_direct,int *array, int max_array_num );
int lstmglidecuv_stop(int dest, int src, int *array, int max_array_num );

#endif /* __GLIDE_CURVE_H__ */
