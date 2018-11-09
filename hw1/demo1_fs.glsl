#version 400
out vec4 fragcolor;      
flat in vec4 color;              
void main(void)
{   

	//try to change the color to red
	//fragcolor = vec4(0.0,1.0,0.0,1.0);
	fragcolor =color;
}




















