#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace; 
layout(location = 1) in vec3 vertexColor; 
 
//argument do wys³ania do fragment shadera.
out vec3 pos; 
out vec3 color;
//argumenty pochodz¹ce z poziomu hosta (CPU)
uniform mat4 MVP;
uniform float width;
uniform float height;

void main()
{	
	gl_Position = MVP *  vec4(vertexPosition_modelspace, 1);
	color = vertexPosition_modelspace;
}