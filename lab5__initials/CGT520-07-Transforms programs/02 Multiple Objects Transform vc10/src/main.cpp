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
#include "shapes.h"    

#pragma warning(disable : 4996)
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glut32.lib")

using namespace std;

bool needRedisplay=false;
GLfloat  sign=+1;
GLint stacks=5, slices=5;
vector<ShapesC*> shapes;

//shader program ID
GLuint shaderProgram;
GLfloat ftime=0.f;
GLint ftimeParameter;
glm::mat4 modelview(1.0f);
GLint modelviewParameter;

//the main window size
GLint wWindow=800;
GLint hWindow=800;

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

	for (unsigned int i=0;i<shapes.size();i++)
		shapes[i]->Render();
}
	
void Idle(void)
{
  glClearColor(0.1,0.1,0.1,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  ftime+=0.1;
  glUniform1f(ftimeParameter,ftime);
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
	  case 'a': {stacks++;break;}
	  case 'A': {
				  stacks--; 
				  if (stacks<2) stacks=2;
				  break;
				}
	  case 'b': {slices++;break;}
	  case 'B': {
				  slices--; 
				  if (slices<2) slices=2;
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


GLuint InitializeProgram(GLuint *program)
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
	return modelviewParameter;
}

void InitShapes(GLuint modelviewParameter)
{
	const float range=1.5f;
	ShapesC *tmp;
	for (int i=0;i<100;i++)
	{
		tmp=new SphereC(7,7,0.02);
		glm::vec3 t=glm::vec3(range*rand()/(float)RAND_MAX-range/2,
						      range*rand()/(float)RAND_MAX-range/2,
							  range*rand()/(float)RAND_MAX-range/2);
		glm::mat4 translateOne=glm::translate(glm::mat4(1.0f),t);
		modelview=glm::mat4(1.0);
		modelview=modelview*translateOne;
		tmp->SetMVP(modelview);
		tmp->SetMatrixParamToShader(modelviewParameter);
		shapes.push_back(tmp);
	}
	for (int i=0;i<100;i++)
	{
		tmp=new CubeC();
		glm::vec3 t=glm::vec3(range*rand()/(float)RAND_MAX-range/2,
						      range*rand()/(float)RAND_MAX-range/2,
							  range*rand()/(float)RAND_MAX-range/2);
		glm::mat4 translate=glm::translate(glm::mat4(1.0f),t);
		glm::mat4 scale=glm::scale(glm::mat4(1.0f),glm::vec3(0.1,0.1,0.1));
		modelview=glm::mat4(1.0);
		modelview=modelview*translate*scale;
		tmp->SetMVP(modelview);
		tmp->SetMatrixParamToShader(modelviewParameter);
		shapes.push_back(tmp);
	}
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
  GLuint modelviewParameter=InitializeProgram(&shaderProgram);
  InitShapes(modelviewParameter);
  glutMainLoop();
  return 0;        
}
	