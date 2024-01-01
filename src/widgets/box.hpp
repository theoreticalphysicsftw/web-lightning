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

#include "widget.hpp"
#include "rendering/quad.hpp"

namespace WL
{
	template <typename Runtime>
	class Box : public Widget<Runtime>
	{
	public:
		using GPUAPI = typename Runtime::GPUAPI;
		using BoxDesc = QuadDesc2D<GPUAPI>;

		Box(const BoxDesc& desc);

		virtual auto AccumulateDrawCommands() -> V override;
		virtual auto Update(const UpdateState& s) -> V override;

		BoxDesc desc;
	};
}


namespace WL
{
	template<typename Runtime>
	Box<Runtime>::Box(const BoxDesc& desc)
		: desc(desc)
	{
	}

	template<typename Runtime>
	inline auto Box<Runtime>::AccumulateDrawCommands() -> V
	{
		auto ar = Runtime::GPUPresentSurface::GetAspectRatio();
		auto drawWidth = desc.width * 2.f;
		auto drawHeight = desc.height * ar * 2.f;
		auto drawOffsetY = -desc.offsetY * ar * 2.f  + 1.f - desc.height * ar;
		auto drawOffsetX = -1.f + desc.offsetX * 2.f + drawWidth / 2.f;
		if (!desc.textured)
		{
			Runtime::Renderer::BoxRenderer::AccumulateBox(desc.color, drawWidth, drawHeight, drawOffsetX, drawOffsetY, desc.radius);
		}
	}


	template<typename Runtime>
	inline auto Box<Runtime>::Update(const UpdateState& s) -> V
	{

	}
}