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
#include <algebra/algebra.hpp>

namespace WL
{
	// Oscillate in a range form high to low value returning to the high value in certainp period.
	template <typename T>
	auto Oscillate(F32 period, T range0, T range1, F32 currentTime) -> T;

	template <typename T>
	auto DiscretePeriodicIncrement(F32 treshold, T range0, T range1, F32 currentTime) -> T;
}


namespace WL
{
	template<typename T>
	auto Oscillate(F32 period, T range0, T range1, F32 currentTime) -> T
	{
		auto normalizedT = Frac((currentTime * Constants<F32>::C2Pi / period) / Constants<F32>::C2Pi) * Constants<F32>::C2Pi;
		auto t = ((Cos(normalizedT) + 1.f) / 2.f);
		return (range0 * t) + (range1 * (1.f - t));
	}

	template<typename T>
	auto DiscretePeriodicIncrement(F32 threshold, T range0, T range1, F32 currentTime) -> T
	{
		U32 ticks = U32(currentTime / threshold);
		auto range = range1 - range0;
		return range0 + ticks % range;
	}
}