#ifndef LTEXTURE_H
#define LTEXTURE_H

#include <QObject>
#include <qgl.h>

class LTexture : public QObject
{
Q_OBJECT
public:
    explicit LTexture(QObject *parent = 0);
    LTexture(GLuint textureName, QString textureImageFileName, QObject *parent = 0);
    virtual ~LTexture();

signals:

public slots:

public:
    GLuint getTextureName();
    void setTextureName(GLuint tName);
    QImage *getTextureImage();
    void setTextureImage(QImage *tImage);
    //create QImage from file
    bool createTextureImage(QString fileName);

private:
    GLuint textureName;
    QImage *textureImage;

};

#endif // LTEXTURE_H
