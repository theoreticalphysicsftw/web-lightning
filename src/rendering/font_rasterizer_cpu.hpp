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

#include <stb_truetype.h>

#include <common/types.hpp>
#include <embedded/embedded_font.hpp>

#include "font_rasterizer.hpp"


namespace WL
{
	template <typename TGPUAPI>
	class FontRasterizerCPU : FontRasterizer<FontRasterizerCPU<TGPUAPI>, TGPUAPI>
	{
	public:
		using GPUAPI = TGPUAPI;

		inline static auto Init() -> B;
		inline static auto AddFont(const Byte* fontData, U32 fontSize) -> U32;

		inline static Map<U64, typename GPUAPI::Image*> fontMaps;
		inline static Array<stbtt_fontinfo> fontInfos;

	};
}


namespace WL
{
	template<typename TGPUAPI>
	inline auto FontRasterizerCPU<TGPUAPI>::Init() -> B
	{
	#ifdef WL_USE_EMBEDDED_FONT
		stbtt_fontinfo info;
		
		if (stbtt_InitFont(&info, OpenSans, 0))
		{
			fontInfos.push_back(info);
			return true;
		}

		return false;
    #else
        return true;
	#endif
	}


	template<typename TGPUAPI>
	inline auto FontRasterizerCPU<TGPUAPI>::AddFont(const Byte* fontData, U32 fontSize) -> U32
	{
		stbtt_fontinfo info;
		if (stbtt_InitFont(&info, fontData, 0))
		{
			fontInfos.push_back(info);
			return fontInfos.size() - 1;
		}

		return ~0u;
	}
}
