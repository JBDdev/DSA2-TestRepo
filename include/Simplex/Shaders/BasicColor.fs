#version 330

in vec3 Color;

vec3 CompColor;
uniform vec3 SolidColor = vec3(-1,-1,-1);
uniform bool Complement = false;
uniform vec3 CompOffset = vec3(1,1,1);

out vec4 Fragment;

void main()
{
	
	if(Complement)
	{
		CompColor = Color;
		CompColor.r = 1 - CompColor.r;
		CompColor.g = 1 - CompColor.g;
		CompColor.b = 1 - CompColor.b;
		Fragment = vec4(CompColor,1);
	}
	else
	{
		Fragment = vec4(Color,1);
	}
	
	
	if(SolidColor.r != -1.0 && SolidColor.g != -1.0 && SolidColor.b != -1.0){
		
		if(Complement)
		{
			CompColor = CompOffset - SolidColor;
			Fragment = vec4(CompColor, 1);
		}
		else
		{
			Fragment = vec4(SolidColor, 1);
		}
	}

	return;
}