//////////////////////////
////debug print file

#include "log_tf.h"

static ES_FILE* g_log_file = NULL;

static __s32 g_enable_dump_to_file = 1;

__s32 log_init(void)
{
    if(g_enable_dump_to_file)
    {
    	if(NULL == g_log_file)
    	{
    		g_log_file = eLIBs_fopen("f:\\log.txt", "ab+");
    	}

        if(NULL == g_log_file)
    	{
    		g_log_file = eLIBs_fopen("f:\\log.txt", "wb");
            if(g_log_file)
            {
                char pflag[]="--------------Begin---------------\n\n";
                eLIBs_fwrite(pflag, 1, eLIBs_strlen(pflag), g_log_file);
            }
    	}
    }
	
	return EPDK_OK;
}


__s32 log_deinit(void)
{
	if(g_log_file)
	{
		eLIBs_fclose(g_log_file);
		g_log_file = NULL;
	}
	
	return EPDK_OK;
}

static int log_vprintf(const char *format, va_list args)
{
    /* I think 1024 byte buffer space big enough, if you don't think so.
     * you can modify it if you kown what you are doing.
     * by sunny at 2010-11-13 17:21:36.
     */
    static char buffer[1024];
    int         rc;
    
    /* construct print format buffer */
    rc = eLIBs_vscnprintf(buffer, 1024, format, args);
    
    /* print formated buffer to output */
    //esSIOS_putstr(buffer);
    if(g_log_file)
	{
		eLIBs_fwrite(buffer, 1, eLIBs_strlen(buffer), g_log_file);
    }
    
    return rc;
}


__s32 log_printf(const char *format, ...)
{	
	va_list args;
    int     rc;

	

	va_start(args, format);
    rc = log_vprintf(format, args);
	va_end(args);

	log_flush();//ÿдһ��֮��ر��ļ����´򿪣���֤������Ϊ���������ԭ��TF�����ݶ�ʧ��

	return EPDK_OK;
	
}

__s32 log_flush(void)
{
    if(g_log_file)
    {
      #if 0
        eLIBs_fsync(g_log_file);
#else

	{
		eLIBs_fclose(g_log_file);
		g_log_file = eLIBs_fopen("f:\\log.txt", "ab+");
	}
#endif
    }
    return EPDK_OK;
}

__s32 eLIBs_log_set_dump_flag(__s32 flag)
{
    g_enable_dump_to_file = flag;
    if(0 == flag)
    {
        log_deinit();
    }
    else
    {
        log_init();
    }
    return EPDK_OK;
}

///end debug print file
///////////////////////////////


///DEMO
//log_init();
///log_printf("Test=%d\n", 20);
//log_printf("Test=%d\n", 18);
//log_printf("Test=%d\n", 17);
//log_flush();
//log_deinit();//���һ��Ҫ��deinit������������ڻ�������������ԭ��ֱ�ӵ��磬û�е���deinit�������ݱ��治ȫ������취��ÿ��ӡһ��֮ǰinit����ӡһ��֮��deinit������Ūһ���ӿڡ���



