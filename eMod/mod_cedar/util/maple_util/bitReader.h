
#ifndef BIT_READER_H
#define BIT_READER_H

typedef struct BITREADER
{
    __u8*   pbuf;
    __u32   bufSize;
    __u32   byteOffset;
    __u32   bitOffset;
}bit_reader_t;

//* return 0 means read bits success, return -1 means read bits failed.
__s32 BitReaderReadBits(bit_reader_t* pReader, __u32 numOfBits, __u32* val);

#endif

