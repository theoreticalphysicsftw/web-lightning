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
		in vec2 voutP0;
		in vec2 voutP1;
		in vec2 voutP2;
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
			vec2 p2 = vec2((1.f + voutP2.x) / 2.f, (1.f + voutP2.y) / 2.f) * uScreenDims;


			vec2 xy = gl_FragCoord.xy + 0.5f;
			float x = xy.x;
			float y = xy.y;

			float y1y2 = p1.y - p2.y;
			float x0x2 = p0.x - p2.x;
			float x2x1 = p2.x - p1.x;
			float y0y2 = p0.y - p2.y;

			float det = y1y2 * x0x2 + x2x1 * y0y2;
			
			float l0 = (y1y2 * (x - p2.x) + x2x1 * (y - p2.y)) / det;
			float l1 = (-y0y2 * (x - p2.x) + x0x2 * (y - p2.y)) / det;
			float l2 = 1.0f - l0 - l1;

			float u = 0.5f * l1 + l2;
			float v = l2;

			float dUdx = (0.5f * y0y2 - y1y2) / det;
			float dUdy = -(0.5f * x0x2 + x2x1) / det;
			float dVdx = (y0y2 - y1y2) / det;
			float dVdy = -(x2x1 + x0x2) / det;

			float implicit = u * u - v;

			vec2 grad = vec2(2.f * u * dUdx - dVdx, 2.f * u * dUdy - dVdy);

			float approxDist = abs(implicit) / length(grad) - (voutWidth * uScreenDims.x) / 2.f;
			float alpha = clamp(1.f - smoothstep(0.f, voutFeather, approxDist), 0.f, 1.f);
			

			// Now cut out around the start and end control points.
			vec2 t0 = normalize(p0 - p1);
			vec2 n0 = vec2(t0.y, -t0.x);
            vec2 t1 = normalize(p2 - p1);
			vec2 n1 = vec2(t1.y, -t0.x);

			vec2 xyp0 = xy - p0;
			vec2 xyp2 = xy - p2;

			vec2 xyp0T0N0 = vec2(dot(xyp0, n0), dot(xyp0, t0));
			vec2 xyp2T1N1 = vec2(dot(xyp2, n1), dot(xyp2, t1));

			if (xyp0T0N0.y > 0.f)
			{
				float dist = length(xyp0T0N0);
				float fadeout = (voutFeatherBegin > 0.f)? voutFeatherBegin : voutFeather;
				alpha *= clamp(1.f - smoothstep(0.f, fadeout, dist), 0.f, 1.f);
			}

			if (xyp2T1N1.y > 0.f)
			{
				float dist = length(xyp2T1N1);
				float fadeout = (voutFeatherEnd > 0.f)? voutFeatherEnd : voutFeather;
				alpha *= clamp(1.f - smoothstep(0.f, fadeout, dist), 0.f, 1.f);
			}

			if (voutFeatherBegin < 0.f)
			{
				alpha *= clamp(1.f - smoothstep(0.f, -voutFeatherBegin + xyp0T0N0.x, l0), 0.f, 1.f);
			}

			outColor = vec4(voutColor.rgb, alpha);
		}
	)";

}
