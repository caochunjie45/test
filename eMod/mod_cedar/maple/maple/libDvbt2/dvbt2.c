
#include "dvbt2_interface.h"
#include "dvbt2_context.h"
#include "dvbt2_eit_pool.h"
#include "dvbt2_eit_parser.h"

dvbt2_ctx_t* g_dvbt2Ctx = NULL;

static __u32 dvbt2_generate_channel_id()
{
    if(g_dvbt2Ctx)
        return g_dvbt2Ctx->localServList.servNum;
    else
        return 0;
}


__s32 dvbt2_get_mutex(__krnl_event_t* mutex)
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


void  dvbt2_release_mutex(__krnl_event_t* mutex)
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


__s32 dvbt2_set_default_char_code_type(__epdk_charset_enm_e codeType)
{
    g_dvbt2Ctx->charCodeType = codeType;
    return 0;
}

void dvbt2_remove_emphasize_char(__u8 *orgPtr, __u8 *srcPtr, __u8 orgNameLen, __u8 *srcNameLen)
{
    __u32 i;
    __u32 j;
    __u32 flag1;
    __u32 flag2;
    __u8* dataPtr;
    
    i       = 0;
    dataPtr = orgPtr;
    while(i < orgNameLen)
    {
        flag1 = (dataPtr[i] <= 0x1f);
        flag2 = (dataPtr[i] >= 0x7f && dataPtr[i] <= 0x9f);
        if(flag1 == 0 && flag2 == 0)
            break;
        
        i++;
    }
    
    j = orgNameLen;
    while(j > 0)
    {
        flag1 = (dataPtr[j-1] <= 0x1f);
        flag2 = (dataPtr[j-1] >= 0x7f && dataPtr[j-1] <= 0x9f);
        if(flag1 == 0 && flag2 == 0)
            break;
            
        j--;
    }
    
    *srcNameLen = orgNameLen;
    
    if(j < orgNameLen)
        *srcNameLen -= (orgNameLen - j);
        
    if(i > 0)
        *srcNameLen -= i;
    
    memcpy(srcPtr, orgPtr+i, *srcNameLen);
}


//* function for converting string to utf8
__s32 dvbt2_convert_str_to_utf8(__u8* src, __u32 srcLen, __u8* dst, __u32 dstLen, __u8* lang_code)
{
    __epdk_charset_enm_e code_type;
    __u8                 first_byte;
    __u8*                ptr;
    __u32                startPos;
	__u8                 realNameLen = 0;
	__u8                 realNamePtr[MAPLE_MAX_NAME_LEN] = {0};
    
    ptr         = src;
    first_byte  = ptr[0];
    code_type   = EPDK_CHARSET_ENM_UNKNOWN;
    
    if(first_byte < 0x20)
    {
        ptr++;
        switch(first_byte)
        {
            case 0x01:
            {
                code_type = EPDK_CHARSET_ENM_ISO_8859_5;
                break;
            }
            
            case 0x02:
            {
                code_type = EPDK_CHARSET_ENM_ISO_8859_6;
                break;
            }
            
            case 0x03:
            {
                code_type = EPDK_CHARSET_ENM_ISO_8859_7;
                break;
            }
                        
            case 0x04:
            {
                code_type = EPDK_CHARSET_ENM_ISO_8859_8;
                break;
            }
            
            case 0x05:
            {
                code_type = EPDK_CHARSET_ENM_ISO_8859_9;
                break;
            }
            
//            case 0x06:
//            {
//                code_type = EPDK_CHARSET_ENM_ISO_8859_10;
//                break;
//            }
            
           //case 0x07:
           // {
           //     code_type = EPDK_CHARSET_ENM_ISO_8859_11;
           //     break;
           // }

            case 0x07:
            {
                code_type = EPDK_CHARSET_ENM_CP874;
                break;
            }
            
//            case 0x08:
//            {
//                code_type = EPDK_CHARSET_ENM_ISO_8859_12;
//                break;
//            }
            
            case 0x09:
            {
                code_type = EPDK_CHARSET_ENM_ISO_8859_13;
                break;
            }
            
//            case 0x0a:
//             case 0x0e:      //* guess by debugging some Brazil streams.
//             {
//                 code_type = EPDK_CHARSET_ENM_ISO_8859_14;
//                 break;
//             }
            
            case 0x0b:
            {
                code_type = EPDK_CHARSET_ENM_ISO_8859_15;
                break;
            }
            
            case 0x10:
            {
                ptr++;
                first_byte = *ptr++;
                switch(first_byte)
                {
                    case 0x01:
                    {
                        code_type = EPDK_CHARSET_ENM_ISO_8859_1;
                        break;
                    }
                    
                    case 0x02:
                    {
                        code_type = EPDK_CHARSET_ENM_ISO_8859_2;
                        break;
                    }
                    
                    case 0x03:
                    {
                        code_type = EPDK_CHARSET_ENM_ISO_8859_3;
                        break;
                    }
                    
                    case 0x04:
                    {
                        code_type = EPDK_CHARSET_ENM_ISO_8859_4;
                        break;
                    }
                    
                    case 0x05:
                    {
                        code_type = EPDK_CHARSET_ENM_ISO_8859_5;
                        break;
                    }
                    
                    case 0x06:
                    {
                        code_type = EPDK_CHARSET_ENM_ISO_8859_6;
                        break;
                    }
                    
                    case 0x07:
                    {
                        code_type = EPDK_CHARSET_ENM_ISO_8859_7;
                        break;
                    }
                    
                    case 0x08:
                    {
                        code_type = EPDK_CHARSET_ENM_ISO_8859_8;
                        break;
                    }
                    
                    case 0x09:
                    {
                        code_type = EPDK_CHARSET_ENM_ISO_8859_9;
                        break;
                    }
                    
//                    case 0x0a:
//                    {
//                        code_type = EPDK_CHARSET_ENM_ISO_8859_10;
//                        break;
//                    }
                    
                    case 0x0b:
                    {
                        code_type = EPDK_CHARSET_ENM_ISO_8859_11;
                        break;
                    }
                    
                    //* spec reserve 0xc for future use
                    case 0x0d:
                    {
                        code_type = EPDK_CHARSET_ENM_ISO_8859_13;
                        break;
                    }
                    
//                    case 0x0e:
//                    {
//                        code_type = EPDK_CHARSET_ENM_ISO_8859_11;
//                        break;
//                    }
                    
                    case 0x0f:
                    {
                        code_type = EPDK_CHARSET_ENM_ISO_8859_15;
                        break;
                    }
                    
                    default:
                    {
                        break;
                    }
                }
                
                break;
            }
            
            case 0x11:
            {
                code_type = EPDK_CHARSET_ENM_UTF16BE;       //* i don't know whether it is little endian or big endian.
                break;
            }
            
            case 0x12:
            {
                code_type = EPDK_CHARSET_ENM_EUC_KR;
                break;
            }
            
            case 0x13:
            {
                code_type = EPDK_CHARSET_ENM_GB2312;
                break;
            }
            
            case 0x14:
            {
                code_type = EPDK_CHARSET_ENM_BIG5;
                break;
            }
            
            case 0x15:
            {
                code_type = EPDK_CHARSET_ENM_UTF8;
                break;
            }
            
            default:
            {
                break;
            }
        }
    }
    
    startPos = ptr - src;
    
    if(code_type == EPDK_CHARSET_ENM_UNKNOWN)
	{
	    code_type = g_dvbt2Ctx->charCodeType;
	}
        
    if(code_type != EPDK_CHARSET_ENM_UTF8)
    {
        if((code_type >= EPDK_CHARSET_ENM_ISO_8859_1) && (code_type <= EPDK_CHARSET_ENM_ISO_8859_16))
        {
            dvbt2_remove_emphasize_char(ptr, realNamePtr, srcLen - startPos, &realNameLen);
            realNameLen = eLIBs_charset_convert(code_type, EPDK_CHARSET_ENM_UTF8, realNamePtr, realNameLen, dst, dstLen-1);
            dst[realNameLen] = 0;
        }
        else
        {
            realNameLen = eLIBs_charset_convert(code_type, EPDK_CHARSET_ENM_UTF8, ptr, srcLen - startPos, dst, dstLen-1);
            dst[realNameLen] = 0;
        }
    }
    else
    {   
        if((dstLen-1) >= (srcLen - startPos))
        {
            memcpy(dst, ptr, srcLen - startPos);
            dst[srcLen - startPos] = 0;
            realNameLen = srcLen - startPos;
        }
        else
        {
            memcpy(dst, ptr, dstLen - 1);
            dst[dstLen - 1] = 0;
            realNameLen = dstLen - 1;
        }
    }
    
    return realNameLen;
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
dvbt2_ctx_t* newDvbt2Ctx()
{
    __s32   i;
    dvbt2_ctx_t* g_dvbt2Ctx;
    g_dvbt2Ctx = (dvbt2_ctx_t*)malloc(sizeof(dvbt2_ctx_t));
    if(g_dvbt2Ctx == NULL)
    {
        WARNING("malloc fail\n");
        return NULL;
    }
    memset(g_dvbt2Ctx, 0, sizeof(dvbt2_ctx_t));

 
    g_dvbt2Ctx->dvbt2MallocBuf = (__u8 *) malloc(sizeof(__u8)*SECTION_BUFIDX_CNT*DVBT2_SECTION_BUF_SIZE);
    if(g_dvbt2Ctx->dvbt2MallocBuf == NULL)
    {
       MESSAGE("malloc memory for si section buffer fail.\n");
       //return -1;
       goto _err0;
     }

	
    g_dvbt2Ctx->localServList.pServItems = (maple_serv_item_t *) malloc(sizeof(maple_serv_item_t)*MAPLE_MAX_SERV_NUM);
    if (g_dvbt2Ctx->localServList.pServItems == NULL)
    {
        MESSAGE("malloc memory for service list fail.\n");
//        free(g_dvbt2Ctx->dvbt2MallocBuf);
//        g_dvbt2Ctx->dvbt2MallocBuf = NULL;
//        free(g_dvbt2Ctx);
//        g_dvbt2Ctx = NULL;
//        return -1;
        goto _err0;
    }
    memset(g_dvbt2Ctx->localServList.pServItems, 0, sizeof(maple_serv_item_t)*MAPLE_MAX_SERV_NUM);
    g_dvbt2Ctx->localServList.servNum = 0;
    g_dvbt2Ctx->localServList.nServItemsCount = MAPLE_MAX_SERV_NUM;
    g_dvbt2Ctx->charCodeType          = EPDK_CHARSET_ENM_ISO_8859_15;    //* default character coding method is ISO8859_15_FDIS

    for(i=0; i<SECTION_BUFIDX_CNT; i++)
    {
       g_dvbt2Ctx->dvbt2SectionBuf[i]     = (__u8 *)(g_dvbt2Ctx->dvbt2MallocBuf+i*DVBT2_SECTION_BUF_SIZE);
    }
    return g_dvbt2Ctx;

_err0:
    if(g_dvbt2Ctx->dvbt2MallocBuf)
    {
        free(g_dvbt2Ctx->dvbt2MallocBuf);
        g_dvbt2Ctx->dvbt2MallocBuf = NULL;
    }
    if(g_dvbt2Ctx->localServList.pServItems)
    {
        free(g_dvbt2Ctx->localServList.pServItems);
        g_dvbt2Ctx->localServList.pServItems = NULL;
    }
    free(g_dvbt2Ctx);
    return NULL;
}

void deleteDvbt2Ctx(dvbt2_ctx_t *thiz)
{
    dvbt2_ctx_t  *g_dvbt2Ctx = thiz;
    if(g_dvbt2Ctx->dvbt2MallocBuf)
    {
        free(g_dvbt2Ctx->dvbt2MallocBuf);
        g_dvbt2Ctx->dvbt2MallocBuf = NULL;
    }
    
    if(g_dvbt2Ctx->localServList.pServItems)
    {   
    	free(g_dvbt2Ctx->localServList.pServItems);
        g_dvbt2Ctx->localServList.pServItems = NULL;
        g_dvbt2Ctx->localServList.servNum = 0;  
    }

    if(g_dvbt2Ctx->eucStringBuf)
    {
        free(g_dvbt2Ctx->eucStringBuf);
        g_dvbt2Ctx->eucStringBuf = NULL;
        g_dvbt2Ctx->eucStringBufSize = 0;
    }
    free(g_dvbt2Ctx);
}


//*****************************************************************************//
//**************** Functions DVB SI library provide to outside ****************//
//*****************************************************************************//

//* open dvb si library.
__s32 dvbt2_open(void)
{
    if(g_dvbt2Ctx)
        return -1;
        
    g_dvbt2Ctx = (dvbt2_ctx_t*)malloc(sizeof(dvbt2_ctx_t));
    if(g_dvbt2Ctx == NULL)
        return -1;
    memset(g_dvbt2Ctx, 0, sizeof(dvbt2_ctx_t));
	
    g_dvbt2Ctx->dvbt2MallocBuf = (__u8 *) malloc(sizeof(__u8) *2* DVBT2_SECTION_BUF_SIZE);
    if(g_dvbt2Ctx->dvbt2SectionBuf == NULL)
    {
        MESSAGE("malloc memory for si section buffer fail.\n");
        return -1;
    }
	
    g_dvbt2Ctx->localServList.pServItems = (maple_serv_item_t *) malloc(sizeof(maple_serv_item_t)*MAPLE_MAX_SERV_NUM);
    if (g_dvbt2Ctx->localServList.pServItems == NULL)
    {
        MESSAGE("malloc memory for service list fail.\n");
        free(g_dvbt2Ctx->dvbt2MallocBuf);
        g_dvbt2Ctx->dvbt2MallocBuf = NULL;
        free(g_dvbt2Ctx);
        g_dvbt2Ctx = NULL;
        return -1;
    }
    memset(g_dvbt2Ctx->localServList.pServItems, 0, sizeof(maple_serv_item_t)*MAPLE_MAX_SERV_NUM);
    g_dvbt2Ctx->localServList.servNum = 0;
    g_dvbt2Ctx->charCodeType          = EPDK_CHARSET_ENM_ISO_8859_1;    //* default character coding method is ISO8859_1
 	 g_dvbt2Ctx->dvbt2SectionBuf[0]     = (__u8 *)(g_dvbt2Ctx->dvbt2MallocBuf);
    g_dvbt2Ctx->dvbt2SectionBuf[1]     = (__u8 *)(g_dvbt2Ctx->dvbt2MallocBuf + DVBT2_SECTION_BUF_SIZE);

    return 0;
}

//* close dvb si library. 
__s32 dvbt2_close(void)
{
    if(g_dvbt2Ctx)
    {
       // dvbt2_stop_sepg_search();
      //  dvbt2_stop_schedule_search();
        dvbt2_stop_service_search();
        dvbt2_close_time_monitor();
        dvbt2_close_epg_monitor();
    
        if(g_dvbt2Ctx->dvbt2MallocBuf)
        {
            free(g_dvbt2Ctx->dvbt2MallocBuf);
            g_dvbt2Ctx->dvbt2MallocBuf = NULL;
        }
	    
        if(g_dvbt2Ctx->localServList.pServItems)
        {   
        	free(g_dvbt2Ctx->localServList.pServItems);
            g_dvbt2Ctx->localServList.pServItems = NULL;
            g_dvbt2Ctx->localServList.servNum    = 0;  
        }
        
        free(g_dvbt2Ctx);
        g_dvbt2Ctx = NULL;
    }
    
    return 0;
}

//* function for service list managing.
//* open service list from file.
__s32 dvbt2_open_list(__u8* listFilePath)
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
            readBytes = fread(g_dvbt2Ctx->localServList.pServItems, 1, sizeof(maple_serv_item_t)*servNum, fpListFile);
            if (readBytes != (sizeof(maple_serv_item_t)*servNum))
            {
                fclose(fpListFile);
                return -1;
            }
			
            g_dvbt2Ctx->localServList.servNum = servNum;
        }
		
        fclose(fpListFile);
    }
    
    return 0;
}

//* save list to file.
__s32 dvbt2_save_list(__u8* listFilePath)
{
    __u32 servNum;
    ES_FILE* fpList;
    
    servNum = g_dvbt2Ctx->localServList.servNum;
    fpList = fopen((char*)listFilePath, "wb");
    if (!fpList)
    {
        MESSAGE("cannot open file %s for service list saving.\n", listFilePath);
        return -1;
    }
    
    fwrite(&servNum, 1, sizeof(__u32), fpList);
    fwrite(g_dvbt2Ctx->localServList.pServItems, 1, sizeof(maple_serv_item_t)*servNum, fpList);
    fclose(fpList);
    return 0;
}

//* delete service list.
__s32 dvbt2_delete_list(void)
{   
    g_dvbt2Ctx->localServList.servNum = 0;
    return 0;
}

/*******************************************************************************
Function name: dvbt2_sort_list
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
__s32 dvbt2_sort_list(__s32 nSortMode)
{
    if(nSortMode == SORTMODE_REMOTE_CONTROL_KEY_ID)
    {
        if(EPDK_OK!=QuickSort_NonRecursive_mapleServItem(g_dvbt2Ctx->localServList.pServItems, g_dvbt2Ctx->localServList.servNum, DataCompareFunc_mapleServItem))
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
__s32 dvbt2_add_serv_item(maple_serv_item_t* item, __u32 check)
{
    __u32               i;
    __u32               orgTsId;
    __u32               orgServId;
    __u32               frequency;
    maple_serv_list_t*  list;
    
    list = &g_dvbt2Ctx->localServList;
    
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
    list->pServItems[i].channelId   = dvbt2_generate_channel_id();
	WARNING("channelId = %d\n",list->pServItems[i].channelId);
    list->pServItems[i].isFavourate = 0;
    list->pServItems[i].isLocked    = 0;
    list->pServItems[i].key         = 0;
    list->servNum++;
    return 0;
}

/*******************************************************************************
Function name: dvb2_update_serv_item
Description: 
    1.更新已经存在的项
Parameters: 
    nAddFlag: 如果要更新的service不在列表中,那么是否增加. 1:增加; 0:不增加,直接返回
Return: 
    EPDK_OK:更新成功或增加成功
    EPDK_FAIL:更新失败
Time: 2012/6/19
*******************************************************************************/
__s32 dvbt2_update_serv_item(maple_serv_item_t* item, __s32 nAddFlag)
{
    __s32   i;
    __s32   ret;
    __u32               orgTsId;
    __u32               orgServId;
    __u32               frequency;
    maple_serv_list_t*  list;
    
    list = &g_dvbt2Ctx->localServList;
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
__s32 dvbt2_set_service_item_favourate_flag(__u32 channelId, __u32 isFavourate)
{
    __u32               i;
    maple_serv_list_t*  list;
    
    list = &g_dvbt2Ctx->localServList;
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
__s32 dvbt2_set_service_item_lock(__u32 channelId, __s32 key)
{
    __u32               i;
    maple_serv_list_t*  list;
    
    list = &g_dvbt2Ctx->localServList;
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


//* add list to dvb2 si lib.
__s32 dvbt2_add_list(maple_serv_list_t* newList)
{
    __s32 i;
    
    for(i=0; i<newList->servNum; i++)
    {
    	WARNING("dvbt2_add_serv_item start \n");
        dvbt2_add_serv_item(&newList->pServItems[i], 1);
    }
    
    return 0;
}


//* delete one service item
__s32 dvbt2_delete_serv_item(__u32 channelId)
{
    __u32               i;
    __u32               tmpServNum;
    maple_serv_list_t*  list;
    maple_serv_item_t*  pItems;
    
    list = &g_dvbt2Ctx->localServList;
    
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
    	 WARNING("dvbt2_delete_serv_item start \n");
        dvbt2_add_serv_item(&pItems[i], 0);
    }
    
    return 0;
}

//* copy list from si lib.
__s32 dvbt2_get_list(maple_serv_list_t* list, __u32 type)
{
    __u32               i;
    maple_serv_list_t*  srcList;
    maple_serv_item_t*  srcItem;
    maple_serv_item_t*  dstItem;
    
    list->servNum = 0;
    srcList = &g_dvbt2Ctx->localServList;
    
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


//* copy one service item from dvb2 si lib.
__s32 dvbt2_get_service_item(maple_serv_item_t* item, __u32 channelId)
{
    __u32 i;
    
    for(i=0; i<g_dvbt2Ctx->localServList.servNum; i++)
    {
        if(g_dvbt2Ctx->localServList.pServItems[i].channelId == channelId)
            break;
    }
    
    if(i < g_dvbt2Ctx->localServList.servNum)
    {
        memcpy(item, &g_dvbt2Ctx->localServList.pServItems[i], sizeof(maple_serv_item_t));
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

__s32 dvbt2_search_service(maple_demod_info_t* tp, maple_serv_list_t* servList)
{
    if(g_dvbt2Ctx->slSearchTaskId != 0 && g_dvbt2Ctx->slSearchStopFlag == 0)
    {
        //* service scan task is running.
        WARNING("dvbt2_search_service--in-1->\n");
        return -1;
    }
    if(g_dvbt2Ctx->schdlSearchTaskId != 0 && g_dvbt2Ctx->schdlSearchStopFlag == 0)
    {
        //* cannot search service when scaning schedule.
        WARNING("dvbt2_search_service--in-2->\n");    
        return -1;
    }
    if(g_dvbt2Ctx->sepgSearchTaskId != 0 && g_dvbt2Ctx->sepgSearchStopFlag == 0)
    {
        //* cannot search service when scaning simple epg.
        WARNING("dvbt2_search_service--in-3->\n");     
        return -1;
    }
    if(g_dvbt2Ctx->timeMonitorTaskId != 0 && g_dvbt2Ctx->timeMonitorStopFlag == 0)
    {
        //* cannot search service when scaning time.
        WARNING("dvbt2_search_service--in-4->\n");     
        return -1;
    }
    if(g_dvbt2Ctx->epgMonitorTaskId != 0 && g_dvbt2Ctx->epgMonitorStopFlag == 0)
    {
        //* cannot search service when scaning epg.
        WARNING("dvbt2_search_service--in-5->\n");    
        return -1;
    }
WARNING("dvbt2_search_service--in-6->\n");     
    g_dvbt2Ctx->slSearchStopFlag  = 0;
    g_dvbt2Ctx->frequency         = tp->frequency;
    g_dvbt2Ctx->bandwidth         = tp->bandwidth;
    g_dvbt2Ctx->slSearchTaskId    = dvbt2_create_task(dvbt2_scan_task, servList, KRNL_priolevel4);
    if(g_dvbt2Ctx->slSearchTaskId == 0)
    {
        return -1;
    }
    WARNING("dvbt2_search_service--in-7->\n");     
    return 0;
}

/*******************************************************************************
Function name: dvbt2_search_service_by_serviceId
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
__s32 dvbt2_search_service_by_serviceId(maple_serv_item_t* pServItem)
{
    __s32   ret;
    ret = dvbt2_scan_by_serviceId(pServItem);
    if(EPDK_OK == ret)
    {
        ret = dvbt2_update_serv_item(pServItem, 0);
    }
    return ret;    
}
//* stop service search
__s32 dvbt2_stop_service_search(void)
{   
    if(g_dvbt2Ctx->slSearchStopFlag == 1 || g_dvbt2Ctx->slSearchTaskId == 0)
        return 0;
    
    dvbt2_delete_task(g_dvbt2Ctx->slSearchTaskId);
    g_dvbt2Ctx->slSearchTaskId = 0;
    return 0;
}

//*********************************************************************************************************//
//************************** function for simple EPG search.***********************************************//
//************************** start simple EPG search        ***********************************************//
//************************** task function for searching simple EPG.***************************************//
//*********************************************************************************************************//
#if 0
__s32 dvbt2_search_sepg(__s32 servIndex, maple_sepg_t* pSEpg)
{
    if(g_dvbt2Ctx->slSearchTaskId != 0 && g_dvbt2Ctx->slSearchStopFlag == 0)
    {
        //* service scan task is running.
        return -1;
    }
    
    if(g_dvbt2Ctx->schdlSearchTaskId != 0 && g_dvbt2Ctx->schdlSearchStopFlag == 0)
    {
        //* cannot search service when scaning schedule.
        return -1;
    }
    
    if(g_dvbt2Ctx->sepgSearchTaskId != 0 && g_dvbt2Ctx->sepgSearchStopFlag == 0)
    {
        //* cannot search service when scaning simple epg.
        return -1;
    }
    
    if(g_dvbt2Ctx->epgMonitorTaskId != 0 && g_dvbt2Ctx->epgMonitorStopFlag == 0)
    {
        //* cannot search service when scaning epg.
        return -1;
    }
    
    if(servIndex >= g_dvbt2Ctx->localServList.servNum)
        return -1;
  
    g_dvbt2Ctx->serviceIdxSepg     = servIndex;
    g_dvbt2Ctx->sepgSearchStopFlag = 0;
    g_dvbt2Ctx->sepgSearchTaskId   = dvb_create_task(dvb_sepg_scan_task, pSEpg, KRNL_priolevel4);
    if(g_dvbt2Ctx->sepgSearchTaskId == 0)
    {
        return -1;
    }
    
    return 0;
}

//* stop simple EPG search
__s32 dvbt2_stop_sepg_search(void)
{
    if(g_dvbt2Ctx->sepgSearchStopFlag == 1 || g_dvbt2Ctx->sepgSearchTaskId == 0)
        return 0;
    
    dvb_delete_task(g_dvbt2Ctx->sepgSearchTaskId);
    g_dvbt2Ctx->sepgSearchTaskId = 0;
    return 0;
}
#endif


//***************************************************************************************************//
//********************************** function for schedule EPG search *******************************//
//*********************************** start schedule search *****************************************//
//***************************************************************************************************//
#if 0
__s32 dvbt2_search_schedule(__s32 servIndex, maple_schdl_list_t* pSchedule)
{   
    if(g_dvbt2Ctx->slSearchTaskId != 0 && g_dvbt2Ctx->slSearchStopFlag == 0)
    {
        //* service scan task is running.
        return -1;
    }
    
    if(g_dvbt2Ctx->schdlSearchTaskId != 0 && g_dvbt2Ctx->schdlSearchStopFlag == 0)
    {
        //* cannot search service when scaning schedule.
        return -1;
    }
    
    if(g_dvbt2Ctx->sepgSearchTaskId != 0 && g_dvbt2Ctx->sepgSearchStopFlag == 0)
    {
        //* cannot search service when scaning simple epg.
        return -1;
    }
    
    if(g_dvbt2Ctx->epgMonitorTaskId != 0 && g_dvbt2Ctx->epgMonitorStopFlag == 0)
    {
        //* cannot search service when scaning epg.
        return -1;
    }
    
    if(servIndex >= g_dvbt2Ctx->localServList.servNum)
        return -1;
    
    g_dvbt2Ctx->serviceIdxSchdl       = servIndex;
    g_dvbt2Ctx->schdlSearchStopFlag   = 0;
    g_dvbt2Ctx->schdlSearchTaskId = dvb_create_task(dvb_schedule_scan_task, pSchedule, KRNL_priolevel4);
    if(g_dvbt2Ctx->schdlSearchTaskId == 0)
    {
        return -1;
    }
    
    return 0;
}

//* stop schedule search
__s32 dvb_stop_schedule_search(void)
{
    if(g_dvbt2Ctx->schdlSearchStopFlag == 1 || g_dvbt2Ctx->schdlSearchTaskId == 0)
        return 0;
    
    dvb_delete_task(g_dvbt2Ctx->schdlSearchTaskId);
    g_dvbt2Ctx->schdlSearchTaskId = 0;
    return 0;
}
#endif

//***************************************************************************************************//
//***************************** function for tv stream time monitor *********************************//
//******************************* start time monitor                *********************************//

__s32 dvbt2_open_time_monitor(void)
{
    if(g_dvbt2Ctx->slSearchTaskId != 0 && g_dvbt2Ctx->slSearchStopFlag == 0)
    {
        //* service scan task is running.
        return -1;
    }
    
    if(g_dvbt2Ctx->timeMonitorTaskId != 0 && g_dvbt2Ctx->timeMonitorStopFlag == 0)
    {
        return -1;
    }
    
    g_dvbt2Ctx->timeMonitorStopFlag = 0;
    g_dvbt2Ctx->timeMonitorTaskId = dvbt2_create_task(dvbt2_time_monitor_task, NULL, KRNL_priolevel4);
    if(g_dvbt2Ctx->timeMonitorTaskId == 0)
    {
        return -1;
    }
    
    return 0;
}

//* stop time monitor
__s32 dvbt2_close_time_monitor(void)
{   
    if(g_dvbt2Ctx->timeMonitorStopFlag == 1 || g_dvbt2Ctx->timeMonitorTaskId == 0)
        return 0;
    
    dvbt2_delete_task(g_dvbt2Ctx->timeMonitorTaskId);
    g_dvbt2Ctx->timeMonitorTaskId = 0;
    return 0;
}

//* get current stream time
__s32 dvbt2_get_time(maple_time_t* curTime)
{  
    #if 0
    curTime->year    = g_dvbt2Ctx->curStreamTime.year;
    curTime->month   = g_dvbt2Ctx->curStreamTime.month;
    curTime->day     = g_dvbt2Ctx->curStreamTime.day;
    curTime->weekDay = g_dvbt2Ctx->curStreamTime.weekDay;
    curTime->hour    = g_dvbt2Ctx->curStreamTime.hour;
	curTime->min     = g_dvbt2Ctx->curStreamTime.min; 
    curTime->sec     = g_dvbt2Ctx->curStreamTime.sec;
    curTime->MJD     = g_dvbt2Ctx->curStreamTime.MJD;
    #else
    if (!curTime)
    {
        return -1;
    }
    memcpy(curTime, &g_dvbt2Ctx->curStreamTime, sizeof(g_dvbt2Ctx->curStreamTime));
    #endif
    return 0;
}


//***************************************************************************************************//
//************************************** function for EPG monitor ***********************************//
//***************************************************************************************************//

__s32 dvbt2_open_epg_monitor(void)
{   
    __u8 err;
    
    if(g_dvbt2Ctx->slSearchTaskId != 0 && g_dvbt2Ctx->slSearchStopFlag == 0)
    {
        //* service scan task is running.
        return -1;
    }
    
    if(g_dvbt2Ctx->schdlSearchTaskId != 0 && g_dvbt2Ctx->schdlSearchStopFlag == 0)
    {
        //* cannot search service when scaning schedule.
        return -1;
    }
    
    if(g_dvbt2Ctx->sepgSearchTaskId != 0 && g_dvbt2Ctx->sepgSearchStopFlag == 0)
    {
        //* cannot search service when scaning simple epg.
        return -1;
    }
    
    if(g_dvbt2Ctx->epgMonitorTaskId != 0 && g_dvbt2Ctx->epgMonitorStopFlag == 0)
    {
        //* cannot search service when scaning simple epg.
        return -1;
    }
    
    if(g_dvbt2Ctx->epgMutex != NULL)
    {
        return -1;
    }
    
    g_dvbt2Ctx->epgMutex = esKRNL_SemCreate(1);
    if(g_dvbt2Ctx->epgMutex == NULL)
    {
        return -1;
    }
    
	eit_parser_clean();
	if(eit_pool_init(EIT_POOL_BUFFER_SIZE) != 0)
	{
	    MESSAGE("eit_pool_init fail.\n");
        esKRNL_SemDel(g_dvbt2Ctx->epgMutex, OS_DEL_ALWAYS, &err);
        g_dvbt2Ctx->epgMutex = NULL;
        return -1;
	}
    
    g_dvbt2Ctx->epgMonitorStopFlag = 0;
    g_dvbt2Ctx->epgMonitorTaskId = dvbt2_create_task(dvbt2_epg_monitor_task, NULL, KRNL_priolevel4);
    if(g_dvbt2Ctx->epgMonitorTaskId == 0)
    {
        eit_pool_release();
        esKRNL_SemDel(g_dvbt2Ctx->epgMutex, OS_DEL_ALWAYS, &err);
        g_dvbt2Ctx->epgMutex = NULL;
        return -1;
    }
    
    return 0;
}

//* stop schedule search
__s32 dvbt2_close_epg_monitor(void)
{
    __u8 err;
    
    if(g_dvbt2Ctx->epgMonitorStopFlag == 1 || g_dvbt2Ctx->epgMonitorTaskId == 0)
    {
        if(g_dvbt2Ctx->epgMutex)
        {
            esKRNL_SemDel(g_dvbt2Ctx->epgMutex, OS_DEL_ALWAYS, &err);
            g_dvbt2Ctx->epgMutex = NULL;
        }
        return 0;
    }
    
    dvbt2_delete_task(g_dvbt2Ctx->epgMonitorTaskId);
    g_dvbt2Ctx->epgMonitorTaskId = 0;
    
    if(g_dvbt2Ctx->epgMutex)
    {
        esKRNL_SemDel(g_dvbt2Ctx->epgMutex, OS_DEL_ALWAYS, &err);
        g_dvbt2Ctx->epgMutex = NULL;
    }
    
	eit_parser_clean();
	eit_pool_release();
    
    return 0;
}

//* get simple epg of specified service
__s32 dvbt2_get_sepg(__u32 channelId, maple_sepg_t* pSEpg)
{
    __u32               i;
    maple_serv_list_t*  list;
    __u32               tsId;
    __u32               servId;
    __u32               origNetworkId;
    
    list = &g_dvbt2Ctx->localServList;
    
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
    
    if(g_dvbt2Ctx->epgMonitorTaskId != 0 && g_dvbt2Ctx->epgMonitorStopFlag == 0)
    {
        //* lock epg table mutex.
        if(dvbt2_get_mutex(g_dvbt2Ctx->epgMutex) != 0)
            return -1;
            
        origNetworkId = list->pServItems[i].networkId;
        tsId          = list->pServItems[i].tsId;
        servId        = list->pServItems[i].servId;
    
        eit_pool_parse_content(0, origNetworkId, tsId, servId, epg_type_pf);
    	eit_parser_get_pf(0, tsId, origNetworkId, servId, pSEpg);
        
        //* unlock epg table mutex.
        dvbt2_release_mutex(g_dvbt2Ctx->epgMutex);
    }
    
    return 0;
}

//* get schedule of specified service
__s32 dvbt2_get_schedule(__u32 channelId, maple_schdl_list_t* pSchedule)
{
    __u32               i;
    maple_serv_list_t*  list;
    __u32               tsId;
    __u32               servId;
    __u32               origNetworkId;
    
    list = &g_dvbt2Ctx->localServList;
    
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
    
    if(g_dvbt2Ctx->epgMonitorTaskId != 0 && g_dvbt2Ctx->epgMonitorStopFlag == 0)
    {
        //* lock epg table mutex.
        if(dvbt2_get_mutex(g_dvbt2Ctx->epgMutex) != 0)
            return -1;
            
        origNetworkId = list->pServItems[i].networkId;
        tsId          = list->pServItems[i].tsId;
        servId        = list->pServItems[i].servId;
        
        eit_pool_parse_content(0, origNetworkId, tsId, servId, epg_type_sch);
    	eit_parser_get_schedule(0, tsId, origNetworkId, servId, pSchedule, MAPLE_MAX_SCHDL_NUM_PER_SERV);
            
        //* unlock epg table mutex.
        dvbt2_release_mutex(g_dvbt2Ctx->epgMutex);
    }
    
    return 0;
}


//* set current playing service
__s32 dvbt2_set_current_service(__u32 channelId)
{
    __u32 i, j;
    __u32 frequency;
    active_service_t service[EIT_POOL_MAX_ACTIVE_SERVICE_NUM];
    
    for(i=0; i<g_dvbt2Ctx->localServList.servNum; i++)
    {
        if(g_dvbt2Ctx->localServList.pServItems[i].channelId == channelId)
            break;
    }
    
    if(i == g_dvbt2Ctx->localServList.servNum)
        return -1;
        
    g_dvbt2Ctx->localServList.curServIdx = i;
    if(g_dvbt2Ctx->epgMonitorTaskId != 0 && g_dvbt2Ctx->epgMonitorStopFlag == 0)
    {
        //* lock epg table mutex.
        if(dvbt2_get_mutex(g_dvbt2Ctx->epgMutex) != 0)
            return -1;
            
        frequency = g_dvbt2Ctx->localServList.pServItems[i].demod_pm.frequency;
        
        for(i=0, j=0; i<g_dvbt2Ctx->localServList.servNum; i++)
        {
            if(g_dvbt2Ctx->localServList.pServItems[i].servType == MAPLE_SERV_TYPE_TV)
            {
                if(g_dvbt2Ctx->localServList.pServItems[i].demod_pm.frequency == frequency)
                {
                    service[j].tpId          = 0;
                    service[j].origNetworkId = g_dvbt2Ctx->localServList.pServItems[i].networkId;
                    service[j].tsId          = g_dvbt2Ctx->localServList.pServItems[i].tsId;
                    service[j].serviceId     = g_dvbt2Ctx->localServList.pServItems[i].servId;
                    j++;
                }
            }
        }

        eit_pool_set_active_services(service, j);
        
        //* unlock epg table mutex.
        dvbt2_release_mutex(g_dvbt2Ctx->epgMutex);
    }
    return 0;
}


//* get service number of service list
__s32 dvbt2_get_service_num(void)
{
    if(g_dvbt2Ctx)
        return g_dvbt2Ctx->localServList.servNum;
    else
        return 0;
}

//* get current service id
__s32 dvbt2_get_cur_service_id(void)
{
    if(g_dvbt2Ctx && g_dvbt2Ctx->localServList.pServItems)
    {
        if(g_dvbt2Ctx->localServList.curServIdx >= 0 && g_dvbt2Ctx->localServList.curServIdx < g_dvbt2Ctx->localServList.servNum)
            return g_dvbt2Ctx->localServList.pServItems[g_dvbt2Ctx->localServList.curServIdx].channelId;
    }
    
    return -1;
}