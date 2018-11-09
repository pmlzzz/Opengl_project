#include <windows.h>

#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "InitShader.h"
#include "imgui_impl_glut.h"

using namespace std;
static int win = 0;

//opengl shader object
GLuint program = -1;

//shader uniform locations
int M_loc = -1;
int Q_loc = -1;
int PV_loc = -1;
int Time_loc = -1;
int Pass_loc = -1;
bool gamma = 0;
//shader attrib locations
int pos_loc = -1;
int tex_coord_loc = -1;

bool pauseRotation = false;
float rev_per_min = 0.0f;
float seconds = 0.0f;
float Qscale = 1.0f;
glm::vec3 Qtrans(0.0f, 0.0f, 0.0f);

GLuint VBO1;
GLuint IndexBuffer;
GLuint TexBackFaces = 0;
GLuint TexFrontFaces = 0;
GLuint TexRender = 0;
GLuint FBOID = 0;       // framebuffer object,

int WindowWidth = 800;
int WindowHeight = 800;

const int num_samples = 1;

void LoadShader();

//check for errors and print error codes.
void glError()
{
   GLenum errCode;
   const GLubyte *errString;
   if((errCode = glGetError()) != GL_NO_ERROR)
   {
      errString = gluErrorString(errCode);
      if(errString > 0)
      {
         cout << "OpenGL Error: " << errString << endl;
      }
      else
      {
         cout << "OpenGL Error: " << errCode << endl;
      }
   }
}

bool CheckFramebufferStatus() 
{
    GLenum status;
    status = (GLenum) glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(status) {
        case GL_FRAMEBUFFER_COMPLETE:
            return true;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			printf("Framebuffer incomplete, incomplete attachment\n");
            return false;
        case GL_FRAMEBUFFER_UNSUPPORTED:
			printf("Unsupported framebuffer format\n");
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			printf("Framebuffer incomplete, missing attachment\n");
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			printf("Framebuffer incomplete, missing draw buffer\n");
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			printf("Framebuffer incomplete, missing read buffer\n");
            return false;
    }
	return false;
}

//Create a cube with texture coordinates
void BufferIndexedVertsUsingMapBuffer()
{
   using namespace glm;
   vec3 pos[8] = {vec3(-1.0f, -1.0f, -1.0f), vec3(+1.0f, -1.0f, -1.0f), vec3(+1.0f, +1.0f, -1.0f), vec3(-1.0f, +1.0f, -1.0f),
      vec3(-1.0f, -1.0f, +1.0f), vec3(+1.0f, -1.0f, +1.0f), vec3(+1.0f, +1.0f, +1.0f), vec3(-1.0f, +1.0f, +1.0f)};

   vec3 tex[8] = {vec3(-1.0f, -1.0f, -1.0f), vec3(+1.0f, -1.0f, -1.0f), vec3(+1.0f, +1.0f, -1.0f), vec3(-1.0f, +1.0f, -1.0f),
      vec3(-1.0f, -1.0f, +1.0f), vec3(+1.0f, -1.0f, +1.0f), vec3(+1.0f, +1.0f, +1.0f), vec3(-1.0f, +1.0f, +1.0f)};

   unsigned short idx[36] = { 0,2,1, 2,0,3, //bottom
      0,5,4, 5,0,1, //front
      1,6,5, 6,1,2, //right 
      2,7,6, 7,2,3, //back
      3,4,7, 4,3,0, //left
      4,5,6, 6,7,4};//top
	//Buffer vertices
	int datasize = sizeof(pos) + sizeof(tex);
   glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, datasize, 0, GL_STATIC_DRAW);
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), pos);
   glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(tex), tex);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Buffer indices
   glGenBuffers(1, &IndexBuffer);
	int nIndices = 12;
	int indexsize = sizeof(idx);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexsize, idx, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   glError();
}

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
void DrawIndexedVerts()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);

	glEnableVertexAttribArray(pos_loc);
   glEnableVertexAttribArray(tex_coord_loc);

	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, false, 0, BUFFER_OFFSET(0));
   glVertexAttribPointer(tex_coord_loc, 3, GL_FLOAT, false, 0, BUFFER_OFFSET(8*3*sizeof(float)));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

	glDisableVertexAttribArray(pos_loc);
   glDisableVertexAttribArray(tex_coord_loc);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void draw_gui()
{
   ImGui_ImplGlut_NewFrame();

   if (ImGui::Button("Reload shader"))
   {
      LoadShader();
   }
   ImGui::Checkbox("Pause rotate", &pauseRotation);
   ImGui::SliderFloat3("Trans", &Qtrans.x, -10.0f, 10.0f);
   ImGui::SliderFloat("Scale", &Qscale, 0.001f, 10.0f);
   ImGui::Checkbox("Gamma", &gamma);
   int gamma_loc = glGetUniformLocation(program, "gamma");
   glUniform1i(gamma_loc, gamma);
   static bool wireframe = false;
   ImGui::Checkbox("Wireframe", &wireframe);
   if (wireframe == true)
   {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   }
   else
   {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   }
   static int option = 0;
   ImGui::RadioButton("radio a", &option, 0); ImGui::SameLine();
   ImGui::RadioButton("radio b", &option, 1); ImGui::SameLine();
   ImGui::RadioButton("radio c", &option, 2);
   int option_loc = glGetUniformLocation(program, "option");
   glUniform1i(option_loc, option);
   ImGui::Render();
 }


void display()
{
   using namespace glm;
	mat4 M = rotate(6.0f*rev_per_min, vec3(0.0f, 0.0f, 1.0f));
   mat4 V = lookAt(vec3(3.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f));
   mat4 P = perspective(70.0f, 1.0f, 0.1f, 100.0f);

   mat4 Q = translate(Qtrans)*scale(vec3(Qscale));
   vec4 campos = inverse(V*M)*vec4(0.0f, 0.0f, 0.0f, 1.0f);

   //draw pass 1: back faces of cube to texture
   glUseProgram(program);
   //set uniforms
   if(M_loc != -1)
   {
	   glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M));
   }

   if(Q_loc != -1)
   {
	   glUniformMatrix4fv(Q_loc, 1, false, glm::value_ptr(Q));
   }

   if(PV_loc != -1)
   {
      glUniformMatrix4fv(PV_loc, 1, false, glm::value_ptr(P*V));
   }

   glBindFramebuffer(GL_FRAMEBUFFER, FBOID);
   glEnable(GL_DEPTH_TEST);

   if(Pass_loc != -1)
   {
      glUniform1i(Pass_loc, 1);
   }

   GLuint tex_target = GL_TEXTURE_2D_MULTISAMPLE;
   if (num_samples <= 1)
   {
      tex_target = GL_TEXTURE_2D;
   }

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(tex_target, 0); //unbind texture so we can write to it (remember, no read-write access)
   //Subsequent drawing should be captured by the framebuffer attached texture
   glDrawBuffer(GL_COLOR_ATTACHMENT0);

   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   //clear the color texture and depth texture
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   glCullFace(GL_FRONT); //draw back faces
   DrawIndexedVerts(); //draw cube
 

   if(Pass_loc != -1)
   {
      glUniform1i(Pass_loc, 2);
   }
   //pass 2: draw front faces to attachment 1
   glCullFace(GL_BACK);
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(tex_target, 0);
   glDrawBuffer(GL_COLOR_ATTACHMENT1);

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //draw cube front faces to fbo
   DrawIndexedVerts();

   //raycasting pass
   if(Pass_loc != -1)
   {
      glUniform1i(Pass_loc, 3);
   }
   //pass 2: draw front faces to attachment 1
   glActiveTexture(GL_TEXTURE2);
   glBindTexture(tex_target, 0);
   glDrawBuffer(GL_COLOR_ATTACHMENT2);

   glClearColor(0.8f, 0.8f, 0.9f, 0.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(tex_target, TexBackFaces);
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(tex_target, TexFrontFaces);

   //draw cube front faces to fbo
   DrawIndexedVerts();

   //blit color attachment 2 to the screen
   glBindFramebuffer(GL_READ_FRAMEBUFFER, FBOID);
   glReadBuffer(GL_COLOR_ATTACHMENT2);
   glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
   glBlitFramebuffer(0, 0, 800, 800, 0, 0, 800, 800, GL_COLOR_BUFFER_BIT, GL_NEAREST);
  
   draw_gui();
	glutSwapBuffers();
   glError();
}

void LoadShader()
{
   if(program != -1)
	{
		glDeleteProgram(program);
	}

   //create and load shaders

   if (num_samples <= 1)
   {
      program = InitShader("vshader.glsl", "fshader.glsl");
   }
   else
   {
      program = InitShader("vshader.glsl", "fshader_ms.glsl"); //multisample version of fragment shader
   }
	
   glUseProgram(program);

   pos_loc = glGetAttribLocation(program, "pos_attrib");
   tex_coord_loc = glGetAttribLocation(program, "tex_coord_attrib");

   Q_loc = glGetUniformLocation(program, "Q");
   M_loc = glGetUniformLocation(program, "M");
   PV_loc = glGetUniformLocation(program, "PV");
   Time_loc = glGetUniformLocation(program, "time");
   Pass_loc = glGetUniformLocation(program, "pass");

   int tex_loc = glGetUniformLocation(program, "backfaces");
   if(tex_loc != -1)
   {
      glUniform1i(tex_loc, 0);
   }

   glError();
}


void InitOpenGL()
{
	glewInit();

   glError();

   if (num_samples > 1)
   {
      glEnable(GL_MULTISAMPLE);
      glEnable(GL_SAMPLE_SHADING);
      glMinSampleShading(1.0f);
   }
   
	glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);

   //create empty textures
   GLuint tex_target = GL_TEXTURE_2D_MULTISAMPLE;
   if (num_samples <= 1)
   {
      tex_target = GL_TEXTURE_2D;
   }

   glGenTextures(1, &TexBackFaces);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(tex_target, TexBackFaces);
   if (tex_target == GL_TEXTURE_2D)
   {
      glTexStorage2D(tex_target, 1, GL_RGBA32F, WindowWidth, WindowHeight);
   }
   else
   {
      glTexStorage2DMultisample(tex_target, num_samples, GL_RGBA32F, WindowWidth, WindowHeight, true);
   }

   glGenTextures(1, &TexFrontFaces);
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(tex_target, TexFrontFaces);
   if (tex_target == GL_TEXTURE_2D)
   {
      glTexStorage2D(tex_target, 1, GL_RGBA32F, WindowWidth, WindowHeight);
   }
   else
   {
      glTexStorage2DMultisample(tex_target, num_samples, GL_RGBA32F, WindowWidth, WindowHeight, true);
   }

   glGenTextures(1, &TexRender);
   glActiveTexture(GL_TEXTURE2);
   glBindTexture(tex_target, TexRender);
   if (tex_target == GL_TEXTURE_2D)
   {
      glTexStorage2D(tex_target, 1, GL_RGBA32F, WindowWidth, WindowHeight);
   }
   else
   {
      glTexStorage2DMultisample(tex_target, num_samples, GL_RGBA32F, WindowWidth, WindowHeight, true);
   }

   
   //create the depth buffer for the framebuffer object
   GLuint RBOID = 0;
   glGenRenderbuffers(1, &RBOID);
   glBindRenderbuffer(GL_RENDERBUFFER, RBOID);
   if (tex_target == GL_TEXTURE_2D)
   {
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight);
   }
   else
   {
      glRenderbufferStorageMultisample(GL_RENDERBUFFER, num_samples, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight);
   }
   

   //create the framebuffer object and attach the cubemap faces and the depth buffer
   glGenFramebuffers(1, &FBOID);
   glBindFramebuffer(GL_FRAMEBUFFER, FBOID);
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex_target, TexBackFaces, 0);
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, tex_target, TexFrontFaces, 0);
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, tex_target, TexRender, 0);
   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBOID);
   CheckFramebufferStatus();
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
   glError();
   BufferIndexedVertsUsingMapBuffer();
   LoadShader();

   glError();
}

void idle()
{
	static int last_time = 0;
	int time = glutGet(GLUT_ELAPSED_TIME);
	int elapsed = time-last_time;
	last_time = time;
	float delta_seconds = 0.001f*elapsed;
	
   seconds += delta_seconds;

   if(pauseRotation == false)
   {
      rev_per_min = 6.0f*seconds;
   }

   //set shader time uniform variable
	glUseProgram(program);

   if(Time_loc != -1)
   {
	   glUniform1f(Time_loc, seconds);
   }

	glutPostRedisplay();
}

void ExitGlut()
{
	glutDestroyWindow(win);
	exit(0);
}

void keyboard(unsigned char key, int x, int y)
{
   ImGui_ImplGlut_KeyCallback(key);

   if(key==27) //press ESC to exit
   {
      ExitGlut();
   }  
   if(key=='r' || key=='R')
   {
	   LoadShader();
   }
   if(key=='t' || key=='T')
   {
	   seconds = 0.0f;
   }

   if(key=='w' || key=='W')
   {
	   Qtrans.x += 0.1f*Qscale;
   }
   if(key=='a' || key=='A')
   {
	   Qtrans.y -= 0.1f*Qscale;
   }
   if(key=='s' || key=='S')
   {
	   Qtrans.x -= 0.1f*Qscale;
   }
   if(key=='d' || key=='D')
   {
	   Qtrans.y += 0.1f*Qscale;
   }
}



void special(int key, int x, int y)
{
   ImGui_ImplGlut_SpecialCallback(key);
   if(key==GLUT_KEY_F1)
   {
	   pauseRotation = !pauseRotation;
   }

   if(key==GLUT_KEY_UP)
   {
	   Qtrans.z += 0.1f*Qscale;
   }
   if(key==GLUT_KEY_DOWN)
   {
	   Qtrans.z -= 0.1f*Qscale;
   }
   if(key==GLUT_KEY_LEFT)
   {
	   Qscale = Qscale*0.99f;
   }
   if(key==GLUT_KEY_RIGHT)
   {
	   Qscale = Qscale*1.01f;
   }

}

void CreateGlutWindow()
{
   if (num_samples <= 1)
   {
      glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
   }
   else
   {
      glutSetOption(GLUT_MULTISAMPLE, num_samples); //Set number of samples per pixel
      glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
   }
	
	glutInitWindowPosition (5, 5);
	glutInitWindowSize (WindowWidth, WindowHeight);
	win = glutCreateWindow ("Raycast");
}

void keyboard_up(unsigned char key, int x, int y)
{
   ImGui_ImplGlut_KeyUpCallback(key);
}

void special_up(int key, int x, int y)
{
   ImGui_ImplGlut_SpecialUpCallback(key);
}

void passive(int x, int y)
{
   ImGui_ImplGlut_PassiveMouseMotionCallback(x,y);
}

void motion(int x, int y)
{
   ImGui_ImplGlut_MouseMotionCallback(x, y);
}

void mouse(int button, int state, int x, int y)
{
   ImGui_ImplGlut_MouseButtonCallback(button, state);
}

void CreateGlutCallbacks()
{
	glutDisplayFunc(display);
	glutIdleFunc(idle);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(special);

   glutKeyboardUpFunc(keyboard_up);
   glutSpecialUpFunc(special_up);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutPassiveMotionFunc(motion);
}

int main (int argc, char **argv)
{
	glutInit(&argc, argv); 
	CreateGlutWindow();
	CreateGlutCallbacks();
	InitOpenGL();
   ImGui_ImplGlut_Init(); // initialize the imgui system

	glutMainLoop();

	ExitGlut();
	return 0;		
}

