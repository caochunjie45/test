
#include "isdb_interface.h"
#include "isdb_context.h"
#include "isdb_eit_pool.h"
#include "isdb_eit_parser.h"

#define SUPPORT_ISDB_EPG	(1)

isdb_ctx_t* g_isdbCtx = NULL;

static __u32 isdb_generate_channel_id()
{
    if(g_isdbCtx)
        return g_isdbCtx->localServList.servNum;
    else
        return 0;
}


__s32 isdb_get_mutex(__krnl_event_t* mutex)
{
    __u8 err;
    
    if(mutex == NULL)
        return -1;
        
    esKRNL_SemPend(mutex, 100, &err);
    
    if(err == OS_NO_ERR)
    {
        return 0;
    }
    else
    {
        WARNING("++++++++++++++ get mutex fail.\n");
        return -1;
    }
}


void  isdb_release_mutex(__krnl_event_t* mutex)
{
    OS_SEM_DATA tmpSemDat;
    
    if(mutex == NULL)
        return;
    
    esKRNL_SemQuery(mutex, &tmpSemDat);
    if(tmpSemDat.OSCnt == 0)
    {
        esKRNL_SemPost(mutex);
    }
}


__s32 isdb_set_default_char_code_type(__epdk_charset_enm_e codeType)
{
    g_isdbCtx->charCodeType = codeType;
    return 0;
}

/*******************************************************************************
Function name: isdb_convert_str_to_utf8
Description: 
    eucStringBuffer�����g_isdbCtx��malloc���.
    ת����ɺ���2���ֽڵ�\0.
Parameters: 
    dstLen:dst buffer���ܳ���,��ת��ʱ��Ҫ����\0������.����dstLen���>2.
Return: 
    ת�����UTF8�ַ����ĳ���.������\0.
Time: 2011/2/16
*******************************************************************************/
//* function for converting string to utf8
__s32 isdb_convert_str_to_utf8(__u8* src, __u32 srcLen, __u8* dst, __u32 dstLen, __u8* lang_code)
{
    //JIS->UTF8
    __u32   utf8StringLen;
    __u32   eucStringLen;
    __u32   ntmpeucStringBufSize = 3*srcLen + 100;

    if(NULL == src || 0 == srcLen || NULL == dst || dstLen <= 2)
    {
        return 0;
    }
    if(g_isdbCtx->charCodeType != EPDK_CHARSET_ENM_JIS)
    {
        //skip first non-spacing, non-displayed char.
        if(src[0] < 0x20)
        {
            WARNING("isdb skip first char[0x%x]\n", src[0]);
            src++;
            srcLen--;
        }
        return eLIBs_charset_convert(g_isdbCtx->charCodeType, EPDK_CHARSET_ENM_UTF8, src, srcLen, dst, dstLen);
    }
#if 0
    //ϵͳ����֧��JIS����,�������м���Լ�����.JIS -> EUC-JP -> unicode -> UTF8
    if(g_isdbCtx->eucStringBuf)
    {
        if(g_isdbCtx->eucStringBufSize < ntmpeucStringBufSize)
        {
            free(g_isdbCtx->eucStringBuf);
            g_isdbCtx->eucStringBuf = NULL;
            g_isdbCtx->eucStringBufSize = 0;
        }
    }
    if(NULL == g_isdbCtx->eucStringBuf)
    {
        g_isdbCtx->eucStringBuf = malloc(ntmpeucStringBufSize);
        if(NULL == g_isdbCtx->eucStringBuf)
        {
            WARNING("malloc eucbuffer fail!\n");
            return 0;
        }
        g_isdbCtx->eucStringBufSize = ntmpeucStringBufSize;
    }
    
    eucStringLen = jis2eucjp(src, srcLen, g_isdbCtx->eucStringBuf, g_isdbCtx->eucStringBufSize);
    //memset(dst, 0, dstLen);
    utf8StringLen = eLIBs_charset_convert_eucjp2utf8(EPDK_CHARSET_ENM_EUC_JP, EPDK_CHARSET_ENM_UTF8, g_isdbCtx->eucStringBuf, eucStringLen, dst, dstLen - 2);
    dst[utf8StringLen] = 0;
    dst[utf8StringLen+1] = 0;
    //dst[dstLen - 2] = 0;
    //dst[dstLen - 1] = 0;
#else
    //ֱ�ӵ���aribjis->utf8���ַ�ת����
    utf8StringLen = conv_arib_to_utf8((char*)src, (int)srcLen, (char*)dst, (int)dstLen - 2, GraphicSetsDesignationMode_SI);
    dst[utf8StringLen] = 0;
    dst[utf8StringLen+1] = 0;
#endif
    
    return utf8StringLen;
}

typedef int (*mapleServItemCompareFunc)(void* ctx, void* data); //ret = 0��ʾƥ��. data����void*Ԫ��, ctx��ƥ��ֵ���û���ʱ����������ģ�һ���void*ָ���struct��������ͬ��.����DArray_FindElem(), ctx >= data, ����ֵΪ>=0.
int DataCompareFunc_mapleServItem(void* ctx, void* data)
{
    maple_serv_item_t   *pItemCtx = (maple_serv_item_t*)ctx;
    maple_serv_item_t   *pItemData = (maple_serv_item_t*)data;
    if(pItemCtx->nRemoteControlKeyId > pItemData->nRemoteControlKeyId)
    {
        return 1;
    }
    else if(pItemCtx->nRemoteControlKeyId < pItemData->nRemoteControlKeyId)
    {
        return -1;
    }

    if(pItemCtx->demod_pm.frequency > pItemData->demod_pm.frequency)
    {
        return 1;
    }
    else if(pItemCtx->demod_pm.frequency < pItemData->demod_pm.frequency)
    {
        return -1;
    }

    if(pItemCtx->servId > pItemData->servId)
    {
        return 1;
    }
    else if(pItemCtx->servId < pItemData->servId)
    {
        return -1;
    }
    else
    {
        WARNING("fatal error! impossbile? [%d][%d][0x%x]\n", pItemCtx->nRemoteControlKeyId, pItemCtx->demod_pm.frequency, pItemCtx->servId);
        return 0;
    }
}

static __u32 IntLog2(__u32 n)   //ģ�������2Ϊ�׵Ķ���,2^N >= n�Ϳ���
{
	__u32 i = 0;
	__u32 tmp;
	for(i=0;;i++)
	{
		tmp = 1<<i;
		if(tmp >= n)
		{
			break;
		}
	}
	return i;
}

/*******************************************************************************
Function name: QuickSort_Split
Description: 
    1.һ�ηָ��һ��Ԫ��xΪ׼����Ϊ��ߵ�Ԫ�ض� < x, �ұߵ�Ԫ�ض�>=x,
    2.һ�������, Ԫ��x��ѡleft
Parameters: 
    
Return: 
    Ԫ��x�ڷָ���λ��
Time: 2011/5/11
*******************************************************************************/
unsigned int QuickSort_Split_mapleServItem(maple_serv_item_t* array, unsigned int left, unsigned int right, mapleServItemCompareFunc cmp)
{
    maple_serv_item_t x;
    memcpy(&x, &array[left], sizeof(maple_serv_item_t));
    // ���left == right, ˵���������������ֻʣһ��Ԫ��,ֱ�ӷ�����
    while(left < right)
	{
        //��һ��whileѭ��, x��left��. left��right֮��(����right�����Բ�����left)��Ҫ��,��Ϊx��left��, left����, ���������
		while(left < right && cmp(&array[right], &x) >= 0) right--;
        //���left == right˵��ȫ��>=x��x����ԭλ�Ϳ����ˣ�����ִ�н��Ƴ�whileѭ��
        //���left != right (һ��left < right), ˵���ҵ���һ����x��С��Ԫ��,��ôx��λ�þ��ø��Ǹ���С��Ԫ��,
        //����x�Լ���������Ϊռ����right��λ��, rightλ�õ�Ԫ���ƶ���left��λ��, left��+1.
		if(left != right)
		{
			//array[left] = array[right];
            memcpy(&array[left], &array[right], sizeof(maple_serv_item_t));
			left++; //leftԭ����λ���Ѿ�ռ���ˣ����Դ���һ��λ�ÿ�ʼѰ���ʺ�left��λ��
		}
        //��һ��whileѭ��������(left�Ѿ�++��)�����Ա�֤:right�ұߵ�Ԫ�ض�>= x, left��ߵ�Ԫ�ض�<x (������left, right�Լ�, right�Լ����ھ���x)��
        // left, right֮���Ԫ��(����left, right�Լ�)���д�����.

        //�ڶ���whileѭ����ʼ,��ʱx��right��λ��,���Դ�left��ʼ���ұ���.���������right��������Ϊright����xӦ���ڵ�λ��. right���, left�ұ�(����left)��С��x
        //���<=0�����⣬���ǵ�ϣ����С��left��Ԫ������ߣ����ڵ���left��Ԫ�����ұߣ����Ը�Ϊ<. ԭʼ����д��<=,���Ľ��Ҳ�����,����xԪ�ص�λ�þͲ�����ȷ����,
        //���ǵ�ϣ����:����кܶ�ֵ��x��ͬ��Ԫ�أ���ôx�������.
        //���Ǹĳ�<=,�ô��ǿ������ƶ�Ԫ��.������������еĿ���.���Ҿ���ĳ�<,Ҳ���ܱ�֤��ȵ�Ԫ�ذ�λ�����С�
		//while(cmp(array[left], x) <= 0 && left < right)	left++;
		while(left < right && cmp(&array[left], &x) <= 0)	left++;
		if(left != right)   //˵���ҵ��� >= x��Ԫ��, ���left == right����ôx��Ӧ����right��λ����
		{
			//array[right] = array[left];
			memcpy(&array[right], &array[left], sizeof(maple_serv_item_t));
			right--;
		}
        //�ڶ���while����ʱ,������Ϊx����left�ĵط�,right���Ǹ�>=x��Ԫ��ռ��.
        //Ȼ��right--, ����ֻ���µ�left��right֮���Ԫ��û�о���������(����right, ��left������Ϊ����x��ʱռ�ݵĵط���)
	}
    //��Ϊ�˳�while()ѭ��ʱ, left == right.����x��λ�þʹ�ȷ���� left���Բ����ܴ���right.
	//array[left] = x;
	memcpy(&array[left], &x, sizeof(maple_serv_item_t));
    
    return left;
}

/*******************************************************************************
Function name: QuickSort_NonRecursive
Description: 
    2. ����stack���ȵ�����:
        ����Ҳ���Խ���һ��ģ�������2Ϊ�׵Ķ����ĺ���IntLog2().
       �ڱ�������, һ��stack_item��2��__u32��������¼uLow��uHigh.
       ����ʽ������:ÿ�ηֳ�2����������д��������ջ,С������ջ�����̳�ջ, ��������,
    3.Ĭ�ϰ���������.
    4. ��ʹ��ջ�ķǵݹ��㷨��, ģ��ջ
����ԭ��:
    1.���Ϸ���, С��ĳԪ�ص�ֵ�ڵ�һ��,���ڵ��ڵ�ֵ�ڵڶ���. ֱ�����ڵ���ֻʣ��һ��Ԫ��Ϊֹ.
    Ԫ�ص�ѡȡ������ģ�һ��ѡ����ߵ�.
    ���С��Ԫ�ص�ֵȫ���Ƶ����, ���ڵ��ڵ����ұ�
    2.���Ա�֤����left <= right.
    3.����������Ч����:
        ����: ��ֵͬ��Ԫ���У����ܱ�֤��ʼλ��������ߵ����������,.
        ����: ��ֵͬ��Ԫ���У����ܱ�֤��ʼλ��������ߵ����������,
Parameters: 
    1. pTable:�����һ��Ԫ�صĵ�ַ, ����Ϊmaple_serv_item_t
    2. nSize: Ԫ�ظ���
    3. CompareFunc:�ȽϺ���,
        >=0: ��ʾDataCompareFunc��ctx >= data, ctx����ߣ��ǵ�һ������
        <0: ��ʾctx < data
    4. DataCopy:��ֵ����
    
2. ��ʹ��ջ�ķǵݹ��㷨
    
Return: 
    EPDK_OK;
Time: 2011/5/11
*******************************************************************************/
static int QuickSort_NonRecursive_mapleServItem(maple_serv_item_t *pTable, unsigned int nSize, mapleServItemCompareFunc CompareFunc)
{
    int ret = EPDK_OK;    //Ret
    unsigned int *puStack = NULL;
    unsigned int uStackTop;
    unsigned int uLow;
    unsigned int uHigh;
    unsigned int uMid;
    unsigned int    max_stacktop = 0;   //��¼ʵ���������ɵ����Ԫ����.
    unsigned int    uStackItemSize; //��¼stack�����ɵ�Item������ֹ����Խ��. ������,һ��Item��2��unsigned int ���
    if(nSize < 1)
    {
        return EPDK_OK;
    }
    uLow = 0;
    uHigh = nSize - 1;
    //puStack = (unsigned int *)malloc((uHigh - uLow + 1) *sizeof(unsigned int));
    uStackItemSize = IntLog2(uHigh - uLow + 1);
    puStack = (unsigned int *)malloc(uStackItemSize * 2 * sizeof(unsigned int));
    if(puStack == NULL)
    {
        WRN("malloc stack [%d]bytes fail\n", uStackItemSize * 2 * sizeof(unsigned int));
        goto _err0;
    }
    uStackTop = 0;
    puStack[uStackTop] = uLow;
    ++uStackTop;
    puStack[uStackTop] = uHigh;
    ++uStackTop;
    while(uStackTop != 0)
    {
        if(max_stacktop < uStackTop)    //ͳ������ʱʵ���õ���ջԪ�ظ���
        {
            max_stacktop = uStackTop;
        }
        if(uStackTop > uStackItemSize * 2)
        {
            WRN("fatal error! overflow memory! check code immediately!!! uStackTop[%d], uStackSize[%d]\n", uStackTop, uStackItemSize * 2);
            ret = EPDK_FAIL;
            break;
        }
        --uStackTop;
        uHigh = puStack[uStackTop];
        --uStackTop;
        uLow = puStack[uStackTop];
        if(uLow < uHigh)
        {
            uMid = QuickSort_Split_mapleServItem(pTable, uLow, uHigh, CompareFunc);
            
            if(uMid - uLow > uHigh - uMid)
            { 
                puStack[uStackTop] = uLow;
                ++uStackTop;
                puStack[uStackTop] = uMid-1;
                ++uStackTop;
                if(uHigh > uMid)
                {
                    puStack[uStackTop] = uMid + 1;
                    ++uStackTop;
                    puStack[uStackTop] = uHigh;
                    ++uStackTop;
                }
            }
            else
            {
                puStack[uStackTop] = uMid + 1;
                ++uStackTop;
                puStack[uStackTop] = uHigh;
                ++uStackTop;
                if(uMid > uLow)
                {
                    puStack[uStackTop] = uLow;
                    ++uStackTop;
                    puStack[uStackTop] = uMid - 1;
                    ++uStackTop;
                }
            }
        }
    }
    //WRN("max_stacktop[%d], uStackSize[%d]\n", max_stacktop, uStackItemSize * 2);
    if(puStack)
    {
        free(puStack);
        puStack = NULL;
    }
    return ret;

_err0:
    if(puStack)
    {
        free(puStack);
        puStack = NULL;
    }
    return EPDK_FAIL;
}

isdb_ctx_t* newIsdbCtx()
{
    __s32   i;
    isdb_ctx_t* g_isdbCtx;
    g_isdbCtx = (isdb_ctx_t*)malloc(sizeof(isdb_ctx_t));
    if(g_isdbCtx == NULL)
    {
        WARNING("malloc fail\n");
        return NULL;
    }
    memset(g_isdbCtx, 0, sizeof(isdb_ctx_t));

 
    g_isdbCtx->isdbMallocBuf = (__u8 *) malloc(sizeof(__u8)*SECTION_BUFIDX_CNT*ISDB_SECTION_BUF_SIZE);
    if(g_isdbCtx->isdbMallocBuf == NULL)
    {
       MESSAGE("malloc memory for si section buffer fail.\n");
       //return -1;
       goto _err0;
     }

	
    g_isdbCtx->localServList.pServItems = (maple_serv_item_t *) malloc(sizeof(maple_serv_item_t)*MAPLE_MAX_SERV_NUM);
    if (g_isdbCtx->localServList.pServItems == NULL)
    {
        MESSAGE("malloc memory for service list fail.\n");
//        free(g_isdbCtx->isdbMallocBuf);
//        g_isdbCtx->isdbMallocBuf = NULL;
//        free(g_isdbCtx);
//        g_isdbCtx = NULL;
//        return -1;
        goto _err0;
    }
    memset(g_isdbCtx->localServList.pServItems, 0, sizeof(maple_serv_item_t)*MAPLE_MAX_SERV_NUM);
    g_isdbCtx->localServList.servNum = 0;
    g_isdbCtx->localServList.nServItemsCount = MAPLE_MAX_SERV_NUM;
    g_isdbCtx->charCodeType          = EPDK_CHARSET_ENM_ISO_8859_15;    //* default character coding method is ISO8859_15_FDIS

    for(i=0; i<SECTION_BUFIDX_CNT; i++)
    {
       g_isdbCtx->isdbSectionBuf[i]     = (__u8 *)(g_isdbCtx->isdbMallocBuf+i*ISDB_SECTION_BUF_SIZE);
    }
    return g_isdbCtx;

_err0:
    if(g_isdbCtx->isdbMallocBuf)
    {
        free(g_isdbCtx->isdbMallocBuf);
        g_isdbCtx->isdbMallocBuf = NULL;
    }
    if(g_isdbCtx->localServList.pServItems)
    {
        free(g_isdbCtx->localServList.pServItems);
        g_isdbCtx->localServList.pServItems = NULL;
    }
    free(g_isdbCtx);
    return NULL;
}

void deleteIsdbCtx(isdb_ctx_t *thiz)
{
    isdb_ctx_t  *g_isdbCtx = thiz;
    if(g_isdbCtx->isdbMallocBuf)
    {
        free(g_isdbCtx->isdbMallocBuf);
        g_isdbCtx->isdbMallocBuf = NULL;
    }
    
    if(g_isdbCtx->localServList.pServItems)
    {   
    	free(g_isdbCtx->localServList.pServItems);
        g_isdbCtx->localServList.pServItems = NULL;
        g_isdbCtx->localServList.servNum = 0;  
    }

    if(g_isdbCtx->eucStringBuf)
    {
        free(g_isdbCtx->eucStringBuf);
        g_isdbCtx->eucStringBuf = NULL;
        g_isdbCtx->eucStringBufSize = 0;
    }
    free(g_isdbCtx);
}



//*****************************************************************************//
//****************** Functions SI library provide to outside ******************//
//*****************************************************************************//

//* open isdb si library.
__s32 isdb_open(void)
{   
    //__u8 i=0;
    
    if(g_isdbCtx)
        return -1;

    g_isdbCtx = newIsdbCtx();
    
    //g_isdbCtx = (isdb_ctx_t*)malloc(sizeof(isdb_ctx_t));
    if(g_isdbCtx == NULL)
        return -1;
//    memset(g_isdbCtx, 0, sizeof(isdb_ctx_t));
//
// 
//    g_isdbCtx->isdbMallocBuf = (__u8 *) malloc(sizeof(__u8)*SECTION_BUFIDX_CNT*ISDB_SECTION_BUF_SIZE);
//    if(g_isdbCtx->isdbMallocBuf == NULL)
//    {
//       MESSAGE("malloc memory for si section buffer fail.\n");
//       return -1;
//     }
//
//	
//    g_isdbCtx->localServList.pServItems = (maple_serv_item_t *) malloc(sizeof(maple_serv_item_t)*MAPLE_MAX_SERV_NUM);
//    if (g_isdbCtx->localServList.pServItems == NULL)
//    {
//        MESSAGE("malloc memory for service list fail.\n");
//        free(g_isdbCtx->isdbMallocBuf);
//        g_isdbCtx->isdbMallocBuf = NULL;
//        free(g_isdbCtx);
//        g_isdbCtx = NULL;
//        return -1;
//    }
//    memset(g_isdbCtx->localServList.pServItems, 0, sizeof(maple_serv_item_t)*MAPLE_MAX_SERV_NUM);
//    g_isdbCtx->localServList.servNum = 0;
//    g_isdbCtx->charCodeType          = EPDK_CHARSET_ENM_ISO_8859_15;    //* default character coding method is ISO8859_15_FDIS
//
//    for(i=0; i<SECTION_BUFIDX_CNT; i++)
//    {
//       g_isdbCtx->isdbSectionBuf[i]     = (__u8 *)(g_isdbCtx->isdbMallocBuf+i*ISDB_SECTION_BUF_SIZE);
//    }
    return 0;
}

//* close isdb si library. 
__s32 isdb_close(void)
{
    if(g_isdbCtx)
    {
//        isdb_stop_sepg_search();
//        isdb_stop_schedule_search();
        isdb_stop_service_search();
        isdb_close_time_monitor();
        isdb_close_epg_monitor();
        
//        if(g_isdbCtx->isdbMallocBuf)
//        {
//            free(g_isdbCtx->isdbMallocBuf);
//            g_isdbCtx->isdbMallocBuf = NULL;
//        }
//	    
//        if(g_isdbCtx->localServList.pServItems)
//        {   
//        	free(g_isdbCtx->localServList.pServItems);
//            g_isdbCtx->localServList.pServItems = NULL;
//            g_isdbCtx->localServList.servNum = 0;  
//        }
//
//        if(g_isdbCtx->eucStringBuf)
//        {
//            free(g_isdbCtx->eucStringBuf);
//            g_isdbCtx->eucStringBuf = NULL;
//            g_isdbCtx->eucStringBufSize = 0;
//        }
//        free(g_isdbCtx);
        deleteIsdbCtx(g_isdbCtx);
        g_isdbCtx = NULL;
    }
    
    return 0;
}

//* function for service list managing.
//* open service list from file.
__s32 isdb_open_list(__u8* listFilePath)
{
    ES_FILE*    fpListFile;
    __u32       readBytes;
    __u32       servNum;
    
    if(listFilePath)
    {
        fpListFile = fopen((char *)listFilePath, "rb");
        if (fpListFile == NULL)
            return -1;
		
        readBytes = fread(&servNum, 1, sizeof(__u32), fpListFile);
        if (readBytes != sizeof(__u32))
        {
            fclose(fpListFile);
            return -1;
        }
		
        if(servNum > MAPLE_MAX_SERV_NUM)
        {
            fclose(fpListFile);
            return -1;
        }
            
        if (servNum > 0)
        {
            readBytes = fread(g_isdbCtx->localServList.pServItems, 1, sizeof(maple_serv_item_t)*servNum, fpListFile);
            if (readBytes != (sizeof(maple_serv_item_t)*servNum))
            {
                fclose(fpListFile);
                return -1;
            }
			
            g_isdbCtx->localServList.servNum = servNum;
        }
		
        fclose(fpListFile);
    }
    
    return 0;
}

//* save list to file.
__s32 isdb_save_list(__u8* listFilePath)
{
    __u32 servNum;
    ES_FILE* fpList;
    
    servNum = g_isdbCtx->localServList.servNum;
    fpList = fopen((char*)listFilePath, "wb");
    if (!fpList)
    {
        MESSAGE("cannot open file %s for service list saving.\n", listFilePath);
        return -1;
    }
    
    fwrite(&servNum, 1, sizeof(__u32), fpList);
    fwrite(g_isdbCtx->localServList.pServItems, 1, sizeof(maple_serv_item_t)*servNum, fpList);
    fclose(fpList);
    return 0;
}

//* delete service list.
__s32 isdb_delete_list(void)
{   
    g_isdbCtx->localServList.servNum = 0;
    return 0;
}

/*******************************************************************************
Function name: isdb_sort_list
Description: 
    SORTMODE_REMOTE_CONTROL_KEY_IDģʽ�±Ƚ�service�Ĺ���:
    remote_control_key_id
    frequency
    service_num

    �����ȫ��ͬ,����.
Parameters: 
    nSortMode:0:��remote control key id����,��ͬ��id�ŵ�service�ٰ�service number����.Ĭ�ϴ�С��������;
Return: 
    
Time: 2012/3/1
*******************************************************************************/
__s32 isdb_sort_list(__s32 nSortMode)
{
    if(nSortMode == SORTMODE_REMOTE_CONTROL_KEY_ID)
    {
        if(EPDK_OK!=QuickSort_NonRecursive_mapleServItem(g_isdbCtx->localServList.pServItems, g_isdbCtx->localServList.servNum, DataCompareFunc_mapleServItem))
        {
            WARNING("why quicksort fail?\n");
        }
        return EPDK_OK;
    }
    else
    {
        WARNING("other sort mode[%d]?\n", nSortMode);
        return EPDK_OK;
    }
    
}

//* add service item
__s32 isdb_add_serv_item(maple_serv_item_t* item, __u32 check)
{
    __u32               i;
    __u32               orgTsId;
    __u32               orgServId;
    __u32               frequency;
    maple_serv_list_t*  list;
    
    list = &g_isdbCtx->localServList;
    
    if(list->servNum >= MAPLE_MAX_SERV_NUM)
        return -1;
    
    if(check == 1)
    {
        for(i=0; i<list->servNum; i++)
        {
            orgTsId     = list->pServItems[i].tsId;
            orgServId   = list->pServItems[i].servId;
            frequency   = list->pServItems[i].demod_pm.frequency;
            if((orgTsId == item->tsId) && (orgServId == item->servId) && (item->demod_pm.frequency == frequency))
                return 0;
        }
    }
    else
        i = list->servNum;
    
    memcpy(&list->pServItems[i], item, sizeof(maple_serv_item_t));
    list->pServItems[i].channelId   = isdb_generate_channel_id();
    list->pServItems[i].isFavourate = 0;
    list->pServItems[i].isLocked    = 0;
    list->pServItems[i].key         = 0;
    list->servNum++;
    return 0;
}

/*******************************************************************************
Function name: isdb_update_serv_item
Description: 
    1.�����Ѿ����ڵ���
Parameters: 
    nAddFlag: ���Ҫ���µ�service�����б���,��ô�Ƿ�����. 1:����; 0:������,ֱ�ӷ���
Return: 
    EPDK_OK:���³ɹ������ӳɹ�
    EPDK_FAIL:����ʧ��
Time: 2012/6/19
*******************************************************************************/
__s32 isdb_update_serv_item(maple_serv_item_t* item, __s32 nAddFlag)
{
    __s32   i;
    __s32   ret;
    __u32               orgTsId;
    __u32               orgServId;
    __u32               frequency;
    maple_serv_list_t*  list;
    
    list = &g_isdbCtx->localServList;
    //1.����
    for(i=0; i<list->servNum; i++)
    {
        orgTsId     = list->pServItems[i].tsId;
        orgServId   = list->pServItems[i].servId;
        frequency   = list->pServItems[i].demod_pm.frequency;
        if((orgTsId == item->tsId) && (orgServId == item->servId) && (item->demod_pm.frequency == frequency))
        {
            memcpy(&list->pServItems[i], item, sizeof(maple_serv_item_t));
            return EPDK_OK;
        }
    }
    WARNING("not find serviceId[0x%x]\n", item->servId);
    if(nAddFlag)
    {
        if(list->servNum < list->nServItemsCount)
        {
            memcpy(&list->pServItems[list->servNum], item, sizeof(maple_serv_item_t));
            list->servNum++;
            ret = EPDK_OK;
        }
        else
        {
            WARNING("maple si lib service list is full, return!\n");
            ret = EPDK_FAIL;
        }
    }
    else
    {
        ret = EPDK_FAIL;
    }
    return ret;
}


//* set service item favourate flag
__s32 isdb_set_service_item_favourate_flag(__u32 channelId, __u32 isFavourate)
{
    __u32               i;
    maple_serv_list_t*  list;
    
    list = &g_isdbCtx->localServList;
    for(i=0; i<list->servNum; i++)
    {
        if(list->pServItems[i].channelId == channelId)
        {
            list->pServItems[i].isFavourate = isFavourate;
            break;
        }
    }
    
    return 0;
}


//* lock service item, key == -1 means unlock
__s32 isdb_set_service_item_lock(__u32 channelId, __s32 key)
{
    __u32               i;
    maple_serv_list_t*  list;
    
    list = &g_isdbCtx->localServList;
    for(i=0; i<list->servNum; i++)
    {
        if(list->pServItems[i].channelId == channelId)
        {
            if(key != -1)
            {
                list->pServItems[i].key      = key;
                list->pServItems[i].isLocked = 1;
            }
            else
            {
                list->pServItems[i].key      = -1;
                list->pServItems[i].isLocked = 0;
            }
            break;
        }
    }
    
    return 0;
}


//* add list to si lib.
__s32 isdb_add_list(maple_serv_list_t* newList)
{
    __s32 i;
    
    for(i=0; i<newList->servNum; i++)
    {
        isdb_add_serv_item(&newList->pServItems[i], 1);
    }
    
    return 0;
}


//* delete one service item
__s32 isdb_delete_serv_item(__u32 channelId)
{
    __u32               i;
    __u32               tmpServNum;
    maple_serv_list_t*  list;
    maple_serv_item_t*  pItems;
    
    list = &g_isdbCtx->localServList;
    
    for(i=0; i<list->servNum; i++)
    {
        if(channelId == list->pServItems[i].channelId)
            break;
    }
    
    if(i == list->servNum)
        return -1;
        
    pItems          = &list->pServItems[i+1];
    tmpServNum      = list->servNum - i - 1;
    list->servNum   = i;
    
    for(i=0; i<tmpServNum; i++)
    {
        isdb_add_serv_item(&pItems[i], 0);
    }
    return 0;
}

//* copy list from si lib.
__s32 isdb_get_list(maple_serv_list_t* list, __u32 type)
{
    __u32               i;
    maple_serv_list_t*  srcList;
    maple_serv_item_t*  srcItem;
    maple_serv_item_t*  dstItem;
    
    list->servNum = 0;
    srcList = &g_isdbCtx->localServList;
    
    if(srcList->servNum <= 0)
    {
        list->servNum = 0;
        return 0;
    }
    
    srcItem = srcList->pServItems;
    dstItem = list->pServItems;
    
    switch(type)
    {
        case MAPLE_SERV_GROUP_TYPE_TV:
        {
            for(i=0; i<srcList->servNum; i++)
            {
                if(srcItem->servType == MAPLE_SERV_TYPE_TV)
                {
                    memcpy(dstItem, srcItem, sizeof(maple_serv_item_t));
                    dstItem++;
                    list->servNum++;
                }
                srcItem++;
            }
            return 0;
        }
        
        case MAPLE_SERV_GROUP_TYPE_RADIO:
        {
            for(i=0; i<srcList->servNum; i++)
            {
                if(srcItem->servType == MAPLE_SERV_TYPE_RADIO)
                {
                    memcpy(dstItem, srcItem, sizeof(maple_serv_item_t));
                    dstItem++;
                    list->servNum++;
                }
                srcItem++;
            }
            return 0;
        }
        
        case MAPLE_SERV_GROUP_TYPE_TELETEXT:
        {
            for(i=0; i<srcList->servNum; i++)
            {
                if(srcItem->servType == MAPLE_SERV_TYPE_TELETEXT)
                {
                    memcpy(dstItem, srcItem, sizeof(maple_serv_item_t));
                    dstItem++;
                    list->servNum++;
                }
                srcItem++;
            }
            return 0;
        }
        
        case MAPLE_SERV_GROUP_TYPE_FAVOURATE:
        {
            for(i=0; i<srcList->servNum; i++)
            {
                if(srcItem->isFavourate == 1)
                {
                    memcpy(dstItem, srcItem, sizeof(maple_serv_item_t));
                    dstItem++;
                    list->servNum++;
                }
                srcItem++;
            }
            return 0;
        }
        
        case MAPLE_SERV_GROUP_TYPE_NEWS:
        case MAPLE_SERV_GROUP_TYPE_SCIENCE:
        case MAPLE_SERV_GROUP_TYPE_SPORT:
        case MAPLE_SERV_GROUP_TYPE_MOVIE:
        case MAPLE_SERV_GROUP_TYPE_CHILDREN:
        case MAPLE_SERV_GROUP_TYPE_MUSIC:
        case MAPLE_SERV_GROUP_TYPE_FATION:
        default:
        {
            list->servNum = srcList->servNum;
            memcpy(list->pServItems, srcList->pServItems, srcList->servNum*sizeof(maple_serv_item_t));
            return 0;
        }
    }
}


//* copy one service item from si lib.
__s32 isdb_get_service_item(maple_serv_item_t* item, __u32 channelId)
{
    __u32 i;
    
    for(i=0; i<g_isdbCtx->localServList.servNum; i++)
    {
        if(g_isdbCtx->localServList.pServItems[i].channelId == channelId)
            break;
    }
    
    if(i < g_isdbCtx->localServList.servNum)
    {
        memcpy(item, &g_isdbCtx->localServList.pServItems[i], sizeof(maple_serv_item_t));
        return 0;
    }
    else
    {
        return -1;
    }
}

//*****************************************************************************************************//
//************************** function for service search.**********************************************//
//************************** start manual service search***********************************************//
//******************************************************************************************************//

__s32 isdb_search_service(maple_demod_info_t* tp, maple_serv_list_t* servList)
{   
    if(g_isdbCtx->slSearchTaskId != 0 && g_isdbCtx->slSearchStopFlag == 0)
    {
        //* service scan task is running.
        return -1;
    }
    
    if(g_isdbCtx->schdlSearchTaskId != 0 && g_isdbCtx->schdlSearchStopFlag == 0)
    {
        //* cannot search service when scaning schedule.
        return -1;
    }
    
    if(g_isdbCtx->sepgSearchTaskId != 0 && g_isdbCtx->sepgSearchStopFlag == 0)
    {
        //* cannot search service when scaning simple epg.
        return -1;
    }
    
    if(g_isdbCtx->timeMonitorTaskId != 0 && g_isdbCtx->timeMonitorStopFlag == 0)
    {
        //* cannot search service when scaning time.
        return -1;
    }
    
    if(g_isdbCtx->epgMonitorTaskId != 0 && g_isdbCtx->epgMonitorStopFlag == 0)
    {
        //* cannot search service when scaning epg.
        return -1;
    }
    
    g_isdbCtx->slSearchStopFlag = 0;
    g_isdbCtx->frequency = tp->frequency;
    g_isdbCtx->bandwidth = tp->bandwidth;
    g_isdbCtx->slSearchTaskId = isdb_create_task(isdb_scan_task, servList, KRNL_priolevel3);
    if(g_isdbCtx->slSearchTaskId == 0)
    {
        return -1;
    }
    
    return 0;
}

/*******************************************************************************
Function name: isdb_search_service_by_serviceId
Description: 
    1.����serviceIdֻ��һ��service����ϸ��Ϣ,ֻ��pat���pmt��,��Ҫ��Ϊ�˵õ������ű����Ϣ,v/a/subt pid,pcrpid��.
    �����SI����.
    2.���Ը���maple si lib���ص����ݿ�(��service�б�)
    3.����ֻ���:patû�ѵ�,����service��û��pmt_pid�����.��������Ժ������ٸ�.
Parameters: 
    
Return: 
    EPDK_OK:�ɹ��ҵ�
    EPDK_FAIL:û���ҵ�.
Time: 2012/6/19
*******************************************************************************/
__s32 isdb_search_service_by_serviceId(maple_serv_item_t* pServItem)
{
    __s32   ret;
    ret = isdb_scan_by_serviceId(pServItem);
    if(EPDK_OK == ret)
    {
        ret = isdb_update_serv_item(pServItem, 0);
    }
    return ret;    
}

//* stop service search
__s32 isdb_stop_service_search(void)                                                                                        
{   
    if(g_isdbCtx->slSearchStopFlag == 1 || g_isdbCtx->slSearchTaskId == 0)
        return 0;
    
    isdb_delete_task(g_isdbCtx->slSearchTaskId);
    g_isdbCtx->slSearchTaskId = 0;
    return 0;
}

//*********************************************************************************************************//
//************************** function for simple EPG search.***********************************************//
//************************** start simple EPG search        ***********************************************//
//************************** task function for searching simple EPG.***************************************//
//*********************************************************************************************************//
#if 0
__s32 isdb_search_sepg(__s32 servIndex, maple_sepg_t* pSEpg)
{
    if(g_isdbCtx->slSearchTaskId != 0 && g_isdbCtx->slSearchStopFlag == 0)
    {
        //* service scan task is running.
        return -1;
    }
    
    if(g_isdbCtx->schdlSearchTaskId != 0 && g_isdbCtx->schdlSearchStopFlag == 0)
    {
        //* cannot search service when scaning schedule.
        return -1;
    }
    
    if(g_isdbCtx->sepgSearchTaskId != 0 && g_isdbCtx->sepgSearchStopFlag == 0)
    {
        //* cannot search service when scaning simple epg.
        return -1;
    }
    
    if(g_isdbCtx->epgMonitorTaskId != 0 && g_isdbCtx->epgMonitorStopFlag == 0)
    {
        //* cannot search service when scaning epg.
        return -1;
    }
    
    if(servIndex >= g_isdbCtx->localServList.servNum)
        return -1;
  
    g_isdbCtx->serviceIdxSepg = servIndex;
      
    g_isdbCtx->sepgSearchStopFlag = 0;
    g_isdbCtx->sepgSearchTaskId = isdb_create_task(isdb_sepg_scan_task, pSEpg, KRNL_priolevel4);
    if(g_isdbCtx->sepgSearchTaskId == 0)
    {
        return -1;
    }
    
    return 0;
}

//* stop simple EPG search
__s32 isdb_stop_sepg_search(void)
{
    if(g_isdbCtx->sepgSearchStopFlag == 1 || g_isdbCtx->sepgSearchTaskId == 0)
        return 0;
    
    isdb_delete_task(g_isdbCtx->sepgSearchTaskId);
    g_isdbCtx->sepgSearchTaskId = 0;
    return 0;
}
#endif

//***************************************************************************************************//
//********************************** function for schedule EPG search *******************************//
//*********************************** start schedule search *****************************************//
//***************************************************************************************************//

#if 0
__s32 isdb_search_schedule(__s32 servIndex, maple_schdl_list_t* pSchedule)
{   
    if(g_isdbCtx->slSearchTaskId != 0 && g_isdbCtx->slSearchStopFlag == 0)
    {
        //* service scan task is running.
        return -1;
    }
    
    if(g_isdbCtx->schdlSearchTaskId != 0 && g_isdbCtx->schdlSearchStopFlag == 0)
    {
        //* cannot search service when scaning schedule.
        return -1;
    }
    
    if(g_isdbCtx->sepgSearchTaskId != 0 && g_isdbCtx->sepgSearchStopFlag == 0)
    {
        //* cannot search service when scaning simple epg.
        return -1;
    }
    
    if(g_isdbCtx->epgMonitorTaskId != 0 && g_isdbCtx->epgMonitorStopFlag == 0)
    {
        //* cannot search service when scaning epg.
        return -1;
    }
    
    if(servIndex >= g_isdbCtx->localServList.servNum)
        return -1;
    
    g_isdbCtx->serviceIdxSchdl = servIndex;
    
    g_isdbCtx->schdlSearchStopFlag = 0;
    g_isdbCtx->schdlSearchTaskId = isdb_create_task(isdb_schedule_scan_task, pSchedule, KRNL_priolevel4);
    if(g_isdbCtx->schdlSearchTaskId == 0)
    {
        return -1;
    }
    
    return 0;
}


//* stop schedule search
__s32 isdb_stop_schedule_search(void)
{
    if(g_isdbCtx->schdlSearchStopFlag == 1 || g_isdbCtx->schdlSearchTaskId == 0)
        return 0;
    
    isdb_delete_task(g_isdbCtx->schdlSearchTaskId);
    g_isdbCtx->schdlSearchTaskId = 0;
    return 0;
}
#endif


//***************************************************************************************************//
//***************************** function for tv stream time monitor *********************************//
//******************************* start time monitor                *********************************//

__s32 isdb_open_time_monitor(void)
{
#if SUPPORT_ISDB_EPG
    if(g_isdbCtx->slSearchTaskId != 0 && g_isdbCtx->slSearchStopFlag == 0)
    {
        //* service scan task is running.
        return -1;
    }
    
    if(g_isdbCtx->timeMonitorTaskId != 0 && g_isdbCtx->timeMonitorStopFlag == 0)
    {
        return -1;
    }
    
    g_isdbCtx->timeMonitorStopFlag = 0;
    g_isdbCtx->timeMonitorTaskId = isdb_create_task(isdb_time_monitor_task, NULL, KRNL_priolevel4);
    if(g_isdbCtx->timeMonitorTaskId == 0)
    {
        return -1;
    }
#endif
    
    return 0;
}

//* stop time monitor
__s32 isdb_close_time_monitor(void)
{   
#if SUPPORT_ISDB_EPG
    if(g_isdbCtx->timeMonitorStopFlag == 1 || g_isdbCtx->timeMonitorTaskId == 0)
        return 0;
    
    isdb_delete_task(g_isdbCtx->timeMonitorTaskId);
    g_isdbCtx->timeMonitorTaskId = 0;
#endif

    return 0;
}

//* get current stream time
__s32 isdb_get_time(maple_time_t* curTime)
{  
#if SUPPORT_ISDB_EPG
    WARNING("year = %d, month = %d, day = %d, second = %d.\n", curTime->year, curTime->month, curTime->day, curTime->sec);
    curTime->year    = g_isdbCtx->curStreamTime.year;
    curTime->month   = g_isdbCtx->curStreamTime.month;
    curTime->day     = g_isdbCtx->curStreamTime.day;
    curTime->weekDay = g_isdbCtx->curStreamTime.weekDay;
    curTime->hour    = g_isdbCtx->curStreamTime.hour;
	curTime->min     = g_isdbCtx->curStreamTime.min; 
    curTime->sec     = g_isdbCtx->curStreamTime.sec;
    curTime->MJD     = g_isdbCtx->curStreamTime.MJD;
#else
    curTime->year    = 0;
    curTime->month   = 0;
    curTime->day     = 0;
    curTime->weekDay = 0;
    curTime->hour    = 0;
	curTime->min     = 0;
    curTime->sec     = 0;
    curTime->MJD     = 0;
#endif
    return 0;
}


//***************************************************************************************************//
//************************************** function for EPG monitor ***********************************//
//***************************************************************************************************//

__s32 isdb_open_epg_monitor(void)
{
#if SUPPORT_ISDB_EPG
    __u8 err;
    
    if(g_isdbCtx->slSearchTaskId != 0 && g_isdbCtx->slSearchStopFlag == 0)
    {
        //* service scan task is running.
        return -1;
    }
    
    if(g_isdbCtx->schdlSearchTaskId != 0 && g_isdbCtx->schdlSearchStopFlag == 0)
    {
        //* cannot search service when scaning schedule.
        return -1;
    }
    
    if(g_isdbCtx->sepgSearchTaskId != 0 && g_isdbCtx->sepgSearchStopFlag == 0)
    {
        //* cannot search service when scaning simple epg.
        return -1;
    }
    
    if(g_isdbCtx->epgMonitorTaskId != 0 && g_isdbCtx->epgMonitorStopFlag == 0)
    {
        //* cannot search service when scaning simple epg.
        return -1;
    }
    
    if(g_isdbCtx->epgMutex != NULL)
    {
        return -1;
    }
    
    g_isdbCtx->epgMutex = esKRNL_SemCreate(1);
    if(g_isdbCtx->epgMutex == NULL)
    {
        return -1;
    }
    
	eit_parser_clean();
	if(eit_pool_init(EIT_POOL_BUFFER_SIZE) != 0)
	{
	    MESSAGE("eit_pool_init fail.\n");
        esKRNL_SemDel(g_isdbCtx->epgMutex, OS_DEL_ALWAYS, &err);
        g_isdbCtx->epgMutex = NULL;
        return -1;
	}
    
    g_isdbCtx->epgMonitorStopFlag = 0;
    g_isdbCtx->epgMonitorTaskId = isdb_create_task(isdb_epg_monitor_task, NULL, KRNL_priolevel4);
    if(g_isdbCtx->epgMonitorTaskId == 0)
    {
        WARNING("fatal error! create isdb_epg_monitor_task fail!\n");
        eit_pool_release();
        esKRNL_SemDel(g_isdbCtx->epgMutex, OS_DEL_ALWAYS, &err);
        g_isdbCtx->epgMutex = NULL;
        return -1;
    }
#endif
    
    return 0;
}

//* stop schedule search
__s32 isdb_close_epg_monitor(void)
{
#if SUPPORT_ISDB_EPG
    __u8 err;
    
    if(g_isdbCtx->epgMonitorStopFlag == 1 || g_isdbCtx->epgMonitorTaskId == 0)
    {
        if(g_isdbCtx->epgMutex)
        {
            esKRNL_SemDel(g_isdbCtx->epgMutex, OS_DEL_ALWAYS, &err);
            g_isdbCtx->epgMutex = NULL;
        }
        return 0;
    }
    
    isdb_delete_task(g_isdbCtx->epgMonitorTaskId);
    g_isdbCtx->epgMonitorTaskId = 0;
    
    if(g_isdbCtx->epgMutex)
    {
        esKRNL_SemDel(g_isdbCtx->epgMutex, OS_DEL_ALWAYS, &err);
        g_isdbCtx->epgMutex = NULL;
    }
    
	eit_parser_clean();
	eit_pool_release();
#endif

    return 0;
}

//* get simple epg of specified service
__s32 isdb_get_sepg(__s32 channelId, maple_sepg_t* pSEpg)
{
#if SUPPORT_ISDB_EPG
    __u32               i;
    maple_serv_list_t*  list;
    __u32               tsId;
    __u32               servId;
    __u32               origNetworkId;

    memset(pSEpg, 0, sizeof(maple_sepg_t));
    
    list = &g_isdbCtx->localServList;
  
    for(i=0; i<list->servNum; i++)
    {
        if(channelId == list->pServItems[i].channelId)
            break;
    }
    
    if(i == list->servNum)
    {
        pSEpg->hasNextEventInfo = 0;
        pSEpg->hasCurEventInfo  = 0;
        return -1;
    }
    
    origNetworkId = list->pServItems[i].networkId;
    tsId          = list->pServItems[i].tsId;
    servId        = list->pServItems[i].servId;
    
    if(g_isdbCtx->epgMonitorTaskId != 0 && g_isdbCtx->epgMonitorStopFlag == 0)
    {
        //* lock epg table mutex.
        if(isdb_get_mutex(g_isdbCtx->epgMutex) != 0)
            return -1;
            
        eit_pool_parse_content(0, origNetworkId, tsId, servId, epg_type_pf);
    	eit_parser_get_pf(0, tsId, origNetworkId, servId, pSEpg);
        
        //* unlock epg table mutex.
        isdb_release_mutex(g_isdbCtx->epgMutex);
    }
    
    return 0;
#else
    pSEpg->hasNextEventInfo = 0;
    pSEpg->hasCurEventInfo  = 0;
    return -1;
#endif
}

//* get schedule of specified service
__s32 isdb_get_schedule(__s32 channelId, maple_schdl_list_t* pSchedule)
{
#if SUPPORT_ISDB_EPG
    __u32               i;
    maple_serv_list_t*  list;
    __u32               tsId;
    __u32               servId;
    __u32               origNetworkId;
    
    list = &g_isdbCtx->localServList;

    
    for(i=0; i<list->servNum; i++)
    {
        if(channelId == list->pServItems[i].channelId)
            break;
    }
    
    if(i == list->servNum)
    {
        pSchedule->eventNum = 0;
        return -1;
    }
    
    origNetworkId = list->pServItems[i].networkId;
    tsId          = list->pServItems[i].tsId;
    servId        = list->pServItems[i].servId;
        
    if(g_isdbCtx->epgMonitorTaskId != 0 && g_isdbCtx->epgMonitorStopFlag == 0)
    {
        //* lock epg table mutex.
        if(isdb_get_mutex(g_isdbCtx->epgMutex) != 0)
            return -1;
            
        eit_pool_parse_content(0, origNetworkId, tsId, servId, epg_type_sch);
    		eit_parser_get_schedule(0, tsId, origNetworkId, servId, pSchedule, MAPLE_MAX_SCHDL_NUM_PER_SERV);
            
        //* unlock epg table mutex.
        isdb_release_mutex(g_isdbCtx->epgMutex);
    }
    
    return 0;
#else
    pSchedule->eventNum = 0;
    return -1;
#endif
}

//* set current playing service
__s32 isdb_set_current_service(__u32 channelId)
{
    __u32 i, j;
    __u32 frequency;
    active_service_t service[EIT_POOL_MAX_ACTIVE_SERVICE_NUM];
    
    for(i=0; i<g_isdbCtx->localServList.servNum; i++)
    {
        if(g_isdbCtx->localServList.pServItems[i].channelId == channelId)
            break;
    }
    
    if(i == g_isdbCtx->localServList.servNum)
        return -1;
        
    g_isdbCtx->localServList.curServIdx = i;
    frequency = g_isdbCtx->localServList.pServItems[i].demod_pm.frequency;
    
    for(i=0, j=0; i<g_isdbCtx->localServList.servNum; i++)
    {
        if(g_isdbCtx->localServList.pServItems[i].demod_pm.frequency == frequency)
        {
            service[j].tpId          = 0;
            service[j].origNetworkId = g_isdbCtx->localServList.pServItems[i].networkId;
            service[j].tsId          = g_isdbCtx->localServList.pServItems[i].tsId;
            service[j].serviceId     = g_isdbCtx->localServList.pServItems[i].servId;
            j++;
        }
    }
    
    if(g_isdbCtx->epgMonitorTaskId != 0 && g_isdbCtx->epgMonitorStopFlag == 0)
    {
        //* lock epg table mutex.
        if(isdb_get_mutex(g_isdbCtx->epgMutex) != 0)
            return -1;
            
        eit_pool_set_active_services(service, j);
        
        //* unlock epg table mutex.
        isdb_release_mutex(g_isdbCtx->epgMutex);
    }
    return 0;
}


//* get service number of service list
__s32 isdb_get_service_num(void)
{
    if(g_isdbCtx)
        return g_isdbCtx->localServList.servNum;
    else
        return 0;
}

//* get current service id
__s32 isdb_get_cur_service_id(void)
{
    if(g_isdbCtx && g_isdbCtx->localServList.pServItems)
    {
        if(g_isdbCtx->localServList.curServIdx >= 0 && g_isdbCtx->localServList.curServIdx < g_isdbCtx->localServList.servNum)
            return g_isdbCtx->localServList.pServItems[g_isdbCtx->localServList.curServIdx].channelId;
    }
    
    return -1;
}

#if 0   //������
/*******************************************************************************
Function name: JIS2Uni
Description: 
    1.��JIS�ַ���תΪEUC-JP����.����������\0,����dst��bufferҪ��һЩ,���ַ�����
    ��Ҫ������һ��\0�ֽ�,���ǹ涨����Ƕ�����2���ֽ�.
    2.eucjp��˫�ֽڱ���,���ܻ���һ���ֽ�Ϊ\0,����ҪС��.
Parameters: 
    dstLen:dst buffer���ܳ���,����Ҫ����\0����.
Return: 
    ת������ַ����ĳ���,������\0.
Time: 2011/2/16
*******************************************************************************/
__u32 jis2eucjp(__u8* src, __u32 srcLen, __u8* dst, __u32 dstSize)
{
    char *tmp;
    __s32 dstStringLen;
    __s32   i;
    if(dstSize <= 0)
    {
        WARNING("fatal error! dstStringlen[%d], check code!\n", dstSize);
        return 0;
    }
    memset(dst, 0xff, dstSize);
    tmp = arib2euc(srcLen, (char*)src, (char*)dst);
    if((__u8*)tmp != dst)
    {
        WARNING("fatal error! tmp[0x%x]!=dst[0x%x], check code!\n", tmp, dst);
    }
    //����dst buffer�洢���ַ����ĳ���.������\0.Ѱ�����һ��0�ֽ�
    for(i = dstSize - 1; i >= 0; i--)
    {
        if(dst[i] == 0)
        {
            break;
        }
    }
    if(i >= 0)
    {
        dstStringLen = i;
    }
    else
    {
        WARNING("fatal error! not find 0 byte String convert wrong!\n");
        dstStringLen = dstSize;
    }
    //if(dstStringLen%2 != 0)
    //{
    //    WARNING("Be careful! dstSTringLen[%d] %2 != 0\n", dstStringLen);
    //}
    //dstStringLen = strlen(dst);
    if(dstStringLen > dstSize - 2)
    {
        WARNING("fatal error! dstStringLen[%d]>dstSize[%d-2]\n", dstStringLen, dstSize);
        dstStringLen = dstSize - 2;
        //���ﲻ�����2�ֽ���0��,������һ������Կ�����.
    }
    return dstStringLen;
}

/*******************************************************************************
Function name: eLIBs_charset_convert_eucjp2utf8
Description: 
    1.������ֻ�����ַ���ת��,����\0������.����,�����dstbuf���п��ܻ�����dst_sizeָʾ�ĳ���
    ,�������\0���ڵ���ǰ����Ԥ������.
Parameters: 
    
Return: 
    ת������ַ����ĳ���,������\0
Time: 2011/2/16
*******************************************************************************/
__u32  eLIBs_charset_convert_eucjp2utf8( __s32 src_charset, __s32 dst_charset,
                              const __u8 *src, __u32 src_size,
                              __u8 *dst, __u32 dst_size )
{
	__s32  src_char_len;
	__s32  dst_char_len;
	__s32  src_total_len;
	__s32  dst_total_len;
	__u16  uni;

	if( src == NULL || src_size == 0 || dst == NULL || dst_size == 0 )
		return 0;

	src_char_len  = 0;
	dst_char_len  = 0;
	src_total_len = 0;
	dst_total_len = 0;
	while( 1 )
	{
		//src_char_len = esCHS_Char2Uni( src_charset, src + src_total_len, src_size - src_total_len, &uni );
		src_char_len = EUCJP_char2uni(src + src_total_len, src_size - src_total_len, &uni);
		if( src_char_len <= 0 )
			return dst_total_len;
		src_total_len += src_char_len;

		dst_char_len = esCHS_Uni2Char( dst_charset, uni, dst + dst_total_len, dst_size - dst_total_len );
		if( dst_char_len <= 0 )
		{
            if(src_size > src_total_len)
            {
                WARNING("FATAL error! string is not convert done! [%d] > [%d], [%d][%d]\n", src_size, src_total_len, dst_size, dst_total_len);
            }
			return dst_total_len;
		}
		dst_total_len += dst_char_len;
	}
}

#endif

