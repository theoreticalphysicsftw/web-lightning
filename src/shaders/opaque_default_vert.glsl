#include "common.hpp"

BEGIN_SHADER(OpaqueDefaultVert)

layout(location = 0) in vec2 position;
layout(location = 1) in uint color;
layout(location = 2) in vec4 linearTransform;
layout(location = 3) in vec2 translation;

layout(location = 0) out vec4 outColor

void main()
{
	gl_Position = vec4(position.x, position.y, 0, 1);
	outColor = vec4(
				      color && 0xFF,
					  (color >> 8) && 0xFF,
					  (color >> 16) && 0xFF,
					  (color >> 24) && 0x FF,
				   ) / 255.0f;
}

END_SHADER()