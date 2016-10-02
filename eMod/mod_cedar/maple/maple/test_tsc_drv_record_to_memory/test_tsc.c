
#include "test_tsc_s.h"
#include "test_tsc.h"

#define DEFAULT_FREQUENCY   722000      //* 722000 KHz
#define DEFAULT_BANDWIDTH   8000        //* 8000 KHz
#define MAX_RECORD_TASK     15

#define CHECK_DATA          (0)

#define PAUSE   \
                        do{                     \
                            __u32 debug = 1;    \
                            while(debug);       \
                        }while(0);

enum
{
    CMD_HELP = 0,
    CMD_EXIT,
    
    CMD_TUNE,
    CMD_SIGNAL_STATUS,
    CMD_LOCK,
    CMD_UNLOCK,
    
    CMD_RECORD,
    CMD_STOP_RECORD,
    
    CMD_ERR
};


typedef struct RECORD_CONTEXT
{
    __u32 pid;
    __u32 isPesMode;
    __u32 isRecording;
    __u32 taskId;
    __mp* tscDrv;
    __u8  filePath[256];
}record_ctx_t;


record_ctx_t    gRecordCtx[MAX_RECORD_TASK];


/*******************************************************************************
Function name: PallocMaxMemoryForTest
Description: 
    1.还是要保留几M左右的.先保留10M看看.
Parameters: 
    pMemSize:实际palloc出来的内存数量.以byte为单位
Return: 
    palloc的内存指针.
Time: 2012/4/5
*******************************************************************************/
__s8* PallocMaxMemoryForTest(__s32 *pMemSize)
{
    __u32   uFreeMemSize;
    __u32   nMMemNum;    //以Mbyte为单位的内存数量
    __u32   i;
    __s8    *pMem = NULL;
    
    *pMemSize = 0;
    uFreeMemSize = esMEMS_FreeMemSize();
    nMMemNum = uFreeMemSize/(1024*1024);
    eLIBs_printf("system left [%d]Mbytes memory!\n", nMMemNum);
    if(nMMemNum <= 10)
    {
        eLIBs_printf("NO enough memory, return !\n");
        return NULL;
    }

    //测试
//    pMem = (__s8*)esMEMS_Palloc(8<<10, MEMS_PALLOC_MODE_BND_NONE | MEMS_PALLOC_MODE_BNK_NONE | MEMS_PALLOC_MODE_AREA_NONE);
//    *pMemSize = 8*1024*1024;
//    return pMem;
    
    for(i=nMMemNum - 5; i>0;i-=10)    //预留5M字节.
    {
        pMem = (__s8*)esMEMS_Palloc(i<<10, MEMS_PALLOC_MODE_BND_NONE | MEMS_PALLOC_MODE_BNK_NONE | MEMS_PALLOC_MODE_AREA_NONE);
        if(pMem)
        {
            eLIBs_printf("palloc [%d]MBytes success!\n", i);
            *pMemSize = i*1024*1024;
            break;
        }
        else
        {
            eLIBs_printf("palloc [%d]MBytes fail!\n", i);
        }
    }
    
    return pMem;
}

//* call back function for tsc filter to notify data arriving.
__s32 DataNotify(void* arg)
{
    __krnl_event_t* sem;
    OS_SEM_DATA     sem_data;

    sem = (__krnl_event_t*)arg;
    esKRNL_SemQuery(sem, &sem_data);
    if (sem_data.OSCnt == 0)
        esKRNL_SemPost(sem);
    else
    {
        WARNING("fatal error! SemCnt[%d]\n", sem_data.OSCnt);
    }

    return 0;    
}

//* function for waiting data using semaphore.
__s32 WaitDataNotify(__krnl_event_t* sem)
{
    __u8 err;
    esKRNL_SemPend(sem, 100, &err);
    return err;
}


static void record_task(void* arg)
{
    __u8                err;
    __s32               result;
    
    record_ctx_t*       p;
    ES_FILE*            file = NULL;
    __s32               chan;
    __krnl_event_t*     semaphore;
    __pCBK_t            callback;
    filter_register_t   filter_register;
    tsf_data_t          tsfData;
    
    __s32 nDumpMemSize = 0;
    __s8  *pDumpMem;
    CDumpDataV3   *pCDumpData = NULL;
    __s32   nMemFullFlag = 0;
    FILE            *pFile = NULL;

    
    p    = (record_ctx_t*)arg;
    file = NULL;
    chan = -1;
    semaphore = NULL;
    
    if(p == NULL)
        goto _record_task_exit;
    
    semaphore = esKRNL_SemCreate(0);
    if(semaphore == NULL)
    {
        eLIBs_printf("can not create semaphore for recording task, pid = %d.\n", p->pid);
        goto _record_task_exit;
    }
    
//    file = eLIBs_fopen((const char*)p->filePath, "wb");
//    if(file == NULL)
//    {
//        eLIBs_printf("can not create file(%s), quit recording task, pid = %d.\n", p->filePath, p->pid);
//        goto _record_task_exit;
//    }
    //确定还剩多少内存, 并且分配
    pDumpMem = PallocMaxMemoryForTest(&nDumpMemSize);
    if(NULL == pDumpMem)
    {
        eLIBs_printf("palloc memory fail! quit recording task! pid = %d.\n", p->pid);
        goto _record_task_exit;
    }
    pFile = fopen("F:\\dtv.ts", "wb+");
    if(NULL == pFile)
    {
        WARNING("can't open file dtv.ts!\n");
        goto _record_task_exit;
    }
    pCDumpData = newCDumpDataV3(pDumpMem, nDumpMemSize, pFile);
    if(NULL == pCDumpData)
    {
        eLIBs_printf("CDumpData malloc fail!\n");
    }
    
    //* open tsc channel.
    callback                        = esKRNL_GetCallBack(DataNotify);
    filter_register.type            = PID_FILTER_TYPE_VIDEO;//PID_FILTER_TYPE_TS_DATA;PID_FILTER_TYPE_VIDEO
    filter_register.pid             = p->pid;
    filter_register.callback        = callback;
    filter_register.callbackparam   = (void*)semaphore;
    chan = esMODS_MIoctrl(p->tscDrv, TSC_CMD_OPEN_CHAN, 0, (void*)&filter_register);
    eLIBs_printf("open tsc channel[%d]\n", chan);
    if(chan < 0)
    {
        eLIBs_printf("can not open a channel from tsc for recording, pid = %d.\n", p->pid);
        goto _record_task_exit;
    }
        
    do
    {
        //* 1. check if there is a request to quit this task.
        if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
        {
            break;
        }
        
        //* 2. wait for video data.
        WaitDataNotify(semaphore);
        
        //* 3. request channel data.
        result = esMODS_MIoctrl(p->tscDrv, TSC_CMD_REQUEST_CHAN_DATA, chan, &tsfData);
        if (result != 0)
        {
            eLIBs_printf("audio receiving task request data fail.\n");
            continue;
        }
        if(0 == tsfData.pktNum)
        {
            eLIBs_printf("tsfData.pktNum[%d], tsfData.ringPktNum[%d]\n", tsfData.pktNum, tsfData.ringPktNum);
        }
        //* 4. save audio ts packets to file.
        if (tsfData.pktNum > 0)
        {
#if CHECK_DATA
            {
                __u8*  pos47;
                __u32  pktCnt;
                
                pos47 = tsfData.pData;
                
                for(pktCnt = tsfData.pktNum; pktCnt > 0; pktCnt--)
                {
                    if(*pos47 != 0x47)
                    {
                        eLIBs_printf("47 sync byte invalid, pid = %d, pktnum[%d]\n", p->pid, tsfData.pktNum);
//                        goto _record_task_exit;
                        break;
                    }
                    
                    pos47 += 188;
                }
            }
#endif
            //eLIBs_fwrite(tsfData.pData, 1, tsfData.pktNum*188, file);
            if(0 == nMemFullFlag)
            {
                if(EPDK_OK != pCDumpData->WriteData(pCDumpData,tsfData.pData, tsfData.pktNum*188))
                {
                    eLIBs_printf("dump buffer is full!\n");
                    nMemFullFlag = 1;
                }
            }
        }

        if (tsfData.ringPktNum > 0)
        {
            //eLIBs_fwrite(tsfData.pRingData, 1, tsfData.ringPktNum*188, file);
            if(0 == nMemFullFlag)
            {
                if(EPDK_OK != pCDumpData->WriteData(pCDumpData,tsfData.pRingData, tsfData.ringPktNum*188))
                {
                    eLIBs_printf("dump buffer is full!\n");
                    nMemFullFlag = 1;
                }
            }
#if CHECK_DATA
            {
                __u8*  pos47;
                __u32  pktCnt;
                
                pos47 = tsfData.pRingData;
                
                for(pktCnt = tsfData.ringPktNum; pktCnt > 0; pktCnt--)
                {
                    if(*pos47 != 0x47)
                    {
                        eLIBs_printf("47 sync byte invalid, pid = %d. ringPktNum[%d]\n", p->pid, tsfData.ringPktNum);
//                        goto _record_task_exit;
                        break;
                    }
                    
                    pos47 += 188;
                }
            }
#endif
        }

        //* 5. flush audio data in tsc filter.
        esMODS_MIoctrl(p->tscDrv, TSC_CMD_FLUSH_CHAN_DATA, chan, (void*)(tsfData.pktNum + tsfData.ringPktNum));

    }while(1);

_record_task_exit:
    
    //* close the channel.
    if(chan >= 0)
    {
        chan = esMODS_MIoctrl(p->tscDrv, TSC_CMD_CLOSE_CHAN, chan, NULL);
        chan = -1;
    }
    
    //* close the file.
    if(file)
    {
        eLIBs_fclose(file);
        file = NULL;
    }
    eLIBs_printf("record task will quit! pid[0x%x]. Input something to continue! then we will write ts to sd card!\n", p->pid);
    __getc();
    if(pCDumpData)
    {
        deleteCDumpDataV3(pCDumpData);
        pCDumpData = NULL;
    }
    if(pFile)
    {
        fclose(pFile);
        pFile = NULL;
    }

    if(pDumpMem)
    {
        esMEMS_Pfree(pDumpMem, nDumpMemSize>>10);
        pDumpMem = NULL;
        nDumpMemSize = 0;
    }
    //* delete semaphore.
    if(semaphore)
    {
        esKRNL_SemDel(semaphore, OS_DEL_ALWAYS, &err);
        semaphore = NULL;
    }
    
    eLIBs_printf("exit recording task of pid %d.\n", p->pid);
    esKRNL_TDel(EXEC_prioself);
}


static __u32 str2val(const char* str)
{
    __u32       tmp;
    __u32       value;
    const char* p;
    
    value = 0;
    p     = str;
    
    while(*p && (*p >= 0x30) && (*p <= 0x39))
    {
        tmp = *p++ - 0x30;
        value = value*10 + tmp;
    }
    
    return value;
}


static void parse_tune_param(const char* str, __u32* pFrequency, __u32* pBandwidth)
{
    __u32 len;
    __u32 i;
    
    *pFrequency = DEFAULT_FREQUENCY;
    *pBandwidth = DEFAULT_BANDWIDTH;
    
    len = eLIBs_strlen(str);
    
    for(i=0; i<len; i++)
    {
        if(str[i] == '-' && i < (len-3))
        {
            if(str[i+1] == 'f')
            {
                *pFrequency = str2val(str+i+3);
            }
            
            if(str[i+1] == 'b')
            {
                *pBandwidth = str2val(str+i+3);
            }
        }
    }
    
    return;
}


static void parse_pid(const char* str, __u32* pPid)
{
    __u32 i;
    __u32 len;
    
    *pPid = 0;
    
    len = eLIBs_strlen(str);
    
    for(i=0; i<len; i++)
    {
        if(str[i] == '-' && i < (len-3))
        {
            if(str[i+1] == 'p' && str[i+2] != 'e')
            {
                *pPid = str2val(str+i+3);
                break;
            }
        }
    }
    
    return;
}


static void parse_file_path(const char* str, __u8 filePath[])
{
    __u32 i;
    __u32 j;
    __u32 len;
    
    len = eLIBs_strlen(str);
    
    for(i=0; i<len; i++)
    {
        if(str[i] == '-' && i < (len-3))
        {
            if(str[i+1] == 'f' && str[i+2] == ' ')
            {
                i += 3;
                j=0;
                while((i+j < len) && str[i+j] != ' ' && str[i+j] != 0)
                {
                    filePath[j] = str[i+j];
                    j++;
                }
                
                break;
            }
        }
    }
    
    return;
}


static void parse_pes_mode(const char* str, __u32* pMode)
{
    __u32 i;
    __u32 len;
    
    *pMode = 0;
    
    len = eLIBs_strlen(str);
    
    for(i=0; i<len; i++)
    {
        if(str[i] == '-' && i < (len-3))
        {
            if(str[i+1] == 'p' && str[i+2] == 'e' && str[i+3] == 's')
            {
                *pMode = 1;
                break;
            }
        }
    }
    
    return;
}


static void printHelp(void)
{
    eLIBs_printf("Usage : command [param].\n");
    eLIBs_printf("example: help                                             (print this usage)\n");
    eLIBs_printf("example: exit                                             (exit this test program)\n");
    eLIBs_printf("example: tune [-f frequency(KHz)] [-b bandwidth(KHz)]     (tune the demod to specific frequency value)\n");
    eLIBs_printf("example: get status                                       (get current signal status)\n");
    eLIBs_printf("example: lock                                             (start a lock task to monitor the signal status)\n");
    eLIBs_printf("example: unlock                                           (delete the lock task)\n");
    eLIBs_printf("example: record [-p pid value] [-f file] [-pes]           (record ts packet or pes data of specific pid to a file");
    eLIBs_printf("example: stop record [-p pid value]                       (stop recording data of specific pid");
    eLIBs_printf("\n");
}


static __s32 lock_callback(void* arg)
{
    lock_cb_param_t *p;
    
    p = (lock_cb_param_t*)arg;
    if(p == NULL)
        return -1;
        
    if(p->lockOk == 0)
    {
        eLIBs_printf("lock task notify: no signal.\n");
    }
    else
    {
        eLIBs_printf("lock task notify: strength = %d, quality = %d.\n", p->signalStatus.strength, p->signalStatus.quality);
    }
    
    return 0;
}


static void start_record(__u32 pid, __u32 pesMode, const char* filePath, __mp* tscDrv)
{
    __u32 i;
    
    for(i=0; i<MAX_RECORD_TASK; i++)
    {
        if(gRecordCtx[i].pid == pid && gRecordCtx[i].isRecording == 1)
        {
            eLIBs_printf("data of pid %d is already being recorded, can not create a new recoding task.\n");
            return;
        }
    }
    
    for(i=0; i<MAX_RECORD_TASK; i++)
    {
        if(gRecordCtx[i].isRecording == 0)
            break;
    }
    
    if(i == MAX_RECORD_TASK)
    {
        eLIBs_printf("create recording task fail, too mush active recording task, maxinum task num is %d.\n", MAX_RECORD_TASK);
        return;
    }
    
    eLIBs_strcpy((char*)gRecordCtx[i].filePath, filePath);
    gRecordCtx[i].pid           = pid;
    gRecordCtx[i].tscDrv        = tscDrv;
    gRecordCtx[i].isPesMode     = pesMode;
    gRecordCtx[i].isRecording   = 1;
    
    gRecordCtx[i].taskId = esKRNL_TCreate(record_task, (void*)&gRecordCtx[i], 0x1000, KRNL_priolevel3);
    if(gRecordCtx[i].taskId == 0)
    {
        gRecordCtx[i].isRecording = 0;
        eLIBs_printf("can not create a task to record data of pid %d.\n", pid);
    }
    
    return;
}


static void stop_record(__u32 pid)
{
    __u32 i;
    
    for(i=0; i<MAX_RECORD_TASK; i++)
    {
        if(gRecordCtx[i].pid == pid && gRecordCtx[i].isRecording == 1 && gRecordCtx[i].taskId != 0)
        {
            esKRNL_TimeDlyResume(gRecordCtx[i].taskId);
            while(esKRNL_TDelReq(gRecordCtx[i].taskId) != OS_TASK_NOT_EXIST)
            {
                esKRNL_TimeDlyResume(gRecordCtx[i].taskId);
                esKRNL_TimeDly(10);
            }
            
            gRecordCtx[i].taskId      = 0;
            gRecordCtx[i].isRecording = 0;
            
            break;
        }
    }
    
    return;
}

static __s32 FEConfig_Init(CFEConfig *pFEConfig)
{
    memset(pFEConfig, 0, sizeof(CFEConfig));
    pFEConfig->nTS_INTERFACE_TYPE = 0;
    pFEConfig->TS_INTERFACE_PACKET_SYNC_METHOD = 1;
    pFEConfig->TS_INTERFACE_SSI_DATA_ORDER = 0;
    pFEConfig->TS_INTERFACE_CLOCK_SIGNAL_POLARITY = 0;
    pFEConfig->TS_INTERFACE_ERROR_SIGNAL_POLARITY = 0;
    pFEConfig->TS_INTERFACE_DATA_VALID_SIGNAL_POLARITY = 0;
    pFEConfig->TS_INTERFACE_PSYNC_SIGNAL_POLARITY = 0;
    pFEConfig->TS_INTERFACE_PACKET_SIZE = 0;
    return EPDK_OK;
}

__s32 test_main(void* arg)
{
    __u32           cmd;
    char            str[256];
    __u32           demodMid;
    __mp*           demodDrv;
    __pCBK_t        callback;
    tune_param_t    tp;
    lock_cb_param_t cbParam;
    demod_ss_t      ss;         //* signal status
    
    __u32           tscMid;
    __mp*           tscDrv;
    
    __u32           pid;
    __u32           pesMode;
    __u8            filePath[256];
    __u32           uFreeMemSize;
    __s8            cmdCount = 0;
    __u32           startTime = 0, endTime = 0;
    
    CFEConfig       FEConfig;

    eLIBs_printf("==================================================================\n");
    eLIBs_printf("=======================| demod test program |=====================\n");
    eLIBs_printf("==================================================================\n");
    
    demodMid = 0;
    demodDrv = NULL;
    
    tscMid   = 0;
    tscDrv   = NULL;
    
    memset(gRecordCtx, 0, sizeof(gRecordCtx));
    
    //* install demod driver
    demodMid = esMODS_MInstall(DEMOD_DRV_FILE_PATH, 0);
    if(!demodMid)
    {
        eLIBs_printf("test program install demod driver fail.\n");
        goto _exit_test;
    }
    
    tscMid = esMODS_MInstall(TSC_DRIVER_FILE_PATH, 0);
    if(!tscMid)
    {
        eLIBs_printf("test program install tsc driver fail.\n");
        goto _exit_test;
    }
    
    //* open demod driver
    demodDrv = esMODS_MOpen(demodMid, 0);
    if(!demodDrv)
    {
        eLIBs_printf("test program open demod driver fail.\n");
        goto _exit_test;
    }
        
    tscDrv = esMODS_MOpen(tscMid, 0);
    if(!tscDrv)
    {
        eLIBs_printf("test program open tsc driver fail.\n");
        goto _exit_test;
    }
    FEConfig_Init(&FEConfig);
    esMODS_MIoctrl(tscDrv, TSC_CMD_SET_FECONFIG, 0, &FEConfig);
    if(EPDK_OK != esMODS_MIoctrl(tscDrv, TSC_CMD_OPEN, 0, NULL))
    {
        eLIBs_printf("tsc IOOpen fail\n");
        goto _exit_test;
    }

    uFreeMemSize = esMEMS_FreeMemSize();
    eLIBs_printf("system left [%d]Mbytes memory!\n", uFreeMemSize/(1024*1024));
    //esKSRV_SysInfo();
    cmdCount = 0;
    startTime = esKRNL_TimeGet();
    while(1)
    {
        if(esKRNL_TDelReq(OS_PRIO_SELF) == OS_TASK_DEL_REQ)
        {
            eLIBs_printf("exit demodulater test program!\n");
            goto _exit_test;
        }

        while(1)
        {
            endTime = esKRNL_TimeGet();
            eLIBs_printf("\n===>test_tsc<===:>");
            if ((endTime-startTime) > 1*60*100) //录制1分钟
            {
                cmdCount = 3;
            }
            //* try to get a command from user
            eLIBs_memset(str, 0x00, 128);
            //esSIOS_gets(str);
            if (0 ==cmdCount)
            {
               eLIBs_memcpy(str, "help", 32);
            }
            else if (1 ==cmdCount)
            {
               eLIBs_memcpy(str, "tune -f 474000 -b 8000", 64; 
            }
            else if (2 ==cmdCount)
            {
                eLIBs_memcpy(str, "record -pe 821 -f f:\\videoData -pes", 64); 
            }
            else if (3 ==cmdCount)
            {
                eLIBs_memcpy(str, "stop record -pe 821", 64); 
                
            }
            else
            {
                cmdCount = -1;
            }
            if((eLIBs_strncmp(str, "help", 4) == 0))
            {
                cmd = CMD_HELP;
                break;
            }

            if((eLIBs_strncmp(str, "exit", 4) == 0))
            {
                cmd = CMD_EXIT;
                break;
            }
            
            if((eLIBs_strncmp(str, "tune", 4) == 0))
            {
                cmd = CMD_TUNE;
                break;
            }

            if((eLIBs_strncmp(str, "get status", 10) == 0))
            {
                cmd = CMD_SIGNAL_STATUS;
                break;
            }

            if((eLIBs_strncmp(str, "lock", 4) == 0))
            {
                cmd = CMD_LOCK;
                break;
            }

            if((eLIBs_strncmp(str, "unlock", 6) == 0))
            {
                cmd = CMD_UNLOCK;
                break;
            }
            
            if((eLIBs_strncmp(str, "record", 6) == 0))
            {
                cmd = CMD_RECORD;
                break;
            }
            
            if((eLIBs_strncmp(str, "stop record", 11) == 0))
            {
                cmd = CMD_STOP_RECORD;
                break;
            }
            
            eLIBs_printf("bad comman!\n");
            esKRNL_TimeDly(6);
        }
        eLIBs_printf("cmd: %s\n", str);
        switch(cmd)
        {
            case CMD_HELP:
                printHelp();
                break;
                
            case CMD_EXIT:
                goto _exit_test;
                
            case CMD_TUNE:
                parse_tune_param(str, &tp.frequency, &tp.bandwidth);
                eLIBs_printf("fre:%d, bw:%d\n", tp.frequency, tp.bandwidth);
                if(esMODS_MIoctrl(demodDrv, DEMOD_TUNE, 0, (void*)&tp) != 0)
                    eLIBs_printf("test program tune demod fail.\n");
                else
                    eLIBs_printf("test program tune demod ok.\n");
                    
                break;
                
            case CMD_SIGNAL_STATUS:
                if(esMODS_MIoctrl(demodDrv, DEMOD_GET_SIGNAL_STATUS, 0, (void*)&ss) != 0)
                    eLIBs_printf("test program get signal status fail.\n");
                else
                    eLIBs_printf("strength = %d, quality = %d.\n", ss.strength, ss.quality);
                    
                break;
                
            case CMD_LOCK:
                cbParam.pOpaque = NULL;
                callback = esKRNL_GetCallBack(lock_callback);
                esMODS_MIoctrl(demodDrv, DEMOD_SET_LOCK_CB, (__u32)&cbParam, (void*)callback);
                esMODS_MIoctrl(demodDrv, DEMOD_LOCK, 0, NULL);
                
                break;
                
            case CMD_UNLOCK:
                esMODS_MIoctrl(demodDrv, DEMOD_UNLOCK, 0, NULL);
                break;
                
            case CMD_RECORD:
                
                pid     = 0;
                pesMode = 0;
                memset(filePath, 0, sizeof(filePath));
                
                parse_pid(str, &pid);
                parse_file_path(str, filePath);
                parse_pes_mode(str, &pesMode);
                eLIBs_printf("pid:%d, fp:%d, pM:%d\n", pid, filePath, pesMode);

                //if(pid > 8192 || filePath[0] == 0)
                if(pid > 8192)
                {
                    eLIBs_printf("param error.\n");
                    break;
                }
                if(pid == 8192)
                {
                    eLIBs_printf("pid = 8192, pass all packet!\n");
                }
                
                start_record(pid, pesMode, (const char*)filePath, tscDrv);                    
                    
                break;
                
            case CMD_STOP_RECORD:
                
                pid     = 0;
                
                parse_pid(str, &pid);
                
                if(pid > 8192)
                {
                    eLIBs_printf("param error.\n");
                    break;
                }
                
                stop_record(pid);
                break;
                
            default:
                break;
        }

        //* delay 500 ms.
        esKRNL_TimeDly(50);
    }
    
_exit_test:
    //* close tsc driver.
    if(tscDrv)
    {
        esMODS_MClose(tscDrv);
        tscDrv = NULL;
        eLIBs_printf("test program close tsc driver.\n");
    }

    //* close demod driver.
    if(demodDrv)
    {
        esMODS_MClose(demodDrv);
        demodDrv = NULL;
        eLIBs_printf("test program close demod driver.\n");
    }
    
    
    //* uninstall demod driver.
    if(demodMid)
    {
        esMODS_MUninstall(demodMid);
        demodMid = 0;
        eLIBs_printf("test program uninstall demod driver.\n");
    }
    
    //* uninstall tsc driver
    if(tscMid)
    {
        esMODS_MUninstall(tscMid);
        tscMid = 0;
        eLIBs_printf("test program uninstall tsc driver.\n");
    }

    //* delete task.
    eLIBs_printf("test program exit.\n");
    esKRNL_TDel(OS_PRIO_SELF);
    
    return 0;
}

