#ifndef FFMPEGREADER_H
#define FFMPEGREADER_H

#include <cstdio>

#include "definesall.h"

#include <QImage>
#include <QPixmap>
#include <QQueue>
#include <QList>

#include <SDL.h>
#include <SDL_thread.h>

#ifdef __MINGW32__
#undef main /* Prevents SDL from overriding main() */
#endif

#define SDL_AUDIO_BUFFER_SIZE 1024

#define BUFFSIZE 50

//headers to work with ffmpeg
extern "C"
{
    #include "./ffmpeg/libavcodec/avcodec.h"
    #include "./ffmpeg/libavformat/avformat.h"
    #include "./ffmpeg/libswscale/swscale.h"
}



typedef struct PacketQueue
{
  AVPacketList *first_pkt, *last_pkt;
  int nb_packets;
  int size;
  SDL_mutex *mutex;
  SDL_cond *cond;
} PacketQueue;


class FFmpegReader : public QObject
{
  Q_OBJECT

public:
    FFmpegReader(QString filename, QWidget *parent = 0);
    virtual ~FFmpegReader();

    //open file, find and open codec, find video stream, etc
    bool open();

    //if video file is opened
    bool isOpened;

    //closes file and frees the memory
    void close();

    // Reads current frame
    bool readFrame();

    //save frame to image
    void paintFrame();

    //seek a frame
    void seek(double step);

    //unsigned char *currImg;

    QQueue<QImage> textures;
    QQueue<unsigned char*> buffers;

    //QImage textureFrame;    
    //SDL queue of audio packets
    static PacketQueue audQ;

    float frameDuration;

private:
  //ffmpeg variables
  AVFormatContext *pFormatCtx;
  int             videoStream;
  int             audioStream;
  AVCodecContext  *pCodecCtx;
  AVCodec         *pCodec;
  AVFrame         *pFrame;
  AVFrame         *pFrameRGB;
  AVPacket        packet;
  int             frameFinished;
  int             numBytes;
  uint8_t         *buffer;

  //audio codec context
  AVCodecContext *aCodecCtx;
  //audio codec
  AVCodec        *aCodec;

  int16_t audio_buf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2];

  QVector<int16_t> audio_full_buff;

  //audio settings structure
  SDL_AudioSpec wanted_spec, spec;

  std::string ppmHeader;

  //=true, if there is no audio or problems loading audio codec
  bool noAudio;

  //name of the video file
  QString filename;  

  static void packet_queue_init(PacketQueue *q);
  static int packet_queue_put(PacketQueue *q, AVPacket *pkt);
  static int packet_queue_get(PacketQueue *q, AVPacket *pkt, int block);
  static void audioCallback(void *userdata, Uint8 *stream, int len);
  static int audio_decode_frame(AVCodecContext *aCodecCtx, uint8_t *audio_buf,
                       int buf_size);



signals:
     void framePainted(QImage frame);

};

#endif // FFMPEGREADER_H
