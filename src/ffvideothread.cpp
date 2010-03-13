#include "ffvideothread.h"

FFVideoThread::FFVideoThread(FFmpegReader *ffMpegReader, QObject *parent)
{
    this->ffMpegReader = ffMpegReader;
    this->step = 0.0;
    this->isPlaying = true;
}

void FFVideoThread::run()
{
    QThread::setTerminationEnabled();

    this->ffMpegReader->isOpened = this->ffMpegReader->open();

    while(true)
    {
        if(!this->isPlaying || !this->ffMpegReader->isOpened)
        {
            this->ffMpegReader->close();
            return;
        }
        if(this->ffMpegReader->textures.size() < BUFFSIZE)
        {
            if(!this->isPlaying || !this->ffMpegReader->readFrame())
            {
                this->ffMpegReader->close();
                return;
            }
        }

        //printf("Thread still running!!!\n");
    }

    //exec();
}
