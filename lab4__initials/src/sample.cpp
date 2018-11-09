/**********************************/
/* GLSL uniform parameter - bulge */
/* (C) Bedrich Benes 2016         */
/* bbenes ~ at ~ purdue.edu       */
/**********************************/

#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <string.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <string>
#include <vector>			//Standard template library class
#include <GL/glew.h>
#include <GL/glut.h>
//in house created libraries
#include "math/vect3d.h"    //for vector manipulation
#include "shaders.h"    

#pragma warning(disable : 4996)
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glut32.lib")

using namespace std;

GLuint points=0; //number of points to display the object
GLuint stacks=15, slices=15;
bool needRedisplay=false;
GLfloat  sign=+1;
Vect3d p0, p1, p2, p3, p4, p5, p6, p7;
//shader program ID
GLuint shaderProgram;
GLfloat ftime, fbulge=0.0;
GLint ftimeParameter,fbulgeParameter;
GLfloat fx = 0.0f, fy = 0.0f, fz = 0.0f;
GLfloat fs=1.0f,fc=0.0f;
GLint fxParameter, fyParameter,fzParameter, fsParameter, fcParameter;



//the main window size
GLint wWindow=600;
GLint hWindow=600;

inline void AddVertex(vector <GLfloat> *a, const GLfloat *A)
{
	a->push_back(A[0]);a->push_back(A[1]);a->push_back(A[2]);
}

void CreateSphere(vector <GLfloat> *a, GLfloat r,int stacks, int slices)
{
	Vect3d v;

	GLfloat deltaTheta=2*M_PI/(GLfloat)slices;
	GLfloat deltaPhi  =  M_PI/(GLfloat)stacks;

	for (GLint i=0;i<stacks;i++)
	{
		GLfloat phi=i*deltaPhi;
		for (GLint j=0;j<slices;j++)
		{
			GLfloat theta=j*deltaTheta;
//the first triangle
			v.Set(r*cos(theta)*sin(phi),
				  r*sin(theta)*sin(phi),
				             r*cos(phi));
			AddVertex(a,v);
			v.Set(r*cos(theta+deltaTheta)*sin(phi),
				  r*sin(theta+deltaTheta)*sin(phi),
				                        r*cos(phi));
			AddVertex(a,v);
			v.Set(r*cos(theta)*sin(phi+deltaPhi),
				  r*sin(theta)*sin(phi+deltaPhi),
				             r*cos(phi+deltaPhi));
			AddVertex(a,v);
//the second triangle
			v.Set(r*cos(theta+deltaTheta)*sin(phi),
				  r*sin(theta+deltaTheta)*sin(phi),
				                        r*cos(phi));
			AddVertex(a,v);
			v.Set(r*cos(theta)*sin(phi+deltaPhi),
				  r*sin(theta)*sin(phi+deltaPhi),
				             r*cos(phi+deltaPhi));
			AddVertex(a,v);
			v.Set(r*cos(theta+deltaTheta)*sin(phi+deltaPhi),
				  r*sin(theta+deltaTheta)*sin(phi+deltaPhi),
				                        r*cos(phi+deltaPhi));
			AddVertex(a,v);
		}
	}
}

void InitArray(int stacks, int slices)
{
	GLuint vaID;

	vector<GLfloat> v;
	p0.Set(-0.5,-0.5,-0.5);
	p1.Set(0.5,-0.5,-0.5);
	p2.Set(-0.5,0.5,-0.5);
	p3.Set(0.5,0.5,-0.5);
	p4.Set(-0.5,-0.5,0.5);
	p5.Set(0.5,-0.5,0.5);
	p6.Set(-0.5,0.5,0.5);
	p7.Set(0.5,0.5,0.5);
	v.push_back(p0.v[0]);
	v.push_back(p0.v[1]);
	v.push_back(p0.v[2]);
	v.push_back(p2.v[0]);
	v.push_back(p2.v[1]);
	v.push_back(p2.v[2]);
	v.push_back(p3.v[0]);
	v.push_back(p3.v[1]);
	v.push_back(p3.v[2]);
	v.push_back(p1.v[0]);
	v.push_back(p1.v[1]);
	v.push_back(p1.v[2]);

	v.push_back(p0.v[0]);
	v.push_back(p0.v[1]);
	v.push_back(p0.v[2]);
	v.push_back(p4.v[0]);
	v.push_back(p4.v[1]);
	v.push_back(p4.v[2]);
	v.push_back(p6.v[0]);
	v.push_back(p6.v[1]);
	v.push_back(p6.v[2]);
	v.push_back(p2.v[0]);
	v.push_back(p2.v[1]);
	v.push_back(p2.v[2]);

	v.push_back(p2.v[0]);
	v.push_back(p2.v[1]);
	v.push_back(p2.v[2]);
	v.push_back(p6.v[0]);
	v.push_back(p6.v[1]);
	v.push_back(p6.v[2]);
	v.push_back(p7.v[0]);
	v.push_back(p7.v[1]);
	v.push_back(p7.v[2]);
	v.push_back(p3.v[0]);
	v.push_back(p3.v[1]);
	v.push_back(p3.v[2]);

	v.push_back(p3.v[0]);
	v.push_back(p3.v[1]);
	v.push_back(p3.v[2]);
	v.push_back(p1.v[0]);
	v.push_back(p1.v[1]);
	v.push_back(p1.v[2]);
	v.push_back(p5.v[0]);
	v.push_back(p5.v[1]);
	v.push_back(p5.v[2]);
	v.push_back(p7.v[0]);
	v.push_back(p7.v[1]);
	v.push_back(p7.v[2]);

	v.push_back(p7.v[0]);
	v.push_back(p7.v[1]);
	v.push_back(p7.v[2]);
	v.push_back(p5.v[0]);
	v.push_back(p5.v[1]);
	v.push_back(p5.v[2]);
	v.push_back(p4.v[0]);
	v.push_back(p4.v[1]);
	v.push_back(p4.v[2]);
	v.push_back(p6.v[0]);
	v.push_back(p6.v[1]);
	v.push_back(p6.v[2]);

	v.push_back(p4.v[0]);
	v.push_back(p4.v[1]);
	v.push_back(p4.v[2]);

	
	//radius stacks slices

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
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); //GL_LINE
	glColor3f(0,0,0);
	glPointSize(2);
	glLineWidth(2);
	glDrawArrays(GL_LINE_LOOP, 0, points / 3);
}
	
void Idle(void)
{
  glClearColor(1.0f,1.f,1.f,1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  ftime+=0.5;
  glUniform1f(ftimeParameter,ftime);
  glUniform1f(fbulgeParameter,fbulge);
  glUniform1f(fxParameter, fx);
  glUniform1f(fyParameter, fy);
  glUniform1f(fzParameter, fz);
  glUniform1f(fsParameter, fs);
  glUniform1f(fcParameter, fc);
  glUseProgram(shaderProgram);
 
  RenderObjects();
  glutSwapBuffers();  
}

void Display(void)
{

}

//keyboard callback
void Kbd(unsigned char a, int x, int y)
{
	switch(a)
	{
 	  case 27 : exit(0);break;
	  case 32 : {needRedisplay=!needRedisplay;break;}
	  //case 'S': 
	  //case 's': {sign=-sign;break;}
	  /*case 'a': {stacks++;InitArray(stacks, slices);break;}
	  case 'A': {
				  stacks--; 
				  if (stacks<2) stacks=2;
				  InitArray(stacks, slices);
				  break;
				}
	  case 'b': {slices++;InitArray(stacks, slices);break;}
	  case 'B': {
				  slices--; 
				  if (slices<2) slices=2;
				  InitArray(stacks, slices);
				  break;
				}*/
	  case 'x': {fx-=0.1f; break; }
	  case 'y': {fy-=0.1f; break; }
	  case 'z': {fz-=0.1f; break; }
	  case 'X': {fx += 0.1f; break; }
	  case 'Y': {fy += 0.1f; break; }
	  case 'Z': {fz += 0.1f; break; }
	  case 's': {fs -= 0.1f; break; }
	  case 'S': {fs += 0.1f; break; }
	  case 'r': {fc=1.0f; break; }
	  case 'g': {fc=2.0f; break; }
	  case 'b': {fc=3.0f; break; }
	  case '+': {fbulge+=0.01f;break;}
	  case '-': {fbulge-=0.01f;break;}
	}
	cout << "[fbulge]=["<< fbulge << "]" << endl;
	cout << "[fc]=[" << fc << "]" << endl;
	cout << "[fx]=[" << fx << "]" << endl;
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


void Mouse(int button,int state,int x,int y)
{
	cout << "Location is " << "[" << x << "'" << y << "]" << endl;
}


void InitializeProgram(GLuint *program)
{
	std::vector<GLuint> shaderList;

//load and compile shaders 	
	shaderList.push_back(CreateShader(GL_VERTEX_SHADER,   LoadShader("shaders/bulge.vert")));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, LoadShader("shaders/bulge.frag")));

//create the shader program and attach the shaders to it
	*program = CreateProgram(shaderList);

//delete shaders (they are on the GPU now)
	std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);

    ftimeParameter =glGetUniformLocation(*program,"time");
	fbulgeParameter=glGetUniformLocation(*program,"bulge");
	fxParameter = glGetUniformLocation(*program, "x");
	fyParameter = glGetUniformLocation(*program, "y");
	fzParameter = glGetUniformLocation(*program, "z");
	fsParameter = glGetUniformLocation(*program, "s");
	fcParameter = glGetUniformLocation(*program, "c");

}



int main(int argc, char **argv)
{ 
  glutInitDisplayString("stencil>=2 rgb double depth samples");
  glutInit(&argc, argv);
  glutInitWindowSize(wWindow,hWindow);
  glutInitWindowPosition(200,100);
  glutCreateWindow("Bulge using GLSL");
  GLenum err = glewInit();
  if (GLEW_OK != err){
   fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
  }
  glutDisplayFunc(Display);
  glutIdleFunc(Idle);
  glutMouseFunc(Mouse);
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Kbd); //+ and -
  glutSpecialUpFunc(SpecKbdRelease); //smooth motion
  glutSpecialFunc(SpecKbdPress);
  InitArray(stacks,slices);
  InitializeProgram(&shaderProgram);
  glutMainLoop();
  return 0;        
}
	