#include <Utils3D.h>

float Utils3D::TOLERANCE= 0.001;

VECTOR4D Utils3D::glTransformPoint(VECTOR4D v, Matrix4D m)
{
    VECTOR4D t;
    double x, y, z, w;

    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
    t.x = m[0]*x + m[4]*y + m[8]*z  + m[12]*w;
    t.y=  m[1]*x + m[5]*y + m[9]*z  + m[13]*w;
    t.z=  m[2]*x + m[6]*y + m[10]*z + m[14]*w;
    t.w = m[3]*x + m[7]*y + m[11]*z + m[15]*w;

    return t;
}



int Utils3D::inverseMatrix4D(Matrix4D m, Matrix4D i)
{
    float fA0 = m[ 0]*m[ 5] - m[ 4]*m[ 1];
    float fA1 = m[ 0]*m[ 9] - m[ 8]*m[ 1];
    float fA2 = m[ 0]*m[13] - m[12]*m[ 1];
    float fA3 = m[ 4]*m[ 9] - m[ 8]*m[ 5];
    float fA4 = m[ 4]*m[13] - m[12]*m[ 5];
    float fA5 = m[ 8]*m[13] - m[12]*m[ 9];
    float fB0 = m[ 2]*m[ 7] - m[ 6]*m[ 3];
    float fB1 = m[ 2]*m[11] - m[10]*m[ 3];
    float fB2 = m[ 2]*m[15] - m[14]*m[ 3];
    float fB3 = m[ 6]*m[11] - m[10]*m[ 7];
    float fB4 = m[ 6]*m[15] - m[14]*m[ 7];
    float fB5 = m[10]*m[15] - m[14]*m[11];

    float fDet = fA0*fB5-fA1*fB4+fA2*fB3+fA3*fB2-fA4*fB1+fA5*fB0;
    if (fabs(fDet) == 0)
    {
        return -1;
    }


    i[ 0] = + m[ 5]*fB5 - m[ 9]*fB4 + m[13]*fB3;
    i[ 1] =
        - m[ 1]*fB5 + m[ 9]*fB2 - m[13]*fB1;
    i[ 2] =
        + m[ 1]*fB4 - m[ 5]*fB2 + m[13]*fB0;
    i[ 3] =
        - m[ 1]*fB3 + m[ 5]*fB1 - m[ 9]*fB0;
    i[ 4] =
        - m[ 4]*fB5 + m[ 8]*fB4 - m[12]*fB3;
    i[ 5] =
        + m[ 0]*fB5 - m[ 8]*fB2 + m[12]*fB1;
    i[ 6] =
        - m[ 0]*fB4 + m[ 4]*fB2 - m[12]*fB0;
    i[ 7] =
        + m[ 0]*fB3 - m[ 4]*fB1 + m[ 8]*fB0;
    i[ 8] =
        + m[ 7]*fA5 - m[11]*fA4 + m[15]*fA3;
    i[ 9] =
        - m[ 3]*fA5 + m[11]*fA2 - m[15]*fA1;
    i[10] =
        + m[ 3]*fA4 - m[ 7]*fA2 + m[15]*fA0;
    i[11] =
        - m[ 3]*fA3 + m[ 7]*fA1 - m[11]*fA0;
    i[12] =
        - m[ 6]*fA5 + m[10]*fA4 - m[14]*fA3;
    i[13] =
        + m[ 2]*fA5 - m[10]*fA2 + m[14]*fA1;
    i[14] =
        - m[ 2]*fA4 + m[ 6]*fA2 - m[14]*fA0;
    i[15] =
        + m[ 2]*fA3 - m[ 6]*fA1 + m[10]*fA0;

    float fInvDet = 1.0/fDet;
    i[ 0] *= fInvDet;
    i[ 4] *= fInvDet;
    i[ 8] *= fInvDet;
    i[12] *= fInvDet;
    i[ 1] *= fInvDet;
    i[ 5] *= fInvDet;
    i[ 9] *= fInvDet;
    i[13] *= fInvDet;
    i[ 2] *= fInvDet;
    i[ 6] *= fInvDet;
    i[10] *= fInvDet;
    i[14] *= fInvDet;
    i[ 3] *= fInvDet;
    i[ 7] *= fInvDet;
    i[11] *= fInvDet;
    i[15] *= fInvDet;

    return 0;
}


VECTOR4D Utils3D::getTransformPosition(Matrix4D transMatrix)
{
    VECTOR4D v(transMatrix[12],transMatrix[13],transMatrix[14],1.0);

    VECTOR4D v1(transMatrix[0],transMatrix[1],transMatrix[2],1.0);
    VECTOR4D v2(transMatrix[4],transMatrix[5],transMatrix[6],1.0);
    VECTOR4D v3(transMatrix[8],transMatrix[9],transMatrix[10],1.0);

    return VECTOR4D(v.dot(v1), v.dot(v2), v.dot(v3), 1.0);
}

void Utils3D::printMatrix(Matrix4D m)
{
        cout.precision(2);
        cout.width(5);
         cout<<"*=========================================*"<<endl;


        cout<<m[ 0]<<" "<<m[ 4]<<" "<<m[ 8]<<" "<<m[12]<<endl;
        cout<<m[ 1]<<" "<<m[ 5]<<" "<<m[ 9]<<" "<<m[13]<<endl;
        cout<<m[ 2]<<" "<<m[ 6]<<" "<<m[10]<<" "<<m[14]<<endl;
        cout<<m[ 3]<<" "<<m[ 7]<<" "<<m[11]<<" "<<m[15]<<endl;
        cout<<"==========================================="<<endl;
}



VECTOR4D::VECTOR4D()
{
    this->w = 1.0;
}

VECTOR4D::VECTOR4D(GLfloat x,GLfloat y,GLfloat z,GLfloat w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}
QString VECTOR4D::toString()
{
    return QString().sprintf("(% 5.4f,% 5.4f,% 5.4f,% 5.4f)", x,y,z,w);
}

VECTOR4D VECTOR4D::getProjectionOnPlane(VECTOR4D normal)
{
    //plane normal probably should be normalized
    VECTOR4D same =  VECTOR4D(x,y,z,w);
    //not sure if all this normalization is necessary but just in case...
    //i'm not a mathematician
    VECTOR4D projected =  normal*same*normal.getUnitary();
    return projected*same.dot(projected);

}
VECTOR4D VECTOR4D::operator*(float operand)
{
        return VECTOR4D(x*operand,y*operand, z*operand, w);
}

VECTOR4D VECTOR4D::operator*(VECTOR4D operand)
{
   return VECTOR4D(y*operand.z - z*operand.y,z*operand.x-x*operand.z, x*operand.y-y*operand.x,1.0);
}



float VECTOR4D::dot(VECTOR4D operand)
{
   return x*operand.x + y*operand.y + z*operand.z;
}


float VECTOR4D::magnitude()
{
    return sqrt(x*x+y*y+z*z);
}


VECTOR4D VECTOR4D::getUnitary()
{
    if (x || y || z )
    {
        return VECTOR4D(x/magnitude(),y/magnitude(),z/magnitude(),w);
    }
    else
    {
        return VECTOR4D(x,y,z,w);
    }
}

void VECTOR4D::normalize()
{

    if (x || y || z)
    {
        float m = magnitude();
        x /= m;
        y/=m;
        z/= m;
    }
}

VECTOR4D VECTOR4D::operator +(VECTOR4D operand)
{
    return VECTOR4D(x + operand.x,y + operand.y, z + operand.z, w);
}

VECTOR4D VECTOR4D::operator -(VECTOR4D operand)
{
    return VECTOR4D(x - operand.x,y - operand.y, z - operand.z, w);
}


