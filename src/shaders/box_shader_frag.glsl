#include "common.hpp"

BEGIN_SHADER(BoxFrag)

layout(location = 0) in vec4 inColor;
layout(location = 1) in float radius;
layout(location = 0) out vec4 outColor;


void main()
{
	outColor = inColor;
}


END_SHADER()