#include "webgl_buffer.hpp"
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


#include "webgl_buffer.hpp"


namespace WL
{
	WebGLBuffer::WebGLBuffer(U32 size) : Buffer(size), id(CInvalidId)
	{
	}

	WebGLBuffer::~WebGLBuffer()
	{
		if (this->id != CInvalidId)
		{
			glDeleteBuffers(1, &id);
		}
	}

	auto WebGLBuffer::Allocate(U32 size, B onGPU, EUsage usage) -> V
	{
		this->size = size;
		this->onGPU = onGPU;
		this->usage = usage;
		glGenBuffers(1, &id);
	}

	auto WebGLBuffer::Update(Byte* data, U32 size) -> V
	{
		glBindBuffer(GL_COPY_WRITE_BUFFER, id);
		glBufferData(GL_COPY_WRITE_BUFFER, size, data, onGPU? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	}

}