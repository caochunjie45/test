#ifndef __GLIDE_CURVE_H__
#define __GLIDE_CURVE_H__

typedef struct 
{
	int screen; 	// ��Ļ���
	int fb_len;		// �ǿհ����򳤶�
	int fb_start;	// �ǿհ�������ʼλ��
	int frame;		// ��Ԫ����
}__gcuv_para_t;


int glidecuv_init(__gcuv_para_t *para);
int glidecuv_start( int src );
int glidecuv_move( int dest , int src , int *array, int max_array_num);
int glidecuv_up(int dest, int src, int speed, int speed_direct,int *array, int max_array_num );
int glidecuv_stop(int dest, int src, int *array, int max_array_num );

#endif /* __GLIDE_CURVE_H__ */
