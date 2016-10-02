
#include "tsDemux_s.h"
//#include "tsDemux.h"

static __s32 dataArriveCallBack(void* param)
{
    __krnl_event_t* sem;
    OS_SEM_DATA semData;

    sem = param;

    esKRNL_SemQuery(sem, &semData);
    if (semData.OSCnt == 0)
        esKRNL_SemPost(sem);
        
    return 0;
}


static __s32 WaitData(__krnl_event_t* sem)
{
    __u8 err;
    __u16 waitTime = 10;
    esKRNL_SemPend(sem, waitTime, &err);
    return err;
}

/*******************************************************************************
Function name: push_avpes_data
Description: 
    1.一般用于接收音视频数据的pes包.
    2.len:可以送多少数据
Parameters: 
    
Return: 
    
Time: 2011/2/1
*******************************************************************************/
static __s32 push_avpes_data(__u8* data, __u32 len, __u32 new_frm, __pes_filter_t* pes)
{
    __u8*               dataPtr;
    
    dataPtr = data;
    
    if(new_frm) //这里的意思是等到下一帧的到来后,才送出上一帧的数据.对于音视频,没有问题。但对于非常离散的字幕帧,会导致一个字幕帧迟迟送不出去.
    {
        if(pes->valid_size > 0)
        {
            pes->ctrl_bits |= LAST_PART_BIT;
            
            //* send data
            pes->data_info.ctrlBits = pes->ctrl_bits;
            pes->data_info.dataLen  = pes->valid_size;
            esKRNL_CallBack(pes->UpdateDateCB, &pes->data_info);
            
            //* clear status
            pes->free_size  = 0;
            pes->valid_size = 0;
            pes->ctrl_bits  = 0;
        }
    }
    
    while(len > 0)
    {
        if(pes->free_size == 0)
        {
            //* request buffer
            esKRNL_CallBack(pes->RequestDataCB, &pes->md_buf);
            if (pes->md_buf.buf == NULL)
            {
                if (pes->md_buf.bufSize == 1)
                    return 1;
                else
                    return -1;
            }
            
            pes->cur_ptr   = pes->md_buf.buf;
            pes->free_size = pes->md_buf.bufSize;
        }
        
        if(new_frm)
        {
            new_frm = 0;
            pes->ctrl_bits |= FIRST_PART_BIT;
            if(pes->pts != -1)
            {
                pes->ctrl_bits |= PTS_VALID_BIT;
                pes->data_info.pts = pes->pts/90;
            }
            
            if(pes->rap_flag)
            {
                pes->ctrl_bits |= RANDOM_ACCESS_FRAME_BIT;
                pes->rap_flag = 0;
            }
        }
        
        if(pes->free_size > len)
        {
            memcpy(pes->cur_ptr, dataPtr, len);
            //esKSRV_memcpy(pes->cur_ptr, dataPtr, len);
            pes->free_size 	-= len;
            pes->cur_ptr    += len;
            pes->valid_size += len;

            //判断pes包是否已经装完, 如果装完立刻送出.
            if(pes->payload_size > 0 && pes->valid_size >= pes->payload_size)
			{
		        pes->ctrl_bits |= LAST_PART_CTRL;
                
                pes->data_info.ctrlBits = pes->ctrl_bits;
                pes->data_info.dataLen  = pes->valid_size;
                esKRNL_CallBack(pes->UpdateDateCB, &pes->data_info);
		            
		        pes->free_size    = 0;
		        pes->valid_size   = 0;
		        pes->payload_size = 0;
		        pes->ctrl_bits  = FIRST_PART_BIT;
			}
            
            break;
        }
        else
        {
            memcpy(pes->cur_ptr, dataPtr, pes->free_size);
            //esKSRV_memcpy(pes->cur_ptr, dataPtr, pes->free_size);
            len     -= pes->free_size;
            dataPtr += pes->free_size;
            
            pes->valid_size += pes->free_size;
            //* send data
            if(pes->payload_size > 0 && pes->valid_size >= pes->payload_size)
			{
                WARNING("very lucky! validsize[0x%x] >= left payload_size[0x%x].\n", pes->valid_size, pes->payload_size);
		        pes->ctrl_bits |= LAST_PART_CTRL;
			}
            pes->data_info.ctrlBits = pes->ctrl_bits;
            pes->data_info.dataLen  = pes->valid_size;
            esKRNL_CallBack(pes->UpdateDateCB, &pes->data_info);
            if(pes->payload_size != 0)
            {
                if(pes->valid_size > pes->payload_size)
                {
                    WARNING("fatal error! validsize[0x%x] > left payload_size[0x%x], check code!\n", pes->valid_size, pes->payload_size);
                    pes->payload_size = 0;;
                }
                else
                {
                    pes->payload_size   -= pes->valid_size;
                }
            }
            pes->free_size		= 0;
            pes->valid_size 	= 0;
            pes->ctrl_bits 	= 0;
        }
    }
    
    return 0;
}
/*******************************************************************************
Function name: push_subpes_data
Description: 
    1.一般用于接收字幕数据的pes包.
    2.len:可以送多少数据
Parameters: 
    
Return: 
    
Time: 2011/2/1
*******************************************************************************/
static __s32 push_subpes_data(__u8* data, __u32 len, __u32 new_frm, __pes_filter_t* pes)
{
    __u8*               dataPtr;
    
    dataPtr = data;
    
    if(new_frm) //这里的意思是等到下一帧的到来后,才送出上一帧的数据.对于音视频,没有问题。但对于非常离散的字幕帧,会导致一个字幕帧迟迟送不出去.
    {
        if(pes->valid_size > 0)
        {
            pes->ctrl_bits |= LAST_PART_BIT;
            
            //* send data
            pes->data_info.ctrlBits = pes->ctrl_bits;
            pes->data_info.dataLen  = pes->valid_size;
            esKRNL_CallBack(pes->UpdateDateCB, &pes->data_info);
            
            //* clear status
            pes->free_size  = 0;
            pes->valid_size = 0;
            pes->ctrl_bits  = 0;
        }
    }
    
    while(len > 0)
    {
        if(pes->free_size == 0)
        {
            //* request buffer
            esKRNL_CallBack(pes->RequestDataCB, &pes->md_buf);
            if (pes->md_buf.buf == NULL)
            {
                if (pes->md_buf.bufSize == 1)
                    return 1;
                else
                    return -1;
            }
            
            pes->cur_ptr   = pes->md_buf.buf;
            pes->free_size = pes->md_buf.bufSize;
        }
        if(new_frm)
        {
            new_frm = 0;
            pes->ctrl_bits |= FIRST_PART_BIT;
            if(pes->pts != -1)
            {	
                pes->ctrl_bits |= PTS_VALID_BIT;
                pes->data_info.pts = pes->pts/90;
            }
					
            if(pes->rap_flag)
            {
                pes->ctrl_bits |= RANDOM_ACCESS_FRAME_BIT;
                pes->rap_flag = 0;
            }
        }
        
        if(pes->free_size > len)
        {
            memcpy(pes->cur_ptr, dataPtr, len);
            //esKSRV_memcpy(pes->cur_ptr, dataPtr, len);
            pes->free_size 	-= len;
            pes->cur_ptr    += len;
            pes->valid_size += len;

            //判断pes包是否已经装完, 如果装完立刻送出.
            if(pes->payload_size > 0 && pes->valid_size >= pes->payload_size)
						{
				        pes->ctrl_bits |= LAST_PART_CTRL;
                pes->data_info.ctrlBits = pes->ctrl_bits;
                pes->data_info.dataLen  = pes->valid_size;
                esKRNL_CallBack(pes->UpdateDateCB, &pes->data_info);
				            
				        pes->free_size    = 0;
				        pes->valid_size   = 0;
				        pes->payload_size = 0;
				        pes->ctrl_bits  = FIRST_PART_BIT;
						}
            
            break;
        }
        else
        {
            memcpy(pes->cur_ptr, dataPtr, pes->free_size);
            //esKSRV_memcpy(pes->cur_ptr, dataPtr, pes->free_size);
            len     -= pes->free_size;
            dataPtr += pes->free_size;
            
            pes->valid_size += pes->free_size;
            //* send data
            if(pes->payload_size > 0 && pes->valid_size >= pes->payload_size)
						{
			            WARNING("very lucky! validsize[0x%x] >= left payload_size[0x%x].\n", pes->valid_size, pes->payload_size);
					        pes->ctrl_bits |= LAST_PART_CTRL;
						}
            pes->data_info.ctrlBits = pes->ctrl_bits;
            pes->data_info.dataLen  = pes->valid_size;
            esKRNL_CallBack(pes->UpdateDateCB, &pes->data_info);
            if(pes->payload_size != 0)
            {
                if(pes->valid_size > pes->payload_size)
                {
                    WARNING("fatal error! validsize[0x%x] > left payload_size[0x%x], check code!\n", pes->valid_size, pes->payload_size);
                    pes->payload_size = 0;;
                }
                else
                {
                    pes->payload_size   -= pes->valid_size;
                }
            }
            pes->free_size		= 0;
            pes->valid_size 	= 0;
            pes->ctrl_bits 	= 0;
        }
    }
    
    return 0;
}
/*******************************************************************************
Function name: push_section_data
Description: 
    1.一般用于接收si表的section.
    2.这个函数会处理si section的完整性.
    3. len表示送入的数据的长度，这笔数据不一定都属一个section.所以可能只处理一部分.
    4.有可能一个packet会包含2个section的开始部分,所以一定要处理这种情况.
    5.默认送进来的数据必须全部处理完才能退出.
    6.考虑弱信号下的容错问题.
Parameters: 
    len:如果送的是section的开始处,一般就把剩下的ts包的长度全送进来. 如果section的开始处之前还有一段数据,就是那段数据的长度.
        一般情况下 >= 本ts包包含的section的长度
    new_section:表示这次送进来的数据是否是si section的开始
Return: 
    -1:失败
Time: 2011/2/1
*******************************************************************************/
static __s32 push_section_data(__u8* data, __u32 len, __u32 new_section, __section_filter_t* tss)
{
    __u8*               dataPtr;
    __u32   nCurLen = len;
    __u32   nCopyLen;   //需要从ts包拷贝的字节长度
    __u32   nNewSectionFlag = new_section;
    __s32   nSectionLeftLen = 0;    //记录当前一个section还剩多少字节没有被拷贝
    dataPtr = data;
    
    if(nNewSectionFlag)
    {   
        tss->free_size  = 0;
        tss->cur_ptr    = NULL;
        if(tss->section_h_size  > 0)
        {
            if(tss->end_of_section_reached != 0)         // find new section, but the last section has not closed, discard the parsed datas
            {   
                WARNING("last section is not received full! But new section detect!\n");
                esKRNL_CallBack(tss->UpdateDateCB, &tss->section_h_size);
                tss->free_size = 0;
            }
            else
            {
                tss->cur_ptr   = tss->md_buf.buf;
                tss->free_size = tss->md_buf.bufSize;
            }
        }
        
        tss->end_of_section_reached = 0;
        tss->section_index  = 0;
        tss->section_h_size = (AV_RB16(dataPtr + 1) & 0xfff) + 3;
        if(tss->section_h_size > MAX_SECTION_SIZE)
        {   
            tss->section_h_size = MAX_SECTION_SIZE;
            WARNING("the size of the section_h_size is error,section_h_size=%d\n",tss->section_h_size);
        }
        if(tss->section_h_size <= 3)
        {   
            //tss->section_h_size = MAX_SECTION_SIZE;
            WARNING("[2]the size of the section_h_size is error,section_h_size=%d\n",tss->section_h_size);
            return 0;
        }
        if(nCurLen > tss->section_h_size)   //一个ts packet可能包含2个section的header,这里是为了避免把第二个section head也包含了.
        {
            nCopyLen = tss->section_h_size;
        }
        else
        {
            nCopyLen = nCurLen;
        }
    }
    else    //如果不是一个新section的开始,而tss->section_h_size ==0,就意味着这个section收不完整了,或者说前面的一部分没有收,那么直接丢掉吧。
    {
        if(0 == tss->section_h_size)
        {
            WARNING("section has not receive header, so discard!, tss->need_start[%d],pid[0x%x]\n", tss->need_start, tss->pid);
            return 0;
        }
        nSectionLeftLen = tss->section_h_size - tss->section_index;
        if(nSectionLeftLen > 0)
        {
            if((__u32)nSectionLeftLen > nCurLen)
            {
                nCopyLen = nCurLen;
            }
            else
            {
                nCopyLen = (__u32)nSectionLeftLen;
            }
        }
        else
        {
            WARNING("fatal error, check code! why section is read over? nNewSectionFlag[%d], nCurLen[%d], section_h_size[%d], section_index[%d]\n",
                nNewSectionFlag, nCurLen, tss->section_h_size, tss->section_index);
            nCopyLen = nCurLen;
        }
    }
    
    while(nCurLen > 0)
    {   
        if(tss->free_size == 0)
        {
            //* request buffer
            esKRNL_CallBack(tss->RequestDataCB, &tss->md_buf);
            if (tss->md_buf.buf == NULL) 
            {
                return -1;
            }
            
            tss->cur_ptr   = tss->md_buf.buf;
            tss->free_size = tss->md_buf.bufSize;         
        }

        //if(tss->free_size < nCurLen)    //这里如果用section_h_size更精确,但是考虑到弱信号下可能出错,所以用len更保险.但是这样可能会丢section,
        if(tss->free_size < nCopyLen)   //一般来说不可能.所以如果出现该情况,就砍掉部分数据,当做完整的section处理了.
        {
            __wrn(" get the buffer size is too small, len=%d,tss->free_size=%d.\n", nCopyLen, tss->free_size);
            memcpy(tss->cur_ptr,dataPtr,tss->free_size);
            //esKSRV_memcpy(tss->cur_ptr,dataPtr,tss->free_size);
            tss->cur_ptr += tss->free_size;
            dataPtr += tss->free_size;
            tss->section_index += tss->free_size;
            nCopyLen = 0;
            nCurLen = 0;
            tss->free_size = 0;
            tss->end_of_section_reached = 1;
            esKRNL_CallBack(tss->UpdateDateCB, &tss->section_h_size);
            tss->free_size = 0;
            tss->section_h_size = 0;
        }
        else
        {
            memcpy(tss->cur_ptr,dataPtr,nCopyLen);
            //esKSRV_memcpy(tss->cur_ptr,dataPtr,nCopyLen);
            tss->free_size -= nCopyLen;
            tss->cur_ptr += nCopyLen;
            dataPtr += nCopyLen;
            tss->section_index += nCopyLen;
            nCurLen -= nCopyLen;
        }

        if ((tss->section_h_size!=0) && (tss->section_index>=tss->section_h_size)) 
        {
            tss->end_of_section_reached = 1;
            esKRNL_CallBack(tss->UpdateDateCB, &tss->section_h_size);   //这里可能有bug,如果这个ts包还包含了另一个section的开头,那么另一个section就被skip了。EIT表的解析可能会遇到这种情况.
            tss->free_size = 0;
            tss->section_h_size = 0;
        }
        
        if(nCurLen > 0) //说明还有剩下的部分,可能是新的section,也可能是冗余数据. 先当做new section处理.
        {
            tss->free_size  = 0;
            tss->cur_ptr    = NULL;
            if(tss->section_h_size  > 0)
            {
                WARNING("fatal error! section_h_size[%d], check code!\n", tss->section_h_size);
                return 0;
//                if(tss->end_of_section_reached != 0)         // find new section, but the last section has not closed, discard the parsed datas
//                {   
                    //WARNING("last section is not received full! But new section detect!\n");
//                    esKRNL_CallBack(tss->UpdateDateCB, &tss->section_h_size);
//                    tss->free_size = 0;
//                }
//                else
//                {
//                    tss->cur_ptr   = tss->md_buf.buf;
//                    tss->free_size = tss->md_buf.bufSize;
//                }
            }
            
            tss->end_of_section_reached = 0;
            tss->section_index  = 0;
            tss->section_h_size = (AV_RB16(dataPtr + 1) & 0xfff) + 3;
            if(tss->section_h_size > MAX_SECTION_SIZE)  //即使是弱信号造成的错误也不管,直接skip
            {   
                if(tss->section_h_size != (0xfff + 3))
                {
                    WARNING("[3]the size of the section_h_size is error,section_h_size=[0x%x], nLeftLen[%d]set to [0x%x]\n",tss->section_h_size, nCurLen, MAX_SECTION_SIZE);
                }
                //tss->section_h_size = MAX_SECTION_SIZE;
                tss->section_h_size = 0;
                return 0;
            }
            if(tss->section_h_size <= 3)    //可以直接返回了.
            {   
                WARNING("[4]the size of the section_h_size is error,section_h_size=%d\n",tss->section_h_size);
                //tss->section_h_size = MAX_SECTION_SIZE;
                tss->section_h_size = 0;
                return 0;
            }
            if(nCurLen > tss->section_h_size)   //一个ts packet可能包含2个section的header,这里是为了避免把第二个section head也包含了.
            {
                nCopyLen = tss->section_h_size;
            }
            else
            {
                nCopyLen = nCurLen;
            }
        }
    }

    return 0;
}

/*******************************************************************************
Function name: push_tspacket_data
Description: 
    1. typedef __s32 (*F_PUSH_TS_DATA)(__u8* data, __u32 len, __tsdata_filter_t* tspacket);
    2. 因为目前是为录制准备的.所以主要按照录制的实现方式来设计.
    3. 因为tspacket实际上不做任何处理, 所以dmx对于这种模式是不需要做拼包处理的,所以
        就直接用调用者设置下来的callback函数来处理.dmx只要把数据送过去就行.
        所以RequestDataCB()是不应该有什么用的.这里形式上调用一下.
Parameters: 
    
Return: 
    
Time: 2012/6/13
*******************************************************************************/
static __s32 push_tspacket_data(__u8* data, __u32 len, __tsdata_filter_t* tspacket)
{
    if(len)
    {
        esKRNL_CallBack(tspacket->RequestDataCB, &tspacket->md_buf);
        tspacket->md_buf.buf = data;
        tspacket->md_buf.bufSize = len;
        esKRNL_CallBack(tspacket->UpdateDateCB, &tspacket->md_buf);
    }
    return EPDK_OK;
}

static __s64 __inline get_pts(const __u8 *p)
{
    __s64 pts = (__s64)((p[0] >> 1) & 0x07) << 30;
    pts |= (AV_RB16(p + 1) >> 1) << 15;
    pts |=  AV_RB16(p + 3) >> 1;
    return pts;
}


/*******************************************************************************
Function name: handle_av_pkt_payload
Description: 
    1.处理一个ts包,拼pes包,只处理VIDEO,AUDIO类型的PES包.
Parameters: 
    1.buf:要处理的buf的起始地址,其实就是一个ts包的范围，起始地址在ts包的前部.
    2.buf_size:要处理的buf的长度,一般是一个ts包剩余的长度.
    3.is_start:是否是一个pes包的开始.
Return: 
    
Time: 2012/4/17
*******************************************************************************/
static void handle_av_pkt_payload(__u8* buf, __s32 buf_size, __s32 is_start, __pes_filter_t* pes)
{
    __u8*	p;
    __s32	len;
    __s32	code;
    __u32	first;
	__s32	j;

    first = 0;

    if (is_start) 
    {
        pes->state = MPEGTS_HEADER;
        pes->data_index = 0;
        pes->payload_size = 0;
    }
    p = buf;

	while (buf_size > 0) 
    {
        switch(pes->state) 
        {
        case MPEGTS_HEADER:
            len = PES_START_SIZE - pes->data_index;
            if (len > buf_size)
                len = buf_size;

			for(j=0; j<len; j++)
				pes->header[pes->data_index++] = *p++;
			
            buf_size -= len;

            if (pes->data_index == PES_START_SIZE) 
            {
                /* we got all the PES or section header. We can now decide */
                if (pes->header[0] == 0x00 && pes->header[1] == 0x00 && pes->header[2] == 0x01) 
                {
                    //* it must be an mpeg2 PES stream
                    code = pes->header[3] | 0x100;
						if (!((code >= 0x1c0 && code <= 0x1df) || (code >= 0x1e0 && code <= 0x1ef) || (code == 0x1bd) || (code == 0x1fd)))
                        goto skip;

                    pes->state = MPEGTS_PESHEADER_FILL;
                    pes->total_size = AV_RB16(pes->header + 4);

                    //* NOTE: a zero total size means the PES size is unbounded            
                    if (pes->total_size)
                        pes->total_size += 6;									
                    else
                    {   //对于mpeg2的视频数据,经常total_size = 0.例如FUJI 12Seg_BCAS_1Seg.ts中的高清service.mpeg2视频数据的total_size = 0,但是音频和字幕的pes包还是有长度的.
                        //WARNING("pes total_size = 0!, code[0x%x]\n", code);
                    }

                    pes->pes_header_size = pes->header[8] + 9;
                    
                    if(pes->total_size > 6)
                        pes->payload_size = pes->total_size - pes->pes_header_size;
                } 
                else    //不是pes包的开头,在刚开始接收数据时,一般是接收了pes包的部分,然后才会收到pes包的开头.这时本tspacket就skip掉
                {
                    /* otherwise, it should be a table, skip packet */
skip:
                    pes->state = MPEGTS_SKIP;
                    continue;
                }
            }
            break;

            /**********************************************/
            /* PES packing parsing */
        case MPEGTS_PESHEADER_FILL:
            len = pes->pes_header_size - pes->data_index;
            if (len > buf_size)
                len = buf_size;

			for(j=0; j<len; j++)
				pes->header[pes->data_index++] = *p++;
            buf_size -= len;

            if (pes->data_index == pes->pes_header_size) 
            {
                const __u8 *r;
                __u32       flags;
															
                flags = pes->header[7];								
                r = pes->header + 9;
                pes->pts = (__s64)-1;   //一个新的pes包开始拿pts了,先初始化pts为-1.
                if ((flags & 0xc0) == 0x80) 
                {
                    pes->pts = get_pts(r);						
                    r += 5;
                } 
                else if ((flags & 0xc0) == 0xc0) 
                {
                    pes->pts = get_pts(r);
                    r += 10;
                }

                if((flags & 0x20) == 0x20)  //ESCR_flag
                {
                    r += 6;
                }
                if((flags & 0x10) == 0x10)  //ES_rate_flag
                {
                    r += 3;
                }
                if((flags & 0x08) == 0x08)  //DSM_trick_mode_flag
                {
                    r += 1;
                }
                if((flags & 0x04) == 0x04)  //additional_copy_info_flag
                {
                    r += 1;
                }
                if((flags & 0x02) == 0x02)  //PES_CRC_flag
                {
                    r += 2;
                }
                if((flags & 0x01) == 0x01)  //PES_extension_flag
                {
                    __u32 PES_extension_field_flags = r[0];
                    r+=1;
                    if((PES_extension_field_flags&0x80) == 0x80)    //PES_private_data_flag
                    {                  	
                        memcpy(pes->data_info.privateData, r, PES_PRIVATE_DATA_LEN);
                        //esKSRV_memcpy(pes->data_info.privateData, r, PES_PRIVATE_DATA_LEN);
						            r += PES_PRIVATE_DATA_LEN;
                    }
                    //剩下的暂时不管了.
                }

                /* we got the full header. We parse it and get the payload */
                pes->state = MPEGTS_PAYLOAD;
            }
            break;

        case MPEGTS_PAYLOAD:
	     if (pes->total_size) 
            {
                len = pes->total_size - pes->data_index;
                if (len > buf_size)
                    len = buf_size;
            } 
            else 
            {
                len = buf_size;
            }
           
        
            if (len > 0) 
            {
	             if (pes->pts != (__s64)-1)
	            {
         						if(pes->pts != pes->pre_pts) 
                      first = 1;									 
              }

							pes->push_avpes_data(p, len, first, pes);
							if(pes->pts != (__s64)-1)
									pes->pre_pts = pes->pts;
			  			pes->pts = -1;
							return;
            }
            buf_size = 0;
            break;

        case MPEGTS_SKIP:
            buf_size = 0;
            break;
        }
    }
}
/*******************************************************************************
Function name: handle_sub_pkt_payload
Description: 
    1.处理一个ts包,拼pes包.现在处理ISDBT中 subtitle的PES包(包括带PTS和不带PTS)。
Parameters: 
    1.buf:要处理的buf的起始地址,其实就是一个ts包的范围，起始地址在ts包的前部.
    2.buf_size:要处理的buf的长度,一般是一个ts包剩余的长度.
    3.is_start:是否是一个pes包的开始.
Return: 
    
Time: 2012/4/17
*******************************************************************************/
static void handle_sub_pkt_payload(__u8* buf, __s32 buf_size, __s32 is_start, __pes_filter_t* pes)
{
    __u8*	p;
    __s32	len;
    __s32	code;
    __u32	first;
		__s32	j;

    first = 0;

    if (is_start) 
    {
        pes->state = MPEGTS_HEADER;
        pes->data_index = 0;
        pes->payload_size = 0;
    }
		
    p = buf;

	while (buf_size > 0) 
    {
        switch(pes->state) 
        {
        case MPEGTS_HEADER:
            len = PES_START_SIZE - pes->data_index;
            if (len > buf_size)
                len = buf_size;

						for(j=0; j<len; j++)
							pes->header[pes->data_index++] = *p++;
			
            buf_size -= len;

            if (pes->data_index == PES_START_SIZE) 
            {
                /* we got all the PES or section header. We can now decide */
                if (pes->header[0] == 0x00 && pes->header[1] == 0x00 && pes->header[2] == 0x01) 
                {
                    //* it must be an mpeg2 PES stream
                    code = pes->header[3] | 0x100;
										if (!((code >= 0x1c0 && code <= 0x1df) || (code >= 0x1e0 && code <= 0x1ef) || (code == 0x1bd) || (code == 0x1fd)))
                        goto skip;

                    pes->state = MPEGTS_PESHEADER_FILL;
                    pes->total_size = AV_RB16(pes->header + 4);

                    //* NOTE: a zero total size means the PES size is unbounded            
                    if (pes->total_size)
                        pes->total_size += 6;									
                    else
                    {   //对于mpeg2的视频数据,经常total_size = 0.例如FUJI 12Seg_BCAS_1Seg.ts中的高清service.mpeg2视频数据的total_size = 0,但是音频和字幕的pes包还是有长度的.
                        //WARNING("pes total_size = 0!, code[0x%x]\n", code);
                    }

                    pes->pes_header_size = pes->header[8] + 9;
                    
                    if(pes->total_size > 6)
                        pes->payload_size = pes->total_size - pes->pes_header_size;
                } 
                else    //不是pes包的开头,在刚开始接收数据时,一般是接收了pes包的部分,然后才会收到pes包的开头.这时本tspacket就skip掉
                {
                    /* otherwise, it should be a table, skip packet */
skip:
                    pes->state = MPEGTS_SKIP;
                    continue;
                }
            }
            break;

            /**********************************************/
            /* PES packing parsing */
        case MPEGTS_PESHEADER_FILL:
            len = pes->pes_header_size - pes->data_index;
            if (len > buf_size)
                len = buf_size;

						for(j=0; j<len; j++)
							pes->header[pes->data_index++] = *p++;
            buf_size -= len;
						
            if (pes->data_index == pes->pes_header_size) 
            {
                const __u8 *r;
                __u32       flags;
								
                flags = pes->header[7];

								r = pes->header + 9;
                pes->pts = (__s64)-1;   //一个新的pes包开始拿pts了,先初始化pts为-1.

								if ((flags & 0xc0) == 0x80) 
                {									
										pes->pts_dts_flag = 1;
                  	pes->pts = get_pts(r);				                  
                  	r += 5;
                } 
                else if ((flags & 0xc0) == 0xc0) 
                {
               			pes->pts_dts_flag = 1;
                   	pes->pts = get_pts(r);											 
                    r += 10;
                }
								else if ((flags & 0xc0) == 0) 
								{
										pes->pts_dts_flag = 0;
										pes->pts = 0;
								}
							
                if((flags & 0x20) == 0x20)  //ESCR_flag
                {
                    r += 6;
                }
                if((flags & 0x10) == 0x10)  //ES_rate_flag
                {
                    r += 3;
                }
                if((flags & 0x08) == 0x08)  //DSM_trick_mode_flag
                {
                    r += 1;
                }
                if((flags & 0x04) == 0x04)  //additional_copy_info_flag
                {
                    r += 1;
                }
                if((flags & 0x02) == 0x02)  //PES_CRC_flag
                {
                    r += 2;
                }
                if((flags & 0x01) == 0x01)  //PES_extension_flag
                {
                    __u32 PES_extension_field_flags = r[0];
                    r+=1;
                    if((PES_extension_field_flags&0x80) == 0x80)    //PES_private_data_flag
                    {
                    	
                        memcpy(pes->data_info.privateData, r, PES_PRIVATE_DATA_LEN);
                        //esKSRV_memcpy(pes->data_info.privateData, r, PES_PRIVATE_DATA_LEN);
                        r += PES_PRIVATE_DATA_LEN;
												
                    }
                    //剩下的暂时不管了.
                }

                /* we got the full header. We parse it and get the payload */
                pes->state = MPEGTS_PAYLOAD;
            }
            break;

        case MPEGTS_PAYLOAD:
           if (pes->total_size) 
            {
                len = pes->total_size - pes->data_index;
                if (len > buf_size)
                    len = buf_size;
            } 
            else 
            {
                len = buf_size;
            }
           
        
            if (len > 0) 
            {
            		if(pes->pts_dts_flag != 0)
            			{
									  if (pes->pts != (__s64)-1)
		                {
	                     if (pes->pts != pes->pre_pts) 
	         				     	{
		         				    	 first = 1;			
	         				     	}
		                }
										pes->push_subpes_data(p, len, first, pes);
										if(pes->pts != (__s64)-1)
										{
												pes->pre_pts = pes->pts;
										}
										  pes->pts = -1;
						          return;
            			}
            
									else
									{											
											first = 1;
											pes->push_subpes_data(p, len, first, pes);
											return;
									}
            	}
            buf_size = 0;
            break;

        case MPEGTS_SKIP:
            buf_size = 0;
            break;
        }
    }
}
void HandleAvPackets(__u8* pktData, __u32 pktNum, __pes_filter_t *pes)
{
    __u32 i;
    __u8* pkt;
    __u8* p;
    __u8* p_end;
    __s32 is_start;
    __s32 afc;
		__s32 tsc;

#if (DEBUG_DUMPDATA)
    if(0x114 == pes->pid)
    {
        pCDumpData->WriteData(pCDumpData, pktData, 188*pktNum);
    }
#endif

		
    pkt = pktData - 188;
    for(i=pktNum; i>0; i--)
    {
   //	WARNING("tsh=0x%x\n",*((__u32*)pkt));
      pkt += 188;

        is_start = pkt[1] & 0x40;
        	/*transport_scrambling_control*///加密标识
				tsc = (pkt[3] >> 4) & 6;
        /* skip adaptation field */
        afc = (pkt[3] >> 4) & 3;
        p = pkt + 4;
        if (afc == 0 || afc == 2) /* reserved value */
            continue;
        
        if (afc == 3) 
        {
            /* skip adaptation field */
            if(is_start && (p[0] > 0))
            {
                pes->rap_flag = (p[1] & 0x40)>>6;   //* get random_access_indicator
            }
            
            p += p[0] + 1;
        }
        
        /* if past the end of packet, ignore */
        p_end = pkt + 188;
        if (p >= p_end)
            continue;

        handle_av_pkt_payload(p, p_end - p, is_start, pes);
    }
}

void HandleSubPackets(__u8* pktData, __u32 pktNum, __pes_filter_t *pes)
{
    __u32 i;
    __u8* pkt;
    __u8* p;
    __u8* p_end;
    __s32 is_start;
    __s32 afc;
		__s32 tsc;

#if(DEBUG_DUMPDATA)
    if(0x114 == pes->pid)
    {
        pCDumpData->WriteData(pCDumpData, pktData, 188*pktNum);
    }
#endif
		
    pkt = pktData - 188;
    for(i=pktNum; i>0; i--)
    {
      pkt += 188;

        is_start = pkt[1] & 0x40;
				/*transport_scrambling_control*///加密标识
				tsc = (pkt[3] >> 4) & 6;
        /* skip adaptation field */
        afc = (pkt[3] >> 4) & 3;
        p = pkt + 4;
        if (afc == 0 || afc == 2) /* reserved value */
            continue;
        
        if (afc == 3) 
        {
            /* skip adaptation field */
            if(is_start && (p[0] > 0))
            {
                pes->rap_flag = (p[1] & 0x40)>>6;   //* get random_access_indicator
            }
            
            p += p[0] + 1;
        }
        
        /* if past the end of packet, ignore */
        p_end = pkt + 188;
        if (p >= p_end)
            continue;
        
        handle_sub_pkt_payload(p, p_end - p, is_start, pes);
    }
}

__u32 HandleSectionPackets(__u8* pktData, __u32 pktNum, __section_filter_t *tss)
{
    __u32 i;
    __u8* pkt;
    __u8* p;
    __u8* p_end;
    __s32 is_start; //表示这个ts包是否含有si section的开头,但是也有可能在si section的开头的前面是上一个section的末尾
    __s32 afc;
    __s32 cc, cc_ok;
    __s32 len;
    __s32 programNum;
    
    pkt = pktData - 188;
    for(i=pktNum; i>0; i--)
    {
    	//	WARNING("tsh=0x%x\n",*((__u32*)pkt));
        pkt += 188;
        is_start = pkt[1] & 0x40;   //查询playload_unit_start_indicator位.置1表示有一个section的开头在这个ts包中.
        if (is_start == 0)
        {
            if (tss->need_start)    //如果ts包没有开始的section,又需要一个section的开头,那么只有skip这个ts包
                continue;
        }
        
        /* skip adaptation field */
        afc = (pkt[3] >> 4) & 3;    //查看adaptation_field_control, 2个bit
        p = pkt + 4;
        if (afc == 0 || afc == 2) /* reserved value,需要的是payload,如果没有,直接skip这个ts包 */
            continue;
        
        if (afc == 3) //如果有adaptation_field就跳过去.直指payload
            p += p[0] + 1;
        
        /* if past the end of packet, ignore */
        p_end = pkt + 188;
        if (p >= p_end)
            continue;
        
        cc = pkt[3] & 0xf;  // 查询continuity_counter
        cc_ok = (tss->last_cc < 0) || (((tss->last_cc + 1) & 0xf) == cc);
        tss->last_cc = cc;
		WARNING("table id = %x,tss->servId = %x,programNum = %x\n",p[1],tss->servId,p[4]<<8|p[5]);

		if((p[1] == 2) && (tss->servId != 0) && (tss->servId != (p[4]<<8|p[5])))//pmt table id
		{	
			WARNING("search same PMT ,but servId different!\n");
			continue;
		}

        if (is_start)
        {
            //* pointer field present.
            len = *p++;
            if ((p + len) > p_end)  //这说明指示的si section的起始位置可能有错,所以直接返回,不再处理.
                return 0;

            if (len && cc_ok && !tss->need_start)   //新section之前的那部分可能属于上一个section,先push_section_data().
            {
              WARNING("wao! we meet section_tail fisrt in a ts packet! tali_len[%d]\n", len);
              tss->push_section_data(p, len, 0, tss);
            }

            p += len;   //这时p指向si_section的开始处
            if (p < p_end)
             {
                tss->push_section_data(p,  p_end - p, is_start, tss);   //section的完整性在push_section_data()中处理
            }
            
        }
        else if(cc_ok)
        {
           tss->push_section_data(p,  p_end - p, is_start, tss);
        }
        
        if (is_start)
            tss->need_start = 0;
    }
    return 0;
}


/*******************************************************************************
Function name: tsfilter_main_task
Description: 
    1.统一pes, si_section和tspacket的处理
Parameters: 
    
Return: 
    
Time: 2012/6/13
*******************************************************************************/
static void tsfilter_main_task(void* arg)
{
    __s32 		        result;
    __s32               chan;
    __mp*               tscDrv;
    tsf_data_t	        tsfData;
    __filter_v2_t*         filter;
    __pes_filter_t*     pes;
    __section_filter_t  *section;
    __tsdata_filter_t   *tspacketfilter;
    __s32   i;

    filter = (__filter_v2_t*)arg;
    //pes    = &filter->u.pes_filter;
    chan   = filter->chan;
    tscDrv = filter->tscDrv;
#if (DEBUG_DUMPDATA)
    if(0x114 == filter->pid)
    {
        WARNING("pid 0x114 pes_main_task is begin!\n");
        pCDumpData->Reinit(pCDumpData);
    }
#endif
    while(1)
    {
        //* 1. check quit request
        if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
            break;

        //* 2. wait for data
        result = WaitData(filter->dataSem);
        if (result != 0)
        {
            WARNING("wait pes data timeout.\n");
//            continue;
        }

        //* 3. get data buffer pointer
        result = esMODS_MIoctrl(tscDrv, TSC_CMD_REQUEST_CHAN_DATA, chan, &tsfData);
        if (result != 0)
            break;

//        if(filter->pid == 0x111)
//        {
//            DISABLE_OS_TASK_SWITCH();
//        }
        //* 4. handle packets,逐个虚通道遍历
        for(i=0;i<VCHANCNT;i++)
        {
            if(filter->vChanArray[i].nRequestCloseFlag)
            {
                WARNING("chan[%d], vchanidx[%d], vchan[0x%x], receive close_request\n", chan, i, filter->vChanArray[i].nVirtualChanId);
                filter->vChanArray[i].dmxType = DMX_TYPE_NONE;
                filter->vChanArray[i].nRequestCloseFlag = 0;
                continue;
            }
            
            if(DMX_TYPE_SECTION == filter->vChanArray[i].dmxType)   //拼si section
            {
                section = &filter->vChanArray[i].vfilter.section_filter;
                if (tsfData.pktNum)
                {
                    eLIBs_FlushDCacheRegion(tsfData.pData, tsfData.pktNum*188);
                    HandleSectionPackets(tsfData.pData, tsfData.pktNum, section);
                }
                
//                if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
//                    break;


                if (tsfData.ringPktNum)
                {
                    eLIBs_FlushDCacheRegion(tsfData.pRingData, tsfData.ringPktNum*188);
                    HandleSectionPackets(tsfData.pRingData, tsfData.ringPktNum, section);
                }
            }
            else if(DMX_TYPE_VIDEO == filter->vChanArray[i].dmxType
                || DMX_TYPE_AUDIO == filter->vChanArray[i].dmxType)//拼video audio pes包
            {
                pes = &filter->vChanArray[i].vfilter.pes_filter;

                if (tsfData.pktNum)
                {
                    eLIBs_FlushDCacheRegion(tsfData.pData, tsfData.pktNum*188);
                    HandleAvPackets(tsfData.pData, tsfData.pktNum, pes);
                }

                if (tsfData.ringPktNum)
                {
                    eLIBs_FlushDCacheRegion(tsfData.pRingData, tsfData.ringPktNum*188);
                    HandleAvPackets(tsfData.pRingData, tsfData.ringPktNum, pes);
                }
            }
			else if(DMX_TYPE_SUBTITLE == filter->vChanArray[i].dmxType)  //拼subtitle pes包
            {
                pes = &filter->vChanArray[i].vfilter.pes_filter;
                if (tsfData.pktNum)
                {
                    eLIBs_FlushDCacheRegion(tsfData.pData, tsfData.pktNum*188);
                    HandleSubPackets(tsfData.pData, tsfData.pktNum, pes);
                }
                
                if (tsfData.ringPktNum)
                {
                    eLIBs_FlushDCacheRegion(tsfData.pRingData, tsfData.ringPktNum*188);
                    HandleSubPackets(tsfData.pRingData, tsfData.ringPktNum, pes);
                }
            }
            else if(DMX_TYPE_TS_DATA_VIDEO == filter->vChanArray[i].dmxType
                || DMX_TYPE_TS_DATA_AUDIO == filter->vChanArray[i].dmxType
                || DMX_TYPE_TS_DATA_SUBTITLE == filter->vChanArray[i].dmxType
                || DMX_TYPE_TS_DATA_SECTION == filter->vChanArray[i].dmxType)   //直接处理ts packet
            {
                tspacketfilter = &filter->vChanArray[i].vfilter.tsdata_filter;
                if (tsfData.pktNum)
                {
                    eLIBs_FlushDCacheRegion(tsfData.pData, tsfData.pktNum*188);
                    tspacketfilter->push_ts_data(tsfData.pData, tsfData.pktNum*188, tspacketfilter);
                }
                
                if (tsfData.ringPktNum)
                {
                    eLIBs_FlushDCacheRegion(tsfData.pRingData, tsfData.ringPktNum*188);
                    tspacketfilter->push_ts_data(tsfData.pRingData, tsfData.ringPktNum*188, tspacketfilter);
                }
            }
            else if(DMX_TYPE_TS_DATA == filter->vChanArray[i].dmxType)
            {
                WARNING("fatal error, impossible! chan[%d], vchanidx[%d], vchan[0x%x], dmxType == DMX_TYPE_TS_DATA\n", chan, i, filter->vChanArray[i].nVirtualChanId);
            }
            else
            {
                
            }
        }
//        if(filter->pid == 0x111)
//        {
//            ENABLE_OS_TASK_SWITCH();
//        }
        esMODS_MIoctrl(tscDrv, TSC_CMD_FLUSH_CHAN_DATA, chan, (void*)(tsfData.pktNum + tsfData.ringPktNum));
    }
    
    esKRNL_TDel(EXEC_prioself);
}

__s32 FreeFilter(__filter_v2_t *filter)
{
    __u8    err;
    if(filter->dataSem)
    {
        esKRNL_SemDel(filter->dataSem, OS_DEL_ALWAYS, &err);
        filter->dataSem = NULL;
    }
    free(filter);
    return EPDK_OK;
}

static pid_filter_type_t GetTscFilterType(dvb_dmx_type_t dmx_type)
{
    pid_filter_type_t   tsc_pid_filter_type;
    switch(dmx_type)
    {
        case DMX_TYPE_VIDEO:
        case DMX_TYPE_TS_DATA_VIDEO:
        {
            tsc_pid_filter_type = PID_FILTER_TYPE_VIDEO;
            break;
        }
        case DMX_TYPE_AUDIO:
        case DMX_TYPE_TS_DATA_AUDIO:
        {
            tsc_pid_filter_type = PID_FILTER_TYPE_AUDIO;
            break;
        }
        case DMX_TYPE_SUBTITLE:
        case DMX_TYPE_TS_DATA_SUBTITLE:
        {
            tsc_pid_filter_type = PID_FILTER_TYPE_SUBTITLE;
            break;
        }
        case DMX_TYPE_SECTION:
        case DMX_TYPE_TS_DATA_SECTION:
        {
            tsc_pid_filter_type = PID_FILTER_TYPE_SECTION;
            break;
        }
        case DMX_TYPE_TS_DATA:
        {
            WARNING("fatal error! can't be DMX_TYPE_TS_DATA!\n");
            tsc_pid_filter_type = PID_FILTER_TYPE_TS_DATA;
            break;
        }
        default:
        {
            WARNING("fatal error! impossible? \n");
            tsc_pid_filter_type = PID_FILTER_TYPE_TS_DATA;
            break;
        }
    }
    return tsc_pid_filter_type;
}

/*******************************************************************************
Function name: OpenFilter
Description: 
    
Parameters: 
    par: 调用者设置下来的参数. 其中nVirtualChanId是要填写，返回给调用者的.
Return: 
    malloc出来的ctx->pids[pid]
Time: 2012/6/12
*******************************************************************************/
__filter_v2_t* OpenFilter(__u32 pid, dvb_dmx_chan_param_t* par, __mpegts_context_t* ctx)
{
    //__s32               result;
    __s32               chan;
    __filter_v2_t*         filter = NULL;
    filter_register_t   filter_register;

//    result = alloc_filter(pid, par, ctx);
//    if(result < 0)
//        return NULL;
    __u8                err;
    //__filter_v2_t*      filter = NULL;
    __pes_filter_t*     pes;
    __section_filter_t* section;
    __tsdata_filter_t   *tspacketfilter;
    __s32   i;
    __s32   nVChanIdx;
    __s32   nChanActiveFlag = 0;    //表示硬件tsc的实通道是否已经在运行了
    
    esKRNL_SemPend(ctx->mutex, 50, &err);
    if (err != 0)
        return NULL;

    if (ctx->pids[pid])
    {
        //esKRNL_SemPost(ctx->mutex);
        //return -1;
        WARNING("chan[%d] for pid[0x%x] is already exit. Virtual channel will be used!\n", ctx->pids[pid]->chan, pid);
        filter = ctx->pids[pid];
        nChanActiveFlag = 1;
    }
    else    //该pid的filter没有打开,开始malloc
    {
        filter = (__filter_v2_t*)malloc(sizeof(__filter_v2_t));
        if (filter == NULL)
        {
            WARNING("malloc filter fail\n");
            esKRNL_SemPost(ctx->mutex);
            return NULL;
        }
        memset(filter, 0, sizeof(__filter_v2_t));
        
        filter->dataSem = esKRNL_SemCreate(0);
        if (filter->dataSem == NULL)
        {
            WARNING("create semaphore for filter fail!\n");
            free(filter);
            esKRNL_SemPost(ctx->mutex);
            return NULL;
        }
        nChanActiveFlag = 0;
    }
    //查空闲的虚通道
    for(i=0; i<VCHANCNT; i++)
    {
        if(DMX_TYPE_NONE == filter->vChanArray[i].dmxType)
        {
            WARNING("find empty vchan[%d] for pid[0x%x]\n", i, pid);
            //查错
            if(i>0 && 0 == nChanActiveFlag)
            {
                WARNING("fatal error! check code! hwfilter is not active, but vchan[%d] > 0!\n", i);
            }
            break;
        }
    }
    if(i >= VCHANCNT)
    {
        WARNING("fatal error! vchan is full for pid[0x%x]\n", pid);
        goto _err0;
    }
    nVChanIdx = i;
    if(par->dmx_type == DMX_TYPE_SECTION)
    {
        section = &filter->vChanArray[nVChanIdx].vfilter.section_filter;

        section->need_start         = 1;
        section->last_cc            = -1;
        section->pid                = pid;
        section->RequestDataCB      = par->request_buffer_cb;
        section->UpdateDateCB       = par->update_data_cb;
        section->push_section_data  = push_section_data;
        section->servId             = par->servId;
    }
    else if(par->dmx_type == DMX_TYPE_VIDEO || par->dmx_type == DMX_TYPE_AUDIO )
    {
        pes = &filter->vChanArray[nVChanIdx].vfilter.pes_filter;
        
        pes->pid            = pid;
        pes->RequestDataCB  = par->request_buffer_cb;
        pes->UpdateDateCB   = par->update_data_cb;
        pes->push_avpes_data  = push_avpes_data;
    }
	else if(par->dmx_type == DMX_TYPE_SUBTITLE)
    {
        pes = &filter->vChanArray[nVChanIdx].vfilter.pes_filter;
        
        pes->pid            = pid;
        pes->RequestDataCB  = par->request_buffer_cb;
        pes->UpdateDateCB   = par->update_data_cb;
        pes->push_subpes_data  = push_subpes_data;
    }
    else if(par->dmx_type == DMX_TYPE_TS_DATA_VIDEO || par->dmx_type == DMX_TYPE_TS_DATA_AUDIO 
        || par->dmx_type == DMX_TYPE_TS_DATA_SUBTITLE || par->dmx_type == DMX_TYPE_TS_DATA_SECTION)
    {
        WARNING("Be careful! request raw ts packet filter. dmx_type[0x%x]\n", par->dmx_type);
        tspacketfilter = &filter->vChanArray[nVChanIdx].vfilter.tsdata_filter;

        tspacketfilter->pid            = pid;
        tspacketfilter->RequestDataCB  = par->request_buffer_cb;
        tspacketfilter->UpdateDateCB   = par->update_data_cb;
        tspacketfilter->push_ts_data  = push_tspacket_data;
    }
    else
    {
        WARNING("fatal error! who request dmx_type [0x%x]?\n", par->dmx_type);
        goto _err0;
    }
    //分2种情况,一是硬件filter未打开,二是已经打开
    if(0 == nChanActiveFlag)    //未打开硬件filter
    {
        filter->pid         = pid;
        filter->tscDrv      = ctx->drvTsc;
        
        ctx->pids[pid]      = filter;   //filter基本配置完成


        //准备调用tsc驱动的接口正式开启tsc的硬件filter.
        //filter_register.type            = pid_filter_type[par->dmx_type];
        filter_register.type            = GetTscFilterType(par->dmx_type);
        filter_register.pid             = filter->pid;
        filter_register.callback        = esKRNL_GetCallBack(dataArriveCallBack);
        filter_register.callbackparam   = filter->dataSem;
        chan = esMODS_MIoctrl(filter->tscDrv, TSC_CMD_OPEN_CHAN, 0, (void*)&filter_register);
        if(chan < 0)
        {
            WARNING("open tsc pid filter fail, pid = %d.\n", pid);
            //free_filter(pid, ctx);
            FreeFilter(filter);
            ctx->pids[pid]  = NULL;
            goto _err0;
        }
        
        filter->chan = chan;
        
        par->nVirtualChanId = filter->vChanArray[nVChanIdx].nVirtualChanId = (__s32)(chan<<16 | nVChanIdx);
        WARNING("virtual channel id = 0x%x, 0x%x\n", par->nVirtualChanId, filter->vChanArray[nVChanIdx].nVirtualChanId);
        filter->vChanArray[nVChanIdx].dmxType = par->dmx_type;   //必须放最后,等所有参数配置好.这有个同步互斥的问题.
        
        filter->filterTaskId = (__u32)esKRNL_TCreate(tsfilter_main_task, (void*)filter, 0x1000, KRNL_priolevel3);    //KRNL_priolevel4
        if(filter->filterTaskId == 0)
        {
           WARNING("create tsfilter main task fail, pid = %d.\n", pid);
           esMODS_MIoctrl(filter->tscDrv, TSC_CMD_CLOSE_CHAN, chan, NULL);
           //free_filter(pid, ctx);
           FreeFilter(filter);
           ctx->pids[pid]  = NULL;
           goto _err0;
        }
    }
    else    //硬件filter已经打开了
    {
        if(filter->pid != pid)
        {
            WARNING("fatal error! filter->pid[0x%x] != pid[0x%x]\n", filter->pid, pid);
        }
        par->nVirtualChanId = filter->vChanArray[nVChanIdx].nVirtualChanId = (__s32)(filter->chan<<16 | nVChanIdx);
        WARNING("[2]virtual channel id = 0x%x\n", filter->vChanArray[nVChanIdx].nVirtualChanId);
        filter->vChanArray[nVChanIdx].dmxType = par->dmx_type;   //必须放最后,等所有参数配置好.这有个同步互斥的问题.
    }
    esKRNL_SemPost(ctx->mutex);
    return filter;

_err0:
    esKRNL_SemPost(ctx->mutex);
    return NULL;

}

/*******************************************************************************
Function name: CloseEntireFilter
Description: 
    1.把一个实通道关掉
Parameters: 
    
Return: 
    
Time: 2012/6/14
*******************************************************************************/
void CloseEntireFilter(__filter_v2_t* filter, __mpegts_context_t* ctx)
{
    __s32   i;
    if(NULL == filter)
    {
        WARNING("filter == NULL, not need close hwfilter!\n");
        return;
    }
    //关闭所有虚通道
    for(i=0;i<VCHANCNT;i++)
    {
        if(DMX_TYPE_NONE != filter->vChanArray[i].dmxType)
        {
            CloseFilter(filter, filter->vChanArray[i].nVirtualChanId, ctx);
        }
    }
}

/*******************************************************************************
Function name: CloseFilter
Description: 
    1.关闭一个虚通道
Parameters: 
    
Return: 
    
Time: 2012/6/14
*******************************************************************************/
void CloseFilter(__filter_v2_t* filter, __s32 nVirtualChanId, __mpegts_context_t* ctx)
{
    __u8    err;
    __s32   i;
    __s32   nVChanIdx = nVirtualChanId&0xffff;
    __s32   nChanIdx = nVirtualChanId>>16;
    __s32   nCloseChanFlag = 0;
    __s32   pid;
    if(NULL == filter)
    {
        WARNING("filter == NULL, not need close filter!\n");
        return;
    }
    pid = filter->pid;
    esKRNL_SemPend(ctx->mutex, 500, &err);  //等5s
    if(err != 0)
    {
        WARNING("fatal error! closefilter pend semaphore fail!\n");
        return;
    }
    if(filter->chan != nChanIdx)
    {
        WARNING("fatal error! filter->chan[%d] != nChanIdx[%d]\n", filter->chan, nChanIdx);
    }
    if(nVChanIdx >= VCHANCNT)
    {
        WARNING("fatal error! nVChanIdx[%d] >= [%d]\n", nVChanIdx, VCHANCNT);
    }
    if(DMX_TYPE_NONE == filter->vChanArray[nVChanIdx].dmxType)
    {
        WARNING("Be careful! pid[0x%x], chan[%d], vchan[%d], dmxType == DMX_TYPE_NONE when closefilter!\n", filter->pid, filter->chan, nVChanIdx);
        goto _err0;
    }
    filter->vChanArray[nVChanIdx].nRequestCloseFlag = 1;
    while(1)
    {
        esKRNL_TimeDly(5);
        if(0 == filter->vChanArray[nVChanIdx].nRequestCloseFlag && DMX_TYPE_NONE == filter->vChanArray[nVChanIdx].dmxType)
        {
            WARNING("pid[0x%x], chan[%d], vchan[%d] close filter done!\n", filter->pid, filter->chan, nVChanIdx);
            break;
        }
    }
    //虚通道关闭完成后, 检查实通道是否需要关闭
    for(i = 0; i<VCHANCNT; i++)
    {
        if(DMX_TYPE_NONE != filter->vChanArray[i].dmxType)
        {
            break;
        }
    }
    if(VCHANCNT == i)   //说明虚通道关完了
    {
        nCloseChanFlag = 1;
    }
    else
    {
        nCloseChanFlag = 0;
    }
    
    if(nCloseChanFlag)
    {
        WARNING("pid[0x%x], chan[%d], all vchan closed, disable filter now!\n", filter->pid, filter->chan);
        esKRNL_TimeDlyResume(filter->filterTaskId);
        while(esKRNL_TDelReq(filter->filterTaskId) != OS_TASK_NOT_EXIST)
        {
           esKRNL_TimeDlyResume(filter->filterTaskId);
           esKRNL_TimeDly(1);
        }
        filter->filterTaskId = 0;
        //free_filter(filter->pid, ctx);
        FreeFilter(filter);
        ctx->pids[pid]  = NULL;
        //wait for filter task exit
        esMODS_MIoctrl(filter->tscDrv, TSC_CMD_CLOSE_CHAN, nChanIdx, NULL);
    }
    esKRNL_SemPost(ctx->mutex);
    return;

_err0:
    esKRNL_SemPost(ctx->mutex);
    return;

}

void OpenPCRDetect(__u32 pid, __mpegts_context_t* ctx)
{
    esMODS_MIoctrl(ctx->drvTsc, TSC_CMD_OPEN_PCR_DETECT, pid, (void*)ctx->pcrCallback);
    return;
}


void ClosePCRDetect(__u32 pid, __mpegts_context_t* ctx)
{
    esMODS_MIoctrl(ctx->drvTsc, TSC_CMD_CLOSE_PCR_DETECT, 0, NULL);
    return;
}




__s32 ReadTsPackets(__filter_v2_t* filter, __u8* buf, __u32 size)
{
    __s32 		        result;
    __u32               chan;
    __mp*               tscDrv;
    tsf_data_t	        tsfData;
    
    chan    = filter->chan;
    tscDrv  = filter->tscDrv;
    
    result = esMODS_MIoctrl(tscDrv, TSC_CMD_REQUEST_CHAN_DATA, chan, (void*)&tsfData);
    if (result != 0)
    {
        WARNING("read error.\n");
        return 0;
    }
    
    if(size <= tsfData.pktNum)
    {
        eLIBs_FlushDCacheRegion(tsfData.pData, size*188);
        memcpy(buf, tsfData.pData, size*188);
        //esKSRV_memcpy(buf, tsfData.pData, size*188);
        esMODS_MIoctrl(tscDrv, TSC_CMD_FLUSH_CHAN_DATA, chan, (void*)size);
        return size;
    }
    else if(size <= (tsfData.pktNum + tsfData.ringPktNum))
    {
        eLIBs_FlushDCacheRegion(tsfData.pData, tsfData.pktNum*188);
        memcpy(buf, tsfData.pData, tsfData.pktNum*188);
        //esKSRV_memcpy(buf, tsfData.pData, tsfData.pktNum*188);
        eLIBs_FlushDCacheRegion(tsfData.pRingData, (size - tsfData.pktNum)*188);
        memcpy(buf + 188*tsfData.pktNum, tsfData.pRingData, (size - tsfData.pktNum)*188);
        esMODS_MIoctrl(tscDrv, TSC_CMD_FLUSH_CHAN_DATA, chan, (void*)size);
        return size;
    }
    else
    {
        eLIBs_FlushDCacheRegion(tsfData.pData, tsfData.pktNum*188);
        memcpy(buf, tsfData.pData, tsfData.pktNum*188);
        //esKSRV_memcpy(buf, tsfData.pData, tsfData.pktNum*188);
        eLIBs_FlushDCacheRegion(tsfData.pRingData, tsfData.ringPktNum*188);
        memcpy(buf + 188*tsfData.pktNum, tsfData.pRingData, tsfData.ringPktNum*188);
        //esKSRV_memcpy(buf + 188*tsfData.pktNum, tsfData.pRingData, tsfData.ringPktNum*188);
        esMODS_MIoctrl(tscDrv, TSC_CMD_FLUSH_CHAN_DATA, chan, (void*)(tsfData.pktNum + tsfData.ringPktNum));
        return tsfData.pktNum + tsfData.ringPktNum;
    }
}

#if 0   //老版本代码保留
static void pes_main_task(void* arg)
{
    __s32 		        result;
    __s32               chan;
    __mp*               tscDrv;
    tsf_data_t	        tsfData;
    __filter_t*         filter;
    __pes_filter_t*     pes;

    filter = (__filter_t*)arg;
    pes    = &filter->u.pes_filter;
    chan   = filter->chan;
    tscDrv = filter->tscDrv;
#if (DEBUG_DUMPDATA)
    if(0x114 == pes->pid)
    {
        WARNING("pid 0x114 pes_main_task is begin!\n");
        pCDumpData->Reinit(pCDumpData);
    }
#endif
    while(1)
    {
        //* 1. check quit request
        if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
            break;

        //* 2. wait for data
        result = WaitData(filter->dataSem);
        if (result != 0)
        {
            MESSAGE("wait pes data timeout.\n");
//            continue;
        }

        //* 3. get data buffer pointer
        result = esMODS_MIoctrl(tscDrv, TSC_CMD_REQUEST_CHAN_DATA, chan, &tsfData);
        if (result != 0)
            break;

//        if(filter->pid == 0x111)
//        {
//            DISABLE_OS_TASK_SWITCH();
//        }
        //* 4. handle packets
        if (tsfData.pktNum)
        {
            eLIBs_FlushDCacheRegion(tsfData.pData, tsfData.pktNum*188);
            HandlePackets(tsfData.pData, tsfData.pktNum, pes);
        }
        
        if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
            break;

        if (tsfData.ringPktNum)
        {
            eLIBs_FlushDCacheRegion(tsfData.pRingData, tsfData.ringPktNum*188);
            HandlePackets(tsfData.pRingData, tsfData.ringPktNum, pes);
        }
//        if(filter->pid == 0x111)
//        {
//            ENABLE_OS_TASK_SWITCH();
//        }
        esMODS_MIoctrl(tscDrv, TSC_CMD_FLUSH_CHAN_DATA, chan, (void*)(tsfData.pktNum + tsfData.ringPktNum));
    }
    
    esKRNL_TDel(EXEC_prioself);
}


/*******************************************************************************
Function name: section_main_task
Description: 
    1.对应实channel的线程,接收数据到来的semaphore. 负责遍历对应的所有虚channel并逐个调用callback处理
Parameters: 
    
Return: 
    
Time: 2011/1/6
*******************************************************************************/
static void section_main_task(void* arg)
{
    __s32 		        result;
    __s32               chan;
    __mp*               tscDrv;
    tsf_data_t	        tsfData;
    __filter_t*         filter;
    __section_filter_t* section;

    filter = (__filter_t*)arg;
    section    = &filter->u.section_filter;
    chan   = filter->chan;
    tscDrv = filter->tscDrv;
    
    section->need_start  = 1;
    section->end_of_section_reached = 0;
    
    while(1)
    {
        //* 1. check quit request
        if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
            break;

        //* 2. wait for data
        result = WaitData(filter->dataSem);
        if (result != 0)
        {
            MESSAGE("wait pes data timeout.\n");
//            continue;
        }

        //* 3. get data buffer pointer
        result = esMODS_MIoctrl(tscDrv, TSC_CMD_REQUEST_CHAN_DATA, chan, &tsfData);
        if (result != 0)
            break;

        //* 4. handle packets
        if (tsfData.pktNum) //先处理第一段数据
        {
            eLIBs_FlushDCacheRegion(tsfData.pData, tsfData.pktNum*188);
            HandleSectionPackets(tsfData.pData, tsfData.pktNum, section);
        }
        
        if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
            break;

        if (tsfData.ringPktNum) //再处理第二段数据
        {
            eLIBs_FlushDCacheRegion(tsfData.pRingData, tsfData.ringPktNum*188);
            HandleSectionPackets(tsfData.pRingData, tsfData.ringPktNum, section);
        }
        
        esMODS_MIoctrl(tscDrv, TSC_CMD_FLUSH_CHAN_DATA, chan, (void*)(tsfData.pktNum + tsfData.ringPktNum));
    }
    
    esKRNL_TDel(EXEC_prioself);
}

static __s32 alloc_filter(__u32 pid, dvb_dmx_chan_param_t* par, __mpegts_context_t* ctx)
{
    __u8                err;
    __filter_t*         filter;
    __pes_filter_t*     pes;
    __section_filter_t* section;
    
    esKRNL_SemPend(ctx->mutex, 50, &err);
    if (err != 0)
        return -1;

    if (ctx->pids[pid])
    {
        esKRNL_SemPost(ctx->mutex);
        return -1;
    }
    
    filter = (__filter_t*)malloc(sizeof(__filter_t));
    if (filter == NULL)
    {
        esKRNL_SemPost(ctx->mutex);
        return -1;
    }
    memset(filter, 0, sizeof(__filter_t));
    
    filter->dataSem = esKRNL_SemCreate(0);
    if (filter->dataSem == NULL)
    {
        free(filter);
        esKRNL_SemPost(ctx->mutex);
        return -1;
    }
    
    if(par->dmx_type == DMX_TYPE_SECTION || par->dmx_type == DMX_TYPE_TS_DATA)
    {
        section = &filter->u.section_filter;
        
        section->last_cc            = -1;
        section->pid                = pid;
        section->RequestDataCB      = par->request_buffer_cb;
        section->UpdateDateCB       = par->update_data_cb;
        section->push_section_data  = push_section_data;
        
    }
    else
    {
        pes = &filter->u.pes_filter;
        
        pes->pid            = pid;
        pes->RequestDataCB  = par->request_buffer_cb;
        pes->UpdateDateCB   = par->update_data_cb;
        pes->push_pes_data  = push_data;
    }
    
    filter->pid         = pid;
    filter->dmxType     = par->dmx_type;
    filter->tscDrv      = ctx->drvTsc;
    
    ctx->pids[pid]      = filter;

    esKRNL_SemPost(ctx->mutex);

    return 0;
}


static __s32 free_filter(__u32 pid, __mpegts_context_t* ctx)
{
    __u8                err;
    __filter_t*         filter;
    
    esKRNL_SemPend(ctx->mutex, 50, &err);
    if (err != 0)
        return -1;

    filter = ctx->pids[pid];
    if (filter == NULL)
    {
        esKRNL_SemPost(ctx->mutex);
        return -1;
    }

    if(filter->dataSem)
    {
        esKRNL_SemDel(filter->dataSem, OS_DEL_ALWAYS, &err);
        filter->dataSem = NULL;
    }

    free(filter);

    ctx->pids[pid]  = NULL;

    esKRNL_SemPost(ctx->mutex);

    return 0;
}

static pid_filter_type_t pid_filter_type[] = {  //注意必须和dvb_dmx_type_t对应
    PID_FILTER_TYPE_VIDEO,
    PID_FILTER_TYPE_AUDIO,
    PID_FILTER_TYPE_SUBTITLE,
    PID_FILTER_TYPE_SECTION,
    PID_FILTER_TYPE_TS_DATA
};

__filter_t* OpenFilter0(__u32 pid, dvb_dmx_chan_param_t* par, __mpegts_context_t* ctx)
{
    __s32               result;
    __s32               chan;
    __filter_t*         filter;
    filter_register_t   filter_register;

    result = alloc_filter(pid, par, ctx);
    if(result < 0)
        return NULL;
    
    filter = ctx->pids[pid];
    
    filter_register.type            = pid_filter_type[par->dmx_type];
    filter_register.pid             = filter->pid;
    filter_register.callback        = esKRNL_GetCallBack(dataArriveCallBack);
    filter_register.callbackparam   = filter->dataSem;
    chan = esMODS_MIoctrl(filter->tscDrv, TSC_CMD_OPEN_CHAN, 0, (void*)&filter_register);
    if(chan < 0)
    {
        WARNING("open tsc pid filter fail, pid = %d.\n", pid);
        free_filter(pid, ctx);
        return NULL;
    }
    
    filter->chan = chan;
    
    if(par->dmx_type <= DMX_TYPE_SUBTITLE)
        filter->filterTaskId = (__u32)esKRNL_TCreate(pes_main_task, (void*)filter, 0x1000, KRNL_priolevel3);    //KRNL_priolevel4
    else           // create section maintask
        filter->filterTaskId = (__u32)esKRNL_TCreate(section_main_task, (void*)filter, 0x1000, KRNL_priolevel4);
    
    if(filter->filterTaskId == 0)
    {
       WARNING("create pes or section main task fail, pid = %d.\n", pid);
       esMODS_MIoctrl(filter->tscDrv, TSC_CMD_CLOSE_CHAN, chan, NULL);
       free_filter(pid, ctx);
       return NULL;
    }
    return filter;
}



void CloseFilter0(__filter_t* filter, __mpegts_context_t* ctx)
{
  
    esKRNL_TimeDlyResume(filter->filterTaskId);
    while(esKRNL_TDelReq(filter->filterTaskId) != OS_TASK_NOT_EXIST)
    {
       esKRNL_TimeDlyResume(filter->filterTaskId);
       esKRNL_TimeDly(1);
    }
    filter->filterTaskId = 0;
    free_filter(filter->pid, ctx);
        //wait for filter task exit
    esMODS_MIoctrl(filter->tscDrv, TSC_CMD_CLOSE_CHAN, filter->chan, NULL);
    return;
}

#endif

