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
#include <rendering/color.hpp>

#include <SDL2/SDL.h>
#include "webgl_api.hpp"

#include "gpu_api.hpp"
#include "webgl_shader.hpp"
#include "webgl_pso.hpp"
#include "webgl_buffer.hpp"
#include "webgl_image.hpp"

namespace WL
{
    struct WebGL : GpuApi<WebGL>
    {
        using Shader = WebGLShader;
        using Pso = WebGLPso;
        using Buffer = WebGLBuffer;
        using Image = WebGLImage;

        static auto Init(SDL_Window* w) -> B;
        static auto GetWindowFlags() -> U32;
        static auto SetPresentSurfaceClearColor(const Color4& color) -> V;
        static auto ClearPresentSurface() -> V;
        static auto EnablePresentSurfaceTransparency() -> V;
        static auto EnableSampleCoverage() -> V;
        static auto Present() -> V;
        static auto UpdateViewport(U32 width, U32 height, U32 x = 0, U32 y = 0) -> V;
    };
}
