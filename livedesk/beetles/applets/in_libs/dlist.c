
#include "dlist.h"


dlist_t *Dlist_Create(int len, dlist_op_t destroy)
{
	dlist_t *list;

	list = In_Malloc(sizeof(*list));
	if (NULL == list)
		return NULL;

	list->head = NULL;
	
	list->len = len;
	
	list->cnt = 0;
	
	list->destroy = destroy;

	list->sem = esKRNL_SemCreate(1);
	
	return list;
}


int Dlist_Count(dlist_t *list)
{
	return list->cnt;
}



static struct node_t *__Alloc_Node(void *data, int len)
{
	struct node_t *n;

	n = In_Malloc(sizeof(*n));
	if (NULL == n)
		goto err0;
		
	n->data = data;

	return n;
err0:
	return NULL;
}



int Dlist_Append(dlist_t *list, void *data)
{
	struct node_t *n;

	if (NULL == (n = __Alloc_Node(data, list->len)))
		return -1;

	if (NULL == list->head)
	{
		n->next = n;	
		n->prev = n;
		list->head = n;
	}
	else
	{
		n->next = list->head;	
		n->prev = list->head->prev;
		list->head->prev->next = n;
		list->head->prev = n;
	}

	list->cnt++;

	return 0;
}



int Dlist_Prepend(dlist_t *list, void *data)
{
	if (0 != Dlist_Append(list, data))
		return -1;

	list->head = list->head->prev;

	return 0;
}



int Dlist_Insert(dlist_t *list, void *data, int idx)
{
	int i;
	struct node_t *n;
	struct node_t *tmp;

	if (idx < 0)
		idx = 0;
	if (idx >= list->cnt)
		idx = list->cnt;

	i = 0, tmp = list->head;
	while (i < idx)
		i++, tmp = tmp->next;

	if (NULL == (n = __Alloc_Node(data, list->len)))
		return -1;

	if (NULL == tmp)
	{
		n->next = n;
		n->prev = n;
		list->head = n;
	}
	else
	{
		n->next = tmp;	
		n->prev = tmp->prev;
		tmp->prev->next = n;
		tmp->prev = n;
	}

	if (0 == idx)
		list->head = n;

	list->cnt++;

	return 0;
}



int Dlist_Retrieve(dlist_t *list, void *data, int idx)
{
	int i;
	struct node_t *tmp;

	esKRNL_SemPend(list->sem,0,NULL);
	
	if (idx < 0 || idx > list->cnt - 1)
	{
		esKRNL_SemPost(list->sem);
		
		return -1;
	}

	
	for (i = 0, tmp = list->head; i < idx; i++, tmp = tmp->next)
		;

	memcpy(data, tmp->data, list->len);

	esKRNL_SemPost(list->sem);
	
	return 0;
}


void * Dlist_Retrieve_Pointer(dlist_t *list,int idx)
{
	int i;
	
	struct node_t *tmp;

	esKRNL_SemPend(list->sem,0,NULL);

	if (idx < 0 || idx > list->cnt - 1)
	{
		esKRNL_SemPost(list->sem);
		
		return NULL;
	}

	
	for (i = 0, tmp = list->head; i < idx; i++, tmp = tmp->next)
		;


	esKRNL_SemPost(list->sem);

	return tmp->data;
}


int Dlist_Modify(dlist_t *list, void *data, int idx)
{
	int i;
	
	struct node_t *tmp;

	esKRNL_SemPend(list->sem,0,NULL);

	if (idx < 0 || idx > list->cnt - 1)
	{
		esKRNL_SemPost(list->sem);

		return -1;
	}
	
	for (i = 0, tmp = list->head; i < idx; i++, tmp = tmp->next)
		;

	memcpy(tmp->data,data,list->len);

	esKRNL_SemPost(list->sem);

	return 0;
}



int Dlist_Find(dlist_t *list, void *key, dlist_cmp_t cmp)
{
	int i;
	
	struct node_t *tmp;

	esKRNL_SemPend(list->sem,0,NULL);

	for (i = 0, tmp = list->head; i < list->cnt; i++, tmp = tmp->next)
	{
		if (cmp(key, tmp->data) == 0)
		{			
			esKRNL_SemPost(list->sem);

			return i;
		}
	}

	esKRNL_SemPost(list->sem);

	return -1;
}


void * Dlist_Find_Pointer(dlist_t *list, void *key, dlist_cmp_t cmp)
{
	int i;
	
	struct node_t *tmp;

	esKRNL_SemPend(list->sem,0,NULL);

	for (i = 0, tmp = list->head; i < list->cnt; i++, tmp = tmp->next)
	{
		if (cmp(key, tmp->data) == 0)
		{						
			esKRNL_SemPost(list->sem);

			return tmp->data;
		}
	}
	
	esKRNL_SemPost(list->sem);

	return NULL;
}


int Dlist_Find_Element(dlist_t *list,void *data,void *key, dlist_cmp_t cmp)
{
	int i;
	
	struct node_t *tmp;

	esKRNL_SemPend(list->sem,0,NULL);

	for (i = 0, tmp = list->head; i < list->cnt; i++, tmp = tmp->next)
	{
		if (cmp(key, tmp->data) == 0)
		{
			memcpy(data,tmp->data,list->len);

			esKRNL_SemPost(list->sem);
			
			return i;
		}
	}

	esKRNL_SemPost(list->sem);

	return -1;
}





int Dlist_Check_Element(dlist_t *list,void *key, dlist_cmp_t cmp)
{
	int i;
	
	struct node_t *tmp;

	esKRNL_SemPend(list->sem,0,NULL);

	for (i = 0, tmp = list->head; i < list->cnt; i++, tmp = tmp->next)
	{
		if (cmp(key, tmp->data) == 0)
		{
			esKRNL_SemPost(list->sem);

			return i;
		}
	}

	esKRNL_SemPost(list->sem);

	return -1;
}





int Dlist_Modify_Element(dlist_t *list,void *data, void *key, dlist_cmp_t cmp)
{
	int i;
	
	struct node_t *tmp;

	esKRNL_SemPend(list->sem,0,NULL);

	for (i = 0, tmp = list->head; i < list->cnt; i++, tmp = tmp->next)
	{
		if (cmp(key, tmp->data) == 0)
		{
			memcpy(tmp->data,data,list->len);

			esKRNL_SemPost(list->sem);

			return i;
		}
	}

	esKRNL_SemPost(list->sem);

	return -1;
}


dlist_t *Dlist_Findall(dlist_t *list, void *key, dlist_cmp_t cmp)
{
	dlist_t *idxs;
	
	int i;
	
	struct node_t *tmp;

	esKRNL_SemPend(list->sem,0,NULL);

	idxs = Dlist_Create(sizeof(int), NULL);
	if (NULL == idxs)
		goto err0;

	for (i = 0, tmp = list->head; i < list->cnt; i++, tmp = tmp->next)
	{
		if (cmp(key, tmp->data) == 0)	
		{
			if (0 != Dlist_Append(idxs, &i))
				goto err1;
		}
	}

	esKRNL_SemPost(list->sem);

	return idxs;

err1:
	Dlist_Destroy(idxs);
err0:

	if(NULL != list->sem)
	{
		esKRNL_SemPost(list->sem);
	}
	
	return NULL;
}



int Dlist_Delby_Idx(dlist_t *list, int idx)
{
	int i;
	
	struct node_t *tmp;

	esKRNL_SemPend(list->sem,0,NULL);

	if (idx < 0)
		idx = 0;
		
	if (idx > list->cnt - 1)
	{
		esKRNL_SemPost(list->sem);

		return -1;
	}
	
	if (NULL == list->head)
	{
		esKRNL_SemPost(list->sem);

		return -1;
	}
	
	i = 0, tmp = list->head;
	
	while (i < idx)
		i++, tmp = tmp->next;

	if (0 == idx)
		list->head = tmp->next;
	
	tmp->next->prev = tmp->prev;
	tmp->prev->next = tmp->next;

	if (NULL != list->destroy)
		list->destroy(tmp->data);

	In_Free(tmp,sizeof(struct node_t));

	list->cnt--;

	if(0 == list->cnt)
	{
		list->head = NULL;
	}

	esKRNL_SemPost(list->sem);
	
	return 0;
}




int Dlist_Delby_Key(dlist_t *list, void *key, dlist_cmp_t cmp)
{
	int i;

	int ret;
	
	struct node_t *tmp, *save;

	esKRNL_SemPend(list->sem,0,NULL);

	for (i = 0, tmp = list->head; i < list->cnt; i++, tmp = save)
	{
		save = tmp->next;	

		if (cmp(key, tmp->data) == 0)
		{
			ret = Dlist_Delby_Idx(list, i);
			if(-1 == ret)
			{
				esKRNL_SemPost(list->sem);

				return -1;
			}
			
			i--;
		}
	}

	esKRNL_SemPost(list->sem);

	return 0;
}

void Dlist_Traverse(dlist_t *list, dlist_op_t op)
{
	struct node_t *tail;

	esKRNL_SemPend(list->sem,0,NULL);

	if (NULL == list->head)
	{
		esKRNL_SemPost(list->sem);

		return;
	}
	
	tail = list->head;
	do {
		op(tail->data);	
		tail = tail->next;
	} while (tail != list->head);


	esKRNL_SemPost(list->sem);
}



void Dlist_Destroy(dlist_t *list)
{
	int i;

	__u8 err;
	
	struct node_t *tail, *save;

	for (i = 0, tail = list->head; i < list->cnt; i++, tail = save)
	{
		save = tail->next;

		if (NULL != list->destroy)
			list->destroy(tail->data);

		In_Free(tail,sizeof(struct node_t));
	}

	list->head = NULL;

	esKRNL_SemDel(list->sem, OS_DEL_ALWAYS, &err);	

	list->sem = NULL;
	
	In_Free(list,sizeof(dlist_t));
}




