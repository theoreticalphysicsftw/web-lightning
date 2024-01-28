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


#pragma once

#include <common/types.hpp>
#include <embedded/embedded_font.hpp>

#include "font_rasterizer.hpp"


namespace WL
{
	template <typename TGPUAPI>
	class FontRasterizerGPU : FontRasterizer<FontRasterizerGPU<TGPUAPI>, TGPUAPI>
	{
	public:
		using GPUAPI = TGPUAPI;

		inline static auto Init() -> B;
		inline static auto AddFont(const Byte* fontData, U32 fontSize) -> U32;

		inline static Map<U64, typename GPUAPI::Image*> fontMaps;
	};
}


namespace WL
{
	template<typename TGPUAPI>
	inline auto FontRasterizerGPU<TGPUAPI>::Init() -> B
	{
	#ifdef WL_USE_EMBEDDED_FONT
	#endif
        return true;
	}


	template<typename TGPUAPI>
	inline auto FontRasterizerGPU<TGPUAPI>::AddFont(const Byte* fontData, U32 fontSize) -> U32
	{
		return ~0u;
	}
}
