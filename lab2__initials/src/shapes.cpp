#include <stdio.h>
#include <iostream>
#include <string.h>
#include <math.h>
#include <string>
#include <vector>
#include <GL/glut.h>
#include "math/vect3d.h"
#include "shapes.h"

using namespace std;


void ShapesC::Render()
{
	cout << "Base class cannot render" << "\n";
}

void ShapesC::SetPos(Vect3d tmp)
{
	pos=tmp;
}

void ShapesC::SetColor(GLubyte r,GLubyte g,GLubyte b)
{
	color[0]=r;
	color[1]=g;
	color[2]=b;
}

void TeapotC::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glColor3ubv(color);
	glTranslatef(pos.GetX(),pos.GetY(),pos.GetZ());
	glRotated(0, 1, 0, 0);
	glutWireTeapot(0.4);
	glPopMatrix();
}

void SphereC::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glColor3ubv(color);
	glTranslatef(pos.GetX(),pos.GetY(),pos.GetZ());
	glRotated(0, 1, 0, 0);
	glutWireSphere(0.4,10,10);
	glPopMatrix();
}

void ConeC::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glColor3ubv(color);
	glTranslatef(pos.GetX(), pos.GetY(), pos.GetZ());
	glRotated(-90, 1, 0, 0);
	glutWireCone(0.4, 0.8, 10, 10);
	glPopMatrix();
}

void CubeC::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glColor3ubv(color);
	glTranslatef(pos.GetX(), pos.GetY(), pos.GetZ());
	glRotated(-90, 1, 0, 0);
	glutWireCube(0.8);
	glPopMatrix();
}

void TorusC::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glColor3ubv(color);
	glTranslatef(pos.GetX(), pos.GetY(), pos.GetZ());
	glRotated(0, 1, 0, 0);
	glutWireTorus(0.3,0.6,10,10);
	glPopMatrix();
}

void DodecahedronC::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glColor3ubv(color);
	glTranslatef(pos.GetX(), pos.GetY(), pos.GetZ());
	glRotated(0, 1, 0, 0);
	glScalef(0.4,0.4,0.4);
	glutWireDodecahedron();
	glPopMatrix();
}

void OctahedronC::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glColor3ubv(color);
	glTranslatef(pos.GetX(), pos.GetY(), pos.GetZ());
	glRotated(0, 1, 0, 0);
	glScalef(0.4, 0.4, 0.4);
	glutWireOctahedron();
	glPopMatrix();
}

void TetrahedronC::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glColor3ubv(color);
	glTranslatef(pos.GetX(), pos.GetY(), pos.GetZ());
	glRotated(0, 1, 0, 0);
	glScalef(0.4, 0.4, 0.4);
	glutWireTetrahedron();
	glPopMatrix();
}

void IcosahedronC::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glColor3ubv(color);
	glTranslatef(pos.GetX(), pos.GetY(), pos.GetZ());
	glRotated(0, 1, 0, 0);
	glScalef(0.4, 0.4, 0.4);
	glutWireIcosahedron();
	glPopMatrix();
}