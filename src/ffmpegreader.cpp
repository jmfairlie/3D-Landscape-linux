#include "ffmpegreader.h"

FFmpegReader::FFmpegReader(QString filename, QWidget *parent)
{
    this->buffer = NULL;
    this->pFrameRGB = NULL;

    this->pFrame = NULL;

    this->pCodecCtx = NULL;

    this->isOpened = false;

    this->aCodecCtx = NULL;

    this->pFormatCtx = NULL;
    this->filename = filename;

    // Register all formats and codecs
    av_register_all();

    this->noAudio = false;

    this->frameDuration = 0.04f;
}

PacketQueue FFmpegReader::audQ;

//open file
bool FFmpegReader::open()
{
    this->noAudio = false;

    //Opens a media file as input. The codecs are not opened. Only the file
    //header (if present) is read.
    if(av_open_input_file(&(this->pFormatCtx), this->filename.toStdString().c_str(), NULL, 0, NULL)!=0)
    {
        printf("File not opened\n");
        return false; // Couldn't open file
    }

    // Retrieve stream information
    if(av_find_stream_info(this->pFormatCtx)<0)
        return false; // Couldn't find stream information    

    // Dump information about file onto standard error
    dump_format(this->pFormatCtx, 0, this->filename.toStdString().c_str(), 0);

    // Find the first video stream
    this->videoStream = -1;

    //find the first audio stream
    this->audioStream = -1;

    for(u_int i = 0; i < this->pFormatCtx->nb_streams; i++)
    {
      if(this->pFormatCtx->streams[i]->codec->codec_type == CODEC_TYPE_VIDEO)
      {
        this->frameDuration = (float)this->pFormatCtx->streams[i]->time_base.num / (float)this->pFormatCtx->streams[i]->time_base.den;
        printf("frame duration: %f\n", this->frameDuration);

        this->videoStream = i;
      }
      else if(this->pFormatCtx->streams[i]->codec->codec_type == CODEC_TYPE_AUDIO)
      {
        this->audioStream = i;
      }
    }

    if(this->videoStream == -1)
        return false; // Didn't find a video stream


    // Get a pointer to the codec context for the video stream
    this->pCodecCtx = this->pFormatCtx->streams[this->videoStream]->codec;     

    // Get a pointer to the codec context for the audio stream
    if(this->audioStream != -1)
    {
        this->aCodecCtx = this->pFormatCtx->streams[this->audioStream]->codec;

        // Find the decoder for the audio stream
        this->aCodec = avcodec_find_decoder(this->aCodecCtx->codec_id);

        if(!aCodec)
        {
            fprintf(stderr, "Unsupported audio codec!\n");
            this->noAudio = true;
        }
        else
        {
            //open audio codec
            if(avcodec_open(aCodecCtx, aCodec) < 0)
            {
                this->noAudio = true;
            }

            //set the audio structure
            if(!this->noAudio)
            {
                wanted_spec.freq = this->aCodecCtx->sample_rate;
                wanted_spec.format = AUDIO_S16SYS;
                wanted_spec.channels = this->aCodecCtx->channels;
                wanted_spec.silence = 0;
                wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE;
                wanted_spec.callback = FFmpegReader::audioCallback;
                wanted_spec.userdata = this->aCodecCtx;

                if(SDL_OpenAudio(&wanted_spec, &spec) < 0)
                {
                  fprintf(stderr, "SDL_OpenAudio: %s\n", SDL_GetError());
                  this->noAudio = true;
                }

                if(!this->noAudio)
                {
                    packet_queue_init(&FFmpegReader::audQ);
                    SDL_PauseAudio(0);

                    //printf("YO!!!\n");
                }
            }
        }

    }
    else
    {

        fprintf(stderr, "no audio stream!\n");
        this->noAudio = true;
    }

    // Find the decoder for the video stream
    this->pCodec = avcodec_find_decoder(this->pCodecCtx->codec_id);

    if(this->pCodec == NULL)
    {
        fprintf(stderr, "Unsupported video codec!\n");
        return false; // Codec not found
    }     

    // Open codec
    if(avcodec_open(this->pCodecCtx, this->pCodec) < 0)
        return false; // Could not open codec

    // Allocate video frame
    this->pFrame = avcodec_alloc_frame();

    // Allocate an AVFrame structure
    this->pFrameRGB=avcodec_alloc_frame();
    if(this->pFrameRGB==NULL)
        return false;

    // Determine required buffer size and allocate buffer
    this->numBytes = avpicture_get_size(PIX_FMT_RGB24, this->pCodecCtx->width,
                              this->pCodecCtx->height);   

    this->buffer = (uint8_t *)av_malloc((this->numBytes)*sizeof(uint8_t));

    // Assign appropriate parts of buffer to image planes in pFrameRGB
    // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
    // of AVPicture
    avpicture_fill((AVPicture *)this->pFrameRGB, this->buffer, PIX_FMT_RGB24,
                 this->pCodecCtx->width, this->pCodecCtx->height);


    QString num;
    QString header("P6\n");
    header.append(num.setNum(this->pCodecCtx->width));
    header.append(' ');
    header.append(num.setNum(this->pCodecCtx->height));
    header.append("\n255\n");

    this->ppmHeader = header.toStdString();

    //allocate BUFFSIZE buffers
    unsigned char *currImg;
    for(int i = 0; i < BUFFSIZE; i++)
    {
        currImg = new unsigned char[3*this->pCodecCtx->width*this->pCodecCtx->height + this->ppmHeader.size()];
        strcpy((char*)currImg, this->ppmHeader.c_str());
        this->buffers.enqueue(currImg);
    }

    printf("ALL oK!!!\n");

    return true;
}

void FFmpegReader::seek(double step)
{    
    int64_t TARGET_PTS = step * AV_TIME_BASE;

    int flags = step < 0 ? AVSEEK_FLAG_BACKWARD : 0;

    av_seek_frame(this->pFormatCtx, -1, TARGET_PTS, flags);
}

bool FFmpegReader::readFrame()
{
    //finished
    if(av_read_frame(pFormatCtx, &packet) < 0)
        return false;

    // Is this a packet from the video stream?
    if(this->packet.stream_index == this->videoStream)
    {
      int iRes = avcodec_decode_video2(this->pCodecCtx, this->pFrame, &this->frameFinished,
                           &this->packet);
      if(iRes < 0)
      {
          av_free_packet(&(this->packet));
          return false;
      }
      if(iRes == 0)
      {
          av_free_packet(&(this->packet));
          return true;
      }

      // Did we get a video frame?
      if(this->frameFinished)
      {
        // Convert the image from its native format to RGB

        static int sws_flags = SWS_BICUBIC;
        struct SwsContext *img_convert_ctx;
        img_convert_ctx = sws_getContext(this->pCodecCtx->width,
            this->pCodecCtx->height,
            this->pCodecCtx->pix_fmt,
            this->pCodecCtx->width,
            this->pCodecCtx->height,
            PIX_FMT_RGB24,
            sws_flags, NULL, NULL, NULL);

        sws_scale(img_convert_ctx, this->pFrame->data, this->pFrame->linesize,
                0, this->pCodecCtx->height,
                this->pFrameRGB->data, this->pFrameRGB->linesize);
        sws_freeContext(img_convert_ctx);

        //Save the frame to QImage
        this->paintFrame();
      }
      // Free the packet that was allocated by av_read_frame
      av_free_packet(&(this->packet));
    }
    else if((this->packet.stream_index == this->audioStream) && (!this->noAudio))
    {
      /*int audio_buf_size;
      int len;

      this->audio_full_buff.clear();

      while (this->packet.size > 0)
      {
         audio_buf_size = sizeof(this->audio_buf);

         len = avcodec_decode_audio3(aCodecCtx, this->audio_buf,
            &audio_buf_size, &packet);

         if(len < 0) break;

         packet.data += len;
         packet.size -= len;

         for(int i = 0; i < audio_buf_size; i++)
         {
             this->audio_full_buff.append(audio_buf[i]);
         }
      }*/
        packet_queue_put(&FFmpegReader::audQ, &this->packet);
        //we do not free the packet here. It will be done later.
    }

    return true;
}

void FFmpegReader::paintFrame()
{
    //printf("Tex size: %d\n", this->textures.size());
    if(this->textures.size() < BUFFSIZE)
    {
        unsigned char *bufferC = this->buffers.dequeue();

        memcpy(&bufferC[this->ppmHeader.size()], this->pFrameRGB->data[0],
            this->pCodecCtx->width*this->pCodecCtx->height*3);

        QImage textureFrameC = QImage::fromData(bufferC, this->pCodecCtx->width*this->pCodecCtx->height*3 + this->ppmHeader.size());

        this->buffers.enqueue(bufferC);

        if(!textureFrameC.isNull() /*&& this->textures.size() < BUFFSIZE*/)
            this->textures.enqueue(textureFrameC);
    }
}

void FFmpegReader::close()
{
  // Free the RGB image
  if(this->buffer)
    av_free(this->buffer);
  if(this->pFrameRGB)
    av_free(this->pFrameRGB);

  // Free the YUV frame
  if(this->pFrame)
    av_free(this->pFrame);

  // Close the codec
  if(this->pCodecCtx)
    avcodec_close(this->pCodecCtx);

  //Close audio codec
  if(!this->noAudio && this->aCodecCtx)
      avcodec_close(this->aCodecCtx);

  // Close the video file
  if(this->pFormatCtx)
    av_close_input_file(this->pFormatCtx);

  //close sound device
  SDL_CloseAudio();

  this->isOpened = false;
}

FFmpegReader::~FFmpegReader()
{
    if(this->isOpened)
        this->close();

    while(!this->buffers.isEmpty())
    {
        delete [] this->buffers.dequeue();
    }
}

//SDL Queue initialization
void FFmpegReader::packet_queue_init(PacketQueue *q)
{
  memset(q, 0, sizeof(PacketQueue));
  q->mutex = SDL_CreateMutex();
  q->cond = SDL_CreateCond();
}

//enqueue packet to the SDL packet queue
int FFmpegReader::packet_queue_put(PacketQueue *q, AVPacket *pkt)
{
  AVPacketList *pkt1;
  if(av_dup_packet(pkt) < 0)
  {
    return -1;
  }

  pkt1 = (AVPacketList*)av_malloc(sizeof(AVPacketList));

  if (!pkt1)
    return -1;

  pkt1->pkt = *pkt;
  pkt1->next = NULL;

  SDL_LockMutex(q->mutex);

  if (!q->last_pkt)
    q->first_pkt = pkt1;
  else
    q->last_pkt->next = pkt1;

  q->last_pkt = pkt1;
  q->nb_packets++;

  q->size += pkt1->pkt.size;

  SDL_CondSignal(q->cond);

  SDL_UnlockMutex(q->mutex);
  return 0;
}

//get packet from queue
int FFmpegReader::packet_queue_get(PacketQueue *q, AVPacket *pkt, int block)
{
  AVPacketList *pkt1;
  int ret;

  SDL_LockMutex(q->mutex);

  for(;;)
  {
    pkt1 = q->first_pkt;

    if (pkt1)
    {
      q->first_pkt = pkt1->next;

      if (!q->first_pkt)
        q->last_pkt = NULL;

      q->nb_packets--;
      q->size -= pkt1->pkt.size;
      *pkt = pkt1->pkt;
      av_free(pkt1);
      ret = 1;
      break;
    }
    else if (!block)
    {
      ret = 0;
      break;
    }
    else
    {
      SDL_CondWait(q->cond, q->mutex);
    }
  }
  SDL_UnlockMutex(q->mutex);
  return ret;
}

//stream must be filled with audio data
void FFmpegReader::audioCallback(void *userdata, Uint8 *stream, int len)
{
  AVCodecContext *aCodecCtx = (AVCodecContext *)userdata;
  int len1, audio_size;

  static uint8_t audio_buf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2];
  static unsigned int audio_buf_size = 0;
  static unsigned int audio_buf_index = 0;

  while(len > 0)
  {
    if(audio_buf_index >= audio_buf_size)
    {
      /* We have already sent all our data; get more */
      audio_size = audio_decode_frame(aCodecCtx, audio_buf,
                                      sizeof(audio_buf));
      if(audio_size < 0)
      {
        /* If error, output silence */
        audio_buf_size = 1024;
        memset(audio_buf, 0, audio_buf_size);
      }
      else
      {
        audio_buf_size = audio_size;
      }
      audio_buf_index = 0;
    }
    len1 = audio_buf_size - audio_buf_index;
    if(len1 > len)
      len1 = len;
    memcpy(stream, (uint8_t *)audio_buf + audio_buf_index, len1);
    len -= len1;
    stream += len1;
    audio_buf_index += len1;
  }
}

int FFmpegReader::audio_decode_frame(AVCodecContext *aCodecCtx, uint8_t *audio_buf,
                       int buf_size)
{
  static AVPacket pkt;
  static uint8_t *audio_pkt_data = NULL;
  static int audio_pkt_size = 0;

  int len1, data_size;

  for(;;)
  {
    while(audio_pkt_size > 0)
    {
      data_size = buf_size;
      len1 = avcodec_decode_audio2(aCodecCtx, (int16_t *)audio_buf, &data_size,
                                  audio_pkt_data, audio_pkt_size);
      if(len1 < 0)
      {
        /* if error, skip frame */
        audio_pkt_size = 0;
        break;
      }
      audio_pkt_data += len1;
      audio_pkt_size -= len1;
      if(data_size <= 0)
      {
        /* No data yet, get more frames */
        continue;
      }
      /* We have data, return it and come back for more later */
      return data_size;
    }

    if(pkt.data)
      av_free_packet(&pkt);

    //first call starts from here
    //now we are not blocking (third parameter is 0)
    //and if there are no audio packets in the queue we are returning -1
    //in this case we will output silence to the audio device
    if(packet_queue_get(&FFmpegReader::audQ, &pkt, 0) <= 0)
    {
      return -1;
    }
    audio_pkt_data = pkt.data;
    audio_pkt_size = pkt.size;
  }
}


