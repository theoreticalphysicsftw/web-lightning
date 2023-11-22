#include "common.hpp"

namespace WL
{ 

	Str BoxFrag =
	R"(
		#version 300 es
		precision highp float;

		layout(location = 0) in vec4 inColor;
		layout(location = 1) in float radius;
		layout(location = 2) in vec2 upperLeft;
		layout(location = 3) in vec2 dims;

		layout(location = 0) out vec4 outColor;
		
		uniform vec2 uScreenDims;

		float roundedBoxSDF(vec2 center, vec2 size, float radius)
		{
			return length(max(abs(center) - size + radius, 0.f)) - radius;
		}

		void main()
		{
			float alpha = 1.f;
			if (radius > 0.f)
			{
				vec2 lowerLeft = vec2(upperLeft.x, upperLeft.y - upperLeft.y);

				//float distance = roundedBoxSDF(gl_FragCoord.xy - upperLeft - dims / 2.f, dims / 2.f, radius);
				float distance = length(gl_FragCoord.xy - upperLeft) * 0.002f;
				alpha = 1.f - smoothstep(0.f, 1.f, distance);
			}
			outColor = vec4(inColor.xyz, inColor.w * alpha);
		}
	)";

}