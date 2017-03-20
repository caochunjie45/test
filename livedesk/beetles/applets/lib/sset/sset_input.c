/*
*********************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           calendar app sample
*
*						        	(c) Copyright 2006-2009, ANDY, China
*											 All Rights Reserved
*
* File    : sset_input.c
* By      :lyn
* Version : V1.00
*********************************************************************************************************
*/
#include "sset_i.h"

/*
************************************************************************************************************************
*                       				_userpara_input
*
*Description: �ݹ�¼�����
*
*Arguments  : 
*
*Return     : 
*            
************************************************************************************************************************
*/
static void _userpara_input(__sset_item_t *pitem, __sset_item_para_t *para)
{
	__u32				i,tmp_cnt;
	__sset_item_t		*pcuritem;
	__sset_item_t		*p_tmp;
	__sset_item_para_t	*p_para;

	p_para = (__sset_item_para_t *)para;

	eLIBs_memcpy( pitem->name, p_para->name, SSET_NAME_MAX );
	pitem->item_sta = p_para->item_sta;
	eLIBs_memcpy( (void *)(&pitem->item_type), (const void *)(&p_para->item_type), sizeof(__sset_item_type_t) );
	eLIBs_memcpy( (void *)(&pitem->item_msg) , (const void *)(&p_para->item_msg) , sizeof(__sset_item_ncmsg_t) );	
	eLIBs_memcpy( (void *)(&pitem->item_attr), (const void *)(&p_para->item_attr), sizeof(__sset_item_attr_t) );	

	tmp_cnt = p_para->n;
	if(tmp_cnt > 0)
	{
		p_para = p_para->childarray;
		
		pcuritem = (__sset_item_t*)esMEMS_Balloc( sizeof(__sset_item_t) );
		eLIBs_memset(pcuritem,0,sizeof(__sset_item_t));
		pcuritem->father = pitem;
		pitem->childlst = pcuritem;
		pcuritem->next = NULL;
		_userpara_input(pcuritem, p_para);
		p_tmp = pcuritem;

		for(i = 1; i < tmp_cnt; i++)
		{		
			p_para++;
			pcuritem = (__sset_item_t*)esMEMS_Balloc( sizeof(__sset_item_t) );
			eLIBs_memset(pcuritem,0,sizeof(__sset_item_t));
			pcuritem->father = pitem;
			pcuritem->childlst = NULL;
			pcuritem->next = NULL;
			pcuritem->prev = p_tmp;
			p_tmp->next = pcuritem;
			_userpara_input(pcuritem, p_para);
			p_tmp = pcuritem;							
		}
	}
}

/*
************************************************************************************************************************
*                       				SSET_ItemAttr_Create
*
*Description: �û�����¼�뺯��
*
*Arguments  : 
*
*Return     : 
*            
************************************************************************************************************************
*/
__sset_item_t* SSET_ItemAttr_Create(__sset_item_para_t	*para)
{
	__u32				i,tmp_cnt;
	__sset_item_t		*phead;
	__sset_item_t		*pcuritem;
	__sset_item_t		*p_tmp;
	__sset_item_para_t	*p_para;

	p_para = (__sset_item_para_t *)para;
	
	phead = (__sset_item_t*)esMEMS_Balloc( sizeof(__sset_item_t) );
	eLIBs_memset( phead, 0, sizeof(__sset_item_t) );
	phead->father = NULL;
	phead->prev = NULL;
	phead->next = NULL;
	phead->childlst = NULL;

	eLIBs_memcpy( (void *)phead->name, (const void *)p_para->name, SSET_NAME_MAX );
	phead->item_sta = p_para->item_sta;
	eLIBs_memcpy( (void *)(&phead->item_type), (const void *)(&p_para->item_type), sizeof(__sset_item_type_t) );	
	eLIBs_memcpy( (void *)(&phead->item_msg) , (const void *)(&p_para->item_msg) , sizeof(__sset_item_ncmsg_t) );	
	eLIBs_memcpy( (void *)(&phead->item_attr), (const void *)(&p_para->item_attr), sizeof(__sset_item_attr_t) );		
	
	tmp_cnt = p_para->n;
	if(tmp_cnt > 0)
	{
		p_para = p_para->childarray;
		
		pcuritem = (__sset_item_t*)esMEMS_Balloc( sizeof(__sset_item_t) );
		eLIBs_memset(pcuritem,0,sizeof(__sset_item_t));
		phead->childlst = pcuritem;
		pcuritem->father = phead;
		pcuritem->childlst = NULL;
		pcuritem->next = NULL;
		pcuritem->prev = NULL;	
		_userpara_input(pcuritem, p_para);
		p_tmp = pcuritem;
		
		for(i = 1; i < tmp_cnt; i++)
		{
			p_para++;
			pcuritem = (__sset_item_t*)esMEMS_Balloc( sizeof(__sset_item_t) );
			eLIBs_memset(pcuritem,0,sizeof(__sset_item_t));
			pcuritem->father = phead;
			pcuritem->childlst = NULL;
			pcuritem->next = NULL;
			pcuritem->prev = p_tmp;
			p_tmp->next = pcuritem;
			_userpara_input(pcuritem, p_para);
			p_tmp = pcuritem;							
		}
	}
	return phead;
}

/*
************************************************************************************************************************
*                       				_userpara_free
*
*Description: �ݹ��ͷ��ڴ�
*
*Arguments  : 
*
*Return     : 
*            
************************************************************************************************************************
*/
static void _userpara_free(__sset_item_t *pitem)
{
	if(pitem->childlst != NULL)
	{
		_userpara_free(pitem->childlst);
		pitem->childlst = NULL;
	}

	if(pitem->next != NULL)
	{
		_userpara_free(pitem->next);
		pitem->next = NULL;
	}
	
	esMEMS_Bfree( pitem, sizeof(__sset_item_t) );
	pitem = NULL;	
}

/*
************************************************************************************************************************
*                       				SSET_ItemAttr_Destroy
*
*Description: �ͷ��û�������ռ�ڴ�
*
*Arguments  : 
*
*Return     : 
*            
************************************************************************************************************************
*/
__s32 SSET_ItemAttr_Destroy( __sset_item_t* phead )
{	
	if( phead )
	{	
		_userpara_free( phead->childlst );
		phead->childlst = NULL;

		esMEMS_Bfree( phead, sizeof(__sset_item_t) );
		phead = NULL;
	}
	return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				SSET_ItemAttr_Insert
*
*Description: ����һ����Ŀ
*
*Arguments  : pitem Ҫ������Ŀ�Ĳ���ڵ�
*			  para	������Ŀ��ͷָ��
*			  num   �������Ŀ��
*
*Return     : 
*            
************************************************************************************************************************
*/
__s32 SSET_ItemAttr_Insert( __sset_item_t* pitem, __sset_item_para_t *para, __u32 num, __sset_item_insert_mode_e mode )
{	
	__u32				i;
	__sset_item_t		*p_item	  = NULL;
	__sset_item_t		*pcuritem = NULL;
	__sset_item_t		*p_father = NULL;
	__sset_item_t		*p_prev	  = NULL;
	__sset_item_t		*p_next	  = NULL;
	__sset_item_para_t	*p_para;

	if(mode == SSET_INSERT_CHLD)
	{
		if(pitem->childlst)
		{
			return EPDK_FAIL;
		}	
		p_father = pitem;
		p_prev 	 = NULL;
		p_next	 = NULL;
	}
	else if(mode == SSET_INSERT_PREV)
	{
		p_father = pitem->father;
		p_prev	= pitem->prev;
		p_next	= pitem;
	}
	else if(mode == SSET_INSERT_NEXT)
	{
		p_father = pitem->father;
		p_prev	= pitem;
		p_next	= pitem->next;
	}
	if(p_father == NULL)
	{
		return EPDK_FAIL;
	}
	
	p_item = pitem;
	p_para = para;

	pcuritem = (__sset_item_t*)esMEMS_Balloc( sizeof(__sset_item_t) );
	p_item->childlst = pcuritem;

	eLIBs_memcpy( pcuritem->name, p_para->name, SSET_NAME_MAX );
	pcuritem->item_sta = p_para->item_sta;
	eLIBs_memcpy( (void *)(&pcuritem->item_type), (const void *)(&p_para->item_type), sizeof(__sset_item_type_t) );		
	eLIBs_memcpy( (void *)(&pcuritem->item_msg) , (const void *)(&p_para->item_msg) , sizeof(__sset_item_ncmsg_t) );	
	eLIBs_memcpy( (void *)(&pcuritem->item_attr), (const void *)(&p_para->item_attr), sizeof(__sset_item_attr_t) );		

	if(mode == SSET_INSERT_CHLD)
	{
		pcuritem->father = p_father;
		pcuritem->childlst = NULL;
		pcuritem->next = p_next;
		pcuritem->prev = p_prev;
		p_prev = pcuritem;
	}
	else
	{
		pcuritem->father = p_father;
		pcuritem->childlst = NULL;
		pcuritem->next = p_next;
		pcuritem->prev = p_prev;
		p_prev->next = pcuritem;
		p_prev = pcuritem;		
	}	
			
	for(i = 1; i < num ; i++)
	{
		p_para++;	
		pcuritem = (__sset_item_t*)esMEMS_Balloc( sizeof(__sset_item_t) );
	
		eLIBs_memcpy( pcuritem->name, p_para->name, SSET_NAME_MAX );
		pcuritem->item_sta = p_para->item_sta;
		eLIBs_memcpy( (void *)(&pcuritem->item_type), (const void *)(&p_para->item_type), sizeof(__sset_item_type_t) );	
		eLIBs_memcpy( (void *)(&pcuritem->item_msg) , (const void *)(&p_para->item_msg) , sizeof(__sset_item_ncmsg_t) );	
		eLIBs_memcpy( (void *)(&pcuritem->item_attr), (const void *)(&p_para->item_attr), sizeof(__sset_item_attr_t) );	
	
		if(mode == SSET_INSERT_CHLD)
		{
			pcuritem->father = p_father;
			pcuritem->childlst = NULL;
			pcuritem->next = NULL;
			pcuritem->prev = p_prev;
			p_prev->next = pcuritem;
			p_prev = pcuritem;
		}
		else
		{
			pcuritem->father = p_father;
			pcuritem->childlst = NULL;
			pcuritem->next = p_next;
			pcuritem->prev = p_prev;
			p_prev->next = pcuritem;
			p_prev = pcuritem;		
		}		
	}
	return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				SSET_ItemAttr_Delete
*
*Description: ɾ��һ����Ŀ
*
*Arguments  : pitem ɾ����Ŀ�����صĽڵ�
*
*Return     : 
*            
************************************************************************************************************************
*/
__s32 SSET_ItemAttr_Delete( __sset_item_t* pitem, __sset_item_delete_mode_e mode )
{	
	__sset_item_t		*p_item;	
	
	p_item = pitem;
	if(p_item != NULL)
	{
		if(p_item->childlst)
		{
			_userpara_free( p_item->childlst );
			p_item->childlst = NULL;
		}
		if(mode == SSET_DELETE_CHLD)
		{
			return EPDK_OK;
		}
		if(p_item->next)
		{
			if(p_item->prev)
			{
				p_item->prev->next = p_item->next;
				p_item->next->prev = p_item->prev;
			}
			else
			{
				p_item->father->childlst = p_item->next;
				p_item->next->prev = NULL;
			}
		}
		else
		{
			p_item->prev->next = NULL;
		}	
		esMEMS_Bfree( p_item, sizeof(__sset_item_t) );
		p_item = NULL;
	}
	
	return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				_userpara_search
*
*Description: �ͷ��û�������ռ�ڴ�
*
*Arguments  : 
*
*Return     : 
*            
************************************************************************************************************************
*/
static __sset_item_t* _userpara_search(char *pname, char *ptmp, __sset_item_t* pitem)
{
	__u32				str_len;
	__u32				str_len1;
	__s32				ret;
	char				*p_name = NULL;
	char				*p_tmp = NULL;
	__sset_item_t		*pcuritem = NULL;
	__sset_item_t		*p_ret;
	
	p_name		= pname;
	pcuritem 	= pitem;	
	while( pcuritem )
	{
		p_tmp = eLIBs_strchr(ptmp, '\\');
		if(p_tmp == NULL)
		{
			str_len = eLIBs_strlen(p_name);
		}
		else	
		{
			str_len = p_tmp - p_name;
			p_tmp++;
		}
		str_len1 = eLIBs_strlen(pcuritem->name);
		if(str_len == str_len1)
		{
			ret = eLIBs_strncmp(p_name, pcuritem->name, str_len1);
			if(ret == 0)
			{
				if(str_len1 == eLIBs_strlen(p_name))
				{
					return pcuritem;
				}
				if(pcuritem->childlst)
				{				
					p_ret = _userpara_search(p_name, p_tmp, pcuritem->childlst);
					return p_ret;
				}			
				return 0;
			}
		}
		pcuritem = pcuritem->next;
	}

	return 0;
}

/*
************************************************************************************************************************
*                       				SSET_ItemAttr_Search
*
*Description: �ͷ��û�������ռ�ڴ�
*
*Arguments  : 
*
*Return     : 
*            
************************************************************************************************************************
*/
__sset_item_t* SSET_ItemAttr_Search( char *name, __sset_item_t* pitem )
{
	char				*p_name;
	__sset_item_t		*pcuritem;
	__sset_item_t		*p_ret = NULL;
	
	if( pitem == NULL )
	{
		return 0;
	}
	p_name		= name;	
	pcuritem	= pitem;
	
	p_ret = _userpara_search(p_name, p_name, pcuritem);
	return p_ret;
}

/*
************************************************************************************************************************
*                       				SSET_ItemAttr_Ennable
*
*Description: �ͷ��û�������ռ�ڴ�
*
*Arguments  : 
*
*Return     : 
*            
************************************************************************************************************************
*/
__s32 SSET_ItemAttr_Ennable( __sset_item_t* pitem )
{	
	if( pitem == NULL )
	{
		return EPDK_FAIL;
	}

	pitem->item_sta = 1;
	
	return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				SSET_ItemAttr_Disable
*
*Description: �ͷ��û�������ռ�ڴ�
*
*Arguments  : 
*
*Return     : 
*            
************************************************************************************************************************
*/
__s32 SSET_ItemAttr_Disable( __sset_item_t* pitem )
{
	if( pitem == NULL )
	{
		return EPDK_FAIL;
	}

	pitem->item_sta = 0;
	
	return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				SSET_Item_SetAttr
*
*Description: �ͷ��û�������ռ�ڴ�
*
*Arguments  : 
*
*Return     : 
*            
************************************************************************************************************************
*/
__s32 SSET_Item_SetAttr( __sset_item_t* pitem, __sset_item_attr_t *attr )
{
	if( pitem == NULL )
	{
		return EPDK_FAIL;
	}

	eLIBs_memcpy( &(pitem->item_attr), attr, sizeof(__sset_item_attr_t) );		

	return EPDK_OK;
}

/*
************************************************************************************************************************
*                       				SSET_Item_GetAttr
*
*Description: �ͷ��û�������ռ�ڴ�
*
*Arguments  : 
*
*Return     : 
*            
************************************************************************************************************************
*/
__s32 SSET_Item_GetAttr( __sset_item_t* pitem, __sset_item_attr_t *attr )
{
	if( pitem == NULL )
	{
		return EPDK_FAIL;
	}

	eLIBs_memcpy( attr, &(pitem->item_attr), sizeof(__sset_item_attr_t) );	

	return EPDK_OK;
}






