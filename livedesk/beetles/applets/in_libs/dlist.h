#ifndef __DLIST_H__
#define __DLIST_H__

#include "beetles_app.h"

#include "mem.h"


#define DLIST_MAGIC		"DLIST"

typedef void (*dlist_op_t)(void *data);

typedef void (*dlist_add_t)(void *data,__u32 ID);

typedef int (*dlist_cmp_t)(void *key, void *data);

typedef int (*dlist_file_t)(void *data, ES_FILE *fp);



struct node_t {

	void *data;
	
	struct node_t *prev;
	
	struct node_t *next;
	
};

typedef struct _dlist {

	struct node_t *head;
	
	int len;
	
	int cnt;
	
	dlist_op_t destroy;
	
	__krnl_event_t* sem;
	
} dlist_t;



dlist_t *Dlist_Create(int len, dlist_op_t destroy);

int Dlist_Count(dlist_t *list);

int Dlist_Append(dlist_t *list, void *data);

int Dlist_Prepend(dlist_t *list, void *data);

int Dlist_Insert(dlist_t *list, void *data, int idx);

int Dlist_Retrieve(dlist_t *list, void *data, int idx);

void * Dlist_Retrieve_Pointer(dlist_t *list,int idx);

int Dlist_Trieve_Next(dlist_t *list, void *data,int idx);

int Dlist_Trieve_Prev(dlist_t *list, void *data,int idx);

int Dlist_Modify(dlist_t *list, void *data, int idx);

int Dlist_Find(dlist_t *list, void *key, dlist_cmp_t cmp);

void * Dlist_Find_Pointer(dlist_t *list, void *key, dlist_cmp_t cmp);

int Dlist_Find_Element(dlist_t *list,void *data, void *key, dlist_cmp_t cmp);

int Dlist_Check_Element(dlist_t *list,void *key, dlist_cmp_t cmp);


int Dlist_Modify_Element(dlist_t *list,void *data, void *key, dlist_cmp_t cmp);

dlist_t *Dlist_Findall(dlist_t *list, void *key, dlist_cmp_t cmp);

int Dlist_Delby_Idx(dlist_t *list, int idx);

int Dlist_Delby_Key(dlist_t *list, void *key, dlist_cmp_t cmp);

void Dlist_Traverse(dlist_t *list, dlist_op_t op);

void Dlist_Destroy(dlist_t *list);


#endif /* !__DLIST_H__ */
