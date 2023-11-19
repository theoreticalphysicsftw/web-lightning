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
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#if __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif


namespace WL
{
    template <class TGPUAPI>
    struct PresentSurface
    {
        static auto Init(const C* appName = "web-lightning") -> B;
        static auto Destroy() -> V;
        static auto PresentLoop() -> V;

        using RenderFunction = Function<V()>;

        static auto AddRenderingCode(const RenderFunction& func) -> V;
        static auto UnitsToPixels(F32 units) -> U32;
        static auto PixelsToUnits(U32 pixels) -> F32;
        static auto GetAspectRatio() -> F32;
        static auto SetClearColor(const Color4& color) -> V;
        static auto EnableTransparency() -> V;

        private:
        static V PresentLoopIteration();
        static V Render();
        static V ProcessInput();

        inline static SDL_Window* window = nullptr;
        inline static RenderFunction renderFunction = []() {};

        inline static U32 width = 0;
        inline static U32 height = 0;
        inline static B isWindowClosed = false;

        inline static Color4 clearColor = Color4(1.0f, 1.0f, 1.0f, 1.0f);
    };
}


namespace WL
{
#ifdef __EMSCRIPTEN__
    EM_JS(U32, GetCanvasWidth, (), { return canvas.width; });
    EM_JS(U32, GetCanvasHeight, (), { return canvas.height; });
#else
    U32 GetCanvasWidth() { return 960; }
    U32 GetCanvasHeight() { return 540; }
#endif


    template <class TGPUAPI>
    auto PresentSurface<TGPUAPI>::Init(const C* appName) -> B
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            isWindowClosed = true;
            return false;
        }

        width = GetCanvasWidth();
        height = GetCanvasHeight();

        window = SDL_CreateWindow(
            appName,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            SDL_WINDOW_SHOWN |
            SDL_WINDOW_RESIZABLE |
            TGPUAPI::GetWindowFlags()
        );

        if (window == nullptr)
        {
            isWindowClosed = true;
            return false;
        }

        if (!TGPUAPI::Init(window))
        {
            isWindowClosed = true;
            return false;
        }

        TGPUAPI::SetPresentSurfaceClearColor(clearColor);

        return true;
    }


    template <class TGPUAPI>
    auto PresentSurface<TGPUAPI>::Destroy() -> V
    {
        if (window != nullptr)
        {
            SDL_DestroyWindow(window);
            SDL_Quit();
        }
        isWindowClosed = true;
    }


    template <class TGPUAPI>
    auto PresentSurface<TGPUAPI>::ProcessInput() -> V
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {

            if (event.window.event == SDL_WINDOWEVENT_CLOSE)
            {
                isWindowClosed = true;
            }

            if (event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                width = event.window.data1;
                height = event.window.data2;
            }

            if (event.type == SDL_KEYDOWN)
            {
                //switch (event.key.keysym.sym)
            }
        }
    }


    template <class TGPUAPI>
    auto PresentSurface<TGPUAPI>::Render() -> V
    {
        renderFunction();
    }


    template <class TGPUAPI>
    auto PresentSurface<TGPUAPI>::PresentLoopIteration() -> V
    {
        Render();
        TGPUAPI::Present();
        ProcessInput();
    }


    template <class TGPUAPI>
    auto PresentSurface<TGPUAPI>::PresentLoop() -> V
    {
#ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(PresentLoopIteration, 0, true);
#else
        while (!isWindowClosed)
        {
            PresentLoopIteration();
        }
#endif
    }


    template <class TGPUAPI>
    auto PresentSurface<TGPUAPI>::AddRenderingCode(const RenderFunction& func) -> V
    {
        renderFunction = func;
    }


    template<class TGPUAPI>
    inline auto WL::PresentSurface<TGPUAPI>::UnitsToPixels(F32 units) -> U32
    {
        return U32(units * width);
    }


    template<class TGPUAPI>
    inline auto WL::PresentSurface<TGPUAPI>::PixelsToUnits(U32 pixels) -> F32
    {
        return F32(pixels) / width;
    }


    template<class TGPUAPI>
    inline auto WL::PresentSurface<TGPUAPI>::GetAspectRatio() -> F32
    {
        return F32(width) / height;
    }


    template<class TGPUAPI>
    inline auto WL::PresentSurface<TGPUAPI>::SetClearColor(const Color4& color) -> V
    {
        clearColor = color;
        TGPUAPI::SetPresentSurfaceClearColor(color);
    }


    template<class TGPUAPI>
    inline auto WL::PresentSurface<TGPUAPI>::EnableTransparency() -> V
    {
        TGPUAPI::EnablePresentSurfaceTransparency();
    }
}