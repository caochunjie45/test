
#include "pgs_s.h"
#include "pgs.h"

//#define DEBUG_PACKET_CONTENTS

#define RGBA(r,g,b,a) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b))

enum SegmentType 
{
    PALETTE_SEGMENT      = 0x14,
	PICTURE_SEGMENT      = 0x15,
	PRESENTATION_SEGMENT = 0x16,
	WINDOW_SEGMENT       = 0x17,
	DISPLAY_SEGMENT      = 0x80,
};

static __inline __u32 bytestream_get_byte(__u8** pptr)
{
	__u32 value;
	__u8* ptr;

	ptr = *pptr;

	value = *ptr++;

	*pptr = ptr;
	return value;
}

static __inline __u32 bytestream_get_be16(__u8** pptr)
{
	__u32 value;
	__u8* ptr;
	
	ptr = *pptr;
	
	value = ptr[0]<<8 | ptr[1];
	ptr += 2;
	
	*pptr = ptr;
	return value;
}

static __inline __u32 bytestream_get_be24(__u8** pptr)
{
	__u32 value;
	__u8* ptr;
	
	ptr = *pptr;
	
	value = ptr[0]<<16 | ptr[1]<<8 | ptr[2];
	ptr += 3;
	
	*pptr = ptr;
	return value;
}

#if 0
static __inline __u32 bytestream_get_be32(__u8** pptr)
{
	__u32 value;
	__u8* ptr;
	
	ptr = *pptr;
	
	value = ptr[0]<<24 | ptr[1]<<16 | ptr[2]<<8 | ptr[3];
	ptr += 4;
	
	*pptr = ptr;
	return value;
}
#endif

#if !USE_16_COLOR_PALLETE
static int decode_rle(__u8* data, __u32 width, __u32 height, __u8 *buf, __u32 buf_size, __u32* colorTab)
#else
static int decode_rle(__u8* data, __u32 width, __u32 height, __u8 *buf, __u32 buf_size, __u8* colorTab)
#endif
{
    __u8*  rle_bitmap_end;
    __u32  pixel_count;
	__u32  line_count;
	__u32  flags;
	__u32  color;
	__s32  run;
	__u32  total_pxl_cnt;
	__s32  k;
#if !USE_16_COLOR_PALLETE
	__u32* rgbData;
#else
	__u8*  rgbData;
#endif
	
    rle_bitmap_end	= buf + buf_size;
    pixel_count		= 0;
    line_count		= 0;
	total_pxl_cnt   = width*height;

	if (total_pxl_cnt > PGS_FRAME_SIZE)
	{
		WARNING("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx pixel cnt exceed, width = %d, height = %d. xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", width, height);
		return -1;
	}
	
    while (buf < rle_bitmap_end && line_count < height) 
	{
        color = bytestream_get_byte(&buf);
        run   = 1;
		
        if (color == 0x00) 
		{
            flags = bytestream_get_byte(&buf);
            run   = flags & 0x3f;
            if (flags & 0x40)
                run = (run << 8) + bytestream_get_byte(&buf);

            color = flags & 0x80 ? bytestream_get_byte(&buf) : 0;
        }
		
        if (run > 0 && (pixel_count + run <= total_pxl_cnt)) 
		{
#if !USE_16_COLOR_PALLETE
			rgbData = (__u32*)(data + (pixel_count<<2));
			k = run;
			while(--k >= 0)
				*rgbData++ = colorTab[color];
#else
			rgbData = data + pixel_count;
			k = run;
            while(--k >= 0)
				*rgbData++ = colorTab[color];
#endif
            pixel_count += run;
        } 
		else if (!run) 
		{
			/*
			 * New Line. Check if correct pixels decoded, if not display warning
			 * and adjust bitmap pointer to correct new line position.
			 */
            if (pixel_count % width > 0)
                WARNING("Decoded %d pixels, when line should be %d pixels\n", pixel_count % width, width);
            line_count++;
        }
    }
	
    return 0;
}

static __inline __s32 decode_one_subtitle(PGSSubContext *ctx, frm_info_t* fb)
{
    fb->startDispTime = 0;
    fb->endDispTime   = 5000;
	fb->x		      = ctx->presentation.x;
	fb->y		      = ctx->presentation.y;
	fb->width	      = ctx->picture.w;
	fb->height	      = ctx->picture.h;
	fb->videoWidth	  = ctx->presentation.video_w;
	fb->videoHeight   = ctx->presentation.video_h;
	
    if (ctx->picture.rle)
	{
        if(decode_rle(fb->data, fb->width, fb->height, ctx->picture.rle, ctx->picture.rle_data_len, ctx->clut) < 0)
            return 0;

		ctx->picture.rle_data_len = 0;
	}
		
	return 1;
}


static __inline __s32 parse_picture_segment(PGSSubContext* ctx, __u8* buf, __u32 buf_size)
{
    __u8  sequence_desc;
    __u32 rle_bitmap_len;
	__u32 width;
	__u32 height;
	
    /* skip 3 unknown bytes: Object ID (2 bytes), Version Number */
    buf += 3;
	
    /* Read the Sequence Description to determine if start of RLE data or appended to previous RLE */
    sequence_desc = bytestream_get_byte(&buf);
	
    if (!(sequence_desc & 0x80)) 
	{
        WARNING("Decoder does not support object data over multiple packets.\n");
        return -1;
    }
	
    /* Decode rle bitmap length */
    rle_bitmap_len = bytestream_get_be24(&buf);
	
    /* Check to ensure we have enough data for rle_bitmap_length if just a single packet */
    if (rle_bitmap_len > buf_size - 7) 
	{
        WARNING("Not enough RLE data for specified length of %d.\n", rle_bitmap_len);
        return -1;
    }
	
    ctx->picture.rle_data_len = rle_bitmap_len;
	
    /* Get bitmap dimensions from data */
    width  = bytestream_get_be16(&buf);
    height = bytestream_get_be16(&buf);
	
    /* Make sure the bitmap is not too large */
    if (ctx->presentation.video_w < width || ctx->presentation.video_h < height) 
	{
        WARNING("Bitmap dimensions larger then video.\n");
        return -1;
    }
	
    ctx->picture.w = width;
    ctx->picture.h = height;
	
	if (rle_bitmap_len > PGS_MAX_RLE_SIZE)
		return -1;
	
    memcpy(ctx->picture.rle, buf, rle_bitmap_len);
	
    return 0;
}


#define YUV_TO_RGB1(cb1, cr1)\
{\
    cb = (cb1) - 128;\
    cr = (cr1) - 128;\
    r_add = 1436 * cr + 512;\
    g_add = - 352 * cb - 732 * cr + 512;\
    b_add = 1815 * cb + 512;\
}

#define YUV_TO_RGB2(r, g, b, y1)\
{\
    y = (y1) << 10;\
    r = cm[(y + r_add) >> 10];\
    g = cm[(y + g_add) >> 10];\
    b = cm[(y + b_add) >> 10];\
}

static void parse_palette_segment(PGSSubContext* ctx, __u8* buf, __s32 buf_size)
{
	__u8* buf_end;
	__s32 color_id;
	__s32 alpha;
	__s32 y;
#if !USE_16_COLOR_PALLETE
	__s32 cb;
	__s32 cr;
	__s32 r;
	__s32 g;
	__s32 b;
	__s32 r_add;
	__s32 g_add;
	__s32 b_add;
    __u8* cm;
#endif

    buf_end = buf + buf_size;

#if !USE_16_COLOR_PALLETE
	cm      = ctx->cropTbl + MAX_NEG_CROP;
#endif
	
    /* Skip two null bytes */
    buf += 2;

	memset(ctx->clut, 16, sizeof(ctx->clut));
	
    while (buf < buf_end) 
	{
        color_id  = bytestream_get_byte(&buf);
        y         = bytestream_get_byte(&buf);
#if !USE_16_COLOR_PALLETE
        cb        = bytestream_get_byte(&buf);
        cr        = bytestream_get_byte(&buf);
#else
        buf += 2;
#endif
        alpha     = bytestream_get_byte(&buf);
#if !USE_16_COLOR_PALLETE
        YUV_TO_RGB1(cb, cr);
        YUV_TO_RGB2(r, g, b, y);
		
        /* Store color in palette */
        ctx->clut[color_id] = RGBA(r,g,b,alpha);
#else
        /* Store color in palette */
		if(alpha == 0)
            ctx->clut[color_id] = 16;
        else
            ctx->clut[color_id] = (__u8)(y>>4);
#endif
    }
}


static void parse_presentation_segment(PGSSubContext *ctx, __u8* buf, __s32 buf_size)
{
	__u32 x;
	__u32 y;
	__u8  block;
	
    ctx->presentation.video_w = bytestream_get_be16(&buf);
    ctx->presentation.video_h = bytestream_get_be16(&buf);
	
    /* Skip 1 bytes of unknown, frame rate? */
    buf++;
	
    ctx->presentation.id_number = bytestream_get_be16(&buf);
	
    /* Next byte is the state. */
    block = bytestream_get_byte(&buf);;
    if (block == 0x80) 
	{
	/*
	* Skip 7 bytes of unknown:
	*     palette_update_flag (0x80),
	*     palette_id_to_use,
	*     Object Number (if > 0 determines if more data to process),
	*     object_id_ref (2 bytes),
	*     window_id_ref,
	*     composition_flag (0x80 - object cropped, 0x40 - object forced)
		*/
        buf += 7;
		
        x = bytestream_get_be16(&buf);
        y = bytestream_get_be16(&buf);
		
        /* TODO If cropping, cropping_x, cropping_y, cropping_width, cropping_height (all 2 bytes).*/
		
//        WARNING("Subtitle Placement x=%d, y=%d\n", x, y);
		
        if (x > ctx->presentation.video_w || y > ctx->presentation.video_h) 
		{
            WARNING("Subtitle out of video bounds. x = %d, y = %d, video width = %d, video height = %d.\n",
					x, y, ctx->presentation.video_w, ctx->presentation.video_h);
            x = 0; y = 0;
        }
		
        /* Fill in dimensions */
        ctx->presentation.x = x;
        ctx->presentation.y = y;

		ctx->blockId        = 0x80;
    } 
	else if (block == 0x00) 
	{
//	    WARNING("    block id = 0x0.\n");
	   /* TODO: Blank context as subtitle should not be displayed.
		*       If the subtitle is blanked now the subtitle is not
		*       on screen long enough to read, due to a delay in
		*       initial display timing.
		*/
		ctx->blockId        = 0x0;
    }
}

#if 0
static int display_end_segment(PGSSubContext *ctx, frm_info_t* fb, __u8* buf, __s32 buf_size)
{
    /*
	*      The end display time is a timeout value and is only reached
	*      if the next subtitle is later then timeout or subtitle has
	*      not been cleared by a subsequent empty display command.
	*/
    fb->startDispTime = 0;
    fb->endDispTime   = 20000;
	fb->x		      = ctx->presentation.x;
	fb->y		      = ctx->presentation.y;
	fb->width	      = ctx->picture.w;
	fb->height	      = ctx->picture.h;
	fb->videoWidth	  = ctx->presentation.video_w;
	fb->videoHeight   = ctx->presentation.video_h;
	
    if (ctx->picture.rle)
	{
        if(decode_rle(fb->data, fb->width, fb->height, ctx->picture.rle, ctx->picture.rle_data_len, ctx->clut) < 0)
            return 0;

		ctx->picture.rle_data_len = 0;
	}
		
	return 1;
}
#endif


PGSSubContext* pgs_dec_create(void)
{
#if !USE_16_COLOR_PALLETE
	__u32			i;
#endif
	PGSSubContext*	ctx;
	ctx = (PGSSubContext*) malloc(sizeof(PGSSubContext));
	if (ctx == NULL)
		return NULL;

	memset(ctx, 0, sizeof(PGSSubContext));
	
	
#if !USE_16_COLOR_PALLETE
    for(i=0; i<256; i++) 
		ctx->cropTbl[i + MAX_NEG_CROP] = i;

    for(i=0; i<MAX_NEG_CROP; i++) 
	{
        ctx->cropTbl[i] = 0;
        ctx->cropTbl[i + MAX_NEG_CROP + 256] = 255;
    }
#endif

    ctx->state = NEED_PRESENTATION_SEGMENT;
	return ctx;
}


__s32 pgs_dec_destroy(PGSSubContext** ppCtx)
{
	PGSSubContext* ctx;

	ctx = *ppCtx;
	if (ctx)
		free(ctx);

	return 0;
}

/*******************************************************************************
Function name: pgs_dec_decode
Description: 
    PGS字幕解一帧一般需要5个segment,空帧除外.本解码函数每次调用可以解多个segment.
    目前外围调用时,一般是一个segment调用一次.
Parameters: 
    
Return: 
    -1:解码失败
    0:正常解码一个或多个segment,但是一帧还未解完
    1:正常解码一个或多个segment,并且一帧正式解完.
    2:空帧
Time: 2011/10/19
*******************************************************************************/

__s32 pgs_dec_decode(PGSSubContext* pCtx, __u8* data, __u32 len, frm_info_t* fb, __u32 pts)
{
    __u8* data_end;
    __u8  segment_type;
    __s32 segment_length;
    __s32 ret;
    
    ret = 0;

    /* Ensure that we have received at a least a segment code and segment length */
    if (len < 3)
        return -1;
    
    data_end = data + len;
    
    if(pts != 0)
    {
        if(0 == pCtx->blockId && pts < pCtx->lastDecodedPts)
        {
            //主要目的是不以PALETTE_SEGMENT的pts为准,这是因为阿凡达AVATAR.2009.Blu-ray.DIY.1080P.AVC.DTSHDMA_D10_视频卡.m2ts这个文件,空字幕也会带PALETTE_SEGMENT，而PALETTE_SEGMENT的pts明显过小
            WARNING("blockId==0, empty PGS frame, but pts[%d] <= lastDecodedPts[%d], we ignore!\n", pts, pCtx->lastDecodedPts);
        }
        else
        {
            pCtx->lastDecodedPts = pts;
        }
    }
    /* Step through buffer to identify segments */
    while (data < data_end) 
    {
        segment_type   = bytestream_get_byte(&data);
        segment_length = bytestream_get_be16(&data);
        //WARNING("segment type[0x%x], length[%d], datalen[%d], pts[%d]\n", segment_type, segment_length, len, pts);
        if (segment_type != DISPLAY_SEGMENT && segment_length > data_end - data)
        {
            pCtx->state = NEED_PRESENTATION_SEGMENT;
            return -1;
        }
        
        switch (segment_type) 
        {
        case PRESENTATION_SEGMENT:
            {
                if(pCtx->state != NEED_PRESENTATION_SEGMENT)
                {
                    WARNING("state error: expected state %d but comes a presentation segment.\n", pCtx->state);
                }
                parse_presentation_segment(pCtx, data, segment_length);
                pCtx->state = NEED_WINDOW_SEGMENT;
                ret = 0;
                break;
            }

        case WINDOW_SEGMENT:
            {
                if(pCtx->state != NEED_WINDOW_SEGMENT)
                {
                    WARNING("state error: expected state %d but comes a window segment.\n", pCtx->state);
                    pCtx->state = NEED_PRESENTATION_SEGMENT;
                    return -1;
                }
                
                /*
                 * Window Segment Structure (No new information provided):
                 *     2 bytes: Unkown,
                 *     2 bytes: X position of subtitle,
                 *     2 bytes: Y position of subtitle,
                 *     2 bytes: Width of subtitle,
                 *     2 bytes: Height of subtitle.
                 */
#if 0
                 {
                    __u32 x_pos, y_pos, width, height;
                    if(len >= 13)
                    {
                        bytestream_get_be16(&data);
                        x_pos  = bytestream_get_be16(&data);
                        y_pos  = bytestream_get_be16(&data);
                        width  = bytestream_get_be16(&data);
                        height = bytestream_get_be16(&data);
                        WARNING("window segment: x_pos = %d, y_pos = %d, width = %d, height = %d.\n",
                                 x_pos, y_pos, width, height);
                    }
                 }
#endif
                 
                 if(pCtx->blockId == 0x80)
                    pCtx->state = NEED_PALLETE_SEGMENT;
                 else
                    pCtx->state = NEED_DISPLAY_SEGMENT;
                    
                ret = 0;
                break;
            }
            
        case PALETTE_SEGMENT:
            {
                if(pCtx->state != NEED_PALLETE_SEGMENT)
                {
//                    pCtx->state = NEED_PRESENTATION_SEGMENT;
//                    return -1;
                    if(pCtx->blockId == 0)
                    {
                        WARNING("state[%d]!=NEED_PALLETE_SEGMENT, blockId==0, meet Palette segment,continue...\n", pCtx->state);
                        parse_palette_segment(pCtx, data, segment_length);
                        ret = 0;
                        break;
                        //pCtx->blockId = 0x80;
                    }
                    else
                    {
                        WARNING("state error: expected state %d, blockId[0x%x] but comes a pallete segment.\n", pCtx->state, pCtx->blockId);
                        pCtx->state = NEED_PRESENTATION_SEGMENT;
                        return -1;
                    }
                }
                
                parse_palette_segment(pCtx, data, segment_length);
                pCtx->state = NEED_PICTURE_SEGMENT;
                
                ret = 0;
                break;
            }

        case PICTURE_SEGMENT:
            {
                if(pCtx->state != NEED_PICTURE_SEGMENT)
                {
                    if(pCtx->blockId == 0)
                    {
                        WARNING("state[%d]!=NEED_PICTURE_SEGMENT, blockId==0, meet Picture segment,continue...\n", pCtx->state);
                        WARNING("we need change blockId 0x0->0x80 now, because this is not empty frame!\n");
                        pCtx->blockId = 0x80;
                    }
                    else
                    {
                        WARNING("state error: expected state %d but comes a picture segment.\n", pCtx->state);
                        pCtx->state = NEED_PRESENTATION_SEGMENT;
                        return -1;
                    }
                }
                    
                if(parse_picture_segment(pCtx, data, segment_length) == 0)
                {
                    pCtx->pictureSegmentPts = pCtx->lastDecodedPts;
                    pCtx->state = NEED_DISPLAY_SEGMENT;
                    ret = 0;
                    break;
                }
                else
                {
                    pCtx->state = NEED_PRESENTATION_SEGMENT;
                    return -1;
                }
            }

        case DISPLAY_SEGMENT:
            {
                if(pCtx->state != NEED_DISPLAY_SEGMENT)
                {
                    WARNING("state error: expected state %d but comes a display segment.\n", pCtx->state);
                    pCtx->state = NEED_PRESENTATION_SEGMENT;
                    return -1;
                }
                
                if(pCtx->blockId == 0x80)
                {
                    if(decode_one_subtitle(pCtx, fb) == 1)
                    {
                        fb->uPTS = pCtx->pictureSegmentPts;
                        pCtx->state = NEED_PRESENTATION_SEGMENT;
                        return 1;
                    }
                    else
                    {
                        pCtx->state = NEED_PRESENTATION_SEGMENT;
                        return -1;
                    }
                }
                else
                {
                    fb->uPTS = pCtx->lastDecodedPts;
                    pCtx->state = NEED_PRESENTATION_SEGMENT;
                    //* return a display end notify.
                    return 2;
                }
            }

        default:
            WARNING("Unknown subtitle segment type 0x%x, length %d\n", segment_type, segment_length);
            return -1;
        }
        
        data += segment_length;
    }
    
    return ret;
}


__s32 pgs_dec_reset(PGSSubContext* pCtx)
{
    pCtx->state             = NEED_PRESENTATION_SEGMENT;
    pCtx->blockId           = 0;
    pCtx->lastDecodedPts    = 0;
    pCtx->pictureSegmentPts = 0;
    
    return 0;
}

