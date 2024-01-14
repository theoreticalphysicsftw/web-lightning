#include "webgl.hpp"
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


#include <common/types.hpp>
#include <algebra/algebra.hpp>


#include <SDL2/SDL.h>

#include "webgl_api.hpp"
#include "webgl.hpp"


namespace WL
{
    SDL_GLContext context = nullptr;
    SDL_Window* window = nullptr;

    U32 WebGL::GetWindowFlags()
    {
        return SDL_WINDOW_OPENGL;
    }

    B WebGL::Init(SDL_Window* w)
    {
        window = w;

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

        context = SDL_GL_CreateContext(w);
        InitWebGLApi();

        return context != nullptr;
    }

    V WebGL::SetPresentSurfaceClearColor(const Color4& color)
    {
        glClearColor(color[0], color[1], color[2], color[3]);
    }

    V WebGL::ClearPresentSurface()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    auto WebGL::EnablePresentSurfaceTransparency() -> V
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    auto WebGL::EnableSampleCoverage() -> V
    {
        glEnable(GL_SAMPLE_COVERAGE);
    }

    V WebGL::Present()
    {
        SDL_GL_SwapWindow(window);
    }

    auto WebGL::UpdateViewport(U32 width, U32 height, U32 x, U32 y) -> V
    {
        glViewport(x, y, width, height);
    }
}
