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

#include <widgets/widget.hpp>
#include <presenting/present_surface.hpp>

#include "config.hpp"

namespace WL
{
	template <typename TGPUAPI, typename TRenderer>
	class Runtime
	{
	public:
		using GPUAPI = TGPUAPI;
		using Renderer = TRenderer;
		using GPUPresentSurface = PresentSurface<GPUAPI>;
		using PresentSurface = PresentSurface<GPUAPI>;
		using PreRenderFunction = GPUPresentSurface::PreRenderFunction;

		auto static Init(const Config& cfg = Config()) -> B;
		auto static Loop() -> V;
		auto static Destroy() -> V;
		auto static Exit() -> V;
		
		auto static MoveToLayer(Widget<Runtime>* widget, U32 newLayer) -> V;
		auto static Register(Widget<Runtime>* widget)->V;
		auto static Deregister(Widget<Runtime>* widget) -> V;

		auto static AddPreRenderingCode(const PreRenderFunction& func) -> V;

		using WidgetLayer = Set<Widget<Runtime>*>;
		using WidgetLayers = ChunkArray<WidgetLayer>;

	private:
		auto static RenderWidgets() -> V;
		auto static UpdateWidgets(const UpdateState& updateState) -> V;
		auto static ResizeLayers(U32 newSize) -> V;
		inline static B widgetsAreDirty = true;
		inline static WidgetLayers widgetLayers;
	};
}


namespace WL
{
	template<typename TGPUAPI, typename TRenderer>
	inline auto Runtime<TGPUAPI, TRenderer>::Init(const Config& cfg) -> B
	{
		if (!GPUPresentSurface::Init(cfg.appName, cfg.width, cfg.height, cfg.resizableWindow, cfg.borderlessWindow))
		{
			return false;
		}
		
		if (!Renderer::FontRenderer::Rasterizer::Init())
		{
			return false;
		}

		if (cfg.useFontAtlases)
		{
			Renderer::FontRenderer::Rasterizer::BuildAtlases(cfg.defaultFontHeight, cfg.defaultFontRanges);
		}

		if (!Renderer::Init())
		{
			return false;
		}

		GPUPresentSurface::EnableTransparency();
		GPUPresentSurface::AddRenderingCode([](){RenderWidgets();});
		GPUPresentSurface::AddUpdateCode([](const UpdateState& us){UpdateWidgets(us);});

		ResizeLayers(8);

		return true;
	}


	template<typename TGPUAPI, typename TRenderer>
	inline auto Runtime<TGPUAPI, TRenderer>::Loop() -> V
	{
		GPUPresentSurface::PresentLoop();
	}


	template<typename TGPUAPI, typename TRenderer>
	inline auto Runtime<TGPUAPI, TRenderer>::Destroy() -> V
	{
		return GPUPresentSurface::Destroy();
	}

	template<typename TGPUAPI, typename TRenderer>
	inline auto Runtime<TGPUAPI, TRenderer>::Exit() -> V
	{
		GPUPresentSurface::Close();
	}

	template<typename TGPUAPI, typename TRenderer>
	inline auto Runtime<TGPUAPI, TRenderer>::MoveToLayer(Widget<Runtime>* widget, U32 newLayer) -> V
	{
		widgetLayers[widget->layer].erase(widget);
		
		ResizeLayers(newLayer + 1);

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
		if (widgetLayers.size() > widget->layer)
		{
			widgetLayers[widget->layer].erase(widget);
		}
	}


	template<typename TGPUAPI, typename TRenderer>
	inline auto Runtime<TGPUAPI, TRenderer>::AddPreRenderingCode(const PreRenderFunction& func) -> V
	{
		GPUPresentSurface::AddPreRenderingCode(func);
	}


	template<typename TGPUAPI, typename TRenderer>
	inline auto Runtime<TGPUAPI, TRenderer>::RenderWidgets() -> V
	{
		TGPUAPI::ClearPresentSurface();

		for (auto& layer : widgetLayers)
		{
			for (auto widgetPtr : layer)
			{
				if (widgetPtr->isVisible)
				{
					widgetPtr->AccumulateDrawState();
				}
			}
			TRenderer::CommitDrawCommands();
			TRenderer::Clear();
		}
	}

	template<typename TGPUAPI, typename TRenderer>
	inline auto Runtime<TGPUAPI, TRenderer>::UpdateWidgets(const UpdateState& updateState) -> V
	{
		auto mousePos = Vec2(updateState.mouseX, updateState.mouseY);

		for (auto& layer : widgetLayers)
		{
			for (auto widgetPtr : layer)
			{
				auto bBox = widgetPtr->GetBBox();
				if (bBox.Contains(mousePos))
				{
					if (updateState.leftPressed)
					{
						if (widgetPtr->updateOnClick)
						{
							widgetPtr->updateOnClick(updateState);
						}
					}
				}
				widgetPtr->Update(updateState);
			}
		}
	}

	template<typename TGPUAPI, typename TRenderer>
	inline auto Runtime<TGPUAPI, TRenderer>::ResizeLayers(U32 newSize) -> V
	{
		if (newSize > widgetLayers.size())
		{
			widgetLayers.resize(newSize);
		}
	}
}