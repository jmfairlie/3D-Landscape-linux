#ifndef __COLLADDA_READER_H__
#define __COLLADDA_READER_H__

#include <dae.h>
#include <dom/domCOLLADA.h>
#include <dom/domElements.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <definesall.h>
#include <QString>
#include <QStringList>

#define FILENAME "./cube5.dae"

using namespace std;

struct TSource
{
	vector<float> data;	
	string name;	
};

struct TVertices
{
	vector<float> data;	
	string name;	
};


struct TMaterial
{
    bool texture;
    string name;
    float color[4]; //RGBA
    string imageURL;
};

//structure, representing <triangles> element with corresponding texture filename, vertices, normals and texture coordinates
struct TTriangles
{
	vector<float> vertices;
	vector<float> normals;
	vector<float> textureCoords;
        //string textureFileName;
        int textureIndex;
        bool texture;
        float color[4]; //RGBA
};

struct TGeomInfo
{
	//number of different vertices arrays
	int groupsNum;
	//size of each vertex array
	int *groupsSize;
	//texture file name for each vertex array
	char **textFileNames;
	//array of vertices arrays
	float **vertices;
	//array of normal arrays corresponding to vertices arrays
	float **normals;
	//array of texture coordinates arrays corresponding to vertices arrays
	float **textureCoords;
	
	~TGeomInfo();
};

struct TGeometry
{
    vector<TTriangles> trianglesStrs;
    vector<TVertices> verticesStrs;
    vector<TSource> sourcesStrs;
    //vector<string> textureFileNames;
    //vector<int> textureIndices;
};

struct TRotation
{
    float rotation[4];
};

struct TTranslation
{
    float translation[3];
};

struct TScaling
{
    float scaling[3];
};


//structure, representing <node> + geometry information
struct TNode
{
    //vector<TGeometry> geometries;
    //translations, scalings, rotations
    //vector<TPut> puts;
    //IDs of geometry elements used by this node
    vector<string> geometryIDs;
    //translations    
    vector<TTranslation> translations;
    //scalings
    vector<TScaling> scalings;
    //rotations
    vector<TRotation> rotations;
    string id;
};

class ColladaReader
{
public:
	ColladaReader();
	ColladaReader(char *path);

        virtual ~ColladaReader();


        //get a scene information from file together with the geometry
        u_short getScene();

        u_short getGeometries();

        //vector of node structures
        vector<TNode> nodes;

        //map of geometry structures (its elements are referenced by nodes)
        map<string, TGeometry> geometries;

        //all the textures used by geometries
        vector<string> textureFileNames;

private:
	u_short getMaterials();

	u_short getVertices(vector<daeElement*> nodes);
        //fills in array of vertices for the geometry
        u_short getVerticesS(TGeometry &geometry, domMesh *currMesh);
        //fills in array of sources for the geometry
        u_short getSourcesS(TGeometry &geometry, domMesh *currMesh);

	u_short getSources(vector<daeElement*> nodes);

        void buildAddTrianglesStructureS(int verticesStructIndex, int normalsStructIndex, int textCoordsStructIndex,
                        int materialsStructIndex,
                        int vertexOffset,
                        int normalOffset,
                        int textCoordOffset,
                        int inputsNum,
                        domListOfUInts pArray,
                        TGeometry &geometry
                        );
	
        //returns an index of the texture file name if it is already in textureFileNames
        //structure. -1, otherwise
        int isTextureLoaded(string textureFileName);


	
	DAE dae;
	daeElement* root;

        string openedDoc;
	
	//arrays of triangles for each mesh
        vector<domTriangles_Array> triangles;
	
	vector<TVertices> verticesStrs;
	vector<TSource> sourcesStrs;
        //vector<string> textureFileNames;
	vector<TMaterial> materialsStrs;
	
	//vector of TTriangles structures
	vector<TTriangles> trianglesStrs;
};

#endif //__COLLADDA_READER_H__

