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

#include "types.hpp"

namespace WL
{
	struct UnicodeRange
	{
		I32 firstChar;
		I32 lastChar;
		
		constexpr auto Count() const -> I32 { return lastChar - firstChar + 1; }
	};

	inline static constexpr UnicodeRange CBasicLatin = { 0x0020, 0x007F };

	inline auto DecodeUTF8(Span<const Byte> in) -> Array<I32>;
}


namespace WL
{
	inline auto DecodeUTF8(Span<const Byte> in) -> Array<I32>
	{
		Array<I32> codepoints;
		for (auto i = 0; i < in.size(); ++i)
		{
			auto byte = in[i];

			if ((byte & 0b10000000) == 0)
			{
				codepoints.push_back(byte);
			}
			else if ((byte & 0b11100000u) == 0b11000000)
			{
				i++;
				if (i < in.size())
				{
					auto c0 = I32(in[i] & 0b00111111);
					auto c1 = I32(byte & 0b00011111) << 6;
					codepoints.push_back(c0 | c1);
				}
			}
			else if ((byte & 0b11110000u) == 0b11100000)
			{
				i += 2;
				if (i < in.size())
				{
					auto c0 = I32(in[i] & 0b00111111);
					auto c1 = I32(in[i - 1] & 0b00111111) << 6;
					auto c2 = I32(byte & 0b00001111) << 12;
					codepoints.push_back(c0 | c1 | c2);
				}
			}
			else if ((byte & 0b11111000u) == 0b11110000)
			{
				i += 3;
				if (i < in.size())
				{
					auto c0 = I32(in[i] & 0b00111111);
					auto c1 = I32(in[i - 1] & 0b00111111) << 6;
					auto c2 = I32(in[i - 2] & 0b00111111) << 12;
					auto c3 = I32(byte & 0b00000111) << 18;
					codepoints.push_back(c0 | c1 | c2 | c3);
				}
			}
		}
		return codepoints;
	}
}