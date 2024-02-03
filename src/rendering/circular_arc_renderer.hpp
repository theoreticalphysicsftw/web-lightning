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
#include <common/utilities.hpp>

#include <rendering/color.hpp>
#include <shaders/common.hpp>

#include <algebra/algebra.hpp>

#include <gpu_api/shader.hpp>
#include <gpu_api/pso.hpp>


namespace WL
{
	template <typename TPresentSurface>
	class CircularArcRenderer
	{
	public:
		using PresentSurface = TPresentSurface;
		using GPUAPI = PresentSurface::GPUAPI;
		using Buffer = GPUAPI::Buffer;
		using Pso = GPUAPI::Pso;

		static auto Init() -> B;
		static auto AccumulateArc(Vec2 center, F32 radius, F32 width, F32 arcStart, F32 arcEnd, ColorU32 color) -> V;
		static auto CommitDrawCommands() -> V;
		static auto Clear() -> V;

		static constexpr U32 initialArcInstancesCapacity = 1024;

	private:
		static auto AllocateBuffers() -> B;
		static auto ReallocateBuffers(U32 newCapacity) -> B;

		inline static U32 instanceCapacity = initialArcInstancesCapacity;
		inline static U32 instances = 0;

		inline static Pso pso;
		inline static Buffer centersBuffer;
		// Vec4 -> (radius, width, arcStart, arcStop)
		inline static Buffer paramsBuffer;
		inline static Buffer colorsBuffer;

		inline static Array<Vec2> centersBufferCPU;
		inline static Array<Vec4> paramsBufferCPU;
		inline static Array<ColorU32> colorsBufferCPU;
	};
}


namespace WL
{
	template<typename TPresentSurface>
	inline auto CircularArcRenderer<TPresentSurface>::Init() -> B
	{
		if (!AllocateBuffers())
		{
			return false;
		}

		pso.AddShader(CircularArcVert.data(), CircularArcVert.size(), EShaderType::Vertex);
		pso.AddShader(CircularArcFrag.data(), CircularArcFrag.size(), EShaderType::Fragment);

		pso.AddVBLayout({ .binding = 0, .type = EType::Float, .components = 2 });
		pso.AddVBLayout({ .binding = 1, .type = EType::Float, .components = 4 });
		pso.AddVBLayout({ .binding = 2, .type = EType::Uint, .components = 1 });
		pso.AddSimpleConstant(0, EType::Float, "uScreenDims");

		return pso.Compile();
	}


	template<typename TPresentSurface>
	inline auto CircularArcRenderer<TPresentSurface>::AccumulateArc(Vec2 center, F32 radius, F32 width, F32 arcStart, F32 arcEnd, ColorU32 color) -> V
	{

		instances++;
		if (instances > instanceCapacity)
		{
			ReallocateBuffers(instanceCapacity * 2);
			instanceCapacity *= 2;
		}

		centersBufferCPU.emplace_back(center);
		colorsBufferCPU.emplace_back(color);
		paramsBufferCPU.emplace_back(radius, width, arcStart, arcEnd);
	}


	template<typename TPresentSurface>
	inline auto CircularArcRenderer<TPresentSurface>::CommitDrawCommands() -> V
	{
		if (!instances)
		{
			return;
		}

		pso.Use();
		colorsBuffer.Update(colorsBufferCPU);
		paramsBuffer.Update(paramsBufferCPU);
		centersBuffer.Update(centersBufferCPU);

		pso.BindVB(0, centersBuffer, true);
		pso.BindVB(1, paramsBuffer, true);
		pso.BindVB(2, colorsBuffer, true);

		pso.UpdateConstant(0, PresentSurface::GetDimensions());

		pso.DrawInstanced(0, 3, instances);

		Clear();
	}


	template<typename TPresentSurface>
	inline auto CircularArcRenderer<TPresentSurface>::AllocateBuffers() -> B
	{
		B status =
			centersBuffer.Allocate(initialArcInstancesCapacity, false) &&
			paramsBuffer.Allocate(initialArcInstancesCapacity, false) &&
			colorsBuffer.Allocate(initialArcInstancesCapacity, false);
		return status;
	}


	template<typename TPresentSurface>
	inline auto CircularArcRenderer<TPresentSurface>::ReallocateBuffers(U32 newCapacity) -> B
	{
		B status =
			centersBuffer.Reallocate(newCapacity, false) &&
			paramsBuffer.Reallocate(newCapacity, false) &&
			colorsBuffer.Reallocate(newCapacity, false);
		return status;
	}


	template<typename TPresentSurface>
	inline auto CircularArcRenderer<TPresentSurface>::Clear() -> V
	{
		instances = 0;
		colorsBufferCPU.clear();
		paramsBufferCPU.clear();
		centersBufferCPU.clear();
	}
}