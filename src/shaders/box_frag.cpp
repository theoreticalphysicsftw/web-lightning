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
				float dist = roundedBoxSDF(gl_FragCoord.xy - voutCenterScreen * uScreenDims, voutDims / 2.f * uScreenDims, voutRadius * uScreenDims.x);
				alpha = 1.f - smoothstep(0.f, 1.f, dist);
			}
			outColor = vec4(voutColor.xyz, voutColor.w * alpha);
		}
	)";

}
