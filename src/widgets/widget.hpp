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

#include <common/types.hpp>
#include <animation/simple.hpp>
#include <presenting/present_surface.hpp>

namespace WL
{
	struct BBox
	{
		F32 x0;
		F32 y0;
		F32 x1;
		F32 y1;

		inline BBox(F32 x0 = 0, F32 y0 = 0, F32 x1 = 0, F32 y1 = 0);

		inline auto Contains(Vec2 p) -> B;
	};

	template <typename TRuntime>
	class Widget
	{
	public:
		using UpdateFunction = Function <V(const UpdateState&)>;
		using Animation = WL::Animation::Simple<TRuntime>;

		virtual auto AccumulateDrawState() const -> V = 0;
		virtual auto Update(const UpdateState&) -> V;
		virtual auto GetBBox(const Widget* w = nullptr) const -> BBox = 0;

		auto AddSerialAnimation(Animation&& a) -> V;
		auto AddParallelAnimation(Animation&& a) -> V;
		
		auto MoveToLayer(U32 layer) -> V;
		
		B isVisible = true;
		B occopiesSpace = true;
		B relativelyPositioned = false;
		B centered = false;

		UpdateFunction updateOnClick;
		UpdateFunction updateOnPress;
		UpdateFunction updateOnHover;
		UpdateFunction updateOnLeave;

		virtual ~Widget();

		Widget* ancestor = nullptr;
	protected:
		U32 layer = 1;

		Deque<Animation> serialAnimationChain;
		Array<Animation> parallelAnimationChain;
		friend TRuntime;
	};
}


namespace WL
{
	inline BBox::BBox(F32 x0, F32 y0, F32 x1, F32 y1) :
		x0(x0), y0(y0), x1(x1), y1(y1)
	{
	}


	inline auto BBox::Contains(Vec2 p) -> B
	{
		return p[0] >= x0 && p[0] <= x1 && p[1] >= y0 && p[1] <= y1;
	}

	template <typename TRuntime>
	Widget<TRuntime>::~Widget()
	{
		TRuntime::Deregister(this);
	}

	template<typename TRuntime>
	inline auto Widget<TRuntime>::Update(const UpdateState& s) -> V
	{
		auto dtS = UsToS(s.dt);
		I32 doneIndex = -1;
		for (auto i = 0u; i < serialAnimationChain.size(); ++i)
		{
			auto& animation = serialAnimationChain[i];
			if (animation.Done())
			{
				doneIndex = i;
			}
			else
			{
				animation.Execute(dtS);
				
				if (!animation.Done())
				{
					break;
				}
				else
				{
					doneIndex = i;
				}
			}
		}

		while (doneIndex >= 0)
		{
			serialAnimationChain.pop_front();
			doneIndex--;
		}

		for (auto& animation : parallelAnimationChain)
		{
			animation.Execute(dtS);
		}
	}

	template<typename TRuntime>
	inline auto Widget<TRuntime>::AddSerialAnimation(Animation&& a) -> V
	{
		serialAnimationChain.emplace_back(Forward<Animation>(a));
	}

	template<typename TRuntime>
	inline auto Widget<TRuntime>::AddParallelAnimation(Animation&& a) -> V
	{
		parallelAnimationChain.emplace_back(Forward<Animation>(a));
	}

	template<typename TRuntime>
	auto WL::Widget<TRuntime>::MoveToLayer(U32 newLayer) -> V
	{
		TRuntime::MoveToLayer(this, newLayer);
	}
}