#include <Object3D.h>
#include <iostream>
using namespace std;

#ifdef __WIN32__
Object3D::Object3D(glTexture* ptexstr, TNode* nodeInfo, map<string,TGeometry> *pgeometries)
{
    geometries = pgeometries;
    vertexCount =0;
    indexCount = 0;
    texCount = 0;
    normalCount = 0;
    nodeStr = nodeInfo;
    textures = ptexstr;
    getTransform();
    this->boundingBoxVisible = false;
    this->risen = false;

    this->getBoundingCube();
}
#else
Object3D::Object3D(LTexture* ptexstr, TNode* nodeInfo, map<string,TGeometry> *pgeometries)
{
    geometries = pgeometries;
    vertexCount =0;
    indexCount = 0;
    texCount = 0;
    normalCount = 0;
    nodeStr = nodeInfo;
    textures = ptexstr;
    getTransform();
    this->boundingBoxVisible = false;
    this->risen = false;

    this->getBoundingCube();
}
#endif

void Object3D::showBoundingBox(bool show)
{
    this->boundingBoxVisible = show;

}

bool Object3D::isBoundingBoxVisible()
{
    return this->boundingBoxVisible;
}

void Object3D::getTransform()
{
    //construct the transform matrix of the object based on the dae file
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glLoadIdentity();


        for (u_int i =0 ; i < nodeStr->translations.size(); i++)
        {
            glTranslatef(nodeStr->translations[i].translation[0],nodeStr->translations[i].translation[1],nodeStr->translations[i].translation[2]);
        }
         for (u_int i =0 ; i < nodeStr->rotations.size(); i++)
        {
            glRotatef(nodeStr->rotations[i].rotation[3],nodeStr->rotations[i].rotation[0],nodeStr->rotations[i].rotation[1],nodeStr->rotations[i].rotation[2]);
        }
        for (u_int i =0 ; i < nodeStr->scalings.size(); i++)
        {
           glScalef(nodeStr->scalings[i].scaling[0],nodeStr->scalings[i].scaling[1],nodeStr->scalings[i].scaling[2]);
        }


        glGetDoublev(GL_MODELVIEW_MATRIX, transform);
     glPopMatrix();     

}

VECTOR4D Object3D::getTransformPosition()
{
    //VECTOR4D vec(-87.0,-10.0,-730.0,1.0);

    //VECTOR4D vec = Utils3D::getTransformPosition(this->transform);
    VECTOR4D vec = VECTOR4D(this->transform[12], this->transform[13], this->transform[14]);
    cout << "VEC x, y, z: " << vec.toString().toStdString() << endl;

    return vec;
}

Object3D::~Object3D()
{
}

void Object3D::renderBoundingCube()
{    
    glColor3d(1, 1, 0);
    glLineWidth(2.0);

    glBegin(GL_LINE_LOOP);
        glVertex3f(this->bdCubePoints[0].x,this->bdCubePoints[0].y,
                   this->bdCubePoints[0].z);
        glVertex3f(this->bdCubePoints[1].x,this->bdCubePoints[1].y,
                   this->bdCubePoints[1].z);
        glVertex3f(this->bdCubePoints[2].x,this->bdCubePoints[2].y,
                   this->bdCubePoints[2].z);
        glVertex3f(this->bdCubePoints[3].x,this->bdCubePoints[3].y,
                   this->bdCubePoints[3].z);
    glEnd();
    
    glBegin(GL_LINE_LOOP);
        glVertex3f(this->bdCubePoints[4].x,this->bdCubePoints[4].y,
                   this->bdCubePoints[4].z);
        glVertex3f(this->bdCubePoints[5].x,this->bdCubePoints[5].y,
                   this->bdCubePoints[5].z);
        glVertex3f(this->bdCubePoints[6].x,this->bdCubePoints[6].y,
                   this->bdCubePoints[6].z);
        glVertex3f(this->bdCubePoints[7].x,this->bdCubePoints[7].y,
                   this->bdCubePoints[7].z);
    glEnd();

    glBegin(GL_LINE_LOOP);
        glVertex3f(this->bdCubePoints[0].x,this->bdCubePoints[0].y,
                   this->bdCubePoints[0].z);
        glVertex3f(this->bdCubePoints[1].x,this->bdCubePoints[1].y,
                   this->bdCubePoints[1].z);
        glVertex3f(this->bdCubePoints[5].x,this->bdCubePoints[5].y,
                   this->bdCubePoints[5].z);
        glVertex3f(this->bdCubePoints[4].x,this->bdCubePoints[4].y,
                   this->bdCubePoints[4].z);
    glEnd();

    glBegin(GL_LINE_LOOP);
        glVertex3f(this->bdCubePoints[3].x,this->bdCubePoints[3].y,
                   this->bdCubePoints[3].z);
        glVertex3f(this->bdCubePoints[2].x,this->bdCubePoints[2].y,
                   this->bdCubePoints[2].z);
        glVertex3f(this->bdCubePoints[6].x,this->bdCubePoints[6].y,
                   this->bdCubePoints[6].z);
        glVertex3f(this->bdCubePoints[7].x,this->bdCubePoints[7].y,
                   this->bdCubePoints[7].z);
    glEnd();

    glBegin(GL_LINE_LOOP);
        glVertex3f(this->bdCubePoints[3].x,this->bdCubePoints[3].y,
                   this->bdCubePoints[3].z);
        glVertex3f(this->bdCubePoints[2].x,this->bdCubePoints[2].y,
                   this->bdCubePoints[2].z);
        glVertex3f(this->bdCubePoints[6].x,this->bdCubePoints[6].y,
                   this->bdCubePoints[6].z);
        glVertex3f(this->bdCubePoints[7].x,this->bdCubePoints[7].y,
                   this->bdCubePoints[7].z);
    glEnd();

    glBegin(GL_LINE_LOOP);
        glVertex3f(this->bdCubePoints[0].x,this->bdCubePoints[0].y,
                   this->bdCubePoints[0].z);
        glVertex3f(this->bdCubePoints[4].x,this->bdCubePoints[4].y,
                   this->bdCubePoints[4].z);
        glVertex3f(this->bdCubePoints[7].x,this->bdCubePoints[7].y,
                   this->bdCubePoints[7].z);
        glVertex3f(this->bdCubePoints[3].x,this->bdCubePoints[3].y,
                   this->bdCubePoints[3].z);
    glEnd();

    glBegin(GL_LINE_LOOP);
        glVertex3f(this->bdCubePoints[1].x,this->bdCubePoints[1].y,
                   this->bdCubePoints[1].z);
        glVertex3f(this->bdCubePoints[5].x,this->bdCubePoints[5].y,
                   this->bdCubePoints[5].z);
        glVertex3f(this->bdCubePoints[6].x,this->bdCubePoints[6].y,
                   this->bdCubePoints[6].z);
        glVertex3f(this->bdCubePoints[2].x,this->bdCubePoints[2].y,
                   this->bdCubePoints[2].z);
    glEnd();
}

void Object3D::getBoundingCube()
{
    float left_most = INFINITYM;
    float right_most = -INFINITYM;
    float top_most = -INFINITYM;
    float bottom_most = INFINITYM;
    float near_most = -INFINITYM;
    float far_most = INFINITYM;

    for (u_int i = 0; i < this->nodeStr->geometryIDs.size(); i++ )
    {
        string geomID = nodeStr->geometryIDs[i];
        map<string, TGeometry>::iterator it;
        it = geometries->find(geomID);

        if(it != geometries->end())
        {
            TGeometry gT = it->second;

            for (u_int j = 0; j < gT.trianglesStrs.size(); j++)
            {
                for(u_int k = 0; k < gT.trianglesStrs[j].vertices.size(); k+=3)
                {
                    //X
                    if(gT.trianglesStrs[j].vertices[k] < left_most)
                        left_most = gT.trianglesStrs[j].vertices[k];
                    else if(gT.trianglesStrs[j].vertices[k] > right_most)
                        right_most = gT.trianglesStrs[j].vertices[k];
                    //Y
                    if(gT.trianglesStrs[j].vertices[k+1] > top_most)
                        top_most = gT.trianglesStrs[j].vertices[k+1];
                    else if(gT.trianglesStrs[j].vertices[k+1] < bottom_most)
                        bottom_most = gT.trianglesStrs[j].vertices[k+1];
                    //Z
                    if(gT.trianglesStrs[j].vertices[k+2] < far_most)
                        far_most = gT.trianglesStrs[j].vertices[k+2];
                    else if(gT.trianglesStrs[j].vertices[k+2] > near_most)
                        near_most = gT.trianglesStrs[j].vertices[k+2];
                }
            }
        }
    }

    //front
    this->bdCubePoints[0].x = left_most;
    this->bdCubePoints[0].y = bottom_most;
    this->bdCubePoints[0].z = near_most;

    this->bdCubePoints[1].x = left_most;
    this->bdCubePoints[1].y = top_most;
    this->bdCubePoints[1].z = near_most;

    this->bdCubePoints[2].x = right_most;
    this->bdCubePoints[2].y = top_most;
    this->bdCubePoints[2].z = near_most;

    this->bdCubePoints[3].x = right_most;
    this->bdCubePoints[3].y = bottom_most;
    this->bdCubePoints[3].z = near_most;

    //rear
    this->bdCubePoints[4].x = left_most;
    this->bdCubePoints[4].y = bottom_most;
    this->bdCubePoints[4].z = far_most;

    this->bdCubePoints[5].x = left_most;
    this->bdCubePoints[5].y = top_most;
    this->bdCubePoints[5].z = far_most;

    this->bdCubePoints[6].x = right_most;
    this->bdCubePoints[6].y = top_most;
    this->bdCubePoints[6].z = far_most;

    this->bdCubePoints[7].x = right_most;
    this->bdCubePoints[7].y = bottom_most;
    this->bdCubePoints[7].z = far_most;
}

//determines whether the given point is inside the bounding cube
bool Object3D::isInsideBDcube(VECTOR4D point)
{
    VECTOR4D transformedPts[8];
    for(u_int i = 0; i < 8; i++)
    {
        transformedPts[i] =
                Utils3D::glTransformPoint(this->bdCubePoints[i], this->transform);
    }

    //first vector along x axis
    VECTOR4D vecX1;
    vecX1.x = transformedPts[3].x - transformedPts[0].x;
    vecX1.y = transformedPts[3].y - transformedPts[0].y;
    vecX1.z = transformedPts[3].z - transformedPts[0].z;

    //first vector along y axis
    VECTOR4D vecY1;
    vecY1.x = transformedPts[1].x - transformedPts[0].x;
    vecY1.y = transformedPts[1].y - transformedPts[0].y;
    vecY1.z = transformedPts[1].z - transformedPts[0].z;

    //first vector along z axis
    VECTOR4D vecZ1;
    vecZ1.x = transformedPts[4].x - transformedPts[0].x;
    vecZ1.y = transformedPts[4].y - transformedPts[0].y;
    vecZ1.z = transformedPts[4].z - transformedPts[0].z;

    //second vector along x axis
    VECTOR4D vecX2;
    vecX2.x = transformedPts[5].x - transformedPts[6].x;
    vecX2.y = transformedPts[5].y - transformedPts[6].y;
    vecX2.z = transformedPts[5].z - transformedPts[6].z;

    //second vector along y axis
    VECTOR4D vecY2;
    vecY2.x = transformedPts[7].x - transformedPts[6].x;
    vecY2.y = transformedPts[7].y - transformedPts[6].y;
    vecY2.z = transformedPts[7].z - transformedPts[6].z;

    //second vector along z axis
    VECTOR4D vecZ2;
    vecZ2.x = transformedPts[2].x - transformedPts[6].x;
    vecZ2.y = transformedPts[2].y - transformedPts[6].y;
    vecZ2.z = transformedPts[2].z - transformedPts[6].z;

    //first vector (connects first cube point to the specified point)
    VECTOR4D first;
    first.x = point.x - transformedPts[0].x;
    first.y = point.y - transformedPts[0].y;
    first.z = point.z - transformedPts[0].z;

    //second vector (connects second cube point to the specified point)
    VECTOR4D second;
    second.x = point.x - transformedPts[6].x;
    second.y = point.y - transformedPts[6].y;
    second.z = point.z - transformedPts[6].z;

    //dot product of first and Ox
    if((first.x*vecX1.x + first.y*vecX1.y + first.z*vecX1.z) < 0)
        return false;
    //dot product of first and Oy
    if((first.x*vecY1.x + first.y*vecY1.y + first.z*vecY1.z) < 0)
        return false;
    //dot product of first and Oz
    if((first.x*vecZ1.x + first.y*vecZ1.y + first.z*vecZ1.z) < 0)
        return false;

    //dot product of second and Ox
    if((second.x*vecX2.x + second.y*vecX2.y + second.z*vecX2.z) < 0)
        return false;
    //dot product of second and Oy
    if((second.x*vecY2.x + second.y*vecY2.y + second.z*vecY2.z) < 0)
        return false;
    //dot product of second and Oz
    if((second.x*vecZ2.x + second.y*vecZ2.y + second.z*vecZ2.z) < 0)
        return false;


    /*cout<<point.x<<","<<point.y<<","<<point.z<<" is inside:"<<endl;

    Utils3D::printMatrix(this->transform);

    for(u_int i = 0; i < 8; i++)
    {
        cout<<this->bdCubePoints[i].x<<" "<<this->bdCubePoints[i].y<<" "<<this->bdCubePoints[i].z<<endl;
    }

    cout<<endl;
    for(u_int i = 0; i < 8; i++)
    {
        cout<<transformedPts[i].x<<" "<<transformedPts[i].y<<" "<<transformedPts[i].z<<endl;
    }
    cout<<endl;*/

    return true;
}

void Object3D::render()
{

        glPushMatrix();
            /*if(this->risen)
                glTranslatef(0.0, 20.0, 0.0);*/

            glMultMatrixd(transform);
            //render a bounding box
            //if (this->boundingBoxVisible)
            {
                //this->renderBoundingCube();
            }
            float white[] = {1.0,1.0,1.0,1.0};
            float black[] = {0.0,0.0,0.0,1.0};
            float blueish[] = {0.1,0.1,0.4,1.0};
            for (u_int i =0; i < nodeStr->geometryIDs.size(); i++ )
            {
                string geomID = nodeStr->geometryIDs[i];
                map<string, TGeometry>::iterator it;
                it = geometries->find(geomID);

                if(it != geometries->end())
                {
                    TGeometry gT = it->second;

                    for (u_int j=0; j<gT.trianglesStrs.size(); j++)
                    {



                        if(nodeStr->id == "star" ||nodeStr->id == "Can" || nodeStr->id == "food")
                        {
                            glMaterialfv(GL_FRONT, GL_EMISSION, blueish);
                        }
                        else if (nodeStr->id == "logoPlane" ||nodeStr->id == "eventsPlane"||nodeStr->id == "galleryPlane_001"||nodeStr->id == "exitSignPlane"||nodeStr->id == "tvPlane"||nodeStr->id == "bottlesPlane" || nodeStr->id == "compassPlane" || nodeStr->id == "bartender")
                        {
                            glMaterialfv(GL_FRONT, GL_EMISSION, white);
                        }
                        else
                        {
                            glMaterialfv(GL_FRONT, GL_EMISSION, black);
                        }

                        if (gT.trianglesStrs[j].textureIndex != -1)
                        {
                                    glEnable(GL_TEXTURE_2D);
                                    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                                    glTexCoordPointer(2,GL_FLOAT,0, &(gT.trianglesStrs[j].textureCoords[0]));
#ifdef __WIN32__
                                    glBindTexture(GL_TEXTURE_2D, textures[gT.trianglesStrs[j].textureIndex].TextureID);
#else
                                    glBindTexture(GL_TEXTURE_2D, textures[gT.trianglesStrs[j].textureIndex].getTextureName());
#endif
                                    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE ,white);
                        }
                        else
                        {
                            glColor4fv(gT.trianglesStrs[j].color);
                            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE ,gT.trianglesStrs[j].color);
                        }

                        glEnableClientState(GL_VERTEX_ARRAY);
                        glEnableClientState(GL_NORMAL_ARRAY);
                        glNormalPointer(GL_FLOAT,0,&(gT.trianglesStrs[j].normals[0]));
                        glVertexPointer(3,GL_FLOAT,0,&(gT.trianglesStrs[j].vertices[0]));
                        glDrawArrays(GL_TRIANGLES, 0, gT.trianglesStrs[j].vertices.size()/3);
                        disableArrays();

                    }

                }
            }
        glPopMatrix();
	glDisable(GL_TEXTURE_2D);


}

void Object3D::disableArrays()
{
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}




