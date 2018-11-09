/**********************************/
/* Lighting					      
   (C) Bedrich Benes 2012         
   Diffuse and specular per vertex.
   bbenes ~ at ~ purdue.edu       */
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
#include <glm/gtc/half_float.hpp>
#include "shaders.h"    
#include "shapes.h"    
#include "lights.h"    

#pragma warning(disable : 4996)
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glut32.lib")

using namespace std;

bool needRedisplay=false;
ShapesC* sphere;
int rot = 1;
int rott = 1;

//shader program ID
GLuint shaderProgram;
GLfloat ftime1 = 0.f;
GLfloat ftime2 = 0.f;
GLfloat tmp1 = 0.f;
GLfloat tmp2 = 0.f;
glm::mat4 view=glm::mat4(1.0);
glm::mat4 proj=glm::perspective(80.0f,//fovy
				  		        1.0f,//aspect
						        0.01f,1000.f); //near, far
class ShaderParamsC
{
public:
	GLint modelParameter;		//modeling matrix
	GLint modelViewNParameter;  //modeliview for normals
	GLint viewParameter;		//viewing matrix
	GLint projParameter;		//projection matrix
	//material
	GLint kaParameter;			//ambient material
	GLint kdParameter;			//diffuse material
	GLint ksParameter;			//specular material
	GLint shParameter;			//shinenness material
} params;


LightC light;
LightC light2;

//the main window size
GLint wWindow=800;
GLint hWindow=800;

float sh=1;

/*********************************
Some OpenGL-related functions
**********************************/

//called when a window is reshaped
void Reshape(int w, int h)
{
  glViewport(0,0,w, h);       
  glEnable(GL_DEPTH_TEST);
//remember the settings for the camera
  wWindow=w;
  hWindow=h;
}

void Arm(glm::mat4 m)
{
//let's use instancing
	if (rott == 1) {
		m = glm::rotate(m, -1.0f*ftime1, glm::vec3(1.0, 0.0, 0.0));
		tmp1 = ftime1;
	}
	else {
		ftime1 = tmp1;
		m = glm::rotate(m, -1.0f*ftime1, glm::vec3(1.0, 0.0, 0.0));
	}
	m=glm::scale(m,glm::vec3(5.0f,5.0f,5.0f));
	sphere->SetModel(m);
	//now the normals
	glm::mat3 modelViewN=glm::mat3(m);
	modelViewN= glm::transpose(glm::inverse(modelViewN));
	sphere->SetModelViewN(modelViewN);
	sphere->Render();
	

}



//the main rendering function
void RenderObjects()
{
	const int range = 3;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(0, 0, 0);
	glPointSize(2);
	glLineWidth(1);
	//set the projection and view once for the scene
	glUniformMatrix4fv(params.projParameter, 1, GL_FALSE, glm::value_ptr(proj));
	//view=glm::lookAt(glm::vec3(25*sin(ftime/40.f),5.f,15*cos(ftime/40.f)),//eye
	//			     glm::vec3(0,0,0),  //destination
	//			     glm::vec3(0,1,0)); //up
	view = glm::lookAt(glm::vec3(0.f, 0.f, 200.f),//eye
		glm::vec3(0, 0, 0),  //destination
		glm::vec3(0, 1, 0)); //up

	glUniformMatrix4fv(params.viewParameter, 1, GL_FALSE, glm::value_ptr(view));
//set the light
	static glm::vec4 pos;
	pos.x = 100; pos.y = 0; pos.z = 0; pos.w = 1;
	light.SetPos(pos);
	light.SetShaders();
	//light.SetLd(glm::vec3(1, 0, 0));
	if (rot == 1) {
		pos.x = 200 * sin(ftime2 / 3); pos.y = 0; pos.z = 200 * cos(ftime2 / 3); pos.w = 1;
		tmp2 = ftime2;
	}
	else {
		ftime2 = tmp2;
		pos.x = 200 * sin(ftime2 / 3); pos.y = 0; pos.z = 200 * cos(ftime2 / 3); pos.w = 1;
		cout << ftime2 << endl;
	}
	light2.SetPos(pos);
	light2.SetShaders();
	light2.SetLd(glm::vec3(0, 0, 1));
	//	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	//	glEnableVertexAttribArray(0);
	//material properties
	//model matrix
	glm::mat4 m=glm::translate(glm::mat4(1.0),glm::vec3(0,0,0));
	Arm(m);
	glutSolidTeapot(100);
}
	
void Idle(void)
{
  glClearColor(0.1,0.1,0.1,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  ftime1 += 0.05;
  ftime2 += 0.05;
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
	  case 'r': 
	  case 'R': {sphere->SetKd(glm::vec3(1,0,0));break;}
	  case 'g': 
	  case 'G': {sphere->SetKd(glm::vec3(0,1,0));break;}
	  case 'b': {rot*=(-1); cout << "rrrrrrrrrrrrrrrrrrrrrrr=" << rot << endl; break; }
	  case 'B': {sphere->SetKd(glm::vec3(0,0,1));break;}
	  case 'w': 
	  case 'W': {sphere->SetKd(glm::vec3(0.7,0.7,0.7));break;}
	  case '+': {sphere->SetSh(sh+=1);break;}
	  case '-': {sphere->SetSh(sh-=1);if (sh<1) sh=1;break;}
	  case ' ': {rott *= (-1); cout << "rrrrrrrrrrrttttttttttttt=" << rott << endl; break; }
	}
	cout << "shineness="<<sh<<endl;
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
	shaderList.push_back(CreateShader(GL_VERTEX_SHADER,   LoadShader("shaders/phong.vert")));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, LoadShader("shaders/phong.frag")));

//create the shader program and attach the shaders to it
	*program = CreateProgram(shaderList);

//delete shaders (they are on the GPU now)
	std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);

	params.modelParameter=glGetUniformLocation(*program,"model");
	params.modelViewNParameter=glGetUniformLocation(*program,"modelViewN");
	params.viewParameter =glGetUniformLocation(*program,"view");
	params.projParameter =glGetUniformLocation(*program,"proj");
	//now the material properties
	params.kaParameter=glGetUniformLocation(*program,"mat.ka");
	params.kdParameter=glGetUniformLocation(*program,"mat.kd");
	params.ksParameter=glGetUniformLocation(*program,"mat.ks");
	params.shParameter=glGetUniformLocation(*program,"mat.sh");
	//now the light properties
	light.SetLaToShader(glGetUniformLocation(*program,"light.la"));
	light.SetLdToShader(glGetUniformLocation(*program,"light.ld"));
	light.SetLsToShader(glGetUniformLocation(*program,"light.ls"));
	light.SetLposToShader(glGetUniformLocation(*program,"light.lPos"));
	//now the light properties
	light2.SetLaToShader(glGetUniformLocation(*program, "light2.la"));
	light2.SetLdToShader(glGetUniformLocation(*program, "light2.ld"));
	light2.SetLsToShader(glGetUniformLocation(*program, "light2.ls"));
	light2.SetLposToShader(glGetUniformLocation(*program, "light2.lPos"));
}

void InitShapes(ShaderParamsC *params)
{
//create one unit sphere in the origin
	sphere=new SphereC(50,50,10);
	sphere->SetKa(glm::vec3(0.1,0.1,10));
	sphere->SetKs(glm::vec3(0,1,1));
	sphere->SetKd(glm::vec3(0.7,0.7,0.7));
	sphere->SetSh(200);
	sphere->SetModel(glm::mat4(1.0));
	sphere->SetModelMatrixParamToShader(params->modelParameter);
	sphere->SetModelViewNMatrixParamToShader(params->modelViewNParameter);
	sphere->SetKaToShader(params->kaParameter);
	sphere->SetKdToShader(params->kdParameter);
	sphere->SetKsToShader(params->ksParameter);
	sphere->SetShToShader(params->shParameter);
}
void menu(int val)
{
	switch (val)
	{
	case 1:break;
	case 2:break;
	case 3:break;
	}
	glutPostRedisplay();
}

void makemenu()
{
	
	glutCreateMenu(menu);
	glutAddSubMenu("sphere", 1);
	glutAddSubMenu("teapot", 2);
	glutAddSubMenu("toroid", 3);
	

	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{ 
  glutInitDisplayString("stencil>=2 rgb double depth samples");
  glutInit(&argc, argv);
  glutInitWindowSize(wWindow,hWindow);
  glutInitWindowPosition(500,100);
  glutCreateWindow("Model View Projection GLSL");
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
  InitializeProgram(&shaderProgram);
  makemenu();
  InitShapes(&params);
  glutMainLoop();
  return 0;        
}
	