

#ifndef __DTV_FREQUENCY_H__
#define __DTV_FREQUENCY_H__



#include "dtv_i.h"




typedef struct frequencys_s   
{
    __u32 StartFreq;            /*Start Transponder Frequency*/
	
    __u32 BandWidth;            /*Transponder Bandwidth*/
    
    
}frequencys_t;

typedef struct area_frequencys_s
{
    __u32                     CountryIndex;
	
    const frequencys_t        *frequencys;
		
    __u32                     frequency_total;
	
    char                      CountryCode[3];
	
}area_frequencys_t;



extern area_frequencys_t area_frequencys_table[2];


#endif
