#include "GFXMath.h"
#include "BBox.h"
#include <cmath>

#ifndef Included_SceneObj_H
#define Included_SceneObj_H

class SceneObj{
	//basic data structure to act as objects in scene graph
	public:
	std::string name;
	SceneObj *parent;
	SceneObj *children[5];
	int numChildren;
	BBox BB;
	FaceList *FL = readPlyModel("ply/trico.ply");

	void init(std::string n, BBox bb, FaceList *fl);

	void addParent(SceneObj *p);

	void addChild(SceneObj *c);

	void removeChild(SceneObj *c);

	bool isRoot();

	bool isLeaf();
};
#endif
