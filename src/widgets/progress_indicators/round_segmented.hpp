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

#include <common/error.hpp>
#include <common/types.hpp>

#include <algebra/algebra.hpp>

#include <rendering/color.hpp>

#include <widgets/widget.hpp>

namespace WL::ProgressIndicator
{
	struct RoundSegmentedDesc
	{
		Vec2 center = Vec2(0.f, 0.f);
		F32 radius = 0.f;
		F32 width = 0.f;
		F32 gapSize = 0.f;
		U32 segments = 0;
		B timer = true;
		F32 period = 0.f;
		F32 blinksPerPeriod = 0;
		ColorU32 emptyColor = 0;
		ColorU32 filledColor = 0;
	};

	template <typename TRuntime>
	class RoundSegmented : public Widget<TRuntime>
	{
	public:
		using Base = Widget<TRuntime>;
		using Runtime = TRuntime;
		using Animation = Base::Animation;
		using GPUAPI = typename Runtime::GPUAPI;
		using Desc = RoundSegmentedDesc;

		RoundSegmented(const RoundSegmentedDesc& desc);

		virtual auto AccumulateDrawState() const->V override;
		virtual auto Update(const UpdateState& s) -> V override;
		virtual auto GetBBox(const Widget<TRuntime>* w = nullptr) const->BBox override;

		RoundSegmentedDesc desc;
	private:
		U32 activeSegment = 0;
		Color4 activeSegmentColor;
	};
}


namespace WL::ProgressIndicator
{
	template<typename TRuntime>
	RoundSegmented<TRuntime>::RoundSegmented(const RoundSegmentedDesc& desc)
		: desc(desc)
	{
		if (desc.timer)
		{
			typename Animation::TransformsArray transforms;
			transforms.emplace_back(&activeSegment, 0u, desc.segments);
			transforms.emplace_back(&activeSegmentColor, desc.emptyColor.operator Color4(), desc.filledColor.operator Color4());
			Animation segmentAnimation
			(
				Forward<typename Animation::TransformsArray>(transforms),
				[&desc] (Animation::TransformsArray& ta, F32 t, F32 dt, F32 duration)
				{
					auto activeSegmentPeriod = desc.period / desc.segments;
					auto newActiveSegment = DiscretePeriodicIncrement(desc.period / desc.segments, 0u, desc.segments, t);
					auto activeSegmentT = Frac(t / activeSegmentPeriod) * activeSegmentPeriod;
					auto newColor = Oscillate(activeSegmentPeriod / (desc.blinksPerPeriod + 0.5f), desc.filledColor.operator Color4(), desc.emptyColor.operator Color4(), activeSegmentT);
					ta[0].SetProperty(newActiveSegment);
					ta[1].SetProperty(newColor);
				}
			);
			this->AddParallelAnimation(Forward<Animation>(segmentAnimation));
		}
	}

	template<typename TRuntime>
	inline auto RoundSegmented<TRuntime>::AccumulateDrawState() const -> V
	{
		Desc renderDesc;
		auto ar = Runtime::GPUPresentSurface::GetAspectRatio();

		auto centerX = desc.center[0];
		auto centerY = desc.center[1];

		if (this->relativelyPositioned)
		{
			WL_ASSERT(this->ancestor);
			auto bBox = this->ancestor->GetBBox(this);

			centerX += bBox.x0;
			centerY += bBox.y0;

			if (this->centered)
			{
				centerX += (bBox.x1 - bBox.x0) / 2.f - desc.radius;
				centerY += (bBox.y1 - bBox.y0) / 2.f - desc.radius;
			}
		}
		else if (this->centered)
		{
			centerX = 0.5f;
			centerY = 0.5f / ar;
		}

		auto renderWidth = desc.width * 2.f;
		auto renderRadius = desc.radius * 2.f;
		auto renderOffsetY = 1.f - centerY * ar * 2.f;
		auto renderOffsetX = centerX * 2.f - 1.f;
		auto renderCenter = Vec2(renderOffsetX, renderOffsetY);

		F32 initAngle = 0.f;
		F32 gapAngle = ArcSin(desc.gapSize / (desc.radius - desc.width));
		F32 deltaAngle = 2.f * Constants<F32>::CPi / desc.segments - gapAngle;
		for (auto i = 0u; i < desc.segments; ++i)
		{
			auto color = (i < activeSegment) ? desc.emptyColor : (i == activeSegment)? ColorU32(activeSegmentColor) : desc.filledColor;
			Runtime::Renderer::ArcRenderer::AccumulateArc(renderCenter, renderRadius, renderWidth, initAngle, initAngle + deltaAngle, color);
			initAngle += deltaAngle + gapAngle;
		}

		
	}


	template<typename TRuntime>
	inline auto RoundSegmented<TRuntime>::Update(const UpdateState& s) -> V
	{
		Base::Update(s);
	}

	template<typename TRuntime>
	inline auto RoundSegmented<TRuntime>::GetBBox(const Widget<TRuntime>* w) const -> BBox
	{
		return BBox(desc.center[0] - desc.radius, desc.center[1] - desc.radius, desc.center[0] + desc.radius, desc.center[1] + desc.radius);
	}
}