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

#include <common/utilities.hpp>

#include "buffer.hpp"
#include "webgl_api.hpp"

namespace WL
{
	class WebGLBuffer : Buffer<WebGLBuffer>
	{
	public:
		WebGLBuffer(U32 size = 0);
		~WebGLBuffer();
		
		auto Allocate(U32 size, B onGPU = true, EUsage usage = EUsage::Undefined) -> B;
		auto Reallocate(U32 size, B onGPU = true, EUsage usage = EUsage::Undefined) -> B;
		auto GetNativeId() const -> GLuint { return id; }

		template <typename T>
		auto Update(const Array<T>& a) -> V;
		auto Update(const Byte* data, U32 size) -> V;
	private:
		GLuint id;
	};
}


namespace WL
{
	template<typename T>
	inline auto WebGLBuffer::Update(const Array<T>& a) -> V
	{
		Update((const Byte*)a.data(), SizeInBytes(a));
	}
}