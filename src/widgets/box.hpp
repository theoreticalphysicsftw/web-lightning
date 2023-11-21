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

#include "widget.hpp"
#include "algebra/algebra.hpp"

namespace WL
{
	template <typename Runtime>
	class Box : Widget<Runtime>
	{
	public:
		Box(ColorU32 color = 0, F32 w = 0, F32 h = 0, F32 x = 0, F32 y = 0, F32 r = 0);

		virtual auto AccumulateDrawCommands() -> V override;
		virtual auto Update(const UpdateState& s) -> V override;

		ColorU32 color;
		F32 width;
		F32 height;
		F32 offsetX;
		F32 offsetY;
		F32 radius;
	};
}


namespace WL
{
	template<typename Runtime>
	Box<Runtime>::Box(ColorU32 color, F32 w, F32 h, F32 x, F32 y, F32 r) 
		: color(color), width(w), height(h), offsetX(x), offsetY(y), radius(r)
	{
	}

	template<typename Runtime>
	inline auto Box<Runtime>::AccumulateDrawCommands() -> V
	{
		Runtime::Renderer::BoxRenderer::AccumulateBox(color, width, height, offsetX, offsetY, radius);
	}


	template<typename Runtime>
	inline auto Box<Runtime>::Update(const UpdateState& s) -> V
	{

	}
}