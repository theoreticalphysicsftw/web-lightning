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

#define SDL_MAIN_HANDLED
#include <SDL2/SDL_syswm.h>

#include <dwmapi.h>

#include <common/error.hpp>


namespace WL
{
	inline auto EnableWindowTransparency(SDL_Window* win) -> V;
}


namespace WL
{
	inline auto EnableWindowTransparency(SDL_Window* win) -> V
	{
		SDL_SysWMinfo info;
		SDL_VERSION(&info.version);
		WL_ASSERT(SDL_GetWindowWMInfo(win, &info));

		auto nativeHandle = info.info.win.window;

		DWM_BLURBEHIND bb = {};
		bb.dwFlags = DWM_BB_ENABLE;
		bb.fEnable = true;
		bb.hRgnBlur = NULL;
		DwmEnableBlurBehindWindow(nativeHandle, &bb);

		MARGINS margins = { -1 };
		DwmExtendFrameIntoClientArea(nativeHandle, &margins);
	}
}