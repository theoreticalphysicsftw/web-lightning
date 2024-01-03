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

#include <algebra/algebra.hpp>

#include "pso.hpp"
#include "webgl_shader.hpp"
#include "webgl_buffer.hpp"
#include "webgl_image.hpp"

namespace WL
{
    inline static constexpr U32 CMaxVBSlots = 8;
    inline static constexpr U32 CMaxUBSlots = 8;
    inline static constexpr U32 CMaxTextureSlots = 8;
    struct WebGLPso : Pso<WebGLPso>
    {
        WebGLPso();
        ~WebGLPso();

        auto Compile() -> B;
        auto Use() -> V;
        auto AddVBLayout(const VBLayout& layout) -> V;
        auto AddSimpleConstant(U32 slot, EType type, const C* = "") -> V;
        auto AddTexture(U32 slot, const C* = "") -> V;
        auto UpdateConstant(U32 slot, F32 v) -> V;
        auto UpdateConstant(U32 slot, Vec2 v) -> V;
        auto UpdateConstant(U32 slot, Vec3 v) -> V;
        auto UpdateConstant(U32 slot, Vec4 v) -> V;
        auto BindVB(U32 slot, const WebGLBuffer& buffer, B perInstance = false) -> V;
        auto BindIB(const WebGLBuffer& buffer) -> V;
        auto BindTexture(const WebGLImage& tex, U32 slot = 0) -> V;
        auto AddShader(const C* source, U32 size, EShaderType type) -> V;
        auto DrawInstanced(U32 first, U32 count, U32 instances) -> V;

        private:
        auto GetUBLocation(U32 slot) -> V;

        GLuint program;
        GLuint vao;
        StaticArray<WebGLShader, (U32) EShaderType::Count> shaders;
        StaticArray<VBLayout, CMaxVBSlots> vbLayouts;

        StaticArray<Str, CMaxTextureSlots> texNames;
        StaticArray<I32, CMaxTextureSlots> texSlots;
        StaticArray<Str, CMaxUBSlots> ubNames;
        StaticArray<I32, CMaxUBSlots> ubSlots;
    };

}
