// MIT License
// 
// Copyright (c) 2023 Mihail Mladenov
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
#include <algebra/algebra.hpp>
#include <rendering/color.hpp>

namespace WL
{
	struct FontRendererOptions
	{
		F32 height = 0.05f;
		Color4 color = Color4(1.f, 0.f, 0.f, 0.f);
		F32 offsetX = 0;
		F32 offestY = 0;
	};

	template <typename TPresentSurface, typename TRasterizer>
	class FontRenderer
	{
	public:
		using Rasterizer = TRasterizer;
		using PresentSurface = TPresentSurface;
		using GPUAPI = PresentSurface::GPUAPI;
		static auto Init() -> B;
		static auto AccumulateDrawCommands(const Array<I32> codepoints, const FontRendererOptions& options) -> V;
		static auto CommitDrawCommands() -> V;
		static auto Clear() -> V;

	private:
		static auto AllocateBuffers() -> B;
		static auto ReallocateBuffers(U32 newCapacity) -> B;


		inline static U32 initialGlyphInstancesCapacity = 1024;
		inline static U32 instances = 0;

		inline static GPUAPI::Pso pso;

		inline static GPUAPI::Buffer linearTransformsBuffer;
		inline static GPUAPI::Buffer translationsBuffer;
		inline static GPUAPI::Buffer colorsBuffer;
		inline static GPUAPI::Buffer glyphIndicesBuffer;

		inline static Array<Mat2x2> linearTransformsBufferCPU;
		inline static Array<Vec2> translationsBufferCPU;
		inline static Array<ColorU32> colorsBufferCPU;
		inline static Array<U32> glyphIndicesBufferCPU;
	};



}

namespace WL
{
	template <typename TPresentSurface, typename TRasterizer>
	auto FontRenderer<TPresentSurface, TRasterizer>::Init() -> B
	{
		if (!Rasterizer::Init())
		{
			return false;
		}


	}


	template <typename TPresentSurface, typename TRasterizer>
	auto FontRenderer<TPresentSurface, TRasterizer>::AccumulateDrawCommands(const Array<I32> codepoints, const FontRendererOptions& options) -> V
	{
		return V();
	}


	template <typename TPresentSurface, typename TRasterizer>
	auto FontRenderer<TPresentSurface, TRasterizer>::CommitDrawCommands() -> V
	{
		return V();
	}


	template <typename TPresentSurface, typename TRasterizer>
	inline auto FontRenderer<TPresentSurface, TRasterizer>::Clear() -> V
	{
		return V();
	}

	template <typename TPresentSurface, typename TRasterizer>
	inline auto FontRenderer<TPresentSurface, TRasterizer>::AllocateBuffers() -> B
	{
		B status =
			linearTransformsBuffer.Allocate(initialGlyphInstancesCapacity, false) &&
			translationsBuffer.Allocate(initialGlyphInstancesCapacity, false) &&
			colorsBuffer.Allocate(initialGlyphInstancesCapacity, false) &&
			glyphIndicesBuffer.Allocate(initialGlyphInstancesCapacity, false);
		return status;
	}


	template <typename TPresentSurface, typename TRasterizer>
	inline auto FontRenderer<TPresentSurface, TRasterizer>::ReallocateBuffers(U32 newCapacity) -> B
	{
		B status =
			linearTransformsBuffer.Reallocate(newCapacity, false) &&
			translationsBuffer.Reallocate(newCapacity, false) &&
			colorsBuffer.Reallocate(newCapacity, false) &&
			glyphIndicesBuffer.Reallocate(newCapacity, false);
		return status;
	}
}