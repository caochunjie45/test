/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, kevn China
*                                              All Rights Reserved
*
* File   : arec.h
* Version: V1.0
* By     : kevin.z
* Date   : 2009-3-22 10:57
*********************************************************************************************************
*/
#ifndef _AREC_H_
#define _AREC_H_

typedef enum __AUDIO_RECORDER_COMMAND
{
    GINKGO_AREC_CMD_START=0x00,         /* start audio recorder                 */
    GINKGO_AREC_CMD_STOP,               /* stop audio recorder                  */
    GINKGO_AREC_CMD_PAUSE,              /* pause audio recorder                 */
    GINKGO_AREC_CMD_CONTI,              /* continue audio recorder              */

    GINKGO_AREC_CMD_SET_PCMSKT,         /* set pcm output socket                */
    GINKGO_AREC_CMD_SET_AUDIOINF,       /* set audio information for record     */
    GINKGO_AREC_CMD_SET_MSGQ,           /* set message queue                    */

    GINKGO_AREC_CMD_SET_SPECTRUM,       /* set spectrum                         */
    GINKGO_AREC_CMD_GET_SPECTRUM,       /* get spectrum                         */

    GINKGO_AREC_CMD_GET_TIME,          /* get time (ms)                            */

    GINKGO_AREC_CMD_SET_AUDIOGAIN, //aux = -20 ~ 20dB,不查错了

    GINKGO_AREC_CMD_FLUSH_BUF,  //清adcbuf

    GINKGO_AREC_CMD_

} __audio_recorder_command_t;


#endif /*_ARDR_H_*/
