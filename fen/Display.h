
#include "lib.h"

#ifndef _HALFEDGES_H_
#define _HALFEDGES_H_
#include "HalfEdge.h"
#endif


class Display
{
public:

	static void Run(int argc, char**argv);
	
	static void SetDisplaySolid(Solid *iSolid);

private:


	static void renderScene();

	static void changeSize(int w, int h);

	static void ProcessNormalKeys(unsigned char key, int x, int y);


	static void ProcessMousePress(int button, int state, int x, int y);
	

	static void ProcessMouseActiveMotion(int x, int y);


	static void CALLBACK tessBegin(GLenum which);
	static void CALLBACK vertexBack(GLvoid * data);
	static void CALLBACK tessEnd();

	static bool _mouseLeftDown, _mouseRightDown;//鼠标的左右键是否按下？
	static GLfloat _mouseX, _mouseY;	//记录鼠标位置
	static GLfloat _cameraDistance;	//控制屏幕远近
	static GLfloat _cameraAngleX;	
	static GLfloat _cameraAngleY;	
	static Solid *_solid;		
};