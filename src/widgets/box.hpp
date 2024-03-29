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

#include <common/error.hpp>

#include "widget.hpp"
#include "rendering/quad.hpp"

namespace WL
{
	template <typename TRuntime>
	class Box : public Widget<TRuntime>
	{
	public:
		using Base = Widget<TRuntime>;
		using Runtime = TRuntime;
		using GPUAPI = typename Runtime::GPUAPI;
		using BoxDesc = QuadDesc2D<GPUAPI>;

		Box(const BoxDesc& desc);

		virtual auto AccumulateDrawState() const -> V override;
		virtual auto Update(const UpdateState& s) -> V override;
		virtual auto GetBBox(const Widget<TRuntime>* w = nullptr) const -> BBox override;

		BoxDesc desc;
	};
}


namespace WL
{
	template<typename TRuntime>
	Box<TRuntime>::Box(const BoxDesc& desc)
		: desc(desc)
	{
	}

	template<typename TRuntime>
	inline auto Box<TRuntime>::AccumulateDrawState() const -> V
	{
		auto renderDesc = desc;
		auto ar = Runtime::GPUPresentSurface::GetAspectRatio();

		auto positionedOffsetX = desc.offsetX;
		auto positionedOffsetY = desc.offsetY;

		if (this->relativelyPositioned)
		{
			WL_ASSERT(this->ancestor);
			auto bBox = this->ancestor->GetBBox(this);

			positionedOffsetX += bBox.x0;
			positionedOffsetY += bBox.y0;
			
			if (this->centered)
			{
				positionedOffsetX += (bBox.x1 - bBox.x0 - desc.width) / 2.f;
				positionedOffsetY += (bBox.y1 - bBox.y0 - desc.height) / 2.f;
			}
		}
		else if (this->centered)
		{
			positionedOffsetX = (1.f - desc.width) / 2.f;
			positionedOffsetY = (1.f / ar - desc.height) / 2.f;
		}

		renderDesc.width = desc.width * 2.f;
		renderDesc.height = desc.height * ar * 2.f;
		renderDesc.offsetY = -positionedOffsetY * ar * 2.f  + 1.f - desc.height * ar;
		renderDesc.offsetX = -1.f + positionedOffsetX * 2.f + renderDesc.width / 2.f;

		Runtime::Renderer::BoxRenderer::AccumulateBox(renderDesc);

	}


	template<typename TRuntime>
	inline auto Box<TRuntime>::Update(const UpdateState& s) -> V
	{
		Base::Update(s);
	}

	template<typename TRuntime>
	inline auto Box<TRuntime>::GetBBox(const Widget<TRuntime>* w) const -> BBox
	{
		return BBox(desc.offsetX, desc.offsetY, desc.offsetX + desc.width, desc.offsetY + desc.height);
	}
}