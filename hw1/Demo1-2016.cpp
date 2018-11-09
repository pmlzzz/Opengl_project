#include <windows.h>

#include <GL/glew.h>

#include <GL/freeglut.h>

#include <GL/gl.h>
#include <GL/glext.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "InitShader.h"
#include "LoadMesh.h"

static const std::string vertex_shader("demo1_vs.glsl");
static const std::string fragment_shader("demo1_fs.glsl");
GLuint shader_program = -1;
GLuint k = 1;
static const std::string mesh_name = "cow.ply";
MeshData mesh_data;
GLint colorParameter;
// glut display callback function.
// This function gets called every time the scene gets redisplayed 
void display()
{
	GLuint timer_query;
	GLuint nanoseconds;
	glGenQueries(1, &timer_query);

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glm::mat4 M = glm::scale(glm::vec3(mesh_data.mScaleFactor));
   
   glm::mat4 V = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
   glm::mat4 P = glm::perspective(40.0f, 1.0f, 0.1f, 100.0f);

   glUseProgram(shader_program);
   int k_loc = glGetUniformLocation(shader_program, "choice");
   if (k_loc != -1)
   {
	   glUniform1i(k_loc, k);
   }
   int PVM_loc = glGetUniformLocation(shader_program, "PVM");
   if(PVM_loc != -1)
   {
      glm::mat4 PVM = P*V*M;
      glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
   }
   int const NUM_INSTANCES = 9;
   glm::vec4 color_data[NUM_INSTANCES] = {
	   glm::vec4(0.0,1.0,0.0,1.0),
	   glm::vec4(0.0,0.0,1.0,1.0),
	   glm::vec4(1.0,0.0,0.0,1.0),
	   glm::vec4(1.0,1.0,0.0,1.0),
	   glm::vec4(0.0,1.0,1.0,1.0),
	   glm::vec4(1.0,0.0,1.0,1.0),
	   glm::vec4(0.5,0.0,0.0,1.0),
	   glm::vec4(0.0,0.5,0.0,1.0),
	   glm::vec4(0.0,0.0,0.5,1.0)
   };
   glm::vec4 move1_data[NUM_INSTANCES] = {
	   glm::vec4(1.0,0.0,0.0,0.0),
	   glm::vec4(1.0,0.0,0.0,0.0),
	   glm::vec4(1.0,0.0,0.0,0.0),
	   glm::vec4(1.0,0.0,0.0,0.0),
	   glm::vec4(1.0,0.0,0.0,0.0),
	   glm::vec4(1.0,0.0,0.0,0.0),
	   glm::vec4(1.0,0.0,0.0,0.0),
	   glm::vec4(1.0,0.0,0.0,0.0),
	   glm::vec4(1.0,0.0,0.0,0.0)
   };
   glm::vec4 move2_data[NUM_INSTANCES] = {
	   glm::vec4(0.0,1.0,0.0,0.0),
	   glm::vec4(0.0,1.0,0.0,0.0),
	   glm::vec4(0.0,1.0,0.0,0.0),
	   glm::vec4(0.0,1.0,0.0,0.0),
	   glm::vec4(0.0,1.0,0.0,0.0),
	   glm::vec4(0.0,1.0,0.0,0.0),
	   glm::vec4(0.0,1.0,0.0,0.0),
	   glm::vec4(0.0,1.0,0.0,0.0),
	   glm::vec4(0.0,1.0,0.0,0.0)
   };
   glm::vec4 move3_data[NUM_INSTANCES] = {
	   glm::vec4(0.0,0.0,1.0,0.0),
	   glm::vec4(0.0,0.0,1.0,0.0),
	   glm::vec4(0.0,0.0,1.0,0.0),
	   glm::vec4(0.0,0.0,1.0,0.0),
	   glm::vec4(0.0,0.0,1.0,0.0),
	   glm::vec4(0.0,0.0,1.0,0.0),
	   glm::vec4(0.0,0.0,1.0,0.0),
	   glm::vec4(0.0,0.0,1.0,0.0),
	   glm::vec4(0.0,0.0,1.0,0.0)
   };
   glm::vec4 move4_data[NUM_INSTANCES] = {
	   glm::vec4(-7.0,7.0,0.0,1.0),
	   glm::vec4(0.0,7.0,0.0,1.0),
	   glm::vec4(7.0,7.0,0.0,1.0),
	   glm::vec4(-7.0,0.0,0.0,1.0),
	   glm::vec4(0.0,0.0,0.0,1.0),
	   glm::vec4(7.0,0.0,0.0,1.0),
	   glm::vec4(-7.0,-7.0,0.0,1.0),
	   glm::vec4(0.0,-7.0,0.0,1.0),
	   glm::vec4(7.0,-7.0,0.0,1.0)
   };
   if (k == 1)
   {
	  
	   GLuint color_buffer;

	   glGenBuffers(1, &color_buffer);
	   glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	   glBufferData(GL_ARRAY_BUFFER,
		   NUM_INSTANCES * sizeof(glm::vec4), color_data,
		   GL_STATIC_DRAW);
	   int color_loc = glGetAttribLocation(shader_program, "allcolor");
	   glVertexAttribPointer(color_loc, 4, GL_FLOAT,
		   GL_FALSE, 0, NULL);
	   glEnableVertexAttribArray(color_loc);
	   glVertexAttribDivisor(color_loc, 1);



	  
	   GLuint move1_buffer;

	   glGenBuffers(1, &move1_buffer);
	   glBindBuffer(GL_ARRAY_BUFFER, move1_buffer);
	   glBufferData(GL_ARRAY_BUFFER,
		   NUM_INSTANCES * sizeof(glm::vec4), move1_data,
		   GL_STATIC_DRAW);
	   int move1_loc = glGetAttribLocation(shader_program, "move1");
	   glVertexAttribPointer(move1_loc, 4, GL_FLOAT,
		   GL_FALSE, 0, NULL);
	   glEnableVertexAttribArray(move1_loc);
	   glVertexAttribDivisor(move1_loc, 1);

	  
	   GLuint move2_buffer;

	   glGenBuffers(1, &move2_buffer);
	   glBindBuffer(GL_ARRAY_BUFFER, move2_buffer);
	   glBufferData(GL_ARRAY_BUFFER,
		   NUM_INSTANCES * sizeof(glm::vec4), move2_data,
		   GL_STATIC_DRAW);
	   int move2_loc = glGetAttribLocation(shader_program, "move2");
	   glVertexAttribPointer(move2_loc, 4, GL_FLOAT,
		   GL_FALSE, 0, NULL);
	   glEnableVertexAttribArray(move2_loc);
	   glVertexAttribDivisor(move2_loc, 1);

	   
	   GLuint move3_buffer;

	   glGenBuffers(1, &move3_buffer);
	   glBindBuffer(GL_ARRAY_BUFFER, move3_buffer);
	   glBufferData(GL_ARRAY_BUFFER,
		   NUM_INSTANCES * sizeof(glm::vec4), move3_data,
		   GL_STATIC_DRAW);
	   int move3_loc = glGetAttribLocation(shader_program, "move3");
	   glVertexAttribPointer(move3_loc, 4, GL_FLOAT,
		   GL_FALSE, 0, NULL);
	   glEnableVertexAttribArray(move3_loc);
	   glVertexAttribDivisor(move3_loc, 1);

	   
	   GLuint move4_buffer;

	   glGenBuffers(1, &move4_buffer);
	   glBindBuffer(GL_ARRAY_BUFFER, move4_buffer);
	   glBufferData(GL_ARRAY_BUFFER,
		   NUM_INSTANCES * sizeof(glm::vec4), move4_data,
		   GL_STATIC_DRAW);
	   int move4_loc = glGetAttribLocation(shader_program, "move4");
	   glVertexAttribPointer(move4_loc, 4, GL_FLOAT,
		   GL_FALSE, 0, NULL);
	   glEnableVertexAttribArray(move4_loc);
	   glVertexAttribDivisor(move4_loc, 1);


	   glBindVertexArray(mesh_data.mVao);

	   glBeginQuery(GL_TIME_ELAPSED, timer_query);
	   glDrawElementsInstanced(GL_TRIANGLES, mesh_data.mNumIndices, GL_UNSIGNED_INT, 0, 1000);
	   glEndQuery(GL_TIME_ELAPSED);
	   glGetQueryObjectuiv(timer_query, GL_QUERY_RESULT, &nanoseconds);
	   printf("with instance time:   %d\n", nanoseconds);
   }
   else
   {
	   //printf("time:time:time:time:time:time:time:time:time:time:time:time:time:time:time:time:time:time:  \n");
	   GLuint timer2_query;
	   GLuint nanoseconds2;
	   glGenQueries(1, &timer2_query);
	   float color1[4] = { 0.0,1.0,0.0,1.0 };
	   glBindVertexArray(mesh_data.mVao);
	   glUniform4fv(colorParameter, 1, color1);
	   glBeginQuery(GL_TIME_ELAPSED, timer2_query);
	   M *= glm::scale(glm::vec3(0.5, 0.5, 0.5));
	   for(int i=0;i<1000;i++)
	   {   
		  
		   M = glm::translate(M, glm::vec3(cos(0.01*i), sin(0.01*i), 0));
		   glm::mat4 PVM = P*V*M;
		   glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
		   glDrawElements(GL_TRIANGLES, mesh_data.mNumIndices, GL_UNSIGNED_INT, 0);
	   }
	   
	   glEndQuery(GL_TIME_ELAPSED);
	   glGetQueryObjectuiv(timer2_query, GL_QUERY_RESULT, &nanoseconds2);
	   printf("without instance time:   %d\n", nanoseconds2);
	   glUseProgram(shader_program);
	   colorParameter = glGetUniformLocation(shader_program, "allcolor2");
   }

   glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();

   const int time_ms = glutGet(GLUT_ELAPSED_TIME);
   float time_sec = 0.001f*time_ms;
   glUseProgram(shader_program);
   int Time_loc = glGetUniformLocation(shader_program, "time");
   if (Time_loc != -1)
   {
	   glUniform1f(Time_loc, time_sec);
   }
   
}

void reload_shader()
{
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


// glut keyboard callback function.
// This function gets called when an ASCII key is pressed
void keyboard(unsigned char key, int x, int y)
{
   std::cout << "key : " << key << ", x: " << x << ", y: " << y << std::endl;

   switch(key)
   {
      case 'r':
      case 'R':
         reload_shader();   
	  case 'i':
		  k *=- 1; printf("%d", k);
      break;
   }
}

void printGlInfo()
{
   std::cout << "Vendor: "       << glGetString(GL_VENDOR)                    << std::endl;
   std::cout << "Renderer: "     << glGetString(GL_RENDERER)                  << std::endl;
   std::cout << "Version: "      << glGetString(GL_VERSION)                   << std::endl;
   std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION)  << std::endl;
}

void initOpenGl()
{
   glewInit();

   glEnable(GL_DEPTH_TEST);

   // Try uncommenting one of these lines at a time, and rerunning the program
   //glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   reload_shader();
   mesh_data = LoadMesh(mesh_name);
}

int main (int argc, char **argv)
{
   //Configure initial window state
   glutInit(&argc, argv); 
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowPosition (5, 5);
   glutInitWindowSize (512, 512);
   int win = glutCreateWindow ("Lab 1");

   printGlInfo();

   //Register callback functions with glut. 
   glutDisplayFunc(display); 
   glutKeyboardFunc(keyboard);
   glutIdleFunc(idle);

   initOpenGl();

   //Enter the glut event loop.
   glutMainLoop();
   glutDestroyWindow(win);
   return 0;		
}

