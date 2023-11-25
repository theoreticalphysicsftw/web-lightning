#include "common.hpp"

namespace WL
{ 

	Str BoxFrag =
	R"(
		#version 300 es
		precision highp float;

		layout(location = 0) in vec4 inColor;
		layout(location = 1) in float radius;
		layout(location = 2) in vec2 centerScreen;
		layout(location = 3) in vec2 dims;

		layout(location = 0) out vec4 outColor;
		
		uniform vec2 uScreenDims;

		float roundedBoxSDF(vec2 directionFromCenter, vec2 halfSide, float radius)
		{
			return length(max(abs(directionFromCenter) - halfSide + radius, 0.f)) - radius;
		}

		void main()
		{
			float alpha = 1.f;
			if (radius > 0.f)
			{
				float distance = roundedBoxSDF(gl_FragCoord.xy - centerScreen * uScreenDims, dims / 2.f * uScreenDims, radius * uScreenDims.x);
				alpha = 1.f - smoothstep(0.f, 1.f, distance) 
;
			}
			outColor = vec4(inColor.xyz, inColor.w * alpha);
		}
	)";

}