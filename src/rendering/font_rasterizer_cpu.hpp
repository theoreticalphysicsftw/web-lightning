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
#include <common/memory.hpp>

#include <embedded/embedded_font.hpp>

#include <gpu_api/image.hpp>

#include "font_rasterizer.hpp"
#include "rasterized_font.hpp"


namespace WL
{
	template <typename TGPUAPI>
	class FontRasterizerCPU : FontRasterizer<FontRasterizerCPU<TGPUAPI>, TGPUAPI>
	{
	public:
		using GPUAPI = TGPUAPI;
		using Image = typename RasterizedFont<GPUAPI>::Image;

		static constexpr U32 CDefaultAtlasSize = 2048;
		static constexpr U32 CDefaultAA = 2;

		inline static auto Init() -> B;
		inline static auto AddFont(const Byte* fontData, U32 fontSize) -> U32;
		inline static auto BuildAtlas(U32 fontIndex, U32 fontHeight, const Array<UnicodeRange>& ranges) -> V;
		inline static auto BuildAtlases(U32 fontHeight, const Array<UnicodeRange>& ranges) -> V;

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
	inline auto FontRasterizerCPU<TGPUAPI>::BuildAtlas(U32 fontIndex, U32 fontHeight, const Array<UnicodeRange>& ranges) -> V
	{
		stbtt_pack_context spc;
		Byte* bitmapData = Allocate(CDefaultAtlasSize * CDefaultAtlasSize);
		stbtt_PackBegin(&spc, bitmapData, CDefaultAtlasSize, CDefaultAtlasSize, 0, 1, nullptr);
		stbtt_PackSetOversampling(&spc, CDefaultAA, CDefaultAA);

		Array<stbtt_packedchar> stbRangesCoords;
		Array<stbtt_pack_range> stbRanges;
		U32 totalGlyphs = 0;

		stbRanges.reserve(ranges.size());
		for (auto& range : ranges)
		{
			stbtt_pack_range r;
			r.font_size = fontHeight;
			r.first_unicode_codepoint_in_range = range.firstChar;
			r.array_of_unicode_codepoints = nullptr;
			r.num_chars = range.Count();
			totalGlyphs += r.num_chars;
			stbRanges.push_back(r);
		}

		stbRangesCoords.resize(totalGlyphs);
		auto rangesCoordsPtr = stbRangesCoords.data();
		for (auto& stbRange : stbRanges)
		{
			stbRange.chardata_for_range = rangesCoordsPtr;
			rangesCoordsPtr += stbRange.num_chars;
		}

		stbtt_PackFontRanges(&spc, fontInfos[fontIndex].data, 0, stbRanges.data(), stbRanges.size());
		stbtt_PackEnd(&spc);

		for (auto& r : stbRanges)
		{
			for (auto i = 0; i < r.num_chars; ++i)
			{
				auto cp = r.first_unicode_codepoint_in_range + i;
				auto scale = 1.f / CDefaultAtlasSize;
				AtlasGlyphDesc glyphDesc =
				{
					.u0 = r.chardata_for_range[i].x0 * scale,
					.v0 = r.chardata_for_range[i].y0 * scale,
					.u1 = r.chardata_for_range[i].x1 * scale,
					.v1 = r.chardata_for_range[i].y1 * scale,
					.xOffset0 = r.chardata_for_range[i].xoff,
					.yOffset0 = r.chardata_for_range[i].yoff,
					.xOffset1 = r.chardata_for_range[i].xoff2,
					.yOffset1 = r.chardata_for_range[i].yoff2,
					.xAdvance = r.chardata_for_range[i].xadvance
				};
				fonts[fontIndex].codePointAndHeightToGlyph.emplace(U64(cp) | U64(fontHeight) << 32, glyphDesc);
			}
		}

		Image* atlas = new Image;
		atlas->Allocate(EFormat::A8, CDefaultAtlasSize, CDefaultAtlasSize, 1);
		atlas->InitData(bitmapData);

		Deallocate(bitmapData);
	}

	template<typename TGPUAPI>
	inline auto FontRasterizerCPU<TGPUAPI>::BuildAtlases(U32 fontHeight, const Array<UnicodeRange>& ranges) -> V
	{
		for (auto i = 0u; i < fonts.size(); ++i)
		{
			BuildAtlas(i, fontHeight, ranges);
		}
	}
}
