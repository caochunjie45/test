

#include "host_win.h"



int Add_New_Hosted_Win(Mixed_CtrlWin_T **pFirstHosted, Mixed_CtrlWin_T *pHosted)
{
    Mixed_CtrlWin_T *head = NULL;
    
    Mixed_CtrlWin_T *prev = NULL;

	if(NULL == pFirstHosted)
	{
		eDbug("pFirstHosted is NULL \n");
		return NULL;
	}

	if(NULL == pHosted)
	{
		eDbug("pHosted is NULL \n");
		return NULL;
	}

   
    head = *pFirstHosted;
    
    if (head)
    {
        while(head) 
        {
            prev = head;
            head = head->pNextHosted;
        }

        prev->pNextHosted = pHosted;
    }
    else
    {
        *pFirstHosted = pHosted;
    }

	eDbug("pHosted is 0x%x \n",pHosted);

    pHosted->pNextHosted = NULL;

    return EPDK_OK;
}


Mixed_CtrlWin_T *  Get_Last_Hosted_Win(Mixed_CtrlWin_T *pFirstHosted)
{
    Mixed_CtrlWin_T *pHosted = NULL;

	if(NULL == pFirstHosted)
	{
		eDbug("pFirstHosted is NULL \n");
		return NULL;
	}

	pHosted = pFirstHosted;

	while(pHosted->pNextHosted)
	{
		pHosted = pHosted->pNextHosted;		
	}

	eDbug("pHosted is 0x%x \n",pHosted);
  
    return pHosted;
}
      

Mixed_CtrlWin_T *  Get_Next_Hosted_Win(Mixed_CtrlWin_T *pHosted)
{
	if(NULL == pHosted)
	{
		eDbug("pHosted is NULL \n");
		return NULL;
	}

    return pHosted->pNextHosted;
    
}



Mixed_CtrlWin_T *  Get_Prev_Hosted_Win(Mixed_CtrlWin_T *pFirstHosted,Mixed_CtrlWin_T *pHosted)
{
	Mixed_CtrlWin_T *tmp = NULL;
	
	if(NULL == pFirstHosted)
	{
		eDbug("pFirstHosted is NULL \n");
		return NULL;
	}

	if(NULL == pHosted)
	{
		eDbug("pHosted is NULL \n");
		return NULL;
	}

	tmp = pFirstHosted;

	while(tmp)
	{
		if(pHosted == tmp->pNextHosted)
		{
			return tmp;
		}

		tmp = tmp->pNextHosted;
	}

	
    return NULL;
}


Mixed_CtrlWin_T *  Find_Hosted_Win(Mixed_CtrlWin_T *pFirstHosted,__u32 ID)
{
	Mixed_CtrlWin_T *tmp = NULL;
	
	if(NULL == pFirstHosted)
	{
		eDbug("pFirstHosted is NULL \n");
		return NULL;
	}


	tmp = pFirstHosted;

	while(tmp)
	{
		if(ID == tmp->ID)
		{
			return tmp;
		}

		tmp = tmp->pNextHosted;
	}

	
    return NULL;
}



int destory_Hosting_Win(Mixed_CtrlWin_T **pFirstHosted)
{
    Mixed_CtrlWin_T *move = NULL;
    
    Mixed_CtrlWin_T *tmp = NULL;

	if(NULL == pFirstHosted)
	{
		eDbug("pFirstHosted is NULL \n");
		return NULL;
	}

	if(NULL == *pFirstHosted)
	{
		eDbug("*pFirstHosted is NULL \n");
		return NULL;
	}


    move = *pFirstHosted;

    while(move)
    {
        tmp = move->pNextHosted;
        
		switch(move->type_ctrl)
		{
			case TYPE_BUTTON:
			{
				move->destory(&(move->mix_ctrl.Button));
			}
			break;
			case TYPE_SLIDER:
			{
				move->destory(&(move->mix_ctrl.Slider));			
			}
			break;
			case TYPE_STATICS:
			{
				move->destory(&(move->mix_ctrl.Statics));			
			}
			break;
			case TYPE_LISTBAR:
			{
				move->destory(&(move->mix_ctrl.Listbar));
			}
			break;

			case TYPE_PROGBARS:
			{
				move->destory(&(move->mix_ctrl.Progbars));
			}
			break;
			
			default:
			{

			}
			break;
		}
		
		In_Free(move,sizeof(Mixed_CtrlWin_T));
        
        move = tmp;
    }

	*pFirstHosted = NULL;
	
    return EPDK_OK;
}

int destory_Last_Hosted_Win(Mixed_CtrlWin_T **pFirstHosted)
{
    Mixed_CtrlWin_T *pHosted = NULL;
    
    Mixed_CtrlWin_T *prev = NULL;

	if(NULL == pFirstHosted)
	{
		eDbug("pFirstHosted is NULL \n");
		return NULL;
	}

	if(NULL == *pFirstHosted)
	{
		eDbug("*pFirstHosted is NULL \n");
		return NULL;
	}

	pHosted = *pFirstHosted;

	if(NULL == (*pFirstHosted)->pNextHosted)
	{
		*pFirstHosted = NULL;		
	}
	
	while(pHosted->pNextHosted)
	{
		prev = pHosted;
		pHosted = pHosted->pNextHosted;		
	}
	
	prev->pNextHosted = NULL;
	
	
	switch(pHosted->type_ctrl)
	{
		case TYPE_BUTTON:
		{
			pHosted->destory(&(pHosted->mix_ctrl.Button));
		}
		break;
		
		case TYPE_SLIDER:
		{
			pHosted->destory(&(pHosted->mix_ctrl.Slider));			
		}
		break;
		
		case TYPE_STATICS:
		{
			pHosted->destory(&(pHosted->mix_ctrl.Statics));			
		}
		break;
		
		case TYPE_LISTBAR:
		{
			pHosted->destory(&(pHosted->mix_ctrl.Listbar));
		}
		break;

		case TYPE_PROGBARS:
		{
			pHosted->destory(&(pHosted->mix_ctrl.Progbars));
		}
		break;
		
		default:
		{

		}
		break;
	}

	eDbug("pHosted is 0x%x \n",pHosted);
	
	In_Free(pHosted,sizeof(Mixed_CtrlWin_T));



    return 0;
}

