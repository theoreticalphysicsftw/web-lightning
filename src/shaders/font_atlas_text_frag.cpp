#include "common.hpp"

namespace WL
{
	Str FontAtlasTextFrag =
		R"( #version 300 es
		precision highp float;

        in vec2 voutUV;
        in vec4 voutColor;

		out vec4 outColor;
		
		uniform vec2 uScreenDims;
		uniform sampler2D uImage0;

		void main()
		{
			float alpha = texture(uImage0, voutUV).r;
			outColor = vec4(voutColor.rgb, alpha);
		}
	)";

}
