#ifndef FFVIDEOTHREAD_H
#define FFVIDEOTHREAD_H

#include <QThread>
#include "ffmpegreader.h"

class FFVideoThread : public QThread
{
protected:
    virtual void run();
public:
    FFVideoThread(FFmpegReader *ffMpegReader, QObject *parent = 0);

    bool isPlaying;
private:
    FFmpegReader *ffMpegReader;
    //value for seek
    double step;
};

#endif // FFVIDEOTHREAD_H
