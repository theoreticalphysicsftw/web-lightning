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
#include <common/unicode.hpp>


namespace WL
{
	struct AtlasGlyphDesc
	{
		// Texture coordinates in atlas
		F32 u0;
		F32 v0;
		F32 u1;
		F32 v1;
		// Screenspace quad position
		F32 xOffset0;
		F32 yOffset0;
		F32 xOffset1;
		F32 yOffset1;
		F32 xAdvance;
	};

	template <typename TGPUAPI>
	class RasterizedFont
	{
	public:
		using GPUAPI = TGPUAPI;
		using Image = GPUAPI::Image;

		Map<U64, AtlasGlyphDesc> codePointAndHeightToGlyph;
		Map<U16, Image*> heightToAtlas;
	};
}