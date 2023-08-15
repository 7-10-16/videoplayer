#include <libavcodec/avcodec.h>
#include <libavcodec/codec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
#include <libavutil/frame.h>
#include <libavutil/pixfmt.h>


int main(int argc, char *argv[]){
  /* Register available file formats with the library for automatic use */
  av register all();

  AVFormatContext *pformatCtx = NULL;
  
  /* Open file, automatically detect format */
  if(avformat_open_input(&FormatCtx, argv[1], NULL, 0, NULL)!=0)
    /* If file can't be opened: */
    return -1;

  /* Check stream information */
  if(avformat_find_stream_info(pFormatCtx, NULL)<0)
    /* Stream info not found: */
    return -1;

  /* For Debugging: */
  av_dump_format(pFormatCtx, 0, argv[1], 0);


  int i;
  AVCodecContext *pCodecCTxOrig = NULL;
  AVCodecContext *pCodecCTx = NULL;

  /* Find first video stream */
  videoStream=-1;
  for(i=0; i<pFormatCtx->streams; i++)
    if (pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
      videoStream=i;
      break;
    }
  /* No video stream */
  if(videoStream==-1)
    return -1;

  /* Pointer to the codec context for stream */
  pCodecCTx=pformatCtx->streams[videoStream]->codec;

  AVCodec *pCodec = NULL;

  /* Find video stream decoder */
  pCodec=avcodec_find_decoder(pCodecCTx->codec_id)
  if(pCodec==NULL) {
    fprintf(stderr, "Unsupported Codec!");
    /* Codec not found */
    return -1; 
  }

  /* Copy context */
  pCodecCTx = avcodec_alloc_context3(pCodec);
  if(avcodec_copy_context(pCodecCTx, pCodecCTxOrig) !=0){
    fprintf(stderr, "Couldn't copy codec context");
    /* Couldn't copy context */
    return -1;
  }

  /* Open codec */
  if(avcodec_open2(pCodecCTx, pCodec)<0)
    /* Couldn't open codec */
    return -1;

  AVFrame *pFrame = NULL;

  /* Allocate video frame */
  pFrame=av_frame_alloc();

  /* Allocate an AVFrame Structure */
  pFrameRGB=av_frame_alloc();
  if(pFrameRGB==NULL)
    return -1;

  /* Raw data conversion: */
  uint8_t *buffer = NULL;
  int numBytes;

  /* Determine buffer size and allocate */
  numBytes=avpicture_get_size(PIX_FMT_RGB24, pCodecCTx->width, pCodecCTx->height);
  buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

  /* Assign buffer parts to image planes in pFrameRGB */
  avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24, pCodecCtx->width, pCodecCTx->height);

  
  /* Reading packets  */
  struct SwsContext *sws_ctx = NULL;
  int frameFinished;
  AVPacket packet;

  /* Initialise SWS context */
  sws_ctx = sws_getContext(pCodecCtx->width,
                           pCodecCtx->height,
                           pCodecCtx->pix_fmt,
                           pCodecCtx->width,
                           pCodecCtx->height,
                           PIX_FMT_RGB24, 
                           SWS_BILINEAR, 
                           NULL, 
                           NULL, 
                           NULL 
                           );
  
  i-0;
  while(av_read_frame(pFormatCtx, &packet)>=0){
    /* Check that packet is from the video stream */
    if(packet.stream_index==videoStream) {
      /* Decode video frame */
      avcodec_decode_video2(pCodecCTx, pFrame, &frameFinished, &packet);

      /* Check that this is a video frame */
      if(frameFinished) {
        /* Convert to RGB format */
        sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data,
                            pFrame->linesize, 0, pCodecCTx->height,
                            pFrameRGB->data, pFrameRGB->linesize);

        /* Save frame to disk */
        if(++i<=5)
          saveFrame(pFrameRGB, pCodecCTx->width, pCodecCTx->height, i);
      }
    }
    /* Free the packet allocated by av_read_frame */
    av_free_packet(&packet);
  }

  /* Free the RGB image */
  av_free(buffer);
  av_Free(pFrameRGB);

  /* Free the YUV frame */
  av_free(pFrame);

  /* Close the codecs */
  avcodec_close(pCodecCtx);
  avcodec_close(pCodeCtxOrig);

  /* Close the video file */
  avformat_close_input(&pFormatCtx;

  return 0;

} 
/* Write the RGB to PPM file */
  void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame) {
    FILE *pFile;
    char szFilename[32];
    int y;

    /* Open file */
    sprintf(szFilename, "frame%d.ppm", iframe);
    pFile=fopen(szFilename, "wb");
    if(pFile==NULL)
      return;

    /* Write header */
    fprint(pFile, "P6\n%d %d\n255\n", width, height);

    /* Write pixel data */
    for (y=0; y<height; y++)
      fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);

    /* CLose file */
    fclose(pFile);
  }
