#include "common.hpp"

namespace WL
{

	Str TexturedQuadFrag =
		R"( #version 300 es
		precision highp float;

        in vec2 voutUV;
        in float voutRadius;
        in vec2 voutCenterScreen;
        in vec2 voutDims;

		out vec4 outColor;
		
		uniform vec2 uScreenDims;
		uniform sampler2D uImage0;

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
				alpha = 1.f - smoothstep(0.f, 1.f, distance);
			}
			outColor = vec4(texture(uImage0, voutUV).rgb, alpha);
		}
	)";

}
