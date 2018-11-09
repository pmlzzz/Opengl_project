/**********************************/
/* Simple transformations	      */
/* (C) Bedrich Benes 2012         */
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
//glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
//in house created libraries
#include "shaders.h"    

#pragma warning(disable : 4996)
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glut32.lib")

using namespace std;

GLuint points=0; //number of points to display the object
GLuint stacks=5, slices=5;
bool needRedisplay=false;
GLfloat  sign=+1;

//shader program ID
GLuint shaderProgram;
GLfloat ftime=0.f;
GLint ftimeParameter;
glm::mat4 modelview(1.0f);
GLint modelviewParameter;

//the main window size
GLint wWindow=800;
GLint hWindow=800;


inline void AddVertex(vector <GLfloat> *a, const glm::vec3 *v)
{
	a->push_back(v->x);
	a->push_back(v->y);
	a->push_back(v->z);
}

void CreateSphere(vector <GLfloat> *a, GLfloat r,int stacks, int slices)
{
	glm::vec3 v;

	GLfloat deltaTheta=2*M_PI/(GLfloat)slices;
	GLfloat deltaPhi  =  M_PI/(GLfloat)stacks;

	for (GLint i=0;i<stacks;i++)
	{
		GLfloat phi=i*deltaPhi;
		for (GLint j=0;j<slices;j++)
		{
			GLfloat theta=j*deltaTheta;
//the first triangle
			v=glm::vec3(r*cos(theta)*sin(phi),
				        r*sin(theta)*sin(phi),
				        r*cos(phi));
			AddVertex(a,&v);
			v=glm::vec3(r*cos(theta+deltaTheta)*sin(phi),
				        r*sin(theta+deltaTheta)*sin(phi),
                        r*cos(phi));
			AddVertex(a,&v);
			v=glm::vec3(r*cos(theta)*sin(phi+deltaPhi),
				        r*sin(theta)*sin(phi+deltaPhi),
				        r*cos(phi+deltaPhi));
			AddVertex(a,&v);
//the second triangle
			v=glm::vec3(r*cos(theta+deltaTheta)*sin(phi),
				             r*sin(theta+deltaTheta)*sin(phi),
				             r*cos(phi));
			AddVertex(a,&v);
			v=glm::vec3(r*cos(theta)*sin(phi+deltaPhi),
				        r*sin(theta)*sin(phi+deltaPhi),
				        r*cos(phi+deltaPhi));
			AddVertex(a,&v);
			v=glm::vec3(r*cos(theta+deltaTheta)*sin(phi+deltaPhi),
				        r*sin(theta+deltaTheta)*sin(phi+deltaPhi),
				        r*cos(phi+deltaPhi));
			AddVertex(a,&v);
		}
	}
}

void InitArray(int stacks, int slices)
{
	GLuint vaID;

	vector<GLfloat> v;
	CreateSphere(&v,0.5f,stacks,slices); //radius stacks slices
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
	glColor3f(0,0,0);
	glPointSize(2);
	glLineWidth(2);
	glDrawArrays(GL_TRIANGLES, 0, 3*points);
}
	
void Idle(void)
{
  glClearColor(0.1,0.1,0.1,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  ftime+=0.1;
  glUniform1f(ftimeParameter,ftime);
  glm::mat4 rotateOneX=glm::rotate(glm::mat4(1.0f),ftime,glm::vec3(0,1,1));
  glm::mat4 translateOne=glm::translate(glm::mat4(1.0f),glm::vec3(0.3,0.3,0));
  modelview=glm::mat4(1.0);
  modelview=modelview*translateOne;
  modelview=modelview*rotateOneX;
  glUniformMatrix4fv(modelviewParameter,1,GL_FALSE,glm::value_ptr(modelview));
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
	  case 'S': 
	  case 's': {sign=-sign;break;}
	  case 'a': {stacks++;InitArray(stacks, slices);break;}
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
				}
	  case '+': break;
	  case '-': break;
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


void Mouse(int button,int state,int x,int y)
{
	cout << "Location is " << "[" << x << "'" << y << "]" << endl;
}


void InitializeProgram(GLuint *program)
{
	std::vector<GLuint> shaderList;

//load and compile shaders 	
	shaderList.push_back(CreateShader(GL_VERTEX_SHADER,   LoadShader("shaders/transform.vert")));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, LoadShader("shaders/passthrough.frag")));

//create the shader program and attach the shaders to it
	*program = CreateProgram(shaderList);

//delete shaders (they are on the GPU now)
	std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);

    ftimeParameter =glGetUniformLocation(*program,"time");
	modelviewParameter=glGetUniformLocation(*program,"modelview");
}



int main(int argc, char **argv)
{ 
  glutInitDisplayString("stencil>=2 rgb double depth samples");
  glutInit(&argc, argv);
  glutInitWindowSize(wWindow,hWindow);
  glutInitWindowPosition(500,100);
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
	