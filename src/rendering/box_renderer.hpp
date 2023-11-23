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
#include <common/utilities.hpp>
#include <rendering/color.hpp>
#include <algebra/algebra.hpp>

#include <gpu_api/shader.hpp>
#include <gpu_api/pso.hpp>
#include <shaders/common.hpp>

#include "quad.hpp"

namespace WL
{
	template <typename TPresentSurface>
	class BoxRenderer
	{
	public:
		using PresentSurface = TPresentSurface;
		using GPUAPI = PresentSurface::GPUAPI;
		static auto Init() -> B;
		static auto AccumulateBox(ColorU32 c, F32 h, F32 w, F32 x, F32 y, F32 r) -> V;
		static auto CommitDrawCommands() -> V;

		static constexpr U32 initialBoxInstancesCapacity = 1024;

	private:
		static auto AllocateBuffers() -> B;
		static auto ReallocateBuffers(U32 newCapacity) -> B;
		static auto Clear() -> V;

		inline static U32 instanceCapacity = initialBoxInstancesCapacity;
		inline static U32 instances = 0;

		inline static GPUAPI::Pso pso;
		inline static GPUAPI::Buffer positionsBuffer;
		inline static GPUAPI::Buffer linearTransformsBuffer;
		inline static GPUAPI::Buffer translationsBuffer;
		inline static GPUAPI::Buffer colorsBuffer;
		inline static GPUAPI::Buffer radiusesBuffer;

		inline static Array<Mat2x2> linearTransformsBufferCPU;
		inline static Array<Vec2> translationsBufferCPU;
		inline static Array<ColorU32> colorsBufferCPU;
		inline static Array<F32> radiusesBufferCPU;
	};
}


namespace WL
{
	template<typename TPresentSurface>
	inline auto BoxRenderer<TPresentSurface>::Init() -> B
	{
		if (!AllocateBuffers())
		{
			return false;
		}
		positionsBuffer.Update((const Byte*)DefaultQuad2D::vertices, sizeof(DefaultQuad2D::vertices));

		pso.AddShader(BoxVert.data(), BoxVert.size(), EShaderType::Vertex);
		pso.AddShader(BoxFrag.data(), BoxFrag.size(), EShaderType::Fragment);
		
		pso.AddVBLayout({ .binding = 0, .type = EType::Float, .components = 2 });
		pso.AddVBLayout({ .binding = 1, .type = EType::Float, .components = 4 });
		pso.AddVBLayout({ .binding = 2, .type = EType::Float, .components = 2 });
		pso.AddVBLayout({ .binding = 3, .type = EType::Uint, .components = 1 });
		pso.AddVBLayout({ .binding = 4, .type = EType::Float, .components = 1 });
		pso.AddSimpleConstant(0, EType::Float, "uScreenDims");

		return pso.Compile();
	}


	template<typename TPresentSurface>
	inline auto BoxRenderer<TPresentSurface>::AccumulateBox(ColorU32 c, F32 w, F32 h, F32 x, F32 y, F32 r) -> V
	{
		instances++;
		if (instances > instanceCapacity)
		{
			ReallocateBuffers(instanceCapacity * 2);
		}

		colorsBufferCPU.emplace_back(c);
		radiusesBufferCPU.emplace_back(r);
		linearTransformsBufferCPU.emplace_back(w / 2.f, 0.f, 0.f, h / 2.f);
		translationsBufferCPU.emplace_back(x, y);
	}


	template<typename TPresentSurface>
	inline auto BoxRenderer<TPresentSurface>::CommitDrawCommands() -> V
	{
		if (!instances)
		{
			return;
		}

		colorsBuffer.Update(colorsBufferCPU);
		radiusesBuffer.Update(radiusesBufferCPU);
		linearTransformsBuffer.Update(linearTransformsBufferCPU);
		translationsBuffer.Update(translationsBufferCPU);

		pso.BindVB(0, positionsBuffer);
		pso.BindVB(1, linearTransformsBuffer, true);
		pso.BindVB(2, translationsBuffer, true);
		pso.BindVB(3, colorsBuffer, true);
		pso.BindVB(4, radiusesBuffer, true);


		pso.Use();
		pso.UpdateConstant(0, PresentSurface::GetDimensions());

		pso.DrawInstanced(0, DefaultQuad2D::verticesCount, instances);

		Clear();
	}


	template<typename TPresentSurface>
	inline auto BoxRenderer<TPresentSurface>::AllocateBuffers() -> B
	{
		B status =
			positionsBuffer.Allocate(DefaultQuad2D::verticesCount) &&
			linearTransformsBuffer.Allocate(initialBoxInstancesCapacity, false) &&
			translationsBuffer.Allocate(initialBoxInstancesCapacity, false) &&
			colorsBuffer.Allocate(initialBoxInstancesCapacity, false) &&
			radiusesBuffer.Allocate(initialBoxInstancesCapacity, false);
		return status;
	}


	template<typename TPresentSurface>
	inline auto BoxRenderer<TPresentSurface>::ReallocateBuffers(U32 newCapacity) -> B
	{
		B status =
			linearTransformsBuffer.Reallocate(newCapacity, false) &&
			translationsBuffer.Reallocate(newCapacity, false) &&
			colorsBuffer.Reallocate(newCapacity, false) &&
			radiusesBuffer.Reallocate(newCapacity, false);
		return status;
	}


	template<typename TPresentSurface>
	inline auto BoxRenderer<TPresentSurface>::Clear() -> V
	{
		instances = 0;
		colorsBufferCPU.clear();
		radiusesBufferCPU.clear();
		linearTransformsBufferCPU.clear();
		translationsBufferCPU.clear();
	}
}