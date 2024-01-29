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


#pragma once

#include <common/types.hpp>
#include <common/error.hpp>

#include <lizard/lizard_frame.h>

namespace WL
{
	inline auto DecompressLizard(Span<const Byte> data) -> Array<Byte>;
}


namespace WL
{
	auto DecompressLizard(Span<const Byte> src) -> Array<Byte>
	{
		WL_ASSERT(src.size() >= 4);

		Array<Byte> result;

		LizardF_decompressionContext_t ctx;
		if (LizardF_isError(LizardF_createDecompressionContext(&ctx, LIZARDF_VERSION)))
		{
			return result;
		}

		// We can assume that it's at least as big as the compressed one.
		result.resize(src.size());
		auto nextToProcess = src.size();
		U64 srcOffset = 0;
		U64 dstOffset = 0;
		while (nextToProcess)
		{
			U64 decodedBytes = result.size() - dstOffset;
			U64 remaining = src.size() - srcOffset;
			nextToProcess = LizardF_decompress(ctx, (V*)(result.data() + dstOffset), &decodedBytes, (V*)(src.data() + srcOffset), &remaining, nullptr);
			dstOffset += decodedBytes;
			srcOffset += remaining;

			if (LizardF_isError(nextToProcess))
			{
				result.clear();
				return result;
			}


			if (dstOffset >= result.size())
			{
				result.resize(result.size() * 2);
			}
		}

		result.resize(dstOffset);

		LizardF_freeDecompressionContext(ctx);
		return result;
	}
}