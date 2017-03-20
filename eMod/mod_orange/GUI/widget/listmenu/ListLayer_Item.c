/*
*******************************************************************************************************************
*                                                         	display driver 
*                                         			the display dirver support module
*
*                             				 Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
*											               All Rights Reserved
*
*File Name��    ListLayer_Item.c
*
*Author��       William Wang
*
*Version :      1.1.0
*
*Date:          2008-8-25
*
*Description :  listlayer interface implement function 
*
*Others :       None at present.
*
* History :
*
* <Author>          <time>          <version>     <description>
*
* William Wang     2008-8-25          1.1.0        Create File
*
*******************************************************************************************************************
*/
#ifndef __LIST_LAYER_ITEM_C____
#define __LIST_LAYER_ITEM_C____

#include "ListLayer_Item.h"
#include "ListBox.h"

extern ES_FILE * gdispfile;    
/*
**********************************************************************************************************************
*                                               ListLayer_UpdateHead
*
* author:           
*
* date:             2009-1-30
*
* Description:      listlayer update head  ɾ��item����µ�ǰItem���ϰ벿
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

static  __u32  ListLayer_DelUpdateHead(LIST_LAYER_MAN_t *   listlayer)
{
    __u32                count;
    __u32                i;
	__pos_t              cur_pos;
    MENUITEM_t *         tmpItem;       

	/*�ӽ���λ�ÿ�ʼ����*/
    cur_pos.x 			= listlayer->foucs_pos.x;
	cur_pos.y 			= listlayer->foucs_pos.y;

	/*��������*/
    count 				= listlayer->upcount + 1 + listlayer->scnnum * (listlayer->fb_depth/2 - 1);
	tmpItem 			= listlayer->menulist->CurItem;
	listlayer->fillup 	= 0;
	
    for(i = 0;i < count;i++)
    {    
    	/*��Ҫ���Ƶ�item,�������һ��item*/
        if(tmpItem != NULL &&tmpItem != listlayer->menulist->tail)
        {
        	ListLayer_FastLoadMenuItem(listlayer,tmpItem,0,cur_pos.x,cur_pos.y);
			tmpItem 	= tmpItem->Prev;
			cur_pos.x 	= cur_pos.x;
			cur_pos.y 	= cur_pos.y - listlayer->menulist->head->rect.height;
			if(cur_pos.y < 0)
		    {
                ListLayer_HeadToTailCopy(listlayer);
		    	cur_pos.y = cur_pos.y + listlayer->listbuffer->vrect.height;
		    }
			listlayer->menulist->load_head 	= tmpItem;
			listlayer->head_pos.x 			= cur_pos.x;
			listlayer->head_pos.y 			= cur_pos.y;
        }
		else
	    {
	    	/*���е�item���Ѿ�ɾ����ɣ����framebuffer*/
			if(tmpItem == NULL)
			{
				#if 0
				void *               base;
	  			__u32                bpp;
	   			__u32                size;
	   
				bpp = listlayer->menulist->bpp;

				size = (listlayer->listbuffer->rectsz.width * 
				        listlayer->listbuffer->rectsz.height * bpp + 7)/8;

				base = (void *)((__u32)listlayer->listbuffer->buffer);
				if(listlayer->plistmenu->lyrsts != LISTMENU_SLEEP)
				{
					/*���framebuffer*/
				    eLIBs_memset(base, 0x00, size);
				}
                #else
                GUI_LyrWinSel(listlayer->layer);
                GUI_SetBkColor(listlayer->bk_color);
                GUI_ClearRect(0,0,listlayer->listbuffer->rectsz.width - 1,listlayer->listbuffer->rectsz.height - 1);
                #endif
			    listlayer->menulist->load_tail 	= NULL;
				listlayer->menulist->load_head 	= NULL;
				listlayer->tail_pos.x 			= 0;
				listlayer->tail_pos.y 			= 0;
				break;
			}
			else if(i == 0)
			{
				ListLayer_FastLoadMenuItem(listlayer,tmpItem,0,cur_pos.x,cur_pos.y);
				tmpItem = tmpItem->Prev;
				cur_pos.x = cur_pos.x;
				cur_pos.y = cur_pos.y - listlayer->menulist->head->rect.height;
				if(cur_pos.y < 0)
			    {
	                ListLayer_HeadToTailCopy(listlayer);
			    	cur_pos.y = cur_pos.y + listlayer->listbuffer->vrect.height;
			    }
				listlayer->menulist->load_head = tmpItem;
				listlayer->head_pos.x = cur_pos.x;
				listlayer->head_pos.y = cur_pos.y;
			}
			else
			{
				listlayer->menulist->load_head = listlayer->menulist->tail;
				ListLayer_ClearMenuItem(listlayer, 0, cur_pos.x, cur_pos.y); 
				listlayer->fillup++;
				cur_pos.x = cur_pos.x;				
				cur_pos.y = cur_pos.y - tmpItem->rect.height;
				if(cur_pos.y < 0)
			    {
	                ListLayer_HeadToTailCopy(listlayer);
			    	cur_pos.y = cur_pos.y + listlayer->listbuffer->vrect.height;
			    }
				listlayer->head_pos.x = cur_pos.x;
				listlayer->head_pos.y = cur_pos.y;
				if(listlayer->fillup == listlayer->upcount)
				{
					break;
				}
			}		
	    }
    }  
    
    return OK;
}
      
/*
**********************************************************************************************************************
*                                               ListLayer_UpdateDraw
*
* author:           
*
* date:             2009-1-30
*
* Description:      listlayer update draw (load item to tail)ɾ��item����µ�ǰItem���°벿
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

static __u32 ListLayer_DelUpdateTail(LIST_LAYER_MAN_t *   listlayer)
{
    __u32                count;
    __u32                i;
	__pos_t              cur_pos;
    MENUITEM_t *         tmpItem;       

    cur_pos.x 			= listlayer->foucs_pos.x;
	cur_pos.y 			= listlayer->foucs_pos.y;
    count 				= listlayer->downcount + 1 + listlayer->scnnum * (listlayer->fb_depth/2 - 1);
	tmpItem 			= listlayer->menulist->CurItem;
	listlayer->filldown = 0;
	
    for(i = 0;i < count;i++)
    {    
        if(tmpItem != NULL &&tmpItem != listlayer->menulist->head)
        {
        	ListLayer_FastLoadMenuItem(listlayer,tmpItem,0,cur_pos.x,cur_pos.y);
			tmpItem = tmpItem->Next;
			cur_pos.x = cur_pos.x;
			cur_pos.y = cur_pos.y + listlayer->menulist->head->rect.height;
			if(cur_pos.y >= listlayer->listbuffer->rectsz.height)
		    {
                ListLayer_TailToHeadCopy(listlayer);
		    	cur_pos.y = cur_pos.y - listlayer->listbuffer->vrect.height;
		    }
			listlayer->menulist->load_tail 	= tmpItem;
			listlayer->tail_pos.x 			= cur_pos.x;
			listlayer->tail_pos.y 			= cur_pos.y;
        }
		else
	    {
			if(tmpItem == NULL)
			{
				#if 0
				void *               base;
	  			__u32                bpp;
	   			__u32                size;
	   
				bpp = listlayer->menulist->bpp;

				size = (listlayer->listbuffer->rectsz.width * 
				        listlayer->listbuffer->rectsz.height * bpp + 7)/8;

				base = (void *)((__u32)listlayer->listbuffer->buffer);
				if(listlayer->plistmenu->lyrsts != LISTMENU_SLEEP)
				{
					/*���framebuffer*/
				    eLIBs_memset(base, 0x00, size);
				}
                #else
                GUI_LyrWinSel(listlayer->layer);
                GUI_SetBkColor(listlayer->bk_color);
                GUI_ClearRect(0,0,listlayer->listbuffer->rectsz.width - 1,listlayer->listbuffer->rectsz.height - 1);
                #endif
			    listlayer->menulist->load_tail 	= NULL;
				listlayer->menulist->load_head 	= NULL;
				listlayer->tail_pos.x 			= 0;
				listlayer->tail_pos.y 			= 0;
				break;
			}
			else if(i == 0)
			{
				ListLayer_FastLoadMenuItem(listlayer,tmpItem,0,cur_pos.x,cur_pos.y);
				tmpItem = tmpItem->Next;
				cur_pos.x = cur_pos.x;
				cur_pos.y = cur_pos.y + listlayer->menulist->head->rect.height;
				if(cur_pos.y >= listlayer->listbuffer->rectsz.height)
			    {
	                ListLayer_TailToHeadCopy(listlayer);
			    	cur_pos.y = cur_pos.y - listlayer->listbuffer->vrect.height;
			    }
				listlayer->menulist->load_tail = tmpItem;
				listlayer->tail_pos.x = cur_pos.x;
				listlayer->tail_pos.y = cur_pos.y;
			}
			else
			{
				listlayer->menulist->load_tail = listlayer->menulist->head;
				ListLayer_ClearMenuItem(listlayer, 0, cur_pos.x, cur_pos.y); 
				listlayer->filldown++;
				cur_pos.x = cur_pos.x;				
				cur_pos.y = cur_pos.y + tmpItem->rect.height;
				if(cur_pos.y >= listlayer->listbuffer->rectsz.height)
			    {
	                ListLayer_TailToHeadCopy(listlayer);
			    	cur_pos.y = cur_pos.y - listlayer->listbuffer->vrect.height;
			    }
				listlayer->tail_pos.x = cur_pos.x;
				listlayer->tail_pos.y = cur_pos.y;
				if(listlayer->filldown == listlayer->downcount)
				{
					break;
				}
			}		
	    }
    }  
    
    return OK;
}

/*
**********************************************************************************************************************
*                                               ListLayer_UpdateDraw
*
* author:           
*
* date:             2009-1-30
*
* Description:      listlayer update draw (load item to tail)ɾ��item����µ�ǰItem���°벿
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

static __u32 ListLayer_AddUpdateTail(LIST_LAYER_MAN_t *   listlayer)
{
    __u32                count;
    __u32                i;
	__pos_t              cur_pos;
    MENUITEM_t *         tmpItem;       

    /*ֻ��һ��Item���������ʱ���ǰΪ��listmenu*/
    if(listlayer->menulist->ItemCount == 1)
    {
        ListLayer_OnDraw(listlayer);
        ListLayer_SetWinFoucs(listlayer);
    }
    else
    {
        listlayer->tail_pos.x   = listlayer->foucs_pos.x;
        listlayer->tail_pos.y   = listlayer->foucs_pos.x + listlayer->menulist->CurItem->rect.height;
        cur_pos.x               = listlayer->tail_pos.x;
        cur_pos.y               = listlayer->tail_pos.y;

        count 				    = listlayer->downcount + listlayer->scnnum * (listlayer->fb_depth/2 - 1);
    	tmpItem 			    = listlayer->menulist->CurItem->Next;
    	listlayer->filldown     = 0;
    	
        for(i = 0;i < count;i++)
        {    
            if(tmpItem != listlayer->menulist->head)
            {
                if(tmpItem == listlayer->menulist->tail)
                {
                	ListLayer_FastLoadMenuItem(listlayer,tmpItem,0,cur_pos.x,cur_pos.y);
        			cur_pos.x = cur_pos.x;
        			cur_pos.y = cur_pos.y + listlayer->menulist->head->rect.height;
        			if(cur_pos.y >= listlayer->listbuffer->rectsz.height)
        		    {
                        ListLayer_TailToHeadCopy(listlayer);
        		    	cur_pos.y = cur_pos.y - listlayer->listbuffer->vrect.height;
        		    }
                }
                else
                {
                    cur_pos.x = cur_pos.x;
        			cur_pos.y = cur_pos.y + listlayer->menulist->head->rect.height;
                    if(cur_pos.y >= listlayer->listbuffer->rectsz.height)
        		    {
        		    	cur_pos.y = cur_pos.y - listlayer->listbuffer->vrect.height;
        		    }
                }
                tmpItem                         = tmpItem->Next;
                listlayer->menulist->load_tail 	= tmpItem;
    			listlayer->tail_pos.x 			= cur_pos.x;
    			listlayer->tail_pos.y 			= cur_pos.y;
                
            }
    		else
    	    {
    			listlayer->menulist->load_tail = listlayer->menulist->head;
				ListLayer_ClearMenuItem(listlayer, 0, cur_pos.x, cur_pos.y); 
				listlayer->filldown++;
				cur_pos.x = cur_pos.x;				
				cur_pos.y = cur_pos.y + tmpItem->rect.height;
				if(cur_pos.y >= listlayer->listbuffer->rectsz.height)
			    {
	                ListLayer_TailToHeadCopy(listlayer);
			    	cur_pos.y = cur_pos.y - listlayer->listbuffer->vrect.height;
			    }
				listlayer->tail_pos.x = cur_pos.x;
				listlayer->tail_pos.y = cur_pos.y;
				if(listlayer->filldown == listlayer->downcount)
				{
					break;
				}		
    	    }
        }  
    }
    
    
    return OK;
}
      
/*
**********************************************************************************************************************
*                                               ListLayer_DelCurItem
*
* author:           
*
* date:             2009-1-30
*
* Description:      listlayer delcuritem 
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

__u32  ListLayer_DelCurItem(LIST_LAYER_MAN_t *   listlayer)
{
    __u32                ret;
	MENUITEM_t*          NewCurItem;

    if(listlayer == NULL)
    {
        return __LINE__;
    }

	if(listlayer->menulist->CurItem == NULL)
	{
		return OK;
	}

    if(listlayer->mode == FIXED_MODE)
    {
        return OK;
    }
    
    /*update head information,��Ҫɾ����item��βitem����ʱ��Ҫ�����ϰ벿*/
    if(listlayer->menulist->CurItem == listlayer->menulist->tail)
    {
    	NewCurItem = listlayer->menulist->CurItem->Prev;
		if(NewCurItem == listlayer->menulist->CurItem)
		{
			NewCurItem = NULL;
		}
        ret = MenuList_DelItem(listlayer->menulist, listlayer->menulist->CurItem);
		if(ret != OK)
		{
			return __LINE__;
		}

		if(NewCurItem != NULL)
		{
			listlayer->menulist->CurItem = NewCurItem;
			ListLayer_DelUpdateHead(listlayer);
			//LISTMENU_PRINTF("foucs x = %d,foucs y = %d\n",listlayer->foucs_pos.x,listlayer->foucs_pos.y);
            ret = ListLayer_DrawFastFoucsMenuItem(listlayer,listlayer->menulist->CurItem,
                                            0,listlayer->foucs_pos.x,listlayer->foucs_pos.y);
            if(ret != OK)
            {
                return __LINE__;
            }
            //LISTMENU_PRINTF("foucs x = %d,foucs y = %d\n",listlayer->foucs_pos.x,listlayer->foucs_pos.y);
            ret = ListLayer_LoadMenuItem(listlayer,listlayer->menulist->CurItem,
                                            0,listlayer->foucs_pos.x,listlayer->foucs_pos.y);
            if(ret != OK)
            {
                return __LINE__;
            }
                  
            listlayer->setfoucs = REAL_FOUCS;
		}
		else
		{
			listlayer->menulist->CurItem = NewCurItem;
			ListLayer_DelUpdateHead(listlayer);	
            
			return OK;
		}
    }
    else  /*update tail information����Ҫ���²������һ��item����ʱ��Ҫ�����°벿*/
    {
		/*��ȡ��item�Ĵ��ھ��*/
        NewCurItem = listlayer->menulist->CurItem->Next;

		/*ֻ��һ��item�����*/
		if(NewCurItem == listlayer->menulist->CurItem)
		{
			NewCurItem = NULL;
		}

		/*��menulist��ɾ����ǰitem*/
        ret = MenuList_DelItem(listlayer->menulist, listlayer->menulist->CurItem);
		if(ret != OK)
		{
			return __LINE__;
		}

		if(NewCurItem != NULL)
		{
			listlayer->menulist->CurItem = NewCurItem;

			/*�����°벿*/
			ListLayer_DelUpdateTail(listlayer);
			//LISTMENU_PRINTF("foucs x = %d,foucs y = %d\n",listlayer->foucs_pos.x,listlayer->foucs_pos.y);
            ret = ListLayer_DrawFastFoucsMenuItem(listlayer,listlayer->menulist->CurItem,
                                            0,listlayer->foucs_pos.x,listlayer->foucs_pos.y);
            if(ret != OK)
            {
                return __LINE__;
            }
            //LISTMENU_PRINTF("foucs x = %d,foucs y = %d\n",listlayer->foucs_pos.x,listlayer->foucs_pos.y);
            ret = ListLayer_LoadMenuItem(listlayer,listlayer->menulist->CurItem,
                                            0,listlayer->foucs_pos.x,listlayer->foucs_pos.y);
            if(ret != OK)
            {
                return __LINE__;
            }
                  
            listlayer->setfoucs = REAL_FOUCS;
		}
		else/*ֻ��һ��item�����*/
		{
			listlayer->menulist->CurItem = NewCurItem;
			ListLayer_DelUpdateTail(listlayer);	

			return OK;
		}
    }  

	return OK;
}

/*
**********************************************************************************************************************
*                                               ListLayer_DelCurItem
*
* author:           
*
* date:             2009-1-30
*
* Description:      listlayer additem 
*
* parameters:       
*
* return:           if success return ERDK_OK
*                   if fail return the number of fail
* modify history: 
**********************************************************************************************************************
*/

__u32  ListLayer_AddItemTail(LIST_LAYER_MAN_t *listlayer,__lstm_item_attr_t *NewItem)
{
    __u32                					ret;
	MENUITEM_t           					*menuitem;
    __rectsz_t           					rect;

    if(listlayer == NULL || NewItem == NULL)
    {
        return __LINE__;
    }

    rect.width                     = listlayer->listbuffer->rectsz.width;
    rect.height                    = NewItem->item_w_unfocs;
    
    /*MenuItem Create*/
    menuitem                       = MenuItem_Create(&rect,NewItem,listlayer->plistmenu->handle);
    if(!menuitem)
    {
        ORANGE_WRN("create menuitem failed!\n");

        return __LINE__;
    }
	
	/*add new item to menulist*/
	ret = MenuList_AddTail(listlayer->menulist,menuitem);
	if(ret != OK)
	{
		return __LINE__;
	}

    ListLayer_AddUpdateTail(listlayer);

	return OK;
}

#endif

