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

#include "logging.hpp"
#include "utilities.hpp"

#ifdef WL_DEBUG
#define WL_ASSERT(ARG) \
	{ \
		Str WL_ASSERT_line = ToString(__LINE__); \
		Str WL_ASSERT_file = __FILE__; \
		Str WL_ASSERT_arg = #ARG; \
		if (!(ARG)) \
		{ \
			LogError(WL_ASSERT_file, ":", WL_ASSERT_line, "->", WL_ASSERT_arg); \
			Terminate(); \
		} \
    }
#else
#define WL_ASSERT(ARG) ARG
#endif

namespace WL
{
	static constexpr U32 CInvalidIdx = -1;
}