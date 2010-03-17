#ifndef DUMMYDEFINITIONS_H
#define DUMMYDEFINITIONS_H

#ifndef INT64
#define INT64 unsigned long long
#endif

#include <qgl.h>

/*
  * Just to make this whole thing compile!!!
  */

// Define Interface Data Types / Structures
//
typedef enum {
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

typedef	struct
{
        GLuint		TextureID;									// Texture ID Used To Select A Texture
        eglTexType	TexType;									// Texture Format
        GLuint		Width;										// Image Width
        GLuint		Height;										// Image Height
        GLuint		Type;										// Image Type (GL_RGB, GL_RGBA)
        GLuint		Bpp;										// Image Color Depth In Bits Per Pixel
} glTexture;



#endif // DUMMYDEFINITIONS_H
