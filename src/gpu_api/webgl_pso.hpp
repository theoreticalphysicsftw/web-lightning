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

#include "pso.hpp"
#include "webgl_shader.hpp"
#include "webgl_buffer.hpp"

namespace WL
{
    inline static constexpr U32 CMaxVBSlots = 8;

    struct WebGLPso : Pso<WebGLPso>
    {
        WebGLPso();
        ~WebGLPso();

        auto Compile() -> B;
        auto Use() -> V;
        auto AddVBLayout(const VBLayout& layout) -> V;
        auto BindVB(U32 slot, const WebGLBuffer& buffer, B perInstance = false) -> V;
        auto BindIB(const WebGLBuffer& buffer) -> V;
        auto AddShader(const C* source, U32 size, EShaderType type) -> V;
        auto DrawInstanced(U32 first, U32 count, U32 instances) -> V;

        private:
        GLuint program;
        GLuint vao;
        StaticArray<WebGLShader, (U32) EShaderType::Count> shaders;
        StaticArray<VBLayout, CMaxVBSlots> vbLayouts;
    };

}
