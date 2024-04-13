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

	Str LineFrag =
		R"( #version 300 es
		precision highp float;

        in vec4 voutColor;
		in vec2 voutP0;
		in vec2 voutP1;
		in float voutWidth;
		in float voutFeather;
		in float voutFeatherBegin;
		in float voutFeatherEnd;

		out vec4 outColor;
		
		uniform vec2 uScreenDims;

		void main()
		{
			vec2 p0 = vec2((1.f + voutP0.x) / 2.f, (1.f + voutP0.y) / 2.f) * uScreenDims;
			vec2 p1 = vec2((1.f + voutP1.x) / 2.f, (1.f + voutP1.y) / 2.f) * uScreenDims;

			vec2 p0p1 = p1 - p0;


			vec2 xy = gl_FragCoord.xy + 0.5f;
			float x = xy.x;
			float y = xy.y;

			float dist = abs(dot(p0p1, vec2(p1.y - y, p0.x - x))) / length(p0p1) - voutWidth * uScreenDims.x / 2.f;

			float alpha = clamp(1.f - smoothstep(0.f, voutFeather, dist), 0.f, 1.f);
			
			// Now cut out around the start and end points.
			vec2 t = normalize(p0 - p1);
			vec2 n = vec2(t.y, -t.x);

			vec2 xyp0 = xy - p0;
			vec2 xyp1 = xy - p1;

			vec2 xyp0TN = vec2(dot(xyp0, n), dot(xyp0, t));
			vec2 xyp1TN = vec2(dot(xyp1, n), dot(xyp1, -t));

			if (xyp0TN.y > 0.f)
			{
				float dist = xyp0TN.y;
				alpha *= clamp(1.f - smoothstep(0.f, voutFeatherBegin, dist), 0.f, 1.f);
			}

			if (xyp1TN.y > 0.f)
			{
				float dist = xyp1TN.y;
				alpha *= clamp(1.f - smoothstep(0.f, voutFeatherEnd, dist), 0.f, 1.f);
			}

			outColor = vec4(voutColor.rgb, alpha);
		}
	)";

}
