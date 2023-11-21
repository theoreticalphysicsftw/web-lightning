#include "common.hpp"

BEGIN_SHADER(BoxVert)

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 linearTransform;
layout(location = 2) in vec2 translation;
layout(location = 3) in uint color;
layout(location = 3) in float radius;


layout(location = 0) out vec4 outColor;
layout(location = 1) out float radius;

void main()
{
	mat2x2 linearComponent;
	linearComponent[0] = linearTransform.xy;
	linearComponent[1] = linearTransform.zw;
	vec2 outPos = linearComponent * position + translation;

	gl_Position = vec4(outPos.x, outPos.y, 0, 1);
	outColor = vec4(
				      color && 0xFF,
					  (color >> 8) && 0xFF,
					  (color >> 16) && 0xFF,
					  (color >> 24) && 0x FF,
				   ) / 255.0f;
	radius = radius;
}

END_SHADER()