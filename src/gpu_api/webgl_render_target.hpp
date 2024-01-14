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

#include "render_target.hpp"
#include "webgl_api.hpp"

namespace WL
{
	class WebGLRenderTarget : RenderTarget<WebGLRenderTarget>
	{
	public:
		using NativeID = GLuint;
		auto Init(U32 width, U32 height, U32 samples = 1, B withDepth = true) -> V;
		auto Wrap(NativeID frameBufferID) -> V;
		auto Bind() -> V;
		auto BlitTo(const WebGLRenderTarget& other) const -> V;
		auto Recreate(U32 width, U32 height, U32 samples = 1, B withDepth = true) -> V;
		~WebGLRenderTarget();
	private:
		auto Destroy() -> V;
		NativeID frameBufferID = CInvalidID;
		NativeID colorBufferID = CInvalidID;
		NativeID depthBufferID = CInvalidID;
		B wrapped;
	};
}