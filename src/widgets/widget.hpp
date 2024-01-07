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

#include "common/types.hpp"

namespace WL
{
	struct UpdateState
	{
		F32 dt;
		F32 mouseX;
		F32 mouseY;
		U32 keyCode;
	};

	struct BBox
	{
		F32 x0;
		F32 y0;
		F32 x1;
		F32 y1;

		inline BBox(F32 x0 = 0, F32 y0 = 0, F32 x1 = 0, F32 y1 = 0);
	};

	template <typename TRuntime>
	class Widget
	{
	public:
		using UpdateFunction = Function <V(const UpdateState&)>;

		virtual auto AccumulateDrawCommands() const -> V = 0;
		virtual auto Update(const UpdateState&) -> V = 0;
		virtual auto GetBBox(const Widget* w = nullptr) const -> BBox = 0;
		
		auto MoveToLayer(U32 layer) -> V;
		
		B isVisible = true;
		B occopiesSpace = true;
		B relativelyPositioned = true;

		UpdateFunction updateOnClick;
		UpdateFunction updateOnPress;
		UpdateFunction updateOnHover;
		UpdateFunction updateOnLeave;

		virtual ~Widget();

		Widget* ancestor = nullptr;
	protected:
		U32 layer = 1;
		friend TRuntime;
	};
}


namespace WL
{
	inline BBox::BBox(F32 x0, F32 y0, F32 x1, F32 y1) :
		x0(x0), y0(y0), x1(x1), y1(y1)
	{
	}

	template <typename TRuntime>
	Widget<TRuntime>::~Widget()
	{
		TRuntime::Deregister(this);
	}

	template<typename TRuntime>
	auto WL::Widget<TRuntime>::MoveToLayer(U32 newLayer) -> V
	{
		TRuntime::MoveToLayer(this, newLayer);
	}
}