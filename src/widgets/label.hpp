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

#include <common/types.hpp>
#include <common/error.hpp>

#include <rendering/font_renderer.hpp>

#include "widget.hpp"


namespace WL
{
	struct LabelDesc
	{
		Str text;
		ColorU32 color;
		F32 offsetX;
		F32 offsetY;
		F32 fontHeight;
		U16 fontIndex;
	};

	template <typename TRuntime>
	class Label : public Widget<TRuntime>
	{
	public:
		using Runtime = TRuntime;
		using GPUAPI = typename Runtime::GPUAPI;
		using BoxDesc = QuadDesc2D<GPUAPI>;

		Label(const LabelDesc& desc);

		virtual auto AccumulateDrawState() const->V override;
		virtual auto Update(const UpdateState& s) -> V override;
		virtual auto GetBBox(const Widget<TRuntime>* w = nullptr) const->BBox override;

		LabelDesc desc;
	};
}


namespace WL
{
	template<typename TRuntime>
	Label<TRuntime>::Label(const LabelDesc& desc)
		: desc(desc)
	{
	}

	template<typename TRuntime>
	inline auto Label<TRuntime>::AccumulateDrawState() const -> V
	{
		FontRendererOptions renderDesc;
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
			// TODO:
		}

		renderDesc.offsetY = -positionedOffsetY * ar * 2.f + 1.f - desc.height * ar;
		renderDesc.offsetX = -1.f + positionedOffsetX * 2.f + renderDesc.width / 2.f;

		//Runtime::Renderer::FontRenderer::AccumulateDrawState(codepoints, renderDesc);
	}


	template<typename TRuntime>
	inline auto Label<TRuntime>::Update(const UpdateState& s) -> V
	{

	}

	template<typename TRuntime>
	inline auto Label<TRuntime>::GetBBox(const Widget<TRuntime>* w) const -> BBox
	{
		// TODO:
		return BBox();
	}
}