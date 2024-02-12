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

		uniform vec2 uScreenDims;

        out vec4 voutColor;
		out vec2 voutP0;
		out vec2 voutP1;
		out vec2 voutP2;
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

		vec2 findIntersection(vec2 p0, vec2 n0, vec2 p1, vec2 n1)
		{
			vec3 projectiveN0 = vec3(n0, -dot(p0, n0));
			vec3 projectiveN1 = vec3(n1, -dot(p1, n1));
			vec3 projectiveIntersection = cross(projectiveN0, projectiveN1);
			return projectiveIntersection.xy / projectiveIntersection.z;
		}

		void main()
		{
			voutWidth = dims.x;
			voutFeather = dims.y;

			vec2 p[3] = vec2[3]
			(
				unpackPos(packedPointsAndColor[0]),
				unpackPos(packedPointsAndColor[1]),
				unpackPos(packedPointsAndColor[2])
			);
			
			
			vec2 p0p1 = normalize(p[1] - p[0]);
			vec2 n0 = vec2(-p0p1.y, p0p1.x);
			vec2 p1p2 = normalize(p[2] - p[1]);
			vec2 n1 = vec2(-p1p2.y, p1p2.x);
			vec2 p2p0 = normalize(p[0] - p[2]);
			vec2 n2 = vec2(-p2p0.y, p2p0.x);

			float offsetScale = 2 * (voutWidth + (voutFeather + 2.f) / uScreenDims.x);

			vec2 line0P = p[0] + offsetScale * n0;
			vec2 line1P = p[1] + offsetScale * n1;
			vec2 line2P = p[0] + offsetScale * n2;

			vec2 conservativeRasterizationPoints[3] = vec2[3]
			(
				findIntersection(line0P, n0, line2P, n2),
				findIntersection(line2P, n2, line1P, n1),
				findIntersection(line0P, n0, line1P, n1)
			);
			
			gl_Position = vec4(conservativeRasterizationPoints[gl_VertexID], 0.f, 1.f);
			
			voutP0 = p[0];
			voutP1 = p[1];
			voutP2 = p[2];

			uint color = packedPointsAndColor.w;
			voutColor = vec4(
							  (color >> 24) & 0xFFu,
							  (color >> 16) & 0xFFu,
							  (color >> 8) & 0xFFu,
							  color & 0xFFu
						   ) / 255.f;

		}
	)";

}
