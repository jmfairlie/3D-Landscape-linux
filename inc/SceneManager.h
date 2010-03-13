


class SceneManager
{
	
public:
	SceneManager();
	~SceneManager();
	void loadScene(char* file);
	TGeomInfo* getModel(int modelId);
	void render();
	
private:
	TGeomInfo *models;
	Object3D* objects;	
}