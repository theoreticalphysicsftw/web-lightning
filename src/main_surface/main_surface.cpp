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


#include "main_surface.hpp"


namespace WL::MainSurface
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    B isWindowClosed = false;


#ifdef __EMSCRIPTEN__
    EM_JS(U32, GetCanvasWidth, (), { return canvas.width; });
    EM_JS(U32, GetCanvasHeight, (), { return canvas.height; });
#else
    U32 GetCanvasWidth() { return 1920; }
    U32 GetCanvasHeight() { return 1080; }
#endif


    B Init(const C* appName)
    {

        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            return false;
        }

        window = SDL_CreateWindow(
                                    appName,
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    GetCanvasWidth(),
                                    GetCanvasHeight(),
                                    SDL_WINDOW_SHOWN |
                                    SDL_WINDOW_RESIZABLE
                                 );

        if (window == nullptr)
        {
            SDL_Quit();
            return false;
        }

        renderer = SDL_CreateRenderer(
                                        window,
                                        -1,
                                        SDL_RENDERER_ACCELERATED |
                                        SDL_RENDERER_PRESENTVSYNC
                                     );
        
        if(!SDL_RenderSetLogicalSize(renderer, GetCanvasHeight(), GetCanvasWidth()))
        
        if (!renderer)
        {
            SDL_Quit();
            return false;
        }

        return true;
    }


    V Destroy()
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }


    V ProcessInput()
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
            }

            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                }
            }
        }
    }


    V Render()
    {
        SDL_SetRenderDrawColor(renderer, 64, 8, 128, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }


    V PresentLoopIteration()
    {
        Render();
        ProcessInput();
    }


    V PresentLoop()
    {
    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(PresentLoopIteration, 0, true);
    #else
        while (isWindowClosed)
        {
            PresentLoopIteration();
        }
    #endif
    }
}

