//uwydatnianie krawedzi 
#version 330 core 
uniform sampler2D myTextureSampler;
out vec4 pixelColors;

//argument pochodz¹cy z Vertex_Shadera
in vec3 pos;
in vec3 color;

uniform float height;
uniform float width;
uniform float value;
//https://forum.processing.org/two/discussion/4173/threshold-shader
void main()
{	
	vec2 coord = vec2(gl_FragCoord.x/width, gl_FragCoord.y/height);
	vec4 texColor = texture(myTextureSampler, coord).rgba;
	float brightness = (0.2126*texColor.r) + (0.7152*texColor.g) + (0.0722*texColor.b);
	if (brightness > value) {
		pixelColors = vec4(1.0, 1.0, 1.0, 1.0);
	} else {
		pixelColors = vec4(0.0, 0.0, 0.0, 1.0);
	}
}