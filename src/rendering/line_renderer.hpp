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

#include <algebra/bezier.hpp>
#include <algebra/vector.hpp>

#include "color.hpp"


namespace WL
{
	template <typename TPresentSurface>
	class LineRenderer
	{
	public:
		using PresentSurface = TPresentSurface;
		using GPUAPI = PresentSurface::GPUAPI;
		using Buffer = GPUAPI::Buffer;
		using Image = GPUAPI::Image;
		using Pso = GPUAPI::Pso;
		using Line = Line<F32, 2>;

		static auto Init() -> B;
		static auto AccumulateLine(const Line& curve, ColorU32 color, F32 width, F32 feather = 1.0f, F32 fBegin = feather, F32 fEnd = feather) -> V;
		static auto CommitDrawCommands() -> V;
		static auto Clear() -> V;

		static constexpr U32 initialLineInstancesCapacity = 1024;

	private:
		static auto AllocateBuffers() -> B;
		static auto ReallocateBuffers(U32 newCapacity) -> B;

		inline static U32 instanceCapacity = initialLineInstancesCapacity;
		inline static U32 instances = 0;

		inline static Pso pso;
		inline static Buffer pointsBuffer;
		inline static Buffer colorsBuffer;
		inline static Buffer widthAndFeathersBuffer;

		inline static Array<Vec4> pointsBufferCPU;
		inline static Array<ColorU32> colorsBufferCPU;
		inline static Array<Vec4> widthAndFeathersBufferCPU;
	};
}


namespace WL
{
	template<typename TPresentSurface>
	inline auto LineRenderer<TPresentSurface>::Init() -> B
	{
		if (!AllocateBuffers())
		{
			return false;
		}

		pso.AddShader(LineVert.data(), LineVert.size(), EShaderType::Vertex);
		pso.AddShader(LineFrag.data(), LineFrag.size(), EShaderType::Fragment);

		pso.AddVBLayout({ .binding = 0, .type = EType::Float, .components = 4 });
		pso.AddVBLayout({ .binding = 1, .type = EType::Uint, .components = 1 });
		pso.AddVBLayout({ .binding = 2, .type = EType::Float, .components = 4 });
		pso.AddSimpleConstant(0, EType::Float, "uScreenDims");

		return pso.Compile();
	}


	template<typename TPresentSurface>
	inline auto LineRenderer<TPresentSurface>::AccumulateLine(const Line& line, ColorU32 color, F32 width, F32 feather, F32 fBegin, F32 fEnd) -> V
	{
		instances++;
		if (instances > instanceCapacity)
		{
			ReallocateBuffers(instanceCapacity * 2);
			instanceCapacity *= 2;
		}

		pointsBufferCPU.emplace_back(line.p0[0], line.p0[1], line.p1[0], line.p1[1]);
		colorsBufferCPU.emplace_back(color);
		widthAndFeathersBufferCPU.emplace_back(width, feather, fBegin, fEnd);
	}


	template<typename TPresentSurface>
	inline auto LineRenderer<TPresentSurface>::CommitDrawCommands() -> V
	{
		if (!instances)
		{
			return;
		}

		pointsBuffer.Update(pointsBufferCPU);
		colorsBuffer.Update(colorsBufferCPU);
		widthAndFeathersBuffer.Update(widthAndFeathersBufferCPU);

		pso.Use();
		pso.BindVB(0, pointsBuffer, true);
		pso.BindVB(1, colorsBuffer, true);
		pso.BindVB(2, widthAndFeathersBuffer, true);

		pso.UpdateConstant(0, PresentSurface::GetDimensions());

		pso.DrawInstanced(0, 6, instances);

		Clear();
	}


	template<typename TPresentSurface>
	inline auto LineRenderer<TPresentSurface>::AllocateBuffers() -> B
	{
		B status =
			pointsBuffer.Allocate(initialLineInstancesCapacity, false) &&
			colorsBuffer.Allocate(initialLineInstancesCapacity, false) &&
			widthAndFeathersBuffer.Allocate(initialLineInstancesCapacity, false);
		return status;
	}


	template<typename TPresentSurface>
	inline auto LineRenderer<TPresentSurface>::ReallocateBuffers(U32 newCapacity) -> B
	{
		B status =
			pointsBuffer.Reallocate(newCapacity, false) &&
			colorsBuffer.Reallocate(newCapacity, false) &&
			widthAndFeathersBuffer.Reallocate(newCapacity, false);
		return status;
	}


	template<typename TPresentSurface>
	inline auto LineRenderer<TPresentSurface>::Clear() -> V
	{
		instances = 0;
		pointsBufferCPU.clear();
		colorsBufferCPU.clear();
		widthAndFeathersBufferCPU.clear();
	}
}