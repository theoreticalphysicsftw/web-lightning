// MIT License
// 
// Copyright (c) 2023 - 2024 Mihail Mladenov
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
