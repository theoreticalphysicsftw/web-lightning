#include "common.hpp"

namespace WL
{ 

	Str BoxFrag =
	R"( #version 300 es
		precision highp float;

        in vec4 voutColor;
        in float voutRadius;
        in vec2 voutCenterScreen;
        in vec2 voutDims;

		out vec4 outColor;
		
		uniform vec2 uScreenDims;

		float roundedBoxSDF(vec2 directionFromCenter, vec2 halfSide, float radius)
		{
			return length(max(abs(directionFromCenter) - halfSide + radius, 0.f)) - radius;
		}

		void main()
		{
			float alpha = 1.f;
			if (voutRadius > 0.f)
			{
				float distance = roundedBoxSDF(gl_FragCoord.xy - voutCenterScreen * uScreenDims, voutDims / 2.f * uScreenDims, voutRadius * uScreenDims.x);
				alpha = 1.f - smoothstep(0.f, 1.f, distance) 
;
			}
			outColor = vec4(voutColor.xyz, voutColor.w * alpha);
		}
	)";

}
