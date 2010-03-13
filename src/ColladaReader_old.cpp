#include "ColladaReader.h"

ColladaReader::ColladaReader()
{
	root = this->dae.open(FILENAME);
}

ColladaReader::ColladaReader(char *path)
{
	if(path != NULL)
	{
		root = this->dae.open(string(path));		
	}	
}

//fills in array of materials
u_short ColladaReader::getMaterials()
{
	//invalid document root
	if(!root)
	{
		return E_ROOT;
	}
	
	materialsStrs.clear();
	
	//add blank material (materialsStrs[0])
	TMaterial materialStruct;
	materialStruct.name = string("blank");
	materialStruct.imageURL = string("");
	materialsStrs.push_back(materialStruct);
	
	daeElement* libMat = root->getDescendant("library_materials");
	
	if (!libMat)
	{
		//invalid document format
		cout << "Stas 1" << endl;
		return E_FORMAT;
	}
	
	daeTArray<daeElementRef> materials = libMat->getChildren();
	
	//both of this elements should have children
	if(materials.getCount() == 0)
	{
		cout << "Stas 2" << endl;
		return E_FORMAT;
	}
	
	for (u_int i = 0; i < materials.getCount(); i++)
	{
		domMaterial *material = (domMaterial*)(domElement*)materials[i];
		string materialName = "";
				
		if(material->getName() != NULL)
		{
			materialName = material->getName();
		}
		else if(material->getId() != NULL)
		{
			materialName = material->getId();
		}
		else
		{
			cout << "Stas 27" << endl;
			continue;
		}
		
		
		domInstance_effectRef instanceEffRef = material->getInstance_effect();
		
		if(!instanceEffRef)
		{
			cout << "Stas 21" << endl;
			continue;
			//return E_FORMAT;
		}
		
		domInstance_effect *instanceEffect = (domInstance_effect*) (domElement*) instanceEffRef;

		daeURI effectUri = instanceEffect->getUrl();
		domElement *effectElt = effectUri.getElement();
		if(!effectElt)
		{
			//return E_RESOLVE;
			cout << "Stas 27" << endl;
			continue;
		}			
		domEffect *effect = (domEffect*) effectElt;
		
		//cout << "IE: " << effect->getId() << endl;
		
		domElement *surface = NULL;
		surface = effect->getDescendant("surface");
								
		if (!surface)
		{
			cout << "Stas 25" << endl;
			//return E_FORMAT;
			continue;
		}
		
		daeElement *initFromElt = surface->getDescendant("init_from");
						
		if(!initFromElt)
		{
			cout << "Stas 24" << endl;
			//return E_FORMAT;
			continue;
		}
		
		//cout << "char data: " << initFromElt->getCharData() << endl;
		daeElement* imgElt = dae.getDatabase()->idLookup(initFromElt->getCharData(), root->getDocument());

		if(!imgElt)
		{
			cout << "Stas 26" << endl;
			//return E_FORMAT;
			continue;
		}
		
		domImage *image = (domImage*)imgElt;	
				
		if (image && image->getInit_from())
		{
			cout << "image path: " << image->getInit_from()->getValue().originalStr() << endl;
			TMaterial materialStruct;
			materialStruct.name = materialName;
			materialStruct.imageURL = image->getInit_from()->getValue().originalStr();
			materialsStrs.push_back(materialStruct);
		}
		else
		{
			cout << "Stas 23" << endl;
			//return E_FORMAT;
			continue;
		}

        }

	return E_NO;
}

//fills in array of vertices (assumes that nodes are correct)
u_short ColladaReader::getVertices(vector<daeElement*> nodes)
{
	verticesStrs.clear();
	
	//vertices
	for(u_int i = 0; i < nodes.size(); i++)
	{
		domMesh *currMesh = (domMesh *)nodes[i];
		domVerticesRef verticesRef = currMesh->getVertices();
		
		if(!verticesRef)
		{
			cout << "Stas 5" << endl;
			return E_FORMAT;
		}
		
		domVertices *vertices = (domVertices*) (domElement*) verticesRef;
		
		//new vertices element
		TVertices newVertices;
		
		//name
		newVertices.name = vertices->getId();
		
		//get local input array
		domInputLocal_Array posarr = vertices->getInput_array();
		
		if(posarr.getCount() == 0)
		{
			cout << "Stas 6" << endl;
			return E_FORMAT;
		}		
		
		//position references
		for(u_int j = 0; j < posarr.getCount(); j++)
		{
			domInputLocal *inputElt = posarr.get(j);
			
			if(!inputElt)
			{
				cout << "Stas 7" << endl;
				return E_FORMAT;
			}
			
			string semanticStr = string(inputElt->getSemantic());
			
			if(semanticStr == "POSITION")
			{
				domURIFragmentType sourceURI = inputElt->getSource();
								
				domElement *source = sourceURI.getElement();
				if(!source)
				{
					return E_RESOLVE;
				}			
				domSource *sourceObj = (domSource*) source;
													
				//float data
				domFloat_arrayRef floatArrRef = sourceObj->getFloat_array();
				
				if(!floatArrRef)
				{
					cout << "Stas 8" << endl;
					return E_FORMAT;
				}
				
				domFloat_array *floatArr = (domFloat_array*) (domElement*)floatArrRef;
				domListOfFloats tempList = floatArr->getValue();
				
				if(tempList.getCount() == 0)
				{
					cout << "Stas 9" << endl;
					return E_FORMAT;
				}				
				
				for(u_int k = 0; k < tempList.getCount(); k++)
				{
					newVertices.data.push_back(tempList.get(k));
				}
			}				
		}
		
		verticesStrs.push_back(newVertices);
	}
	return E_NO;
}

//fills in array of sources
u_short ColladaReader::getSources(vector<daeElement*> nodes)
{
	sourcesStrs.clear();
	
	//sources
	for(u_int i = 0; i < nodes.size(); i++)
	{
		domMesh *currMesh = (domMesh *)nodes[i];
		
		//array of sources
		domSource_Array sourceArr = currMesh->getSource_array();
		
		if(sourceArr.getCount() == 0)
		{
			cout << "Stas 10" << endl;
			return E_FORMAT;
		}
		
		//parse sources
		for(u_int j = 0; j < sourceArr.getCount(); j++)
		{
			domSource *sourceObj = sourceArr.get(j);
			
			if(!sourceObj)
			{
				cout << "Stas 11" << endl;
				return E_FORMAT;
			}
			
			//source object to add
			TSource newSource;
			//name
			newSource.name = string(sourceObj->getId());
			
			//float data
			domFloat_arrayRef floatArrRef = sourceObj->getFloat_array();
			
			if(!floatArrRef)
			{
				cout << "Stas 12" << endl;
				return E_FORMAT;
			}
						
			domFloat_array *floatArr = (domFloat_array*) (domElement*)floatArrRef;
			domListOfFloats tempList = floatArr->getValue();
			
			if(tempList.getCount() == 0)
			{
				cout << "source id: " << sourceObj->getId() << endl;
				cout << "Stas 13" << endl;
				return E_FORMAT;
			}
				
			for(u_int k = 0; k < tempList.getCount(); k++)
			{				
				newSource.data.push_back(tempList.get(k));
			}
			sourcesStrs.push_back(newSource);
		}		
	}
	return E_NO;
}

//fills in the array of vertices
u_short ColladaReader::getGeometry(TGeomInfo *geomInfo)
{
	if(!this->root)
	{
		return E_ROOT;
	}
	
	this->triangles.clear();
	
	//meshes
	vector<daeElement*> nodes = this->dae.getDatabase()->typeLookup(domMesh::ID());
	
	if(nodes.size() == 0)
	{
		cout << "Stas 14" << endl;
		return E_FORMAT;
	}
	
	//build structures
	u_short e = E_NO;
	if((e = this->getMaterials()) != E_NO)
		return e;
	if((e = this->getVertices(nodes)) != E_NO)
		return e;
	if((e = this->getSources(nodes)) != E_NO)
		return e;
	
	
	//triangles (elements)
	for(u_int i = 0; i < nodes.size(); i++)
	{
		domMesh *currMesh = (domMesh *)nodes[i];
		
		domTriangles_Array triArray = currMesh->getTriangles_array();
		
		this->triangles.push_back(triArray);
	}	
	
	//for each mesh
	for(u_int i = 0; i < this->triangles.size(); i++)
	{		
		domInputLocalOffset_Array inoff;		
			
		if(this->triangles[i].getCount() == 0)
		{
			cout << "Stas 15" << endl;
			return E_FORMAT;
		}
			
		//for each triangles group
		for(u_int j = 0; j < this->triangles[i].getCount(); j++)
		{			
			int vertexOffset = 0;
			int normalOffset = 0;
			int textCoordOffset = 0;
			
			//number of input elements
			int inputsNum = 0;
			
			string verticesName = "";
			string normalsName = "";
			string textCoordsName = "";
			string triMaterialName = "";
			
			int verticesStructIndex = -1;
			int normalsStructIndex = -1;
			int textCoordsStructIndex = -1;
			//blank material by default
			int materialsStructIndex = 0;
			
			domTriangles* trianglesInst = (domTriangles*) (domElement*) this->triangles[i].get(j);
			if(!trianglesInst)
			{
				cout << "Stas 16" << endl;
				return E_FORMAT;
			}
			
			domInputLocalOffset_Array inoff = trianglesInst->getInput_array();
			if(inoff.getCount() == 0)
			{
				cout << "Stas 17" << endl;
				return E_FORMAT;
			}
			inputsNum = inoff.getCount();
			
			//reference to indices
			domPRef pRef = trianglesInst->getP();
			if(!pRef)
			{
				cout << "Stas 18" << endl;
				return E_FORMAT;
			}
			
			domP *indicesP = (domP*) (domElement*) pRef;			
			
			//P array
			domListOfUInts indicesUintArr = indicesP->getValue();
			if(!indicesP || (indicesUintArr.getCount() == 0))
			{
				cout << "Stas 19" << endl;
				return E_FORMAT;
			}
						
			//<input> elements
			for(u_int k = 0; k < inoff.getCount(); k++)
			{	
				if(inoff[k]->getSemantic() == string("VERTEX"))
				{
					vertexOffset = inoff[k]->getOffset();	
										
					domURIFragmentType uri = inoff[k]->getSource();
										 	
					string tempStr = uri.originalStr();
					verticesName = tempStr.substr(1, tempStr.length());	
																				
				}
				else if(inoff[k]->getSemantic() == string("NORMAL"))
				{
					normalOffset = inoff[k]->getOffset();	

					domURIFragmentType uri = inoff[k]->getSource();
										
					string tempStr = uri.originalStr();
					normalsName = tempStr.substr(1, tempStr.length());
				}
				else if(inoff[k]->getSemantic() == string("TEXCOORD"))
				{
					textCoordOffset = inoff[k]->getOffset();	

					domURIFragmentType uri = inoff[k]->getSource();					
										
					string tempStr = uri.originalStr();
					textCoordsName = tempStr.substr(1, tempStr.length());
				}
			}
			
			//get vertices
			for(u_int k = 0; k < verticesStrs.size(); k++)
			{
				//match!
				if(verticesStrs[k].name == verticesName)
				{
					verticesStructIndex = k;
					break;
				}
			}			
			
			//get normals and texture coordinates			
			for(u_int k = 0; k < sourcesStrs.size(); k++)
			{
				//match!
				if(sourcesStrs[k].name == normalsName)
				{
					normalsStructIndex = k;
					break;
				}				
			}
			for(u_int k = 0; k < sourcesStrs.size(); k++)
			{
				//match!
				if(sourcesStrs[k].name == textCoordsName)
				{
					textCoordsStructIndex = k;
					break;
				}
			}			
			
                        cout << "We are here 1!!!" << endl;
			//get texture filename
			triMaterialName = trianglesInst->getMaterial();
                        cout << "We are here 2!!!" << endl;

                        //cout << "material name: " << triMaterialName << endl;
			for(u_int k = 0; k < materialsStrs.size(); k++)
			{
				//match!
				if(materialsStrs[k].name == triMaterialName)
				{
					materialsStructIndex = k;
					break;
				}
			}			
			
			if((verticesStructIndex != -1) && (inputsNum != 0)/*&& (normalsStructIndex != -1) &&
				(textCoordsStructIndex != -1) && (materialsStructIndex != -1)*/)
			{
				this->buildAddTrianglesStructure(verticesStructIndex, normalsStructIndex, 
					textCoordsStructIndex,
					materialsStructIndex,
					vertexOffset,
					normalOffset,
					textCoordOffset,
					inputsNum,
					indicesUintArr
					);
			}		
			else
			{
				cout << "Stas 20" << endl;
				//return E_FORMAT;
			}
		}			
	}
	
	//populate structure
	this->populateGeometryStructure(geomInfo);
		
	return E_NO;
}


void ColladaReader::buildAddTrianglesStructure(int verticesStructIndex, int normalsStructIndex, int textCoordsStructIndex,
			int materialsStructIndex, 
			int vertexOffset,
			int normalOffset,
			int textCoordOffset,
			int inputsNum,
			domListOfUInts pArray)
{
		
	TTriangles newTriangles;
	
	//cout << "Triangle: " << endl;
	int index = 0;
	int i = 0;	
	
	while(i < (int)pArray.getCount())
	{
		for(int j = 1; j <= inputsNum; j++)
		{
			unsigned long long uintTmp = pArray.get(i + j - 1);		
						
			index = (int)uintTmp;
			
			if(j == (vertexOffset + 1))
			{
				index *= 3;
				for(int k = index; k < index + 3; k++)
				{
					float findex = this->verticesStrs[verticesStructIndex].data[k];
					//cout << findex << " ";
					newTriangles.vertices.push_back(findex);
				}
			}
			else if(j == (normalOffset + 1))
			{
				if(normalsStructIndex != -1)
				{
					index *= 3;
					for(int k = index; k < index + 3; k++)
					{
						float findex = this->sourcesStrs[normalsStructIndex].data[k];
						//cout << findex << " ";
						newTriangles.normals.push_back(findex);
					}
				}
			}
			else if(j == (textCoordOffset + 1))
			{
				if(textCoordsStructIndex != -1)
				{
					index *= 2;
					for(int k = index; k < index + 2; k++)
					{
						float findex = this->sourcesStrs[textCoordsStructIndex].data[k];
						//cout << findex << " ";
						newTriangles.textureCoords.push_back(findex);
					}
				}
			}			
		}
		
		i += inputsNum;
	}	
	
	//texture image file name
	newTriangles.textureFileName = this->materialsStrs[materialsStructIndex].imageURL;
	
	this->trianglesStrs.push_back(newTriangles);
}

//fills in TGeomInfo structure
void ColladaReader::populateGeometryStructure(TGeomInfo *geomInfo)
{        
	//set groups number
	int groupsNum = this->trianglesStrs.size();
	geomInfo->groupsNum = groupsNum;
	geomInfo->groupsSize = new int[groupsNum];
	geomInfo->textFileNames = new char*[groupsNum];
	geomInfo->vertices = new float*[groupsNum];
	geomInfo->normals = new float*[groupsNum];
	geomInfo->textureCoords = new float*[groupsNum];
	
	for(int i = 0; i < groupsNum; i++)
	{
		//text file names
		if(this->trianglesStrs[i].textureFileName == "")
		{
			geomInfo->textFileNames[i] = "\0";
		}
		else
		{
			geomInfo->textFileNames[i] = new char[this->trianglesStrs[i].textureFileName.size() + 1];
				
			strncpy(geomInfo->textFileNames[i],
				this->trianglesStrs[i].textureFileName.c_str(), 
				this->trianglesStrs[i].textureFileName.size() + 1);
		}
			
		//groups size
		geomInfo->groupsSize[i] = this->trianglesStrs[i].vertices.size();
			
		//vertices
		geomInfo->vertices[i] = new float[this->trianglesStrs[i].vertices.size()];
		//populate vertices
		for(u_int j = 0; j < this->trianglesStrs[i].vertices.size(); j++)
		{
			geomInfo->vertices[i][j] = this->trianglesStrs[i].vertices[j];		
		}
		
		//normals
		geomInfo->normals[i] = new float[this->trianglesStrs[i].normals.size()];
		//populate vertices
		for(u_int j = 0; j < this->trianglesStrs[i].normals.size(); j++)
		{
			geomInfo->normals[i][j] = this->trianglesStrs[i].normals[j];		
		}
		
		//texture coordinates
		geomInfo->textureCoords[i] = new float[this->trianglesStrs[i].textureCoords.size()];
		//populate texture coordinates
		for(u_int j = 0; j < this->trianglesStrs[i].textureCoords.size(); j++)
		{
			geomInfo->textureCoords[i][j] = this->trianglesStrs[i].textureCoords[j];		
		}
	}
}

TGeomInfo::~TGeomInfo()
{
	//delete all arrays and free memory
	delete[] groupsSize;
	
	for(int i = 0; i < groupsNum; i++)
	{
		delete[] textFileNames[i];
	}
	delete[] textFileNames;
	
	for(int i = 0; i < groupsNum; i++)
	{
		delete[] vertices[i];
	}
	delete[] vertices;
	
	for(int i = 0; i < groupsNum; i++)
	{
		delete[] normals[i];
	}
	delete[] normals;
	
	for(int i = 0; i < groupsNum; i++)
	{
		delete[] textureCoords[i];
	}
	delete[] textureCoords;
}

