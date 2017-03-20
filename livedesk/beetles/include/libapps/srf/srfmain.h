/* 
 **********************************************************************************
 *                             name: srfmain.h             
 *
 *                             function:gbktoutf8 & srf
 *							
 *							   version:3.0
 *
 *						       Author: CX@AL2, 28-02-2011
 ************************************************************************************
 */

#define SRF_PATH  BEETLES_APP_ROOT"apps\\srf\\"
#define SRFINSIZE 512//����Ķ�512����֤����ӿڵ�ƴ������ĸ����С��32
#define SRFOUTSIZE 10240//����Ķ�10240����֤����ӿڵ�ƴ������ĸ����С��32


typedef struct
{
int   srfmode; //0��ʾƴ�����뷨��1��ʾT9�������뷨��2��ʾT9��Ч���뷨��3��ʾ�ʻ����뷨��Ĭ��Ϊ0.                     
int   jiantiorfanti; //���廹�Ƿ��壬0��ʾ���壬1��ʾ���塣Ĭ��Ϊ0��
int   cikuuse;        //�Ƿ�ʹ���û��ʿ⣬0��ʾʹ�ã�1��ʾ��ʹ�á�Ĭ��Ϊ0,����������Ҳ���������û����뵽�û��ʿ��С�
int   wordsassociation; //�Ƿ�֧�����빦�ܣ�0��ʾ֧�֣�1��ʾ��֧�֡�Ĭ��Ϊ0.
int   otherset ;//δ�����ܻḽ���ϵ����ã�����Ϊ0��Ϊ0��ʾû���������á�
} SRF_SET_T;


typedef struct
{
SRF_SET_T    *srf_set; //�û�����
unsigned char    inputstring[SRFINSIZE]; //���뷨������ַ���
unsigned char    inputstringlen;//����ʵ�ʳ���
int   inputstringmore[SRFINSIZE]; //������������ݣ�����T9��ƴ������T9�����ִ���inputstring�д�š�  ��СΪ32.
unsigned char    inputstringmorelen;
} SRFINPUT_T;


typedef struct
{
unsigned char    outputstring[SRFOUTSIZE];//�����������Ϣ��ÿ��ƴ������Ӧһ�����ִ�����ÿ��ƴ���������֮ǰ��һ��ֵ��ʾ��������ɶ��ٸ�ƴ���ó��ġ�����shenmesh �����8shenmeshiʲô��  6shenme ʲô����Ȼ����ֵ��һ��CHARֵ��С�����볤��32��SRFOUTSIZE��СΪ5000.                                   
int    outputstringlen;   //�������                                 
unsigned char    outputstringmore[SRFOUTSIZE];//����һЩ�����������Ϣ��������outputstring�������𣬽���������������纺�ִ���T9�ж�Ӧ�����ִ���
int    outputstringmorelen;
} SRFOUTPUT_T;



//���뷨�ӿ�
int srf_main(SRFINPUT_T  *srf_input,SRFOUTPUT_T  *srf_output);

void srf_gbk2utf8(unsigned char *gbktxt,unsigned char *utf8txt); //gbkתutf8
//gb2utf.txt�ļ�·����BEETLES_APP_ROOTapps\\gb2utf.txt

