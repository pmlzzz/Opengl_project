/********************************/
/* .net 2010 sample framework   */
/* (C) Bedrich Benes 2012       */
/* bbenes ~ at ~ purdue.edu     */
/********************************/

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <math.h>
#include <string>
#include <vector>			//Standard template library class
#include <GL/glut.h>

//in house created libraries
#include "math/vect3d.h"    //for vector manipulation
#include "shapes.h"			//shapes used in this project

using namespace std;

//the main container of the shapes is a STL vector of pointers to ShapesC
//ShapesC are polymorphed to other shapes in shapes.cpp
vector <ShapesC*> shapes;

//the main window size
GLint wWindow=800;
GLint hWindow=600;
bool mouseLeftDown;
bool mouseRightDown;
float mouseX, mouseY;
float px, py;
float cameraAngleX;
float cameraAngleY;
float R = 255;
float G = 0;
float B = 0;
int lock = 0;
int objectnumber;
int colornumber;
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
  glViewport(50,0,w, h);       
  glEnable(GL_DEPTH_TEST);
//remember the settings for the camera
  wWindow=w;
  hWindow=h;
}


//the main rendering function
void RenderObjects()
{
//display the ground
  glBegin(GL_QUADS);
   glColor3ub(0,128,0);
   glVertex3f(-3,-0.6,0);
   glVertex3f(3,-0.6,0);
   glVertex3f(3,-0.6,-20);
   glVertex3f(-3,-0.6,-20);
  glEnd();

//draw the objects
  for (unsigned int i=0;i<shapes.size();i++)
  {
	  shapes[i]->Render();  //by using pointers, polymorphism will call the proper functions
  }//of for
}
	

//the display callback
void Display(void)
{
  glClearColor(0.9,0.9,0.9,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  GLMessage("Hi There, Right button :select mode, Left button: rotate mode ");
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40,(GLfloat)wWindow/(GLfloat)hWindow,0.01,100); //set the camera
  glMatrixMode(GL_MODELVIEW); //set the scene
  glLoadIdentity();
  glTranslatef(0,-1,-6);      //move the scena a little bit back and down
  glRotatef(cameraAngleX, 0, 1, 0);//rotate y
  //glRotatef(cameraAngleY, 1, 0, 0);//rotate x
  RenderObjects();			  //render the scene
  glutSwapBuffers();  
}

//keyboard callback
void Kbd(unsigned char a, int x, int y)
{
	switch(a)
	{
 	  case 27 : exit(0);break;
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
		    printf("asdsdasdasdasda");
		    cameraAngleY -= 45;
		    break;
		  }
	  case GLUT_KEY_RIGHT : 
		  {
		    cameraAngleY += 45;
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





//Initialization of the object
//each object is created in dynamic memory (using new) 
//pointer to it is added to the ShapeC* container
void InitShapes()
{
	switch (objectnumber)
	{
	case 1:
	{
		if(!shapes.empty()) shapes.erase(shapes.begin());
		TeapotC *teapot;
		teapot = new(TeapotC);
		teapot->SetColor(R, G, B);
		teapot->SetPos(Vect3d(0, 0, 0));
		shapes.push_back(teapot);
		break;
	}
	case 2:
	{
		if (!shapes.empty()) shapes.erase(shapes.begin());
		SphereC *sphere;
		sphere = new(SphereC);
		sphere->SetColor(R, G, B);
		sphere->SetPos(Vect3d(0, 0, 0));
		shapes.push_back(sphere);
		break;
	}
	case 3:
	{
		if (!shapes.empty()) shapes.erase(shapes.begin());
		ConeC *cone;
		cone = new(ConeC);
		cone->SetColor(R, G, B);
		cone->SetPos(Vect3d(0, 0, 0));
		shapes.push_back(cone);
		break;
	}
	case 4:
	{
		if (!shapes.empty()) shapes.erase(shapes.begin());
		TorusC *torus;
		torus = new(TorusC);
		torus->SetColor(R, G, B);
		torus->SetPos(Vect3d(0, 0, 0));
		shapes.push_back(torus);
		break;
	}
	case 5:
	{
		if (!shapes.empty()) shapes.erase(shapes.begin());
		CubeC *cube;
		cube = new(CubeC);
		cube->SetColor(R, G, B);
		cube->SetPos(Vect3d(0, 0, 0));
		shapes.push_back(cube);
		break;
	}
	case 6:
	{
		if (!shapes.empty()) shapes.erase(shapes.begin());
		TetrahedronC *tetrahedron;
		tetrahedron = new(TetrahedronC);
		tetrahedron->SetColor(R, G, B);
		tetrahedron->SetPos(Vect3d(0, 0, 0));
		shapes.push_back(tetrahedron);
		break;
	}
	case 7:
	{
		if (!shapes.empty()) shapes.erase(shapes.begin());
		OctahedronC *octahedron;
		octahedron = new(OctahedronC);
		octahedron->SetColor(R, G, B);
		octahedron->SetPos(Vect3d(0, 0, 0));
		shapes.push_back(octahedron);
		break;
	}
	case 8:
	{
		if (!shapes.empty()) shapes.erase(shapes.begin());
		DodecahedronC *dodecahedron;
		dodecahedron = new(DodecahedronC);
		dodecahedron->SetColor(R, G, B);
		dodecahedron->SetPos(Vect3d(0, 0, 0));
		shapes.push_back(dodecahedron);
		break;
	}
	case 9:
	{
		if (!shapes.empty()) shapes.erase(shapes.begin());
		IcosahedronC *icosahedron;
		icosahedron = new(IcosahedronC);
		icosahedron->SetColor(R, G, B);
		icosahedron->SetPos(Vect3d(0, 0, 0));
		shapes.push_back(icosahedron);
		break;
	}
	default: break;
	}
	glutPostRedisplay();
}

void passivemouse(int x, int y)
{

	if (colornumber == 1)
	{
		cout << "RRRLocation is " << "[" << x << "'" << y << "]" << "RGB is " << "[" << R << "," << G << "," << B << "]" << endl;
		if (lock == 0)
		{
			px = x;
			py = y;
			lock++;
		}
		else
		{
			R += (x - px)*0.8;
			if (R > 255) R = 255; px = x;
			if (R < 0) R = 0; px = x;
		}
	}
	else if (colornumber == 2)
	{
		cout << "GGGLocation is " << "[" << x << "'" << y << "]" << "RGB is "<<"["<<R <<"," << G <<","<<B <<"]"<<endl;
		if (lock == 0)
		{
			px = x;
			py = y;
			lock++;
		}
		else
		{
			G += (x - px)*0.8;
			if (G > 255) G = 255; px = x;
			if (G < 0) G = 0; px = x;
		}
	}
	else if (colornumber == 3)
	{
		cout << "BBBLocation is " << "[" << x << "'" << y << "]" << "RGB is " << "[" << R << "," << G << "," << B << "]" << endl;
		if (lock == 0)
		{
			px = x;
			py = y;
			lock++;
		}
		else
		{
			B += (x - px)*0.8;
			if (B > 255) B = 255; px = x;
			if (B < 0) B = 0; px = x;
		}
	}
	InitShapes();
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
}
void mouseMotion(int x, int y)
{
	if (mouseLeftDown)
	{
		cameraAngleX += (x - mouseX);
		cameraAngleY += (y - mouseY);
		mouseX = x;
		mouseY = y;
	}
	glutPostRedisplay();
}

void menu(int val)
{
	switch (val)
	{
	case 1:colornumber = 1; break;
	case 2:colornumber = 2; break;
	case 3:colornumber = 3; break;
	case 4:colornumber = 4; break;
	case 5:objectnumber = 1; break;
	case 6:objectnumber = 2; break;
	case 7:objectnumber = 3;  break;
	case 8:objectnumber = 4;  break;
	case 9:objectnumber = 5; break;
	case 10:objectnumber = 6; break;
	case 11:objectnumber = 7; break;
	case 12:objectnumber = 8;  break;
	case 13:objectnumber = 9; break;
	case 14:break;
	case 15:break;
	case 16:break;
	case 17:exit(0); break;
	}
	glutPassiveMotionFunc(passivemouse);
	glutPostRedisplay();
}

void makemenu()
{
	int s1, s2;
	s1 = glutCreateMenu(menu);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 3);
	glutAddMenuEntry("None", 4);

	s2 = glutCreateMenu(menu);
	glutAddMenuEntry("Teapot", 5);
	glutAddMenuEntry("Sphere", 6);
	glutAddMenuEntry("Cone", 7);
	glutAddMenuEntry("Torus", 8);
	glutAddMenuEntry("Cube", 9);
	glutAddMenuEntry("Tetrahedron", 10);
	glutAddMenuEntry("Octahedron", 11);
	glutAddMenuEntry("Dodecahedron", 12);
	glutAddMenuEntry("Icosahedron", 13);


	glutCreateMenu(menu);
	glutAddSubMenu("color", s1);
	glutAddSubMenu("Object", s2);

	glutAddMenuEntry("gg", 14);
	glutAddMenuEntry("gl", 15);
	glutAddMenuEntry("wp", 16);
	glutAddMenuEntry("quit", 17);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutPostRedisplay();
}


int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(wWindow,hWindow);
  glutInitWindowPosition(1000, 100);
  glutCreateWindow("Simple OpenGL framework");
  glutIgnoreKeyRepeat(GL_FALSE);                     
  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);
  glutIgnoreKeyRepeat(1);//YES, ignore key repeat please
  glutKeyboardFunc(Kbd); //+ and -
  glutSpecialUpFunc(SpecKbdRelease); //smooth motion
  glutSpecialFunc(SpecKbdPress);

  glutMouseFunc(mouse);
  glutMotionFunc(mouseMotion);
  makemenu();
  InitShapes();
  glutMainLoop();
  return 0;        
}
