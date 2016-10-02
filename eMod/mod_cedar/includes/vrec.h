/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                              mod_mmp sub-system
*
*                                     (c) Copyright 2008-2009, kevn China
*                                              All Rights Reserved
*
* File   : vrec.h
* Version: V1.0
* By     : kevin.z
* Date   : 2009-3-26 10:54
*********************************************************************************************************
*/
#ifndef _VREC_H_
#define _VREC_H_

typedef enum __VIDEO_RECORDER_COMMAND
{
    GINKGO_VREC_CMD_START=0x00,         /* start video recorder                 */
    GINKGO_VREC_CMD_STOP,               /* stop video recorder                  */
    GINKGO_VREC_CMD_PAUSE,              /* pause video recorder                 */
    GINKGO_VREC_CMD_CONTI,              /* continue video recorder              */

    GINKGO_VREC_CMD_SET_PICSKT=0x20,    /* set picture socket                   */
    GINKGO_VREC_CMD_SET_MSGQ,           /* set message queue                    */

    GINKGO_VREC_CMD_

} __video_recorder_command_t;


#endif /*_VREC_H_*/
