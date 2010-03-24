#include <ltexture.h>

LTexture::LTexture(QObject *parent) :
    QObject(parent)
{
    this->textureName = 0;
    this->textureImage = NULL;
}

LTexture::LTexture(GLuint textureName, QString textureImageFileName, QObject *parent) :
         QObject(parent)
{
    this->textureName = textureName;
    this->textureImage = new QImage(textureImageFileName);
}

GLuint LTexture::getTextureName()
{
    return this->textureName;
}

void LTexture::setTextureName(GLuint tName)
{
    this->textureName = tName;
}

QImage *LTexture::getTextureImage()
{
    return this->textureImage;
}

void LTexture::setTextureImage(QImage *tImage)
{
    this->textureImage = tImage;
}

bool LTexture::createTextureImage(QString fileName)
{
    this->textureImage = new QImage(fileName);

    if(!this->textureImage->isNull())
    {
        return true;
    }
    else
    {
        return false;
    }
}

LTexture::~LTexture()
{
    delete this->textureImage;
    this->textureImage = NULL;
}

