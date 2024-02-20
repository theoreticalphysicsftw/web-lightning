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

	Str LineVert =
		R"( #version 300 es
		precision highp float;

		layout(location = 0) in vec4 points;
		layout(location = 1) in uint color;
		layout(location = 2) in vec4 dims;

		uniform vec2 uScreenDims;

        out vec4 voutColor;
		out vec2 voutP0;
		out vec2 voutP1;
		out float voutWidth;
		out float voutFeather;
		out float voutFeatherBegin;
		out float voutFeatherEnd;


		void main()
		{
			voutWidth = dims.x;
			voutFeather = dims.y;
			voutFeatherBegin = dims.z;
			voutFeatherEnd = dims.w;
			voutP0 = points.xy;
			voutP1 = points.zw;

			vec2 dir = voutP0 - voutP1;
			vec2 norm = vec2(-dir.y, dir.x);

			float offset0 = (voutWidth + voutFeather / uScreenDims.x) / 2.f + 0.1f;
			float offset1 = (voutFeatherBegin + voutFeatherEnd) / uScreenDims.x;

			vec2 boundingRect[6] = vec2[6]
			(
				voutP0 + dir * offset1 - norm * offset0,
				voutP0 + dir * offset1 + norm * offset0,
				voutP1 - dir * offset1 - norm * offset0,
				voutP1 - dir * offset1 - norm * offset0,
				voutP1 - dir * offset1 + norm * offset0,
				voutP0 + dir * offset1 + norm * offset0
			);

			gl_Position = vec4(boundingRect[gl_VertexID], 0.f, 1.f);

			voutColor = vec4(
							  (color >> 24) & 0xFFu,
							  (color >> 16) & 0xFFu,
							  (color >> 8) & 0xFFu,
							  color & 0xFFu
						   ) / 255.f;

		}
	)";

}
