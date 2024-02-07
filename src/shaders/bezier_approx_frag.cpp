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

	Str BezierApproxFrag =
		R"( #version 300 es
		precision highp float;

        in vec4 voutColor;
        in vec2 voutUV;
		in float voutWidth;
		in float voutFeather;

		out vec4 outColor;
		
		uniform vec2 uScreenDims;

		void main()
		{
			float implicit = voutUV.x * voutUV.x - voutUV.y;
			float dUdx = dFdx(voutUV.x);
			float dUdy = dFdy(voutUV.x);
			float dVdx = dFdx(voutUV.y);
			float dVdy = dFdy(voutUV.y);
			vec2 grad = vec2(2 * voutUV.x * dUdx - dVdx, 2 * voutUV.x * dUdy - dVdy);
			float approxDist = abs(implicit) / length(grad) - (voutWidth * uScreenDims.x - 1.f) / 2.f;
			float alpha = clamp(1.f - smoothstep(0.f, voutFeather, approxDist), 0.f, 1.f);

			outColor = vec4(voutColor.rgb * alpha, alpha);
		}
	)";

}
