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

#include "widgets/widget.hpp"
#include "main_surface/main_surface.hpp"

namespace WL
{
	template <typename TGPUAPI, typename TRenderer>
	class Runtime
	{
	public:
		using GPUAPI = TGPUAPI;
		auto static Init() -> B;
		auto static Loop() -> V;
		auto static Destroy() -> V;

		using MainSurface = MainSurface<GPUAPI>;
		using WidgetLayers = ChunkArray<ChunkArray<Widget*>>;

	private:
		auto static AddWidgetRenderingCode() -> V;
		inline static WidgetLayers widgetLayers;
	};
}


namespace WL
{
	template<typename TGPUAPI, typename TRenderer>
	inline auto Runtime<TGPUAPI, TRenderer>::Init() -> B
	{
		if (!MainSurface::Init())
		{
			return false;
		}

		AddWidgetRenderingCode();

		return true;
	}


	template<typename TGPUAPI, typename TRenderer>
	inline auto Runtime<TGPUAPI, TRenderer>::Loop() -> V
	{
		MainSurface::PresentLoop();
	}


	template<typename TGPUAPI, typename TRenderer>
	inline auto Runtime<TGPUAPI, TRenderer>::Destroy() -> V
	{
		return MainSurface::Destroy();
	}


	template<typename TGPUAPI, typename TRenderer>
	inline auto Runtime<TGPUAPI, TRenderer>::AddWidgetRenderingCode() -> V
	{
		for (auto layer = 0; layer < widgetLayers.size(); ++layer)
		{
			TRenderer::CommitDrawCommands();
		}
	}
}