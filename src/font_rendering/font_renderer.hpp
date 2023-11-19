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

#include <common/types.hpp>
#include <algebra/algebra.hpp>

namespace WL
{
	struct FontRendererOptions
	{
		F32 height = 0.05f;
		Color4 color = Color4(1.f, 0.f, 0.f, 0.f);
		F32 offsetX = 0;
		F32 offestY = 0;
	};

	template <typename GPUAPI, typename Rasterizer>
	class FontRenderer
	{
	public:
		auto AccumulateDrawCommands(const Array<I32> codepoints, const FontRendererOptions& options) -> V;
		auto CommitDrawCommands() -> V;

	private:
		GPUAPI::Pso pso;
	};

}

namespace WL
{
	template<typename GPUAPI, typename Rasterizer>
	inline auto FontRenderer<GPUAPI, Rasterizer>::AccumulateDrawCommands(const Array<I32> codepoints, const FontRendererOptions& options) -> V
	{
		return V();
	}


	template<typename GPUAPI, typename Rasterizer>
	inline auto FontRenderer<GPUAPI, Rasterizer>::CommitDrawCommands() -> V
	{
		return V();
	}
}