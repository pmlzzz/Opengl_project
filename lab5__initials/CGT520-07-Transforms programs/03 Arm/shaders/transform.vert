#version 400
/*	(C) 2012 
	Bedrich Benes 
	Purdue University
	bbenes<#!&^#>purdue<.>edu
*/	


layout (location=0) in vec4 iPosition;
out vec4 theColor;
uniform float time;
uniform mat4 modelview;
void main()					
{
	gl_Position=modelview*iPosition;	
	theColor=vec4(1,1,1,1);			//set the output color
}
