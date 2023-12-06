#include "common.hpp"

namespace WL
{

	Str BoxVert =
	R"( #version 300 es
		precision highp float;

		layout(location = 0) in vec2 position;
		layout(location = 1) in vec4 linearTransform;
		layout(location = 2) in vec2 translation;
		layout(location = 3) in uint color;
		layout(location = 4) in float radius;

		out vec4 voutColor;
		out float voutRadius;
		out vec2 voutCenterScreen;
	    out vec2 voutDims;

		void main()
		{
			mat2x2 linearComponent;
			linearComponent[0] = linearTransform.xy;
			linearComponent[1] = linearTransform.zw;
			vec2 outPos = linearComponent * position + translation;

			gl_Position = vec4(outPos.x, outPos.y, 0.f, 1.f);
			voutColor = vec4(
							  (color >> 24) & 0xFFu,
							  (color >> 16) & 0xFFu,
							  (color >> 8) & 0xFFu,
							  color & 0xFFu
						   ) / 255.f;
			voutRadius = radius;
			voutDims = vec2(linearTransform.x, linearTransform.w);
			voutCenterScreen = (translation + 1.f) / 2.f;
		}
	)";

}
