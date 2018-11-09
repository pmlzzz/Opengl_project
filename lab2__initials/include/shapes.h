#include <stdio.h>
#include <iostream>
#include <string.h>
#include <math.h>
#include <string>
#include <vector>
#include <GL/glut.h>
#include "math/vect3d.h"

using namespace std;

//Base class for shapes
class ShapesC
{
public:
	virtual void SetPos(Vect3d tmp);
	virtual void SetColor(GLubyte r,GLubyte g,GLubyte b);
	virtual void Render();

protected:
  Vect3d  pos;
  GLubyte color[3];
};

//derived class from ShapesC
class TeapotC: public ShapesC
{
public:
	virtual void Render();		
};

//derived class from ShapesC
class SphereC: public ShapesC
{
public:
	virtual void Render();		
};

class ConeC : public ShapesC
{
public:
	virtual void Render();
};

class CubeC : public ShapesC
{
public:
	virtual void Render();
};

class TorusC : public ShapesC
{
public:
	virtual void Render();
};

class DodecahedronC : public ShapesC
{
public:
	virtual void Render();
};

class OctahedronC : public ShapesC
{
public:
	virtual void Render();
};

class TetrahedronC : public ShapesC
{
public:
	virtual void Render();
};

class IcosahedronC : public ShapesC
{
public:
	virtual void Render();
};