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

#include <plutosvg/plutovg/include/plutovg.h>
#include <plutosvg/source/plutosvg.h>

#include "vector_rasterizer.hpp"
#include "vector_path.hpp"

namespace WL
{
	template <typename TGPUAPI>
	class VectorRasterizerCPU : public VectorRasterizer<VectorRasterizerCPU<TGPUAPI>, TGPUAPI>
	{
	public:
		using GPUAPI = TGPUAPI;
		using Image = GPUAPI::Image;
		using Path2D = Path2D<F32>;
		
		static auto RasterizeToImage(Span<const Path2D> paths, Image* out) -> V;
		static auto RasterizeToImage(StrView svg, Image* out) -> V;
		static auto Init() -> B;
	private:
	};
}


namespace WL
{
	template<typename TGPUAPI>
	inline auto VectorRasterizerCPU<TGPUAPI>::RasterizeToImage(Span<const Path2D> paths, Image* out) -> V
	{
		const auto width = out->width;
		const auto height = out->height;
		auto surface = plutovg_surface_create(width, height);
		auto context = plutovg_create(surface);

		for (auto& path : paths)
		{
			plutovg_save(context);

			for (auto& prim : path.primitives)
			{
				if (HoldsAlternative<Path2D::Line>(prim))
				{
					auto& line = Get<Path2D::Line>(prim);
					plutovg_move_to(context, line.p0[0] * width, line.p0[1] * height);
					plutovg_line_to(context, line.p1[0] * width, line.p1[1] * height);
				}

				if (HoldsAlternative<Path2D::QuadraticBezier>(prim))
				{
					auto& qb = Get<Path2D::QuadraticBezier>(prim);
					plutovg_move_to(context, qb.p0[0] * width, qb.p0[1] * height);
					plutovg_quad_to(context, qb.p1[0] * width, qb.p1[1] * height, qb.p2[0] * width, qb.p2[1] * height);
				}

				if (HoldsAlternative<Path2D::CubicBezier>(prim))
				{
					auto& cb = Get<Path2D::CubicBezier>(prim);
					plutovg_move_to(context, cb.p0[0] * width, cb.p0[1] * height);
					plutovg_cubic_to(context, cb.p1[0] * width, cb.p1[1] * height, cb.p2[0] * width, cb.p2[1] * height, cb.p3[0] * width, cb.p3[1] * height);
				}
			}

			if (path.closed)
			{
				plutovg_close_path(context);
			}

			if (path.filled)
			{
				auto color = path.fillColor.operator Color4();
				plutovg_set_source_rgba(context, color[0], color[1], color[2], color[3]);
				plutovg_fill_preserve(context);
			}
			if (path.outlined)
			{
				auto color = path.outlineColor.operator Color4();
				plutovg_set_source_rgba(context, color[0], color[1], color[2], color[3]);
				plutovg_set_line_width(context, path.outlineWidth * width);
				plutovg_stroke_preserve(context);
			}
			plutovg_restore(context);
		}

		auto rasterizedData = plutovg_surface_get_data(surface);
		out->InitData(rasterizedData);
		plutovg_surface_destroy(surface);
		plutovg_destroy(context);
	}


	template<typename TGPUAPI>
	inline auto VectorRasterizerCPU<TGPUAPI>::RasterizeToImage(StrView svg, Image* out) -> V
	{
		auto surface = plutosvg_load_from_memory(svg.data(), svg.size(), nullptr, out->width, out->height, 0);
		WL_ASSERT(surface);
		auto rasterizedData = plutovg_surface_get_data(surface);
		out->InitData(rasterizedData);
		plutovg_surface_destroy(surface);
	}


	template<typename TGPUAPI>
	inline auto VectorRasterizerCPU<TGPUAPI>::Init() -> B
	{
		return true;
	}
}