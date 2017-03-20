
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
    1.һ�����ڽ�������Ƶ���ݵ�pes��.
    2.len:�����Ͷ�������
Parameters: 
    
Return: 
    
Time: 2011/2/1
*******************************************************************************/
static __s32 push_avpes_data(__u8* data, __u32 len, __u32 new_frm, __pes_filter_t* pes)
{
    __u8*               dataPtr;
    
    dataPtr = data;
    
    if(new_frm) //�������˼�ǵȵ���һ֡�ĵ�����,���ͳ���һ֡������.��������Ƶ,û�����⡣�����ڷǳ���ɢ����Ļ֡,�ᵼ��һ����Ļ֡�ٳ��Ͳ���ȥ.
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

            //�ж�pes���Ƿ��Ѿ�װ��, ���װ�������ͳ�.
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
    1.һ�����ڽ�����Ļ���ݵ�pes��.
    2.len:�����Ͷ�������
Parameters: 
    
Return: 
    
Time: 2011/2/1
*******************************************************************************/
static __s32 push_subpes_data(__u8* data, __u32 len, __u32 new_frm, __pes_filter_t* pes)
{
    __u8*               dataPtr;
    
    dataPtr = data;
    
    if(new_frm) //�������˼�ǵȵ���һ֡�ĵ�����,���ͳ���һ֡������.��������Ƶ,û�����⡣�����ڷǳ���ɢ����Ļ֡,�ᵼ��һ����Ļ֡�ٳ��Ͳ���ȥ.
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

            //�ж�pes���Ƿ��Ѿ�װ��, ���װ�������ͳ�.
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
    1.һ�����ڽ���si���section.
    2.��������ᴦ��si section��������.
    3. len��ʾ��������ݵĳ��ȣ�������ݲ�һ������һ��section.���Կ���ֻ����һ����.
    4.�п���һ��packet�����2��section�Ŀ�ʼ����,����һ��Ҫ�����������.
    5.Ĭ���ͽ��������ݱ���ȫ������������˳�.
    6.�������ź��µ��ݴ�����.
Parameters: 
    len:����͵���section�Ŀ�ʼ��,һ��Ͱ�ʣ�µ�ts���ĳ���ȫ�ͽ���. ���section�Ŀ�ʼ��֮ǰ����һ������,�����Ƕ����ݵĳ���.
        һ������� >= ��ts��������section�ĳ���
    new_section:��ʾ����ͽ����������Ƿ���si section�Ŀ�ʼ
Return: 
    -1:ʧ��
Time: 2011/2/1
*******************************************************************************/
static __s32 push_section_data(__u8* data, __u32 len, __u32 new_section, __section_filter_t* tss)
{
    __u8*               dataPtr;
    __u32   nCurLen = len;
    __u32   nCopyLen;   //��Ҫ��ts���������ֽڳ���
    __u32   nNewSectionFlag = new_section;
    __s32   nSectionLeftLen = 0;    //��¼��ǰһ��section��ʣ�����ֽ�û�б�����
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
        if(nCurLen > tss->section_h_size)   //һ��ts packet���ܰ���2��section��header,������Ϊ�˱���ѵڶ���section headҲ������.
        {
            nCopyLen = tss->section_h_size;
        }
        else
        {
            nCopyLen = nCurLen;
        }
    }
    else    //�������һ����section�Ŀ�ʼ,��tss->section_h_size ==0,����ζ�����section�ղ�������,����˵ǰ���һ����û����,��ôֱ�Ӷ����ɡ�
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

        //if(tss->free_size < nCurLen)    //���������section_h_size����ȷ,���ǿ��ǵ����ź��¿��ܳ���,������len������.�����������ܻᶪsection,
        if(tss->free_size < nCopyLen)   //һ����˵������.����������ָ����,�Ϳ�����������,����������section������.
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
            esKRNL_CallBack(tss->UpdateDateCB, &tss->section_h_size);   //���������bug,������ts������������һ��section�Ŀ�ͷ,��ô��һ��section�ͱ�skip�ˡ�EIT��Ľ������ܻ������������.
            tss->free_size = 0;
            tss->section_h_size = 0;
        }
        
        if(nCurLen > 0) //˵������ʣ�µĲ���,�������µ�section,Ҳ��������������. �ȵ���new section����.
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
            if(tss->section_h_size > MAX_SECTION_SIZE)  //��ʹ�����ź���ɵĴ���Ҳ����,ֱ��skip
            {   
                if(tss->section_h_size != (0xfff + 3))
                {
                    WARNING("[3]the size of the section_h_size is error,section_h_size=[0x%x], nLeftLen[%d]set to [0x%x]\n",tss->section_h_size, nCurLen, MAX_SECTION_SIZE);
                }
                //tss->section_h_size = MAX_SECTION_SIZE;
                tss->section_h_size = 0;
                return 0;
            }
            if(tss->section_h_size <= 3)    //����ֱ�ӷ�����.
            {   
                WARNING("[4]the size of the section_h_size is error,section_h_size=%d\n",tss->section_h_size);
                //tss->section_h_size = MAX_SECTION_SIZE;
                tss->section_h_size = 0;
                return 0;
            }
            if(nCurLen > tss->section_h_size)   //һ��ts packet���ܰ���2��section��header,������Ϊ�˱���ѵڶ���section headҲ������.
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
    2. ��ΪĿǰ��Ϊ¼��׼����.������Ҫ����¼�Ƶ�ʵ�ַ�ʽ�����.
    3. ��Ϊtspacketʵ���ϲ����κδ���, ����dmx��������ģʽ�ǲ���Ҫ��ƴ�������,����
        ��ֱ���õ���������������callback����������.dmxֻҪ�������͹�ȥ����.
        ����RequestDataCB()�ǲ�Ӧ����ʲô�õ�.������ʽ�ϵ���һ��.
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
    1.����һ��ts��,ƴpes��,ֻ����VIDEO,AUDIO���͵�PES��.
Parameters: 
    1.buf:Ҫ�����buf����ʼ��ַ,��ʵ����һ��ts���ķ�Χ����ʼ��ַ��ts����ǰ��.
    2.buf_size:Ҫ�����buf�ĳ���,һ����һ��ts��ʣ��ĳ���.
    3.is_start:�Ƿ���һ��pes���Ŀ�ʼ.
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
                    {   //����mpeg2����Ƶ����,����total_size = 0.����FUJI 12Seg_BCAS_1Seg.ts�еĸ���service.mpeg2��Ƶ���ݵ�total_size = 0,������Ƶ����Ļ��pes�������г��ȵ�.
                        //WARNING("pes total_size = 0!, code[0x%x]\n", code);
                    }

                    pes->pes_header_size = pes->header[8] + 9;
                    
                    if(pes->total_size > 6)
                        pes->payload_size = pes->total_size - pes->pes_header_size;
                } 
                else    //����pes���Ŀ�ͷ,�ڸտ�ʼ��������ʱ,һ���ǽ�����pes���Ĳ���,Ȼ��Ż��յ�pes���Ŀ�ͷ.��ʱ��tspacket��skip��
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
                pes->pts = (__s64)-1;   //һ���µ�pes����ʼ��pts��,�ȳ�ʼ��ptsΪ-1.
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
                    //ʣ�µ���ʱ������.
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
    1.����һ��ts��,ƴpes��.���ڴ���ISDBT�� subtitle��PES��(������PTS�Ͳ���PTS)��
Parameters: 
    1.buf:Ҫ�����buf����ʼ��ַ,��ʵ����һ��ts���ķ�Χ����ʼ��ַ��ts����ǰ��.
    2.buf_size:Ҫ�����buf�ĳ���,һ����һ��ts��ʣ��ĳ���.
    3.is_start:�Ƿ���һ��pes���Ŀ�ʼ.
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
                    {   //����mpeg2����Ƶ����,����total_size = 0.����FUJI 12Seg_BCAS_1Seg.ts�еĸ���service.mpeg2��Ƶ���ݵ�total_size = 0,������Ƶ����Ļ��pes�������г��ȵ�.
                        //WARNING("pes total_size = 0!, code[0x%x]\n", code);
                    }

                    pes->pes_header_size = pes->header[8] + 9;
                    
                    if(pes->total_size > 6)
                        pes->payload_size = pes->total_size - pes->pes_header_size;
                } 
                else    //����pes���Ŀ�ͷ,�ڸտ�ʼ��������ʱ,һ���ǽ�����pes���Ĳ���,Ȼ��Ż��յ�pes���Ŀ�ͷ.��ʱ��tspacket��skip��
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
                pes->pts = (__s64)-1;   //һ���µ�pes����ʼ��pts��,�ȳ�ʼ��ptsΪ-1.

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
                    //ʣ�µ���ʱ������.
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
        	/*transport_scrambling_control*///���ܱ�ʶ
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
				/*transport_scrambling_control*///���ܱ�ʶ
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
    __s32 is_start; //��ʾ���ts���Ƿ���si section�Ŀ�ͷ,����Ҳ�п�����si section�Ŀ�ͷ��ǰ������һ��section��ĩβ
    __s32 afc;
    __s32 cc, cc_ok;
    __s32 len;
    __s32 programNum;
    
    pkt = pktData - 188;
    for(i=pktNum; i>0; i--)
    {
    	//	WARNING("tsh=0x%x\n",*((__u32*)pkt));
        pkt += 188;
        is_start = pkt[1] & 0x40;   //��ѯplayload_unit_start_indicatorλ.��1��ʾ��һ��section�Ŀ�ͷ�����ts����.
        if (is_start == 0)
        {
            if (tss->need_start)    //���ts��û�п�ʼ��section,����Ҫһ��section�Ŀ�ͷ,��ôֻ��skip���ts��
                continue;
        }
        
        /* skip adaptation field */
        afc = (pkt[3] >> 4) & 3;    //�鿴adaptation_field_control, 2��bit
        p = pkt + 4;
        if (afc == 0 || afc == 2) /* reserved value,��Ҫ����payload,���û��,ֱ��skip���ts�� */
            continue;
        
        if (afc == 3) //�����adaptation_field������ȥ.ֱָpayload
            p += p[0] + 1;
        
        /* if past the end of packet, ignore */
        p_end = pkt + 188;
        if (p >= p_end)
            continue;
        
        cc = pkt[3] & 0xf;  // ��ѯcontinuity_counter
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
            if ((p + len) > p_end)  //��˵��ָʾ��si section����ʼλ�ÿ����д�,����ֱ�ӷ���,���ٴ���.
                return 0;

            if (len && cc_ok && !tss->need_start)   //��section֮ǰ���ǲ��ֿ���������һ��section,��push_section_data().
            {
              WARNING("wao! we meet section_tail fisrt in a ts packet! tali_len[%d]\n", len);
              tss->push_section_data(p, len, 0, tss);
            }

            p += len;   //��ʱpָ��si_section�Ŀ�ʼ��
            if (p < p_end)
             {
                tss->push_section_data(p,  p_end - p, is_start, tss);   //section����������push_section_data()�д���
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
    1.ͳһpes, si_section��tspacket�Ĵ���
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
        //* 4. handle packets,�����ͨ������
        for(i=0;i<VCHANCNT;i++)
        {
            if(filter->vChanArray[i].nRequestCloseFlag)
            {
                WARNING("chan[%d], vchanidx[%d], vchan[0x%x], receive close_request\n", chan, i, filter->vChanArray[i].nVirtualChanId);
                filter->vChanArray[i].dmxType = DMX_TYPE_NONE;
                filter->vChanArray[i].nRequestCloseFlag = 0;
                continue;
            }
            
            if(DMX_TYPE_SECTION == filter->vChanArray[i].dmxType)   //ƴsi section
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
                || DMX_TYPE_AUDIO == filter->vChanArray[i].dmxType)//ƴvideo audio pes��
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
			else if(DMX_TYPE_SUBTITLE == filter->vChanArray[i].dmxType)  //ƴsubtitle pes��
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
                || DMX_TYPE_TS_DATA_SECTION == filter->vChanArray[i].dmxType)   //ֱ�Ӵ���ts packet
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
    par: ���������������Ĳ���. ����nVirtualChanId��Ҫ��д�����ظ������ߵ�.
Return: 
    malloc������ctx->pids[pid]
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
    __s32   nChanActiveFlag = 0;    //��ʾӲ��tsc��ʵͨ���Ƿ��Ѿ���������
    
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
    else    //��pid��filterû�д�,��ʼmalloc
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
    //����е���ͨ��
    for(i=0; i<VCHANCNT; i++)
    {
        if(DMX_TYPE_NONE == filter->vChanArray[i].dmxType)
        {
            WARNING("find empty vchan[%d] for pid[0x%x]\n", i, pid);
            //���
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
    //��2�����,һ��Ӳ��filterδ��,�����Ѿ���
    if(0 == nChanActiveFlag)    //δ��Ӳ��filter
    {
        filter->pid         = pid;
        filter->tscDrv      = ctx->drvTsc;
        
        ctx->pids[pid]      = filter;   //filter�����������


        //׼������tsc�����Ľӿ���ʽ����tsc��Ӳ��filter.
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
        filter->vChanArray[nVChanIdx].dmxType = par->dmx_type;   //��������,�����в������ú�.���и�ͬ�����������.
        
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
    else    //Ӳ��filter�Ѿ�����
    {
        if(filter->pid != pid)
        {
            WARNING("fatal error! filter->pid[0x%x] != pid[0x%x]\n", filter->pid, pid);
        }
        par->nVirtualChanId = filter->vChanArray[nVChanIdx].nVirtualChanId = (__s32)(filter->chan<<16 | nVChanIdx);
        WARNING("[2]virtual channel id = 0x%x\n", filter->vChanArray[nVChanIdx].nVirtualChanId);
        filter->vChanArray[nVChanIdx].dmxType = par->dmx_type;   //��������,�����в������ú�.���и�ͬ�����������.
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
    1.��һ��ʵͨ���ص�
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
    //�ر�������ͨ��
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
    1.�ر�һ����ͨ��
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
    esKRNL_SemPend(ctx->mutex, 500, &err);  //��5s
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
    //��ͨ���ر���ɺ�, ���ʵͨ���Ƿ���Ҫ�ر�
    for(i = 0; i<VCHANCNT; i++)
    {
        if(DMX_TYPE_NONE != filter->vChanArray[i].dmxType)
        {
            break;
        }
    }
    if(VCHANCNT == i)   //˵����ͨ��������
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

#if 0   //�ϰ汾���뱣��
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
    1.��Ӧʵchannel���߳�,�������ݵ�����semaphore. ���������Ӧ��������channel���������callback����
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
        if (tsfData.pktNum) //�ȴ����һ������
        {
            eLIBs_FlushDCacheRegion(tsfData.pData, tsfData.pktNum*188);
            HandleSectionPackets(tsfData.pData, tsfData.pktNum, section);
        }
        
        if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
            break;

        if (tsfData.ringPktNum) //�ٴ���ڶ�������
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

static pid_filter_type_t pid_filter_type[] = {  //ע������dvb_dmx_type_t��Ӧ
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

