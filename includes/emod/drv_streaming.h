/*
*********************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                               hello world sample
*
*                                (c) Copyright 2006-2007, Steven.ZGJ China
*                                            All    Rights Reserved
*
* File    : drv_audio.h
* By      : kevin
* Version : V1.00
* Date    : 2009-4-6 18:34
*********************************************************************************************************
*/
#ifndef _DRV_STREAMING_H_
#define _DRV_STREAMING_H_

#include "typedef.h"
#include "cfgs.h"

typedef enum __STREAMING_DEV_CMD_SET
{
    STREAMING_DEV_CMD_NONE = 0,                  /* �����������Ӧ                 */

    /*���͸�streaming  manager dev��������*/
    STREAMING_DEV_CMD_NEWSTREAM = 0x100,         /* ע��һ���µ�streaming�豸 */
    STREAMING_DEV_CMD_DELSTREAM,                 /* ɾ��streaming�豸 */
    
    /*���͸�streaming dev ��������*/
    STREAMING_DEV_FLUSH = 0x200,                 /* flush streaming */
    STREAMING_DEV_CMD_QUERY_BUFSIZE,             /* ��ѯ�û���������ؿռ����,aux = __streaming_dev_query_size_t   */
    STREAMING_DEV_CMD_QUERY_STATUS,             /* ��ѯ�豸״̬: ����__streaming_status_t*/
    STREAMING_DEV_CMD_SINK,                /*֪ͨ���ն�д�������*/
} __streaming_dev_cmd_set_t;

typedef enum __STREAMING_STATUS
{
    STREAMING_DEV_STATUS_TRANS = 0,             /* ���ݴ���״̬��������������/д��*/
    STREAMING_DEV_STATUS_SINK,                  /* sink״̬��д���ݽ�������ȡʱӦ����ʣ������ȡ��*/
} __streaming_status_t;


typedef enum __STREAMING_DEV_QUERY_BUF_SIZE_TYPE
{
    STREAMING_DEV_QUERY_BUF_SIZE_NONE = 0,      /* δ����Ҫ��ȡ�ռ������           */
    STREAMING_DEV_QUERY_BUF_SIZE_DATA,          /* ��ȡ�������ڵ�����size           */
    STREAMING_DEV_QUERY_BUF_SIZE_FREE,          /* ��ȡ�������ڵĿ��пռ�size       */
    STREAMING_DEV_QUERY_BUF_SIZE_TOTAL,         /* ��ȡ�������ܿռ�                 */

    STREAMING_DEV_QUERY_BUF_SIZE_

} __streaming_dev_query_size_t;

#endif /* _DRV_STREAMING_H_ */

