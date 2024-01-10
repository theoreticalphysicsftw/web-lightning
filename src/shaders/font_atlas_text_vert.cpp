#include "common.hpp"

namespace WL
{

	Str FontAtlasTextVert =
		R"( #version 300 es
		precision highp float;

		layout(location = 0) in vec4 linearTransform;
		layout(location = 1) in vec2 translation;
		layout(location = 2) in vec4 uvBbox;
		layout(location = 3) in uint color;

		out vec2 voutUV;
		out vec4 voutColor;

	    vec2 positions[6] = vec2[6]
		(
			vec2(-1.f, -1.f),
			vec2(1.f, -1.f),
			vec2(-1.f, 1.f),

			vec2(-1.f, 1.f),
			vec2(1.f, -1.f),
			vec2(1.f, 1.f)
		);

		vec4 uvTransforms[6] = vec4[6]
		(
			vec4(1.f, 0.f, 0.f, 1.f),
			vec4(0.f, 0.f, 1.f, 1.f),
			vec4(1.f, 1.f, 0.f, 0.f),

			vec4(1.f, 1.f, 0.f, 0.f),
			vec4(0.f, 0.f, 1.f, 1.f),
			vec4(0.f, 1.f, 1.f, 0.f)
		);

		void main()
		{
			mat2x2 linearComponent;
			linearComponent[0] = linearTransform.xy;
			linearComponent[1] = linearTransform.zw;
			vec2 outPos = linearComponent * positions[gl_VertexID] + translation;

			gl_Position = vec4(outPos.x, outPos.y, 0.f, 1.f);

			voutColor = vec4(
							  (color >> 24) & 0xFFu,
							  (color >> 16) & 0xFFu,
							  (color >> 8) & 0xFFu,
							  color & 0xFFu
						   ) / 255.f;			

			voutUV = uvBbox.xy * uvTransforms[gl_VertexID].xy + uvBbox.zw * uvTransforms[gl_VertexID].zw;
		}
	)";

}