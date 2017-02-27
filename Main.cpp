/*
* Programmer: Jason Carruthers
* Last modified: 4/17/16
*/
#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <list>
#include <vector>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include "Main.h"
#include "MyMath.h"
#include "General.h"
#include "Rectangle.h"
#include "Curves.h"
#include "GUI.h"
#include "Graph.h"







/*
* Functions
*/
void TestRectangleIntersection()
{
	MyRectangle rect1 = MyRectangle(Vector2I(100, 100), Vector2I(100, 100), GetRandomColor().GetColor3());
	MyRectangle rect2 = MyRectangle(Vector2I(150, 200), Vector2I(50, 50), GetRandomColor().GetColor3());

	if (rect1.ContainsRect(rect2))
		int x = 5;
}

int main(int argc, char *argv[])
{
	/*Seed the random number generator*/
	srand(static_cast<int>(time(0)));


	glutInit(&argc, argv);
	InitGlobalVariables();
	InitGUI();
	InitOpenGLStuff();
	//TestRectangleIntersection();

	glutDisplayFunc(Display);
	glutReshapeFunc(ReshapeWindow);
	glutKeyboardFunc(KeyboardInput);
	glutMouseFunc(MouseInput);
	glutPassiveMotionFunc(PassiveMouseMove);
	glutMotionFunc(NonpassiveMouseMove);


	glutMainLoop();
	return 0;
}

void InitGlobalVariables()
{
	pixelBuffer = new float[WINDOW_WIDTH * WINDOW_HEIGHT * 3];

	curveIndex = MAGIC_GARBAGE;
	SetCurveResolution(250);
	splitT = 0.5f;
	isUsingBernstein = false; /*By default, use the deCasteljau algorithm*/
	controlPointsVisible = false; 
	controlPolygonVisible = false; 
	MBBVisible = false; 

	InitPascalsTriangle();
}

void InitOpenGLStuff()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	int mainWindow = glutCreateWindow("The de Casteljau Algorithm and Bezier Curves");
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}


void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	/*Stuff to display goes here*/
	glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, pixelBuffer);
	DrawGUI();
	if (graphModeIsOn)
		graph->Draw();
	/*End stuff to display*/
	
	
	glutPostRedisplay();
	
	glFlush();
	glutSwapBuffers();
}

void ReshapeWindow(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);

	glClear(GL_COLOR_BUFFER_BIT);
}

void KeyboardInput(unsigned char key, int mouseX, int mouseY)
{
	switch (key)
	{
	case 'W': /*Increment curveIndex*/
	case 'w':
		if (curveIndex == MAGIC_GARBAGE)
			curveIndex = 0;
		else
		{
			curveIndex = (curveIndex + 1) % curveVec.size();
			curveIndex = (curveIndex == 0) ? MAGIC_GARBAGE : curveIndex;
		}
		/*Force redraw of curves*/
		for (unsigned int i = 0; i < curveVec.size(); i++)
			HandleCurveDraw(i);
		break;
	case 'S': /*Decrement curveIndex*/
	case 's':
		curveIndex = curveIndex - 1;
		if (curveIndex == -1)
			curveIndex = MAGIC_GARBAGE;
		else
			curveIndex = (curveIndex == MAGIC_GARBAGE - 1) ? curveVec.size() - 1 : curveIndex;
		/*Force redraw of curves*/
		for (unsigned int i = 0; i < curveVec.size(); i++)
			HandleCurveDraw(i);
		break;
	case 'F': /*Split curve*/
	case 'f':
		if (curveIndex >= 0 && (unsigned int)curveIndex < curveVec.size())
		{
			/*Split the curve in focus*/
			curveVec.push_back(curveVec[curveIndex]->Split(splitT));

			/*Clear any intermediate lines that may have been onscreen, then force a redraw of all curves.*/
			for (unsigned int i = 0; i < curveVec.size(); i++)
				curveVec[i]->ClearIntermediateLines();
			for (unsigned int i = 0; i < curveVec.size(); i++)
				HandleCurveDraw(i);
		}
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

void MouseInput(int button, int action, int mouseX, int mouseY)
{
	/*Pass along mouse info for updating checkboxes*/
	checkboxHandler->MonitorCheckboxes(mouseX, mouseY, button, action);

	/*Pass along mouse info for update sliders*/
	if ( !graphModeIsOn )
		sliderHandler->MonitorSliders(mouseX, mouseY, button, action);

	/*If left mouse button pressed down and the user has clicked within the curve space,
	  plot a control point*/
	if ( !graphModeIsOn )
	{
		if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN &&
			curveSpace->ContainsPoint(mouseX, mouseY))
		{
			HandleNewControlPoint(Vector2I(mouseX, WINDOW_HEIGHT - mouseY));
		}
	}
}

void PassiveMouseMove(int mouseX, int mouseY)
{
	/*Pass along mouse info for updating checkboxes*/
	checkboxHandler->MonitorCheckboxes(mouseX, mouseY, MAGIC_GARBAGE, MAGIC_GARBAGE);
}

void NonpassiveMouseMove(int mouseX, int mouseY)
{
	/*Pass along mouse info for updating checkboxes*/
	checkboxHandler->MonitorCheckboxes(mouseX, mouseY, MAGIC_GARBAGE, MAGIC_GARBAGE);

	/*Pass along mouse info for update sliders*/
	if (!graphModeIsOn)
		sliderHandler->MonitorSliders(mouseX, mouseY, MAGIC_GARBAGE, MAGIC_GARBAGE);
}