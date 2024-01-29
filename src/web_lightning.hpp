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

#include <rendering/font_renderer.hpp>
#include <rendering/font_rasterizer.hpp>
#include <rendering/font_rasterizer_cpu.hpp>
#include <rendering/vector_rasterizer_cpu.hpp>
#include <rendering/box_renderer.hpp>
#include <rendering/textured_quad_renderer.hpp>
#include <rendering/circular_arc_renderer.hpp>
#include <rendering/renderer.hpp>

#include <presenting/present_surface.hpp>
#include <gpu_api/gpu_api.hpp>
#include <gpu_api/webgl.hpp>

#include <runtime.hpp>

#include <widgets/box.hpp>
#include <widgets/text_box.hpp>
#include <widgets/grid.hpp>
#include <widgets/vector_element.hpp>

#include <widgets/progress_indicators/round_segmented.hpp>

#include <decoders/webp.hpp>
#include <decoders/lizard.hpp>

namespace WL
{
	using GPUAPIDefault = WebGL;
	using PresentSurfaceDefault = PresentSurface<GPUAPIDefault>;
	using FontRasterizerDefault = FontRasterizerCPU<GPUAPIDefault>;
	using VectorRasterizerDefault = VectorRasterizerCPU<GPUAPIDefault>;
	
	using FontRendererDefault = FontRenderer<PresentSurfaceDefault, FontRasterizerDefault>;
	using BoxRendererDefault = BoxRenderer<PresentSurfaceDefault>;
	using TexturedQuadRendererDefault = TexturedQuadRenderer<PresentSurfaceDefault>;
	using ArcRendererDefault = CircularArcRenderer<PresentSurfaceDefault>;

	struct RenderersDefault
	{
		using FontRenderer = FontRendererDefault;
		using BoxRenderer = BoxRendererDefault;
		using TexturedQuadRenderer = TexturedQuadRendererDefault;
		using ArcRenderer = ArcRendererDefault;
		using VectorRasterizer = VectorRasterizerDefault;
	};

	using RendererDefault = Renderer<RenderersDefault>;
	using RuntimeDefault = Runtime<GPUAPIDefault, RendererDefault>;
}