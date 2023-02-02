#version 330

in vec3 Color;

uniform vec3 SolidColor = vec3(-1,-1,-1);
uniform bool Invert = false;

out vec4 Fragment;

void main()
{
	vec3 show = Color;
	Fragment = vec4(Color,1);
	
	if(SolidColor.r != -1.0 && SolidColor.g != -1.0 && SolidColor.b != -1.0)
		show = SolidColor;
	
	if(Invert == true)
		show = vec3(1.0f, 1.0f, 1.0f) - show;

	Fragment = vec4(show, 1);
	return;
}