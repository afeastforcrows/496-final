#include "SceneGraph.h"

//basic data structure to act as scene graph
void SceneGraph::init(){
	showBB = -1; //keep track of which bounding volume to show
	boolBB = true;
	hitFlag = false; //keep track of whether the pick() hit a model
	//myObjs[0] is the world
	myObjs[1].FL = readPlyModel("ply/trico.ply");
	myObjs[1].FL->translate(-2.0, 0.0, 0.0);
	//calcRitterBoundingSphere(myObjs[0]->center, &(myObjs[0]->radius), myObjs[0]);
	myObjs[2].FL = readPlyModel("ply/spider.ply");
	myObjs[2].FL->translate(2.0, 0.0, 2.0);
	myObjs[3].FL = readPlyModel("ply/shark.ply");
	myObjs[3].FL->translate(2.0, 4.0, 2.0);
	myObjs[4].FL = readPlyModel("ply/urn.ply");
	myObjs[4].FL->translate(-2.0, 4.0, 2.0);
	myObjs[5].FL = readPlyModel("ply/urn.ply");
	myObjs[5].FL->translate(-2.0, -20.0, 2.0);
	std::string myNames[6]={"World", "Trico", "Spider", "Shark", "Urn", "Test"};
	for(int i = 0; i < numObj; i++){
		calcRitterBoundingSphere(myObjs[i].FL->center, &(myObjs[i].FL->radius), myObjs[i].FL);
		myObjs[i].BB.update(Vec3(myObjs[i].FL->center[0], myObjs[i].FL->center[1], myObjs[i].FL->center[2]), myObjs[i].FL->radius);
	}

	worldBB.update(Vec3(0,0,0), 10);
	myObjs[0].init("World", worldBB, NULL);	//scene object that represents the world
	for(int n = 1; n < numObj; n++){			//All objects begin as children of World
		myObjs[n].init(myNames[n], myObjs[n].BB, myObjs[n].FL);
		myObjs[0].addChild(&myObjs[n]);
		myObjs[n].addParent(&myObjs[0]);
	}
}

void SceneGraph::drawBoundingSphere(Vec3 center, float s){
	FaceList *sphere = readPlyModel("ply/sphere.ply");
	sphere -> translate(center[0], center[1], center[2]);
	sphere -> scale(s);
	sphere -> drawSphere(); 
}

void SceneGraph::updatePly(){
	for(int p = 1; p<numObj; p++){
		calcRitterBoundingSphere(myObjs[p].FL->center, &(myObjs[p].FL->radius), myObjs[p].FL);
	}
}

bool SceneGraph::cullIt(Vec3 centerPosition, Vec3 eyePosition, Vec3 upVector, Vec3 midPoint, Mat4 modelViewMatrix){		
	
	Mat4 modelViewInverse = modelViewMatrix.inverse( );
	Vec3 testVec;
	Vec4 planes[4];
	planes[0] = modelViewInverse*Vec4(-1,1,0,1);
	
	
	glLineWidth(5);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(centerPosition[0], centerPosition[1], centerPosition[2]);
	glVertex3f(planes[0][0], planes[0][1], planes[0][2]);
	glEnd();

	
	
	for(int x=0; x<4; x++){
		testVec = normalize(midPoint);
		testVec -= centerPosition;
		
			//printf("Dot %d: %f \n", x, dot(testVec,planes[x]));
		
		if((dot(Vec4(testVec,1),planes[x]))>0){
			//printf("boop: (%f, %f, %f) - %d \n", centerPosition[0], centerPosition[1], centerPosition[2], x);			
			//return false;
		}
	}

	
	return true;
}

void SceneGraph::draw(Vec3 centerPosition, Vec3 eyePosition, Vec3 upVector, Mat4 modelViewMatrix){
	for(int p = 1; p <numObj; p++){
		if(myObjs[p].draw)
		{
			myObjs[p].FL->draw();//call FaceList draw() function
			//myObjs[p].BB.drawBB();
		}
		myObjs[p].BB.update(Vec3(myObjs[p].FL->center[0], myObjs[p].FL->center[1], myObjs[p].FL->center[2]), myObjs[p].FL->radius);
	}
}

void SceneGraph::update(Vec3 centerPosition, Vec3 eyePosition, Vec3 upVector, Mat4 modelViewMatrix){
	//float dist;
	//check for collisions
	/*for(int x = 0; x < numObj; x++){
		for(int y = x+1; y < numObj; y++){
			dist = distance(myObjs[x].BB.center, myObjs[y].BB.center);
			if(dist<=(float(myObjs[x].BB.width+ myObjs[y].BB.width))){
				//printf("COLLISION with %s and %s -- %f\n", myObjs[x].name.c_str(), myObjs[y].name.c_str(), dist );
				if(myObjs[x].BB.width > myObjs[y].BB.width){
					//myObjs[x].BB is larger and will become the parent
					//but first we have to make sure the smaller obj doesn't already have a parent
				}
				else{
					//myObjs[y].BB is larger and will become the parent
					//but first we have to make sure the smaller obj doesn't already have a parent
				}
			}
		}
	}*/
	//update parents/children
	updatePly();
	draw(centerPosition, eyePosition, upVector, modelViewMatrix);
}

float SceneGraph::distance(Vec3 a, Vec3 b){
	return sqrt(	( abs(a[0]-b[0]) ) *( abs(a[0]-b[0]) )+
			( abs(a[1]-b[1]) ) *( abs(a[1]-b[1]) )+
			( abs(a[2]-b[2]) ) *( abs(a[2]-b[2]) ));
}

void SceneGraph::testPar(){
	//myObjs[1].addChild(&myObjs[2]);
}

void SceneGraph::translate(SceneObj *s, float n){
	s->FL->translate(0,n,0);
	for(int n = 0; n < s->numChildren; n++){
		s->children[n]->FL->translate(0,n,0);
	}
}

void SceneGraph::drawSphere(float radius, int slices, int stacks, double x, double y, double z){
	GLUquadricObj *quadObj;
	quadObj = gluNewQuadric();
	//assert(quadObj);
	
	//gluQuadricDrawStyle(quadObj, GLU_FILL);
	//gluQuadricNormals(quadObj, GLU_SMOOTH);
	
	gluQuadricDrawStyle(quadObj, GLU_LINE);
	//gluQuadricNormals(quadObj, GLU_SMOOTH);

	glTranslatef(.05f,0.1f,z);
	gluSphere(quadObj, radius, slices, stacks);

	

}
