
#include "lib.h"
#include "Display.h"

GLfloat Display::_mouseX = 0.0;
GLfloat Display::_mouseY = 0.0;
bool Display::_mouseLeftDown = false;
bool Display::_mouseRightDown = false;
GLfloat Display::_cameraDistance = -25.0;
GLfloat Display::_cameraAngleX = 0.0;
GLfloat Display::_cameraAngleY = 0.0;
Solid* Display::_solid = NULL;


void Display::changeSize( int w, int h )
{
	// 防止除数为0
	if (h == 0)	h = 1;
	float ratio = 1.0* w / h;
	glMatrixMode(GL_PROJECTION);	// 单位化投影矩阵。
	glLoadIdentity();
	glViewport(0, 0, w, h);// 设置视口大小为整个窗口大小
	gluPerspective(45,ratio,1,1000);	// 设置正确的投影矩阵
	glMatrixMode(GL_MODELVIEW);	//下面是设置模型视图矩阵
	glLoadIdentity();
	gluLookAt(0.0,0.0,10.0, 0.0,0.0,0.0,0.0f,1.0f,0.0f);
}


void Display::ProcessNormalKeys( unsigned char key, int x, int y )
{
	switch (key)
	{
	//是ESC则退出
	case 27:
		exit(0);
		break;
	}
}

void Display::ProcessMousePress( int button, int state, int x, int y )
{
	_mouseX = x;
	_mouseY = y;

	if (button == GLUT_LEFT_BUTTON)
	{
		if(state == GLUT_DOWN)
			_mouseLeftDown = true;
		else if (state == GLUT_UP)
			_mouseLeftDown = false;
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
			_mouseRightDown = true;
		else if(state == GLUT_UP)
			_mouseRightDown = false;
	}
}

void Display::ProcessMouseActiveMotion( int x, int y )
{
	if (_mouseLeftDown)
	{
		_cameraDistance += (y - _mouseY) * 0.2f;
		_mouseY = y;
	}

	if (_mouseRightDown)
	{
		_cameraAngleY += (x - _mouseX);
		_cameraAngleX += (y - _mouseY);
		_mouseX = x;
		_mouseY = y;

	}

	glutPostRedisplay();
}

void Display::Run( int argc, char**argv )
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1000, 600);
	glutCreateWindow("CAD Window");
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	glutDisplayFunc(renderScene);	//显示场景
	glutKeyboardFunc(ProcessNormalKeys);	//ESC按下则退出
	glutMouseFunc(ProcessMousePress);
	glutMotionFunc(ProcessMouseActiveMotion);
	glutReshapeFunc(changeSize);	//改变窗口大小后,重新计算投影
	glutMainLoop();
}

void Display::renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glPushMatrix();  
	glTranslatef(0, 0, _cameraDistance);
	glRotatef(_cameraAngleX, 1, 0, 0);
	glRotatef(_cameraAngleY, 0, 1, 0);
;
		glColor3d (1.0, 1.0, 1.0);
		glLineWidth(10.0); 

		Solid *pTempSolid = _solid;
		while (pTempSolid != NULL)
		{
			Edge *edge = pTempSolid->edge;
			while (edge != NULL)
			{
				//EdgesList.push_back(edge);//存储边
				glBegin(GL_LINES);
				GLdouble *XYZ;
				Vertex *v1 = edge->HalfEdgeL->startVe;
				Vertex *v2 = edge->HalfEdgeL->endVe;
				XYZ = v1->point.GetCoord();
				glVertex3d(XYZ[0], XYZ[1], XYZ[2]);
				XYZ = v2->point.GetCoord();
				glVertex3d(XYZ[0], XYZ[1], XYZ[2]);
				edge = edge->nexte;

				glEnd();
			}
			pTempSolid = pTempSolid->nexts;
		}

	glPopMatrix();
	glutSwapBuffers();
}

void Display::SetDisplaySolid( Solid *iSolid )
{
	Display::_solid = iSolid;
}

void Display::tessBegin( GLenum which )
{
	glBegin(which);
}

void Display::vertexBack( GLvoid * data )
{
	const GLdouble *ptr = (const GLdouble*)data;
	glColor3dv(ptr);
	glVertex3dv(ptr); 
}

void Display::tessEnd()
{
	glEnd();
}

