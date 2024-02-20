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

#include "rendering/vector_path.hpp"

#include "widget.hpp"


namespace WL
{
	template <typename TRuntime>
	class VectorPaths : public Widget<TRuntime>
	{
	public:
		using Base = Widget<TRuntime>;
		using Runtime = TRuntime;
		using GPUAPI = typename Runtime::GPUAPI;
		using Image = GPUAPI::Image;
		using Path2D = Path2D<F32>;
		using Line = Path2D::Line;
		using Cubic = Path2D::CubicBezier;
		using Quadratic = Path2D::QuadraticBezier;

		VectorPaths(Span<const Path2D> paths, F32 width, F32 height, F32 offsetX, F32 offsetY);

		virtual auto AccumulateDrawState() const -> V override;
		virtual auto GetBBox(const Widget<TRuntime>* w = nullptr) const -> BBox override;
		virtual auto Update(const UpdateState& s) -> V override;

		auto UpdatePaths(Span<const Path2D> paths) -> V;
		auto GetPaths() const -> Span<const Path2D>;

		Array<Path2D> paths;
		F32 width;
		F32 height;
		F32 offsetX;
		F32 offsetY;
	};
}


namespace WL
{
	template<typename TRuntime>
	VectorPaths<TRuntime>::VectorPaths(Span<const Path2D> paths, F32 width, F32 height, F32 offsetX, F32 offsetY)
		: width(width), height(height), offsetX(offsetX), offsetY(offsetY)
	{
		UpdatePaths(paths);
	}


	template<typename TRuntime>
	inline auto VectorPaths<TRuntime>::UpdatePaths(Span<const Path2D> paths) -> V
	{
		this->paths.clear();
		for (auto& path : paths)
		{
			auto tolerance = 28.f / (TRuntime::PresentSurface::GetDimensions()[0] * width);
			this->paths.emplace_back(path.CloneWithoutPrimitives());
			for (auto& primitive : path.primitives)
			{
				if (HoldsAlternative<Line>(primitive) || HoldsAlternative<Quadratic>(primitive))
				{
					Visit<V>
					(
						[&](const auto& prim) -> V
						{
							auto transformed = TRuntime::PresentSurface::NormalizedImageToWLCoords(prim, width, height, offsetX, offsetY);
							this->paths.back().primitives.emplace_back(transformed);
						},
						primitive
					);
				}
				else
				{
					WL_ASSERT(HoldsAlternative<Cubic>(primitive));
					auto& cubic = Get<Cubic>(primitive);
					auto transformedCubic = TRuntime::PresentSurface::NormalizedImageToWLCoords(cubic, width, height, offsetX, offsetY);
					auto approxQuadratics = ApproximateByQuadratics(transformedCubic, tolerance);
					for (auto& curve : approxQuadratics)
					{
						this->paths.back().primitives.emplace_back(curve);
					}
				}
			}
		}
	}


	template<typename TRuntime>
	inline auto VectorPaths<TRuntime>::GetPaths() const -> Span<const Path2D>
	{
		return Span<const Path2D>(paths.begin(), paths.end());
	}


	template<typename TRuntime>
	inline auto VectorPaths<TRuntime>::AccumulateDrawState() const -> V
	{
		for (auto& path : paths)
		{
			for (auto i = 0u; i < path.primitives.size(); ++i)
			{
				auto& primitive = path.primitives[i];
				auto featherBegin = (i == 0) ? path.outlineFeatherBegin : 0;
				auto featherEnd = (i == path.primitives.size() - 1) ? path.outlineFeatherEnd : 0;

				if (HoldsAlternative<Line>(primitive))
				{
					auto& line = Get<Line>(primitive);
					auto renderLine = Runtime::PresentSurface::WLToRenderCoords(line);

					Runtime::Renderer::LineRenderer::AccumulateLine
					(
						renderLine,
						path.outlineColor,
						path.outlineWidth,
						path.outlineFeather,
						featherBegin,
						featherEnd
					);
				}
				else
				{
					WL_ASSERT(HoldsAlternative<Quadratic>(primitive));
					auto& curve = Get<Quadratic>(primitive);
					auto renderCurve = Runtime::PresentSurface::WLToRenderCoords(curve);

					Runtime::Renderer::BezierRenderer::AccumulateBezier
					(
						renderCurve,
						path.outlineColor,
						path.outlineWidth,
						path.outlineFeather,
						featherBegin,
						featherEnd
					);
				}
			}
		}
	}

	template<typename TRuntime>
	inline auto VectorPaths<TRuntime>::Update(const UpdateState& s) -> V
	{
		Base::Update(s);
	}

	template<typename TRuntime>
	inline auto VectorPaths<TRuntime>::GetBBox(const Widget<TRuntime>* w) const -> BBox
	{
		return BBox(offsetX, offsetY, offsetX + width, offsetY + height);
	}
}