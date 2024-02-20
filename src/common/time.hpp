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

#if __EMSCRIPTEN__
	#include <emscripten/emscripten.h>
#else
	#include <chrono>
#endif

namespace WL
{
	inline auto GetTimeStampUS() -> F64;

	template <typename TF>
	constexpr inline auto UsToS(TF us) -> TF;
	template <typename TF>
	constexpr inline auto SToUs(TF s) -> TF;
}


namespace WL
{
	inline auto GetTimeStampUS() -> F64
	{
	#if __EMSCRIPTEN__
		return F64(emscripten_get_now() * 1000.0);
	#else
		static auto staticTimePoint = std::chrono::steady_clock::now();
		auto timePoint = std::chrono::steady_clock::now();
		return F64(std::chrono::duration_cast<std::chrono::nanoseconds>(timePoint - staticTimePoint).count() / 1000.f);
	#endif
	}

	template <typename TF>
	constexpr inline auto UsToS(TF us) -> TF
	{
		return us / TF(1000000);
	}

	template<typename TF>
	constexpr inline auto SToUs(TF s) -> TF
	{
		return s * TF(1000000);
	}
}