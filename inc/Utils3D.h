#ifndef _3DUTILS_H
#define _3DUTILS_H
#include <qgl.h>
#include <cmath>
#include <iostream>

using namespace std;

typedef GLdouble Matrix4D[16];

struct VECTOR4D
{
	GLfloat x;            
	GLfloat y;           
	GLfloat z;
	GLfloat w;

        VECTOR4D();
        VECTOR4D(GLfloat x,GLfloat y,GLfloat z,GLfloat w=1.0);
        float dot(VECTOR4D operand);
        VECTOR4D operator*(float operand);
        VECTOR4D operator*(VECTOR4D operand);
        VECTOR4D operator+(VECTOR4D operand);
        VECTOR4D operator-(VECTOR4D operand);
        void normalize();
        VECTOR4D getUnitary();
        float magnitude();
        VECTOR4D getProjectionOnPlane(VECTOR4D normal);
        QString toString();

};




class Utils3D
{
public:
	 static VECTOR4D glTransformPoint(VECTOR4D v, Matrix4D m);
         static int inverseMatrix4D(Matrix4D m, Matrix4D i);
         static void printMatrix(Matrix4D m);
         static VECTOR4D getTransformPosition(Matrix4D transMatrix);
         static float TOLERANCE;
};




#endif
