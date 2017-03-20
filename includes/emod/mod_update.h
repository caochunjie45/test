/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                                Update Module
*
*                                 (c) Copyright 2006-2010, All winners Co,Ld.
*                                             All Rights Reserved
*
* File    : mod_update.h
* By      : james
* Version : 1.0.0
* Date    : 2011-08-05
* Descript: 
* Update  : date              author         version   notes
*           2011-08-05        james deng     1.0.0     build the file.
*********************************************************************************************************
*/

#ifndef _MOD_UPDATE_H_
#define _MOD_UPDATE_H_

// ������״̬��ͨ��UPDATE_CMD_STATUS������Ի�ȡ��ǰ��״̬��Ϣ
typedef enum __UPDATE_STATUS
{
    UPDATE_STATUS_ERROR,
    UPDATE_STATUS_PREPARE,
    UPDATE_STATUS_START,
    UPDATE_STATUS_FINISH
} __update_status_t;

typedef enum __UPDATE_CMD_SET           // ���������б�(IOCtrol)          
{
    UPDATE_CMD_START = 0x01,            // ��ʼ����, pbuffer = (file name)
    UPDATE_CMD_STATUS,                  // ����״̬                       
    UPDATE_CMD_RATIO,                   // ��������                       
	UPDATE_CMD_STOP,                    // ��ֹ����
	UPDATE_CMD_CHECK_PROG  = UPDATE_CMD_RATIO,				// ������, return ratio(0-100), -1 = fail
	UPDATE_CMD_NULL=UPDATE_CMD_STOP+1,                    // ������
} __update_cmd_set_t;


#endif  /* _MOD_UPDATE_H_ */


