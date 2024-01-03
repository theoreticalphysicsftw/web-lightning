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

#include "image.hpp"
#include "webgl_api.hpp"

namespace WL
{
	extern GLint gFormatToGLInternalFormat[];
	extern GLenum gFormatToGLFormat[];
	extern GLenum gFormatToGLType[];

	class WebGLImage : Image<WebGLImage>
	{
	public:
		WebGLImage(const RawCPUImage* cpuImage = nullptr);
		auto Allocate(EFormat format, U32 w, U32 h = 1, U32 d = 1) -> B;
		auto InitData(V* inData) -> V;
		auto UpdateData(const Extent& extend, void* inData) -> V;

		auto GetNativeId() const -> GLuint { return id; }

		EFormat format = EFormat::Invalid;
		U32 height = 0;
		U32 width = 0;
		U32 depth = 0;
		
	private:
		GLuint id;
	};
}