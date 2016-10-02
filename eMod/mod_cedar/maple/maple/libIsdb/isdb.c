
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
    eucStringBuffer最好在g_isdbCtx中malloc完成.
    转换完成后会加2个字节的\0.
Parameters: 
    dstLen:dst buffer的总长度,在转换时需要考虑\0的因素.所以dstLen最好>2.
Return: 
    转换后的UTF8字符串的长度.不包括\0.
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
    //系统本身不支持JIS编码,所以由中间件自己处理.JIS -> EUC-JP -> unicode -> UTF8
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
    //直接调用aribjis->utf8的字符转换库
    utf8StringLen = conv_arib_to_utf8((char*)src, (int)srcLen, (char*)dst, (int)dstLen - 2, GraphicSetsDesignationMode_SI);
    dst[utf8StringLen] = 0;
    dst[utf8StringLen+1] = 0;
#endif
    
    return utf8StringLen;
}

typedef int (*mapleServItemCompareFunc)(void* ctx, void* data); //ret = 0表示匹配. data就是void*元素, ctx是匹配值，用户临时传入的上下文，一般和void*指向的struct类型是相同的.用于DArray_FindElem(), ctx >= data, 返回值为>=0.
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

static __u32 IntLog2(__u32 n)   //模拟计算以2为底的对数,2^N >= n就可以
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
    1.一次分割，以一个元素x为准，分为左边的元素都 < x, 右边的元素都>=x,
    2.一般情况下, 元素x就选left
Parameters: 
    
Return: 
    元素x在分割后的位置
Time: 2011/5/11
*******************************************************************************/
unsigned int QuickSort_Split_mapleServItem(maple_serv_item_t* array, unsigned int left, unsigned int right, mapleServItemCompareFunc cmp)
{
    maple_serv_item_t x;
    memcpy(&x, &array[left], sizeof(maple_serv_item_t));
    // 如果left == right, 说明本次排序的区域只剩一个元素,直接返回了
    while(left < right)
	{
        //第一个while循环, x在left处. left和right之间(包括right，可以不包括left)都要查,因为x在left处, left不动, 从右往左查
		while(left < right && cmp(&array[right], &x) >= 0) right--;
        //这儿left == right说明全部>=x，x就在原位就可以了，继续执行将推出while循环
        //如果left != right (一般left < right), 说明找到了一个比x更小的元素,那么x的位置就让给那个更小的元素,
        //至于x自己，可以认为占据了right的位置, right位置的元素移动到left的位置, left再+1.
		if(left != right)
		{
			//array[left] = array[right];
            memcpy(&array[left], &array[right], sizeof(maple_serv_item_t));
			left++; //left原来的位置已经占据了，所以从下一个位置开始寻找适合left的位置
		}
        //第一个while循环结束后(left已经++了)，可以保证:right右边的元素都>= x, left左边的元素都<x (不包括left, right自己, right自己现在就是x)，
        // left, right之间的元素(包括left, right自己)还有待排序.

        //第二个while循环开始,此时x在right的位置,所以从left开始向右遍历.如果遍历到right，可以认为right就是x应该在的位置. right左边, left右边(包括left)都小于x
        //这个<=0有问题，我们的希望是小于left的元素在左边，大于等于left的元素在右边，所以改为<. 原始代码写成<=,最后的结果也不会错,但是x元素的位置就不容易确定了,
        //我们的希望是:如果有很多值和x相同的元素，那么x在最左边.
        //还是改成<=,好处是可以少移动元素.减少排序过程中的拷贝.并且就算改成<,也不能保证相等的元素按位置排列。
		//while(cmp(array[left], x) <= 0 && left < right)	left++;
		while(left < right && cmp(&array[left], &x) <= 0)	left++;
		if(left != right)   //说明找到了 >= x的元素, 如果left == right，那么x就应该在right的位置上
		{
			//array[right] = array[left];
			memcpy(&array[right], &array[left], sizeof(maple_serv_item_t));
			right--;
		}
        //第二个while结束时,可以认为x到了left的地方,right被那个>=x的元素占据.
        //然后right--, 现在只有新的left和right之间的元素没有经过排序了(包括right, 而left可以认为就是x暂时占据的地方了)
	}
    //因为退出while()循环时, left == right.所以x的位置就此确定， left绝对不可能大于right.
	//array[left] = x;
	memcpy(&array[left], &x, sizeof(maple_serv_item_t));
    
    return left;
}

/*******************************************************************************
Function name: QuickSort_NonRecursive
Description: 
    2. 关于stack长度的问题:
        另外也可以借助一个模拟的求以2为底的对数的函数IntLog2().
       在本函数中, 一个stack_item是2个__u32的数，记录uLow和uHigh.
       不等式的由来:每次分成2个区间后，其中大的区间入栈,小区间入栈再立刻出栈, 继续排序,
    3.默认按升序排列.
    4. 在使用栈的非递归算法中, 模拟栈
排序原理:
    1.不断分区, 小于某元素的值在第一区,大于等于的值在第二区. 直到所在的区只剩下一个元素为止.
    元素的选取是随机的，一般选最左边的.
    这里，小于元素的值全部移到左边, 大于等于的在右边
    2.可以保证的是left <= right.
    3.排序结束后的效果是:
        升序: 在同值的元素中，不能保证初始位置在最左边的仍在最左边,.
        降序: 在同值的元素中，不能保证初始位置在最左边的仍在最左边,
Parameters: 
    1. pTable:数组第一个元素的地址, 类型为maple_serv_item_t
    2. nSize: 元素个数
    3. CompareFunc:比较函数,
        >=0: 表示DataCompareFunc的ctx >= data, ctx在左边，是第一个参数
        <0: 表示ctx < data
    4. DataCopy:赋值函数
    
2. 不使用栈的非递归算法
    
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
    unsigned int    max_stacktop = 0;   //记录实际运行容纳的最大元素数.
    unsigned int    uStackItemSize; //记录stack能容纳的Item数，防止数组越界. 在这里,一个Item由2个unsigned int 组成
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
        if(max_stacktop < uStackTop)    //统计运行时实际用到的栈元素个数
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
    SORTMODE_REMOTE_CONTROL_KEY_ID模式下比较service的规则:
    remote_control_key_id
    frequency
    service_num

    如果完全相同,报错.
Parameters: 
    nSortMode:0:按remote control key id排序,相同的id号的service再按service number排序.默认从小到大排序;
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
    1.更新已经存在的项
Parameters: 
    nAddFlag: 如果要更新的service不在列表中,那么是否增加. 1:增加; 0:不增加,直接返回
Return: 
    EPDK_OK:更新成功或增加成功
    EPDK_FAIL:更新失败
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
    //1.查找
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
    1.根据serviceId只搜一个service的详细信息,只搜pat表和pmt表,主要是为了得到这两张表的信息,v/a/subt pid,pcrpid等.
    其余的SI表不搜.
    2.可以更新maple si lib本地的数据库(即service列表)
    3.现在只针对:pat没搜到,导致service项没有pmt_pid的情况.其他情况以后遇到再改.
Parameters: 
    
Return: 
    EPDK_OK:成功找到
    EPDK_FAIL:没有找到.
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

#if 0   //封存代码
/*******************************************************************************
Function name: JIS2Uni
Description: 
    1.将JIS字符串转为EUC-JP编码.本函数考虑\0,所以dst的buffer要长一些,除字符串外
    还要能容纳一个\0字节,我们规定最好是多容纳2个字节.
    2.eucjp的双字节编码,可能会有一个字节为\0,所以要小心.
Parameters: 
    dstLen:dst buffer的总长度,所以要考虑\0因素.
Return: 
    转换后的字符串的长度,不包括\0.
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
    //计算dst buffer存储的字符串的长度.不包括\0.寻找最后一个0字节
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
        //这里不把最后2字节置0了,这样万一出错可以看出来.
    }
    return dstStringLen;
}

/*******************************************************************************
Function name: eLIBs_charset_convert_eucjp2utf8
Description: 
    1.本函数只关心字符的转换,不管\0的事情.所以,送入的dstbuf，有可能会填满dst_size指示的长度
    ,如果想有\0，在调用前自行预留处理.
Parameters: 
    
Return: 
    转换后的字符串的长度,不包括\0
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

