/********************************/
/* .net 2015 Math Example       */
/* (C) Bedrich Benes 2016       */
/* bbenes ~ at ~ purdue.edu     */
/********************************/

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <math.h>
#include <time.h>
#include <string>
#include <vector>			//Standard template library class
#include <GL/glut.h>
#pragma warning(disable : 4996)

//in house created libraries
#include "math/vect3d.h"    //for vector manipulation

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
using namespace std;

bool needRedisplay=false;

//the main container of the polylines is a STL vector of pointers to 
vector <Vect3d> points;
vector <Vect3d> vectors;
Vect3d testPoint;
Vect3d originPoint;

//the main window size
GLint wWindow=800;
GLint hWindow=600;

/*********************************
Some OpenGL-related functions
**********************************/
//displays the text message in the GL window
void GLMessage(char *message)
{
	static int i;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.f,100.f,0.f,100.f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3ub(0,0,255);
	glRasterPos2i(10,10);
	for(i=0;i<(int)strlen(message); i++ ) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,message[i]);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

//called when a window is reshaped
void Reshape(int w, int h)
{
  glViewport(0,0,w, h);       
  glEnable(GL_DEPTH_TEST);
//remember the settings for the camera
  wWindow=w;
  hWindow=h;
}

void DisplayPolygon(vector <Vect3d> &points) {
	glColor3ub(0, 0, 255);
	glLineWidth(4);
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < points.size(); i++)
		glVertex3fv(points[i]);
	glEnd();
}

void DisplayPoint(Vect3d p) {
	glColor3ub(0, 0, 0);
	glPointSize(4);
	glBegin(GL_POINTS);
	glVertex3fv(p);
	glEnd();
}

//This is the main routine you need to focus on!
//input is a set of points (3D vectors with z coordinate set to 0)
//and the test point
//return true or false
bool Winding(vector <Vect3d> points, Vect3d testPoint) {
	Vect3d v;
	Vect3d nv;
	Vect3d direction;
	float alpha = 0;
	int m = points.size();
	//printf("say hello!!!!!!!!!!!!!%d\n ", m);

	for (int i = 0; i < m; i++)
	{
		//points[i].Normalize();
		//printf("point%d:     (%f,%f,%f)........%f\n", i + 1, points[i].v[0], points[i].v[1], points[i].v[2], points[i].Length());
		points[i].Subtract(testPoint, v);
		//printf("vector%d:     (%f,%f,%f)........%f\n", i + 1, v.v[0], v.v[1], v.v[2], v.Length());
		nv = v.GetNormalized();
		//printf("Normalizedpoint%d:     (%f,%f,%f)........%f\n\n", i + 1, nv.v[0], nv.v[1], nv.v[2], nv.Length());
		vectors.push_back(nv);
	}
	float delta = 0;
	for (int i = 0; i < m; i++)
	{
		if (i != m-1) { delta = acos(vectors[i].Dot(vectors[i + 1])) * 180 / M_PI; direction = vectors[i].Cross(vectors[i + 1]); }
		else {
			delta = acos(vectors[i].Dot(vectors[0])) * 180 / M_PI; direction = vectors[i].Cross(vectors[0]);
		}
		if (direction.v[2] < 0) { delta *= (-1); }
		alpha += delta;
		//printf("direction:     (%f,%f,%f)........%f\n", direction.v[0], direction.v[1], direction.v[2], direction.Length());
		//printf("alpha:  %f\n", alpha);
	}
	if (abs(alpha) <= 1)
	{
		//printf("The total alpha is %f degree\n  The point is not in the polygon\n", alpha);
		return false;
	}
	else
	{
		//printf("The total alpha is %f\n  The point is in the polygon\n", alpha);
		return true;
	}
}

void Idle(void)
{
  static clock_t tStart=0, tEnd=1;
  tStart=clock();
  glClearColor(0.5,0.5,0.5,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  char message[200];
  double fps=1.f/((float)tEnd-tStart);
  sprintf(message, "Is the point inside? %s", Winding(points, testPoint) ? "true" : "false");
  GLMessage(message);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-10,10,-10,10,0,10); //set the camera
  glMatrixMode(GL_MODELVIEW); //set the scene
  glLoadIdentity();
  DisplayPolygon(points);
  DisplayPoint(testPoint);
  glutSwapBuffers();
  tEnd=clock();
}


void Display(void)
{
}

//keyboard callback
void Kbd(unsigned char a, int x, int y)
{
	cout << "The key you have pressed is "<< a << endl;
	switch(a)
	{
 	  case 27 : exit(0);break;
	  case 32 : {needRedisplay=!needRedisplay;break;}
	}
	int modifier=glutGetModifiers();
	switch (modifier)
	{
		case GLUT_ACTIVE_SHIFT: cout << "shift was pressed ";break;
		case GLUT_ACTIVE_ALT:   cout << "alt was pressed ";break;
		case GLUT_ACTIVE_CTRL:  cout << "ctrl was pressed ";break;
		case GLUT_ACTIVE_SHIFT | GLUT_ACTIVE_ALT:  cout << "shift AND alt were pressed ";break;
		default: break;
	}

	glutPostRedisplay();
}


//special keyboard callback
void SpecKbdPress(int a, int x, int y)
{
   	switch(a)
	{
 	  case GLUT_KEY_LEFT  : 
		  {
			  break;
		  }
	  case GLUT_KEY_RIGHT : 
		  {
			break;
		  }
 	  case GLUT_KEY_DOWN    : 
		  {
			break;
		  }
	  case GLUT_KEY_UP  :
		  {
			break;
		  }

	}
	glutPostRedisplay();
}

//called when a special key is released
void SpecKbdRelease(int a, int x, int y)
{
	switch(a)
	{
 	  case GLUT_KEY_LEFT  : 
		  {
			  break;
		  }
	  case GLUT_KEY_RIGHT : 
		  {
			  break;
		  }
 	  case GLUT_KEY_DOWN  : 
		  {
			break;
		  }
	  case GLUT_KEY_UP  :
		  {
			break;
		  }
	}
	glutPostRedisplay();
}

//Initialization of the polygon
void InitShapes()
{
	Vect3d p;
	p.Set(0, 0, 0);
	points.push_back(p);
	p.Set(-2,-2, 0);
	points.push_back(p);
	p.Set(0, 4, 0);
	points.push_back(p);
	p.Set(-1, -3, 0);
	points.push_back(p);
	p.Set(3, 4, 0);
	points.push_back(p);
	p.Set(3, 0, 0);
	points.push_back(p);
	testPoint.Set(1.7, 2, 0);

}

void Mouse(int button,int state,int x,int y)
{
	cout << "Location is " << "[" << x << "'" << y << "]" << endl;
}

int main(int argc, char **argv)
{ 
//  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  
  glutInitDisplayString("stencil>=2 rgb double depth samples");
  glutInit(&argc, argv);
  glutInitWindowSize(wWindow,hWindow);
  glutInitWindowPosition(500,100);
  glutCreateWindow("My _KHOOL_ OPENGL APP!");
  glutIgnoreKeyRepeat(GL_FALSE);                     
  glutDisplayFunc(Display);
  glutIdleFunc(Idle);
  glutMouseFunc(Mouse);
  glutReshapeFunc(Reshape);
  glutIgnoreKeyRepeat(1);//YES, ignore key repeat please
  glutKeyboardFunc(Kbd); //+ and -
  glutSpecialUpFunc(SpecKbdRelease); //smooth motion
  glutSpecialFunc(SpecKbdPress);
  InitShapes();
  glutMainLoop();
  
  return 0;        
}
