#ifndef __DARR_H__
#define __DARR_H__



#include "beetles_app.h"

#include "mem.h"


#define DARR_MAGIC	"darr"


typedef void (*darr_op_t)(void *data);

typedef int (*darr_cmp_t)(void *key, void *data);

typedef int (*darr_file_t)(void *data, ES_FILE *fp);


typedef struct _darr {

	void *p;
	int len;
	int cnt;
	darr_op_t destroy;

	
} darr_t;


darr_t *Darr_Create(int len, darr_op_t destroy);


int Darr_Count(darr_t *d);


int Darr_Append(darr_t *d, void *data);

int Darr_Prepend(darr_t *d, void *data);

int Darr_Insert(darr_t *d, void *data, int idx);

int Darr_Retrieve(darr_t *d, void *data, int idx);

int Darr_Find(darr_t *d, void *key, darr_cmp_t cmp);

darr_t *Darr_Findall(darr_t *d, void *key, darr_cmp_t cmp);

int Darr_Delby_Idx(darr_t *d, int idx);

int Darr_Delby_Key(darr_t *d, void *key, darr_cmp_t cmp);

void Darr_Traverse(darr_t *d, darr_op_t op);

void Darr_Destroy(darr_t *d);

int Darr_Save(darr_t *d, const char *path, darr_file_t fileop);

darr_t *Darr_Load(darr_t *d, const char *path, darr_file_t fileop);

#endif /* !__DARR_H__ */
