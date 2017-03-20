#ifndef __FILELIST_H__
#define __FILELIST_H__
#include "Beetles_app.h"


/*!
  * \brief
  *     ɾ��������Ϣ
*/
typedef struct file_op_info_s
{
    //item_type_t     		type;               		//��Ŀ���ͣ��ļ�����Ŀ¼
    __u64               total_size;         //��Ҫ������������;
    __u64               finish_size;        //�ѿ�����������;
    __u32               total_num;          //Ŀ¼ʱ���追��/ɾ�����ļ�����
    __u32               finish_num;         //Ŀ¼ʱ������ɿ���/ɾ�����ļ���    
    __u32               percent;            //�ѿ���/ɾ�����ݵİٷֱȣ���ʽ=�ѿ�������/�追����������
    __u32				dir_level;			//Ŀ¼����
	void 				(*OnDraw)(struct file_op_info_s* op_info);//��ɾ�����Ȼص�����
}file_op_info_t;



typedef struct file_item_s
{		
		char* name;						//�ļ����ļ�������
		__u32 name_len;					//������ͷ��ļ����ռ��ʱ����
		
		__u8  fatdirattr;         		/* FAT only. Contains the "DIR_Attr" field of an entry. */
										//���ļ��л����ļ�, �ļ��� = FSYS_ATTR_DIRECTORY
		__u8  status;					//�Ƿ������ļ���
		__u64 file_size;				//�ļ���С
		__u32 create_time;				//����ʱ�䣬��ʱ���ʽ�������
		
		struct file_item_s *previous;	//ָ����һ����Ŀ
		struct file_item_s *next;		//ָ����һ����Ŀ
}file_item_t;


typedef struct file_list_s
{
		char  file_path[RAT_MAX_FULL_PATH_LEN];			//��ǰĿ¼�ļ�·��		
		//char  USB_path[20];				//
		//char  SD_path[20];				//
		__u32 start_id;					//��ʾ���б��е���ʼID��
		__u32 focus_id;					//��ʾ���б��н���ID��				
		__u32 total;					//��ǰĿ¼�����ļ�����(�����ļ���,�ļ�,�� "..")
						
		file_item_t*	item_list;		//��Ŀ�б��׵�ַ
		file_item_t*	cur_item;		//��ǰ��Ŀ
				
		struct file_list_s  *parent;	//��һ��Ŀ¼ ��Ϣ
		struct file_list_s  *child;		//��һ��Ŀ¼ ��Ϣ
				
}file_list_t;

#define C_MAX_DIR_SCAN_LEVEL        16

extern __s32 new_file_list(const char *path, file_item_t** pp_item_list, void *ext_para);
extern __s32 delete_file_list(file_item_t  *item_list, __u32 total);

extern __s32 delete_file_item_from_list(file_item_t* item, file_list_t* list);
extern __s32 insert_file_item_to_list(file_item_t* cur_item, file_item_t* item);
extern file_item_t* get_file_list_item(file_list_t* nod, __u32 index);
extern char* get_file_list_item_name(file_list_t* nod, __u32 index);
extern rat_media_type_t  get_file_list_item_file_type(file_item_t* item);

extern file_list_t* new_file_root_list_nod(void* ext_para);
extern file_list_t* new_file_list_nod(const char *path, void* ext_para);
extern __s32 delete_file_list_nod(file_list_t* nod);
extern __s32 delete_file_list_chain(file_list_t* top_nod);

extern char* get_root_device_disp_name(char*string, __u8 attr);
extern __s32 delete_dir(char* dir_name, file_op_info_t *op_info);
#endif 






















