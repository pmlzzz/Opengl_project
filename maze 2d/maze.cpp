#include <windows.h>

#include <GL/glew.h>


#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <iostream>

#include "InitShader.h"
#include "LoadMesh.h"
using namespace std;

static const std::string shader_names[2] = {"per_vertex", "per_fragment"};
int current_shader_name_index = 0;

GLuint shader_program = -1;
bool mouseLeftDown;
bool mouseRightDown;

int uplock = 0;
int downlock = 0;
int leftlock = 0;
int rightlock = 0;

int uplock2 = 0;
int downlock2 = 0;
int leftlock2 = 0;
int rightlock2 = 0;

float mouseX, mouseY;
static const std::string mesh_name = "cube.obj";
static const std::string mesh_name2 = "bigwhite.obj";
static const std::string mesh_name3 = "over.obj";
//static const std::string mesh_name = "sphere.obj";
MeshData mesh_data;
MeshData mesh_data2;
MeshData mesh_data3;
float color1[4] = { 1.0,1.0,0.0,0.0 };
float color2[4] = { 1.0,1.0,1.0,0.0 };
float color3[4] = { 1.0,0.0,0.0,0.0 };
GLint colorParameter;
glm::vec3 pos(0.0f, 0.0f, 0.0f);
float angle = 0.0f;
float hangle = 20.0f;
float tmp = 1.0f;
GLfloat ftime = 0.f;
float xx = 0.0f;
float zz = 0.0f;
float xx2 = 0.0f;
float zz2 = 0.0f;
float xx3 = 0.2f;
float zz3 = 0.25f;
float scale = 1.0f;
float textscale =0.0f;
glm::vec3 cam_pos(0.0f, 0.0f, 3.0f);
GLfloat BWrotate = 0.0f;
GLfloat BWrotate2 = 90.0f;
int xfloor, xloc, xceil, zfloor, zloc, zceil, time;
int xfloor2, xloc2, xceil2, zfloor2, zloc2, zceil2, time2;
int choice1 = 0;
int choice2 = 0;
char a[46][46];
// glut display callback function.
// This function gets called every time the scene gets redisplayed 
void test(float xx, float zz)
{
	//cout << "x:" << floor(0.1*xx - 0.3) << "," << floor(0.1*xx) << "," << ceil(0.1*xx + 0.3) << "," << "y:" << floor(0.1*zz + 1.5 - 0.3) << "," << floor(0.1*zz + 1.5) << "," << ceil(0.1*zz + 1.5 + 0.3);
	time = floor(ftime);
	xfloor = floor(0.1*xx - 0.3);
	xloc = floor(0.1*xx);
	xceil = floor(0.1*xx + 0.3);
	zfloor = floor(0.1*zz + 1.5 - 0.3);
	zloc = floor(0.1*zz + 1.5);
	zceil = floor(0.1*zz + 1.5 + 0.3);
	//cout << "up:" << xloc << "," << zfloor << "down:" << xloc << "," << zceil << "left:" << xfloor << "," << zloc << "right:" << xceil << "," << zloc << endl;

	if (a[zfloor][xloc] == ' ') { uplock = 0; }
	else { uplock = 1; }
	if (a[zceil][xloc] == ' ') { downlock = 0; }
	else { downlock = 1; }
	if (a[zloc][xfloor] == ' ') { leftlock = 0; }
	else { leftlock = 1; }
	if (a[zloc][xceil] == ' ') { rightlock = 0; }
	else {
		rightlock = 1; //printf("asdasd");
	}
}
void testrobot(float *xx2, float *zz2,int *choice)
{
	cout << xx2 << endl;
	//cout << "x:" << floor(0.1*xx - 0.3) << "," << floor(0.1*xx) << "," << ceil(0.1*xx + 0.3) << "," << "y:" << floor(0.1*zz + 1.5 - 0.3) << "," << floor(0.1*zz + 1.5) << "," << ceil(0.1*zz + 1.5 + 0.3);
	time2 = floor(ftime*100);
	xfloor2 = floor(0.1* *xx2 - 0.3);
	xloc2 = floor(0.1* *xx2);
	xceil2 = floor(0.1* *xx2 + 0.3);
	zfloor2 = floor(0.1* *zz2 + 2.5 - 0.3);
	zloc2 = floor(0.1* *zz2 + 2.5);
	zceil2 = floor(0.1* *zz2 + 2.5 + 0.3);
	//cout << "up:" << xloc << "," << zfloor << "down:" << xloc << "," << zceil << "left:" << xfloor << "," << zloc << "right:" << xceil << "," << zloc << endl;
	if (time2 % 500 == 0) {
		*choice = rand() % 4 + 1; //cout << time2*100 % 500 << "              " << choice << endl;
	}
	//cout << time2  % 500 << "              " << choice << endl;
	if (a[zfloor2][xloc2] == ' '&& *choice == 1) { *zz2 -=  0.1; cout << "zz2" << endl;
	}
	else if (a[zceil2][xloc2] == ' '&& *choice == 2) { *zz2 +=  0.1; cout << "zz2" << endl;
	}
	else if (a[zloc2][xfloor2] == ' '&& *choice == 3) { *xx2 -=  0.1; cout << "xx2" << endl;
	}
	else if (a[zloc2][xceil2] == ' '&& *choice == 4) { *xx2 +=0.1; cout << "xx2" << endl;
	}
	else { *choice = rand() % 4 + 1; }
}
void readTxt(string file)
{
	ifstream infile;
	int sum=0;
	infile.open("zz.txt");
	infile >> noskipws;
	for (int i = 0; i < 47; i++)
	{
		for (int j = 0; j < 47; j++)
		{
			infile >> a[i][j];
			//cout << a[i][j];
			if (a[i][j] == ' ') { sum++; }
			
		}
		
	}
	//cout << endl;
	infile.close();
	//cout << sum << endl;
}
void object(int M_loc, glm::vec3 move)
{
	glm::mat4 M2 = glm::mat4(1.0);
	M2 = glm::translate(M2, move);
	M2 = M2* glm::scale(glm::vec3(0.1, 0.2, 0.1))*glm::scale(glm::vec3(mesh_data.mScaleFactor));

	glBindVertexArray(mesh_data.mVao);
	glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M2));
	glUniform4fv(colorParameter, 1, color2);
	glDrawElements(GL_TRIANGLES, mesh_data2.mNumIndices, GL_UNSIGNED_INT, 0);
	colorParameter = glGetUniformLocation(shader_program, "color");
}

void testgg(float x, float z)
{
	if ((x - 2) < xx && xx < (x + 2) && (z - 2 + 10) < zz && zz < (z + 2 + 10))
	{
		textscale = 0.5*scale;
	}
	else {  }
}
void display()
{

	if(tmp==0)
	{
		angle = -ftime / 5.f;
	}
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glm::mat4 V = glm::mat4(1.0);
   glm::mat4 M = glm::mat4(1.0);
   M = glm::translate(M, glm::vec3(0, 0, 0));
   M=M*glm::scale(glm::vec3(mesh_data.mScaleFactor))*glm::scale(glm::vec3(0.1, 0.1, 0.1));
   V = glm::lookAt(glm::vec3(40 * sin(angle)*scale, 90 * scale, 40 * cos(angle)*scale),//eye
	   glm::vec3(xx*0.01 - 0.05, -0.05 - 0.1, zz*0.01 + 0.15),  //destination
	   glm::vec3(0, 40, 0)); //up
   glm::mat4 P = glm::perspective(1.0f, 1.0f,10.0f, 600.0f);
   ftime += 0.01;
   glUseProgram(shader_program);
   int P_loc = glGetUniformLocation(shader_program, "P");
   if(P_loc != -1)
   { 
      glUniformMatrix4fv(P_loc, 1, false, glm::value_ptr(P));
   }

   int V_loc = glGetUniformLocation(shader_program, "V");
   if (V_loc != -1)
   {
	   glUniformMatrix4fv(V_loc, 1, false, glm::value_ptr(V));
   }

   int M_loc = glGetUniformLocation(shader_program, "M");
   if (M_loc != -1)
   {

	   //glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M));
   }
   
   //cout << angle <<"," << acos(cos(angle ))<< "," << endl;
	glm::mat4 M2 = glm::mat4(1.0);
	M2 = glm::translate(M2, glm::vec3(xx*0.01-0.05, -0.05-0.1, zz*0.01+0.15));
	M2 = glm::rotate(M2, BWrotate +(angle/1.57f*90.0f), glm::vec3(0.0, 1.0, 0.0));
	M2 = glm::translate(M2, glm::vec3(0, 0, -0.01));
	M2 = M2* glm::scale(glm::vec3(0.05, 0.05, 0.05))*glm::scale(glm::vec3(mesh_data2.mScaleFactor));

	glm::mat4 M3 = glm::mat4(1.0);
	M3 = glm::translate(M3, glm::vec3(xx2*0.01 - 0.05, -0.05-0.1, zz2*0.01 + 0.25));
	M3 = glm::rotate(M3, 0.0f, glm::vec3(0.0, 1.0, 0.0));
	M3 = glm::translate(M3, glm::vec3(0, 0, -0.01));
	M3 = M3* glm::scale(glm::vec3(0.05, 0.05, 0.05))*glm::scale(glm::vec3(mesh_data2.mScaleFactor));
	test(xx, zz);
	testrobot(&xx2, &zz2, &choice1);
	testgg(xx2, zz2);

	glm::mat4 M4 = glm::mat4(1.0);
	M4 = glm::translate(M4, glm::vec3(xx3*0.01 - 0.05, -0.05 - 0.1, zz3*0.01 + 0.25));
	M4 = glm::rotate(M4, 0.0f, glm::vec3(0.0, 1.0, 0.0));
	M4 = glm::translate(M4, glm::vec3(0, 0, -0.01));
	M4 = M4* glm::scale(glm::vec3(0.05, 0.05, 0.05))*glm::scale(glm::vec3(mesh_data2.mScaleFactor));
	testrobot(&xx3, &zz3, &choice2);
	testgg(xx3, zz3);

	glm::mat4 M4 = glm::mat4(1.0);
	M4 = glm::translate(M4, glm::vec3(xx3*0.01 - 0.05 + 0.2, -0.05 - 0.1, zz3*0.01 + 0.25 + 0.25));
	M4 = glm::rotate(M4, 0.0f, glm::vec3(0.0, 1.0, 0.0));
	M4 = glm::translate(M4, glm::vec3(0, 0, -0.01));
	M4 = M4* glm::scale(glm::vec3(0.05, 0.05, 0.05))*glm::scale(glm::vec3(mesh_data2.mScaleFactor));
	testrobot(&xx3, &zz3, &choice2);
	testgg(xx3, zz3);

	glm::mat4 text = glm::mat4(1.0);
	text = glm::translate(text, glm::vec3(xx*0.01 -0.2,0.5, zz*0.01 -0.1));
	text = glm::rotate(text, -90.0f, glm::vec3(1.0, 0.0, 0.0));
	text = glm::translate(text, glm::vec3(0, 0, -0.01));
	text = text* glm::scale(glm::vec3(textscale, textscale, textscale))*glm::scale(glm::vec3(mesh_data3.mScaleFactor));

	glm::mat4 text2 = glm::mat4(1.0);
	text2 = glm::translate(text2, glm::vec3(xx*0.01 +0.3, 0.5, zz*0.01 -0.1));
	text2 = glm::rotate(text2, -90.0f, glm::vec3(1.0, 0.0, 0.0));
	text2 = glm::translate(text2, glm::vec3(0, 0, -0.01));
	text2 = text2* glm::scale(glm::vec3(textscale, textscale, textscale))*glm::scale(glm::vec3(mesh_data3.mScaleFactor));
	
	/*

	glm::mat4 M4 = glm::mat4(1.0);
	M4 = glm::translate(M4, glm::vec3(40 * 0.01  , -0.05, 15 * 0.01+ 25*sin(ftime)*0.01));
	if (sin(ftime) == 1) BWrotate2 = 90;
	M4 = glm::rotate(M4, BWrotate2, glm::vec3(0.0, 1.0, 0.0));
	M4 = M4* glm::scale(glm::vec3(0.1, 0.1, 0.1))*glm::scale(glm::vec3(mesh_data2.mScaleFactor));

	glm::mat4 M5 = glm::mat4(1.0);
	M5 = glm::translate(M5, glm::vec3(0* 0.01, -0.05, 70 * 0.01 - 25 * sin(ftime)*0.01));
	if (sin(ftime) == 1) BWrotate2 = 90;
	M5 = glm::rotate(M5, BWrotate2, glm::vec3(0.0, 1.0, 0.0));
	M5 = M5* glm::scale(glm::vec3(0.1, 0.1, 0.1))*glm::scale(glm::vec3(mesh_data2.mScaleFactor));
	

	glBindVertexArray(mesh_data.mVao); 
	glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M));
	glUniform4fv(colorParameter, 1, color1);
	glDrawElements(GL_TRIANGLES, mesh_data.mNumIndices, GL_UNSIGNED_INT, 0);
	colorParameter = glGetUniformLocation(shader_program, "color");
	 */
	
	for (int i = 0; i < 46; i++)
	{
		for (int j = 0; j < 46; j++)
		{
			if (a[i][j] != ' ') {object(M_loc, glm::vec3(0.1*j, 0, 0.1*i)); 
			}

		}

	}
	glBindVertexArray(mesh_data2.mVao);
	glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M2));
	glUniform4fv(colorParameter, 1, color2);
	glDrawElements(GL_TRIANGLES, mesh_data2.mNumIndices, GL_UNSIGNED_INT, 0);
	colorParameter = glGetUniformLocation(shader_program, "color");

	glBindVertexArray(mesh_data2.mVao);
	glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M3));
	glUniform4fv(colorParameter, 1, color3);
	glDrawElements(GL_TRIANGLES, mesh_data2.mNumIndices, GL_UNSIGNED_INT, 0);
	colorParameter = glGetUniformLocation(shader_program, "color");

	glBindVertexArray(mesh_data3.mVao);
	glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(text));
	glUniform4fv(colorParameter, 1, color3);
	glDrawElements(GL_TRIANGLES, mesh_data3.mNumIndices, GL_UNSIGNED_INT, 0);
	colorParameter = glGetUniformLocation(shader_program, "color");
	glBindVertexArray(mesh_data3.mVao);
	glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(text2));
	glUniform4fv(colorParameter, 1, color3);
	glDrawElements(GL_TRIANGLES, mesh_data3.mNumIndices, GL_UNSIGNED_INT, 0);
	colorParameter = glGetUniformLocation(shader_program, "color");
	glBindVertexArray(mesh_data2.mVao);
	glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M4));
	glUniform4fv(colorParameter, 1, color3);
	glDrawElements(GL_TRIANGLES, mesh_data2.mNumIndices, GL_UNSIGNED_INT, 0);
	colorParameter = glGetUniformLocation(shader_program, "color");
	/*
	
	glBindVertexArray(mesh_data2.mVao);
	glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M3));
	glUniform4fv(colorParameter, 1, color3);
	glDrawElements(GL_TRIANGLES, mesh_data2.mNumIndices, GL_UNSIGNED_oINT, 0);
	colorParameter = glGetUniformLocation(shader_program, "color");

	glBindVertexArray(mesh_data2.mVao);
	glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M4));
	glUniform4fv(colorParameter, 1, color3);
	glDrawElements(GL_TRIANGLES, mesh_data2.mNumIndices, GL_UNSIGNED_INT, 0);
	colorParameter = glGetUniformLocation(shader_program, "color");

	glBindVertexArray(mesh_data2.mVao);
	glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M5));
	glUniform4fv(colorParameter, 1, color3);
	glDrawElements(GL_TRIANGLES, mesh_data2.mNumIndices, GL_UNSIGNED_INT, 0);
	colorParameter = glGetUniformLocation(shader_program, "color");
	*/
	
	glutSwapBuffers();
	
}


float anim_timer = 0.0f;
glm::vec3 target_pos;
float target_angle;
glm::vec3 vel(0.0f);
float angular_vel = 0.0f;

void idle()
{
   static int last_time = 0;
   static float seconds = 0.0f;

   int time = glutGet(GLUT_ELAPSED_TIME);
   int elapsed = time - last_time;
   last_time = time;
   float delta_seconds = 0.001f*elapsed;
   seconds += delta_seconds;

   glUseProgram(shader_program);
   int Time_loc = glGetUniformLocation(shader_program, "time");
   if (Time_loc != -1)
   {
      glUniform1f(Time_loc, seconds);
   }

   glutPostRedisplay();
   
}

void reload_shader()
{
   const std::string vertex_shader = shader_names[current_shader_name_index] + "_vs.vert";
   const std::string fragment_shader = shader_names[current_shader_name_index] + "_fs.frag";
   GLuint new_shader = InitShader(vertex_shader.c_str(), fragment_shader.c_str());

   if(new_shader == -1) // loading failed
   {
      glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
   }
   else
   {
      glClearColor(0.35f, 0.35f, 0.35f, 0.0f);

      if(shader_program != -1)
      {
         glDeleteProgram(shader_program);
      }
      shader_program = new_shader;

      if(mesh_data.mVao != -1)
      {
         BufferIndexedVerts(mesh_data);
      }
   }
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'f':tmp=1; break;
	case 'w':  if (!((cos(angle)>0 && uplock == 1) || (cos(angle)<0 && downlock == 1) || (sin(angle)>0 && leftlock == 1) || (sin(angle)<0 && rightlock == 1))) { zz -= cos(angle); xx -= sin(angle); } BWrotate = -180; break;
	case 's':  if (!((cos(angle)<0 && uplock == 1) || (cos(angle)>0 && downlock == 1) || (sin(angle)<0 && leftlock == 1) || (sin(angle)>0 && rightlock == 1))) { zz += cos(angle); xx += sin(angle); } BWrotate = 0; break;
	case 'a':  if (!((sin(angle)<0 && uplock == 1) || (sin(angle)>0 && downlock == 1) || (cos(angle)>0 && leftlock == 1) || (cos(angle)<0 && rightlock == 1))) { zz += sin(angle); xx -= cos(angle); } BWrotate = -90; break;
	case 'd':  if (!((sin(angle)>0 && uplock == 1) || (sin(angle)<0 && downlock == 1) || (cos(angle)<0 && leftlock == 1) || (cos(angle)>0 && rightlock == 1))) {zz -= sin(angle); xx += cos(angle);  } BWrotate = 90; break;
	}
	glutPostRedisplay();
}

void special(int key, int x, int y)
{


   switch (key)
   {
   case GLUT_KEY_F1:
      current_shader_name_index = 0;
      reload_shader();
      break;

   case GLUT_KEY_F2:
      current_shader_name_index = 1;
      reload_shader();
      break;

   }
}

void mouse(int button, int state, int x, int y)
{
	mouseX = x;
	mouseY = y;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseLeftDown = true;
		}
		else if (state == GLUT_UP)
			mouseLeftDown = false;
	}

	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseRightDown = true;
		}
		else if (state == GLUT_UP)
			mouseRightDown = false;
	}
	else if (button == GLUT_WHEEL_UP)
	{
		scale += 0.02;
	}
	else if (button == GLUT_WHEEL_DOWN)
	{
		scale -= 0.02;
	}
}
void mouseMotion(int x, int y)
{
	if (mouseRightDown)
	{
		angle -= (x - mouseX)*0.01;
		//hangle += (y - mouseY)*0.01;
		mouseX = x;
		mouseY = y;
	}
	glutPostRedisplay();
}

void initOpenGl()
{
   glewInit();

   glEnable(GL_DEPTH_TEST);
   glClear(GL_DEPTH_BUFFER_BIT);
   //const GLubyte *ch  = glGetString(GL_VENDOR);


   reload_shader();
   mesh_data = LoadMesh(mesh_name);
   mesh_data2 = LoadMesh(mesh_name2);
   mesh_data3 = LoadMesh(mesh_name3);
}

int main (int argc, char **argv)
{
   //Configure initial window state
   glutInit(&argc, argv); 
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowPosition (5, 5);
   glutInitWindowSize (512, 512);
   int win = glutCreateWindow ("mazzzzzzzzzzzzzzzzzzzzzzzzzzze");

   readTxt("zz.txt");
   //Register callback functions with glut. 
   glutDisplayFunc(display); 
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(special);
   glutMouseFunc(mouse);
   glutMotionFunc(mouseMotion);
   glutIdleFunc(idle);

   initOpenGl();
   
   //Enter the glut event loop.
   glutMainLoop();
   glutDestroyWindow(win);
   return 0;		
}

