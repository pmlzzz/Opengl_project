#version 330
/*	(C) 2016
	Bedrich Benes 
	Purdue University
	bbenes<#!&^#>purdue<.>edu
*/	

vec4 Lighting(vec4 iPosition,vec3 iNorm)
{
 vec3 lightPos=vec3(0,10,-10);//positionVT
 vec3 kd=vec3(0.4,0.4,1); //diff material
 vec3 ld=vec3(1.0,1.0,1); //diffuse light
 vec3 lDir=normalize(lightPos-iPosition.xyz);
 float dLight=max(dot(iNorm,lDir),0.0);
 vec3  color=dLight*kd*ld;
 return vec4(color,1);
}



layout (location=0) in vec4 iPosition;
out vec4 theColor;
uniform float time;
uniform float bulge;
uniform float x;
uniform float y;
uniform float z;
uniform float s;
uniform float c;


mat4 modelViewProjection=mat4(1.0,0.0, 0.0,0.0,
						      0.0,1.0, 0.0,0.0,
						      0.0,0.0, 1.0,0.0,
						      0.0,0.0, 0.0,1.0);

mat4 matx=mat4(cos(time/50),-sin(time/50), 0.0,0.0,
						      sin(time/50),cos(time/50), 0.0,0.0,
						      0.0,0.0, 1.0,0.0,
						      0.0,0.0, 0.0,1.0);
							  
mat4 maty=mat4(1.0,0.0, 0.0,0.0,
						      0.0,cos(time/50), -sin(time/50),0.0,
						      0.0,sin(time/50), cos(time/50),0.0,
						      0.0,0.0, 0.0,1.0);

mat4 matz=mat4(cos(time/50),0.0,-sin(time/50),0.0,
						      0.0,1.0, 0.0,0.0,
						      sin(time/50),0.0, cos(time/50),0.0,
						      0.0,0.0, 0.0,1.0);
/*					  
mat4 modelViewProjection=mat4(1.0,0.0, 0.0,0.0,
						      0.0,1.0, 0.0,0.0,
						      0.0,0.0, 1.0,0.0,
						      0.0,0.0, 0.0,1.0)
					    *mat4(cos(time/50),-sin(time/50), 0.0,0.0,
						      sin(time/50),cos(time/50), 0.0,0.0,
						      0.0,0.0, 1.0,0.0,
						      0.0,0.0, 0.0,1.0)
						*mat4(1.0,0.0, 0.0,0.0,
						      0.0,cos(time/50), -sin(time/50),0.0,
						      0.0,sin(time/50), cos(time/50),0.0,
						      0.0,0.0, 0.0,1.0)
						*mat4(cos(time/50),0.0,-sin(time/50),0.0,
						      0.0,1.0, 0.0,0.0,
						      sin(time/50),0.0, cos(time/50),0.0,
						      0.0,0.0, 0.0,1.0);
							  */

void main()					
{
	float delta=bulge*sin(iPosition.x*time)*sin(iPosition.y*time/2.0)*sin(iPosition.z*time/3.0);
	vec4 dir=normalize(iPosition);
	vec4 tmp=iPosition+delta*dir;
	tmp.w=1; //make sure w is not modified!
	gl_Position=modelViewProjection*tmp;	
	theColor=vec4(0.0f, 0.0f, 0.0f, 1.0f);	
	gl_Position*=mat4(cos(x),-sin(x), 0.0,0.0,
						      sin(x),cos(x), 0.0,0.0,
						      0.0,0.0, 1.0,0.0,
						      0.0,0.0, 0.0,1.0)
							  *mat4(1.0,0.0, 0.0,0.0,
						      0.0,cos(z), -sin(z),0.0,
						      0.0,sin(z), cos(z),0.0,
						      0.0,0.0, 0.0,1.0)
							  *mat4(cos(y),0.0,-sin(y),0.0,
						      0.0,1.0, 0.0,0.0,
						      sin(y),0.0, cos(y),0.0,
						      0.0,0.0, 0.0,1.0)
							  *mat4(s,0.0, 0.0,0.0,
						      0.0,s, 0.0,0.0,
						      0.0,0.0, s,0.0,
						      0.0,0.0, 0.0,1.0);
	//gl_Position*=matx;
	//if(x==true) {gl_Position*=matx;}
	//if(y==true) {gl_Position*=maty;}
	//if(z==true) {gl_Posvition*=matz;}

	if(c==1.0f){theColor=vec4(1.0f, 0.0f, 0.0f, 1.0f);	}
	if(c==2.0f){theColor=vec4(0.0f, 1.0f, 0.0f, 1.0f);	}
	if(c==3.0f){theColor=vec4(0.0f, 0.0f, 1.0f, 1.0f);	}
			//set the output color
}
