#include <libavcodec/avcodec.h>
#include <libavcodec/codec.h>
#include <libavformat/avformat.h>
#include <ffmpeg/swscale.h>
#include <libavutil/avutil.h>
#include <libavutil/pixfmt.h>


int main(int argc, charg *argv[]
{
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
}
