#include <darr.h>



darr_t *Darr_Create(int len, darr_op_t destroy)
{
	darr_t *d;

	d = In_Malloc(sizeof(darr_t));
	if (NULL == d)
	{
		return NULL;
	}
	
	d->len = len;
	
	d->cnt = 0;
	
	d->p = NULL;
	
	d->destroy = destroy;
 
	return d;
}

int Darr_Count(darr_t *d)
{
	return d->cnt;
}

int Darr_Append(darr_t *d, void *data)
{
	void *newm;

	newm = In_Malloc(d->len * (d->cnt + 1));
	if (NULL == newm)
	{
		return -1;
	}
	
	eLIBs_memcpy((char *)newm, (char *)(d->p), d->len * d->cnt);
	
	eLIBs_memcpy((char *)newm + (d->len * d->cnt), (char *)data, d->len);

	In_Free(d->p,d->len * d->cnt);
	d->p = newm;

	d->cnt++;

	return 0;
}

int Darr_Prepend(darr_t *d, void *data)
{
	void *newm;

	newm = In_Malloc(d->len * (d->cnt + 1));
	if (NULL == newm)
	{
		return -1;
	}
	
	eLIBs_memcpy((char *)newm, (char *)data, d->len);
	
	eLIBs_memcpy((char *)newm + d->len, (char *)(d->p), d->len * d->cnt);

	In_Free(d->p,d->len * d->cnt);
	d->p = newm;

	d->cnt++;

	return 0;
}

int Darr_Insert(darr_t *d, void *data, int idx)
{
	void *newm;

	if (idx < 0)
		idx = 0;
	if (idx >= d->cnt)
		idx = d->cnt;

	newm = In_Malloc(d->len * (d->cnt + 1));
	if (NULL == newm)
	{
		return -1;
	}
	
	eLIBs_memcpy((char *)newm, (char *)(d->p), d->len * idx);
	eLIBs_memcpy((char *)newm + d->len * idx, (char *)data, d->len);
	eLIBs_memcpy((char *)newm + d->len * (idx + 1), (char *)(d->p) + d->len * idx, d->len * (d->cnt - idx));

	In_Free(d->p,d->len * d->cnt);
	
	d->p = newm;

	d->cnt++;

	return 0;
}

int Darr_Retrieve(darr_t *d, void *data, int idx)
{
	if (idx < 0)
		idx = 0;
		
	if (idx > d->cnt - 1)
	{
		return -1;
	}
	
	eLIBs_memcpy((char *)data, (char *)(d->p) + idx * d->len, d->len);

	return 0;
}


int Darr_Find(darr_t *d, void *key, darr_cmp_t cmp)
{
	int i;

	for (i = 0; i < d->cnt; i++)
	{
		if (cmp(key, (char *)(d->p) + i * d->len) == 0)
			return i;
	}

	return -1;
}

darr_t *Darr_Findall(darr_t *d, void *key, darr_cmp_t cmp)
{
	darr_t *idxs;
	int i;

	idxs = Darr_Create(sizeof(int), NULL);

	for (i = 0; i < d->cnt; i++)
	{
		if (cmp(key, (char *)(d->p) + i * d->len) == 0)	
			Darr_Append(idxs, &i);
	}

	return idxs;
}

int Darr_Delby_Idx(darr_t *d, int idx)
{
	void *newm;

	if (idx < 0)
		idx = 0;
	if (idx > d->cnt - 1)
	{
		return -1;
	}
	
	newm = In_Malloc(d->len * (d->cnt - 1));
	if (NULL == newm)
	{
		return -1;
	}
	
	if (d->destroy != NULL)
	{
		d->destroy((char *)(d->p) + idx * d->len);
	}
	
	eLIBs_memcpy((char *)newm, (char *)(d->p), d->len * idx);
	
	eLIBs_memcpy((char *)newm + d->len * idx,(char *)(d->p) + d->len * (idx + 1), d->len * (d->cnt - 1 - idx));

	In_Free(d->p,d->len * d->cnt);
	d->p = newm;

	d->cnt--;

	return 0;
}

int Darr_Delby_Key(darr_t *d, void *key, darr_cmp_t cmp)
{
	int i;

	for (i = 0; i < d->cnt; i++)
	{
		if (cmp(key, (char *)(d->p) + i * d->len) == 0)	
		{
			Darr_Delby_Idx(d, i);	
			i--;
		}
	}

	return 0;
}

void Darr_Traverse(darr_t *d, darr_op_t op)
{
	int i;

	for (i = 0; i < d->cnt; i++)
	{
		op((char *)(d->p) + i * d->len);
	}
}

void Darr_Destroy(darr_t *d)
{
	int i;

	if (NULL != d->destroy)
	{
		for (i = 0; i < d->cnt; i++)
		{
			d->destroy((char *)(d->p) + i * d->len);
		}
	}

	In_Free(d->p,d->len * d->cnt);
	
	In_Free(d,sizeof(darr_t));
}

int Darr_Save(darr_t *d, const char *path, darr_file_t fileop)
{
	ES_FILE *fp;
	
	__u8 ret;
	
	int i;

	if (NULL == (fp = eLIBs_fopen(path, "w")))
	{
		goto err0;
	}

	ret = eLIBs_fwrite(DARR_MAGIC, 1, sizeof(DARR_MAGIC), fp);
	
	ret = eLIBs_fwrite(&d->cnt, sizeof(int), 1, fp);

	if (NULL == fileop)
	{
		ret = eLIBs_fwrite(d->p, d->len, d->cnt, fp);	
	}
	else
	{
		for (i = 0; i < d->cnt; i++)	
		{
			ret = fileop((char *)(d->p) + i * d->len, fp);	
		}
	}

	eLIBs_fclose(fp);

	return 0;
err0:
	return -1;
}

darr_t *Darr_Load(darr_t *d, const char *path, darr_file_t fileop)
{
	darr_t *dl;
	
	ES_FILE *fp;
	
	__u8 ret;

	void *data;
	
	char buf[sizeof(DARR_MAGIC)];
	
	int i, cnt;

	dl = Darr_Create(d->len, d->destroy);
	Darr_Destroy(d);

	if (NULL == (fp = eLIBs_fopen(path, "r")))
	{
		goto err0;
	}

	ret = eLIBs_fread(buf, 1, sizeof(DARR_MAGIC), fp);
	if (ret != sizeof(DARR_MAGIC) || strcmp(DARR_MAGIC, buf) != 0)
		goto err1;
	ret = eLIBs_fread(&cnt, sizeof(int), 1, fp);

	data = In_Malloc(dl->len);
	
	for (i = 0; i < cnt; i++)
	{
		if (NULL == fileop)
		{
			ret = eLIBs_fread(data, d->len, 1, fp);	
		}
		else
		{
			ret = fileop(data, fp);	
		}

		Darr_Append(dl, data);
	}
	
	In_Free(data,dl->len);

	eLIBs_fclose(fp);

	return dl;

err1:
	eLIBs_fclose(fp);
err0:
	Darr_Destroy(dl);
	
	return NULL;
}
