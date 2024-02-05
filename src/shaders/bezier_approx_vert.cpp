// MIT License
// 
// Copyright (c) 2024 Mihail Mladenov
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#include "common.hpp"

namespace WL
{

	Str BezierApproxVert =
		R"( #version 300 es
		precision highp float;

		layout(location = 0) in uvec4 packedPointsAndColor;
		layout(location = 1) in vec2 dims;

        out vec4 voutColor;
        out vec2 voutUV;
		out float voutWidth;
		out float voutFeather;

		vec2 UVs[3] = vec2[3]
		(
			vec2(0.0f, 0.0f),
			vec2(0.5f, 0.0f),
			vec2(1.0f, 1.0f)
		);

		vec2 unpackPos(uint pos)
		{
			uvec2 uCoords = uvec2(pos & 0xFFFFu, (pos >> 16));
			vec2 fpos = vec2(uCoords & 0x7FFFu);
			bvec2 sign = bvec2(uCoords & 0x8000u);
			fpos /= float(0x7FFFu);
			fpos.x = sign.x? fpos.x : -fpos.x;
			fpos.y = sign.y? fpos.y : -fpos.y;
			return fpos;
		}

		void main()
		{
			vec2 position = unpackPos(packedPointsAndColor[gl_VertexID]);
			gl_Position = vec4(position, 0.f, 1.f);
			voutUV = UVs[gl_VertexID];

			uint color = packedPointsAndColor.w;
			voutColor = vec4(
							  (color >> 24) & 0xFFu,
							  (color >> 16) & 0xFFu,
							  (color >> 8) & 0xFFu,
							  color & 0xFFu
						   ) / 255.f;

			voutWidth = dims.x;
			voutFeather = dims.y;
		}
	)";

}
