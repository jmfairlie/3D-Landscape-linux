#include "qtextureloader.h"

QTextureLoader::QTextureLoader(QObject *parent) :
    QObject(parent)
{
    m_TextureFilterType = txBilinear;
}

bool QTextureLoader::LoadTextureFromDisk(QString fileName, glTexture *pglTexture)
{
    if (fileName.endsWith(".jpg") || fileName.endsWith(".jpeg"))
    {
            pglTexture->TexType = txJpg;

            cout << "jpg" << endl;

            if(this->buildTexture(fileName, pglTexture))
                return true;
            else
                return false;
    }
    else
    {
        return false;
    }   
}


bool QTextureLoader::buildTexture(QString fileName, glTexture *pglTexture)
{
    GLint   glMaxTexDim;
    //maximum supported texture size
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);

    QImage img(fileName);

    /*if(!img.load(fileName))
        cout << "LOADING FAILED for some reason" << endl;*/

    cout << fileName.toStdString() << endl;
    cout << QDir::currentPath().toStdString() << endl;

    if(img.isNull())
    {
        cout << "img is NULL" << endl;
        return false;
    }

    int picWidthPixels = img.width();
    int picHeightPixels = img.height();

    // Resize Image To Closest Power Of Two
    // Is Image Width Less Than Or Equal To Cards Limit
    if (picWidthPixels <= glMaxTexDim)
    {
        picWidthPixels = 1 << (int)floor((log((double)picWidthPixels)/log(2.0f)) + 0.5f);
    }
    // Otherwise  Set Width To "Max Power Of Two" That The Card Can Handle
    else
    {
        picWidthPixels = glMaxTexDim;
    }

    // Is Image Height Greater Than Cards Limit
    if (picHeightPixels <= glMaxTexDim)
    {
        picHeightPixels = 1 << (int)floor((log((double)picHeightPixels)/log(2.0f)) + 0.5f);
    }
    // Otherwise  Set Height To "Max Power Of Two" That The Card Can Handle
    else
    {
        picHeightPixels = glMaxTexDim;
    }

    img.scaled(picWidthPixels, picHeightPixels);

    pglTexture->Width = picWidthPixels;
    pglTexture->Height = picHeightPixels;
    // Image Color Depth In Bits Per Pixel (hardcoded for now - RGBA_8888)
    pglTexture->Bpp = 32;
    pglTexture->Type = GL_RGBA;

    // Typical Texture Generation Using Data From The Bitmap
    // Bind To The Texture ID
    glBindTexture(GL_TEXTURE_2D, pglTexture->TextureID);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    bool mipping;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1 );

    switch(m_TextureFilterType)
    {
        default:
        case txNoFilter:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            mipping = false;
            break;

        case txBilinear:
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            mipping = false;
            break;

        case txTrilinear:
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            mipping = true;
            break;
    }

    QImage tex = QGLWidget::convertToGLFormat(img);


    // crank out the texture
    if (mipping == false)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(),
            0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
    }
    else
    {
        // Build Mipmaps (builds different versions of the picture for distances - looks better)
        gluBuild2DMipmaps(GL_TEXTURE_2D,  GL_RGBA,
                       tex.width(),
                       tex.height(),
                       GL_RGBA,
                       GL_UNSIGNED_BYTE,
                       tex.bits());
    }

    return true;
}

void QTextureLoader::FreeTexture(glTexture *pglTexture)
{
        glDeleteTextures(1, &pglTexture->TextureID);
}

QTextureLoader::~QTextureLoader()
{
}
