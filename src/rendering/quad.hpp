// MIT License
// 
// Copyright (c) 2023 Mihail Mladenov
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


#pragma once

#include <algebra/algebra.hpp>

#include "color.hpp"

namespace WL
{
	struct DefaultQuad2D
	{
		static constexpr U32 verticesCount = 6;
		inline static Vec2 vertices[verticesCount] =
		{
			Vec2(-1.f, -1.f),
			Vec2(1.f, -1.f),
			Vec2(-1.f, 1.f),

			Vec2(-1.f, 1.f),
			Vec2(1.f, -1.f),
			Vec2(1.f, 1.f)
		};
	};

	template <typename TGPUAPI>
	struct QuadDesc2D
	{
		using GPUAPI = TGPUAPI;
		using Image = typename GPUAPI::Image;

		F32 offsetX = 0;
		F32 offsetY = 0;
		F32 width = 0;
		F32 height = 0;

		// Texture coordinates
		F32 u0 = 0;
		F32 v0 = 0;
		F32 u1 = 0;
		F32 v1 = 0;

		// Radius if it has rounded corners
		F32 radius;
		
		ColorU32 color = 0;

		Image* texture;
		B textured = false;
	};
}