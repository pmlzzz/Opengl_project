/********************************/
/* surface of revolution        */
/* (C) Bedrich Benes 2012       */
/* bbenes ~ at ~ purdue.edu     */
/********************************/

#include <stdio.h>
#include <iostream>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <string>
#include <vector>			//Standard template library class
#include <GL/glew.h>
#include <GL/glut.h>
//in house created libraries
#include "math/vect3d.h"    //for vector manipulation

#pragma warning(disable : 4996)
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glut32.lib")

using namespace std;

GLuint points=0; //number of points to display the object
int steps=2;//# of recursive subdivisions
bool needRedisplay=false;
GLfloat  sign=+1;
const GLfloat defaultIncrement=0.03;
GLfloat  angleIncrement=defaultIncrement;
int i=0;
Vect3d p0, p1, p2, p3, p4, q;
//the main window size
GLint wWindow=800;
GLint hWindow=600;
inline void AddVertex(vector <GLfloat> *a, const GLfloat *A)
{
	a->push_back(A[0]);a->push_back(A[1]);a->push_back(A[2]);
}


//default
GLfloat P(GLfloat u)
{
	return 0.2*sin(4*M_PI*u)+1.f;
}

//sklenicka
GLfloat P1(GLfloat u)
{
	u=1-u;
	return (0.2*sin(2*M_PI*u)*(u+0.2)+0.2);
}

//sphere
GLfloat P2(GLfloat u)
{
	u=1*0.5-1*u;//reparametrize to <-1,1>
	return (sqrt(1.f*0.25-u*u));
}

void input()  
{
	printf("please input the location of the points,just x and y (one by one,-1<x<1,-1<y<1 is better)\n");
	scanf("%f %f", &p0.v[0], &p0.v[1]);
	printf("the first point is : (%f ,%f)\n", p0.v[0], p0.v[1]);
	scanf("%f %f", &p1.v[0], &p1.v[1]);
	printf("the second point is : (%f ,%f)\n", p1.v[0], p1.v[1]);
	scanf("%f %f", &p2.v[0], &p2.v[1]);
	printf("the third point is : (%f ,%f)\n", p2.v[0], p2.v[1]);
	scanf("%f %f", &p3.v[0], &p3.v[1]);
	printf("the fourth point is : (%f ,%f)\n", p3.v[0], p3.v[1]);
}


Vect3d Bezier(Vect3d a, Vect3d b, Vect3d c, Vect3d d, GLfloat u)
{
	GLfloat m1, m2, m3;
	GLfloat n1, n2, n3;
	Vect3d result;
	m3 = 3.0*(b.v[0] - a.v[0]);
	m2 = 3.0*(c.v[0] - b.v[0])-m3;
	m1 = d.v[0] - a.v[0] - m3 - m2;

	n3 = 3.0*(b.v[1] - a.v[1]);
	n2 = 3.0*(c.v[1] - b.v[1]) - n3;
	n1 = d.v[1] - a.v[1] - n3 - n2;
	
	result.v[0] = m1*u*u*u + m2*u*u + m3*u + a.v[0];
	result.v[1] = n1*u*u*u + n2*u*u + n3*u + a.v[1];
	result.v[2] = result.v[0];

	return result;
}

inline Vect3d S(GLfloat u, GLfloat v)
{
	Vect3d q,a,b,c,d;
	q = Bezier(p0, p1, p2, p3, u);
	//q = Vect3d(q.v[0] * sin(2 * M_PI*v), q.v[1], q.v[2] * cos(2 * M_PI*v));
	if (i == 3) { q.v[0] = q.v[0]; q.v[1] = q.v[1]; q.v[2] = 0;  }//show the curve{ q.v[0] = P(u); q.v[1] = u; q.v[2] = P(u);  }
	else if (i == 2) { q.v[0] = q.v[0] *sin(2 * M_PI*v); q.v[1] = q.v[1]; q.v[2] = q.v[2] *cos(2 * M_PI*v); }//rotate y{ q.v[0] = P(u)*sin(2 * M_PI*v); q.v[1] = u; q.v[2] = P(u)*cos(2 * M_PI*v); }
	else if (i == 1) { q.v[0] = q.v[0]; q.v[1] = q.v[1] *sin(2 * M_PI*v); q.v[2] = q.v[1] *cos(2 * M_PI*v); }//rotate x { q.v[0] = P(u); q.v[1] = u*sin(2 * M_PI*v); q.v[2] = u*cos(2 * M_PI*v); }
	else { q.v[0] = q.v[0]; q.v[1] = q.v[1]; q.v[2] = 0; }
	
	return q;

}

void CreateRevo(vector <GLfloat> *a, int n)
{
	GLfloat step=1.f/n;
	for (int i=0;i<n;i++)
	{
		for (int j=0;j<n;j++)
		{
			//lower triangle
			AddVertex(a,S(i*step,j*step));
			AddVertex(a,S((i+1)*step,j*step));
			AddVertex(a,S((i+1)*step,(j+1)*step));
			//upper triangle
			AddVertex(a,S(i*step,j*step));
			AddVertex(a,S((i+1)*step,(j+1)*step));
			AddVertex(a,S(i*step,(j+1)*step));
		}
	}
	
}

void InitArray(int n)
{
	GLuint vaID;

	vector<GLfloat> v;
	CreateRevo(&v,n); 
	glGenVertexArrays(1,&vaID);
	glBindVertexArray(vaID);

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	points=v.size();
	glBufferData(GL_ARRAY_BUFFER, points*sizeof(GLfloat), &v[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(0);

	v.clear(); //no need for the data, it is on the GPU now
}



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


//the main rendering function
void RenderObjects()
{
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glColor3f(0,0,1);
	glPointSize(2);
	glLineWidth(1);
	glDrawArrays(GL_TRIANGLES, 0, points/3);
	glBegin(GL_LINE_STRIP);
		glVertex3f(p0.v[0], p0.v[1], p0.v[2]);
		glVertex3f(p1.v[0], p1.v[1], p1.v[2]);
		glVertex3f(p2.v[0], p2.v[1], p2.v[2]);
		glVertex3f(p3.v[0], p3.v[1], p3.v[2]);
	glEnd();
}
	

void Idle(void)
{
  glClearColor(1.9,1.9,1.9,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  GLMessage("Surface of revolution, Cubic Bézier curves, press x,y,z ,+,-	");
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40,(GLfloat)wWindow/(GLfloat)hWindow,0.01,100); //set the camera
  glMatrixMode(GL_MODELVIEW); //set the scene
  glLoadIdentity();
  gluLookAt(0,2,5,0,0,0,0,1,0);
  static GLfloat angle = 0;
  angle += angleIncrement;
  if (angle >= 360.f) angle = 0.f;
  glRotatef(sign*angle,0,1,0);
  RenderObjects();
  glutSwapBuffers();  
}

void Display(void)
{

}

void Kbd(unsigned char a, int x, int y)//keyboard callback
{
	switch(a)
	{
 	  case 27 : exit(0);break;
	  case 32 : {
		  if (angleIncrement==0) angleIncrement=defaultIncrement;
						    else angleIncrement=0;
		  break;
		}
	  case 's': {sign=-sign;break;}
	  case '-': {
				  steps--; 
				  if (steps<1) steps=1;
				  InitArray(steps);
				  break;
				}
	  case '+': {
				  steps++; 
				  InitArray(steps);
				  break;
				}
	  case 120: {
		  i = 1;
		  printf("Xx key\n");
		  InitArray(steps);
		  break;
	  }
	  case 121: {
		  i = 2;
		  printf("Yy key\n");
		  InitArray(steps);
		  break;
	  }
	  case 122: {
		  i = 3;
		  printf("Zz key\n");
		  InitArray(steps);
		  break;
	  }
	}
	cout << "[steps]=["<< steps << "]" << endl;
	glutPostRedisplay();
}


int main(int argc, char **argv)
{ 
	p0.Set(0, 0, 0);
	p1.Set(1, 0.5, 0);
	p2.Set(-1, 0.5, 0);
	p3.Set(0, 1, 0);
  input();
  glutInitDisplayString("stencil>=2 rgb double depth samples");
  glutInit(&argc, argv);
  glutInitWindowSize(wWindow,hWindow);
  glutInitWindowPosition(500,100);
  glutCreateWindow("Surface of Revolution");
  GLenum err = glewInit();
  if (GLEW_OK != err){
  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
  }
  glutDisplayFunc(Display);
  glutIdleFunc(Idle);
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Kbd); //+ and -
  glutMouseFunc(NULL);
  glutSpecialUpFunc(NULL); 
  glutSpecialFunc(NULL);
  InitArray(steps);
  glutMainLoop();
  return 0;        
}
