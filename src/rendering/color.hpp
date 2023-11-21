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
	using Color2 = Vec2;
	using Color3 = Vec3;
	using Color4 = Vec4;

	union ColorU32
	{
		U32 packed;
		struct
		{
			U8 r;
			U8 g;
			U8 b;
			U8 a;
		};

		ColorU32(U32 raw = 0);
		ColorU32(U8 r, U8 g, U8 b, U8 a);
		ColorU32(const Color4 c4);

		operator Color4() const;
	};
}

namespace WL
{
	inline ColorU32::ColorU32(U32 raw)
		: packed(raw)
	{
	}

	inline ColorU32::ColorU32(U8 r, U8 g, U8 b, U8 a)
		: r(r), g(g), b(b), a(a)
	{
	}

	inline ColorU32::ColorU32(const Color4 c4)
	{
		r = U8(c4[0] * 255);
		g = U8(c4[1] * 255);
		b = U8(c4[2] * 255);
		a = U8(c4[3] * 255);
	}

	inline ColorU32::operator Color4() const
	{
		return Color4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
	}
}