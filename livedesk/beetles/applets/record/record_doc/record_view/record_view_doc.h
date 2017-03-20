#ifndef __RECORD_VIEW_DOC_H__
#define __RECORD_VIEW_DOC_H__


#include "record_ui.h"

#define RECORD_FILTER					".mp3|.wav|.wma"

typedef enum RECORD_STATE
{
	RECORD_START,
	RECORD_PAUSE,
	RECORD_STOP

}RECORD_STATE;

typedef enum RECORD_AUDIO_TYPE
{
	RECORD_MP3,
	RECORD_WAV,
	//RECORD_WMA,
	RECORD_AUDIO_TYPE_MAX

}RECORD_AUDIO_TYPE;

typedef enum RECORD_TYPE
{
	RECORD_MIC,
	RECORD_FM
}RECORD_TYPE;

#define RECORD_SPCTRM_LEVEL	32

#define NO_FILES_TIMES		10

typedef struct tag_record_doc
{
	__s32			curRootType;
	char			curSearchPath[50];
	char			curSearchDisk[10];

	__u32			fileSize;				//��λKB
	__u32			diskFreeTime;			//��λ��
	__u64			diskFreeSize;

	RECORD_STATE	recState;				//¼��״̬
	__u32			recordMaxTimes;			//����¼��ʱ��
	__u32			recordCurTimes;			//��ǰ¼��ʱ��

	char	recordSrc[10];
	char	recordName[25];					//¼���ļ���
	char	recordFilePath[50];				//¼���ļ�·��

    __u32   				mid_ginkgo;		//ģ��
    __mp    				*mod_ginkgo;	//ģ��
    __krnl_event_t 			*msg_queue;		// ������Ϣ����
	__u16					oldSpctrmLevel[RECORD_SPCTRM_LEVEL];//¼��Ƶ��
	__u16					spctrmLevel[RECORD_SPCTRM_LEVEL];	//¼��Ƶ��
    __ginkgo_media_file_inf	media_file;		// ¼���ļ���Ϣ
    RECORD_AUDIO_TYPE		audioType;		//.mp3 or wav
    RECORD_AUDIO_TYPE		audioTypeTemp;	//��ʱ����.mp3 or wav
    RECORD_TYPE				recordType;		//mic or FM
	QUALITY_STATE			recQuality;		//¼������
	QUALITY_STATE			recQualityTemp;	//��ʱ����¼������

	__u32			timeCnt;

}record_doc_t, *precord_doc_t;

extern __s32 record_init(precord_doc_t precord_doc);
extern __s32 record_unInit(precord_doc_t precord_doc);
extern __u32 record_get_files(precord_doc_t precord_doc);
extern __s32 record_start(precord_doc_t precord_doc);
extern __s32 record_pause(precord_doc_t precord_doc);
extern __s32 record_continue(precord_doc_t precord_doc);
extern __s32 record_stop(precord_doc_t precord_doc);
extern __s32 record_updateCurTime(precord_doc_t precord_doc);

extern void record_updateFreeTime( precord_doc_t precord_doc );











#endif//__RECORD_VIEW_DOC_H__

