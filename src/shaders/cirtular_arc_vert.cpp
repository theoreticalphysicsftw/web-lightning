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
	Str CircularArcVert =
		R"( #version 300 es
		precision highp float;

		layout(location = 0) in vec2 center;
		layout(location = 1) in vec4 params;
		layout(location = 2) in uint color;

		out vec2 voutCenter;
		out vec4 voutParams;
		out vec4 voutColor;

		uniform vec2 uScreenDims;

		void main()
		{
			float radius = params.x;
			float width = params.y;
			float arcStart = params.z;
			float arcEnd = params.w;

			float ar = uScreenDims.x / uScreenDims.y;
			
			vec2 startVert = vec2(cos(arcStart), sin(arcStart) * ar) * radius + center;
			vec2 endVert = vec2(cos(arcEnd), sin(arcEnd) * ar) * radius + center;
			// Scale to create a bounding triangle;
			float scale = radius / length((endVert - startVert) / 2.f - center);

			vec2 positions[3] = vec2[3]
			(
				center,
				startVert * scale,
				endVert * scale
			);

			gl_Position = vec4(positions[gl_VertexID], 0.f, 1.f);

			voutCenter = center;
			voutParams = params;

			voutColor = vec4(
							  (color >> 24) & 0xFFu,
							  (color >> 16) & 0xFFu,
							  (color >> 8) & 0xFFu,
							  color & 0xFFu
						   ) / 255.f;
		}
	)";

}