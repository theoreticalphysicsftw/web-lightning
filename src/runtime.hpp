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
#include "presenting/present_surface.hpp"

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

		using PresentSurface = PresentSurface<GPUAPI>;
		using WidgetLayers = ChunkArray<ChunkArray<Widget<Runtime>*>>;

	private:
		auto static WidgetRenderingCode() -> V;
		inline static B widgetsAreDirty = true;
		inline static WidgetLayers widgetLayers;
	};
}


namespace WL
{
	template<typename TGPUAPI, typename TRenderer>
	inline auto Runtime<TGPUAPI, TRenderer>::Init() -> B
	{
		if (!PresentSurface::Init())
		{
			return false;
		}

		PresentSurface::EnableTransparency();
		PresentSurface::AddRenderingCode([](){WidgetRenderingCode();});

		return true;
	}


	template<typename TGPUAPI, typename TRenderer>
	inline auto Runtime<TGPUAPI, TRenderer>::Loop() -> V
	{
		PresentSurface::PresentLoop();
	}


	template<typename TGPUAPI, typename TRenderer>
	inline auto Runtime<TGPUAPI, TRenderer>::Destroy() -> V
	{
		return PresentSurface::Destroy();
	}


	template<typename TGPUAPI, typename TRenderer>
	inline auto Runtime<TGPUAPI, TRenderer>::WidgetRenderingCode() -> V
	{
		TGPUAPI::ClearPresentSurface();

		for (auto layer = 0u; layer < widgetLayers.size(); ++layer)
		{
			for (auto wIdx = 0u; wIdx < widgetLayers[layer].size(); ++wIdx)
			{
				widgetLayers[layer][wIdx]->AccumulateDrawCommands();
			}
			TRenderer::CommitDrawCommands();
		}
	}
}