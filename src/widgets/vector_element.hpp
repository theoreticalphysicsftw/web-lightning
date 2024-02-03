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


#pragma once

#include <common/error.hpp>

#include "rendering/vector_path.hpp"

#include "widget.hpp"
#include "box.hpp"

namespace WL
{
	template <typename TRuntime>
	class VectorElement : public Box<TRuntime>
	{
	public:
		using Base = Box<TRuntime>;
		using Runtime = TRuntime;
		using GPUAPI = typename Runtime::GPUAPI;
		using Image = GPUAPI::Image;
		using BoxDesc = Base::BoxDesc;
		using Path2D = Path2D<F32>;

		VectorElement(Span<const Path2D> paths, F32 width, F32 height, F32 offsetX, F32 offsetY, B copySource = true);
		VectorElement(StrView svg, F32 width, F32 height, F32 offsetX, F32 offsetY, B copySource = true);

		virtual auto Update(const UpdateState& s) -> V override;
	private:
		Map<U32, Image*> rasterizedCache;
		Array<Path2D> paths;
		Span<const Path2D> pathsView;
		Str svg;
		StrView svgView;
	};
}


namespace WL
{
	template<typename TRuntime>
	VectorElement<TRuntime>::VectorElement(Span<const Path2D> paths, F32 width, F32 height, F32 offsetX, F32 offsetY, B copySource) :
		Base(BoxDesc())
	{
		this->desc.width = width;
		this->desc.height = height;
		this->desc.offsetX = offsetX;
		this->desc.offsetY = offsetY;
		this->desc.u0 = 0;
		this->desc.v0 = 0;
		this->desc.u1 = 1;
		this->desc.v1 = 1;
		this->desc.textured = true;
		
		auto pxWidth = Runtime::GPUPresentSurface::UnitsToPixels(width);
		auto pxHeight = Runtime::GPUPresentSurface::UnitsToPixels(height);

		// TODO: Cleanup in destructor.
		auto img = new Image;
		rasterizedCache[pxWidth] = img;
		img->Allocate(EFormat::BGRA8, pxWidth, pxHeight);
		Runtime::Renderer::VectorRasterizer::RasterizeToImage(paths, img);
		this->desc.texture = img;

		if (copySource)
		{
			this->paths.assign( paths.begin(), paths.end());
			pathsView = { this->paths.data(), this->paths.size() };
		}
		else
		{
			pathsView = paths;
		}

	}


	template<typename TRuntime>
	inline VectorElement<TRuntime>::VectorElement(StrView svg, F32 width, F32 height, F32 offsetX, F32 offsetY, B copySource) :
		Base(BoxDesc())
	{
		this->desc.width = width;
		this->desc.height = height;
		this->desc.offsetX = offsetX;
		this->desc.offsetY = offsetY;
		this->desc.u0 = 0;
		this->desc.v0 = 0;
		this->desc.u1 = 1;
		this->desc.v1 = 1;
		this->desc.textured = true;

		auto pxWidth = Runtime::GPUPresentSurface::UnitsToPixels(width);
		auto pxHeight = Runtime::GPUPresentSurface::UnitsToPixels(height);

		// TODO: Cleanup in destructor.
		auto img = new Image;
		rasterizedCache[pxWidth] = img;
		img->Allocate(EFormat::BGRA8, pxWidth, pxHeight);
		Runtime::Renderer::VectorRasterizer::RasterizeToImage(svg, img);
		this->desc.texture = img;

		if (copySource)
		{
			this->svg = svg;
			svgView = { this->svg.data(), this->svg.size() };
		}
		else
		{
			svgView = svg;
		}
	}


	template<typename TRuntime>
	inline auto VectorElement<TRuntime>::Update(const UpdateState& s) -> V
	{
		Base::Update(s);
	}
}