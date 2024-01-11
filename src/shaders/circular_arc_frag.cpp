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

	Str CircularArcFrag =
		R"( #version 300 es
		precision highp float;

		in vec2 voutCenter;
		in vec4 voutParams;
		in vec4 voutColor;

		out vec4 outColor;
		
		uniform vec2 uScreenDims;

		float arcSDF(vec2 position, vec2 center, float outerRadius, float innerRadius)
		{
			vec2 relativePos = center - position;
			float relativePosLength = length(relativePos);

			return max(innerRadius - relativePosLength, relativePosLength - outerRadius);
		}

		void main()
		{
			float radius = voutParams.x / 2.f * uScreenDims.x;
			float width = voutParams.y / 2.f * uScreenDims.x;
			vec2 center = vec2((voutCenter.x + 1.f) / 2.f, 1.f - (voutCenter.y + 1.f) / 2.f) * uScreenDims;
			float innerRadius = radius - width;

			float dist = arcSDF(gl_FragCoord.xy, center, radius, innerRadius);
			float alpha = 1.f - smoothstep(0.f, 1.f, dist);

			outColor = vec4(voutColor.rgb, alpha);
		}
	)";
}