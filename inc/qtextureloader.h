#ifndef QTEXTURELOADER_H
#define QTEXTURELOADER_H

#include <QObject>
#include <qgl.h>
#include <cmath>
#include <iostream>
#include <QDir>

using namespace std;

// Define Interface Data Types / Structures
//
typedef enum
{
    txUnknown	= 0,	// images
    txBmp		= 1,
    txJpg		= 2,
    txPng		= 3,
    txTga		= 4,
    txGif		= 5,
    txIco		= 6,
    txEmf		= 7,
    txWmf		= 8,
    // add new ones at the end
} eglTexType;

typedef enum
{
    txNoFilter	= 0,
    txBilinear	= 1,
    txTrilinear	= 2,
    // add new ones at the end
} eglTexFilterType;

typedef	struct
{
    GLuint		TextureID;									// Texture ID Used To Select A Texture
    eglTexType          TexType;									// Texture Format
    GLuint		Width;										// Image Width
    GLuint		Height;										// Image Height
    GLuint		Type;										// Image Type (GL_RGB, GL_RGBA)
    GLuint		Bpp;										// Image Color Depth In Bits Per Pixel
} glTexture;

typedef struct
{
    float   s;
    float   t;
} _glTexturCord;


typedef struct
{
    _glTexturCord TopRight;
    _glTexturCord TopLeft;
    _glTexturCord BottomLeft;
    _glTexturCord BottomRight;
} glTexturCordTable;

class QTextureLoader : public QObject
{
Q_OBJECT
public:
    explicit QTextureLoader(QObject *parent = 0);
    virtual ~QTextureLoader();
    //void    SetAlphaMatch(GLboolean fEnabled, GLubyte RedAlphaMatch, GLubyte GreenAlphaMatch, GLubyte BlueAlphaMatch);
    //void    SetHighQualityTextures(GLboolean fEnabled);
    //void    SetMipMapping(GLboolean fEnabled);
    //void    SetTextureFilter(eglTexFilterType type);

    bool LoadTextureFromDisk(QString fileName, glTexture *pglTexture);
    //int   LoadTextureFromRam(unsigned char *pData, int Length, glTexture *pglTexture, eglTexType TexType);
    //int   LoadTextureFromResource(unsigned int ResourceName, char *pResourceType, glTexture *pglTexture, eglTexType TexType);
    void    FreeTexture(glTexture *pglTexture);

signals:

public slots:

private:
    bool buildTexture(QString filename, glTexture *pglTexture);

    eglTexFilterType    m_TextureFilterType;

};

#endif // QTEXTURELOADER_H
