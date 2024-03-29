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

#include <common/random.hpp>
#include <widgets/widget.hpp>
#include <widgets/vector_paths.hpp>
#include <animation/path_traversal.hpp>

namespace WL::Particles
{
	template <typename TRuntime>
	class CurveBound : public VectorPaths<TRuntime>
	{
	public:
		using Runtime = TRuntime;
		using Base = VectorPaths<TRuntime>;
		using Path2D = Path2D<F32>;
		using Line = Line<F32, 2>;
		using Cubic = CubicBezier<F32, 2>;
		using Quadratic = QuadraticBezier<F32, 2>;
		using Traversal = PathTraversal<F32>;
		using Param = Traversal::ParamValue;

		virtual auto AccumulateDrawState() const -> V override;
		virtual auto Update(const UpdateState& s) -> V override;
		virtual auto GetBBox(const Widget<TRuntime>* w = nullptr) const -> BBox override;

		CurveBound(const Base& inPaths, Span<const Vec2> sizes = {}, Span<const F32> speeds = {}, Span<const F32> initialDelays = {}, B alternatingUpdate = false);

		B alternatingUpdate;

	protected:
		Span<const Path2D> inPaths;
		Array<Traversal> traversals;
		Array<F32> widths;
		Array<Span<const Param>> params;
		VectorPaths<TRuntime> highlight;

		U32 currentPath;
	};
}


namespace WL::Particles
{
	template<typename TRuntime>
	inline CurveBound<TRuntime>::CurveBound(const Base& inPaths, Span<const Vec2> sizes, Span<const F32> speeds, Span<const F32> initialDelays, B alternatingUpdate) :
		Base({}, inPaths.width, inPaths.height, inPaths.offsetX, inPaths.offsetY),
		highlight({}, inPaths.width, inPaths.height, inPaths.offsetX, inPaths.offsetY),
		alternatingUpdate(alternatingUpdate)
	{
		this->inPaths = inPaths.GetPaths();
		params.resize(this->inPaths.size());
		for (auto i = 0u; i < this->inPaths.size(); ++i)
		{
			auto width = (sizes.size() > i) ? sizes[i][0] : inPaths.paths[i].outlineWidth;
			auto speed = (speeds.size() > i) ? speeds[i] : 0.0000005f;
			auto length = (sizes.size() > i) ? sizes[i][1] : 0.25f;
			auto delay = (initialDelays.size() > i) ? initialDelays[i] : 0.f;
			widths.emplace_back(width);
			traversals.emplace_back(inPaths.paths[i], length, speed, delay);
		}

		if (alternatingUpdate)
		{
			currentPath = GetUniformU32(0, params.size() - 1);
		}
	}


	template<typename TRuntime>
	inline auto CurveBound<TRuntime>::AccumulateDrawState() const -> V
	{
		Base::AccumulateDrawState();
		highlight.AccumulateDrawState();
	}


	template<typename TRuntime>
	inline auto CurveBound<TRuntime>::Update(const UpdateState& s) -> V
	{
		Base::Update(s);
		U32 startPath = (alternatingUpdate) ? currentPath : 0u;
		U32 updateCount = (alternatingUpdate) ? 1u : params.size();
		this->paths.resize(params.size());
		highlight.paths.resize(params.size());
		for (auto i = startPath; i < startPath + updateCount; ++i)
		{
			params[i] = traversals[i].Update(s.dt);
			this->paths[i].primitives.clear();
			highlight.paths[i].primitives.clear();
			if (!params[i].empty() && params[i][0].curveIdx != CInvalidIdx)
			{
				this->paths[i].outlineWidth = widths[i] / 8.f;
				this->paths[i].outlined = true;
				this->paths[i].outlineColor = 0x4affffff;
				this->paths[i].outlineFeather = inPaths[i].outlineFeather * 1.5f;
				this->paths[i].outlineFeatherBegin = -inPaths[i].outlineFeather * 0.9;

				highlight.paths[i].outlineWidth = widths[i] / 16.f;
				highlight.paths[i].outlined = true;
				highlight.paths[i].outlineColor = 0xffffffff;
				highlight.paths[i].outlineFeather = inPaths[i].outlineFeather / 2.0f;
				highlight.paths[i].outlineFeatherBegin = -inPaths[i].outlineFeather * 0.7;

				for (auto j = 0; j < params[i].size(); ++j)
				{
					auto& primVar = inPaths[i].primitives[params[i][j].curveIdx];
					if (HoldsAlternative<Quadratic>(primVar))
					{
						auto& prim = Get<Quadratic>(primVar);
						auto renderPrim = GetSlice(prim, params[i][j].t0, params[i][j].t1);
						this->paths[i].primitives.emplace_back(renderPrim);
						highlight.paths[i].primitives.emplace_back(renderPrim);
					}
					if (HoldsAlternative<Line>(primVar))
					{
						auto& prim = Get<Line>(primVar);
						auto renderPrim = GetSlice(prim, params[i][j].t0, params[i][j].t1);
						this->paths[i].primitives.emplace_back(renderPrim);
						highlight.paths[i].primitives.emplace_back(renderPrim);
					}
				}
			}
			else if (!params[i].empty())
			{
				auto oldPath = currentPath;
				while (params.size() != 1 && oldPath == currentPath)
				{
					currentPath = GetUniformU32(0u, params.size() - 1);
				}
				break;
			}
		}
	}


	template<typename TRuntime>
	inline auto CurveBound<TRuntime>::GetBBox(const Widget<TRuntime>* w) const -> BBox
	{
		return Base::GetBBox();
	}
}