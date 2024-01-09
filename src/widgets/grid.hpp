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
	class Grid : public Widget<TRuntime>
	{
	public:
		using Runtime = TRuntime;
		using GPUAPI = typename Runtime::GPUAPI;

		Grid(F32 offsetX = 0, F32 offsetY = 0, F32 cellSize = 0, U32 cellsX = 0, U32 cellsY = 0);

		virtual auto AccumulateDrawCommands() const -> V override;
		virtual auto Update(const UpdateState& s) -> V override;
		virtual auto GetBBox(const Widget<TRuntime>* w = nullptr) const->BBox override;

		auto PlaceInCell(Widget<TRuntime>* w, U32 linearIdx) -> V;
		auto PlaceInCell(Widget<TRuntime>* w, U32 cellX, U32 cellY);
		
		B centeredCells = true;

	private:
		Map<Widget<TRuntime>*, U32> slots;
		F32 offsetX;
		F32 offsetY;
		F32 cellSize;
		U32 cellsX;
		U32 cellsY;
	};
}


namespace WL
{
	template<typename TRuntime>
	inline Grid<TRuntime>::Grid(F32 offsetX, F32 offsetY, F32 cellSize, U32 cellsX, U32 cellsY) :
		offsetX(offsetX), offsetY(offsetY), cellSize(cellSize), cellsX(cellsX), cellsY(cellsY)
	{
	}

	template<typename TRuntime>
	inline auto Grid<TRuntime>::AccumulateDrawCommands() const -> V
	{
		for (auto& widgetPtr : slots)
		{
			widgetPtr.first->AccumulateDrawCommands();
		}
	}

	template<typename TRuntime>
	inline auto Grid<TRuntime>::Update(const UpdateState& s) -> V
	{
		for (auto& widgetPtr : slots)
		{
			widgetPtr.first->Update(s);
		}
	}

	template<typename TRuntime>
	inline auto Grid<TRuntime>::GetBBox(const Widget<TRuntime>* w) const -> BBox
	{
		if (w)
		{
			Widget<TRuntime>* nonConstW = const_cast<Widget<TRuntime>*>(w);
			auto linearSlot = slots.find(nonConstW)->second;
			auto slotY = linearSlot / cellsX;
			auto slotX = linearSlot % cellsX;
			
			return BBox(offsetX + slotX * cellSize, offsetY + slotY * cellSize, offsetX + (slotX + 1) * cellSize, offsetY + (slotY + 1) * cellSize);
		}

		return BBox(offsetX, offsetY, offsetX + cellsX * cellSize, offsetY + cellsY * cellSize);
	}

	template<typename TRuntime>
	inline auto Grid<TRuntime>::PlaceInCell(Widget<TRuntime>* w, U32 linearIdx) -> V
	{
		w->ancestor = this;
		w->relativelyPositioned = true;
		slots[w] = linearIdx;

		if (centeredCells)
		{
			w->centered = true;
		}
	}

	template<typename TRuntime>
	inline auto Grid<TRuntime>::PlaceInCell(Widget<TRuntime>* w, U32 cellX, U32 cellY)
	{
		auto linearSlot = cellY * cellsX + cellX;
		PlaceInCell(w, linearSlot);
	}
}