/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_cedar sub-system
*                          (module name, e.g.mpeg4 decoder plug-in) module
*
*          (c) Copyright 2010-2012, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : darray_v2.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-5-7
* Description:
    �ڶ���͵�һ��Ĳ�ͬ���ڣ��ڶ���DArray�Լ�ֱ�Ӱ�struct�Ĵ�С�����ڴ棬
    ��һ����void*����, ָ���struct��ȻҪ�û��Լ����䣬ʵ���϶���struct���ԣ������������ģ�
    ֻ��void*�������ѡ�
    
    ���������С���õڶ����ֱ��.���ڶ���darrayV2������struct���Ǻܴ��������Ǻܶ࣬���û�ϣ���������������������������

    ����������󣬲��롢ɾ��������, ���Ҷ࣬�õ�һ���һЩ��struct��malloc�����ģ����ǲ��ң����򷽱㡣
һ��void*ָ���struct��malloc�����ġ�

    �����������, ���롢ɾ�������࣬��ôӦ����˫������void*һ��ָ���struct��malloc�����ġ�

    ʹ��ע������:
    thiz->data_size��һ��struct���ڴ���ռ�ݵ��ֽ�����һ����ڵ�����������ֽ�����
    ��Ϊ���ڴ�����ԭ��һ��Ҫ��sizeof()���õ�ֵ

    ������Ҫ������ʵ�ֵ�4���ص�������˵��: DArray�����汾������
    DataExitFuncV2:����û���struct����ָ�룬��ָ��ĵ�ַ��malloc�����ģ���ô�û�һ��Ҫʵ������ص��������ڷ���ʼ��ʱ��DArrayV2����á�
����û�����Ҫfree����ô������ΪNULL������DArray�Ͳ���ʲô���������
    DataCopyFuncV2: ����û�ϣ���������ô���Լ�ʵ������ص�����. DArray���ڸ�ֵʱ���á�����û�
û�����������������ΪNULL��DArrayĬ��ǳ������
    ����������������DArray��ʼ��ʱ����ģ���DArray�����ڼ䲻�ܸı�ġ�

    DataCompareFuncV2:
    DataVisitFuncV2:
    �������ص���������findelem��foreachʱ��Ϊ���������,ÿ�ο��Բ�ͬ����ʵ�ֲ�ͬ��
����Ҫ������ʱ�ı�struct������
********************************************************************************
*/
#ifndef _DARRAY_V2_H_
#define _DARRAY_V2_H_

DECLS_BEGIN;


//typedef Ret (*SortFunc)(void** array, size_t nr, DataCompareFunc cmp);    //��̬�����ǿ��Կ��������

//Ԫ��data struct����Լ��ĳ�Ա��malloc�������ڴ�, ��ô��struct�ڷ���ǰ��ȡ��ǰ����exitһ�£�
//���ڴ��������Դ�ͷŵ�. ctx��exit�õ������ģ����ʼ��ʱ�ʹ�����ָ�룬�����ڵ���exit()ǰ�޸���ָ������ݽṹ��exit()ִ��
//���������ڲο�
typedef int      (*DataExitFuncV2)(void* data_exit_ctx, void* data); //RET_OK. ����ʼ��ʱ, struct�ڵ�ָ�룬Ҫ��Ҫfree��ʹ�����Լ�����, ctx�����ʼ��ʱ�ʹ���
typedef int      (*DataCopyFuncV2)(void* pDesdata, void* pSrcdata); //RET_OK. �����ǳ������ʹ�����Լ�����
//typedef int      (*DataCompareFuncV2)(void* ctx, void* data); //ret = 0��ʾƥ��. data����Ԫ��, ctx��ƥ��ֵ���û���ʱ����������ģ�һ���DArrayV2��struct������ͬ, ����DArray_FindElem()
//typedef int      (*DataVisitFuncV2)(void* ctx, void* data);   //Ret, data������һ��struct��ָ��, ctx���û���ʱ�����������,����DArray_ForeachElem()

typedef struct tag_DArrayV2 DArrayV2;
typedef int (*DArrayV2_InsertElem)(DArrayV2* thiz, unsigned int index, void* data);  //����ֵRet_OK
typedef int (*DArrayV2_PrependElem)(DArrayV2* thiz, void* data);    //����ֵRet_OK
typedef int (*DArrayV2_AppendElem)(DArrayV2* thiz, void* data);     //����ֵRet_OK, data��ָ��struct��ָ��
typedef int (*DArrayV2_DeleteElem)(DArrayV2* thiz, unsigned int index);   //����ֵRet_OK
typedef int (*DArrayV2_GetElemByIndex)(DArrayV2* thiz, unsigned int index, void** data);  //����ֵRet_OK, data��ָ��struct��ָ�룬ֻ�õ�ָ��ֵ, void**data = ����struct��ָ��
typedef int (*DArrayV2_SetElemByIndex)(DArrayV2* thiz, unsigned int index, void* data);   //����ֵRet_OK, Ҫȡ��ԭ�е�Ԫ��,ԭ����Ԫ�ػ����exit()����ʼ����Ȼ�����copy()����
typedef unsigned int (*DArrayV2_GetLength)(DArrayV2* thiz);  //����ֵΪԪ�ظ���
typedef int (*DArrayV2_ForeachElem)(DArrayV2* thiz, DataVisitFunc visit, void* ctx);    //����ֵRet_OK
typedef unsigned int (*DArrayV2_FindElem)(DArrayV2* thiz, DataCompareFunc cmp, void* ctx);   //����ֵΪ�±��, size��ʾû�ҵ�
typedef struct tag_DArrayV2
{
	void* data;                     // �û���struct�������ʼ��ַ,����������Ҫ�����Ķ�������,��Ϊ��֪�����ͣ����Ա���֪��һ������Ĵ�С�����ܷ����ڴ�
    unsigned int data_size;         // һ��structԪ����ռ���ֽ���
	unsigned int size;              // ��ǰ��ռ��Ԫ�ظ���
	unsigned int alloc_size;        // ��ǰ�Ѿ�malloc������Ԫ���ܸ���
    unsigned int nMinPreAllocNr;    // ���Ԥ�����Ԫ�ظ���, #define MIN_PRE_ALLOCATE_NR 10

	void* data_exit_ctx; //����data_exit()ʱ����Ϊ����ʼ�������ģ���Ϊ����ʱ�Ĳο�������֪��������. �����ʼ��ʱ�ʹ�����. ÿ����ʼ��һ��Ԫ��ǰ�����Ը���ctx�����ݣ���exit()ʱ���ο�. ����������
	DataExitFuncV2 data_exit;   //ɾ��Ԫ��ʱ�����Ԫ����ָ�룬ָ��һ��malloc�����ݽṹ������Ҫ���øú���free. �����ΪNULL���Ͳ�������
	DataCopyFuncV2 data_copy;   //InsertElemʱ��һ����������������������ΪNULL����Ĭ��ǳ����

    //DArray�Ĳ��������⹫����API
    DArrayV2_InsertElem       InsertElem;
    DArrayV2_PrependElem      PrependElem;
    DArrayV2_AppendElem       AppendElem;   //�����β
    DArrayV2_DeleteElem       DeleteElem;
    DArrayV2_GetElemByIndex   GetElemByIndex;
    DArrayV2_SetElemByIndex   SetElemByIndex;
    DArrayV2_GetLength        GetLength;
    DArrayV2_ForeachElem      ForeachElem;
    DArrayV2_FindElem         FindElem;
}DArrayV2;
//��ʱ�򣬶�̬�����Լ�ֱ����Ϊ�������ڣ���ʱ��ʼ��������
extern int DArrayV2_Init(DArrayV2* thiz, DataExitFuncV2 data_exit, void* data_exit_ctx, DataCopyFuncV2 data_copy, unsigned int data_size, unsigned int nMinPreAllocNr);
extern int DArrayV2_Exit(DArrayV2* thiz);
//��ʱ�򣬶�̬����ֻ����Ϊһ��ָ����ڣ���ʱ��Ҫnewһ������
extern DArrayV2* newDArrayV2(DataExitFuncV2 data_exit, void* data_exit_ctx, DataCopyFuncV2 data_copy, unsigned int data_size, unsigned int nMinPreAllocNr); //����Ĳ�����Ϊ�˷���ʼ��void*ָ���struct����,��Щstruct������DArrayV2�Լ�malloc�������߼��������ڴ�
extern void deleteDArrayV2(DArrayV2* thiz);  //ɾ��DArray������Ԫ�أ����Ԫ��ָ������ݽṹ��Ҫfree,Ҳ�����data_destroy() ����free

DECLS_END

#endif  /* _DARRAY_V2_H_ */

