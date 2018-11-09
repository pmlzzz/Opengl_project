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
#include "LoadTexture.h"
#include "imgui_impl_glut.h"
using namespace std;
static const std::string vertex_shader("fbo_vs.glsl");
static const std::string fragment_shader("fbo_fs.glsl");

GLuint shader_program = -1;
GLuint texture_id = -1; //Texture map for fish
GLuint texture_id1 = -1; //Texture map for fish
GLuint texture_id2 = -1; //Texture map for fish
GLuint picktexture = -1;

GLuint quad_vao = -1;
GLuint quad_vbo = -1;

GLuint fbo_id = -1;       // Framebuffer object,
GLuint rbo_id = -1;       // and Renderbuffer (for depth buffering)
GLuint fbo_texture = -1;  // Texture rendered into.
GLuint fbo_texture2 = -1;  // Texture rendered into.
int id = -1;
int pick = 0;
static const std::string mesh_name = "Amago0.obj";
static const std::string texture_name = "AmagoT.bmp";
static const std::string texture_name1 = "AmagoT.bmp";
static const std::string texture_name2 = "pick.bmp";
MeshData mesh_data;
float time_sec = 0.0f;
float angle = 0.0f;
static float color[4] = { 1.0f,1.0f,1.0f,1.0f };
bool check_framebuffer_status();
float scale = 1.0f;
float camerascale = 1.0f;
bool no_LOD = true;
static int magification = 0;
static int minification = 0;
void draw_pass_1()
{
   const int pass = 1;

   glm::mat4 M = glm::rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f))*glm::scale(glm::vec3(mesh_data.mScaleFactor));
   glm::mat4 V = glm::lookAt(camerascale*glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
   glm::mat4 P = glm::perspective(3.141592f/4.0f, 1.0f, 0.1f, 100.0f);
   M = M* glm::scale(glm::vec3(scale, scale, scale));
   int pass_loc = glGetUniformLocation(shader_program, "pass");
   if(pass_loc != -1)
   {
      glUniform1i(pass_loc, pass);
   }

   
   
   int PVM_loc = glGetUniformLocation(shader_program, "PVM");
   if(PVM_loc != -1)
   {
      glm::mat4 PVM = P*V*M;
      glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
   }
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texture_id);
   if (minification == 0)
   {
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	   //cout << "minification0" << endl;
   }
   else if (minification == 1)
   {
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	  // cout << "minification1" << endl;
   }
   else if (minification == 2)
   {
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	   //cout << "minification2" << endl;
   }
   if (magification == 0)
   {
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	   //cout << "magification0" << endl;
   }
   else if (magification == 1)
   {
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	   //cout << "magification1" << endl;
   }
   int tex_loc = glGetUniformLocation(shader_program, "texture");
   if (tex_loc != -1)
   {
	   glUniform1i(tex_loc, 0); // we bound our texture to texture unit 0
   }
   
 
   if (pick == 1)
   {
	   glActiveTexture(GL_TEXTURE1);
	   glBindTexture(GL_TEXTURE_2D, texture_id2);
	   int tex_loc2 = glGetUniformLocation(shader_program, "texture");
	   if (tex_loc2 != -1)
	   {
		   glUniform1i(tex_loc2, 1); // we bound our texture to texture unit 0
	   }

   }
   
	  
   
  
   //glActiveTexture(GL_TEXTURE0);
   //glBindTexture(GL_TEXTURE_2D, picktexture);

   glBindVertexArray(mesh_data.mVao);
   glDrawElementsInstanced(GL_TRIANGLES, mesh_data.mNumIndices, GL_UNSIGNED_INT, 0, 9);
}

void draw_pass_2()
{
   const int pass = 2;
   int pass_loc = glGetUniformLocation(shader_program, "pass");
   if(pass_loc != -1)
   {
      glUniform1i(pass_loc, pass);
   }

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, fbo_texture);
  
   int tex_loc = glGetUniformLocation(shader_program, "texture");
   if (tex_loc != -1)
   {
	   glUniform1i(tex_loc, 0); // we bound our texture to texture unit 0
   }
   /*if (pick == 1)
   {
	   glActiveTexture(GL_TEXTURE1);
	   glBindTexture(GL_TEXTURE_2D, fbo_texture2);
	   int tex_loc2 = glGetUniformLocation(shader_program, "texture");
	   if (tex_loc2 != -1)
	   {
		   glUniform1i(tex_loc2, 1); // we bound our texture to texture unit 0
	   }

   }*/


   
	   
	   
   
   
   



   glBindVertexArray(quad_vao);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	

}

void APIENTRY openglCallbackFunction(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam) {

	cout << "---------------------opengl-callback-start------------" << endl;
	cout << "message: " << message << endl;
	cout << "type: ";
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		cout << "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		cout << "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		cout << "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		cout << "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		cout << "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		cout << "OTHER";
		break;
	}
	cout << endl;

	cout << "id: " << id << endl;
	cout << "severity: ";
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:
		cout << "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		cout << "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		cout << "HIGH";
		break;
	}
	cout << endl;
	cout << "---------------------opengl-callback-end--------------" << endl;
}
void draw_gui()
{
   ImGui_ImplGlut_NewFrame();

   //create a slider to change the angle variables
   #if _DEBUG
   ImGui::SliderFloat("View angle", &angle, -3.141592f, +3.141592f);
   if (ImGui::ImageButton((void*)texture_id1, ImVec2(100, 100)))
   {
	   color[1]; pick = 0;

   }
   if (ImGui::ImageButton((void*)texture_id2, ImVec2(100, 100)))
   {
	   color[1]; pick = 1;
	   
	   

   }
   ImGui::ImageButton((void*)fbo_texture, ImVec2(100, 100));
   ImGui::ColorEdit4("color", color);
   ImGui::SliderFloat("scale", &scale, 0.1f, 10.0f);
   ImGui::SliderFloat("camerascale", &camerascale, 0.1f, 10.0f);
   ImGui::Combo("magification", &magification, " GL_NEAREST\0GL_LINEAR\0\0");   // Combo using values packed in a single constant string (for really quick combo)
   ImGui::Combo("minification", &minification, " GL_NEAREST\0GL_LINEAR\0GL_LINEAR_MIPMAP_LINEAR\0\0");   // Combo using values packed in a single constant string (for really quick combo)
   ImGui::Checkbox("no_LOD", &no_LOD); ImGui::SameLine(200);
	#endif
   int no_LOD_loc = glGetUniformLocation(shader_program, "no_LOD");
   glUniform1i(no_LOD_loc, no_LOD);
   int colorParameter = glGetUniformLocation(shader_program, "color");
   glUniform4fv(colorParameter, 1, color);
   

   static bool show = false;
   ImGui::ShowTestWindow(&show);
   ImGui::Render();
 }

// glut display callback function.
// This function gets called every time the scene gets redisplayed 
void display()
{

   glUseProgram(shader_program);

   glBindFramebuffer(GL_FRAMEBUFFER, fbo_id); // Render to FBO.
   glDrawBuffer(GL_COLOR_ATTACHMENT0); //Out variable in frag shader will be written to the texture attached to GL_COLOR_ATTACHMENT0.
   GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
   glDrawBuffers(2, drawBuffers);
   //Make the viewport match the FBO texture size.
   int tex_w, tex_h;
   glBindTexture(GL_TEXTURE_2D, fbo_texture2);
   glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tex_w);
   glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &tex_h);
   glViewport(0, 0, tex_w, tex_h);

   //clear the FBO
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   draw_pass_1();
         
   glBindFramebuffer(GL_FRAMEBUFFER, 0); // Do not render the next pass to FBO.
   //glDrawBuffer(GL_BACK); // Render to back buffer.

   const int w = glutGet(GLUT_WINDOW_WIDTH);
   const int h = glutGet(GLUT_WINDOW_HEIGHT);
   glViewport(0, 0, w, h); //Render to the full viewport
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the back buffer

   draw_pass_2();

   draw_gui();

   glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();

   const int time_ms = glutGet(GLUT_ELAPSED_TIME);
   time_sec = 0.001f*time_ms;
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

   reload_shader();
   const int w = glutGet(GLUT_WINDOW_WIDTH);
   const int h = glutGet(GLUT_WINDOW_HEIGHT);
   //mesh and texture for pass 1
   mesh_data = LoadMesh(mesh_name);
   texture_id = LoadTexture(texture_name.c_str());
   texture_id1 = LoadTexture(texture_name1.c_str());
   
  
   
   
   //mesh for pass 2 (full screen quadrilateral)
   glGenVertexArrays(1, &quad_vao);
   glBindVertexArray(quad_vao);

   float vertices[] = {1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f};

   //create vertex buffers for vertex coords
   glGenBuffers(1, &quad_vbo);
   glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   int pos_loc = glGetAttribLocation(shader_program, "pos_attrib");
   if(pos_loc >= 0)
   {
      glEnableVertexAttribArray(pos_loc);
	   glVertexAttribPointer(pos_loc, 3, GL_FLOAT, false, 0, 0);
   }

   //create texture to render pass 1 into
   
   glGenTextures(1, &fbo_texture);
   glBindTexture(GL_TEXTURE_2D, fbo_texture);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

   
   
   glGenerateMipmap(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, 0);   
   
   glGenTextures(1, &fbo_texture2);
   glBindTexture(GL_TEXTURE_2D, fbo_texture2);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glBindTexture(GL_TEXTURE_2D, 0);

   //Create renderbuffer for depth.
   glGenRenderbuffers(1, &rbo_id);
   glBindRenderbuffer(GL_RENDERBUFFER, rbo_id);
   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);

   //Create the framebuffer object
   glGenFramebuffers(1, &fbo_id);
   glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0);
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, fbo_texture2, 0);
   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_id); // attach depth renderbuffer

   check_framebuffer_status();

   glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

// glut callbacks need to send keyboard and mouse events to imgui
void keyboard(unsigned char key, int x, int y)
{
   ImGui_ImplGlut_KeyCallback(key);

	#if _DEBUG
   std::cout << "key : " << key << ", x: " << x << ", y: " << y << std::endl;
	#endif
   switch(key)
   {
      case 'r':
      case 'R':
         reload_shader();     
      break;
   }
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

void special(int key, int x, int y)
{
   ImGui_ImplGlut_SpecialCallback(key);
}

void motion(int x, int y)
{
   ImGui_ImplGlut_MouseMotionCallback(x, y);
}

void mouse(int button, int state, int x, int y)
{
   ImGui_ImplGlut_MouseButtonCallback(button, state);
   unsigned char buffer[4];
   glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
   glReadBuffer(GL_COLOR_ATTACHMENT1);
   glPixelStorei(GL_PACK_ALIGNMENT, 1);
   glReadPixels(x, 640 - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
   if (pick == 1)
   {
	   id = buffer[0];
	   if (id != 89)
	   {
		   cout << (id - 10) / 10 << endl;
	   }
   }
   
   
   
   int pick_loc = glGetUniformLocation(shader_program, "pick");
   if (pick_loc != -1)
   {
	   glUniform1i(pick_loc, id);
   }
}


int main (int argc, char **argv)
{
   //Configure initial window state
   glutInit(&argc, argv); 
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowPosition (5, 5);
   glutInitWindowSize (640, 640);
   int win = glutCreateWindow ("Imgui demo");

   printGlInfo();

   //Register callback functions with glut. 
   glutDisplayFunc(display); 
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(special);
   glutKeyboardUpFunc(keyboard_up);
   glutSpecialUpFunc(special_up);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutPassiveMotionFunc(motion);

   glutIdleFunc(idle);
   ImGui_ImplGlut_Init(); // initialize the imgui system
   initOpenGl();
   
#if _DEBUG
   if (glDebugMessageCallback) {
	   std::cout << "Register OpenGL debug callback " << std::endl;
	   glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	   glDebugMessageCallback(openglCallbackFunction, nullptr);
	   GLuint unusedIds = 0;
	   glDebugMessageControl(GL_DONT_CARE,
		   GL_DONT_CARE,
		   GL_DONT_CARE,
		   0,
		   &unusedIds,
		   true);
   }
   else
	   std::cout << "glDebugMessageCallback not available" << std::endl;
#endif
   //Enter the glut event loop.
   glutMainLoop();
   glutDestroyWindow(win);

   return 0;	

}

bool check_framebuffer_status() 
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


