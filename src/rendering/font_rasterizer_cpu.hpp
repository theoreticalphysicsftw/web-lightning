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
#include <common/unicode.hpp>
#include <embedded/embedded_font.hpp>

#include "font_rasterizer.hpp"
#include "rasterized_font.hpp"


namespace WL
{
	template <typename TGPUAPI>
	class FontRasterizerCPU : FontRasterizer<FontRasterizerCPU<TGPUAPI>, TGPUAPI>
	{
	public:
		using GPUAPI = TGPUAPI;
		using Image = typename GPUAPI::Image;

		static constexpr U32 CDefaultAtlasSize = 2048;
		static constexpr U32 CDefaultAA = 2;

		inline static auto Init() -> B;
		inline static auto AddFont(const Byte* fontData, U32 fontSize) -> U32;
		inline static auto BuildAtlas(U32 fontIndex, U32 fontHeight, const Array<UnicodeRange>& ranges) -> const Image&;

	private:
		static constexpr U32 CInitialFontCapacity = 128;

		inline static Array<RasterizedFont<GPUAPI>> fonts;
		inline static Array<stbtt_fontinfo> fontInfos;

	};
}


namespace WL
{
	template<typename TGPUAPI>
	inline auto FontRasterizerCPU<TGPUAPI>::Init() -> B
	{
		fonts.reserve(CInitialFontCapacity);

	#ifdef WL_USE_EMBEDDED_FONT
		stbtt_fontinfo info;
		
		if (stbtt_InitFont(&info, OpenSans, 0))
		{
			fontInfos.push_back(info);
			fonts.emplace_back();
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


	template<typename TGPUAPI>
	inline auto FontRasterizerCPU<TGPUAPI>::BuildAtlas(U32 fontIndex, U32 fontHeight, const Array<UnicodeRange>& ranges) -> const Image&
	{
		Image* img = nullptr;

		stbtt_pack_context spc;
		Byte* bitmapData = Allocate(CDefaultAtlasSize * CDefaultAtlasSize);
		stbtt_PackBegin(&spc, bitmapData, CDefaultAtlasSize, CDefaultAtlasSize, 0, 1, nullptr);
		stbtt_PackSetOversampling(&spc, CDefaultAA, CDefaultAA);

		for (auto& range : ranges)
		{

		}

		stbtt_PackEnd(&spc);
		return *img;
	}
}
