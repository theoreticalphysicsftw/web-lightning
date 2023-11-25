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
		using Renderer = TRenderer;
		using PresentSurface = PresentSurface<GPUAPI>;
		using PreRenderFunction = PresentSurface::PreRenderFunction;

		auto static Init() -> B;
		auto static Loop() -> V;
		auto static Destroy() -> V;
		
		auto static MoveToLayer(Widget<Runtime>* widget, U32 newLayer) -> V;
		auto static Register(Widget<Runtime>* widget)->V;
		auto static Deregister(Widget<Runtime>* widget) -> V;

		auto static AddPreRenderingCode(const PreRenderFunction& func) -> V;

		using WidgetLayer = Set<Widget<Runtime>*>;
		using WidgetLayers = ChunkArray<WidgetLayer>;

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
		if (!(PresentSurface::Init() && Renderer::Init()))
		{
			return false;
		}

		PresentSurface::EnableTransparency();
		PresentSurface::AddRenderingCode([](){WidgetRenderingCode();});

		widgetLayers.resize(8);

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
	inline auto Runtime<TGPUAPI, TRenderer>::MoveToLayer(Widget<Runtime>* widget, U32 newLayer) -> V
	{
		widgetLayers[widget->layer].erase(widget);
		
		while (widgetLayers.size() <= newLayer)
		{
			widgetLayers.push_back(WidgetLayer());
		}

		widgetLayers[newLayer].insert(widget);
		widget->layer = newLayer;
	}


	template<typename TGPUAPI, typename TRenderer>
	inline auto Runtime<TGPUAPI, TRenderer>::Register(Widget<Runtime>* widget) -> V
	{
		widgetLayers[widget->layer].insert(widget);
	}


	template<typename TGPUAPI, typename TRenderer>
	inline auto Runtime<TGPUAPI, TRenderer>::Deregister(Widget<Runtime>* widget) -> V
	{
		widgetLayers[widget->layer].erase(widget);
	}


	template<typename TGPUAPI, typename TRenderer>
	inline auto Runtime<TGPUAPI, TRenderer>::AddPreRenderingCode(const PreRenderFunction& func) -> V
	{
		PresentSurface::AddPreRenderingCode(func);
	}


	template<typename TGPUAPI, typename TRenderer>
	inline auto Runtime<TGPUAPI, TRenderer>::WidgetRenderingCode() -> V
	{
		TGPUAPI::ClearPresentSurface();

		for (auto& layer : widgetLayers)
		{
			for (auto widget : layer)
			{
				if (widget->isVisible)
				{
					widget->AccumulateDrawCommands();
				}
			}
			TRenderer::CommitDrawCommands();
			TRenderer::Clear();
		}
	}
}