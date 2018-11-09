#version 400
out vec4 fragcolor;      
in vec3 n;           
flat in int InstanceID;
uniform float time;
in vec3 pos;
void main(void)
{   

	//try to change the color to red
	fragcolor = vec4(0.4* InstanceID*sin(pos.x*5+time*10) ,0.1* (5-(InstanceID%3)),0.1* (10-(InstanceID%3))*sin(pos.y*5+time*10),0.0);
}




















