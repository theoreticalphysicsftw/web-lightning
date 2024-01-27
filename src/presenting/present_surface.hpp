// MIT License
// 
// Copyright (c) 2023 - 2024 Mihail Mladenov
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
#include <common/time.hpp>
#include <algebra/algebra.hpp>


#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "events.hpp"

#if __EMSCRIPTEN__
    #include <emscripten/emscripten.h>
#elif _WIN32
    #include "present_surface_win32.hpp"
#elif __linux__
    #include "present_surface_x11.hpp"
#endif


namespace WL
{
    struct UpdateState
    {
        F32 dt;
        F32 mouseX;
        F32 mouseY;
        U32 keyCode;
    };

    template <class TGPUAPI>
    struct PresentSurface
    {
        using GPUAPI = TGPUAPI;

        static auto Init(const Str& appName, U32 w = 0, U32 h = 0, B resizableWindow = true, B borderlessWindow = false) -> B;
        static auto Destroy() -> V;
        static auto PresentLoop() -> V;

        using RenderFunction = Function<V()>;
        using UpdateFunction = Function<V(const UpdateState&)>;
        using PreRenderFunction = Function<V(F32 dt)>;

        static auto AddPreRenderingCode(const PreRenderFunction& func) -> V;
        static auto AddUpdateCode(const UpdateFunction& func) -> V;
        static auto AddRenderingCode(const RenderFunction& func) -> V;
        static auto UnitsToPixels(F32 units) -> U32;
        static auto PixelsToUnits(U32 pixels) -> F32;
        static auto GetAspectRatio() -> F32;
        static auto SetClearColor(const Color4& color) -> V;
        static auto EnableTransparency() -> V;
        static auto GetDimensions() -> Vec2;

        private:
        static auto PresentLoopIteration() -> V;
        static auto ProcessInput() -> UpdateState;

        inline static SDL_Window* window = nullptr;
        inline static RenderFunction renderFunction = [](){};
        inline static UpdateFunction updateFunction = [](const UpdateState& updateState) {};
        inline static PreRenderFunction preRenderFunction = [](F32 dt){};

        inline static F64 timeStamp = 0;

        inline static U32 width = 0;
        inline static U32 height = 0;
        inline static U32 samples = 8;
        inline static B isWindowClosed = false;
        inline static Color4 clearColor = Color4(0.0f, 0.0f, 0.0f, 0.0f);

        inline static GPUAPI::RenderTarget presentTarget;
        inline static GPUAPI::RenderTarget offscreenTargetMS;
    };
}


namespace WL
{
#ifdef __EMSCRIPTEN__
    EM_JS(U32, GetCanvasWidth, (), { return canvas.width; });
    EM_JS(U32, GetCanvasHeight, (), { return canvas.height; });
#else
    inline U32 GetCanvasWidth() { return 960; }
    inline U32 GetCanvasHeight() { return 540; }
#endif


    template <class TGPUAPI>
    auto PresentSurface<TGPUAPI>::Init(const Str& appName, U32 w, U32 h, B resizableWindow, B borderlessWindow) -> B
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            isWindowClosed = true;
            return false;
        }

        width = w? w : GetCanvasWidth();
        height = h? h : GetCanvasHeight();

        U32 extraFlags = 0;
        extraFlags |= resizableWindow ? SDL_WINDOW_RESIZABLE : 0;
        extraFlags |= borderlessWindow ? SDL_WINDOW_BORDERLESS : 0;

        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

        window = SDL_CreateWindow(
            appName.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            SDL_WINDOW_SHOWN |
            extraFlags |
            TGPUAPI::GetWindowFlags()
        );

        if (window == nullptr)
        {
            isWindowClosed = true;
            return false;
        }

        if (borderlessWindow)
        {
            EnableWindowTransparency(window);
        }

        if (!TGPUAPI::Init(window))
        {
            isWindowClosed = true;
            return false;
        }

        TGPUAPI::SetPresentSurfaceClearColor(clearColor);
        auto presentBufferID = TGPUAPI::GetAttachedFrameBufferID();
        presentTarget.Wrap(presentBufferID);
        #ifndef __EMSCRIPTEN__
        offscreenTargetMS.Init(width, height, samples);
        #endif
        TGPUAPI::EnableSampleCoverage();
        timeStamp = GetTimeStampUS();
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
    auto PresentSurface<TGPUAPI>::ProcessInput() -> UpdateState
    {
        UpdateState us;
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
                #ifndef __EMSCRIPTEN__
                offscreenTargetMS.Recreate(width, height, samples);
                offscreenTargetMS.Bind();
                #endif
                TGPUAPI::UpdateViewport(width, height);
            }

            if (event.type == SDL_KEYDOWN)
            {
                //switch (event.key.keysym.sym)
            }
        }

        auto newTimeStamp = GetTimeStampUS();
        auto dt = newTimeStamp - timeStamp;
        timeStamp = newTimeStamp;
        us.dt = dt;

        return us;
    }


    template <class TGPUAPI>
    auto PresentSurface<TGPUAPI>::PresentLoopIteration() -> V
    {
        auto updateState = ProcessInput();
        updateFunction(updateState);
        preRenderFunction(updateState.dt);
        renderFunction();
        #ifndef __EMSCRIPTEN__
        offscreenTargetMS.BlitTo(presentTarget);
        presentTarget.Bind();
        #endif
        TGPUAPI::Present();
        #ifndef __EMSCRIPTEN__
        offscreenTargetMS.Bind();
        #endif
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


    template<class TGPUAPI>
    inline auto PresentSurface<TGPUAPI>::AddPreRenderingCode(const PreRenderFunction& func) -> V
    {
        preRenderFunction = func;
    }

    template<class TGPUAPI>
    inline auto PresentSurface<TGPUAPI>::AddUpdateCode(const UpdateFunction& func) -> V
    {
        updateFunction = func;
    }

    template <class TGPUAPI>
    auto PresentSurface<TGPUAPI>::AddRenderingCode(const RenderFunction& func) -> V
    {
        renderFunction = func;
    }


    template<class TGPUAPI>
    inline auto PresentSurface<TGPUAPI>::UnitsToPixels(F32 units) -> U32
    {
        return U32(units * width + 0.5);
    }


    template<class TGPUAPI>
    inline auto PresentSurface<TGPUAPI>::PixelsToUnits(U32 pixels) -> F32
    {
        return F32(pixels) / width;
    }


    template<class TGPUAPI>
    inline auto PresentSurface<TGPUAPI>::GetAspectRatio() -> F32
    {
        return F32(width) / height;
    }


    template<class TGPUAPI>
    inline auto PresentSurface<TGPUAPI>::SetClearColor(const Color4& color) -> V
    {
        clearColor = color;
        TGPUAPI::SetPresentSurfaceClearColor(color);
    }


    template<class TGPUAPI>
    inline auto PresentSurface<TGPUAPI>::EnableTransparency() -> V
    {
        TGPUAPI::EnablePresentSurfaceTransparency();
    }


    template<class TGPUAPI>
    inline auto PresentSurface<TGPUAPI>::GetDimensions() -> Vec2
    {
        return Vec2(width, height);
    }
}