#ifndef __COLLADDA_READER_H__
#define __COLLADDA_READER_H__

#include <dae.h>
#include <dom/domCOLLADA.h>
#include <dom/domElements.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <definesall.h>

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
	string name;
	string imageURL; 
};

//structure, representing <triangles> element with corresponding texture filename, vertices, normals and texture coordinates
struct TTriangles
{
	vector<float> vertices;
	vector<float> normals;
	vector<float> textureCoords;
	string textureFileName;	
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

class ColladaReader
{
public:
	ColladaReader();
	ColladaReader(char *path);
	
	//TGeomInfo - structure, representing geometry
	u_short getGeometry(TGeomInfo *geomInfo);	

private:
	u_short getMaterials();
	u_short getVertices(vector<daeElement*> nodes);
	u_short getSources(vector<daeElement*> nodes);
	void buildAddTrianglesStructure(int verticesStructIndex, int normalsStructIndex, int textCoordsStructIndex,
			int materialsStructIndex, 
			int vertexOffset,
			int normalOffset,
			int textCoordOffset,
			int inputsNum,
			domListOfUInts pArray
			);
	
	//fills in TGeomInfo structure
	void populateGeometryStructure(TGeomInfo *geomInfo);
	
	DAE dae;
	daeElement* root;
	
	//arrays of triangles for each mesh
	vector<domTriangles_Array> triangles;	
	
	vector<TVertices> verticesStrs;
	vector<TSource> sourcesStrs;
	vector<string> textureFileNames;
	vector<TMaterial> materialsStrs;
	
	//vector of TTriangles structures
	vector<TTriangles> trianglesStrs;
};

#endif //__COLLADDA_READER_H__

