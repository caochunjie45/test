
#ifndef CRC_H
#define CRC_H

__u32 Crc32Compute(__u32 initVector, __u8* pData, __u32 dataLen);

__u32 Crc8Compute(__u8 initVector, __u8* pData, __u32 dataLen);

#endif

