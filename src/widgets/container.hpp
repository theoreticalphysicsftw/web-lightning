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

#include "widget.hpp"

namespace WL
{

	template <typename TRuntime>
	class Container : public Widget<TRuntime>
	{
	public:
		using Base = Widget<TRuntime>;
		using Runtime = TRuntime;
		using GPUAPI = typename Runtime::GPUAPI;

		Container(F32 maxWidth = 0.f, F32 maxHeight = 0.f, F32 offsetX = 0.f, F32 offsetY = 0.f);

		virtual auto AccumulateDrawState() const->V override;
		virtual auto Update(const UpdateState& s) -> V override;
		virtual auto GetBBox(const Widget<TRuntime>* w = nullptr) const->BBox override;

		B scrollableX = false;
		B scrollableY = false;

	public:
		F32 maxWidth;
		F32 maxHeight;
		F32 offsetX;
		F32 offsetY;
		F32 currentWidth = 0;
		F32 currentHeight = 0;
		F32 scrollX = 0;
		F32 scrollY = 0;

		Array<Widget<TRuntime>*> widgets;
	};

}

namespace WL
{
	template<typename TRuntime>
	inline Container<TRuntime>::Container(F32 maxWidth, F32 maxHeight, F32 offsetX, F32 offsetY)
		: maxWidth(maxWidht), maxHeight(maxHeight), offsetX(offsetX), offsetY(offsetY)
	{
	}

	template<typename TRuntime>
	inline auto Container<TRuntime>::AccumulateDrawState() const -> V override
	{
		for (auto widgetPtr : widgets)
		{
			widgetPtr->AccumulateDrawState();
		}
	}

	template<typename TRuntime>
	inline auto Container<TRuntime>::Update(const UpdateState& s) -> V override
	{
		Base::Update(s);
		for (auto widgetPtr : widgets)
		{
			widgetPtr->Update(s);
		}
	}

	template<typename TRuntime>
	inline auto Container<TRuntime>::GetBBox(const Widget<TRuntime>* w) const -> BBox override
	{
		auto startX = scrollableX? offsetX + scrollX : offsetX;
		auto startY = scrollableY? offsetY + scrollY : offsetY;

		auto endX = scrollableX? offsetX + Max(maxWidth, currentWidth) : offsetX + currentWidth;
		auto endY = scrollableY ? offsetY + Max(maxHeight, currentHeight) : offsetY + currentHeight;

		return BBox(startX, startY, endX, endY);
	}
}
