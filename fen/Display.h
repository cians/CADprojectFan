
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

	static bool _mouseLeftDown, _mouseRightDown;//�������Ҽ��Ƿ��£�
	static GLfloat _mouseX, _mouseY;	//��¼���λ��
	static GLfloat _cameraDistance;	//������ĻԶ��
	static GLfloat _cameraAngleX;	
	static GLfloat _cameraAngleY;	
	static Solid *_solid;		
};