#include "ColladaReader.h"

ColladaReader::ColladaReader()
{
    this->openedDoc = string(FILENAME);
    root = this->dae.open(FILENAME);
}

ColladaReader::ColladaReader(char *path)
{
	if(path != NULL)
	{
            this->openedDoc = string(path);
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
        materialStruct.texture = false;
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

                //no surface
		if (!surface)
		{
			cout << "Stas 25" << endl;
			//return E_FORMAT;
                        //continue;
                        daeElement *ambientElt = effect->getDescendant("ambient");

                        if(!ambientElt)
                        {
                            cout << "Stas 31" << endl;
                            //return E_FORMAT;
                            continue;
                        }

                        domLight::domTechnique_common::domAmbient *ambient =
                                (domLight::domTechnique_common::domAmbient *) ambientElt;

                        daeElement *colorA = ambient->getDescendant("color");

                        if(!colorA)
                        {
                            cout << "Stas 47" << endl;
                            //return E_FORMAT;
                            continue;
                        }


                        string colorStr = colorA->getCharData();

                        QString colorQStr(colorStr.c_str());

                        QStringList colorsList = colorQStr.split(" ");

                        if(colorsList.size() != 4)
                        {
                            cout << "Stas 48" << endl;
                            //return E_FORMAT;
                            continue;
                        }


                        TMaterial materialStruct;
                        materialStruct.name = materialName;
                        materialStruct.texture = false;

                        bool ok = true;
                        for(u_int y = 0; y < 4; y++)
                        {
                            float tmp = colorsList[y].toFloat(&ok);

                            if(!ok)
                            {
                                cout << "Stas 49" << endl;
                                //return E_FORMAT;
                                break;
                            }
                            materialStruct.color[y] = tmp;
                        }

                        if(!ok)
                        {
                            continue;
                        }

                        materialsStrs.push_back(materialStruct);

		}
                else
                {
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
                            //cout << "image path: " << image->getInit_from()->getValue().originalStr() << endl;

                            TMaterial materialStruct;
                            materialStruct.name = materialName;
                            materialStruct.imageURL = image->getInit_from()->getValue().originalStr();
                            materialStruct.texture = true;
                            materialsStrs.push_back(materialStruct);
                    }
                    else
                    {
                            cout << "Stas 23" << endl;
                            //return E_FORMAT;
                            continue;
                    }
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

//fills in array of vertices for the geometry
u_short ColladaReader::getVerticesS(TGeometry &geometry, domMesh *currMesh)
{
    //vertices
    domVerticesRef verticesRef = currMesh->getVertices();

    if(!verticesRef)
    {
            cout << "Stas 33" << endl;
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
            cout << "Stas 34" << endl;
            return E_FORMAT;
    }

    //position references
    for(u_int j = 0; j < posarr.getCount(); j++)
    {
            domInputLocal *inputElt = posarr.get(j);

            if(!inputElt)
            {
                    cout << "Stas 35" << endl;
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
                            cout << "Stas 36" << endl;
                            return E_FORMAT;
                    }

                    domFloat_array *floatArr = (domFloat_array*) (domElement*)floatArrRef;
                    domListOfFloats tempList = floatArr->getValue();

                    if(tempList.getCount() == 0)
                    {
                            cout << "Stas 37" << endl;
                            return E_FORMAT;
                    }

                    for(u_int k = 0; k < tempList.getCount(); k++)
                    {
                            newVertices.data.push_back(tempList.get(k));
                    }
            }
    }

    geometry.verticesStrs.push_back(newVertices);

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

//fills in array of sources for the geometry
u_short ColladaReader::getSourcesS(TGeometry &geometry, domMesh *currMesh)
{
    //array of sources
    domSource_Array sourceArr = currMesh->getSource_array();

    if(sourceArr.getCount() == 0)
    {
            cout << "Stas 38" << endl;
            return E_FORMAT;
    }

    //parse sources
    for(u_int j = 0; j < sourceArr.getCount(); j++)
    {
            domSource *sourceObj = sourceArr.get(j);

            if(!sourceObj)
            {
                    cout << "Stas 39" << endl;
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
                    cout << "Stas 40" << endl;
                    return E_FORMAT;
            }

            domFloat_array *floatArr = (domFloat_array*) (domElement*)floatArrRef;
            domListOfFloats tempList = floatArr->getValue();

            if(tempList.getCount() == 0)
            {
                    cout << "source id: " << sourceObj->getId() << endl;
                    cout << "Stas 41" << endl;
                    return E_FORMAT;
            }

            for(u_int k = 0; k < tempList.getCount(); k++)
            {
                    newSource.data.push_back(tempList.get(k));
            }
            geometry.sourcesStrs.push_back(newSource);
    }

    return E_NO;
}

u_short ColladaReader::getGeometries()
{
    cout<<"ColladaReader::getGeometries() called"<<endl;
    if(!this->root)
    {
        return E_ROOT;
    }

    //geometries
    vector<daeElement*> geometryElts = this->dae.getDatabase()->typeLookup(domGeometry::ID());

    //walk through geometries
    for(u_int j = 0; j < geometryElts.size(); j++)
    {        
        TGeometry newGeometry;

        domGeometry *geometry = (domGeometry*) geometryElts[j];

        string currGeomId = geometry->getId();

        domMesh *geomMesh = geometry->getMesh();
        //meshes.push_back(geomMesh);

        u_short e = E_NO;
        if((e = this->getVerticesS(newGeometry, geomMesh)) != E_NO)
            continue;
        if((e = this->getSourcesS(newGeometry, geomMesh)) != E_NO)
            continue;

        //array of <triangles>
        domTriangles_Array triArray = geomMesh->getTriangles_array();

        //for each triangles group
        for(u_int t = 0; t < triArray.getCount(); t++)
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

            domTriangles* trianglesInst = (domTriangles*) (domElement*) triArray.get(t);
            if(!trianglesInst)
            {
                    cout << "Stas 42" << endl;
                    return E_FORMAT;
            }

            domInputLocalOffset_Array inoff = trianglesInst->getInput_array();
            if(inoff.getCount() == 0)
            {
                    cout << "Stas 43" << endl;
                    return E_FORMAT;
            }
            inputsNum = inoff.getCount();

            //reference to indices
            domPRef pRef = trianglesInst->getP();
            if(!pRef)
            {
                    cout << "Stas 44" << endl;
                    return E_FORMAT;
            }

            domP *indicesP = (domP*) (domElement*) pRef;

            //P array
            domListOfUInts indicesUintArr = indicesP->getValue();
            if(!indicesP || (indicesUintArr.getCount() == 0))
            {
                    cout << "Stas 45" << endl;
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
            for(u_int k = 0; k < newGeometry.verticesStrs.size(); k++)
            {
                    //match!
                    if(newGeometry.verticesStrs[k].name == verticesName)
                    {
                            verticesStructIndex = k;
                            break;
                    }
            }

            //get normals and texture coordinates
            for(u_int k = 0; k < newGeometry.sourcesStrs.size(); k++)
            {
                    //match!
                    if(newGeometry.sourcesStrs[k].name == normalsName)
                    {
                            normalsStructIndex = k;
                            break;
                    }
            }
            for(u_int k = 0; k < newGeometry.sourcesStrs.size(); k++)
            {
                    //match!
                    if(newGeometry.sourcesStrs[k].name == textCoordsName)
                    {
                            textCoordsStructIndex = k;
                            break;
                    }
            }

            //cout << "We are here 1!!!" << endl;
            //get texture filename
            triMaterialName = trianglesInst->getMaterial();
            //cout << "We are here 2!!!" << endl;

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
                    this->buildAddTrianglesStructureS(verticesStructIndex, normalsStructIndex,
                            textCoordsStructIndex,
                            materialsStructIndex,
                            vertexOffset,
                            normalOffset,
                            textCoordOffset,
                            inputsNum,
                            indicesUintArr,
                            newGeometry
                            );
            }
            else
            {
                    cout << "Stas 46" << endl;
                    //return E_FORMAT;
            }
        } // for each triangles group
        //this->geometries[currGeomId] = newGeometry;
        this->geometries.insert(pair<string, TGeometry> (currGeomId, newGeometry));
    }

    cout << "Geometries size: " << this->geometries.size() << endl;

    return E_NO;
}

//get a scene information from file together with the geometry
u_short ColladaReader::getScene()
{
    cout<<"ColladaReader::getScene() called"<<endl;
    if(!this->root)
    {
        return E_ROOT;
    }

    //scene nodes
    vector<daeElement*> sNodes = this->dae.getDatabase()->typeLookup(domNode::ID());

    //vector<domMesh*> meshes;

    if(sNodes.size() == 0)
    {
            cout << "Stas 28" << endl;
            return E_FORMAT;
    }

    //get all materials
    u_short e = E_NO;
    if((e = this->getMaterials()) != E_NO)
        return e;
    //get all geometries
    if((e = this->getGeometries()) != E_NO)
        return e;

    //iterate through nodes
    for(u_int i = 0; i < sNodes.size(); i++)
    {
        domNode *currNode = (domNode*)sNodes[i];
        domInstance_geometry_Array inGeomArr = currNode->getInstance_geometry_array();

        TNode newNode;        

        //node contains geometry (we are interested only in such nodes)
        if(inGeomArr.getCount() > 0)
        {
            //set node id
            newNode.id = currNode->getId();

            //walk through geometries
            for(u_int j = 0; j < inGeomArr.getCount(); j++)
            {
                domInstance_geometry *instanceGeom = (domInstance_geometry*) (domElement*) inGeomArr[j];

                daeURI geomUri = instanceGeom->getUrl();
                domElement *geomElt = geomUri.getElement();

                if(!geomElt)
                {
                        //return E_RESOLVE;
                        cout << "Stas 29" << endl;
                        continue;
                }

                domGeometry *geom = (domGeometry *)geomElt;

                string currGeomId = geom->getId();
                newNode.geometryIDs.push_back(currGeomId);
            }

            //get rotations
            domRotate_Array rotationsArr = currNode->getRotate_array();

            for(u_int j = 0; j < rotationsArr.getCount(); j++)
            {
                domRotate *rotation = rotationsArr.get(j);

                TRotation rot;

                domFloat4 rotFloats = rotation->getValue();

                if(rotFloats.getCount() != 4)
                {
                    cout << "Stas 50" << endl;
                    continue;
                }

                for(u_int k = 0; k < 4; k++)
                {
                    rot.rotation[k] = rotFloats[k];
                }

                newNode.rotations.push_back(rot);
            }

            //get translations
            domTranslate_Array translationsArr = currNode->getTranslate_array();

            for(u_int j = 0; j < translationsArr.getCount(); j++)
            {
                domTranslate *translation = translationsArr.get(j);

                TTranslation tr;

                domFloat3 trFloats = translation->getValue();

                if(trFloats.getCount() != 3)
                {
                    cout << "Stas 51" << endl;
                    continue;
                }

                for(u_int k = 0; k < 3; k++)
                {
                    tr.translation[k] = trFloats[k];                    
                }

                newNode.translations.push_back(tr);
            }

            //get scalings
            domScale_Array scalingsArr = currNode->getScale_array();

            for(u_int j = 0; j < scalingsArr.getCount(); j++)
            {
                domScale *scaling = scalingsArr.get(j);

                TScaling sc;

                domFloat3 scFloats = scaling->getValue();

                if(scFloats.getCount() != 3)
                {
                    cout << "Stas 52" << endl;
                    continue;
                }

                for(u_int k = 0; k < 3; k++)
                {
                    sc.scaling[k] = scFloats[k];
                }

                newNode.scalings.push_back(sc);
            }


            this->nodes.push_back(newNode);
        }
    }

    return E_NO;
}


//fills in the array of vertices




void ColladaReader::buildAddTrianglesStructureS(int verticesStructIndex, int normalsStructIndex, int textCoordsStructIndex,
                        int materialsStructIndex,
                        int vertexOffset,
                        int normalOffset,
                        int textCoordOffset,
                        int inputsNum,
                        domListOfUInts pArray,
                        TGeometry &geometry)
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
                                        float findex = geometry.verticesStrs[verticesStructIndex].data[k];
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
                                                float findex = geometry.sourcesStrs[normalsStructIndex].data[k];
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
                                                float findex = geometry.sourcesStrs[textCoordsStructIndex].data[k];
                                                //cout << findex << " ";
                                                newTriangles.textureCoords.push_back(findex);
                                        }
                                }
                        }
                }

                i += inputsNum;
        }

        //texture image file name or color
        if(this->materialsStrs[materialsStructIndex].texture)
        {
             newTriangles.texture = true;
             //newTriangles.textureFileName = this->materialsStrs[materialsStructIndex].imageURL;

             //do we have such image already?
             int indexT = this->isTextureLoaded(this->materialsStrs[materialsStructIndex].imageURL);

             if(indexT == -1)
             {
                this->textureFileNames.push_back(this->materialsStrs[materialsStructIndex].imageURL);
                newTriangles.textureIndex = this->textureFileNames.size() - 1;
             }
             else
             {
                 newTriangles.textureIndex = indexT;
             }
        }
        else
        {
            newTriangles.texture = false;
            newTriangles.textureIndex = -1;

            for(u_int y = 0; y < 4; y++)
            {
                newTriangles.color[y] = this->materialsStrs[materialsStructIndex].color[y];
            }
        }

        geometry.trianglesStrs.push_back(newTriangles);
}

//fills in TGeomInfo structure

//returns an index of the texture file name if it is already in textureFileNames
//structure. -1, otherwise
int ColladaReader::isTextureLoaded(string textureFileName)
{
    for(u_int i = 0; i < this->textureFileNames.size(); i++)
    {
        if(this->textureFileNames[i] == textureFileName)
        {
            return i;
        }
    }
    return -1;
}


ColladaReader::~ColladaReader()
{
    if(this->root)
    {
        this->dae.close(this->openedDoc);
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

