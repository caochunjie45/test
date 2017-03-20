/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											  All Rights Reserved
*
* File    	: circbuf.c
* By      	: Andy.zhang
* Func		: ���ζ��л������ 
* Version	: v1.0
* ============================================================================================================
* 2009-11-3 9:39:42  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "monkey_i.h"
#include "circbuf.h"

#define CB_MAX_SLOT			4
#define CACHE_MAX_LINE		10

/***********************************************************************************************************************
	slot[0]->CbSlot->CbSlot->CbSlot (������Ϣ)
	slot[1]->CbSlot->CbSlot->CbSlot (������Ϣ)
	|
	|
	|
	|
	slot[CB_MAX_SLOT-1]->CbSlot->CbSlot->CbSlot (������Ϣ)
	
	1. ���ζ��л��� CB_MAX_SLOT �εķ�����Ϣ;
	2. ÿ���ڰ����ķ�����Ϣ�ǿ���ȡ�̶����������ݷ��еõ�, ÿ�ΰ������������̶�, ���Բ��÷ֿ�����ķ�ʽ���;
	3. ���е�slot ���һ�����ζ���;
***********************************************************************************************************************/

/**********************************************************************************************************************/
typedef struct tag_CbSlot
{
	MkLine		line[CACHE_MAX_LINE];
	int 		count;	
	struct tag_CbSlot *next;		
}CbSlot;
/**********************************************************************************************************************/
/** ����һ�� slot */
static CbSlot *cbslot_create(void)
{
	CbSlot *slot;
	
	slot = (CbSlot *)g_malloc(sizeof(CbSlot));
	if( !slot )
	{
		__err(" cbslot_create error \n");
		return NULL;
	}
	g_memset( slot, 0, sizeof(CbSlot) );
	
	return slot;	
}

/** ��λslot , ����������ݣ����ǲ��ͷ��ڴ� */
static void cbslot_flush(CbSlot *slot)
{
	CbSlot *tmp;
	
	tmp = slot;
	
	while(tmp != NULL)			//ע�⣬��������Ҫ�ͷţ������free_txt_line���ͷŷ���Ļ���
	{
		tmp->count =0;
		g_memset(tmp->line, 0, sizeof(MkLine)*CACHE_MAX_LINE);
		tmp = tmp->next;
	}
	
	return;	
}

/** ɾ�� slot, �ͷ��ڴ� */
static void cbslot_delete(CbSlot *slot)
{
	CbSlot *tmp1, *tmp2;
	
	tmp1 = slot;
	while(tmp1->next != NULL)
	{
		tmp2 = tmp1;
		tmp1 = tmp1->next;
		
		g_free(tmp2);
	}
	
	g_free(tmp1);
	return;
}

///** ����slot index ������ */
//static void cbslot_setValue( CbSlot *slot, int index, MkLine *line)
//{
//	CbSlot *tmp = slot;
//
//	while( index >CACHE_MAX_LINE )
//	{
//		index -= CACHE_MAX_LINE;
//		tmp = tmp->next;
//		if(tmp == NULL)
//		{
//			tmp = (CbSlot *)g_malloc(sizeof(CbSlot));
//			g_memset(tmp, 0, sizeof(CbSlot));
//		}	
//	}
//
//	tmp->line[index-1].len		= line->len;
//	tmp->line[index-1].start    = line->start;	
//	tmp->count = index;
//}

/* ���� slot index ������*/
static MkLine *cbslot_getValue( CbSlot *slot, int index )
{
	CbSlot *tmp = slot;
	
	while( index >= CACHE_MAX_LINE )
	{
		index -= CACHE_MAX_LINE;
		tmp = tmp->next;			
	}

	if(tmp == NULL)
	{	
		return NULL;
	}	
	
	return &(tmp->line[index]);
}

/** ��slotβ���������*/
static __s32 cbslot_pushValue( CbSlot *slot, MkLine *line)
{
	int index;
	CbSlot *tmp = slot, *tmp1 = NULL;
	
	while(tmp->count == CACHE_MAX_LINE)
	{
		tmp1 = tmp;
		tmp  = tmp->next;
	}
	
	if( tmp == NULL)
	{
		tmp = (CbSlot *)g_malloc(sizeof(CbSlot));
		g_memset(tmp, 0, sizeof(CbSlot));
		tmp1->next = tmp;
	}
		
	index = tmp->count+1;
	tmp->line[index-1].len		 = line->len;
	tmp->line[index-1].start     = line->start;
	tmp->count = index;
	
	return EPDK_OK;
}

/** slot �ڲ�������item ��Ŀ */
static __s32 cbslot_getSize( CbSlot *slot )
{
	CbSlot *tmp = slot;
	int count=0;

	while( tmp != NULL)				
	{
		count += tmp->count;	
		tmp = tmp->next;
	}

	return count;	
}

/** slot ����ʼƫ���� */
static __s32 cbslot_getStartOffset( CbSlot *slot )
{
	return slot->line[0].start;
}

/** slot ��β��ƫ���� */
static __s32 cbslot_getEndOffset( CbSlot *slot )
{
	CbSlot *tmp1, *tmp2;
	int offset;
	
	tmp1 = tmp2 = slot;
	
	if( tmp1->count == 0 )
		return 0;
		
	while( tmp1->count == CACHE_MAX_LINE )
	{
		tmp2 = tmp1;
		tmp1 = tmp1->next;
	}
	
	if((tmp1->count == 0)||( tmp1 == NULL))
	{
		tmp1 = tmp2;
	}
	
	offset  = tmp1->line[tmp1->count-1].start;
	offset += tmp1->line[tmp1->count-1].len;
	
	return offset;
}

/**********************************************************************************************************************/
/*  ���λ����� */
struct tag_CircularBuff
{
	CbSlot*				slot[CB_MAX_SLOT];				
	int 	 			qin, qout;					/* ���λ����� in, out */
//	__s32				status;						/* ���ζ���״̬, ���� */
	__bool 	 			startFlag,  endFlag;		/* qin �Ƿ��ѵ��ĵ���ǰ�棬 qout �Ƿ��ѵ��ĵ������ */
	
	__s32				startQIndex, startLIndex;	/* ��ǰ��ʾҳ������Ϣ */
	__s32				endQIndex,   endLIndex;	
	//__s32				startOffset, endOffset;
	
	__u32				seekoffset;
	
	//__krnl_event_t 		*cacheMbox;					/* ��Ϣ����, ��������	*/
	__krnl_event_t 		*cacheQ;
	__krnl_event_t		*respMbox;					/* ��Ӧ����, ��Ӧ����	*/
	__u8			 	 tid;						/* ��̨�����߳� id		*/
	txtEncodeParser 	*plugin;					/* ��� 				*/
	LineConfig		 	 config;
};

/* cache thread communication cmd*/
typedef enum
{
	QCACHE_CMD_NEXT_BUF = 1,
	QCACHE_CMD_PREV_BUF,
	QCACHE_CMD_NEXT_PAGE,
	QCACHE_CMD_PREV_PAGE,	
	QCACHE_CMD_SEEK,
	QCACHE_CMD_REFRESH,
	QCACHE_CMD_EXIT
}QCacheCmd;

typedef struct 
{
	MkEncode 		encodeType;
	txtEncodeParser* (*plugin)(void);
}ParserPluginArray;

static ParserPluginArray pluginArray[]=
{
	{ MK_CHAR_ENCODE_GBK, 			gb_parserNew},
	{ MK_CHAR_ENCODE_UNICODE,   	utf16little_parserNew},
	{ MK_CHAR_ENCODE_UNICODE_BIG, 	utf16big_parserNew},
	{ MK_CHAR_ENCODE_UTF8, 			utf8_parserNew},
	{ MK_CHAR_ENCODE_UNKNOWN,		gb_parserNew}
};

static void qcache_thread( void *p_arg );

/**********************************************************************************************************************/
/* �������λ����� */
CircularBuff *circbuf_create(LineConfig *para)
{
	CircularBuff *cb;
	QCacheCmd	recev;
	int i;
	__u8 err;
	
	/* �������ڴ� */
	cb = (CircularBuff *)g_malloc( sizeof(CircularBuff) );
	if( !cb )
	{
		__err(" CircularBuff malloc error \n");
		return NULL;
	}
	g_memset( cb, 0, sizeof(CircularBuff) );
	
	/* װ�ؽ�����*/
	for( i= 0; i<sizeof(pluginArray)/sizeof(pluginArray[0]); i++)
	{
		if( pluginArray[i].encodeType == para->encodeType )
		{
			cb->plugin =  (*(pluginArray[i].plugin))();
			break;
		}
	}	
		
	/* ���ò�� */
	cb->plugin->config(cb->plugin, para);
	
	/* ��ʼ�����ζ��� */
	cb->qin = cb->qout = 0;	
	for( i=0; i<CB_MAX_SLOT; i++)
	{
		cb->slot[i] = cbslot_create();
	}
		
	cb->startQIndex = cb->endQIndex = 0;
	cb->startLIndex = cb->endLIndex = 0;
	
	/* ����ͨ������ */
	cb->respMbox  = (__krnl_event_t *)esKRNL_MboxCreate(NULL); 
	if( !cb->respMbox )
	{
		__err(" respMbox create error \n");
		return NULL;
	} 
	
	cb->cacheQ	  = (__krnl_event_t *)esKRNL_QCreate(5);
	if( !cb->cacheQ )
	{
		__err(" cacheq create error \n");
		return NULL;
	}

	/* ����cache�߳� */
	cb->tid = esKRNL_TCreate( qcache_thread, (void *)cb, 0x500, KRNL_priolevel5);
	if( !cb->tid )
	{
		__err(" qcache_thread create error \n");
		return NULL;
	}
	/* ֪ͨ�߳� */
	//esKRNL_MboxPost( cb->cacheMbox, (void *)QCACHE_CMD_REFRESH);
	esKRNL_QPost(cb->cacheQ, (void *)QCACHE_CMD_REFRESH);
	while(1)
	{
		recev = (QCacheCmd)(__s32)esKRNL_MboxPend(cb->respMbox, 0, &err);
		if(recev == QCACHE_CMD_REFRESH)
			break;
	}
	
	return cb;
}

/* ��ո�λ���ζ��� 		*/
__s32 	circbuf_flush(CircularBuff *cb)
{
	int i;
	
	cb->qin = cb->qout = 0;	
	for( i=0; i<CB_MAX_SLOT; i++ )
	{
		cbslot_flush(cb->slot[i]);
	}
	
	return EPDK_OK;
}


/* ���ö��в��� */
__s32	circbuf_setting(CircularBuff *cb, LineConfig *para)
{	
	QCacheCmd	recev;
	int  i;
	__u8 err;
	
	/* ˢ�»��ζ��� */
	circbuf_flush(cb);
	
	/* ˢ�µ�ǰ��ʾҳ������Ϣ*/
	cb->startQIndex = cb->startLIndex = 0;
	cb->endQIndex   = cb->endLIndex   = 0;
	
	/* ���²�� */
	if( cb->config.encodeType != para->encodeType )
	{
		cb->plugin->destroy(cb->plugin);
		cb->plugin = NULL;
		
		for( i= 0; i<sizeof(pluginArray)/sizeof(pluginArray[0]); i++)
		{
			if( pluginArray[i].encodeType == para->encodeType )
			{
				cb->plugin =  (*(pluginArray[i].plugin))();
				break;
			}
		}	
	}
	/* ���ò�� */
	cb->plugin->config(cb->plugin, para);
	
	/* ����������Ϣ */
	g_memcpy( &(cb->config), para, sizeof(LineConfig));
	
	/* ֪ͨ�߳� */
	//esKRNL_MboxPost( cb->cacheMbox, (void *)QCACHE_CMD_REFRESH);
	esKRNL_QPost(cb->cacheQ, (void *)QCACHE_CMD_REFRESH);
	while(1)
	{
		recev = (QCacheCmd)(__s32)esKRNL_MboxPend(cb->respMbox, 0, &err);
		if(recev == QCACHE_CMD_REFRESH)
			break;
	}
	
	return EPDK_OK;
}

__s32 	circbuf_seek( CircularBuff *cb, __u32 offset)
{
	QCacheCmd	recev;
	__u8 		err;
	
	/* ˢ�»��ζ��� */
	circbuf_flush(cb);
	
	/* ˢ�µ�ǰ��ʾҳ������Ϣ*/
	cb->startQIndex = cb->startLIndex = 0;
	cb->endQIndex   = cb->endLIndex   = 0;
	
	cb->seekoffset  = offset;
	
	//esKRNL_MboxPost( cb->cacheMbox, (void *)QCACHE_CMD_SEEK);
	esKRNL_QPost(cb->cacheQ, (void *)QCACHE_CMD_SEEK);
	while(1)
	{
		recev = (QCacheCmd)(__s32)esKRNL_MboxPend(cb->respMbox, 0, &err);
		if(recev == QCACHE_CMD_SEEK)
			break;
	}
	
	return EPDK_OK;
}

static __bool circbuf_isFull( CircularBuff *cb )
{
	if( (cb->qout + 1)%CB_MAX_SLOT == cb->qin )
		return EPDK_TRUE;
	else
		return EPDK_FALSE;
}

static CbSlot *circbuf_getIndex( CircularBuff *cb, int index)
{
	return cb->slot[index%CB_MAX_SLOT];
}

static CbSlot *circbuf_getHead( CircularBuff *cb )
{
	return cb->slot[cb->qin];
}

static CbSlot *circbuf_getTail( CircularBuff *cb )
{
	return cb->slot[cb->qout];
}

static CbSlot *circbuf_addHead(CircularBuff *cb)
{
	cb->qin = (cb->qin - 1 + CB_MAX_SLOT)%CB_MAX_SLOT;
	return cb->slot[cb->qin];
}

static CbSlot *circbuf_addTail(CircularBuff *cb)
{
	cb->qout = (cb->qout + 1)%CB_MAX_SLOT;
	return cb->slot[cb->qout];
}

__s32 	circbuf_getStartLines(CircularBuff *cb, MkLine *line, int num)
{
	CbSlot 		*slot;
	MkLine 		*tmpline;
	int i, curline, size, recev;
	__u8 err;
		
	g_memset(line, 0, sizeof(MkLine)*num);
	
	slot 	= circbuf_getIndex(cb, cb->startQIndex);
	size 	= cbslot_getSize(slot);	
	curline = cb->startLIndex;
	
	if( curline == size )
	{
		if( cb->startQIndex == cb->qout )
		{
			if(cb->endFlag == EPDK_TRUE)
				return -1;
		}		
	}
	
	for(i=0; i< num; i++)										//дlineofpage�ж�����Ϣ��ҳ����ȥshow
	{
		if(curline == size)										//����queue item ��ĩ��
		{
			if( cb->startQIndex == cb->qout)					//��һ��queue�Ƿ����
			{
				if(cb->endFlag == EPDK_TRUE)					//����Ѿ���������β��
				{					
					cb->endLIndex = curline-1;
					return 0;
				}
				else											//���û������β��
				{				
					esKRNL_QFlush(cb->cacheQ);
					esKRNL_QPost(cb->cacheQ,  (void *)QCACHE_CMD_NEXT_BUF);
					while(1)
					{
						recev = (QCacheCmd)(__s32)esKRNL_MboxPend(cb->respMbox, 0, &err);
						if(recev == QCACHE_CMD_NEXT_BUF)
							break;
					}
				}
			}
						
			curline = 0;

			cb->endQIndex	= ( cb->startQIndex+1 )%CB_MAX_SLOT;
			
			slot = circbuf_getIndex(cb, cb->endQIndex);
			size = cbslot_getSize(slot);
			
			esKRNL_QFlush(cb->cacheQ);
			esKRNL_QPost(cb->cacheQ,  (void *)QCACHE_CMD_NEXT_BUF);										
		}
		
		tmpline = cbslot_getValue(slot, curline);		
		g_memcpy( line + i, tmpline, sizeof(MkLine));
		curline++;
	}
	
	/* ��λ����λ�� */
	cb->endLIndex = curline-1;
	
	/* ������ҳ�߳� */
	esKRNL_QPost( cb->cacheQ, (void *)QCACHE_CMD_NEXT_PAGE);		//������ҳ�߳�
	
	return 0;
}

/* ����ȡ num �ж�����Ϣ 	*/
__s32 	circbuf_getNextLines(CircularBuff *cb, MkLine *line, int num)
{
	CbSlot 		*slot;
	MkLine 		*tmpline;
	QCacheCmd	recev;
	int 		i, curline, size;
	__u8 		err;
	
	
	g_memset(line, 0, sizeof(MkLine)*num);	
	
	
	/*  ��λ����ȡ�����ڻ��ζ����еĿ�ʼλ�� */
	slot = circbuf_getIndex(cb, cb->endQIndex);
	size = cbslot_getSize(slot);
	if( ( cb->endLIndex + 1) < size  )				/* ��ʼλ����endQIndex slot */
	{
		cb->startQIndex	= cb->endQIndex;
		cb->startLIndex	= cb->endLIndex+1;		
	}
	else											/* ��ʼλ������һ��slot */
	{
		if( cb->endQIndex == cb->qout ) 			/* ��һ��slot���ݲ��ڻ��ζ��� */
		{		
			if( EPDK_TRUE == cb->endFlag )			/* ������ζ��������Ѿ���������β�� */
			{
				return -1;							/* ����-1, �Ѿ���������β����û�ж������� */
			}
			else									/* ֪ͨ�߳�д�뻷�ζ�����һ��slot����*/
			{						
				esKRNL_QFlush(cb->cacheQ);
				esKRNL_QPost(cb->cacheQ,  (void *)QCACHE_CMD_NEXT_BUF);
				while(1)
				{					
					recev = (QCacheCmd)(__s32)esKRNL_MboxPend(cb->respMbox, 0, &err);
					if(recev == QCACHE_CMD_NEXT_BUF)
						break;
				}
			}
		}

		cb->startQIndex	= ( cb->endQIndex + 1 )%CB_MAX_SLOT;
		cb->startLIndex	= 0;
		
		slot = circbuf_getIndex(cb, cb->startQIndex);
		size = cbslot_getSize(slot);
	}
	
	
	cb->endQIndex 	= cb->startQIndex;
	curline	  		= cb->startLIndex;

	/* ��ȡnum�ж�����Ϣ */
	for(i=0; i< num; i++)
	{
		if(curline == size)									/* ����startQIndex slot ��ĩ�� */
		{
			if( cb->startQIndex == cb->qout )				/* ��һ��slot���ڶ�����, �������߳�д���� */
			{
				if(cb->endFlag == EPDK_TRUE)				/* ������ζ��������Ѿ���������β�� */
				{					
					cb->endLIndex = curline-1;
					return 0;
				}
				else										/* ���û������β��, ֪ͨ�߳�д�뻷�ζ�����һ��slot����*/
				{					
					esKRNL_QFlush(cb->cacheQ);
					esKRNL_QPost(cb->cacheQ,  (void *)QCACHE_CMD_NEXT_BUF);
					while(1)
					{
						recev = (QCacheCmd)(__s32)esKRNL_MboxPend(cb->respMbox, 0, &err);
						if(recev == QCACHE_CMD_NEXT_BUF)
							break;
					}
				}
			}
			
			curline = 0;

			cb->endQIndex = (cb->startQIndex +1)%CB_MAX_SLOT;
			slot = circbuf_getIndex(cb, cb->endQIndex);
			size = cbslot_getSize(slot);
			
			/* ������ҳ�߳� */
			if( (cb->startQIndex - cb->qout + CB_MAX_SLOT)%CB_MAX_SLOT > CB_MAX_SLOT/2)
			{
				esKRNL_QFlush(cb->cacheQ);
				esKRNL_QPost(cb->cacheQ,  (void *)QCACHE_CMD_NEXT_BUF);
			}
			
		}
		
		// ���ƶ�������
		tmpline = cbslot_getValue(slot, curline);		
		g_memcpy( line + i, tmpline, sizeof(MkLine));
		curline++;
	}
	
	/* ��λ����λ�� */
	cb->endLIndex = curline-1;
	
	__msg(" size = %d , startq = %d , startl = %d , endq = %d , endl = %d  \n", size, cb->startQIndex, cb->startLIndex, cb->endQIndex, cb->endLIndex);
	
	/* ������ҳ�߳� */
	esKRNL_QPost( cb->cacheQ, (void *)QCACHE_CMD_NEXT_PAGE);		//������ҳ�߳�
	
	return 0;
}

/* ��ǰ��ȡ num �ж�����Ϣ 	*/
__s32	circbuf_getPrevLines(CircularBuff *cb, MkLine *line, int num)
{
	CbSlot 			*slot;
	MkLine 			*tmpline;
	QCacheCmd		recev;
	int 			i, curline, size;
	__u8 			err;
		
	g_memset(line, 0, sizeof(MkLine)*num);
		
	/* ��λ����λ�� */
	slot	= circbuf_getIndex(cb, cb->startQIndex);
	size	= cbslot_getSize(slot);
	if( cb->startLIndex > 0 )						/* ����λ���� startQIndex slot */
	{
		cb->endQIndex		= cb->startQIndex;
		cb->endLIndex		= cb->startLIndex -1;		
	}
	else											/* ����λ���� ����һ�� slot */
	{
		if( cb->startQIndex == cb->qin )			/* ��һ��slot���ݲ��ڻ��ζ��� */
		{
			if( cb->startFlag == EPDK_TRUE )		/* ������ζ��������Ѿ����������ײ� */
			{
				return -1;							/* ����-1, �Ѿ����������ײ���û�ж������� */
			}
			else									/* ֪ͨ�߳�д�뻷�ζ�����һ��slot����*/
			{
				esKRNL_QFlush(cb->cacheQ);
				esKRNL_QPost(cb->cacheQ, (void *)QCACHE_CMD_PREV_BUF);
				while(1)
				{
					recev = (QCacheCmd)(__s32)esKRNL_MboxPend(cb->respMbox, 0, &err);
					if(recev == QCACHE_CMD_PREV_BUF)
						break;
				}			
			}
		}
		
		cb->endQIndex = (cb->startQIndex - 1 + CB_MAX_SLOT)%CB_MAX_SLOT;
		slot = circbuf_getIndex(cb, cb->endQIndex);
		size = cbslot_getSize(slot);
		cb->endLIndex = size - 1;		
	}

	curline			= cb->endLIndex;
	cb->startQIndex = cb->endQIndex;

	for( i= num-1; i>=0; i--)
	{
		if( curline < 0)												/* ���� endQIndex slot ���׶� */
		{
			if( cb->endQIndex == cb->qin )								/* ��һ��slot���ڶ����У��������߳�ȥд */
			{
				if( cb->startFlag == EPDK_TRUE )						/* ������ζ��������Ѿ����������ײ� */ 
				{
					break;
				}
				else										/* ���û�������ײ�, ֪ͨ�߳�д�뻷�ζ�����һ��slot����*/					
				{
					esKRNL_QFlush(cb->cacheQ);
					esKRNL_QPost(cb->cacheQ, (void *)QCACHE_CMD_PREV_BUF);
					while(1)
					{
						recev = (QCacheCmd)(__s32)esKRNL_MboxPend(cb->respMbox, 0, &err);
						if(recev == QCACHE_CMD_PREV_BUF)
							break;
					}
				}
			}
		
			cb->startQIndex = (cb->endQIndex - 1 + CB_MAX_SLOT)%CB_MAX_SLOT;

			slot	= circbuf_getIndex(cb, cb->startQIndex);
			size	= cbslot_getSize(slot);
			curline	= size-1;
			
			esKRNL_QFlush(cb->cacheQ);
			esKRNL_QPost(cb->cacheQ, (void *)QCACHE_CMD_PREV_BUF);		
		}
		
		tmpline = cbslot_getValue(slot, curline);
		
		//__msg(" curline = %d, start = %d, len = %d \n", curline, tmpline->start, tmpline->len);
		
		g_memcpy( line + i, tmpline, sizeof(MkLine));	
		curline--;
	}
	
	cb->startLIndex = curline+1;
	
	if( i>=0 )		// ��һҳ���ݲ���
	{
		int j;
		
		//__getc();
		g_memset(line, 0, sizeof(MkLine)*num);	
		curline = cb->startLIndex;
		
		for( j=0; j<=num-1; j++)
		{
			if(curline == size)
			{
				curline = 0;

				cb->endQIndex = (cb->startQIndex +1)%CB_MAX_SLOT;
				slot = circbuf_getIndex(cb, cb->endQIndex);
				size = cbslot_getSize(slot);
			}
			
			// ���ƶ�������
			tmpline = cbslot_getValue(slot, curline);		
			g_memcpy( line + j, tmpline, sizeof(MkLine));
			curline++;
		}
		
		return -2;
	}
	
	__msg(" size = %d , startq = %d , startl = %d , endq = %d , endl = %d  \n", size, cb->startQIndex, cb->startLIndex, cb->endQIndex, cb->endLIndex);
	
	esKRNL_QPost(cb->cacheQ, (void *)QCACHE_CMD_PREV_PAGE);		
	return 0;
}

/* ɾ�����λ����� 			*/
__s32	circbuf_delete(CircularBuff *cb)
{
	__u8 			err;
	int  			i;
	QCacheCmd		recev;
	
	/* ɾ���߳� */	
	esKRNL_QFlush(cb->cacheQ);
	esKRNL_QPost(cb->cacheQ,  (void *)QCACHE_CMD_EXIT);
	
	while(1)
	{					
		recev = (QCacheCmd)(__s32)esKRNL_MboxPend(cb->respMbox, 0, &err);
		if(recev == QCACHE_CMD_EXIT)
			break;
	}
	
		
	/* ɾ������ */
	esKRNL_MboxDel( cb->respMbox, 0, &err );
	esKRNL_QDel( cb->cacheQ, 0, &err);
	
	/* �ͷŻ��ζ��� */
	for( i=0; i< CB_MAX_SLOT; i++ )
	{
		cbslot_delete(cb->slot[i]);
	}
	
	/* ж�ز�� */
	cb->plugin->destroy(cb->plugin);
	
	/* �ͷž��*/
	g_free( cb );
	
	return EPDK_OK;
}

/**********************************************************************************************************************/
//static void qcache_onCmdStart( CircularBuff *cb )
//{
//	txtEncodeParser	 *parser;
//	CbSlot			 *slot;	
//	MkLine 			  line;
//	__s32			  ret;
//	
//	parser = cb->plugin;
//	
//	slot   = (CbSlot *)circbuf_getIndex(cb, 0);
//	parser->next( parser, parser->start );
//	while(1)
//	{
//		g_memset(&line, 0, sizeof(MkLine));
//		
//		ret = parser->analysis(parser, &line);
//		if( ret< 0)			// �������
//			break;
//		cbslot_pushValue(slot, &line);
//	}
//
//	cb->startFlag = parser->bof;
//	cb->endFlag   = parser->eof;
//	return ;
//} 

/** �建���������¶��У����ǲ���Ҫ���¶�λ��ʼλ�� */
static void qcache_onCmdRefresh( CircularBuff *cb )
{
	txtEncodeParser	 *parser;
	MkLine 			  line;
	CbSlot			 *slot;
	__s32			  ret;
	
	parser = cb->plugin;
	
	//circbuf_flush(cb);
	
	slot   = (CbSlot *)circbuf_getIndex(cb, 0);	
	
	parser->next(parser, parser->start);
	
	while(1)
	{
		g_memset(&line, 0, sizeof(MkLine));
		
		ret = parser->analysis(parser, &line);	//����һ������
//		cbslot_pushValue( slot, &line );		//�������õ�������д�� circbuf 	
//		if( ret< 0)
//			break;	
		if( ret == -2)
		{
			cbslot_pushValue( slot, &line );	//�������õ�������д�� circbuf
			break;
		}
		else if( ret == -1 )
		{
			break;
		}
		else
		{
			cbslot_pushValue( slot, &line );		//�������õ�������д�� circbuf 	
		}
	}
	
	cb->startFlag = parser->bof;
	cb->endFlag   = parser->eof;
	
	return;
}

/** �建���������¶��У���ת��һ��ƫ����, ��Ҫ���¶�λ��ʼλ�� */
static void qcache_onCmdSeek( CircularBuff *cb)
{
	txtEncodeParser	 *parser;
	MkLine 			  line;
	CbSlot			 *slot;
	__s32		      ret;
	
	parser = cb->plugin;
	
	circbuf_flush(cb);
	slot   = (CbSlot *)circbuf_getIndex(cb, 0);
	
	parser->start_seek(parser, cb->seekoffset);
	parser->next(parser, parser->start);
	
	while(1)
	{
		g_memset(&line, 0, sizeof(MkLine));
		
		ret = parser->analysis(parser, &line);	//����һ������
//		cbslot_pushValue( slot, &line );		//�������õ�������д�� circbuf		
//		if( ret< 0)
//			break;
		if( ret == -2)
		{
			cbslot_pushValue( slot, &line );	//�������õ�������д�� circbuf
			break;
		}
		else if( ret == -1 )
		{
			break;
		}
		else
		{
			cbslot_pushValue( slot, &line );		//�������õ�������д�� circbuf 	
		}
	}
	
	cb->startFlag = parser->bof;
	cb->endFlag   = parser->eof;
	return;
}

static void qcache_onCmdNextPage( CircularBuff *cb )
{
	CbSlot			 *slot;		
	txtEncodeParser	 *parser;
	MkLine 			  line;
	int 			  start;
	__bool 			  full;	
	__s32			  ret;
	
	parser 	= cb->plugin;
	full 	= circbuf_isFull(cb);
	
	if( (full == EPDK_FALSE )&&( cb->endFlag == EPDK_FALSE) )
	{
		slot  = circbuf_getTail(cb);
		start = cbslot_getEndOffset(slot);
		parser->next( parser, start );
		
		slot  = circbuf_addTail(cb);
		cbslot_flush(slot);
		
		while(1)
		{
			g_memset(&line, 0, sizeof(MkLine));
			
			ret = parser->analysis(parser, &line);	//����һ������
//			cbslot_pushValue( slot, &line );		//�������õ�������д�� circbuf			
//			if( ret< 0)
//				break;		
			if( ret == -2)
			{
				cbslot_pushValue( slot, &line );	//�������õ�������д�� circbuf
				break;
			}
			else if( ret == -1 )
			{
				break;
			}
			else
			{
				cbslot_pushValue( slot, &line );		//�������õ�������д�� circbuf 	
			}	 
		}
		
		cb->endFlag = parser->eof;
		
		__msg(" startFlag = %d, endFlag = %d , qin = %d , qout = %d \n", cb->startFlag, cb->endFlag, cb->qin, cb->qout);
	}		
}

static void qcache_onCmdPrevPage( CircularBuff *cb )
{
	CbSlot			*slot;	
	int 			start;
	__bool 			full;
	__s32		 	ret;
	txtEncodeParser	 *parser;
	MkLine 			  line;

	
	parser 	= cb->plugin;
	full 	= circbuf_isFull(cb);
	
	if( (full == EPDK_FALSE )&&( cb->startFlag == EPDK_FALSE) )
	{
		slot  = circbuf_getHead(cb);
		start = cbslot_getStartOffset(slot);
		parser->prev( parser, start );
		
		slot  = circbuf_addHead(cb);
		cbslot_flush(slot);
		
		while(1)
		{
			g_memset(&line, 0, sizeof(MkLine));
			
			ret = parser->analysis(parser, &line);	//����һ������					
			if( ret == -2)
			{
				cbslot_pushValue( slot, &line );	//�������õ�������д�� circbuf
				break;
			}
			else if( ret == -1 )
			{
				break;
			}
			else
			{
				cbslot_pushValue( slot, &line );		//�������õ�������д�� circbuf 	
			}				
		}
		cb->startFlag = parser->bof;
		
		__msg(" startFlag = %d, endFlag = %d , qin = %d , qout = %d \n", cb->startFlag, cb->endFlag, cb->qin, cb->qout);
	}
	
}

static void qcache_onCmdNext( CircularBuff *cb )
{
	CbSlot			 *slot;	
	int				  start;
	__s32			  ret;
	__bool			  full;
	txtEncodeParser	 *parser;
	MkLine 			  line;
	
	parser 	= cb->plugin;
	full 	= circbuf_isFull(cb);
	
	if( cb->endFlag == EPDK_FALSE )
	{
		slot 	= circbuf_getTail(cb);
		start	= cbslot_getEndOffset(slot);
		
		parser->next(parser, start);
		
		if( full)
		{
			cb->qin = (cb->qin + 1)%CB_MAX_SLOT;
			cb->startFlag = EPDK_FALSE;
		}
		
		slot    = circbuf_addTail(cb);
		cbslot_flush(slot);
			
		while(1)
		{
			g_memset(&line, 0, sizeof(MkLine));
		
			ret = parser->analysis(parser, &line);	//����һ������
//			cbslot_pushValue( slot, &line );		//�������õ�������д�� circbuf 
//			
//			if( ret< 0)
//				break;	
			if( ret == -2)
			{
				cbslot_pushValue( slot, &line );	//�������õ�������д�� circbuf
				break;
			}
			else if( ret == -1 )
			{
				break;
			}
			else
			{
				cbslot_pushValue( slot, &line );		//�������õ�������д�� circbuf 	
			}
		}
		
		cb->endFlag = parser->eof;
		
		__msg(" startFlag = %d, endFlag = %d , qin = %d , qout = %d \n", cb->startFlag, cb->endFlag, cb->qin, cb->qout);		
	}
		
	return;
}

static void qcache_onCmdPrev( CircularBuff *cb )
{	
	CbSlot			 *slot;	
	txtEncodeParser	 *parser;
	int				  start;
	MkLine 			  line;
	__s32			  ret;
	__bool			  full;
	
	
	parser 	= cb->plugin;
	full 	= circbuf_isFull(cb);	
	
	if( cb->startFlag == EPDK_FALSE )
	{		
		slot 	= circbuf_getHead(cb);
		start	= cbslot_getStartOffset(slot);
		
		parser->prev(parser, start);
				
		if( full)
		{
			cb->qout = (cb->qout - 1 + CB_MAX_SLOT)%CB_MAX_SLOT;
			cb->endFlag = EPDK_FALSE;
		}
		
		slot    = circbuf_addHead(cb);
		cbslot_flush(slot);
		
		while(1)
		{
			g_memset(&line, 0, sizeof(MkLine));
		
			ret = parser->analysis(parser, &line);	//����һ������
//			cbslot_pushValue( slot, &line );		//�������õ�������д�� circbuf 				
//			if( ret< 0)
//				break;
			if( ret == -2)
			{
				cbslot_pushValue( slot, &line );	//�������õ�������д�� circbuf
				break;
			}
			else if( ret == -1 )
			{
				break;
			}
			else
			{
				cbslot_pushValue( slot, &line );		//�������õ�������д�� circbuf 	
			}
		}
		
		cb->startFlag = parser->bof;	
		
		__msg(" startFlag = %d, endFlag = %d , qin = %d , qout = %d \n", cb->startFlag, cb->endFlag, cb->qin, cb->qout);		
	}
	
	return;
}

/* д���λ������߳� (���ȼ��ȶ��̵߳�)*/
static void qcache_thread( void *p_arg )
{
	CircularBuff *cb = (CircularBuff *)p_arg;
	QCacheCmd		cmd;
	__u8 			err;
	OS_MBOX_DATA 	mbox_data;	
	
	
	for( ; ; )
	{	
		cmd = (QCacheCmd)(__s32)esKRNL_QPend( cb->cacheQ , 0, &err);
		
		__msg("cmd = %d \n", cmd);
		
		switch(cmd)
		{			
			case QCACHE_CMD_PREV_BUF:			/* ������һ��cache buffer */
				qcache_onCmdPrev(cb);
				
				esKRNL_MboxQuery(cb->respMbox, &mbox_data);				
				if( mbox_data.OSEventGrp != 0)						// ������ȴ�, ���Ӧͬ����Ϣ
				{					
					esKRNL_MboxPost(cb->respMbox,(void *)QCACHE_CMD_PREV_BUF);
				}	
				break;
				
			case QCACHE_CMD_NEXT_BUF:			/* ������һ��cache buffer */
				
				qcache_onCmdNext(cb);
				
				esKRNL_MboxQuery(cb->respMbox, &mbox_data);				
				if( mbox_data.OSEventGrp != 0)						// ������ȴ�, ���Ӧͬ����Ϣ
				{					
					esKRNL_MboxPost(cb->respMbox,(void *)QCACHE_CMD_NEXT_BUF);
				}	
				break;	
					
			case QCACHE_CMD_NEXT_PAGE:		/* ������ζ���û��, �������һ��cache buffer */				
				qcache_onCmdNextPage(cb);				
				break;
				
			case QCACHE_CMD_PREV_PAGE:		/* ������ζ���û��, �������һ��cache buffer */
				qcache_onCmdPrevPage(cb);
				break;	
							
			case QCACHE_CMD_SEEK:			/* ��ת���ܣ���Ҫ���ҵ���ʼλ�ã� */
				qcache_onCmdSeek(cb);
				esKRNL_MboxQuery(cb->respMbox, &mbox_data);				
				if( mbox_data.OSEventGrp != 0)						// ������ȴ�, ���Ӧͬ����Ϣ
				{					
					esKRNL_MboxPost(cb->respMbox,(void *)QCACHE_CMD_SEEK);
				}	
				break;
				
			case QCACHE_CMD_REFRESH:		/* ����������Ϣ */
				
				qcache_onCmdRefresh(cb);
				
				esKRNL_MboxQuery(cb->respMbox, &mbox_data);				
				if( mbox_data.OSEventGrp != 0)						// ������ȴ�, ���Ӧͬ����Ϣ
				{					
					
					esKRNL_MboxPost(cb->respMbox,(void *)QCACHE_CMD_REFRESH);
				}		
						
				break;
				
			case QCACHE_CMD_EXIT:
				esKRNL_MboxPost(cb->respMbox,(void *)QCACHE_CMD_EXIT);
				esKRNL_TDel(OS_PRIO_SELF);
				break;
			default:
				break;			
		}
	}
}



