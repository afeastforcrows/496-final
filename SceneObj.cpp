//////////////////////////////////////////////////////////////////
//	Walter Wyatt Dorn					//
//	CPSC 486						//
//	Camera Control & View Frustum Culling – Assignment 3	//
//	Dr. Shafae - CSU Fullerton				//	
//	5/18/16							//
//////////////////////////////////////////////////////////////////

#include "SceneObj.h"

//basic data structure to act as objects in scene graph
void SceneObj::init(std::string n, BBox bb, FaceList *fl){
	name = n;
	parent = NULL;
	numChildren = 0;
	BB = bb;
	FL = fl;
	draw = true;
}

void SceneObj::addParent(SceneObj *p){
	parent = p;
	p->addChild(this);
}

void SceneObj::addChild(SceneObj *c){
	for(int n = 0; n < numChildren; n++)
	{
		if(children[n] == c){
			return;
		}
	}
	children[numChildren] = c;
	numChildren++;
}

void SceneObj::removeChild(SceneObj *c){
	children[numChildren] = NULL;
	numChildren--;
}

bool SceneObj::isRoot(){
	return (parent==NULL);
}

bool SceneObj::isLeaf(){
	return (numChildren==0);
}

