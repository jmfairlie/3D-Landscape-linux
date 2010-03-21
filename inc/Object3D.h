#ifndef _OBJ3D_H
#define _OBJ3D_H

#include <Utils3D.h>
#include <ColladaReader.h>

#ifdef __WIN32__
#include <TextureLoader.h>
#else
#include <DummyDefinitions.h>
#include <ltexture.h>
#endif

#include <iostream>
#include <map>
using namespace std;

class Object3D
{
   public:
      Object3D();
#ifdef __WIN32__
      Object3D(glTexture* ptexstr, TNode* nodeInfo, map<string,TGeometry> *pgeometries);   // Constructor.
#else
      Object3D(LTexture* ptexstr, TNode* nodeInfo, map<string,TGeometry> *pgeometries);   // Constructor.
#endif
      ~Object3D();  // Destructor.

        void render();

        void getBoundingCube();

        void renderBoundingCube();

        TNode* nodeStr;

        VECTOR4D bdCubePoints[8];

        bool isInsideBDcube(VECTOR4D point);

        bool risen;
        void showBoundingBox(bool show);
        bool isBoundingBoxVisible();
        void getTransform();

        VECTOR4D getTransformPosition();

			
   protected:
        /* each vertex has position, texcoordinate, normal and color */

        void disableArrays();

        Matrix4D transform;
        map<string,TGeometry> *geometries;
        /*
        GLuint texName;
        GLuint texWidth;
        GLuint texHeight;
        */
#ifdef __WIN32__
        glTexture *textures;
#else
        LTexture *textures;
#endif
        GLuint vertexCount;                        /*!< Mesh vertex coordinate count */
        GLuint vertexFormat;                       /*!< Mesh vertex format */
        GLfloat **vertices;                         /*!< Mesh vertex table */
        GLuint numGroups;
        GLuint texCount;
        GLfloat *texcoords;                     /*!< Mesh texture coordinates */
        GLuint normalCount;
        GLfloat *normals;                       /*!< Mesh vertex normals */
        GLubyte *colors;                        /*!< Mesh vertex colors */

        /* index list */
        GLuint indexCount;	                        /*!< Mesh triangle index count */
        GLubyte *indices;                      /*!< Mesh index table */
        bool boundingBoxVisible;
        //unsigned char* texture;
        //pointer to texture data
};
#endif
